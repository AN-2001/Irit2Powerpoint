using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace Irit2Powerpoint
{
    public static class ITDParser
    {

        [StructLayout(LayoutKind.Sequential)]
        public struct VertexStruct
        {
            public double x, y, z;
            public double nx, ny, nz;
        }

        public static int VERTEX_STRUCT_SIZE = Marshal.SizeOf(typeof(VertexStruct));

        [StructLayout(LayoutKind.Sequential)]
        public struct ITDMesh
        {
            public VertexStruct[] Vertecies; /* All the vertecies that this ITD file uses. */
            public int[] PolygonMeshSizes, /* The number of polygon meshes is the length of this array. */
                         PolylineMeshSizes; /* The number of polyline meshes is the length of this array. */
        }

        [StructLayout(LayoutKind.Sequential)]
        private struct MeshStruct
        {
            public IntPtr Vertices;
            public IntPtr PolygonMeshSizes, PolylineMeshSizes;
            public int TotalVertices, TotalPolygonMeshes, TotalPolylineMeshes;
        }

        [DllImport("ITDParser.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr ITDParserParse([MarshalAs(UnmanagedType.LPStr)] StringBuilder Str);

        [DllImport("ITDParser.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr ITDParserFree(IntPtr Struct);

        public static ITDMesh Parse(string Path)
        {
            ITDMesh Mesh;
            MeshStruct Struct;
            StringBuilder 
                PathAsBuilder = new StringBuilder(Path);
            IntPtr /* Call C API to parse ITD file. */
                Ret = ITDParserParse(PathAsBuilder);
            if (Ret == IntPtr.Zero)
                throw new Exception($"Error parsing ITD file at {Path}");

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

            Ret.PolygonMeshSizes = new int[Mesh.TotalPolygonMeshes];
            Ret.PolylineMeshSizes = new int[Mesh.TotalPolylineMeshes];
            Ret.Vertecies = new VertexStruct[Mesh.TotalVertices];

            if (Mesh.TotalPolygonMeshes > 0)
                Marshal.Copy(Mesh.PolygonMeshSizes, Ret.PolygonMeshSizes, 0, Mesh.TotalPolygonMeshes);
            if (Mesh.TotalPolylineMeshes > 0)
                Marshal.Copy(Mesh.PolylineMeshSizes, Ret.PolylineMeshSizes, 0, Mesh.TotalPolylineMeshes);
            /* Structs have to copied like this sadly... */
            for (i = 0; i < Mesh.TotalVertices; i++)
                Ret.Vertecies[i] =
                    Marshal.PtrToStructure<VertexStruct>(Mesh.Vertices +
                                                         i * VERTEX_STRUCT_SIZE);
            return Ret;
        }

    }
}
