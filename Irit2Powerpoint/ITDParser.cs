using System;
using System.Runtime.InteropServices;

namespace Irit2Powerpoint
{
    public static class ITDParser
    {
        [StructLayout(LayoutKind.Sequential)]
        public struct LightStruct
        {
            public int IsEnabled;
            public double x, y, z;
            public double ar, ag, ab,
                          dr, dg, db,
                          sr, sg, sb;
        }
        public static int LIGHT_STRUCT_SIZE = Marshal.SizeOf(typeof(LightStruct));

        [StructLayout(LayoutKind.Sequential)]
        public struct VertexStruct
        {
            public double x, y, z;
            public double nx, ny, nz;
            public double r, g, b;
            public double u, v;
        }

        public static int VERTEX_STRUCT_SIZE = Marshal.SizeOf(typeof(VertexStruct));
        public static int I2P_NUM_LIGHTS = 32;

        [StructLayout(LayoutKind.Sequential)]
        public struct ITDMesh
        {
            public VertexStruct[] Vertecies; /* All the vertecies that this ITD file uses. */
            public int VertexCutoffOffset;
            public LightStruct[] Lights;
            public double[] ViewMat, ProjMat; /* 4x4 matrices in row major form. */
            public double ZOffset;
        }

        [StructLayout(LayoutKind.Sequential)]
        private struct MeshStruct
        {
            public IntPtr Vertices;
            public IntPtr Lights, ViewMatrix, ProjMatrix;
            public double ZOffset;
            public int TotalVertices, VertexCutoffOffset;
        }

        [DllImport("ITDParser.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr ITDParserParse(string Path, string ImportSettings);

        [DllImport("ITDParser.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr ITDParserFree(IntPtr Struct);

        [DllImport("ITDParser.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr ITDParserSetLogger(LoggerDelegate Logger);

        public static ITDMesh Parse(string Path, string Settings)
        {
            ITDMesh Mesh;
            MeshStruct Struct;
            IntPtr /* Call C API to parse ITD file. */
                Ret = ITDParserParse(Path, Settings);
            if (Ret == IntPtr.Zero)
            {
                Logger.GetInstance().Error($"Error parsing ITD file at {Path} with commands {Settings}");
                throw new ParseException($"Error parsing ITD file at {Path} with commands {Settings}");
            }

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

            Ret.ZOffset = Mesh.ZOffset;

            Ret.VertexCutoffOffset = Mesh.VertexCutoffOffset;

            Ret.Vertecies = new VertexStruct[Mesh.TotalVertices];
            Ret.Lights = new LightStruct[I2P_NUM_LIGHTS];

            Ret.ViewMat = new double[16];
            Marshal.Copy(Mesh.ViewMatrix, Ret.ViewMat, 0, 16);

            Ret.ProjMat = new double[16];
            Marshal.Copy(Mesh.ProjMatrix, Ret.ProjMat, 0, 16);


            /* Structs have to copied like this sadly... */
            for (i = 0; i < Mesh.TotalVertices; i++)
                Ret.Vertecies[i] =
                    Marshal.PtrToStructure<VertexStruct>(Mesh.Vertices +
                                                         i * VERTEX_STRUCT_SIZE);
            for (i = 0; i < 32; i++)
                Ret.Lights[i] =
                    Marshal.PtrToStructure<LightStruct>(Mesh.Lights + i * LIGHT_STRUCT_SIZE);

            return Ret;
        }

    }
}
