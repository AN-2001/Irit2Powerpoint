using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace Irit2Powerpoint
{
    public static class ITDParser
    {

        [StructLayout(LayoutKind.Sequential)]
        public struct VertexStruct
        {
            public double x, y, z;
            public double nx, ny, nz;
            public double r, g, b;
            public double u, v;
        }

        public static int VERTEX_STRUCT_SIZE = Marshal.SizeOf(typeof(VertexStruct));

        [StructLayout(LayoutKind.Sequential)]
        public struct ITDMesh
        {
            public VertexStruct[] Vertecies; /* All the vertecies that this ITD file uses. */
            public int VertexCutoffOffset;
            public double[] ViewMat, ProjMat; /* 4x4 matrices in row major form. */
            public double[] Min, Max;
        }

        [StructLayout(LayoutKind.Sequential)]
        private struct MeshStruct
        {
            public IntPtr Vertices;
            public IntPtr ViewMatrix, ProjMatrix;
            public double MinX, MinY, MinZ,
                          MaxX, MaxY, MaxZ;
            public int TotalVertices, VertexCutoffOffset;
        }

        [DllImport("ITDParser.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr ITDParserParse(string Path, string ImportSettings);

        [DllImport("ITDParser.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr ITDParserFree(IntPtr Struct);

        public static ITDMesh Parse(string Path, string Settings)
        {
            ITDMesh Mesh;
            MeshStruct Struct;
            IntPtr /* Call C API to parse ITD file. */
                Ret = ITDParserParse(Path, Settings);
            if (Ret == IntPtr.Zero)
                throw new ParseException($"Error parsing ITD file at {Path}");

            /* Turn pointer into C# struct. */
            Struct = Marshal.PtrToStructure<MeshStruct>(Ret);
            /* Turn c# struct with pointers into user-friendly struct. */
            Mesh = MarshalMesh(Struct);
            /* We have to free here since Ret is unmanaged by the GC. */
            ITDParserFree(Ret);
            return Mesh;
        }

        /* Allocates the arrays in managed memories and performs basic copies. */
        private static ITDMesh MarshalMesh(MeshStruct Mesh)
        {
            ITDMesh Ret;
            int i;

            Ret.ViewMat = null;
            Ret.ProjMat = null;
            Ret.Min = new double[3];
            Ret.Max = new double[3];

            Ret.VertexCutoffOffset = Mesh.VertexCutoffOffset;
            Ret.Min[0] = Mesh.MinX; Ret.Min[1] = Mesh.MinY; Ret.Min[2] = Mesh.MinZ;
            Ret.Max[0] = Mesh.MaxX; Ret.Max[1] = Mesh.MaxY; Ret.Max[2] = Mesh.MaxZ;

            Ret.Vertecies = new VertexStruct[Mesh.TotalVertices];

            if (Mesh.ViewMatrix != IntPtr.Zero)
            {
                Ret.ViewMat = new double[16];
                Marshal.Copy(Mesh.ViewMatrix, Ret.ViewMat, 0, 16);
            }

            if (Mesh.ProjMatrix != IntPtr.Zero)
            {
                Ret.ProjMat = new double[16];
                Marshal.Copy(Mesh.ProjMatrix, Ret.ProjMat, 0, 16);
            }

            /* Structs have to copied like this sadly... */
            for (i = 0; i < Mesh.TotalVertices; i++)
                Ret.Vertecies[i] =
                    Marshal.PtrToStructure<VertexStruct>(Mesh.Vertices +
                                                         i * VERTEX_STRUCT_SIZE);
            return Ret;
        }

    }
}
