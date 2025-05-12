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

#define ITD_PRSR_SUCCESS (1)
#define ITD_PRSR_FAILURE (0)
#define ITD_PRSR_MALLOC(Size) ((Size) ? malloc(Size) : NULL)
#define ITD_PRSR_ZAP(Ptr, Size) ((Ptr) && memset(Ptr, 0, sizeof(*(Ptr)) * Size))

static void ParserHandleArgs(ParserStruct* Parser, char* Settings);
static void ParserTraverseObjects(ParserStruct* Parser);
static void ParserHandlePoly(ParserStruct* Parser, IPObjectStruct* PObj);
static void ParserPostProcess(ParserStruct* Parser);
static void ParserSubmitNormal(ParserStruct* Parser, double nx, double ny, double nz);
static void ParserSubmitLineVertex(ParserStruct* Parser, VertexStruct v);
static void ParserSubmitTriangleVertex(ParserStruct* Parser, VertexStruct v);
static void ParserSubmitPointVertex(ParserStruct* Parser, VertexStruct v);
static MeshStruct* ParserFinalize(ParserStruct* Parser);
static void GetUV(IPVertexStruct* Vert, double* u, double* v);
static int GetColor(IPAttributeStruct* Attr, double* r, double* g, double* b);
static void BuildArgv(const char* String, int* Argc, char **Argv);

IRIT_STATIC_DATA const char
* ConfigStr = "I2P n%- N%- I%-#IsoLines!s F%-PlgnOpti|PlgnFineNess!d!F f%-PllnOpti|PllnFineNess!d!F L%-Normal|Size!F b%-r,g,b|(background)!s c%-r,g,b|(Polyline)!s C%-r,g,b|(Polygon)!s W%-Wiresetup!d L%-x,y,z|(Light)!s p%-Point|Size!F Z%-ZMin|ZMax!F!F M%- V%- P%- t%- o%- w%-";
IRIT_STATIC_DATA const ParserSettingsStruct
    DefaultSettings = {
	0, 0, /* VNormal, PNormal */
	{10, 10, 10}, /* Isolines */
	{0, 0, 0}, /* Background Colour */
	FALSE, {0, 0, 0}, /* OverridePolylineCol - Polyline Colour */
	FALSE, {0, 0, 0}, /* OverridePolygonCol - Polygon Colour */
	FALSE, /* PolygonOptiApprox */
	SYMB_CRV_APPROX_CURVATURE, /* PolylineOptiApprox */
	FALSE, /* DrawSurfacePoly */
	FALSE, /* DrawSurfaceMesh */
	FALSE, /* DrawModelsMonolithic */
	FALSE, /* DrawSurfaceOrient */
	0, /* DrawWireSetup */
	FALSE, /* FlipNormalOrient */
	FALSE, /* Wireframe */
	1, /* NormalSize */
	40, /* PlgnFineness */
	40, /* PllnFineness */
	{1, 1, 1}, /* LightPos */
	0.05, /* PointSize */
	10, 100 /* ZMin, ZMax */
    };

ITDPARSER_API MeshStruct* ITDParserParse(const char* Path, const char* Settings)
{
    ParserStruct Parser;

    memset(&Parser, 0, sizeof(Parser));
    ParserHandleArgs(&Parser, Settings);
    LoadFromFile(&Parser, Path);

    ParserTraverseObjects(&Parser);
    ParserPostProcess(&Parser);
    return ParserFinalize(&Parser);
}

static void ParserHandleArgs(ParserStruct* Parser, char* Settings)
{
    int 
	IsolineFlag = FALSE,
	PolygonFinenessFlag = FALSE,
	PolylineFinenessFlag = FALSE,
	NormalSizeFlag = FALSE,
	BackgroundColourFlag = FALSE,
	PolygonColourFlag = FALSE,
	PolylineColourFlag = FALSE,
	SurfaceWireSetupFlag = FALSE,
	LightPosFlag = FALSE,
	PointSizeFlag = FALSE,
	ZMinMaxFlag = FALSE;
    char
	*IsolineString = NULL,
	*BackgroundString = NULL,
	*PolygonString = NULL,
	*PolylineString = NULL,
	*LightPosString = NULL;
    ParserSettingsStruct* PSettings = &(Parser->Settings);
    int Err;
    char *Argv[1024];
    int Argc;

    Argc = 0;
    memset(Argv, NULL, sizeof(Argv));
    BuildArgv(Settings, &Argc, Argv);

    memcpy(PSettings, &DefaultSettings, sizeof(DefaultSettings));
    if ((Err = IritMiscGetArgs(Argc, Argv, ConfigStr,
	&PSettings->DrawVNormal, &PSettings->DrawPNormal,
	&IsolineFlag, &IsolineString,
	&PolygonFinenessFlag, &PSettings->PolygonOptiApprox, &PSettings->PlgnFineness,
	&PolylineFinenessFlag, &PSettings->PolylineOptiApprox, &PSettings->PllnFineness,
	&NormalSizeFlag, &PSettings->NormalSize,
	&BackgroundColourFlag, &BackgroundString,
	&PolylineColourFlag, &PolylineString,
	&PolygonColourFlag, &PolygonString,
	&SurfaceWireSetupFlag, &PSettings->SurfaceWireSetup,
	&LightPosFlag, &LightPosString,
	&PointSizeFlag, &PSettings->PointSize,
	&ZMinMaxFlag, &PSettings->ZMin, &PSettings->ZMax,
	&PSettings -> DrawSurfaceMesh,
	&PSettings -> DrawModelsMonolithic,
	&PSettings -> DrawSurfacePoly,
	&PSettings -> DrawSurfaceOrient,
	&PSettings -> FlipNormalOrient,
	&PSettings -> Wireframe, NULL))) {
	return;
    }

    if (!SurfaceWireSetupFlag)
	PSettings -> SurfaceWireSetup = 0;

    if (BackgroundColourFlag) {
	if (BackgroundString &&
	    (sscanf(BackgroundString, "%d,%d,%d", &PSettings -> Background[0],
	     &PSettings -> Background[1],
	     &PSettings -> Background[2]) != 3) &&
	    (sscanf(BackgroundString, "%d %d %d", &PSettings -> Background[0],
		&PSettings -> Background[1],
		&PSettings -> Background[2]) != 3)) {
	    return;
	}
    }

    if (PolylineColourFlag) {
	PSettings -> OverridePolylineCol = TRUE;
	if (PolylineString &&
	    ((sscanf(PolylineString, "%d,%d,%d", &PSettings -> PolylineCol[0],
	     &PSettings -> PolylineCol[1],
	     &PSettings -> PolylineCol[2])) != 3) &&
	    (sscanf(PolylineString, "%d %d %d", &PSettings -> PolylineCol[0],
		&PSettings -> PolylineCol[1],
		&PSettings -> PolylineCol[2]) != 3)) {
	    return;
	}
    }

    if (PolygonColourFlag) {
	PSettings -> OverridePolygonCol = TRUE;
	if (PolygonString &&
	    (sscanf(PolygonString, "%d,%d,%d", &PSettings -> PolygonCol[0],
	    &PSettings -> PolygonCol[1],
	    &PSettings -> PolygonCol[2]) != 3) &&
	    (sscanf(PolygonString, "%d %d %d", &PSettings -> PolygonCol[0],
		&PSettings -> PolygonCol[1],
		&PSettings -> PolygonCol[2]) != 3)) {
	    return;
	}
    }

    if (IsolineFlag) {
	if (IsolineString &&
	    (sscanf(IsolineString, "%d,%d,%d", &PSettings->Isolines[0],
	    &PSettings -> Isolines[1],
	    &PSettings -> Isolines[2]) != 3) &&
	    (sscanf(IsolineString, "%d %d %d", &PSettings -> Isolines[0],
		&PSettings -> Isolines[1],
		&PSettings -> Isolines[2]) != 3) &&
	    (sscanf(IsolineString, "%d,%d", &PSettings -> Isolines[0],
		&PSettings -> Isolines[1]) != 2) &&
	    (sscanf(IsolineString, "%d %d", &PSettings -> Isolines[0],
		&PSettings -> Isolines[1]) != 2) &&
	    (sscanf(IsolineString, "%d", &PSettings -> Isolines[0]) != 1)) {
	    return;
	}
    }

    if (LightPosFlag) {
	if (LightPosString &&
	    (sscanf(LightPosString, "%f,%f,%f", &PSettings -> LightPos[0],
	     &PSettings -> LightPos[1],
	     &PSettings -> LightPos[2]) != 3) &&
	    (sscanf(LightPosString, "%f %f %f", &PSettings -> LightPos[0],
		&PSettings -> LightPos[1],
		&PSettings -> LightPos[2]) != 3)) {
	    return;
	}
    }
}

static void ParserTraverseObjects(ParserStruct* Parser)
{
    IPObjectStruct* Iter;
    IrtHmgnMatType Mat;
    const char* Name;

    for (Iter = Parser->PObj; Iter; Iter = Iter->Pnext) {
	switch (Iter->ObjType) {
	case IP_OBJ_MATRIX:
	    memcpy(Mat, Iter->U.Mat, sizeof(Mat));
	    Name = IP_GET_OBJ_NAME(Iter);
	    if (strcmp(Name, "VIEW_MAT") == 0) {
		Parser->ViewMatrix = malloc(sizeof(double) * 16);
		assert(Parser->ViewMatrix);
		memcpy(Parser->ViewMatrix, Mat, sizeof(Mat));
	    }
	    else if (strcmp(Name, "PROJ_MAT") == 0 ||
		     strcmp(Name, "PRSP_MAT") == 0) {
		Parser->ProjMatrix = malloc(sizeof(double) * 16);
		assert(Parser->ProjMatrix);
		memcpy(Parser->ProjMatrix, Mat, sizeof(Mat));
	    }
	    continue;
	case IP_OBJ_POLY:
	    ParserHandlePoly(Parser, Iter);
	    continue;
	}
    }
}

static void ParserSubmitPointVertex(ParserStruct* Parser, VertexStruct Vert)
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

    memcpy(&v, &Vert, sizeof(v));
    x = Vert.x; y = Vert.y; z = Vert.z;
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

    __r = 1.0; __g = 0; __b = 0;
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
		ParserSubmitPointVertex(Parser, Vert);

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

static MeshStruct* ParserFinalize(ParserStruct* Parser)
{
    MeshStruct* Ret;
    GMBBBboxStruct BBox;
    ParserCommandsStruct* Iter, * Next;
    int nv;

    Ret = malloc(sizeof(*Ret));
    if (!Ret)
	return NULL;
    memset(Ret, 0, sizeof(*Ret));

    Ret->TotalVertices = Parser->NumLines * 2 +
	Parser->NumTriangles * 3;

    Ret->Vertices = malloc(sizeof(VertexStruct) * Ret->TotalVertices);
    if (!Ret->Vertices) {
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

    IritGeomBBComputeBboxObjectList(Parser->PObj, &BBox, 0);
    Ret->MinX = BBox.Min[0]; Ret->MinY = BBox.Min[1]; Ret->MinZ = BBox.Min[2];
    Ret->MaxX = BBox.Max[0]; Ret->MaxY = BBox.Max[1]; Ret->MaxZ = BBox.Max[2];

    Ret -> ViewMatrix = Parser -> ViewMatrix;
    Ret -> ProjMatrix = Parser -> ProjMatrix;

    IritPrsrFreeObjectList(Parser->PObj);
    return Ret;
}

static void BuildArgv(const char* String, int* Argc, char **Argv)
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
    free(Mesh);
}

