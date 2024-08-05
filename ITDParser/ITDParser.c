#include "inc_irit/irit_sm.h"
#include "inc_irit/allocate.h"
#include "inc_irit/iritprsr.h"
#include "ITDParser.h"
#include <stdlib.h>
#include <memory.h>

#define ITD_PARSER_SUCCESS (1)
#define ITD_PARSER_FAILURE (0)

static MeshStruct *ITDParserAllocStruct(int NumVertices,
				        int NumPolygonIndices,
				        int NumPolygonMeshes,
				        int NumPolylineIndices,
				        int NumPolylineMeshes);
static void CreateCube(VertexStruct *Vertices);
static void CreateCubeTopology(int *Topology, int *Sizes);

ITDPARSER_API MeshStruct *ITDParserParse(const char *Path)
{
    MeshStruct
	*Mesh = ITDParserAllocStruct(24, 36, 1, 0, 0);
    if (!Mesh)
	return NULL;
    CreateCube(Mesh -> Vertices);
    CreateCubeTopology(Mesh -> PolygonIndices, Mesh -> PolygonMeshSizes);
    return Mesh;
}

ITDPARSER_API void ITDParserFree(MeshStruct *Mesh)
{
    free(Mesh -> PolygonIndices);
    free(Mesh -> PolygonMeshSizes);

    free(Mesh -> PolylineIndices);
    free(Mesh -> PolylineMeshSizes);

    free(Mesh -> Vertices);
    free(Mesh);
}

static MeshStruct *ITDParserAllocStruct(int NumVertices,
				        int NumPolygonIndices,
				        int NumPolygonMeshes,
				        int NumPolylineIndices,
				        int NumPolylineMeshes)
{
    MeshStruct
	*Ret = malloc(sizeof(*Ret));
    if (!Ret)
	return NULL;
    memset(Ret, 0, sizeof(*Ret));

    Ret -> Vertices = malloc(sizeof(*Ret -> Vertices) * NumVertices);
    Ret -> TotalVertices = NumVertices;
    if (!Ret -> Vertices) {
	ITDParserFree(Ret);
	return NULL;
    }
    memset(Ret -> Vertices, 0, sizeof(*Ret -> Vertices) * Ret -> TotalVertices);

    Ret -> PolygonIndices = malloc(sizeof(*Ret -> PolygonIndices) *
						     NumPolygonIndices);
    Ret -> TotalPolygonIndices = NumPolygonIndices;
    if (!Ret  -> PolygonIndices) {
	ITDParserFree(Ret);
	return NULL;
    }
    memset(Ret -> PolygonIndices, 0,
	     sizeof(*Ret -> PolygonIndices) *
		 Ret -> TotalPolygonIndices);

    Ret -> PolylineIndices = malloc(sizeof(*Ret -> PolylineIndices) *
						     NumPolylineIndices);
    Ret -> TotalPolylineIndices = NumPolylineIndices;
    if (!Ret  -> PolylineIndices) {
	ITDParserFree(Ret);
	return NULL;
    }
    memset(Ret -> PolylineIndices, 0,
	     sizeof(*Ret -> PolylineIndices) *
			 Ret -> TotalPolylineIndices);

    Ret -> PolygonMeshSizes = malloc(sizeof(*Ret -> PolygonMeshSizes) *
						NumPolygonMeshes);
    Ret -> TotalPolygonMeshes = NumPolygonMeshes;
    if (!Ret  -> PolygonMeshSizes) {
	ITDParserFree(Ret);
	return NULL;
    }
    memset(Ret -> PolygonMeshSizes, 0,
	     sizeof(*Ret -> PolygonMeshSizes) *
		     Ret -> TotalPolygonMeshes);

    Ret -> PolylineMeshSizes = malloc(sizeof(*Ret -> PolylineMeshSizes) *
					     NumPolylineMeshes);
    Ret -> TotalPolylineMeshes = NumPolylineMeshes;
    if (!Ret  -> PolylineMeshSizes) {
	ITDParserFree(Ret);
	return NULL;
    }
    memset(Ret -> PolylineMeshSizes, 0,
	 sizeof(*Ret -> PolylineMeshSizes) *
	     Ret -> TotalPolylineMeshes);
    return Ret;
}

static void CreateCube(VertexStruct *Vertices)
{
    int i;
    double Positions[] = {
	 1.0,  1.0,  1.0,
	 1.0,  1.0, -1.0,
	 1.0, -1.0,  1.0,
	 1.0, -1.0, -1.0,
	-1.0,  1.0,  1.0,
	-1.0,  1.0, -1.0,
	-1.0, -1.0,  1.0,
	-1.0, -1.0, -1.0,
    },
    Normals[] = {
	
	 0.0,  0.0,  1.0,
	 0.0,  1.0,  0.0,
	 1.0,  0.0,  0.0,

	 0.0,  0.0, -1.0,
	 0.0,  1.0,  0.0,
	 1.0,  0.0,  0.0,

	 0.0,  0.0,  1.0,
	 0.0, -1.0,  0.0,
	 1.0,  0.0,  0.0,

	 0.0,  0.0, -1.0,
	 0.0, -1.0,  0.0,
	 1.0,  0.0,  0.0,

	 0.0,  0.0,  1.0,
	 0.0,  1.0,  0.0,
	-1.0,  0.0,  0.0,

	 0.0,  0.0, -1.0,
	 0.0,  1.0,  0.0,
	-1.0,  0.0,  0.0,

	 0.0,  0.0,  1.0,
	 0.0, -1.0,  0.0,
	-1.0,  0.0,  0.0,

	 0.0,  0.0, -1.0,
	 0.0, -1.0,  0.0,
	-1.0,  0.0,  0.0,
    };

    for (i = 0; i < 24; i++) {
	Vertices[i] = (VertexStruct){.x = Positions[(i / 3) * 3 + 0],
				     .y = Positions[(i / 3) * 3 + 1],
				     .z = Positions[(i / 3) * 3 + 2],
				     .nx = Normals[i * 3 + 0],
				     .ny = Normals[i * 3 + 1],
				     .nz = Normals[i * 3 + 2]};;
     }
				     
}

static void CreateCubeTopology(int *Topology, int *Sizes)
{
    /*     
       4----0 Add 1 to go to back side.
       |    |
       6----2
    */

    int Arr[36] = {
	    0 * 3 + 0, 2 * 3 + 0, 6 * 3 + 0,
	    0 * 3 + 0, 6 * 3 + 0, 4 * 3 + 0,

	    1 * 3 + 2, 3 * 3 + 2, 2 * 3 + 2,
	    1 * 3 + 2, 2 * 3 + 2, 0 * 3 + 2,

	    4 * 3 + 2, 6 * 3 + 2, 7 * 3 + 2,
	    4 * 3 + 2, 7 * 3 + 2, 5 * 3 + 2,

	    1 * 3 + 1, 0 * 3 + 1, 4 * 3 + 1,
	    1 * 3 + 1, 4 * 3 + 1, 5 * 3 + 1,

	    2 * 3 + 1, 3 * 3 + 1, 7 * 3 + 1,
	    2 * 3 + 1, 7 * 3 + 1, 6 * 3 + 1,

	    5 * 3 + 0, 7 * 3 + 0, 3 * 3 + 0,
	    5 * 3 + 0, 3 * 3 + 0, 1 * 3 + 0,
    };

    memcpy(Topology, Arr, sizeof(Arr));
    Sizes[0] = sizeof(Arr) / sizeof(*Arr);
}
