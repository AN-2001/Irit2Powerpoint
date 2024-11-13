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
        public static readonly string LIGHT_BLOCK_NAME = "Lights";
        public static readonly string POLY_VERT = @"
        #version 330 core
        layout (location = 0) in vec3 Position;
        layout (location = 1) in vec3 Normal;
        layout (location = 2) in vec3 Colour;
        layout (location = 3) in vec2 uv;

        layout (std140) uniform Transforms
        {
            mat4 World;
            mat4 InvTranspose;
            mat4 View;
            mat4 Projection;
        };
    

        out vec3 Norm;
        out vec3 Pos;
        out vec3 VertColour;
        
        void main()
        {
            mat4 ModelView = World * View;

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

        layout (std140) uniform Lights
        {
            vec3 LightPos[3];
            vec3 LightCol[3];
        };

        layout (std140) uniform Transforms
        {
            mat4 World;
            mat4 InvTranspose;
            mat4 View;
            mat4 Projection;
        };

        void main()
        {
            vec3 CameraPos = (inverse(View) * inverse(World) * vec4(vec3(0.f), 1.f)).xyz;
            vec3 DiffuseCoeff = VertColour;
            vec3 AmbientCoeff = VertColour * 0.1f;
            vec3 SpecularCoeff = vec3(1.f);
            float SpecularPower = 4.f;

            vec3 LightWorld = (World * View * vec4(LightPos[0], 1.f)).xyz;
            vec3 LightVector = normalize(LightWorld - Pos);
            vec3 ViewVector = normalize(CameraPos - Pos);

            vec3 N = normalize(Norm);
            vec3 R = 2 * dot(LightVector, N) * N - LightVector;

            float DiffuseAngle = max(dot(LightVector, N), 0.f);
            float Specular = pow(max(dot(R, normalize(ViewVector)), 0.f), SpecularPower) * DiffuseAngle;

            vec3 PixelColour = max(min(AmbientCoeff + DiffuseCoeff * DiffuseAngle + Specular * SpecularCoeff, vec3(1.f)), vec3(0.f));

            FragColor = vec4(vec3(1.f), 1.f);
        }";

        public static readonly string CRV_VERT = @"
        #version 330 core
        layout (location = 0) in vec3 Position;
        layout (location = 1) in vec3 Normal;
        layout (location = 2) in vec3 Colour;
        layout (location = 3) in vec2 uv;

        layout (std140) uniform Transforms
        {
            mat4 World;
            mat4 InvTranspose;
            mat4 View;
            mat4 Projection;
        };
        
        out vec3 Col;

        void main()
        {
            mat4 ModelView = World * View;
            vec3 ViewPos = (ModelView * vec4(Position, 1.f)).xyz;

            gl_Position = Projection * vec4(ViewPos, 1.f); 
            gl_Position.z -= 0.0001f;
            Col = Colour;
        }"; 

        public static readonly string CRV_FRAG = @"
        #version 330 core
        out vec4 FragColor;
        in vec3 Col;

        void main()
        {
            FragColor = vec4(Col, 1.0f);
        }";
    }

    struct TransformBlock 
    {
        public OpenTK.Matrix4 World;
        public OpenTK.Matrix4 InvTranspose;
        public OpenTK.Matrix4 View;
        public OpenTK.Matrix4 Projection;
        public static readonly int
            Size = OpenTK.BlittableValueType<TransformBlock>.Stride;
    }

    struct LightBlock
    {
        public OpenTK.Vector3 LightPos0;
        public OpenTK.Vector3 LightPos1;
        public OpenTK.Vector3 LightPos2;
        public OpenTK.Vector3 LightCol0;
        public OpenTK.Vector3 LightCol1;
        public OpenTK.Vector3 LightCol2;
        public static readonly int
            Size = OpenTK.BlittableValueType<LightBlock>.Stride;

    }

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
        private int zNear = -300,
                    zFar = 300;
        private OpenTK.Matrix4 SavedWorld,
                               ActiveWorld,
                               View,
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
            OpenTK.Matrix4 World = SavedWorld * ActiveWorld;
            TransformBlock Block = new TransformBlock();

            Block.World = World;
            Block.View = View;
            Block.InvTranspose = Block.World.Inverted();
            Block.InvTranspose.Transpose();
            Block.Projection = Projection;

            PerformSyncImpl(Block);
        }

        public OpenTK.Matrix4 GetActiveWorld()
        {
            return ActiveWorld;
        }

        public void SaveActiveWorld()
        {
            SavedWorld = SavedWorld * ActiveWorld;
            ActiveWorld = OpenTK.Matrix4.Identity;
            NeedUpdate = true;
        }

        public void SetView(OpenTK.Matrix4 Mat)
        {
            View = Mat;
            NeedUpdate = true;
        }

        public void SetActiveWorld(OpenTK.Matrix4 Mat)
        {
            ActiveWorld = Mat;
            NeedUpdate = true;
        }

        public void SetRes(int Width, int Height)
        {
            this.Width = Width;
            this.Height = Height;
            this.AspectRatio = (float)Width / Height;
            UpdateProjection();
        }

        private void UpdateProjection()
        {
            Projection = OpenTK.Matrix4.CreateOrthographic(1 * AspectRatio, 1,
                                                           zNear, zFar);
            NeedUpdate = true;
        }

        public void UpdateProjection(OpenTK.Vector3 BBMin, OpenTK.Vector3 BBMax)
        {
            Projection = OpenTK.Matrix4.CreateOrthographicOffCenter( BBMin.X, BBMax.X,
                                                                     BBMin.Y, BBMax.Y,
                                                                     zNear, zFar);
            NeedUpdate = true;
        }



        public void SetZoom(int Direction)
        {
            float Dir;
            if (Direction > 0)
                Dir = 1 / 1.1f;
            else
                Dir = 1.1f;

            Zoom = Zoom * Dir;
            SetActiveWorld(ActiveWorld * OpenTK.Matrix4.CreateScale(Dir));
        }

        public float GetZoom()
        {
            return Zoom;
        }

        public void Reset()
        {
            Zoom = 1;
            ActiveWorld = SavedWorld = OpenTK.Matrix4.Identity;
            SetView(OpenTK.Matrix4.LookAt(new OpenTK.Vector3(0, 0, 1),
                                          new OpenTK.Vector3(0, 0, 0),
                                          new OpenTK.Vector3(0, 0, 1)));
            UpdateProjection();
        }
    }

    public class LightContext : BaseContext
    {
        OpenTK.Vector3[] Positions;
        OpenTK.Vector3[] Colours;

        public LightContext() : base(ShaderSources.LIGHT_BLOCK_NAME, LightBlock.Size, 1)
        {
            Positions = new OpenTK.Vector3[3];
            Colours = new OpenTK.Vector3[3];
        }

        override public void PerformSync()
        {
            if (!NeedUpdate)
                return;
            LightBlock Block = new LightBlock();

            Block.LightPos0 = Positions[0];
            Block.LightPos1 = Positions[1];
            Block.LightPos2 = Positions[2];

            Block.LightCol0 = Colours[0];
            Block.LightCol1 = Colours[1];
            Block.LightCol2 = Colours[2];

            PerformSyncImpl(Block);
        }

        public void SetPos(int Idx, OpenTK.Vector3 Pos)
        {
            Positions[Idx] = Pos;
            NeedUpdate = true;
        }

        public void SetCol(int Idx, OpenTK.Vector3 Col)
        {
            Colours[Idx] = Col;
            NeedUpdate = true;
        }
    }

    public class GlRenderer
    {
        public struct RenderSettings
        {
            public bool FlipNormals;
            public OpenTK.Vector3 DefaultSolidColour;
            public OpenTK.Vector3 DefaultCurveColour;
            public OpenTK.Vector3 BackgroundColour;
            public OpenTK.Vector3 GridColour;
            public bool OverrideColour;
            public int AntiAliasingLevel;
            public bool DisplayGrid;
            public bool BackfaceCulling;
            public bool DisplayNormals;
            public double NearPlane, FarPlane;
            public double NormalLength;
        };

        public static string SerializeRenderSettings(RenderSettings Settings)
        {
            StringBuilder
                sb = new StringBuilder();

            sb.AppendFormat("{0};{1},{2},{3};{4},{5},{6};{7},{8},{9};{10},{11},{12};{13};{14};{15};{16};{17};{18};{19};{20}",
                            Settings.FlipNormals,
                            Settings.DefaultSolidColour.X, Settings.DefaultSolidColour.Y, Settings.DefaultSolidColour.Z,
                            Settings.DefaultCurveColour.X, Settings.DefaultCurveColour.Y, Settings.DefaultCurveColour.Z,
                            Settings.BackgroundColour.X, Settings.BackgroundColour.Y, Settings.BackgroundColour.Z,
                            Settings.GridColour.X, Settings.GridColour.Y, Settings.GridColour.Z,
                            Settings.OverrideColour, Settings.AntiAliasingLevel, Settings.DisplayGrid,
                            Settings.BackfaceCulling, Settings.DisplayNormals, Settings.NearPlane,
                            Settings.FarPlane,
                            Settings.NormalLength);
            return sb.ToString();
        }

        public static RenderSettings DeserializeRenderSettings(string String)
        {
            RenderSettings
                Settings = new RenderSettings();
            String[] Splitted = String.Split(';');

            try
            {
                Settings.FlipNormals = bool.Parse(Splitted[0]);
                Settings.DefaultSolidColour = VecFromStr(Splitted[1]);
                Settings.DefaultCurveColour = VecFromStr(Splitted[2]);
                Settings.BackgroundColour = VecFromStr(Splitted[3]);
                Settings.GridColour = VecFromStr(Splitted[4]);
                Settings.OverrideColour = bool.Parse(Splitted[5]);
                Settings.AntiAliasingLevel = int.Parse(Splitted[6]);
                Settings.DisplayGrid = bool.Parse(Splitted[7]);
                Settings.BackfaceCulling = bool.Parse(Splitted[8]);
                Settings.DisplayNormals = bool.Parse(Splitted[9]);
                Settings.NearPlane = double.Parse(Splitted[10]);
                Settings.FarPlane = double.Parse(Splitted[11]);
                Settings.NormalLength = double.Parse(Splitted[12]);
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
        private bool Loaded;
        private string LastPath;
        private DateTime Time;
        public TransformContext TransCtx;
        public LightContext LightCtx;
        public static RenderSettings DefaultRenderSettings = new RenderSettings()
        {
            FlipNormals = false,
            DefaultSolidColour = new OpenTK.Vector3(1.0f, 0.0f, 0.0f),
            DefaultCurveColour = new OpenTK.Vector3(1.0f, 1.0f, 0.0f),
            BackgroundColour = new OpenTK.Vector3(0.0f, 0.0f, 0.0f),
            GridColour = new OpenTK.Vector3(0.2f, 0.2f, 0.2f),
            OverrideColour = false,
            AntiAliasingLevel = 0,
            DisplayGrid = true,
            BackfaceCulling = false,
            DisplayNormals = true,
            NearPlane = -300,
            FarPlane = 300,
            NormalLength = 1.0f
        };

        public GlRenderer(IntPtr hWnd)
        {

            Info = OpenTK.Platform.Utilities.CreateWindowsWindowInfo(hWnd);
            Context = new GraphicsContext(GraphicsMode.Default,
                            Info, 4, 0, GraphicsContextFlags.Default);
            Context.MakeCurrent(Info);
            Context.LoadAll();

            TransCtx = new TransformContext();
            LightCtx = new LightContext();

            GL.Enable(EnableCap.DepthTest);
            GL.Enable(EnableCap.Blend);
            GL.BlendFunc(BlendingFactor.SrcAlpha, BlendingFactor.OneMinusSrcAlpha);

            ShaderSources.PolyProg = InitShader(ShaderSources.POLY_VERT,
                                                ShaderSources.POLY_FRAG);
            ShaderSources.CrvProg = InitShader(ShaderSources.CRV_VERT,
                                               ShaderSources.CRV_FRAG);
            LightCtx.InitSync(ShaderSources.PolyProg);
            GL.ClearColor(new Color4(0.0f, 0.0f, 0.0f, 0.0f));
            Loaded = false;
            LastPath = null;
            Time = DateTime.Now;

            LightCtx.SetPos(0, new OpenTK.Vector3(10, 10, 10));
            LightCtx.SetCol(0, new OpenTK.Vector3(1, 1, 1));
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
                    SetActiveModel(LastPath);
                    Time = DateTime.Now;
                }
            }

            TransCtx.PerformSync();
            LightCtx.PerformSync();

            GL.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);
            if (Loaded)
            {
                GL.BindVertexArray(ActiveResource.VAO);

                GL.UseProgram(ShaderSources.PolyProg);
                GL.DrawArrays(PrimitiveType.Triangles,
                              ActiveResource.PolygonRecord.Offset,
                              ActiveResource.PolygonRecord.NumElements);

                GL.UseProgram(ShaderSources.CrvProg);
                GL.DrawArrays(PrimitiveType.LineStrip,
                              ActiveResource.PolylineRecord.Offset,
                              ActiveResource.PolylineRecord.NumElements);
            }

            Context.SwapBuffers();
            GL.BindVertexArray(0);
            GL.UseProgram(0);
        }

        public void SetActiveModel(string Filepath)
        {
            try
            {
                this.ActiveResource = I2P.GetResourceManager().GetResource(Filepath);
                Loaded = true;
                LastPath = null;
                TransCtx.Reset();

                if (this.ActiveResource.ContainsView)
                    TransCtx.SetView(this.ActiveResource.ViewMat);
               // TransCtx.UpdateProjection(this.ActiveResource.BBoxMin,
               //                           this.ActiveResource.BBoxMax);
            }
            catch (StillLoadingException)  
            {
                Loaded = false;
                LastPath = Filepath;
            }

        }

        public void Destroy()
        {
            GL.DeleteProgram(ShaderSources.PolyProg);
            GL.DeleteProgram(ShaderSources.CrvProg);
            TransCtx.Destroy();
            LightCtx.Destroy();
        }

    }
}
