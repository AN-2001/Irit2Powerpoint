#pragma once

#include <stdbool.h>

#ifdef ITDPARSER_EXPORTS
#define ITDPARSER_API __declspec(dllexport)
#else
#define ITDPARSER_API __declspec(dllimport)
#endif

typedef ITDPARSER_API struct VertexStruct {
    double x, y, z;
    double nx, ny, nz;
    double r, g, b;
    double u, v;
} VertexStruct;

typedef ITDPARSER_API struct ImportSettings
{
    int PolygonFineness;
    int PolylineFineness;
    int IsolinesSamples;
    int PolygonOptimal;
    int PolylineOptimal;
    int FlipNormals;
} ImportSettings;

typedef ITDPARSER_API struct MeshStruct {
    VertexStruct *Vertices;
    int *PolygonMeshSizes, 
        *PolylineMeshSizes;
    double *ViewMatrix, *ProjMatrix, 
            MinX, MinY, MinZ,
            MaxX, MaxY, MaxZ;
    int TotalVertices,
        TotalPolygonMeshes,
        TotalPolylineMeshes;
    int PerVertexColour;
} MeshStruct;

ITDPARSER_API MeshStruct *ITDParserParse(const char *Path, ImportSettings Settings);
ITDPARSER_API void ITDParserFree(MeshStruct *Mesh);
