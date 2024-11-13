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

        [StructLayout(LayoutKind.Sequential)]
        public struct ImportSettings
        {
            public int PolygonFineness;
            public int PolylineFineness;
            public int IsolinesSamples;
            public int PolygonOptimal;
            public int PolylineOptimal;
        };

        public static string SerializeImportSettings(ImportSettings Settings)
        {
            StringBuilder
                sb = new StringBuilder();

            sb.AppendFormat("{0};{1};{2};{3};{4}",
                Settings.PolygonFineness, Settings.PolylineFineness,
                Settings.IsolinesSamples, Settings.PolygonOptimal,
                Settings.PolylineOptimal);
            return sb.ToString();
        }

        public static ImportSettings DeserializeImportSettings(string String)
        {
            ImportSettings
                Settings = new ImportSettings();
            string[] Splitted = String.Split(';');

            try
            {
                Settings.PolygonFineness = int.Parse(Splitted[0]);
                Settings.PolylineFineness = int.Parse(Splitted[1]);
                Settings.IsolinesSamples = int.Parse(Splitted[2]);
                Settings.PolygonOptimal = int.Parse(Splitted[3]);
                Settings.PolylineOptimal = int.Parse(Splitted[4]);
            } catch(Exception)
            {
                MessageBox.Show("Import settings string was corrupted, using default.");
                Settings = DefaultImportSettings;
            }

            return Settings;
        }

        public static int VERTEX_STRUCT_SIZE = Marshal.SizeOf(typeof(VertexStruct));
        public static ImportSettings DefaultImportSettings = new ImportSettings()
        {
            PolygonFineness = 64,
            PolylineFineness = 32,
            IsolinesSamples = 0,
            PolygonOptimal = 0,
            PolylineOptimal = 0,
        };

        [StructLayout(LayoutKind.Sequential)]
        public struct ITDMesh
        {
            public VertexStruct[] Vertecies; /* All the vertecies that this ITD file uses. */
            public int[] PolygonMeshSizes, /* The number of polygon meshes is the length of this array. */
                         PolylineMeshSizes; /* The number of polyline meshes is the length of this array. */
            public double[] ViewMat, ProjMat; /* 4x4 matrices in row major form. */
            public double[] Min, Max;
        }

        [StructLayout(LayoutKind.Sequential)]
        private struct MeshStruct
        {
            public IntPtr Vertices;
            public IntPtr PolygonMeshSizes, PolylineMeshSizes;
            public IntPtr ViewMatrix, ProjMatrix;
            public double MinX, MinY, MinZ,
                          MaxX, MaxY, MaxZ;
            public int TotalVertices, TotalPolygonMeshes, TotalPolylineMeshes;
        }

        [DllImport("ITDParser.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr ITDParserParse([MarshalAs(UnmanagedType.LPStr)] StringBuilder Str, ImportSettings Settings);

        [DllImport("ITDParser.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr ITDParserFree(IntPtr Struct);

        public static ITDMesh Parse(string Path, ImportSettings Settings)
        {
            ITDMesh Mesh;
            MeshStruct Struct;
            StringBuilder 
                PathAsBuilder = new StringBuilder(Path);
            IntPtr /* Call C API to parse ITD file. */
                Ret = ITDParserParse(PathAsBuilder, Settings);
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

            Ret.Min[0] = Mesh.MinX; Ret.Min[1] = Mesh.MinY; Ret.Min[2] = Mesh.MinZ;
            Ret.Max[0] = Mesh.MaxX; Ret.Max[1] = Mesh.MaxY; Ret.Max[2] = Mesh.MaxZ;

            Ret.PolygonMeshSizes = new int[Mesh.TotalPolygonMeshes];
            Ret.PolylineMeshSizes = new int[Mesh.TotalPolylineMeshes];
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
