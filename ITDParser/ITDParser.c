#include "inc_irit/irit_sm.h"
#include "inc_irit/allocate.h"
#include "inc_irit/iritprsr.h"
#include "ITDParser.h"
#include <stdlib.h>

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

static void CreateCubeTopology(int *Topology)
{
    /*     
       4----0
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
}


ITDPARSER_API MeshStruct *ITDParserParse(const char *Path)
{
    MeshStruct
	*Ret = malloc(sizeof(*Ret));
    if (!Ret)
	return NULL;
    memset(Ret, 0, sizeof(*Ret));
    Ret -> Vertices = malloc(sizeof(*Ret -> Vertices) * 24);
    Ret -> TotalVertices = 24;
    CreateCube(Ret -> Vertices);

    Ret -> PolygonIndices = malloc(sizeof(*Ret -> PolygonIndices) * 36);
    Ret -> PolygonMeshSizes = malloc(sizeof(*Ret -> PolygonMeshSizes));
    Ret -> PolygonMeshSizes[0] = 36;
    CreateCubeTopology(Ret -> PolygonIndices);

    Ret -> TotalPolygonIndices = 36;
    Ret -> TotalPolygonMeshes = 1;

    Ret -> PolylineIndices = Ret -> PolylineMeshSizes = NULL;
    Ret -> TotalPolylineIndices = 0;
    Ret -> TotalPolylineMeshes = 0;

    IPObjectStruct *PObj = IritPrsrAllocObject("hello", 0, NULL);
    return Ret;
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
