using System;
using OpenTK.Graphics.OpenGL4;
using OpenTK.Graphics;
using System.Diagnostics;


namespace Irit2Powerpoint
{
    class TransformLocations
    {
        public readonly string
            MODEL_VIEW_NAME = "ModelView",
            INVERSE_TRANSPOSE_NAME = "InverseTranspose",
            PROJECTION_NAME = "Projection";
        public int 
            ModelView,
            InverseTranspose,
            Projection;
    }

    class TransformContext
    {
        private float Zoom;
        private int Width, Height;
        private int zNear = -300,
                    zFar = 300;
        private OpenTK.Matrix4 SavedWorld,
                               ActiveWorld,
                               View,
                               Projection;
        private bool NeedMatrixUpdate;
        private bool NeedProjectionUpdate;
        private TransformLocations Locations;

        public TransformContext()
        {
            Locations = new TransformLocations();
            Reset();
        }

        public void InitSync(int Program)
        {
            Locations.ModelView = GL.GetUniformLocation(Program, Locations.MODEL_VIEW_NAME);
            Locations.InverseTranspose = GL.GetUniformLocation(Program, Locations.INVERSE_TRANSPOSE_NAME);
            Locations.Projection = GL.GetUniformLocation(Program, Locations.PROJECTION_NAME);
        }

        public void PerformSync()
        {
            if (!NeedMatrixUpdate)
                return;
            OpenTK.Matrix4 ModelView, InverseTranspose,
                World = SavedWorld * ActiveWorld;

            ModelView = World * View;
            InverseTranspose = ModelView.Inverted();
            InverseTranspose.Transpose();

            /* Upload matrices to GPU. */
            GL.UniformMatrix4(Locations.ModelView, false, ref ModelView);
            GL.UniformMatrix4(Locations.InverseTranspose, false, ref InverseTranspose);
            if (NeedProjectionUpdate)
                GL.UniformMatrix4(Locations.Projection, false, ref Projection);
            NeedMatrixUpdate = false;
            NeedProjectionUpdate = false;
        }

        public OpenTK.Matrix4 GetActiveWorld()
        {
            return ActiveWorld;
        }

        public void SaveActiveWorld()
        {
            SavedWorld = SavedWorld * ActiveWorld;
            ActiveWorld = OpenTK.Matrix4.Identity;
            NeedMatrixUpdate = true;
        }

        public void SetView(OpenTK.Matrix4 Mat)
        {
            View = Mat;
            NeedMatrixUpdate = true;
        }

        public void SetActiveWorld(OpenTK.Matrix4 Mat)
        {
            ActiveWorld = Mat;
            NeedMatrixUpdate = true;
        }

        public void SetRes(int Width, int Height)
        {
            this.Width = Width;
            this.Height = Height;
            UpdateProjection();
        }

        private void UpdateProjection()
        {
            Projection = OpenTK.Matrix4.CreateOrthographic(Width * Zoom, Height * Zoom,
                                                            zNear, zFar);
            NeedProjectionUpdate = true;
            NeedMatrixUpdate = true;
        }

        public void SetZoom(int Direction)
        {
            if (Direction > 0)
                Zoom /= 1.1f;
            else
                Zoom *= 1.1f;

            UpdateProjection();
        }

        public float GetZoom()
        {
            return Zoom;
        }

        public void Reset()
        {
            Zoom = 1;
            ActiveWorld = SavedWorld = OpenTK.Matrix4.Identity;
            SetView(OpenTK.Matrix4.LookAt(new OpenTK.Vector3(0, 0, 30),
                                          new OpenTK.Vector3(0, 0, 0),
                                          new OpenTK.Vector3(0, 1, 0)));
            UpdateProjection();
        }


    }

    class GlRenderer
    {
        private string vertexShaderSource = @"
        #version 330 core
        layout (location = 0) in vec3 Pos;
        layout (location = 1) in vec3 Normal;

        uniform mat4 ModelView;
        uniform mat4 InverseTranspose;
        uniform mat4 Projection;

        out vec3 Norm;
        
        void main()
        {
            vec4 WorldPos = ModelView * vec4(Pos, 1.f);

            gl_Position = Projection * WorldPos; 
            Norm = (InverseTranspose * vec4(Normal, 1.f)).xyz;
        }"; 
        
        private string fragmentShaderSource = @"
        #version 330 core
        in vec3 Norm;
        out vec4 FragColor;

        void main()
        {
            FragColor = vec4((Norm + vec3(1.f)) / 2.f, 1.0f);
        }";

        private int MainProgram;
        private GraphicsContext Context;
        private OpenTK.Platform.IWindowInfo Info;
        private GlResourceManager ResourceManager;
        private GlResource ActiveResource;
        private bool Loaded;
        public TransformContext TransCtx;

        public GlRenderer(IntPtr hWnd)
        {

            Info = OpenTK.Platform.Utilities.CreateWindowsWindowInfo(hWnd);
            Context = new GraphicsContext(GraphicsMode.Default,
                            Info, 4, 0, GraphicsContextFlags.Default);
            Context.MakeCurrent(Info);
            Context.LoadAll();

            TransCtx = new TransformContext();

            GL.Enable(EnableCap.DepthTest);
            ResourceManager = new GlResourceManager();
            InitShader();

            // GL.ClearColor(new Color4(0.2f, 0.7f, 0.8f, 0.0f));
            GL.ClearColor(new Color4(1.0f, 1.0f, 1.0f, 0.0f));
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

            TransCtx.InitSync(MainProgram);
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
            TransCtx.SetRes(Width, Height);
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

            TransCtx.PerformSync();
            GL.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);
            if (Loaded)
            {
                GL.BindVertexArray(ActiveResource.VAO);
                foreach (GlMeshRecord Record in ActiveResource.PolygonRecords)
                    GL.DrawArrays(PrimitiveType.Triangles, Record.Offset, Record.NumElements);
                foreach (GlMeshRecord Record in ActiveResource.PolylineRecords)
                    GL.DrawArrays(PrimitiveType.LineStrip, Record.Offset, Record.NumElements);
            }

            Context.SwapBuffers();
            GL.BindVertexArray(0);
        }

        public void SetActiveModel(string Filepath)
        {
            this.ActiveResource = ResourceManager.GetResource(Filepath);
            TransCtx.Reset();
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
