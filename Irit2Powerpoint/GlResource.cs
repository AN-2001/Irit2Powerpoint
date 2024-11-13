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
        public OpenTK.Matrix4 ViewMat;
        public OpenTK.Matrix4 ProjMat;
        public OpenTK.Vector3 BBoxMin;
        public OpenTK.Vector3 BBoxMax;
        public bool ContainsView;
        public bool ContainsProj;
        public int VAO, VBO;
        public GlRenderer.RenderSettings RenderSettings;

        /* Mesh records, stores offsets + vertex count. */
        public GlMeshRecord PolygonRecord, PolylineRecord;

        public GlResource(ITDParser.ITDMesh Mesh, GlRenderer.RenderSettings RenderSettings)
        {
            InitFromMesh(Mesh);
            this.RenderSettings = RenderSettings;
        }

        public GlResource(ITDParser.ITDMesh Mesh)
        {
            InitFromMesh(Mesh);
        }

        private void InitFromMesh(ITDParser.ITDMesh Mesh)
        {
            int 
                NumVertices = Mesh.Vertecies.Length;

            GenMateices(Mesh.ViewMat, Mesh.ProjMat);
            GenRecords(Mesh);

            BBoxMin = new OpenTK.Vector3((float)Mesh.Min[0], (float)Mesh.Min[1], (float)Mesh.Min[2]);
            BBoxMax = new OpenTK.Vector3((float)Mesh.Max[0], (float)Mesh.Max[1], (float)Mesh.Max[2]);
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

        private void GenMateices(double[] View, double[] Proj)
        {
            ProjMat = ViewMat = OpenTK.Matrix4.Zero;
            ContainsProj = ContainsView = false;

            if (View != null)
            {
                ViewMat = new OpenTK.Matrix4((float)View[ 0], (float)View[ 1], (float)View[ 2], (float)View[ 3],
                                             (float)View[ 4], (float)View[ 5], (float)View[ 6], (float)View[ 7],
                                             (float)View[ 8], (float)View[ 9], (float)View[10], (float)View[11],
                                             (float)View[12], (float)View[13], (float)View[14], (float)View[15]);
                ContainsView = true;
            }

            if (Proj != null)
            {
                ProjMat = new OpenTK.Matrix4((float)Proj[ 0], (float)Proj[ 1], (float)Proj[ 2], (float)Proj[ 3],
                                             (float)Proj[ 4], (float)Proj[ 5], (float)Proj[ 6], (float)Proj[ 7],
                                             (float)Proj[ 8], (float)Proj[ 9], (float)Proj[10], (float)Proj[11],
                                             (float)Proj[12], (float)Proj[13], (float)Proj[14], (float)Proj[15]);
                ContainsProj = true;
            }

        }

        private void GenRecords(ITDParser.ITDMesh Mesh)
        {
            int TotalSize, Offset;

            Offset = 0;
            TotalSize = Mesh.PolygonMeshSizes.Sum();
            this.PolygonRecord = new GlMeshRecord(Offset, TotalSize);

            Offset = TotalSize;
            TotalSize = Mesh.PolylineMeshSizes.Sum();
            this.PolylineRecord = new GlMeshRecord(Offset, TotalSize);
        }

        public void Destroy()
        {
            GL.DeleteVertexArray(VAO);
            GL.DeleteBuffer(VBO);
        }
    }
}
