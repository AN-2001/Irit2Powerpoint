using System;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.Drawing;
using Office = Microsoft.Office.Core;

namespace Irit2Powerpoint
{
    [ComVisible(true)]
    public class Ribbon : Office.IRibbonExtensibility
    {
        private Office.IRibbonUI ribbon;

        public Ribbon()
        {
        }

        #region IRibbonExtensibility Members

        public string GetCustomUI(string ribbonID)
        {
            return GetResourceText("Irit2Powerpoint.Ribbon.xml");
        }

        #endregion

        #region Ribbon Callbacks

        public void Ribbon_Load(Office.IRibbonUI ribbonUI)
        {
            this.ribbon = ribbonUI;
        }


        public Bitmap OnI2PButtonImage(Office.IRibbonControl Control)
        {
            return new Bitmap(Properties.Resources.button);
        }


        /* Extract the path from the import string. */
        private string ExtractPath(string ImportSettings)
        {
            string NoWhiteSpaces = ImportSettings.TrimEnd();
            int Index;
            if (NoWhiteSpaces.EndsWith("\""))
            {
                Index = NoWhiteSpaces.LastIndexOf('\"', NoWhiteSpaces.Length - 2);
                return NoWhiteSpaces.Substring(Index + 1, NoWhiteSpaces.Length - Index - 2);
            } else if (NoWhiteSpaces.EndsWith("\'"))
            {
                Index = NoWhiteSpaces.LastIndexOf('\'', NoWhiteSpaces.Length - 2);
                return NoWhiteSpaces.Substring(Index + 1, NoWhiteSpaces.Length - Index - 2);
            }

            /* Space seperated. */
            Index = NoWhiteSpaces.LastIndexOf(' ', NoWhiteSpaces.Length - 2);
            return NoWhiteSpaces.Substring(Index + 1, NoWhiteSpaces.Length - Index - 1);
        }

        public void OnI2PButton(Office.IRibbonControl Control)
        {
            I2P AddIn = Globals.I2P;
            string
                Settings = AddIn.GetCurrentImportSettings();
            string InputPath;
            Resources.ImportSettings
                SettingsForm = new Resources.ImportSettings(Settings);

            Logger.GetInstance().Trace("Detected I2P button click.");
            if (SettingsForm.ShowDialog() == DialogResult.OK)
            {
                InputPath = ExtractPath(SettingsForm.PickedSettings);
                AddIn.InitDummyRect(InputPath, SettingsForm.PickedSettings);
                Logger.GetInstance().Trace($"Detected user selection for settings and path: Path = {InputPath}, Settings = {SettingsForm.PickedSettings}");
            }
        }


        #endregion

        

        #region Helpers

        private static string GetResourceText(string resourceName)
        {
            Assembly asm = Assembly.GetExecutingAssembly();
            string[] resourceNames = asm.GetManifestResourceNames();
            for (int i = 0; i < resourceNames.Length; ++i)
            {
                if (string.Compare(resourceName, resourceNames[i], StringComparison.OrdinalIgnoreCase) == 0)
                {
                    using (StreamReader resourceReader = new StreamReader(asm.GetManifestResourceStream(resourceNames[i])))
                    {
                        if (resourceReader != null)
                        {
                            return resourceReader.ReadToEnd();
                        }
                    }
                }
            }
            return null;
        }

        #endregion
    }
}
