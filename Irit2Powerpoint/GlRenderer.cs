using System;
using OpenTK.Graphics.OpenGL4;
using OpenTK.Graphics;
using System.Diagnostics;


namespace Irit2Powerpoint
{
    class GlRenderer
    {
        private string vertexShaderSource = @"
        #version 330 core
        layout (location = 0) in vec3 Pos;
        layout (location = 1) in vec3 Normal;

        out vec3 Norm;
        
        void main()
        {
            gl_Position = vec4(Pos * 0.25f, 1.f);
            Norm = Normal;
        }"; 
        
        private string fragmentShaderSource = @"
        #version 330 core
        in vec3 Norm;
        out vec4 FragColor;

        void main()
        {
            FragColor = vec4(Norm, 1.0f);
        }";

        private int MainProgram;

        private GraphicsContext Context;
        private OpenTK.Platform.IWindowInfo Info;
        private GlResourceManager ResourceManager;
        private GlResource ActiveResource;
        private bool Loaded;

        public GlRenderer(IntPtr hWnd)
        {

            Info = OpenTK.Platform.Utilities.CreateWindowsWindowInfo(hWnd);
            Context = new GraphicsContext(GraphicsMode.Default,
                            Info, 4, 0, GraphicsContextFlags.Default);
            Context.MakeCurrent(Info);
            Context.LoadAll();

            GL.Enable(EnableCap.DepthTest);
            GL.Enable(EnableCap.CullFace);
            GL.CullFace(CullFaceMode.Front);
            ResourceManager = new GlResourceManager();
            InitShader();

            GL.ClearColor(new Color4(0.0f, 0.0f, 0.0f, 0.0f));
            Loaded = false;
        }

        private void InitShader()
        {
            int vertexShader, fragmentShader;

            vertexShader = GL.CreateShader(ShaderType.VertexShader);
            GL.ShaderSource(vertexShader, vertexShaderSource);
            GL.CompileShader(vertexShader);
            CheckShaderCompileStatus(vertexShader);

            fragmentShader = GL.CreateShader(ShaderType.FragmentShader);
            GL.ShaderSource(fragmentShader , fragmentShaderSource);
            GL.CompileShader(fragmentShader);
            CheckShaderCompileStatus(fragmentShader);

            MainProgram = GL.CreateProgram();
            GL.AttachShader(MainProgram, vertexShader);
            GL.AttachShader(MainProgram, fragmentShader);
            GL.LinkProgram(MainProgram);
            CheckProgramLinkStatus(MainProgram);
                
            GL.DeleteShader(vertexShader);
            GL.DeleteShader(fragmentShader);

        }

        public void UpdateViewport(int Width, int Height)
        {
            System.Drawing.Point
                P = new System.Drawing.Point(0, 0),
                Size = new System.Drawing.Point(Width, Height);
            System.Drawing.Size
                S = new System.Drawing.Size(Size);
            System.Drawing.Rectangle
                R = new System.Drawing.Rectangle(P, S);
            GL.Viewport(R);
        }

        private void CheckShaderCompileStatus(int shader)
        {
            GL.GetShader(shader, ShaderParameter.CompileStatus, out int success);
            if (success == 0)
            {
                string infoLog = GL.GetShaderInfoLog(shader);
                throw new Exception($"Shader compilation failed ({shader}): {infoLog}");
            }
        }

        private void CheckProgramLinkStatus(int program)
        {
            GL.GetProgram(program, GetProgramParameterName.LinkStatus, out int success);
            if (success == 0)
            {
                string infoLog = GL.GetProgramInfoLog(program);
                throw new Exception($"Program linking failed: {infoLog}");
            }
        }

        public void Render()
        {
            GL.UseProgram(MainProgram);
            GL.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);
            if (Loaded)
            {
                GL.BindVertexArray(ActiveResource.VAO);
                GL.BindBuffer(BufferTarget.ElementArrayBuffer, ActiveResource.EBO);
                GL.DrawElements(BeginMode.Triangles, ActiveResource.NumElements, DrawElementsType.UnsignedInt, 0);
            }

            Context.SwapBuffers();
            GL.BindBuffer(BufferTarget.ElementArrayBuffer, 0);
            GL.BindVertexArray(0);
        }

        public void SetActiveModel(string Filepath)
        {
            this.ActiveResource = ResourceManager.GetResource(Filepath);
            Loaded = true;
        }

        public void Destroy()
        {
            ResourceManager.Destroy();
        }

        public static string GetErrorString(ErrorCode errorCode)
        {
            switch (errorCode)
            {
                case ErrorCode.InvalidEnum:
                    return "Invalid Enum";
                case ErrorCode.InvalidValue:
                    return "Invalid Value";
                case ErrorCode.InvalidOperation:
                    return "Invalid Operation";
                case ErrorCode.OutOfMemory:
                    return "Out of Memory";
                case ErrorCode.TableTooLarge:
                    return "Table Too Large";
                case ErrorCode.NoError:
                    return "No Error";
                default:
                    return $"Unknown Error ({errorCode})";
            }

        }

    }
}
