using System;
using OpenTK.Graphics.OpenGL4;
using OpenTK.Graphics;
using System.Diagnostics;
using System.Threading;


namespace Irit2Powerpoint
{
    struct ShaderSources
    {
        public static int PolyProg, CrvProg, GridProg;

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
    
        layout (std140) uniform Lights
        {
            vec3 LightPos[3];
            vec3 LightCol[3];
        };

        out vec3 Norm;
        
        void main()
        {
            mat4 ModelView = View * World;

            vec3 ViewPos = (ModelView * vec4(Position, 1.f)).xyz;
            vec3 ViewNorm = (InvTranspose * vec4(Normal, 1.f)).xyz;

            gl_Position = Projection * vec4(ViewPos, 1.f); 
            Norm = Colour;
        }"; 
        
        public static readonly string POLY_FRAG = @"
        #version 330 core
        in vec3 Norm;
        in vec3 Diffuse;
        out vec4 FragColor;

        void main()
        {
            FragColor = vec4(Norm, 1.0f);
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
            mat4 ModelView = View * World;
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

        public static readonly string GRID_VERT = @"
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

        out vec4 NearPoint;
        out vec4 FarPoint;

        void main()
        {
            mat4 ModelViewInv = inverse(View * World);
            mat4 ProjectionInv = inverse(Projection);

            NearPoint = ModelViewInv * ProjectionInv * vec4( Position.xy, -1.f, 1.f);
            NearPoint = NearPoint / NearPoint.w;

            FarPoint = ModelViewInv * ProjectionInv * vec4( Position.xy, 1.f, 1.f);
            FarPoint = FarPoint / FarPoint.w;

            gl_Position = vec4(Position, 1.f);
        }"; 

        public static readonly string GRID_FRAG = @"
        #version 330 core
        out vec4 FragColor;
        in vec4 NearPoint, FarPoint;

        layout (std140) uniform Transforms
        {
            mat4 World;
            mat4 InvTranspose;
            mat4 View;
            mat4 Projection;
        };


        vec4 grid(vec3 fragPos3D, float scale, bool drawAxis) {
            vec2 coord = fragPos3D.xz * scale;
            vec2 derivative = fwidth(coord);
            vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
            float line = min(grid.x, grid.y);
            float minimumz = min(derivative.y, 1);
            float minimumx = min(derivative.x, 1);
            vec4 color = vec4(0.2, 0.2, 0.2, 1.0 - min(line, 1.0));
            // z axis
            if(fragPos3D.x > -0.1 * minimumx && fragPos3D.x < 0.1 * minimumx)
                color.z = 1.0;
            // x axis
            if(fragPos3D.z > -0.1 * minimumz && fragPos3D.z < 0.1 * minimumz)
                color.x = 1.0;
            return color;
        }

        float computeDepth(vec3 pos) {

            vec4 clip_space_pos = Projection * View * World * vec4(pos.xyz, 1.0);
            float NDC = clip_space_pos.z / clip_space_pos.w;
            float Far = gl_DepthRange.far;
            float Near = gl_DepthRange.near;
            return (((Far - Near) * NDC) + Near + Far) / 2.0f;
        }
        
        float getLinearDepth(vec3 Pos) {
            vec4 clip_space_pos = Projection * View * World * vec4(Pos.xyz, 1.0);
            float NDC = clip_space_pos.z / clip_space_pos.w;
            return NDC;
        }   

        void main()
        {
            float t = -NearPoint.y / (FarPoint.y - NearPoint.y);
            vec3 fragPos3D = (NearPoint + t * (FarPoint - NearPoint)).xyz;

            vec4 GridCol = grid(fragPos3D, 10, true) + grid(fragPos3D, 1, true);
            float Fade = max(0.5f - getLinearDepth(fragPos3D), 0.f);

            FragColor = GridCol;
            FragColor.a *= Fade;
            gl_FragDepth = computeDepth(fragPos3D);
        }";
    }
   
    struct Constants
    {
        public static readonly string GRID_KEY = "_GRID_";
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

    abstract class BaseContext
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
            /* Upload matrices to GPU. */
            GL.BindBuffer(BufferTarget.UniformBuffer, ubo);
            GL.BufferSubData(BufferTarget.UniformBuffer,
                    IntPtr.Zero,
                    BlockSize,
                    ref Block);
            GL.BindBuffer(BufferTarget.UniformBuffer, 0);
            NeedUpdate = false;
        }

    }

    class TransformContext : BaseContext
    {
        private float Zoom;
        private int Width, Height;
        private int zNear = -300,
                    zFar = 300;
        private OpenTK.Matrix4 SavedWorld,
                               ActiveWorld,
                               View,
                               Projection;

        public TransformContext() : base(ShaderSources.TRANS_BLOCK_NAME, TransformBlock.Size, 0)
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
            UpdateProjection();
        }

        private void UpdateProjection()
        {
            Projection = OpenTK.Matrix4.CreateOrthographic(Width * Zoom, Height * Zoom,
                                                            zNear, zFar);
            NeedUpdate = true;
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
            float Len = (float)Math.Sqrt(10 * 10 + 30 * 30);
            SetView(OpenTK.Matrix4.LookAt(new OpenTK.Vector3(0, 10, 30),
                                          new OpenTK.Vector3(0, 0, 0),
                                          new OpenTK.Vector3(0, 30 / Len, -10 / Len)));
            UpdateProjection();
        }
    }

    class LightContext : BaseContext
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

    class GlRenderer
    {
        private GraphicsContext Context;
        private OpenTK.Platform.IWindowInfo Info;
        private GlResource ActiveResource;
        private bool Loaded;
        private string LastPath;
        private DateTime Time;
        public TransformContext TransCtx;
        public LightContext LightCtx;

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

            CreateGridResource();
            ShaderSources.PolyProg = InitShader(ShaderSources.POLY_VERT,
                                                ShaderSources.POLY_FRAG);
            ShaderSources.CrvProg = InitShader(ShaderSources.CRV_VERT,
                                               ShaderSources.CRV_FRAG);
            ShaderSources.GridProg = InitShader(ShaderSources.GRID_VERT,
                                                ShaderSources.GRID_FRAG);
            LightCtx.InitSync(ShaderSources.PolyProg);
            GL.ClearColor(new Color4(0.0f, 0.0f, 0.0f, 0.0f));
            Loaded = false;
            LastPath = null;
            Time = DateTime.Now;
        }

        private void CreateGridResource()
        {
            int i;
            ITDParser.ITDMesh Grid = new ITDParser.ITDMesh();
            OpenTK.Vector3[] Geometry = new OpenTK.Vector3[6]
            {
                new OpenTK.Vector3(-1, -1, 0),
                new OpenTK.Vector3( 1, -1, 0),
                new OpenTK.Vector3(-1,  1, 0),

                new OpenTK.Vector3( 1, 1, 0),
                new OpenTK.Vector3(-1, 1, 0),
                new OpenTK.Vector3( 1,-1, 0)
            };

            Grid.Vertecies = new ITDParser.VertexStruct[6];
            Grid.PolygonMeshSizes = new int[1];
            Grid.PolylineMeshSizes = Array.Empty<int>();

            Grid.PolygonMeshSizes[0] = 6;

            for (i = 0; i < 6; i++)
            {
                Grid.Vertecies[i].x = Geometry[i].X;
                Grid.Vertecies[i].y = Geometry[i].Y;
                Grid.Vertecies[i].z = Geometry[i].Z;
                Grid.Vertecies[i].nx = 0;
                Grid.Vertecies[i].ny = 0;
                Grid.Vertecies[i].nz = 0;
                Grid.Vertecies[i].r = 0;
                Grid.Vertecies[i].g = 0;
                Grid.Vertecies[i].b = 0;
                Grid.Vertecies[i].u = 0;
                Grid.Vertecies[i].v = 0;
            }

            I2P.GetResourceManager().SetResource("_GRID_", Grid);
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

            /* Draw loading screen here. */

            GL.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);
            if (Loaded)
            {
                GL.BindVertexArray(ActiveResource.VAO);

                GL.UseProgram(ShaderSources.PolyProg);
                foreach (GlMeshRecord Record in ActiveResource.PolygonRecords)
                    GL.DrawArrays(PrimitiveType.Triangles, Record.Offset, Record.NumElements);

                GL.UseProgram(ShaderSources.CrvProg);
                foreach (GlMeshRecord Record in ActiveResource.PolylineRecords)
                    GL.DrawArrays(PrimitiveType.LineStrip, Record.Offset, Record.NumElements);
            }

            GlResource GridResource = I2P.GetResourceManager().GetResource(Constants.GRID_KEY);

            GL.BindVertexArray(GridResource.VAO);
                GL.UseProgram(ShaderSources.GridProg);
            foreach (GlMeshRecord Record in GridResource.PolygonRecords)
                GL.DrawArrays(PrimitiveType.Triangles, Record.Offset, Record.NumElements);

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
            GL.DeleteProgram(ShaderSources.GridProg);
            TransCtx.Destroy();
            LightCtx.Destroy();
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
