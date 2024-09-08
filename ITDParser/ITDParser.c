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

#define ITD_PRSR_SUCCESS (1)
#define ITD_PRSR_FAILURE (0)
#define ITD_PRSR_MALLOC(Size) ((Size) ? malloc(Size) : NULL)
#define ITD_PRSR_ZAP(Ptr, Size) ((Ptr) && memset(Ptr, 0, sizeof(*(Ptr)) * Size))

static MeshStruct *ITDParserAllocStruct(int NumVertices,
				        int NumPolygonMeshes,
				        int NumPolylineMeshes);
static int CountGeom(IPObjectStruct *PObj, int *Vertecies, int *Polygons, int *Polylines);
static int PopulateGeom(MeshStruct *Mesh, IPObjectStruct *PObj);
static int PopulateGeomAux(MeshStruct *Mesh, IPObjectStruct *PObj, int *vc);

ITDPARSER_API MeshStruct *ITDParserParse(const char *Path)
{
    MeshStruct *Mesh;
    IPObjectStruct *PObj;
    int 
	NumVertices = 0,
	NumPolygonMeshes = 0,
	NumPolylineMeshes = 0;

    PObj = LoadFromFile(Path);
    if (!PObj)
	return NULL;

    if (!CountGeom(PObj, &NumVertices, &NumPolygonMeshes, &NumPolylineMeshes)) {
	IritPrsrFreeObject(PObj);
	return NULL;
    }

    Mesh = ITDParserAllocStruct(NumVertices, NumPolygonMeshes, NumPolylineMeshes);
    if (!Mesh) {
	IritPrsrFreeObject(PObj);
	return NULL;
    }

    if (!PopulateGeom(Mesh, PObj)) {
	IritPrsrFreeObject(PObj);
	ITDParserFree(Mesh);
	return NULL;
    }
    IritPrsrFreeObjectList(PObj);
    return Mesh;
}

ITDPARSER_API void ITDParserFree(MeshStruct *Mesh)
{
    free(Mesh -> PolygonMeshSizes);
    free(Mesh -> PolylineMeshSizes);
    free(Mesh -> Vertices);
    free(Mesh);
}

static MeshStruct *ITDParserAllocStruct(int NumVertices,
				        int NumPolygonMeshes,
				        int NumPolylineMeshes)
{
    MeshStruct
	*Ret = ITD_PRSR_MALLOC(sizeof(*Ret));
    if (!Ret)
	return NULL;

    memset(Ret, 0, sizeof(*Ret));

    /* Written in a way so that NULL is gracefully handled. */

    Ret -> Vertices = ITD_PRSR_MALLOC(sizeof(*Ret -> Vertices) * NumVertices);
    Ret -> TotalVertices = NumVertices;
    ITD_PRSR_ZAP(Ret -> Vertices, Ret -> TotalVertices);

    Ret -> PolygonMeshSizes = ITD_PRSR_MALLOC(sizeof(*Ret -> PolygonMeshSizes) * NumPolygonMeshes);
    Ret -> TotalPolygonMeshes = NumPolygonMeshes;
    ITD_PRSR_ZAP(Ret -> PolygonMeshSizes, Ret -> TotalPolygonMeshes);

    Ret -> PolylineMeshSizes = ITD_PRSR_MALLOC(sizeof(*Ret -> PolylineMeshSizes) * NumPolylineMeshes);
    Ret -> TotalPolylineMeshes = NumPolylineMeshes;
    ITD_PRSR_ZAP(Ret -> PolylineMeshSizes, Ret -> TotalPolylineMeshes);

    /* Verify that all allocations passed. */
    if ((Ret -> TotalVertices && !Ret -> Vertices) ||
	(Ret -> TotalPolygonMeshes && !Ret -> PolygonMeshSizes) ||
	(Ret -> TotalPolylineMeshes && !Ret -> PolylineMeshSizes)) {
	ITDParserFree(Ret);
	return NULL;
    }

    return Ret;
}

static int CountGeom(IPObjectStruct *PObj, int *Vertecies, int *Polygons, int *Polylines)
{
    IPPolygonStruct *PolyIter;
    IPVertexStruct *VertIter;
    if (!PObj || !Vertecies || !Polygons || !Polylines)
	return ITD_PRSR_FAILURE;

    *Vertecies = *Polygons = *Polylines = 0;
    
    for (; PObj; PObj = PObj -> Pnext) {
	PolyIter = PObj -> U.Pl;
	while (PolyIter) {
	    VertIter = PolyIter -> PVertex;
	    do {
		(*Vertecies)++;
		VertIter = VertIter -> Pnext;
	    } while (VertIter && VertIter != PolyIter -> PVertex);
	    PolyIter = PolyIter -> Pnext;
	}
	if (IP_IS_POLYGON_OBJ(PObj))
	    (*Polygons)++;
	else if (IP_IS_POLYLINE_OBJ(PObj))
	    (*Polylines)++;
    }

    return ITD_PRSR_SUCCESS;
}
static int PopulateGeom(MeshStruct *Mesh, IPObjectStruct *PObj)
{
    IPObjectStruct *Iter;
    int vc, pc, n;

    vc = 0;

    /* First populate polygons, then polylines. */    
    for (Iter = PObj, pc = 0; Iter; Iter = Iter -> Pnext) {
	if (IP_IS_POLYGON_OBJ(Iter)) {
	    n = PopulateGeomAux(Mesh, Iter, &vc); 
	    Mesh -> PolygonMeshSizes[pc++] = n;
	}
    }

    for (Iter = PObj, pc = 0; Iter; Iter = Iter -> Pnext) {
	if (IP_IS_POLYLINE_OBJ(Iter)) {
	    n = PopulateGeomAux(Mesh, Iter, &vc); 
	    Mesh -> PolylineMeshSizes[pc++] = n;
	}
    }

    return ITD_PRSR_SUCCESS;
}

static void GetColor(IPObjectStruct *PObj, double *r, double *g, double *b)
{
    int Colour, i, ri, gi, bi;
    static int ColourTable[][4] =  {
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


    if (IritMiscAttrGetObjectRGBColor(PObj, &ri, &gi, &bi)) {
	*r = ri / 255.0;
	*g = bi / 255.0;
	*b = gi / 255.0;
	return;
    }

    if ((Colour = IritMiscAttrGetObjectColor(PObj)) != IP_ATTR_NO_COLOR) {
	for (i = 0; ColourTable[i][0] != -1; i++) {
	    if (Colour == ColourTable[i][0]) {
		*r = ColourTable[i][1] / 255.0;
		*g = ColourTable[i][2] / 255.0;
		*b = ColourTable[i][3] / 255.0;
		return;
	    }
	}
    }

    *r = 1.0;
    *g = 0;
    *b = 0;
}

static void GetUV(IPVertexStruct *Vert, double *u, double *v)
{
    float *UV;

    if ((UV = IritMiscAttrGetUVAttrib(Vert -> Attr, "uvals") != NULL)) {
	*u = UV[0];
	*v = UV[1];
	return;
    }
    *u = 0;
    *v = 0;
}

static int PopulateGeomAux(MeshStruct *Mesh, IPObjectStruct *PObj, int *vc)
{
    IPPolygonStruct *PolyIter;
    IPVertexStruct *VertIter;
    double r, g, b, u, v;
    int NumVertices;

    PolyIter = PObj -> U.Pl;

    GetColor(PObj, &r, &g, &b);
    NumVertices = 0;
    while (PolyIter) {
	VertIter = PolyIter -> PVertex;
	GetUV(VertIter, &u, &v);
	do {
	    /* Flip y and z. */
	    Mesh -> Vertices[(*vc)++] = (VertexStruct){.x = VertIter -> Coord[0],
						       .y = VertIter -> Coord[2],
						       .z = VertIter -> Coord[1],
						       .nx = VertIter -> Normal[0],
						       .ny = VertIter -> Normal[2],
						       .nz = VertIter -> Normal[1],
						       .r = r, .g = g, .b = b,
						       .u = u, .v = v};
	    NumVertices++;
	    VertIter = VertIter -> Pnext;
	} while (VertIter && VertIter != PolyIter -> PVertex);
	PolyIter = PolyIter -> Pnext;
    }
    return NumVertices;
}
