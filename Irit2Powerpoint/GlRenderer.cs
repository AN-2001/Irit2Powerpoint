using System;
using OpenTK.Graphics.OpenGL4;
using OpenTK.Graphics;
using System.Drawing;
using System.Drawing.Imaging;
using System.Runtime.CompilerServices;
using System.Collections.Generic;

namespace Irit2Powerpoint
{
    struct ShaderSources
    {
        public static int PolyProg, CrvProg, PostProcessProg;

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

            FragColor = vec4(PixelColour, 1.f);
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
            gl_Position.z += 0.0001f;
            Col = Colour;
        }"; 

        public static readonly string CRV_FRAG = @"
        #version 330 core
        out vec4 FragColor;
        in vec3 Col;
        void main()
        {
            vec3 Colour = Col;
            FragColor = vec4(Colour, 1.0f);
        }";

        public static readonly string POST_PROCESS_VERT = @"
        #version 330 core
        layout (location = 0) in vec2 Position;
        layout (location = 1) in vec2 UV;

        out vec2 UVCoords;

        void main()
        {
            UVCoords = UV;
            gl_Position = vec4(Position, 0, 1);
        }"; 

        public static readonly string POST_PROCESS_FRAG = @"
        #version 330 core

        layout (std140) uniform Transforms
        {
            mat4 ModelView;
            mat4 Inverse;
            mat4 InvTranspose;
            mat4 Projection;
        };

        in vec2 UVCoords;
        out vec4 FragColor;
        uniform sampler2DMS ScreenTex;
        uniform sampler2D BackgroundTex;

        void main()
        {
            ivec2 texel = ivec2(floor(gl_FragCoord.xy));

            vec4 Col0 = vec4(0.0);
            vec4 Col1 = texelFetch( BackgroundTex, texel, 0 );
            float accumAlpha = 0.0;
            int hits = 0;
            
            for (int i = 0; i < 16; i++) {
                vec4 fetched = texelFetch( ScreenTex, texel, i );
                if (fetched.a > 0.5) {
                    Col0 += fetched;
                    hits++;
                }
                accumAlpha += fetched.a;
            }
            
            accumAlpha = accumAlpha / 16.0;
            if (hits > 0)
                Col0 = Col0 / float( hits );
            FragColor = mix( Col1, Col0, accumAlpha );
        }";

        public static void SetUniformInt(int Prog, string varName, int n)
        {
            int Location;

            GL.UseProgram(Prog);

            Location = GL.GetUniformLocation(Prog, varName);
            Logger.GetInstance().Trace($"Uniform location for '{varName}' is {Location}");
            if (Location == -1)
            {
                Logger.GetInstance().Error($"GL Error: Could not locate uniform variable '{varName}'");
                return;
            }

            GL.Uniform1(Location, n);
            GL.UseProgram(0);
            Logger.GetInstance().Trace($"Bound '{varName}' to {n} in {Prog}");
            GlRenderer.DoGlErrorCheck();
        }

        private static void CheckShaderCompileStatus(string ShaderName, int Shader)
        {
            GL.GetShader(Shader, ShaderParameter.CompileStatus, out int success);
            if (success == 0)
            {
                string infoLog = GL.GetShaderInfoLog(Shader);
                Logger.GetInstance().Error($"GL Error: shader '{ShaderName}' compilation failed: {infoLog}");
            }
            Logger.GetInstance().Trace($"Compiled shader for {ShaderName}");
            GlRenderer.DoGlErrorCheck();
        }

        private static void CheckProgramLinkStatus(string ShaderName, int Program)
        {
            GL.GetProgram(Program, GetProgramParameterName.LinkStatus, out int success);
            if (success == 0)
            {
                string InfoLog = GL.GetProgramInfoLog(Program);
                Logger.GetInstance().Error($"GL Error: shader program '{ShaderName}' linking failed: {InfoLog}");
            }
            Logger.GetInstance().Trace($"Completed shader setup for {ShaderName}");
            GlRenderer.DoGlErrorCheck();
        }


        public static int InitShader(string ShaderName, string VertSrc, string FragSrc)
        {
            int VertexShader, FragmentShader, Program;

            VertexShader = GL.CreateShader(ShaderType.VertexShader);
            Logger.GetInstance().Trace($"Vertex shader number for {ShaderName} is {VertexShader}");
            GL.ShaderSource(VertexShader, VertSrc);
            GL.CompileShader(VertexShader);
            CheckShaderCompileStatus(ShaderName, VertexShader);

            FragmentShader = GL.CreateShader(ShaderType.FragmentShader);
            Logger.GetInstance().Trace($"Fragment shader number for {ShaderName} is {FragmentShader}");
            GL.ShaderSource(FragmentShader , FragSrc);
            GL.CompileShader(FragmentShader);
            CheckShaderCompileStatus(ShaderName, FragmentShader);

            Program = GL.CreateProgram();
            Logger.GetInstance().Trace($"Shader Program number for {ShaderName} is {Program}");
            GL.AttachShader(Program, VertexShader);
            GL.AttachShader(Program, FragmentShader);
            GL.LinkProgram(Program);
            CheckProgramLinkStatus(ShaderName, Program);
                
            GL.DeleteShader(VertexShader);
            GL.DeleteShader(FragmentShader);

            GlRenderer.DoGlErrorCheck();
            return Program;
        }
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
        public float a4;
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

            Logger.GetInstance().Trace($"Created uniform buffer object for {UniformName} numbered {ubo}");

            GL.BindBuffer(BufferTarget.UniformBuffer, ubo);
            /* Allocate the struct. */
            GL.BufferData(BufferTarget.UniformBuffer,
                    BlockSize,
                    IntPtr.Zero,
                    BufferUsageHint.StaticDraw);
            GL.BindBufferBase(BufferRangeTarget.UniformBuffer, BindingIndex, ubo);
            GL.BindBuffer(BufferTarget.UniformBuffer, 0);
            NeedUpdate = true;
            GlRenderer.DoGlErrorCheck();
        }

        public void Destroy()
        {
            GL.DeleteBuffer(ubo);
            Logger.GetInstance().Trace($"Destroyed context.");
            GlRenderer.DoGlErrorCheck();
        }

        public void InitSync(int Program)
        {
            int Index = GL.GetUniformBlockIndex(Program, UniformName);
            GL.UniformBlockBinding(Program, Index, BindingIndex);
            Logger.GetInstance().Trace($"Initialized ubo sync for {UniformName} in Program={Program} at BindingIndex={BindingIndex} Program Index={Index}");
            GlRenderer.DoGlErrorCheck();
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

            GlRenderer.DoGlErrorCheck();
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
            Logger.GetInstance().Trace($"Initialized TransformContext: ubo = {this.ubo}");
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
            GlRenderer.DoGlErrorCheck();
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
            GlRenderer.DoGlErrorCheck();
        }

        public void SetProjection(OpenTK.Matrix4 Mat)
        {
            Projection = Mat;
            NeedUpdate = true;
            GlRenderer.DoGlErrorCheck();
        }

        public OpenTK.Matrix4 GetProjection()
        {
            return Projection;
        }

        public void SetActiveModelView(OpenTK.Matrix4 Mat)
        {
            ActiveModelView = Mat;
            NeedUpdate = true;
            GlRenderer.DoGlErrorCheck();
        }

        public void SetRes(int Width, int Height)
        {
            this.Width = Width;
            this.Height = Height;
            this.AspectRatio = (float)Width / Height;
            NeedUpdate = true;
            GlRenderer.DoGlErrorCheck();
            Logger.GetInstance().Trace($"Sat TransformContext res to {Width}, {Height}");
        }

        public void UpdateProjection(OpenTK.Vector3 BBMin, OpenTK.Vector3 BBMax)
        {
            float w = (BBMax.X - BBMin.X);
            float h = (BBMax.Y - BBMin.Y);
            float m = Math.Max(w, h);
            Projection = OpenTK.Matrix4.CreateOrthographic(m * AspectRatio, m, 1000, -1000);
            NeedUpdate = true;
            GlRenderer.DoGlErrorCheck();
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
            GlRenderer.DoGlErrorCheck();
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
            GlRenderer.DoGlErrorCheck();
        }
    }

    public class SettingsContext : BaseContext
    {
        GlRenderer.RenderSettings Settings;

        public SettingsContext() : base(ShaderSources.SETTINGS_BLOCK_NAME, SettingsBlock.Size, 1)
        {
            Reset();
            Logger.GetInstance().Trace($"Initialized SettingsContext: ubo = {this.ubo}");
            GlRenderer.DoGlErrorCheck();
        }
        
        public void SetLight(OpenTK.Vector3 LightPos)
        {
            Settings.LightPosition = LightPos;
            NeedUpdate = true;
            GlRenderer.DoGlErrorCheck();
        }

        public void UpdateSettings(GlRenderer.RenderSettings Settings)
        {
            this.Settings = Settings;
            GL.ClearColor(Settings.BackgroundColour.X, Settings.BackgroundColour.Y, Settings.BackgroundColour.Z, 0.0f);
            NeedUpdate = true;
            GlRenderer.DoGlErrorCheck();
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
            GlRenderer.DoGlErrorCheck();
        }

        public override void Reset()
        {
            Settings = GlRenderer.DefaultRenderSettings;
            GlRenderer.DoGlErrorCheck();
        }

    }

    public class OffScreenManager
    {
        int FBO, BackgroundTexture, ScreenTexture, RBO;
        int Width, Height;

        public OffScreenManager() 
        {
            Width = Height = 400;
            BackgroundTexture = GL.GenTexture();

            ScreenTexture = GL.GenTexture();
            GL.BindTexture(TextureTarget.Texture2DMultisample, ScreenTexture);
            GL.TexImage2DMultisample(TextureTargetMultisample.Texture2DMultisample, 16,
                                    PixelInternalFormat.Rgba8, Width, Height, true);
            GL.BindTexture(TextureTarget.Texture2DMultisample, 0);

            RBO = GL.GenRenderbuffer();
            GL.BindRenderbuffer(RenderbufferTarget.Renderbuffer, RBO);
            GL.RenderbufferStorageMultisample(RenderbufferTarget.Renderbuffer, 16,
                                   RenderbufferStorage.Depth24Stencil8,
                                   Width, Height);
            GL.BindRenderbuffer(RenderbufferTarget.Renderbuffer, 0);


            FBO = GL.GenFramebuffer();
            GL.BindFramebuffer(FramebufferTarget.Framebuffer, FBO);
            GL.FramebufferTexture2D(FramebufferTarget.Framebuffer,
                                    FramebufferAttachment.ColorAttachment0,
                                    TextureTarget.Texture2DMultisample, ScreenTexture, 0);

            GL.FramebufferRenderbuffer(FramebufferTarget.Framebuffer,
                                       FramebufferAttachment.DepthStencilAttachment,
                                       RenderbufferTarget.Renderbuffer, RBO);
            if (GL.CheckFramebufferStatus(FramebufferTarget.Framebuffer) != FramebufferErrorCode.FramebufferComplete)
                Logger.GetInstance().Warn("Gl Error: Framebuffer creation incomplete.");
            GL.BindFramebuffer(FramebufferTarget.Framebuffer, 0);

            Logger.GetInstance().Trace($"Initialized OffScreenManager: FBO = {FBO}, RBO = {RBO}, ScreenTexture = {ScreenTexture}, BackgroundTexture = {BackgroundTexture}");
            GlRenderer.DoGlErrorCheck();
        }

        public void SetSize(int Width, int Height)
        {

            this.Width = Width;
            this.Height = Height;

            GL.BindTexture(TextureTarget.Texture2DMultisample, ScreenTexture);
            GL.TexImage2DMultisample(TextureTargetMultisample.Texture2DMultisample, 16,
                                    PixelInternalFormat.Rgba8, Width, Height, true);
            GL.BindTexture(TextureTarget.Texture2DMultisample, 0);

            GL.BindRenderbuffer(RenderbufferTarget.Renderbuffer, RBO);
            GL.RenderbufferStorageMultisample(RenderbufferTarget.Renderbuffer, 16,
                                   RenderbufferStorage.Depth24Stencil8,
                                   Width, Height);

            GL.BindRenderbuffer(RenderbufferTarget.Renderbuffer, 0);
            Logger.GetInstance().Trace($"Sat offscreen buffer res to {Width}, {Height}");
            GlRenderer.DoGlErrorCheck();
        }

        public void SetBackgroundTexture(Bitmap Bitmap)
        {
            GL.BindTexture(TextureTarget.Texture2D, BackgroundTexture);

            BitmapData
                Data = Bitmap.LockBits(
                    new Rectangle(0, 0, Bitmap.Width, Bitmap.Height),
                    ImageLockMode.ReadOnly,
                    System.Drawing.Imaging.PixelFormat.Format32bppArgb);

            GL.TexImage2D(TextureTarget.Texture2D,
                          0,
                          PixelInternalFormat.Rgba,
                          Data.Width,
                          Data.Height,
                          0,
                          OpenTK.Graphics.OpenGL4.PixelFormat.Bgra,
                          PixelType.UnsignedByte,
                          Data.Scan0);
            Bitmap.UnlockBits(Data);

            GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMinFilter, (int)TextureMinFilter.Nearest);
            GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMagFilter, (int)TextureMagFilter.Nearest);
            GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureWrapS, (int)TextureWrapMode.ClampToEdge);
            GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureWrapT, (int)TextureWrapMode.ClampToEdge);
            GL.BindTexture(TextureTarget.Texture2D, 0);

            Logger.GetInstance().Trace($"Sent slide screenshot to the GPU");

            GlRenderer.DoGlErrorCheck();
        }

        public void setupFirstPass()
        {
            GL.BindFramebuffer(FramebufferTarget.Framebuffer, FBO);
            GL.Viewport(0, 0, Width, Height);
            GL.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);
            GlRenderer.DoGlErrorCheck();
        }

        public void setupSecondPass()
        {
            GL.BindFramebuffer(FramebufferTarget.Framebuffer, 0);
            GL.Viewport(0, 0, Width, Height);
            GL.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);

            GL.ActiveTexture(TextureUnit.Texture0);
            GL.BindTexture(TextureTarget.Texture2DMultisample, ScreenTexture);

            GL.ActiveTexture(TextureUnit.Texture1);
            GL.BindTexture(TextureTarget.Texture2D, BackgroundTexture);
            GlRenderer.DoGlErrorCheck();
        }

        public void FinalizeRender()
        {
            GL.ActiveTexture(TextureUnit.Texture0);
            GL.BindTexture(TextureTarget.Texture2DMultisample, 0);

            GL.ActiveTexture(TextureUnit.Texture1);
            GL.BindTexture(TextureTarget.Texture2D, 0);

            GL.ActiveTexture(TextureUnit.Texture0);
            GlRenderer.DoGlErrorCheck();
        }


        public void Destroy()
        {
            GL.DeleteFramebuffer(FBO);
            GL.DeleteTexture(ScreenTexture);
            GL.DeleteTexture(BackgroundTexture);
            GL.DeleteRenderbuffer(RBO);
            Logger.GetInstance().Trace($"Destroyed OffScreenManager");
            GlRenderer.DoGlErrorCheck();
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

        private IGraphicsContext Context;
        private GlResource ActiveResource, PostProcessResource;
        private RenderSettings ActiveSettings;
        private string LastPath;
        public TransformContext TransCtx;
        public SettingsContext SettingsCtx;
        public OffScreenManager OffScreenManager;

        public static RenderSettings DefaultRenderSettings = new RenderSettings()
        {
            LightPosition = new OpenTK.Vector3(-10.0f, 10.0f, -10.0f),
            LightColour = new OpenTK.Vector3(1.0f, 1.0f, 1.0f),
            DefaultSolidColour = new OpenTK.Vector3(0.7f, 0.4f, 0.2f),
            DefaultCurveColour = new OpenTK.Vector3(0.9f, 0.75f, 0.2f),
            BackgroundColour = new OpenTK.Vector3(0.0f, 0.0f, 0.0f),
        };

        public GlRenderer(IGraphicsContext Context)
        {

            this.Context = Context;

            TransCtx = new TransformContext();
            SettingsCtx = new SettingsContext();

            GL.Disable(EnableCap.Blend);
            GL.Disable(EnableCap.Multisample);

            GL.Enable(EnableCap.DepthTest);
            GL.DepthFunc(DepthFunction.Greater);
            GL.ClearDepth(0.0);

            ShaderSources.PolyProg = ShaderSources.InitShader("POLY_SHADER",
                                                ShaderSources.POLY_VERT,
                                                ShaderSources.POLY_FRAG);
            ShaderSources.CrvProg = ShaderSources.InitShader("CRV_SHADER",
                                               ShaderSources.CRV_VERT,
                                               ShaderSources.CRV_FRAG);
            ShaderSources.PostProcessProg = ShaderSources.InitShader("POSTPROCESSOR_SHADER",
                                                       ShaderSources.POST_PROCESS_VERT,
                                                       ShaderSources.POST_PROCESS_FRAG);

            TransCtx.InitSync(ShaderSources.PolyProg);
            Logger.GetInstance().Trace($"Initialized TransCtx sync for POLY_SHADER");
            TransCtx.InitSync(ShaderSources.CrvProg);

            Logger.GetInstance().Trace($"Initialized TransCtx sync for CRV_SHADER");

            TransCtx.InitSync(ShaderSources.PostProcessProg);
            Logger.GetInstance().Trace($"Initialized TransCtx sync for POSTPROCESS_SHADER");

            SettingsCtx.InitSync(ShaderSources.PolyProg);
            Logger.GetInstance().Trace($"Initialized SettingsCtx sync for POLY_SHADER");

            SettingsCtx.InitSync(ShaderSources.CrvProg);
            Logger.GetInstance().Trace($"Initialized SettingsCtx sync for CRV_SHADER");

            SettingsCtx.UpdateSettings(DefaultRenderSettings);

            ShaderSources.SetUniformInt(ShaderSources.PostProcessProg, "ScreenTex", 0);

            ShaderSources.SetUniformInt(ShaderSources.PostProcessProg, "BackgroundTex", 1);

            OffScreenManager = new OffScreenManager();
            PostProcessResource = new GlResource(new OpenTK.Vector4d[]{
                    new OpenTK.Vector4d(-1,-1, 0, 0),
                    new OpenTK.Vector4d( 1,-1, 1, 0),
                    new OpenTK.Vector4d( 1, 1, 1, 1),
                    new OpenTK.Vector4d(-1,-1, 0, 0),
                    new OpenTK.Vector4d( 1, 1, 1, 1),
                    new OpenTK.Vector4d(-1, 1, 0, 1),
                });

            LastPath = null;
            Logger.GetInstance().Trace("Renderer setup done.");
            DoGlErrorCheck();
        }

        public static void DoGlErrorCheck( [CallerFilePath] string GlFile = "",
                                           [CallerMemberName] string GlFunction = "",
                                           [CallerLineNumber] int GlLine = 0)
        {
            Dictionary<ErrorCode, string> ErrorMessages = new Dictionary<ErrorCode, string>
            {
                { ErrorCode.InvalidEnum, "Invalid enum: An unacceptable value is specified for an enumerated argument." },
                { ErrorCode.InvalidOperation, "Invalid operation: An invalid GL operation was performed."},
                { ErrorCode.InvalidValue, "Invalid value: A numeric argument is out of range." },
                { ErrorCode.OutOfMemory, "Out of memory: There is not enough memory left to execute the command." },
                { ErrorCode.InvalidFramebufferOperation, "Invalid framebuffer operation: The framebuffer is not complete." },
                { ErrorCode.ContextLost, "ContextLost: Lost GL context." },
                { ErrorCode.TableTooLarge, "Table Too Large." }
            };

            ErrorCode Error;
            while ((Error = GL.GetError()) != ErrorCode.NoError)
                Logger.GetInstance().Warn($"Encountered GL Error: {ErrorMessages[Error]}", GlFile, GlFunction, GlLine);
        }

        public void UpdateViewport(int Width, int Height)
        {
            TransCtx.SetRes(Width, Height);
            OffScreenManager.SetSize(Width, Height);
            Logger.GetInstance().Trace($"Updated renderer viewport to Width = {Width}, Height = {Height}");
            DoGlErrorCheck();
        }

        public void UpdateBackground(Bitmap Bitmap)
        {
            OffScreenManager.SetBackgroundTexture(Bitmap);
            Logger.GetInstance().Trace($"Updated renderer background.");
            DoGlErrorCheck();
        }

        public void ForceFetchAndRender()
        {
            bool Fetched = false;
            if (LastPath == null)
                return;
            do
            {
                Fetched = SetActiveModel(LastPath, ActiveSettings);
                Render();
                GL.Flush();
                GL.Finish();
                DoGlErrorCheck();
            }  while (!Fetched);
        }

        public void Render()
        {
            TransCtx.PerformSync();

            SettingsCtx.PerformSync();
            OffScreenManager.setupFirstPass();

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

            OffScreenManager.setupSecondPass();

            GL.UseProgram(ShaderSources.PostProcessProg);

            GL.BindVertexArray(PostProcessResource.VAO);

            /* Draw a quad to cover the entire screen. */
            GL.DrawArrays(PrimitiveType.Triangles, 0, 6);

            OffScreenManager.FinalizeRender();
            GL.BindVertexArray(0);
            GL.UseProgram(0);

            Context.SwapBuffers();
            DoGlErrorCheck();
        }

        public bool SetActiveModel(string Filepath, GlRenderer.RenderSettings Settings)
        {
            LastPath = Filepath;
            ActiveSettings = Settings;

            try
            {
                ActiveResource = I2P.GetResourceManager().GetResource(Filepath);

                TransCtx.Reset();
                SettingsCtx.Reset();

                SettingsCtx.UpdateSettings(Settings);

                SettingsCtx.SetLight(ActiveResource.LightPos);
                if (this.ActiveResource.ContainsView) {
                    TransCtx.SetActiveModelView(this.ActiveResource.ViewMat);
                    TransCtx.SaveActiveModelView();
                }

                if (this.ActiveResource.ContainsProj)
                    TransCtx.SetProjection(this.ActiveResource.ProjMat);
                else
                    TransCtx.UpdateProjection(this.ActiveResource.BBoxMin,
                                              this.ActiveResource.BBoxMax);

                Logger.GetInstance().Trace($"Fetched renderer resource at {Filepath}");
                DoGlErrorCheck();
                return true;
            }
            catch (StillLoadingException)  
            {
                return false;
            }
        }

        public void Destroy()
        {
            GL.DeleteProgram(ShaderSources.PolyProg);
            GL.DeleteProgram(ShaderSources.CrvProg);
            GL.DeleteProgram(ShaderSources.PostProcessProg);
            TransCtx.Destroy();
            SettingsCtx.Destroy();
            OffScreenManager.Destroy();
            Logger.GetInstance().Trace($"Destroyed renderer.");
            DoGlErrorCheck();
        }

    }
}
