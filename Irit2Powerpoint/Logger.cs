using System;
using System.IO;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.Diagnostics;

namespace Irit2Powerpoint
{

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void LoggerDelegate(
        int level,
        [MarshalAs(UnmanagedType.LPStr)] string file,
        [MarshalAs(UnmanagedType.LPStr)] string function,
        int line,
        [MarshalAs(UnmanagedType.LPStr)] string message
    );

    enum LogLevel {
        TRACE = 0,
        INFO = 1,
        WARN = 2,
        ERROR = 3,
    }

    class Logger
    {
        private static readonly string LOG_NAME = "I2P_log.txt";
        private static Logger Instance = new Logger();
        private readonly object WriterLog = new object();
        private LogLevel MinLevel = LogLevel.INFO;
        private string Filepath = null;
        private StreamWriter LogWriter = null;
        private bool IsLoggingDisabled = false;
        private string CurrentDir;
        private static LoggerDelegate Delegate;

        private Logger()
        {
            FileStream Stream;
            this.CurrentDir = Path.GetTempPath();

            /* Open the log file at the temp directory first and acquire the handle.     */
            Filepath = Path.Combine(CurrentDir, Path.GetTempFileName() + ".txt");
            try
            {
                Stream = new FileStream(
                        Filepath,
                        FileMode.Create,
                        FileAccess.Write,
                        FileShare.Read
                    );
            }
            catch (Exception e)
            {
                DisableLogging(e.ToString());
                return;
            }

            LogWriter = new StreamWriter(Stream);
            Delegate = CallUnmanagedLog;

            string envLevel = Environment.GetEnvironmentVariable("I2P_LOG_LEVEL");
            if (!string.IsNullOrEmpty(envLevel) && Enum.TryParse(envLevel, true, out LogLevel parsedLevel))
                MinLevel = parsedLevel;
            
            
            /* Setup C logging. */
            ITDParser.ITDParserSetLogger(GetDelegate());
        }

        private void DisableLogging(string ExceptionMessage)
        {
            if (IsLoggingDisabled)
                return;
            IsLoggingDisabled = true;
            MessageBox.Show("Logging is disabled due to exception:\n" + ExceptionMessage, "I2P Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
        }

        public void Destroy()
        {
            lock (WriterLog)
            {
                LogWriter.Flush();
                LogWriter?.Dispose();
                LogWriter = null;
                IsLoggingDisabled = true;
            }
        }

        public void MoveToDirectory(string NewDir)
        {
            string NewPath;
            FileStream Stream;
            StreamWriter NewWriter;
            StreamWriter OldWriter;
            int pid = Process.GetCurrentProcess().Id;
            string Timestamp = DateTime.Now.ToString("yyyy-MM-dd--HH-mm");

            if (CurrentDir == NewDir)
                return;


            /* If at some point we're provided with a new directory                      */
            /* Copy the already used log file to it and acquire a new file handle.       */


            NewPath = Path.Combine(NewDir, $"{Timestamp}_{pid}_{LOG_NAME}");

            /* Locking is required so we only move after all writing is done.            */
            lock(WriterLog)
            {
                if (IsLoggingDisabled)
                    return;

                try
                {
                    /* Make sure all writes are in. */
                    LogWriter.Flush();
                    File.Copy(Filepath, NewPath, true);
                    Stream = new FileStream(
                            NewPath,
                            FileMode.Append,
                            FileAccess.Write,
                            FileShare.Read
                        );
                } catch(Exception e)
                {
                    DisableLogging(e.ToString());
                    return;
                }


                NewWriter = new StreamWriter(Stream);

                Filepath = NewPath;

                /* The below sequence is important, it appears thread safe but if we don't do this atomic */
                /* swap and dipose after then task threads will see a disposed writer....                 */
                OldWriter = LogWriter;
                LogWriter = NewWriter;

                try
                {
                    LogWriter.Flush();
                } catch(Exception e)
                {
                    DisableLogging(e.ToString());
                }

                OldWriter?.Dispose();
            }

            CurrentDir = NewDir;
        }

        private void LogAux(LogLevel Level, string File, string Function, int Line, string Message)
        {
            /* Log the message formatted as:                                             */
            /* <TIME>::<COMPONENT>::<FILE>:<FUNCTION>:<LINE> -:- (<LEVEL>) <MESSAGE>     */
            if (Level < this.MinLevel)
                return;

            lock (WriterLog)
            {
                if (IsLoggingDisabled)
                    return;

                string Timestamp = DateTime.Now.ToString("yyyy-MM-ddTHH:mm:ss.fff");
                string Filename = Path.GetFileName(File);

                try
                {
                    LogWriter.WriteLine($"({Level}) {Timestamp} [{Function}:{Line}@{Filename}] {Message}");
                    LogWriter.Flush();
                } catch(Exception e)
                {
                    DisableLogging(e.ToString());
                    return;
                }
            }

        }

        public void Trace(string Message,
                [CallerFilePath] string File = "",
                [CallerMemberName] string Function = "",
                [CallerLineNumber] int Line = 0)
        {
            LogAux(LogLevel.TRACE, File, Function, Line, Message);
        }

        public void Info(string Message,
                [CallerFilePath] string File = "",
                [CallerMemberName] string Function = "",
                [CallerLineNumber] int Line = 0)
        {
            LogAux(LogLevel.INFO, File, Function, Line, Message);
        }

        public void Warn(string Message,
                [CallerFilePath] string File = "",
                [CallerMemberName] string Function = "",
                [CallerLineNumber] int Line = 0)
        {
            LogAux(LogLevel.WARN, File, Function, Line, Message);
        }

        private void UnmanagedLog(int Level, string File, string Function, int Line, string Message)
        {
            LogAux((LogLevel)Level, File, Function, Line, Message);
        }

        public void Error(string Message,
                [CallerFilePath] string File = "",
                [CallerMemberName] string Function = "",
                [CallerLineNumber] int Line = 0)
        {
            LogAux(LogLevel.ERROR, File, Function, Line, Message);
        }

        public LoggerDelegate GetDelegate()
        {
            return Delegate;
        }

        public static void CallUnmanagedLog(int Level, string File, string Function, int Line, string Message)
        {
            Instance.UnmanagedLog(Level, File, Function, Line, Message);
        }

        public static Logger GetInstance()
        {
            return Instance;
        }

    }
}
