#ifdef ITDPARSER_EXPORTS
#define ITDPARSER_API __declspec(dllexport)
#else
#define ITDPARSER_API __declspec(dllimport)
#endif

typedef ITDPARSER_API struct VertexStruct {
    double x, y, z;
    double nx, ny, nz;
} VertexStruct;

typedef ITDPARSER_API struct MeshStruct {
    VertexStruct *Vertices;
    int *PolygonIndices,
        *PolylineIndices,
        *PolygonMeshSizes, 
        *PolylineMeshSizes, 
        TotalVertices,
        TotalPolygonIndices,
        TotalPolylineIndices,
        TotalPolygonMeshes,
        TotalPolylineMeshes;
} MeshStruct;

ITDPARSER_API MeshStruct *ITDParserParse(const char *Path);
ITDPARSER_API void ITDParserFree(MeshStruct *Mesh);
