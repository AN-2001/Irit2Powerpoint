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
    public class GlMeshRecord
    {
        public int Offset;
        public int NumElements;
        public GlMeshRecord(int Offset, int NumElements)
        {
            this.Offset = Offset;
            this.NumElements = NumElements;
        }
    }

    public class GlResource
    {
        public int VAO, VBO;

        /* Mesh records, stores offsets + vertex count. */
        public List<GlMeshRecord> PolygonRecords, PolylineRecords;

        public GlResource(ITDParser.ITDMesh Mesh)
        {
            int 
                NumVertices = Mesh.Vertecies.Length;

            GenRecords(Mesh);
            GL.GenVertexArrays(1, out this.VAO);
            GL.GenBuffers(1, out this.VBO);

            GL.BindVertexArray(this.VAO);
            GL.BindBuffer(BufferTarget.ArrayBuffer, this.VBO);

            GL.BufferData(BufferTarget.ArrayBuffer,
                    NumVertices * ITDParser.VERTEX_STRUCT_SIZE,
                    Mesh.Vertecies,
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

            GL.VertexAttribPointer(2,
                3,
                VertexAttribPointerType.Double,
                false,
                ITDParser.VERTEX_STRUCT_SIZE,
                6 * sizeof(double));

            GL.VertexAttribPointer(3,
                2,
                VertexAttribPointerType.Double,
                false,
                ITDParser.VERTEX_STRUCT_SIZE,
                9 * sizeof(double));

            GL.EnableVertexArrayAttrib(this.VAO, 0);
            GL.EnableVertexArrayAttrib(this.VAO, 1);
            GL.EnableVertexArrayAttrib(this.VAO, 2);
            GL.EnableVertexArrayAttrib(this.VAO, 3);

            GL.BindBuffer(BufferTarget.ArrayBuffer, 0);
            GL.BindVertexArray(0);
        }

        private void GenRecords(ITDParser.ITDMesh Mesh)
        {
            int 
                Offset = 0;

            this.PolygonRecords = new List<GlMeshRecord>();
            foreach(int Size in Mesh.PolygonMeshSizes)
            {
                this.PolygonRecords.Add(new GlMeshRecord(Offset, Size));
                Offset = Offset + Size;
            }

            this.PolylineRecords = new List<GlMeshRecord>();
            foreach(int Size in Mesh.PolylineMeshSizes)
            {
                this.PolylineRecords.Add(new GlMeshRecord(Offset, Size));
                Offset = Offset + Size;
            }

        }

        public void Destroy()
        {
            GL.DeleteVertexArray(VAO);
            GL.DeleteBuffer(VBO);
        }
    }
}
