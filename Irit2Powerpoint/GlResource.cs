using OpenTK.Graphics.OpenGL4;

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
        public OpenTK.Vector3 LightPos;
        public bool PerVertexColour;
        public bool ContainsView;
        public bool ContainsProj;
        public int VAO, VBO;
        public GlRenderer.RenderSettings RenderSettings;

        /* Mesh records, stores offsets + vertex count. */
        public GlMeshRecord PolygonRecord, PolylineRecord;

        public GlResource(ITDParser.ITDMesh Mesh)
        {
            int 
                NumVertices = Mesh.Vertecies.Length;

            GenMateices(Mesh.ViewMat, Mesh.ProjMat);
            GenRecords(Mesh);

            BBoxMin = new OpenTK.Vector3((float)Mesh.Min[0], (float)Mesh.Min[1], (float)Mesh.Min[2]);
            BBoxMax = new OpenTK.Vector3((float)Mesh.Max[0], (float)Mesh.Max[1], (float)Mesh.Max[2]);
            LightPos = new OpenTK.Vector3((float)Mesh.LightPos[0], (float)Mesh.LightPos[1], (float)Mesh.LightPos[2]);
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

            Logger.GetInstance().Trace($"Uploaded mesh to GPU: numVertecies = {NumVertices}, VAO = {VAO}, VBO = {VBO}");
            GlRenderer.DoGlErrorCheck();
        }

        public GlResource(OpenTK.Vector4d[] Vertices)
        {
            int i;
            double[] Arr = new double[Vertices.Length * 4];

            for (i = 0; i < Vertices.Length; i++) {
                Arr[i * 4 + 0] = Vertices[i].X;
                Arr[i * 4 + 1] = Vertices[i].Y;
                Arr[i * 4 + 2] = Vertices[i].Z;
                Arr[i * 4 + 3] = Vertices[i].W;
            }

            GL.GenVertexArrays(1, out this.VAO);
            GL.GenBuffers(1, out this.VBO);

            GL.BindVertexArray(this.VAO);
            GL.BindBuffer(BufferTarget.ArrayBuffer, this.VBO);

            GL.BufferData(BufferTarget.ArrayBuffer,
                    Arr.Length * sizeof(double),
                    Arr,
                    BufferUsageHint.StaticDraw);

            GL.VertexAttribPointer(0,
                2,
                VertexAttribPointerType.Double,
                false,
                sizeof(double) * 4,
                0);

            GL.VertexAttribPointer(1,
                2,
                VertexAttribPointerType.Double,
                false,
                sizeof(double) * 4,
                sizeof(double) * 2);

            GL.EnableVertexArrayAttrib(this.VAO, 0);
            GL.EnableVertexArrayAttrib(this.VAO, 1);

            GL.BindBuffer(BufferTarget.ArrayBuffer, 0);
            GL.BindVertexArray(0);

            Logger.GetInstance().Trace($"Uploaded mesh to GPU: numVertecies = {Vertices.Length}, VAO = {VAO}, VBO = {VBO}");
            GlRenderer.DoGlErrorCheck();
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
                Logger.GetInstance().Trace($"Mesh contains view matrix.");
                ContainsView = true;
            }

            if (Proj != null)
            {
                ProjMat = new OpenTK.Matrix4((float)Proj[ 0], (float)Proj[ 1], (float)Proj[ 2], (float)Proj[ 3],
                                             (float)Proj[ 4], (float)Proj[ 5], (float)Proj[ 6], (float)Proj[ 7],
                                             (float)Proj[ 8], (float)Proj[ 9], (float)Proj[10], (float)Proj[11],
                                             (float)Proj[12], (float)Proj[13], (float)Proj[14], (float)Proj[15]);
                Logger.GetInstance().Trace($"Mesh contains projection matrix.");
                ContainsProj = true;
            }

        }

        private void GenRecords(ITDParser.ITDMesh Mesh)
        {
            int TotalSize, Offset;

            Offset = 0;
            TotalSize = Mesh.VertexCutoffOffset;
            this.PolygonRecord = new GlMeshRecord(Offset, TotalSize);

            Logger.GetInstance().Trace($"Created polygonal mesh recrods: PolygonRecord=[{Offset}, {TotalSize}]");

            Offset = TotalSize;
            TotalSize = Mesh.Vertecies.Length - Offset;
            this.PolylineRecord = new GlMeshRecord(Offset, TotalSize);
            Logger.GetInstance().Trace($"Created curve mesh recrods: PolylineRecord=[{Offset}, {TotalSize}]");
        }

        public void Destroy()
        {
            GL.DeleteVertexArray(VAO);
            GL.DeleteBuffer(VBO);
            Logger.GetInstance().Trace($"Destroyed render resrouce.");
            GlRenderer.DoGlErrorCheck();
        }
    }
}
