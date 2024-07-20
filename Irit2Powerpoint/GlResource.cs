using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OpenTK.Graphics.OpenGL4;
using OpenTK.Graphics;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace Irit2Powerpoint
{

    class GlResource
    {
        public int VAO, VBO, EBO, NumElements, NumVertices;

        public GlResource(string Filepath)
        {
            ITDParser.ITDMesh
                Mesh = ITDParser.Parse(Filepath);

            this.NumElements = Mesh.PolygonMeshSizes[0];
            this.NumVertices = Mesh.Vertecies.Length;

            GL.GenVertexArrays(1, out this.VAO);
            GL.GenBuffers(1, out this.VBO);
            GL.GenBuffers(1, out this.EBO);

            GL.BindVertexArray(this.VAO);
            GL.BindBuffer(BufferTarget.ArrayBuffer, this.VBO);
            GL.BindBuffer(BufferTarget.ElementArrayBuffer, this.EBO);

            GL.BufferData(BufferTarget.ArrayBuffer,
                    this.NumVertices * ITDParser.VERTEX_STRUCT_SIZE,
                    Mesh.Vertecies,
                    BufferUsageHint.StaticDraw);

            GL.BufferData(BufferTarget.ElementArrayBuffer,
                    this.NumElements * sizeof(int),
                    Mesh.PolygonIndiecs,
                    BufferUsageHint.StaticDraw);

            GL.VertexAttribPointer(0,
                3,
                VertexAttribPointerType.Double,
                false,
                ITDParser.VERTEX_STRUCT_SIZE,
                0);

            GL.VertexAttribPointer(1,
                3,
                VertexAttribPointerType.Double,
                false,
                ITDParser.VERTEX_STRUCT_SIZE,
                3 * sizeof(double));

            GL.EnableVertexArrayAttrib(this.VAO, 0);
            GL.EnableVertexArrayAttrib(this.VAO, 1);

            GL.BindBuffer(BufferTarget.ArrayBuffer, 0);
            GL.BindBuffer(BufferTarget.ElementArrayBuffer, 0);
            GL.BindVertexArray(0);
        }

        public void Destroy()
        {
            GL.DeleteVertexArray(VAO);
            GL.DeleteBuffer(EBO);
            GL.DeleteBuffer(VBO);
        }
    }
}
