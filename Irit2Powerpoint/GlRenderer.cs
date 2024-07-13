using System;
using OpenTK.Graphics.OpenGL4;
using OpenTK.Graphics;


namespace Irit2Powerpoint
{
    class GlRenderer
    {
        private GraphicsContext Context;
        private OpenTK.Platform.IWindowInfo Info;

        private uint VAO;
        private uint VBO;


        public GlRenderer(IntPtr hWnd)
        {

            Info = OpenTK.Platform.Utilities.CreateWindowsWindowInfo(hWnd);
            Context = new GraphicsContext(GraphicsMode.Default, Info, 4, 0, GraphicsContextFlags.Default);
            Context.MakeCurrent(Info);
            Context.LoadAll();
            ///////////////////////////////////////////////// // this here for testing purposes ////////////////////////////////////////////////////////////

            string vertexShaderSource = @"
            #version 330 core
            layout (location = 0) in vec3 aPos;
            void main()
            {
                gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
            }"; 
            
            string fragmentShaderSource = @"
            #version 330 core
            out vec4 FragColor;

            void main()
            {
                FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
            }";

            float[] vertices ={
                -0.5f , -0.5f , 0.0f,
                0.5f , -0.5f, 0.0f,
                0.0f, 0.5f , 0.0f
            };

            int vertexShader = GL.CreateShader(ShaderType.VertexShader);
            GL.ShaderSource(vertexShader, vertexShaderSource);
            GL.CompileShader(vertexShader);
            CheckShaderCompileStatus(vertexShader);

            int fragmentShader  = GL.CreateShader(ShaderType.FragmentShader);
            GL.ShaderSource( fragmentShader , fragmentShaderSource);
            GL.CompileShader(fragmentShader);
            CheckShaderCompileStatus(fragmentShader);

            int shaderProgram = GL.CreateProgram();
            GL.AttachShader(shaderProgram, vertexShader);
            GL.AttachShader(shaderProgram, fragmentShader);
            GL.LinkProgram(shaderProgram);
            CheckProgramLinkStatus(shaderProgram);
                
            GL.DeleteShader(vertexShader);
            GL.DeleteShader(fragmentShader);

            GL.GenBuffers(1, out VBO);
            GL.BindBuffer(BufferTarget.ArrayBuffer, VBO);
            GL.BufferData(BufferTarget.ArrayBuffer, vertices.Length * sizeof(float), vertices, BufferUsageHint.StaticDraw);

            GL.GenVertexArrays(1, out VAO);
            GL.BindVertexArray(VAO);
            GL.VertexAttribPointer(0, 3, VertexAttribPointerType.Float, false, 3 * sizeof(float), 0);
            GL.EnableVertexAttribArray(0);

            GL.BindVertexArray(0);
            GL.BindBuffer(BufferTarget.ArrayBuffer, 0);

            GL.ClearColor(new Color4(0.2f, 0.3f, 0.3f, 1.0f));
            GL.UseProgram(shaderProgram);
            Render();
            //  GL.DeleteVertexArray(VAO);
            //  GL.DeleteBuffer(VBO);
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
            GL.ClearColor(new Color4(0.2f, 0.3f, 0.3f, 1.0f));
            GL.BindVertexArray(VAO);
            GL.Clear(ClearBufferMask.ColorBufferBit);
            GL.DrawArrays(PrimitiveType.Triangles, 0, 3);
            Context.SwapBuffers();
        }
    }
}
