using System;
using OpenTK.Graphics.OpenGL4;
using OpenTK.Graphics;
using System.Drawing;
using System.Drawing.Imaging;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace Irit2Powerpoint
{
    struct ShaderSources
    {
        public static int PolyProg, CrvProg, PostProcessProg;

        public static readonly string TRANS_BLOCK_NAME = "Transforms";
        public static readonly string SETTINGS_BLOCK_NAME = "Settings";
        public static readonly string LIGHTS_BLOCK_NAME = "Lights";
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
            mat4 AspectRatioCorrector;
        };

        out vec3 Norm;
        out vec3 Pos;
        out vec3 VertColour;
        
        void main()
        {
            vec3 ViewPos = (ModelView * vec4(Position, 1.f)).xyz;
            vec3 ViewNorm = (InvTranspose * vec4(Normal, 1.f)).xyz;
            vec4 ClipSpacePos = Projection * AspectRatioCorrector * vec4(ViewPos, 1.f);

            gl_Position = ClipSpacePos;

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
            mat4 AspectRatioCorrector;
        };

        struct Light {
            int IsEnabled;
            vec3 Position;
            vec3 Ambient;
            vec3 Diffuse;
            vec3 Specular;
        };

        layout (std140) uniform Lights
        {
            Light LightsArr[32];
        };

        void main()
        {
            int i;
            vec3 AccumColour = vec3(0.f);
            vec3 CameraPos = (Inverse * vec4(vec3(0.f), 1.f)).xyz;
            float SpecularPower = 4.f;
            vec3 N = normalize(Norm);
            vec3 ViewVector = normalize(CameraPos - Pos);

            for (i = 0; i < 32; i++) {
                if (LightsArr[i].IsEnabled != 1)
                    break;
                vec3 DiffuseCoeff = VertColour * LightsArr[i].Diffuse;
                vec3 AmbientCoeff = VertColour * LightsArr[i].Ambient;
                vec3 LightWorld = (ModelView * vec4(LightsArr[i].Position, 1.f)).xyz;
                vec3 LightVector = normalize(LightWorld - Pos);
                vec3 R = reflect(-LightVector, N);
                float DiffuseAngle = max(dot(LightVector, N), 0.f);
                float Specular = pow(max(dot(R, normalize(ViewVector)), 0.f), SpecularPower) * DiffuseAngle;

                AccumColour += AmbientCoeff + DiffuseCoeff * DiffuseAngle + Specular * LightsArr[i].Specular;
            }

            AccumColour = max(min( AccumColour, vec3(1.f)), vec3(0.f));

            FragColor = vec4(AccumColour, 1.f);
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
            mat4 AspectRatioCorrector;
        };

        layout (std140) uniform Settings
        {
            vec3 ZOffset;
        };
        
        out vec3 Col;

        void main()
        {
            vec3 ViewPos = (ModelView * vec4(Position, 1.f)).xyz;
            vec4 ClipSpacePos = Projection * AspectRatioCorrector * vec4(ViewPos, 1.f);
            ClipSpacePos.z += ZOffset.x;

            gl_Position = ClipSpacePos;
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

        in vec2 UVCoords;
        out vec4 FragColor;
        uniform sampler2DMS ScreenTex;
        uniform sampler2D BackgroundTex;

        void main()
        {
            ivec2 texel = ivec2(floor(gl_FragCoord.xy));

            vec4 Col0 = vec4(0.0);
            vec4 Col1 = texelFetch( BackgroundTex, texel, 0 );
            vec3 Gamma = vec3(2.2);
            vec3 UnGamma = vec3(1.0 / 2.2);
            

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

            Col0.rgb = pow( Col0.rgb, Gamma );
            Col1.rgb = pow( Col1.rgb, Gamma );

            FragColor = mix( Col1, Col0, accumAlpha );
            FragColor.rgb = pow( FragColor.rgb, UnGamma );
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

    [StructLayout(LayoutKind.Sequential)]
    struct TransformBlock 
    {
        public OpenTK.Matrix4 ModelView;
        public OpenTK.Matrix4 Inverse;
        public OpenTK.Matrix4 InvTranspose;
        public OpenTK.Matrix4 Projection;
        public OpenTK.Matrix4 AspectRatioCorrector;
        public static readonly int
            Size = OpenTK.BlittableValueType<TransformBlock>.Stride;
    }

#pragma warning disable CS0649

    /* DONT REMOVE a1, a2, a3, ... they're for alllignment on GPU memory. */
    [StructLayout(LayoutKind.Sequential)]
    struct SettingsBlock
    {
        public OpenTK.Vector3 ZOffset;
        public float a1;

        public static readonly int
            Size = OpenTK.BlittableValueType<SettingsBlock>.Stride;
    }

    /* Stored as an array on the GPU. */
    [StructLayout(LayoutKind.Sequential)]
    public struct LightBlock
    {
        public int IsEnabled;
        public OpenTK.Vector3 a0;

        public OpenTK.Vector3 Position;
        public float a1;

        public OpenTK.Vector3 Ambient;
        public float a2;

        public OpenTK.Vector3 Diffuse;
        public float a3;

        public OpenTK.Vector3 Specular;
        public float a4;

        public static readonly int
            Size = OpenTK.BlittableValueType<LightBlock>.Stride;
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
        virtual public void Reset()
        {
            GlRenderer.DoGlErrorCheck();
        }

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

        protected void PerformSyncImpl<T>(T[] Block) where T : struct
        {
            /* Upload block to GPU. */
            GL.BindBuffer(BufferTarget.UniformBuffer, ubo);
            var handle = System.Runtime.InteropServices.GCHandle.Alloc(Block,
                                System.Runtime.InteropServices.GCHandleType.Pinned);
            try
            {
                IntPtr Ptr = handle.AddrOfPinnedObject();
                GL.BufferSubData(BufferTarget.UniformBuffer,
                        IntPtr.Zero,
                        BlockSize,
                        Ptr);
            } 
            finally
            {
                handle.Free();
            }

            GL.BindBuffer(BufferTarget.UniformBuffer, 0);
            NeedUpdate = false;

            GlRenderer.DoGlErrorCheck();
        }

    }

    public class TransformContext : BaseContext
    {
        private float Zoom;
        private int Width, Height;
        private OpenTK.Matrix4 SavedModelView,
                               ActiveModelView,
                               AspectRatioCorrector,
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
            Block.AspectRatioCorrector = AspectRatioCorrector;

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

        public OpenTK.Matrix4 CalculateAspectRatioCorrector(int Width, int Height)
        {
            OpenTK.Matrix4 Ret;
            float Ratio;

            if (Width < Height)
            {
                Ratio = (float)Width / Height;
                Ret = new OpenTK.Matrix4(1, 0, 0, 0,
                                         0, Ratio, 0, 0,
                                         0, 0, 1, 0,
                                         0, 0, 0, 1);
            }
            else
            {
                Ratio = (float)Height / Width;
                Ret = new OpenTK.Matrix4(Ratio, 0, 0, 0,
                                         0, 1, 0, 0,
                                         0, 0, 1, 0,
                                         0, 0, 0, 1);
            }

            Logger.GetInstance().Trace($"Calculated aspect ratio corrector: Width = {Width}, Height = {Height}, Ratio = {Ratio}");

            return Ret;
        }

        public void SetRes(int Width, int Height)
        {
            this.Width = Width;
            this.Height = Height;

            this.AspectRatioCorrector = CalculateAspectRatioCorrector(Width, Height);

            NeedUpdate = true;
            GlRenderer.DoGlErrorCheck();
            Logger.GetInstance().Trace($"Sat TransformContext res to {Width}, {Height}");
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

        public string getMatricesString()
        {
            OpenTK.Matrix4 ModelView = SavedModelView * ActiveModelView;
            return 
$@"[OBJECT MATRICES
    [OBJECT VIEW_MAT
        [MATRIX
            {ModelView.M11}  {ModelView.M12} {ModelView.M13}  {ModelView.M14}
            {ModelView.M21}  {ModelView.M22} {ModelView.M23}  {ModelView.M24}
            {ModelView.M31}  {ModelView.M32} {ModelView.M33}  {ModelView.M34}
            {ModelView.M41}  {ModelView.M42} {ModelView.M43}  {ModelView.M44}
        ]
    ]
    [OBJECT PRSP_MAT
        [MATRIX
            {Projection.M11}  {Projection.M12} {Projection.M13}  {Projection.M14}
            {Projection.M21}  {Projection.M22} {Projection.M23}  {Projection.M24}
            {Projection.M31}  {Projection.M32} {Projection.M33}  {Projection.M34}
            {Projection.M41}  {Projection.M42} {Projection.M43}  {Projection.M44}
        ]
    ]
]";
        }

        public OpenTK.Matrix4 GetAspectRatioCorrector()
        {
            return this.AspectRatioCorrector;
        }

        public override void Reset()
        {
            Zoom = 1;
            SavedModelView = OpenTK.Matrix4.Identity;
            GlRenderer.DoGlErrorCheck();
        }
    }

    public class SettingsContext : BaseContext
    {
        private float ZOffset;

        public SettingsContext() : base(ShaderSources.SETTINGS_BLOCK_NAME, SettingsBlock.Size, 1)
        {
            Reset();
            Logger.GetInstance().Trace($"Initialized SettingsContext: ubo = {this.ubo}");
            GlRenderer.DoGlErrorCheck();
        }

        public void SetZOffset(float Offset)
        {
            this.ZOffset = Offset;
            NeedUpdate = true;
            GlRenderer.DoGlErrorCheck();
        }

        override public void PerformSync()
        {
            if (!NeedUpdate)
                return;
            SettingsBlock Block = new SettingsBlock();

            Block.ZOffset = new OpenTK.Vector3(this.ZOffset);

            PerformSyncImpl(Block);
            GlRenderer.DoGlErrorCheck();
        }
    }

    public class LightContext : BaseContext
    {
        private LightBlock []Lights;

        public LightContext() : base(ShaderSources.LIGHTS_BLOCK_NAME, LightBlock.Size * ITDParser.I2P_NUM_LIGHTS, 2)
        {
            Reset();
            Logger.GetInstance().Trace($"Initialized LightContext: ubo = {this.ubo}");
            GlRenderer.DoGlErrorCheck();
        }

        public void SetLights(LightBlock []Lights)
        {
            this.Lights = (LightBlock[])Lights.Clone();
            NeedUpdate = true;
            GlRenderer.DoGlErrorCheck();
        }

        override public void PerformSync()
        {
            if (!NeedUpdate)
                return;
            PerformSyncImpl(Lights);
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
        private IGraphicsContext Context;
        private GlResource ActiveResource, PostProcessResource;
        private string LastPath;
        public TransformContext TransCtx;
        public SettingsContext SettingsCtx;
        public LightContext LightCtx;
        public OffScreenManager OffScreenManager;

        public GlRenderer(IGraphicsContext Context)
        {

            this.Context = Context;

            TransCtx = new TransformContext();
            SettingsCtx = new SettingsContext();
            LightCtx = new LightContext();

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

            SettingsCtx.InitSync(ShaderSources.CrvProg);
            Logger.GetInstance().Trace($"Initialized SettingsCtx sync for CRV_SHADER");

            LightCtx.InitSync(ShaderSources.PolyProg);
            Logger.GetInstance().Trace($"Initialized LightCtx sync for POLY_SHADER");

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
                Fetched = SetActiveModel(LastPath);
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
            LightCtx.PerformSync();

            OffScreenManager.setupFirstPass();

            if (ActiveResource != null)
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

        public bool SetActiveModel(string Filepath)
        {
            LastPath = Filepath;

            try
            {
                ActiveResource = I2P.GetResourceManager().GetResource(Filepath);

                TransCtx.Reset();
                SettingsCtx.Reset();

                if (ActiveResource != null)
                {

                    TransCtx.SetActiveModelView(this.ActiveResource.ViewMat);
                    TransCtx.SaveActiveModelView();
                    TransCtx.SetProjection(this.ActiveResource.ProjMat);

                    SettingsCtx.SetZOffset(ActiveResource.ZOffset);

                    LightCtx.SetLights(ActiveResource.Lights);

                    Logger.GetInstance().Trace($"Fetched renderer resource at {Filepath}");
                } else
                    Logger.GetInstance().Warn($"Detected failed mesh import, rendering nothing.");
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
