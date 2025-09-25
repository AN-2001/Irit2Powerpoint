#include "inc_irit/irit_sm.h"
#include "inc_irit/allocate.h"
#include "inc_irit/iritprsr.h"
#include "ITDParser.h"
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include "inc_irit/allocate.h"
#include "inc_irit/attribut.h"
#include "inc_irit/geom_lib.h"
#include "inc_irit/user_lib.h"
#include "inc_irit/ip_cnvrt.h"
#include "objectProcessor.h"
#include <ctype.h>
#include <stdarg.h>

#define ITD_PRSR_SUCCESS (1)
#define ITD_PRSR_FAILURE (0)
#define ITD_PRSR_MALLOC(Size) ((Size) ? malloc(Size) : NULL)
#define ITD_PRSR_ZAP(Ptr, Size) ((Ptr) && memset(Ptr, 0, sizeof(*(Ptr)) * Size))

IRIT_STATIC_DATA const IrtHmgnMatType
    DefaultIsometricMat = {		     /* Isometric view, by default. */
	{ -0.707107, -0.408248, 0.577350, 0.000000 },
	{  0.707107, -0.408248, 0.577350, 0.000000 },
	{  0.000000,  0.816496, 0.577350, 0.000000 },
	{  0.000000,  0.000000, 0.000000, 1.000000 }
    };


void ApplyIsometricMatPreservingScaleAndTranslation(IrtHmgnMatType Mat);
void CalculateObjectNormalizer(IPObjectStruct *PObj, IrtHmgnMatType Mat);

static int ParserHandleArgs(ParserStruct* Parser, char* Settings);
static void ParserTraverseObjects(ParserStruct* Parser);
static void ParserHandlePoly(ParserStruct* Parser, IPObjectStruct* PObj);
static void ParserPostProcess(ParserStruct* Parser);
static void ParserSubmitNormal(ParserStruct* Parser, double nx, double ny, double nz);
static void ParserSubmitLineVertex(ParserStruct* Parser, VertexStruct v);
static void ParserSubmitTriangleVertex(ParserStruct* Parser, VertexStruct v);
static void ParserSubmitPointVertex(ParserStruct* Parser, VertexStruct v, int IsVector);
static MeshStruct* ParserFinalize(ParserStruct* Parser);
static void GetUV(IPVertexStruct* Vert, double* u, double* v);
static int GetColor(IPAttributeStruct* Attr, double* r, double* g, double* b);
static void BuildArgv(char* String, int* Argc, char **Argv);
static void SetClippingPlanes(ParserStruct *Parser, IrtHmgnMatType ProjectionMat, double ZMin, double ZMax);

IRIT_STATIC_DATA const char
* ConfigStr = "I2P n%- N%- I%-#IsoLines!s F%-PlgnOpti|PlgnFineNess!d!F f%-PllnOpti|PllnFineNess!d!F L%-Normal|Size!F c%-Override?|r,g,b|(Polyline)!d!s C%-Override?|r,g,b|(Polygon)!d!s W%-Wiresetup!d S%-x,y,z{,a,d,s}|(Light)!s p%-Point|Size!F Z%-ZMin|ZMax!F!F z%-ZOffset!F M%- V%- P%- t%- o%- w%-";

LoggerFunc GlobalLogger = NULL;

IRIT_STATIC_DATA const ParserSettingsStruct
    DefaultSettings = {
	0, 0, /* VNormal, PNormal */
	{10, 10, 10}, /* Isolines */
	FALSE, {1, 1, 0}, /* OverridePolylineCol - Polyline Colour */
	FALSE, {1, 0, 0}, /* OverridePolygonCol - Polygon Colour */
	FALSE, /* PolygonOptiApprox */
	SYMB_CRV_APPROX_CURVATURE, /* PolylineOptiApprox */
	TRUE, /* DrawSurfacePoly */
	FALSE, /* DrawSurfaceMesh */
	FALSE, /* DrawModelsMonolithic */
	FALSE, /* DrawSurfaceOrient */
	1, /* DrawWireSetup */
	FALSE, /* FlipNormalOrient */
	FALSE, /* Wireframe */
	FALSE, /* ChangeClippingPlanes */
	1, /* NormalSize */
	40, /* PlgnFineness */
	40, /* PllnFineness */
	{1, 2, 5}, /* LightPos */
	0.05, /* PointSize */
	-1, 1, /* ZMin, ZMax */
	0.001, /* ZOffset */
	0.3, /* Ambient */
	1, /* Diffuse */
	1, /* Specular */
    };

ITDPARSER_API void ITDParserSetLogger(LoggerFunc Logger)
{
    GlobalLogger = Logger;
}

ITDPARSER_API MeshStruct* ITDParserParse(const char* Path, const char* Settings)
{
    ParserStruct Parser;
    char *SettingsCpy;

    I2P_LOG_TRACE( "Handling parse request for Path = %s, Settings = %s", Path, Settings);

    memset(&Parser, 0, sizeof(Parser));

    Parser.BBox[0][0] =  INFINITY; Parser.BBox[0][1] =  INFINITY; Parser.BBox[0][2] =  INFINITY;
    Parser.BBox[1][0] = -INFINITY; Parser.BBox[1][1] = -INFINITY; Parser.BBox[1][2] = -INFINITY;

    /* Initialize matrices. */
    IritMiscMatGenUnitMat(Parser.InternalViewMat);

    /* Orthographic Projection. */
    IritMiscMatGenUnitMat(Parser.InternalProjMat);

    /* Process command line arguemnts into the parser struct. */
    SettingsCpy = _strdup(Settings);
    if (!ParserHandleArgs(&Parser, SettingsCpy)) {
	free(SettingsCpy);
	return NULL;
    }
    free(SettingsCpy);

    I2P_LOG_TRACE( "%s: Loading file at Path = %s", Parser.SettingsString);
    /* Load the file and preprocess it into a consistent structure + triangulate it. */
    LoadFromFile(&Parser, Path);
    if (!Parser.PObj) {
	I2P_LOG_ERROR( "%s: could not get the PObject from the provided path", Parser.SettingsString);
	return NULL;
    }

    I2P_LOG_TRACE( "%s: Traversing object and aggregating mesh info", Parser.SettingsString);
    /* Extract the mesh out of the PObj. */
    ParserTraverseObjects(&Parser);

    I2P_LOG_TRACE( "%s: Post processing mesh", Parser.SettingsString );
    /* Post process the mesh. */
    ParserPostProcess(&Parser);

    I2P_LOG_TRACE( "%s: Finalizing mesh and emitting back to I2P", Parser.SettingsString );
    /* Finalize the mesh for sending back into c# */
    return ParserFinalize(&Parser);
}

static int ParserHandleArgs(ParserStruct* Parser, char* Settings)
{
    int 
	IsolineFlag = FALSE,
	PolygonFinenessFlag = FALSE,
	PolylineFinenessFlag = FALSE,
	NormalSizeFlag = FALSE,
	PolygonColourFlag = FALSE,
	PolylineColourFlag = FALSE,
	SurfaceWireSetupFlag = FALSE,
	LightFlag = FALSE,
	PointSizeFlag = FALSE,
	ZOffsetFlag = FALSE;
    char
	*IsolineString = NULL,
	*PolygonString = NULL,
	*PolylineString = NULL,
	*LightString = NULL;
    ParserSettingsStruct* PSettings = &(Parser->Settings);
    int Err;
    char *Argv[1024];
    char ErrorMessage[4096];
    int Argc;

    Parser -> SettingsString = _strdup(Settings);

    Argc = 0;
    memset(Argv, 0, sizeof(Argv));
    BuildArgv(Settings, &Argc, Argv);

    memcpy(PSettings, &DefaultSettings, sizeof(DefaultSettings));
    if ((Err = IritMiscGetArgs(Argc, Argv, ConfigStr,
	&PSettings->DrawVNormal, &PSettings->DrawPNormal,
	&IsolineFlag, &IsolineString,
	&PolygonFinenessFlag, &PSettings->PolygonOptiApprox, &PSettings->PlgnFineness,
	&PolylineFinenessFlag, &PSettings->PolylineOptiApprox, &PSettings->PllnFineness,
	&NormalSizeFlag, &PSettings->NormalSize,
	&PolylineColourFlag, &PSettings -> OverridePolylineCol, &PolylineString,
	&PolygonColourFlag, &PSettings -> OverridePolygonCol, &PolygonString,
	&SurfaceWireSetupFlag, &PSettings->SurfaceWireSetup,
	&LightFlag, &LightString,
	&PointSizeFlag, &PSettings->PointSize,
	&PSettings -> ChangeClippingPlanes, &PSettings -> ZMin, &PSettings -> ZMax,
	&ZOffsetFlag, &PSettings -> ZOffset,
	&PSettings -> DrawSurfaceMesh,
	&PSettings -> DrawModelsMonolithic,
	&PSettings -> DrawSurfacePoly,
	&PSettings -> DrawSurfaceOrient,
	&PSettings -> FlipNormalOrient,
	&PSettings -> Wireframe, NULL))) {
	IritMiscStringErrMsg(Err, ErrorMessage);
	I2P_LOG_ERROR( "%s: Invalid args provided to parser error = %s", Parser -> SettingsString, ErrorMessage);
	return FALSE;
    }

    if (PSettings -> ChangeClippingPlanes &&
	PSettings -> ZMin >= PSettings -> ZMax) {
	I2P_LOG_ERROR( "%s: Invalid Input ZMin must be strictly less than ZMax: ZMin = %f, ZMax = %f", Parser -> SettingsString,PSettings -> ZMin, PSettings -> ZMax);
	return FALSE;
    }

    if (PolylineColourFlag) {
	int r, g, b;
	if (sscanf(PolylineString, "%d,%d,%d", &r, &g, &b) != 3) {
	    I2P_LOG_ERROR( "%sL Could not parse PolylineString got = %s, expected = %s", Parser -> SettingsString, PolylineString, "%d,%d,%d" );
	    return FALSE;
	}
	PSettings -> PolylineCol[0] = r / 255.0;
	PSettings -> PolylineCol[1] = g / 255.0;
	PSettings -> PolylineCol[2] = b / 255.0;
    }

    if (PolygonColourFlag) {
	int r, g, b;
	if (sscanf(PolygonString, "%d,%d,%d", &r, &g, &b) != 3) {
	    I2P_LOG_ERROR( "%s: Could not parse PolygonString got = %s, expected = %s", Parser -> SettingsString, PolygonString, "%d,%d,%d");
	    return FALSE;
	}
	PSettings -> PolygonCol[0] = r / 255.0;
	PSettings -> PolygonCol[1] = g / 255.0;
	PSettings -> PolygonCol[2] = b / 255.0;
    }


    if (IsolineFlag) {
	if ((sscanf(IsolineString, "%d,%d,%d", &PSettings->Isolines[0],
	    &PSettings -> Isolines[1],
	    &PSettings -> Isolines[2]) != 3) &&
	    (sscanf(IsolineString, "%d,%d", &PSettings -> Isolines[0],
		&PSettings -> Isolines[1]) != 2) &&
	    (sscanf(IsolineString, "%d", &PSettings -> Isolines[0]) != 1)) {
	    I2P_LOG_ERROR( "%s: Could not parse IsolineString got = %s, expected = %s", Parser -> SettingsString, IsolineString, "%d,%d,%d OR %d %d %d OR %d %d OR %d,%d OR %d");
	    return FALSE;
	}
    }

    if (LightFlag) {
	if (sscanf(LightString, "%lf,%lf,%lf,%lf,%lf,%lf",
		   &PSettings -> LightPos[0],
		   &PSettings -> LightPos[1],
		   &PSettings -> LightPos[2],
		   &PSettings -> Ambient,
		   &PSettings -> Diffuse,
		   &PSettings -> Specular) != 6 &&
	    sscanf(LightString, "%lf,%lf,%lf",
		   &PSettings -> LightPos[0],
		   &PSettings -> LightPos[1],
		   &PSettings -> LightPos[2]) != 3) {
	    I2P_LOG_ERROR( "%s: Could not parse LightString got = %s, expected = %s", Parser -> SettingsString, LightString, "%f,%f,%f OR %f %f %f OR %f,%f,%f,%f,%f,%f OR %f %f %f %f %f %f");
	    return FALSE;
	}
    }

    I2P_LOG_TRACE( "%s: Processed all command line args", Parser -> SettingsString );
    return TRUE;
}

static void ParserTraverseObjects(ParserStruct* Parser)
{
    IPObjectStruct* Iter;
    const char* Name;
    int
	HasViewMat = FALSE;

    for (Iter = Parser->PObj; Iter; Iter = Iter->Pnext) {
	switch (Iter->ObjType) {
	case IP_OBJ_MATRIX:
	    Name = IP_GET_OBJ_NAME(Iter);
	    if (strcmp(Name, "VIEW_MAT") == 0) {
		IRIT_HMGN_MAT_COPY(Parser -> InternalViewMat, Iter -> U.Mat);
		HasViewMat = TRUE;
	    } else if (strcmp(Name, "PROJ_MAT") == 0)
		IRIT_HMGN_MAT_COPY(Parser -> InternalProjMat, Iter -> U.Mat);
	    continue;
	case IP_OBJ_POLY:
	    ParserHandlePoly(Parser, Iter); /* Process polygonal objects. */
	    continue;
	}
    }

    if (!HasViewMat) {
	ApplyIsometricMatPreservingScaleAndTranslation(Parser -> InternalViewMat);
	CalculateObjectNormalizer(Parser -> PObj, Parser -> InternalViewMat);
    }

}

static void ParserSubmitPointVertex(ParserStruct* Parser, VertexStruct Vert, int IsVector)
{
    IrtRType x, y, z, tx, ty, tz;
    VertexStruct v;
    int i;
    static const double Vectors[4][3] = 
	{
	    { 1,  1,  1},
	    { 1,  1, -1},
	    { 1, -1,  1},
	    { 1, -1, -1},
	};

    memcpy(&v, &Vert, sizeof(Vert));

    x = v.x; y = v.y; z = v.z;
    if (IsVector) {
	v.x = 0; v.y = 0; v.z = 0;
	ParserSubmitLineVertex(Parser, v);

	v.x = x; v.y = y; v.z = z;
	ParserSubmitLineVertex(Parser, v);
    }

    for (i = 0; i < 4; i++) {
	tx = Vectors[i][0] * IritGrapGlblState.PointSize;
	ty = Vectors[i][1] * IritGrapGlblState.PointSize;
	tz = Vectors[i][2] * IritGrapGlblState.PointSize;

	v.x = x + tx; v.y = y + ty; v.z = z + tz;
	ParserSubmitLineVertex(Parser, v);

	v.x = x - tx; v.y = y - ty; v.z = z - tz;
	ParserSubmitLineVertex(Parser, v);
    } 
}

static void ParserHandlePoly(ParserStruct* Parser, IPObjectStruct* PObj)
{
    IPPolygonStruct* PolyIter;
    IPVertexStruct* VertIter;
    double __r, __g, __b, _r, _g, _b, r, g, b, u, v;
    IrtRType x, y, z, pnx, pny, pnz, nx, ny, nz;
    VertexStruct Vert;

    if (IP_IS_POLYGON_OBJ(PObj)) {
	__r = Parser -> Settings.PolygonCol[0];
	__g = Parser -> Settings.PolygonCol[1];
	__b = Parser -> Settings.PolygonCol[2];
    } else {
	__r = Parser -> Settings.PolylineCol[0];
	__g = Parser -> Settings.PolylineCol[1];
	__b = Parser -> Settings.PolylineCol[2];
    }

    GetColor(PObj -> Attr, &__r, &__g, &__b);

    PolyIter = PObj->U.Pl;
    while (PolyIter) {
	_r = __r; _g = __g; _b = __b;
	GetColor(PolyIter -> Attr, &_r, &_g, &_b);

	VertIter = PolyIter->PVertex;
	pnx = pny = pnz = 0;
	if (IP_HAS_PLANE_POLY(PolyIter)) {
	    pnx = PolyIter->Plane[0]; pny = PolyIter->Plane[1]; pnz = PolyIter->Plane[2];
	}

	if (Parser -> Settings.FlipNormalOrient) {
	    pnx = -pnx; pny = -pny; pnz = -pnz;
	}

	do {

	    r = _r; g = _g; b = _b;
	    GetColor(VertIter -> Attr, &r, &g, &b);

	    GetUV(VertIter, &u, &v);
	    nx = pnx; ny = pny; nz = pnz;
	    if (IP_HAS_NORMAL_VRTX(VertIter)) {
		nx = VertIter->Normal[0]; ny = VertIter->Normal[1]; nz = VertIter->Normal[2];
	    }

	    if (Parser -> Settings.FlipNormalOrient) {
		nx = -nx; ny = -ny; nz = -nz;
	    }

	    x = VertIter->Coord[0]; y = VertIter->Coord[1]; z = VertIter->Coord[2];
	    Vert = (VertexStruct){ .x = x, .y = y, .z = z,
				  .nx = nx, .ny = ny, .nz = nz,
				  .r = r, .g = g, .b = b,
				  .u = u, .v = v };

	    if (IP_IS_POLYGON_OBJ(PObj)) {
		ParserSubmitTriangleVertex(Parser, Vert);
		ParserSubmitNormal(Parser, pnx, pny, pnz);
	    }
	    else if (IP_IS_POLYLINE_OBJ(PObj)) {
		ParserSubmitLineVertex(Parser, Vert);
		if (VertIter != PolyIter->PVertex && VertIter->Pnext && VertIter->Pnext != PolyIter->PVertex)
		    ParserSubmitLineVertex(Parser, Vert);
	    } else if(IP_IS_POINTLIST_OBJ(PObj)) 
		ParserSubmitPointVertex(Parser, Vert, strncmp(PObj -> ObjName, "VECTOR", 6) == 0);

	    VertIter = VertIter->Pnext;
	} while (VertIter && VertIter != PolyIter->PVertex);
	PolyIter = PolyIter->Pnext;
    }
}

static void ParserSubmitLineVertex(ParserStruct* Parser, VertexStruct v)
{
    ParserCommandsStruct* Last, * New, ** Curr;
    if (!Parser)
	return;

    if (Parser -> Settings.OverridePolylineCol) {
	v.r = Parser -> Settings.PolylineCol[0];
	v.g = Parser -> Settings.PolylineCol[1];
	v.b = Parser -> Settings.PolylineCol[2];
    } 

    Last = Parser->LineCommands;
    if (Last && Last->VertexCount < 2) {
	Last->Vertices[Last->VertexCount++] = v;
	Parser->NumLines += Last->VertexCount == 2;
	return;
    }

    Curr = Last ? &New : &(Parser->LineCommands);
    *Curr = malloc(sizeof(ParserCommandsStruct));
    assert(*Curr);

    memset(*Curr, 0, sizeof(ParserCommandsStruct));
    (*Curr)->Vertices[(*Curr)->VertexCount++] = v;
    (*Curr)->Next = Last;
    Parser->LineCommands = *Curr;

}

static void ParserSubmitTriangleVertex(ParserStruct* Parser, VertexStruct v)
{
    ParserCommandsStruct* Last, * New, ** Curr;
    if (!Parser)
	return;

    if (Parser -> Settings.OverridePolygonCol) {
	v.r = Parser -> Settings.PolygonCol[0];
	v.g = Parser -> Settings.PolygonCol[1];
	v.b = Parser -> Settings.PolygonCol[2];
    } 

    Last = Parser->TriCommands;
    if (Last && Last->VertexCount < 3) {
	Last->Vertices[Last->VertexCount++] = v;
	Parser->NumTriangles += (Last->VertexCount == 3);
	return;
    }

    Curr = Last ? &New : &(Parser->TriCommands);
    *Curr = malloc(sizeof(ParserCommandsStruct));
    assert(*Curr);

    memset(*Curr, 0, sizeof(ParserCommandsStruct));
    (*Curr)->Vertices[(*Curr)->VertexCount++] = v;
    (*Curr)->Next = Last;
    Parser->TriCommands = *Curr;

    Parser -> BBox[0][0] = min(Parser -> BBox[0][0], v.x);
    Parser -> BBox[0][1] = min(Parser -> BBox[0][1], v.y);
    Parser -> BBox[0][2] = min(Parser -> BBox[0][2], v.z);

    Parser -> BBox[1][0] = max(Parser -> BBox[1][0], v.x);
    Parser -> BBox[1][1] = max(Parser -> BBox[1][1], v.y);
    Parser -> BBox[1][2] = max(Parser -> BBox[1][2], v.z);
}

static void ParserSubmitNormal(ParserStruct* Parser, double nx, double ny, double nz)
{
    if (!Parser || !Parser->TriCommands)
	return;

    Parser->TriCommands->nx = nx;
    Parser->TriCommands->ny = ny;
    Parser->TriCommands->nz = nz;
}

static void ParserPostProcess(ParserStruct* Parser)
{
    ParserCommandsStruct *Iter, *Next;
    double x, y, z, r, g, b;
    VertexStruct v;
    int i;

    memset(&v, 0, sizeof(v));

    /* Add face normals. */
    if (Parser -> Settings.DrawPNormal) {
	Iter = Parser -> TriCommands;
	for (;Iter; Iter = Iter -> Next) {
	    x = Iter -> Vertices[0].x + Iter -> Vertices[1].x + Iter -> Vertices[2].x;
	    y = Iter -> Vertices[0].y + Iter -> Vertices[1].y + Iter -> Vertices[2].y;
	    z = Iter -> Vertices[0].z + Iter -> Vertices[1].z + Iter -> Vertices[2].z;

	    r = Iter -> Vertices[0].r + Iter -> Vertices[1].r + Iter -> Vertices[2].r;
	    g = Iter -> Vertices[0].g + Iter -> Vertices[1].g + Iter -> Vertices[2].g;
	    b = Iter -> Vertices[0].b + Iter -> Vertices[1].b + Iter -> Vertices[2].b;

	    x = x / 3.0; y = y / 3.0; z = z / 3.0; 
	    r = r / 3.0; g = g / 3.0; b = b / 3.0; 

	    v.r = r; v.g = g; v.b = b;

	    v.x = x; v.y = y; v.z = z;
	    ParserSubmitLineVertex(Parser, v);

	    v.x = x + Iter -> nx * Parser -> Settings.NormalSize;
	    v.y = y + Iter -> ny * Parser -> Settings.NormalSize;
	    v.z = z + Iter -> nz * Parser -> Settings.NormalSize;
	    ParserSubmitLineVertex(Parser, v);
	}
    }

    /* Add vertex normals. */
    if (Parser -> Settings.DrawVNormal) {
	Iter = Parser -> TriCommands;
	for (;Iter; Iter = Iter -> Next) {
	    for (i = 0; i < Iter -> VertexCount; i++) {
		x = Iter -> Vertices[i].x; y = Iter -> Vertices[i].y; z = Iter -> Vertices[i].z;
		r = Iter -> Vertices[i].r; g = Iter -> Vertices[i].g; b = Iter -> Vertices[i].b;

		v.x = x; v.y = y; v.z = z;
		v.r = r; v.g = g; v.b = b;
		ParserSubmitLineVertex(Parser, v);

		v.x = x + Iter -> Vertices[i].nx * Parser -> Settings.NormalSize;
		v.y = y + Iter -> Vertices[i].ny * Parser -> Settings.NormalSize;
		v.z = z + Iter -> Vertices[i].nz * Parser -> Settings.NormalSize;
		v.r = r; v.g = g; v.b = b;
		ParserSubmitLineVertex(Parser, v);
	    }
	}
    }


    /* Handle wireframe by turning triangles into lines. */
    if (Parser -> Settings.Wireframe) {
	Iter = Parser -> TriCommands;
	for (;Iter; Iter = Next) {
	    Next = Iter -> Next;
	    ParserSubmitLineVertex(Parser, Iter -> Vertices[0]);
	    ParserSubmitLineVertex(Parser, Iter -> Vertices[1]);

	    ParserSubmitLineVertex(Parser, Iter -> Vertices[1]);
	    ParserSubmitLineVertex(Parser, Iter -> Vertices[2]);

	    ParserSubmitLineVertex(Parser, Iter -> Vertices[2]);
	    ParserSubmitLineVertex(Parser, Iter -> Vertices[0]);
	    free(Iter);
	}
	Parser -> TriCommands = NULL;
    }

}

static void SetClippingPlanes(ParserStruct *Parser, IrtHmgnMatType ProjectionMat, double ZMin, double ZMax)
{
    IrtHmgnMatType
	ProjInv, Remapper;
    IrtRType
	 MaxVec[] = {0, 0, 1, 1},
	 MinVec[] = {0, 0, -1, 1};
    IrtRType diff0, diff1, k;

    I2P_LOG_TRACE( "%s: Swapping Zmin, Zmax values: zMin = %f, zMax = %f", Parser -> SettingsString, ZMin, ZMax);

    if (!IritMiscMatInverseMatrix(ProjectionMat, ProjInv)) {
	I2P_LOG_WARN( "%s: Projection matrix is not invertible, could not set new clipping values", Parser -> SettingsString);
	return;
    }

    IritMiscMatMultVecby4by4(MaxVec, MaxVec, ProjInv);
    IritMiscMatMultVecby4by4(MinVec, MinVec, ProjInv);

    if (fabs(MaxVec[3]) < IRIT_EPS)
	MaxVec[3] = IRIT_EPS * (MaxVec[3] < 0 ? -1 : 1);

    if (fabs(MinVec[3]) < IRIT_EPS)
	MinVec[3] = IRIT_EPS * (MinVec[3] < 0 ? -1 : 1);

    IRIT_VEC_SCALE(MaxVec, 1.0 / MaxVec[3]);
    IRIT_VEC_SCALE(MinVec, 1.0 / MinVec[3]);

    I2P_LOG_TRACE( "%s: Computed original zMin zFar from provided projection matrix: zMin = %f, zMax = %f", Parser -> SettingsString, MinVec[2], MaxVec[2]);

    diff0 = MaxVec[2] - MinVec[2];
    diff1 = ZMax - ZMin;

    if (fabs(diff1) < IRIT_EPS)
	diff1 = IRIT_EPS;
    k = diff0 / diff1;

    IritMiscMatGenUnitMat(Remapper);
    Remapper[2][2] = k;
    Remapper[3][2] = MinVec[2] - ZMin * k;

    IritMiscMatMultTwo4by4(ProjectionMat, Remapper, ProjectionMat);
    I2P_LOG_TRACE( "%s: Swapped in the new zMin-zMax values", Parser -> SettingsString); 
}

static MeshStruct* ParserFinalize(ParserStruct* Parser)
{
    MeshStruct* Ret;
    ParserCommandsStruct* Iter, * Next;
    int nv;

    Ret = malloc(sizeof(*Ret));
    if (!Ret) {
	I2P_LOG_ERROR( "%s: Could not allocate finalized object", Parser -> SettingsString);
	return NULL;
    }
    memset(Ret, 0, sizeof(*Ret));

    Ret->TotalVertices = Parser->NumLines * 2 +
	Parser->NumTriangles * 3;

    Ret->Vertices = malloc(sizeof(VertexStruct) * Ret->TotalVertices);
    if (!Ret->Vertices) {
	I2P_LOG_ERROR( "%s: Could not allocate finalized object vertices", Parser -> SettingsString);
	free(Ret);
	return NULL;
    }

    nv = 0;
    for (Iter = Parser->TriCommands; Iter; Iter = Next) {
	Next = Iter->Next;
	Ret->Vertices[nv++] = Iter->Vertices[0];
	Ret->Vertices[nv++] = Iter->Vertices[1];
	Ret->Vertices[nv++] = Iter->Vertices[2];
	free(Iter);
    }

    Ret->VerticesCutoffOffset = nv;
    for (Iter = Parser->LineCommands; Iter; Iter = Next) {
	Next = Iter->Next;
	Ret->Vertices[nv++] = Iter->Vertices[0];
	Ret->Vertices[nv++] = Iter->Vertices[1];
	free(Iter);
    }

    Ret -> Lights = malloc( sizeof(*Ret -> Lights) * I2P_NUM_LIGHTS );

    memcpy(Ret -> Lights, Parser -> Lights, sizeof(Parser -> Lights));

    if (!Ret -> Lights[0].IsEnabled) {
	Ret -> Lights[0].IsEnabled = TRUE;
	Ret -> Lights[0].x = Parser -> Settings.LightPos[0];
	Ret -> Lights[0].y = Parser -> Settings.LightPos[1];
	Ret -> Lights[0].z = Parser -> Settings.LightPos[2];
	Ret -> Lights[0].ar = Parser -> Settings.Ambient;
	Ret -> Lights[0].ag = Parser -> Settings.Ambient;
	Ret -> Lights[0].ab = Parser -> Settings.Ambient;

	Ret -> Lights[0].dr = Parser -> Settings.Diffuse;
	Ret -> Lights[0].dg = Parser -> Settings.Diffuse;
	Ret -> Lights[0].db = Parser -> Settings.Diffuse;

	Ret -> Lights[0].sr = Parser -> Settings.Specular;
	Ret -> Lights[0].sg = Parser -> Settings.Specular;
	Ret -> Lights[0].sb = Parser -> Settings.Specular;

	I2P_LOG_TRACE( "%s: No lights in file detected, using the light provided from the command line.", Parser -> SettingsString );
    } else {
	I2P_LOG_WARN( "%s: Detected lights in file, the light command line argument will be ignored and only the Ambient, Diffuse, Specular inteseties will be read.", Parser -> SettingsString );
    }

    if (Parser -> Settings.ChangeClippingPlanes)
	SetClippingPlanes(Parser, Parser -> InternalProjMat, Parser -> Settings.ZMin, Parser -> Settings.ZMax);

    Ret -> ViewMatrix = malloc(sizeof(double) * 16);
    memcpy(Ret -> ViewMatrix, Parser -> InternalViewMat, sizeof(IrtHmgnMatType));

    Ret -> ProjMatrix = malloc(sizeof(double) * 16);
    memcpy(Ret -> ProjMatrix, Parser -> InternalProjMat, sizeof(IrtHmgnMatType));

    Ret -> ZOffset = Parser -> Settings.ZOffset;

    IritPrsrFreeObjectList(Parser->PObj);
    free(Parser -> SettingsString);
    return Ret;
}

static void BuildArgv(char* String, int* Argc, char **Argv)
{
    char 
	*Context = NULL,
	*Token = NULL;

    Argv[(*Argc)++] = "I2P";
    if (!String)
	return;

    Token = strtok_s(String, " ", &Context);
    while (Token) {
	Argv[(*Argc)++] = Token;
	Token = strtok_s(NULL, " ", &Context);
    }

}

static int GetColor(IPAttributeStruct *Attr, double* r, double* g, double* b)
{
    int Colour, i, ri, gi, bi;
    static int ColourTable[][4] = {
	{0, 0, 0, 0},
	{1, 0, 0, 255},
	{2, 0, 255, 0},
	{3, 0, 255, 255},
	{4, 255, 0, 0},
	{5, 255, 0, 255},
	{6, 50, 0, 0},
	{7, 127, 127, 127},
	{8, 63, 63, 63},
	{9, 0, 0, 255},
	{10, 0, 255, 0},
	{11, 0, 255, 255},
	{12, 255, 0, 0},
	{13, 255, 0, 255},
	{14, 255, 255, 0},
	{15, 255, 255, 255},
	{20, 50, 0, 0},
	{56, 63, 63, 63},
	{57, 0, 0, 255},
	{58, 0, 255, 0},
	{59, 0, 255, 255},
	{60, 255, 0, 0},
	{61, 255, 0, 255},
	{62, 255, 255, 0},
	{63, 255, 255, 255},
	{-1, 0, 0, 0},
    };


    if (IritMiscAttrIDGetRGBColor(Attr, &ri, &gi, &bi)) {
	*r = ri / 255.0;
	*g = gi / 255.0;
	*b = bi / 255.0;
	return TRUE;
    }

    if ((Colour = IritMiscAttrIDGetColor(Attr)) != IP_ATTR_NO_COLOR) {
	for (i = 0; ColourTable[i][0] != -1; i++) {
	    if (Colour == ColourTable[i][0]) {
		*r = ColourTable[i][1] / 255.0;
		*g = ColourTable[i][2] / 255.0;
		*b = ColourTable[i][3] / 255.0;
		return TRUE;
	    }
	}
    }

    return FALSE;
}

static void GetUV(IPVertexStruct* Vert, double* u, double* v)
{
    float* UV;

    if ((UV = IritMiscAttrGetUVAttrib(Vert->Attr, "uvals")) != NULL) {
	*u = UV[0];
	*v = UV[1];
	return;
    }
    *u = 0;
    *v = 0;
}

ITDPARSER_API void ITDParserFree(MeshStruct* Mesh)
{
    if (!Mesh)
	return;
    free(Mesh->Vertices);
    free(Mesh->ViewMatrix);
    free(Mesh->ProjMatrix);
    free(Mesh->Lights);
    free(Mesh);
    I2P_LOG_TRACE( "Freed ITD Parser reousrces.");
}

void ITDParserLog(int Level, const char *File, const char *Function, int Line, const char *Format, ...)
{
    va_list Args;

    if (!GlobalLogger)
	return;

    va_start(Args, Format);

    va_list ArgsCopy;
    va_copy(ArgsCopy, Args);
    size_t Len = vsnprintf(NULL, 0, Format, ArgsCopy);
    va_end(ArgsCopy);

    if (Len < 0) {
	va_end(Args);
	return; 
    }

    char* Buffer = (char*)malloc(Len + 1);
    if (!Buffer) {
	va_end(Args);
	return; 
    }

    vsnprintf(Buffer, Len + 1, Format, Args);
    va_end(Args);

    GlobalLogger(Level, File, Function, Line, Buffer);
    free(Buffer);
}

void ApplyIsometricMatPreservingScaleAndTranslation(IrtHmgnMatType Mat)
{
    int i, j;
    IrtHmgnMatType TmpMat, IsometricMat;
    IRIT_HMGN_MAT_COPY(IsometricMat, DefaultIsometricMat);

    IrtRType
	Scale = IritMiscMatScaleFactorMatrix(Mat) /
      					     IritMiscMatScaleFactorMatrix(IsometricMat);

    IritMiscMatGenMatUnifScale(Scale, TmpMat);
    IritMiscMatMultTwo4by4(IsometricMat, IsometricMat, TmpMat);

    /* Copy the 3 by 3 block of rotation/scale, leaving translation intact. */
    for (i = 0; i < 3; i++)
	for (j = 0; j < 3; j++)
	    Mat[i][j] = IsometricMat[i][j];
}

void CalculateObjectNormalizer(IPObjectStruct *PObj, IrtHmgnMatType Mat)
{
    int BBoxPosWeights;
    IrtRType s;

    /* Precompute proper bounding box to begin transformation with. */
    GMBBBboxStruct BBox;
    IrtVecType Center, Scaling;
    IrtHmgnMatType Mat1, Mat2;

    IritGeomBBComputeBboxObjectList(PObj, &BBox, FALSE);

    IRIT_PT_ADD(Center, BBox.Max, BBox.Min);
    IRIT_PT_SCALE(Center, 0.5);
    IritMiscMatGenMatTrans(-Center[0], -Center[1], -Center[2], Mat1);

    IRIT_PT_SUB(Scaling, BBox.Max, BBox.Min);
    s = IRIT_MAX(Scaling[0], IRIT_MAX(Scaling[1], Scaling[2]));
    if (s < IRIT_EPS)
	s = IRIT_EPS;
    IritMiscMatGenMatUnifScale(1.0 / s, Mat2);

    IritMiscMatMultTwo4by4(Mat1, Mat1, Mat2);
    IritMiscMatMultTwo4by4(Mat, Mat1, Mat);
}
