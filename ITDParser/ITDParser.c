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
	    n = PopulateGeomAux(Mesh, Iter, &vc, &pc); 
	    Mesh -> PolygonMeshSizes[pc++] = n;
	}
    }

    for (Iter = PObj, pc = 0; Iter; Iter = Iter -> Pnext) {
	if (IP_IS_POLYLINE_OBJ(Iter)) {
	    n = PopulateGeomAux(Mesh, Iter, &vc, &pc); 
	    Mesh -> PolylineMeshSizes[pc++] = n;
	}
    }

    return ITD_PRSR_SUCCESS;
}

static int PopulateGeomAux(MeshStruct *Mesh, IPObjectStruct *PObj, int *vc)
{
    IPPolygonStruct *PolyIter;
    IPVertexStruct *VertIter;
    int NumVertices;

    PolyIter = PObj -> U.Pl;
    NumVertices = 0;
    while (PolyIter) {
	VertIter = PolyIter -> PVertex;
	do {
	    Mesh -> Vertices[(*vc)++] = (VertexStruct){.x = VertIter -> Coord[0],
						       .y = VertIter -> Coord[1],
						       .z = VertIter -> Coord[2],
						       .nx = PolyIter -> Plane[0],
						       .ny = PolyIter -> Plane[1],
						       .nz = PolyIter -> Plane[2]};
	    NumVertices++;
	    VertIter = VertIter -> Pnext;
	} while (VertIter && VertIter != PolyIter -> PVertex);
	PolyIter = PolyIter -> Pnext;
    }
    return NumVertices;
}
