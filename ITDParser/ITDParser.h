#pragma once

#include <stdbool.h>
#include <Windows.h>

#ifdef ITDPARSER_EXPORTS
#define ITDPARSER_API __declspec(dllexport)
#else
#define ITDPARSER_API __declspec(dllimport)
#endif

#define I2P_LOG_AUX( Level, Format, ... ) ITDParserLog( Level, __FILE__, __FUNCTION__, __LINE__, Format, ##__VA_ARGS__)

#define I2P_LOG_TRACE( Format, ... ) I2P_LOG_AUX( 0, Format, ##__VA_ARGS__ )
#define I2P_LOG_INFO( Format, ... )  I2P_LOG_AUX( 1, Format, ##__VA_ARGS__ )
#define I2P_LOG_WARN( Format, ... )  I2P_LOG_AUX( 2, Format, ##__VA_ARGS__ )
#define I2P_LOG_ERROR( Format, ... ) I2P_LOG_AUX( 3, Format, ##__VA_ARGS__ )

#define I2P_NUM_LIGHTS (32)

typedef void(*LoggerFunc)(
    int level,          
    const char* file,   
    const char* function,
    int line,
    const char* message
);

typedef ITDPARSER_API struct LightStruct {
    int IsEnabled;
    double x, y, z;
    double ar, ag, ab, /* Ambient Colour */
           dr, dg, db, /* Diffuse Colour */
           sr, sg, sb; /* Specular Colour */
} LightStruct;

typedef ITDPARSER_API struct VertexStruct {
    double x, y, z;
    double nx, ny, nz;
    double r, g, b;
    double u, v;
} VertexStruct;

typedef ITDPARSER_API struct MeshStruct {
    VertexStruct *Vertices;
    LightStruct *Lights;
    double *ViewMatrix, *ProjMatrix, ZOffset;
    int TotalVertices,
        VerticesCutoffOffset; /* Cutoff offset between polygons and polylines. */
} MeshStruct;

typedef struct ParserCommandsSturct {
    struct ParserCommndsSturct *Next;
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
        Wireframe,
        ChangeClippingPlanes;
    double NormalSize, PlgnFineness, PllnFineness,
           LightPos[3], PointSize, ZMin, ZMax, ZOffset,
           Ambient, Diffuse, Specular;
} ParserSettingsStruct;

typedef struct ParserStruct {
    char *SettingsString;
    ParserCommandsStruct *LineCommands, *TriCommands;
    IPObjectStruct *PObj;
    ParserSettingsStruct Settings;
    IrtHmgnMatType InternalViewMat, InternalProjMat;
    LightStruct Lights[I2P_NUM_LIGHTS];
    double BBox[2][3];
    int NumTriangles, NumLines;
} ParserStruct;

DWORD TLSIndex;
HANDLE gMutex;
extern LoggerFunc GlobalLogger;

ITDPARSER_API MeshStruct *ITDParserParse(const char *Path, const char *Settings);
ITDPARSER_API void ITDParserSetLogger(LoggerFunc Logger);
ITDPARSER_API void ITDParserFree(MeshStruct *Mesh);

void ITDParserLog(int Level, const char *File, const char *Function, int Line, const char *Format, ...);
