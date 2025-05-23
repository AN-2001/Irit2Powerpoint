#pragma once

#include <stdbool.h>
#include <Windows.h>

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

typedef ITDPARSER_API struct MeshStruct {
    VertexStruct *Vertices;
    double *ViewMatrix, *ProjMatrix, 
            MinX, MinY, MinZ,
            MaxX, MaxY, MaxZ,
            lx, ly, lz;
    int TotalVertices,
        VerticesCutoffOffset; /* Cutoff offset between polygons and polylines. */
} MeshStruct;

typedef struct ParserCommandsSturct {
    struct ParserCommandsSturct *Next;
    VertexStruct Vertices[3]; /* We can have 3 vertices at most. */
    double nx, ny, nz;
    int VertexCount;
} ParserCommandsStruct;

typedef struct ParserSettingsStruct {
    int DrawVNormal, DrawPNormal,
        Isolines[3];
    int OverridePolylineCol;
    double PolylineCol[3];
    int OverridePolygonCol;
    double PolygonCol[3];
    int PolygonOptiApprox,
        PolylineOptiApprox,
        DrawSurfacePoly,
        DrawSurfaceMesh,
        DrawModelsMonolithic,
        DrawSurfaceOrient,
        SurfaceWireSetup,
        FlipNormalOrient,
        Wireframe;
    double NormalSize, PlgnFineness, PllnFineness,
           LightPos[3], PointSize;
} ParserSettingsStruct;

typedef struct ParserStruct {
    ParserCommandsStruct *LineCommands, *TriCommands;
    IPObjectStruct *PObj;
    ParserSettingsStruct Settings;
    double *ViewMatrix, *ProjMatrix;
    int NumTriangles, NumLines;
} ParserStruct;

DWORD TLSIndex;
HANDLE gMutex;

ITDPARSER_API MeshStruct *ITDParserParse(const char *Path, const char *Settings);
ITDPARSER_API void ITDParserFree(MeshStruct *Mesh);
