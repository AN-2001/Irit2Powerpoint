using System;
using OpenTK.Graphics.OpenGL4;
using OpenTK.Graphics;
using System.Text;
using System.Windows.Forms;

namespace Irit2Powerpoint
{
    struct ShaderSources
    {
        public static int PolyProg, CrvProg;

        public static readonly string TRANS_BLOCK_NAME = "Transforms";
        public static readonly string SETTINGS_BLOCK_NAME = "Settings";
        public static readonly string POLY_VERT = @"
        #version 330 core
        layout (location = 0) in vec3 Position;
        layout (location = 1) in vec3 Normal;
        layout (location = 2) in vec3 Colour;
        layout (location = 3) in vec2 uv;

        layout (std140) uniform Transforms
        {
            mat4 ModelView;
            mat4 Inverse;
            mat4 InvTranspose;
            mat4 Projection;
        };

        layout (std140) uniform Settings
        {
            vec3 LightPos;
            vec3 LightCol;
            vec3 DfltSolidClr;
            vec3 DfltCrvClr;
        };
    
        out vec3 Norm;
        out vec3 Pos;
        out vec3 VertColour;
        
        void main()
        {
            vec3 ViewPos = (ModelView * vec4(Position, 1.f)).xyz;
            vec3 ViewNorm = (InvTranspose * vec4(Normal, 1.f)).xyz;
            gl_Position = Projection * vec4(ViewPos, 1.f); 
            Norm = ViewNorm;
            Pos = ViewPos;
            VertColour = Colour;
        }"; 
        
        public static readonly string POLY_FRAG = @"
        #version 330 core
        in vec3 Norm;
        in vec3 Pos;
        in vec3 VertColour;
        out vec4 FragColor;

        layout (std140) uniform Transforms
        {
            mat4 ModelView;
            mat4 Inverse;
            mat4 InvTranspose;
            mat4 Projection;
        };

        layout (std140) uniform Settings
        {
            vec3 LightPos;
            vec3 LightCol;
            vec3 DfltSolidClr;
            vec3 DfltCrvClr;
        };

        void main()
        {
            vec3 Colour = VertColour;

            vec3 CameraPos = (Inverse * vec4(vec3(0.f), 1.f)).xyz;
            vec3 DiffuseCoeff = Colour;
            vec3 AmbientCoeff = Colour * 0.25f;
            vec3 SpecularCoeff = vec3(1.f);
            float SpecularPower = 4.f;

            vec3 LightWorld = (ModelView * vec4(LightPos, 1.f)).xyz;
            vec3 LightVector = normalize(LightWorld - Pos);
            vec3 ViewVector = normalize(CameraPos - Pos);

            vec3 N = normalize(Norm);
            vec3 R = 2 * dot(LightVector, N) * N - LightVector;

            float DiffuseAngle = max(dot(LightVector, N), 0.f);
            float Specular = pow(max(dot(R, normalize(ViewVector)), 0.f), SpecularPower) * DiffuseAngle;

            vec3 PixelColour = max(min(AmbientCoeff + DiffuseCoeff * DiffuseAngle + Specular * SpecularCoeff, vec3(1.f)), vec3(0.f));

            FragColor = vec4(vec3(PixelColour), 1.f);
        }";

        public static readonly string CRV_VERT = @"
        #version 330 core
        layout (location = 0) in vec3 Position;
        layout (location = 1) in vec3 Normal;
        layout (location = 2) in vec3 Colour;
        layout (location = 3) in vec2 uv;

        layout (std140) uniform Transforms
        {
            mat4 ModelView;
            mat4 Inverse;
            mat4 InvTranspose;
            mat4 Projection;
        };

        layout (std140) uniform Settings
        {
            vec3 LightPos;
            vec3 LightCol;
            vec3 DfltSolidClr;
            vec3 DfltCrvClr;
        };
        
        out vec3 Col;

        void main()
        {
            vec3 ViewPos = (ModelView * vec4(Position, 1.f)).xyz;

            gl_Position = Projection * vec4(ViewPos, 1.f); 
            gl_Position.z -= 0.0001f;
            Col = Colour;
        }"; 

        public static readonly string CRV_FRAG = @"
        #version 330 core
        out vec4 FragColor;
        in vec3 Col;

        layout (std140) uniform Settings
        {
            vec3 LightPos;
            vec3 LightCol;
            vec3 DfltSolidClr;
            vec3 DfltCrvClr;
        };

        void main()
        {
            vec3 Colour = Col;
            FragColor = vec4(Colour, 1.0f);
        }";
    }

    struct TransformBlock 
    {
        public OpenTK.Matrix4 ModelView;
        public OpenTK.Matrix4 Inverse;
        public OpenTK.Matrix4 InvTranspose;
        public OpenTK.Matrix4 Projection;
        public static readonly int
            Size = OpenTK.BlittableValueType<TransformBlock>.Stride;
    }

#pragma warning disable CS0649
    /* DONT REMOVE a1, a2, a3, they're for alllignment on GPU memory. */
    struct SettingsBlock
    {
        public OpenTK.Vector3 LightPos;
        public float a1;
        public OpenTK.Vector3 LightCol;
        public float a2;
        public OpenTK.Vector3 DfltSolidClr;
        public float a3;
        public OpenTK.Vector3 DfltCrvClr;
        public static readonly int
            Size = OpenTK.BlittableValueType<SettingsBlock>.Stride;
    }
#pragma warning restore CS0649

    public abstract class BaseContext
    {
        protected int ubo;
        protected int BlockSize;
        protected bool NeedUpdate;
        int BindingIndex;
        string UniformName;

        public BaseContext(string UniformName, int BlockSize, int BindingIndex)
        {
            this.BindingIndex = BindingIndex;
            this.UniformName = UniformName;
            this.BlockSize = BlockSize;
            ubo = GL.GenBuffer();
            GL.BindBuffer(BufferTarget.UniformBuffer, ubo);
            /* Allocate the struct. */
            GL.BufferData(BufferTarget.UniformBuffer,
                    BlockSize,
                    IntPtr.Zero,
                    BufferUsageHint.StaticDraw);
            GL.BindBufferBase(BufferRangeTarget.UniformBuffer, BindingIndex, ubo);
            GL.BindBuffer(BufferTarget.UniformBuffer, 0);
            NeedUpdate = true;
        }

        public void Destroy()
        {
            GL.DeleteBuffer(ubo);
        }

        public void InitSync(int Program)
        {
            int Index = GL.GetUniformBlockIndex(Program, UniformName);
            GL.UniformBlockBinding(Program, Index, BindingIndex);
        }


        abstract public void PerformSync();
        abstract public void Reset();

        protected void PerformSyncImpl<T>(T Block) where T : struct
        {
            /* Upload block to GPU. */
            GL.BindBuffer(BufferTarget.UniformBuffer, ubo);
            GL.BufferSubData(BufferTarget.UniformBuffer,
                    IntPtr.Zero,
                    BlockSize,
                    ref Block);
            GL.BindBuffer(BufferTarget.UniformBuffer, 0);
            NeedUpdate = false;
        }

    }

    public class TransformContext : BaseContext
    {
        private float Zoom, AspectRatio;
        private int Width, Height;
        private OpenTK.Matrix4 SavedModelView,
                               ActiveModelView,
                               Projection;

        public TransformContext()
            : base(ShaderSources.TRANS_BLOCK_NAME, TransformBlock.Size, 0)
        {
            Reset();
        }

        public override void PerformSync()
        {
            if (!NeedUpdate)
                return;
            OpenTK.Matrix4 ModelView = SavedModelView * ActiveModelView;
            TransformBlock Block = new TransformBlock();

            Block.ModelView = ModelView;
            Block.Inverse = ModelView.Inverted();
            Block.InvTranspose = ModelView.Inverted();
            Block.InvTranspose.Transpose();
            Block.Projection = Projection;

            PerformSyncImpl(Block);
        }

        public OpenTK.Matrix4 GetActiveWorld()
        {
            return ActiveModelView;
        }

        public void SaveActiveModelView()
        {
            SavedModelView = SavedModelView * ActiveModelView;
            ActiveModelView = OpenTK.Matrix4.Identity;
            NeedUpdate = true;
        }

        public void SetProjection(OpenTK.Matrix4 Mat)
        {
            Projection = Mat;
            NeedUpdate = true;
        }

        public OpenTK.Matrix4 GetProjection()
        {
            return Projection;
        }

        public void SetActiveModelView(OpenTK.Matrix4 Mat)
        {
            ActiveModelView = Mat;
            NeedUpdate = true;
        }

        public void SetRes(int Width, int Height)
        {
            this.Width = Width;
            this.Height = Height;
            this.AspectRatio = (float)Width / Height;
        }

        public void UpdateProjection(OpenTK.Vector3 BBMin, OpenTK.Vector3 BBMax)
        {
            float w = (BBMax.X - BBMin.X);
            float h = (BBMax.Y - BBMin.Y);
            float m = Math.Max(w, h);
            Projection = OpenTK.Matrix4.CreateOrthographic(m * AspectRatio, m, -1000, 1000);
            NeedUpdate = true;
        }

        public void SetZoom(int Direction)
        {
            float Dir;
            if (Direction > 0)
                Dir = 1.1f;
            else
                Dir = 1.0f / 1.1f;

            Zoom = Zoom * Dir;
            SetActiveModelView(ActiveModelView *
                            OpenTK.Matrix4.CreateScale(Dir));
        }

        public float GetZoom()
        {
            return Zoom;
        }

        public float GetWidth()
        {
            return Width;
        }

        public float GetHeight()
        {
            return Height;
        }

        public override void Reset()
        {
            Zoom = 1;
            SavedModelView = OpenTK.Matrix4.Identity;
            SetActiveModelView(OpenTK.Matrix4.LookAt(new OpenTK.Vector3(0, 0, 1),
                                                     new OpenTK.Vector3(0, 0, 0),
                                                     new OpenTK.Vector3(0, 1, 0)));
            SaveActiveModelView();
        }
    }

    public class SettingsContext : BaseContext
    {
        GlRenderer.RenderSettings Settings;

        public SettingsContext() : base(ShaderSources.SETTINGS_BLOCK_NAME, SettingsBlock.Size, 1)
        {
            Reset();
        }

        public void UpdateSettings(GlRenderer.RenderSettings Settings)
        {
            this.Settings = Settings;
            GL.ClearColor(Settings.BackgroundColour.X, Settings.BackgroundColour.Y, Settings.BackgroundColour.Z, 0.0f);
            NeedUpdate = true;
        }

        override public void PerformSync()
        {
            if (!NeedUpdate)
                return;
            SettingsBlock Block = new SettingsBlock();

            Block.LightPos = Settings.LightPosition;
            Block.LightCol = Settings.LightColour;
            Block.DfltSolidClr = Settings.DefaultSolidColour;
            Block.DfltCrvClr = Settings.DefaultCurveColour;
            PerformSyncImpl(Block);
        }

        public override void Reset()
        {
            Settings = GlRenderer.DefaultRenderSettings;
        }

    }

    public class GlRenderer
    {
        public struct RenderSettings
        {
            public OpenTK.Vector3 LightPosition;
            public OpenTK.Vector3 LightColour;
            public OpenTK.Vector3 DefaultSolidColour;
            public OpenTK.Vector3 DefaultCurveColour;
            public OpenTK.Vector3 BackgroundColour;
        };

        public static string SerializeRenderSettings(RenderSettings Settings)
        {
            StringBuilder
                sb = new StringBuilder();

            sb.AppendFormat("{0},{1},{2};{3},{4},{5};{6},{7},{8};{9},{10},{11};{12},{13},{14}",
                            Settings.LightPosition.X, Settings.LightPosition.Y, Settings.LightPosition.Z,
                            Settings.LightColour.X, Settings.LightColour.Y, Settings.LightColour.Z,
                            Settings.DefaultSolidColour.X, Settings.DefaultSolidColour.Y, Settings.DefaultSolidColour.Z,
                            Settings.DefaultCurveColour.X, Settings.DefaultCurveColour.Y, Settings.DefaultCurveColour.Z,
                            Settings.BackgroundColour.X, Settings.BackgroundColour.Y, Settings.BackgroundColour.Z);
            return sb.ToString();
        }

        public static RenderSettings DeserializeRenderSettings(string String)
        {
            RenderSettings
                Settings = new RenderSettings();
            String[] Splitted = String.Split(';');

            try
            {
                Settings.LightPosition = VecFromStr(Splitted[0]);
                Settings.LightColour = VecFromStr(Splitted[1]);
                Settings.DefaultSolidColour = VecFromStr(Splitted[2]);
                Settings.DefaultCurveColour = VecFromStr(Splitted[3]);
                Settings.BackgroundColour = VecFromStr(Splitted[4]);
            } catch(Exception)
            {
                MessageBox.Show("Render Settings string was corrupted, ignoring and using defaults.");
                Settings = DefaultRenderSettings;
            }

            return Settings;
        }

        private static OpenTK.Vector3 VecFromStr(string String)
        {
            OpenTK.Vector3
                Vec = new OpenTK.Vector3();
            String[] Splitted = String.Split(',');

            Vec.X = float.Parse(Splitted[0]);
            Vec.Y = float.Parse(Splitted[1]);
            Vec.Z = float.Parse(Splitted[2]);
            return Vec;
        }

        private GraphicsContext Context;
        private OpenTK.Platform.IWindowInfo Info;
        private GlResource ActiveResource;
        private RenderSettings ActiveSettings;
        private bool Loaded;
        private string LastPath;
        private DateTime Time;
        public TransformContext TransCtx;
        public SettingsContext SettingsCtx;
        public static RenderSettings DefaultRenderSettings = new RenderSettings()
        {
            LightPosition = new OpenTK.Vector3(0.0f, 10.0f, 0.0f),
            LightColour = new OpenTK.Vector3(1.0f, 1.0f, 1.0f),
            DefaultSolidColour = new OpenTK.Vector3(0.7f, 0.4f, 0.2f),
            DefaultCurveColour = new OpenTK.Vector3(0.9f, 0.75f, 0.2f),
            BackgroundColour = new OpenTK.Vector3(0.2f, 0.2f, 0.2f),
        };

        public GlRenderer(IntPtr hWnd)
        {

            GraphicsMode Mode;
            Info = OpenTK.Platform.Utilities.CreateWindowsWindowInfo(hWnd);
            Mode = new GraphicsMode(32, 24, 0, 4);

            Context = new GraphicsContext(Mode,
                            Info, 3, 5, GraphicsContextFlags.Default | GraphicsContextFlags.ForwardCompatible);
            Context.MakeCurrent(Info);
            Context.LoadAll();


            TransCtx = new TransformContext();
            SettingsCtx = new SettingsContext();

            GL.Enable(EnableCap.DepthTest);
            GL.Enable(EnableCap.Multisample);

            ShaderSources.PolyProg = InitShader(ShaderSources.POLY_VERT,
                                                ShaderSources.POLY_FRAG);
            ShaderSources.CrvProg = InitShader(ShaderSources.CRV_VERT,
                                               ShaderSources.CRV_FRAG);
            SettingsCtx.InitSync(ShaderSources.PolyProg);
            SettingsCtx.InitSync(ShaderSources.CrvProg);

            SettingsCtx.UpdateSettings(DefaultRenderSettings);
            Loaded = false;
            LastPath = null;
            Time = DateTime.Now;

        }

        private int InitShader(string VertSrc, string FragSrc)
        {
            int VertexShader, FragmentShader, Program;

            VertexShader = GL.CreateShader(ShaderType.VertexShader);
            GL.ShaderSource(VertexShader, VertSrc);
            GL.CompileShader(VertexShader);
            CheckShaderCompileStatus(VertexShader);

            FragmentShader = GL.CreateShader(ShaderType.FragmentShader);
            GL.ShaderSource(FragmentShader , FragSrc);
            GL.CompileShader(FragmentShader);
            CheckShaderCompileStatus(FragmentShader);

            Program = GL.CreateProgram();
            GL.AttachShader(Program, VertexShader);
            GL.AttachShader(Program, FragmentShader);
            GL.LinkProgram(Program);
            CheckProgramLinkStatus(Program);
                
            GL.DeleteShader(VertexShader);
            GL.DeleteShader(FragmentShader);

            TransCtx.InitSync(Program);
            return Program;
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
            /* Keep trying to load, try once a second so we don't exhaust the CPU. */
            if (!Loaded && LastPath != null)
            {
                TimeSpan
                    Span = DateTime.Now - Time;
                if (Span.TotalSeconds >= 1)
                {
                    SetActiveModel(LastPath, ActiveSettings);
                    Time = DateTime.Now;
                }
            }

            TransCtx.PerformSync();
            SettingsCtx.PerformSync();

            GL.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);
            if (Loaded)
            {
                GL.BindVertexArray(ActiveResource.VAO);

                if (ActiveResource.PolygonRecord.NumElements > 0)
                {
                    GL.UseProgram(ShaderSources.PolyProg);
                    GL.DrawArrays(PrimitiveType.Triangles,
                                  ActiveResource.PolygonRecord.Offset,
                                  ActiveResource.PolygonRecord.NumElements);
                }

                if (ActiveResource.PolylineRecord.NumElements > 0)
                {
                    GL.UseProgram(ShaderSources.CrvProg);
                    GL.DrawArrays(PrimitiveType.Lines,
                                  ActiveResource.PolylineRecord.Offset,
                                  ActiveResource.PolylineRecord.NumElements);
                }
            }

            ErrorCode code = GL.GetError();

            Context.SwapBuffers();

            GL.BindVertexArray(0);
            GL.UseProgram(0);
        }

        public void SetActiveModel(string Filepath, GlRenderer.RenderSettings Settings)
        {

            try
            {
                ActiveResource = I2P.GetResourceManager().GetResource(Filepath);
                Loaded = true;
                LastPath = null;
                TransCtx.Reset();
                SettingsCtx.Reset();

                SettingsCtx.UpdateSettings(Settings);
                if (this.ActiveResource.ContainsView) {
                    TransCtx.SetActiveModelView(this.ActiveResource.ViewMat);
                    TransCtx.SaveActiveModelView();
                }

                if (this.ActiveResource.ContainsProj)
                {
                    TransCtx.SetProjection(this.ActiveResource.ProjMat);
                }
                else
                {
                    TransCtx.UpdateProjection(this.ActiveResource.BBoxMin,
                                              this.ActiveResource.BBoxMax);
                }
            }
            catch (StillLoadingException)  
            {
                Loaded = false;
                LastPath = Filepath;
                ActiveSettings = Settings;
            }

        }

        public void Destroy()
        {
            GL.DeleteProgram(ShaderSources.PolyProg);
            GL.DeleteProgram(ShaderSources.CrvProg);
            TransCtx.Destroy();
            SettingsCtx.Destroy();
        }

    }
}
