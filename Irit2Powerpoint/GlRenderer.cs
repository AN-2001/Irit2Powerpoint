
using System;
using OpenTK.Graphics.OpenGL4;
using OpenTK.Graphics;
using System.Diagnostics;
using OpenTK;

//namespace Irit2Powerpoint
//{
//  public class Camera
//{
//  public Vector3 camera_position {get; set;}
// public Vector3 camera_up{get; set;}
// public Vector3 camera_right { get; set; }
//public Vector3 worldUp { get; set; }
// public Vector3 front { get; set; }
// public float camera_roll apperantly not used ;
//public float camera_pitch {get; set;}
//public float camera_yaw {get; set;}
//public float camera_speed{get; set;} 
//public float camera_sensitivity { get; set; }   

//public float zoom { get; set; } 

//public Camera(Vector3 position , Vector3 up , float yaw ,float pitch)
//{
//  camera_position = position; // maybe not needed and just init to 000? 
//camera_pitch = pitch;
//camera_up = up;
//camera_yaw = yaw;
//camera_front = new Vector3 
// return;
//}
//}
//class GlRenderer
//    {
//      private string vertexShaderSource = @"
//       #version 330 core
///      layout (location = 0) in vec3 Pos;
//    layout (location = 1) in vec3 Normal;

//  out vec3 Norm;

//  void main()
// {
//   gl_Position = vec4(Pos * 0.25f, 1.f);
//  Norm = Normal;
//}"; 

//private string fragmentShaderSource = @"
//#version 330 core
//in vec3 Norm;
//out vec4 FragColor;

//void main()
//{
//  FragColor = vec4(vec3(1.0f), 1.0f);
//}";

//private int MainProgram;

//private GraphicsContext Context;
//private OpenTK.Platform.IWindowInfo Info;
//private GlResourceManager ResourceManager;
//private GlResource ActiveResource;
//private bool Loaded;

// public GlRenderer(IntPtr hWnd)
//{

//  Info = OpenTK.Platform.Utilities.CreateWindowsWindowInfo(hWnd);
// Context = new GraphicsContext(GraphicsMode.Default,
//              Info, 4, 0, GraphicsContextFlags.Default);
//Context.MakeCurrent(Info);
//Context.LoadAll();

//GL.Enable(EnableCap.DepthTest);
//GL.Enable(EnableCap.CullFace);
//GL.CullFace(CullFaceMode.Front);
//ResourceManager = new GlResourceManager();
//InitShader();

//GL.ClearColor(new Color4(0.0f, 0.0f, 0.0f, 0.0f));
//Loaded = false;
//}

/// private void InitShader()
//{
//  int vertexShader, fragmentShader;

//vertexShader = GL.CreateShader(ShaderType.VertexShader);
//GL.ShaderSource(vertexShader, vertexShaderSource);
//GL.CompileShader(vertexShader);
//CheckShaderCompileStatus(vertexShader);

//fragmentShader = GL.CreateShader(ShaderType.FragmentShader);
//GL.ShaderSource(fragmentShader , fragmentShaderSource);
//GL.CompileShader(fragmentShader);
//CheckShaderCompileStatus(fragmentShader);

//MainProgram = GL.CreateProgram();
//GL.AttachShader(MainProgram, vertexShader);
//GL.AttachShader(MainProgram, fragmentShader);
//GL.LinkProgram(MainProgram);
//CheckProgramLinkStatus(MainProgram);

//GL.DeleteShader(vertexShader);
//GL.DeleteShader(fragmentShader);

//}

//public void UpdateViewport(int Width, int Height)
//{
//  System.Drawing.Point
//    P = new System.Drawing.Point(0, 0),
//  Size = new System.Drawing.Point(Width, Height);
//System.Drawing.Size
//   S = new System.Drawing.Size(Size);
//System.Drawing.Rectangle
//   R = new System.Drawing.Rectangle(P, S);
//GL.Viewport(R);
//}

// private void CheckShaderCompileStatus(int shader)
//{
//  GL.GetShader(shader, ShaderParameter.CompileStatus, out int success);
// if (success == 0)
//{
// string infoLog = GL.GetShaderInfoLog(shader);
//throw new Exception($"Shader compilation failed ({shader}): {infoLog}");
//}
//}

// private void CheckProgramLinkStatus(int program)
//{
//  GL.GetProgram(program, GetProgramParameterName.LinkStatus, out int success);
// if (success == 0)
//{
//   string infoLog = GL.GetProgramInfoLog(program);
// throw new Exception($"Program linking failed: {infoLog}");
//}
//}

//  public void Render()
// {
/* OpenTK.Matrix4 Mat = OpenTK.Matrix4.CreateTranslation(new OpenTK.Vector3(1, 0, 0)); */
/* OpenTK.Matrix4.Create*(args) */

/*   GL.UseProgram(MainProgram);
   GL.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);
   //if (Loaded)
   //{
   //    GL.BindVertexArray(ActiveResource.VAO);
       //foreach (GlMeshRecord Record in ActiveResource.PolygonRecords)
         //  GL.DrawArrays(PrimitiveType.Triangles, Record.Offset, Record.NumElements);
       //foreach (GlMeshRecord Record in ActiveResource.PolylineRecords)
     //      GL.DrawArrays(PrimitiveType.Lines, Record.Offset, Record.NumElements);
   //}

  // Context.SwapBuffers();
//   GL.BindVertexArray(0);
//}

//public void SetActiveModel(string Filepath)
//{
   //this.ActiveResource = ResourceManager.GetResource(Filepath);
 //  Loaded = true;
//}

//public void Destroy()
//{
//    ResourceManager.Destroy();
//  }

//    public static string GetErrorString(ErrorCode errorCode)
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

//  }
//}
*/
//using System;
//using OpenTK.Graphics.OpenGL4;
//using OpenTK.Graphics;
//using OpenTK.Mathematics;  // Ensure OpenTK.Mathematics is imported for vector and matrix operations

namespace Irit2Powerpoint
{
    public class Camera
    {
        public Vector3 Position { get; set; }
        public Vector3 Front { get; private set; }
        public Vector3 Up { get; private set; }
        public Vector3 Right { get; private set; }
        public Vector3 WorldUp { get; private set; }

        public float Yaw { get; set; }
        public float Pitch { get; set; }
        public float MovementSpeed { get; set; }
        public float MouseSensitivity { get; set; }
        public float Zoom { get; set;}

        public float XPos;
        public float YPos;

        public Camera(Vector3 position, Vector3 up, float yaw, float pitch)
        {
            Position = position;
            WorldUp = up;
            Yaw = yaw;
            Pitch = pitch;
            MovementSpeed = 2.5f;
            MouseSensitivity = 0.1f;
            Zoom = 45.0f;
            XPos = 0.0f;
            YPos = 0.0f;
            UpdateCameraVectors();
        }

        public Matrix4 GetViewMatrix()
        {
            return Matrix4.LookAt(Position, Position + Front, Up);
        }

        public void ProcessKeyboard(string direction, float deltaTime)
        {
            float velocity = MovementSpeed * deltaTime;
            if (direction == "FORWARD")
                Position += Front * velocity;
            if (direction == "BACKWARD")
                Position -= Front * velocity;
            if (direction == "LEFT")
                Position -= Right * velocity;
            if (direction == "RIGHT")
                Position += Right * velocity
                 ;
            // might want to add to this to include more options
        }

        public void ProcessMouseMovement(float currentMouseX, float currentMouseY, bool constrainPitch = true)
        {
            // Calculate the offset
            float xoffset = (currentMouseX - XPos) * MouseSensitivity;
            float yoffset = (currentMouseY - YPos) * MouseSensitivity;

            // Update the last mouse position for the next frame
            XPos = currentMouseX;
            YPos = currentMouseY;

            // Update yaw and pitch
            Yaw += xoffset;
            Pitch += yoffset;

            // Constrain the pitch
            if (constrainPitch)
            {
                if (Pitch > 89.0f)
                    Pitch = 89.0f;
                if (Pitch < -89.0f)
                    Pitch = -89.0f;
            }

            // Update the camera vectors
            UpdateCameraVectors();
        }

        public void ProcessMouseScroll(float yoffset)
        {
            Zoom -= yoffset > 0 ? 1 : -1;
            if (Zoom < 1.0f)
                Zoom = 1.0f;
            if (Zoom > 45.0f)
                Zoom = 45.0f;
        }

        private void UpdateCameraVectors()
        {
            // Calculate the new Front vector
            Vector3 front;
            front.X = (float)Math.Cos(MathHelper.DegreesToRadians(Yaw)) * (float)Math.Cos(MathHelper.DegreesToRadians(Pitch));
            front.Y = (float)Math.Sin(MathHelper.DegreesToRadians(Pitch));
            front.Z = (float)Math.Sin(MathHelper.DegreesToRadians(Yaw)) * (float)Math.Cos(MathHelper.DegreesToRadians(Pitch));
            Front = Vector3.Normalize(front);

            // Recalculate the Right and Up vector
            Right = Vector3.Normalize(Vector3.Cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
            Up = Vector3.Normalize(Vector3.Cross(Right, Front));
        }

    }

    class GlRenderer
    {
        private string vertexShaderSource = @"
        #version 330 core
        layout (location = 0) in vec3 Pos;
        layout (location = 1) in vec3 Normal;

        out vec3 Norm;
        
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main()
        {
            gl_Position = projection * view * model * vec4(Pos, 1.0);
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
        private Camera camera;

        public GlRenderer(IntPtr hWnd)
        {
            Info = OpenTK.Platform.Utilities.CreateWindowsWindowInfo(hWnd);
            Context = new GraphicsContext(GraphicsMode.Default, Info, 4, 0, GraphicsContextFlags.Default);
            Context.MakeCurrent(Info);
            Context.LoadAll();

            GL.Enable(EnableCap.DepthTest);
            GL.Enable(EnableCap.CullFace);
            GL.CullFace(CullFaceMode.Front);
            ResourceManager = new GlResourceManager();
            InitShader();
            GL.ClearColor(new Color4(0.0f, 0.0f, 0.0f, 0.0f));
            Loaded = false;

            // Initialize the camera
            camera = new Camera(new Vector3(0.0f, 0.0f, 3.0f), new Vector3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
        }

        private void InitShader()
        {
            int vertexShader, fragmentShader;

            vertexShader = GL.CreateShader(ShaderType.VertexShader);
            GL.ShaderSource(vertexShader, vertexShaderSource);
            GL.CompileShader(vertexShader);
            CheckShaderCompileStatus(vertexShader);

            fragmentShader = GL.CreateShader(ShaderType.FragmentShader);
            GL.ShaderSource(fragmentShader, fragmentShaderSource);
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
            GL.Viewport(0, 0, Width, Height);
        }
        public void OnMouseMove(float mouseX, float mouseY)
        {
            camera.ProcessMouseMovement(mouseX, mouseY);
        }
        public void OnMouseWheel(int Wheel)
        {
            camera.ProcessMouseScroll(Wheel);
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
                Matrix4 model = Matrix4.Identity;  // Model transformation (local to world space)
                Matrix4 view = camera.GetViewMatrix();  // View transformation (world to camera space)
                Matrix4 projection = Matrix4.CreatePerspectiveFieldOfView(MathHelper.DegreesToRadians(camera.Zoom), 800f / 600f, 0.1f, 100.0f);  // Projection transformation (camera to clip space)

                GL.UniformMatrix4(GL.GetUniformLocation(MainProgram, "model"), false, ref model);
                GL.UniformMatrix4(GL.GetUniformLocation(MainProgram, "view"), false, ref view);
                GL.UniformMatrix4(GL.GetUniformLocation(MainProgram, "projection"), false, ref projection);

                GL.BindVertexArray(ActiveResource.VAO);
                foreach (GlMeshRecord Record in ActiveResource.PolygonRecords)
                    GL.DrawArrays(PrimitiveType.Triangles, Record.Offset, Record.NumElements);
                foreach (GlMeshRecord Record in ActiveResource.PolylineRecords)
                    GL.DrawArrays(PrimitiveType.Lines, Record.Offset, Record.NumElements);
            }

            Context.SwapBuffers();
            GL.BindVertexArray(0);
        }

        public void SetActiveModel(string Filepath)
        {
            ActiveResource = ResourceManager.GetResource(Filepath);
            Loaded = true;
        }

        public void Destroy()
        {
            ResourceManager.Destroy();
        }

        /*  public static string GetErrorString(ErrorCode errorCode)
 {
     return errorCode switch
     {
         ErrorCode.InvalidEnum => "Invalid Enum",
         ErrorCode.InvalidValue => "Invalid Value",
         ErrorCode.InvalidOperation => "Invalid Operation",
         ErrorCode.OutOfMemory => "Out of Memory",
         ErrorCode.TableTooLarge => "Table Too Large",
         ErrorCode.NoError => "No Error",
         _ => $"Unknown Error ({errorCode})",
     };
 }*/
    }
}

