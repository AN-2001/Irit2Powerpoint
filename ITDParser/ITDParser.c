#include "inc_irit/irit_sm.h"
#include "inc_irit/allocate.h"
#include "inc_irit/iritprsr.h"
#include "ITDParser.h"
#include <stdlib.h>
#include <memory.h>

#define ITD_PRSR_SUCCESS (1)
#define ITD_PRSR_FAILURE (0)
#define ITD_PRSR_MALLOC(Size) ((Size) ? malloc(Size) : NULL)
#define ITD_PRSR_ZAP(Ptr, Size) ((Ptr) && memset(Ptr, 0, sizeof(*(Ptr)) * Size))

static MeshStruct *ITDParserAllocStruct(int NumVertices,
				        int NumPolygonIndices,
				        int NumPolygonMeshes,
				        int NumPolylineIndices,
				        int NumPolylineMeshes);
static IPObjectStruct *LoadObjectFromFile(const char *FileName);
static int CountGeom(IPObjectStruct *PObj, int *Vertex, int *Polygon);
static int PopulateGeom(MeshStruct *Mesh, IPObjectStruct *PObj);

IPObjectStruct *ConvertPolysTriangles(IPObjectStruct *PObj)
{
    IPObjectStruct *PTris;
    int MinWeight = IritMiscAttrIDGetObjectIntAttrib(PObj,
					     IRIT_ATTR_CREATE_ID(MinWeight));

    if (!IP_ATTR_IS_BAD_INT(MinWeight) && MinWeight) {
	/* Do min weighted triangulation. */
	IPPolygonStruct
	    *Tris = IritGeomTriangulatePolygonList(PObj -> U.Pl);

	if (Tris == NULL)
	    return NULL;

	PTris = IritPrsrGenPOLYObject(Tris);
    }
    else
	PTris = IritGeomConvertPolysToTriangles(PObj);

    if (PTris == NULL)
	return NULL;
    return PTris;
}

ITDPARSER_API MeshStruct *ITDParserParse(const char *Path)
{
    MeshStruct *Mesh;
    IPObjectStruct *AsTris, *PObj;
    int 
	NumVertices = 0,
	NumTris = 0;

    IritPrsrSetPolyListCirc(TRUE);
    IritBoolSetHandleCoplanarPoly(TRUE);
    PObj = LoadObjectFromFile(Path);
    if (!PObj)
	return NULL;
    AsTris = ConvertPolysTriangles(PObj);
    IrtHmgnMatType Mat;
    IrtPtType C;

    IRIT_PT_ADD(C, AsTris -> BBox[0], AsTris -> BBox[1]);
    IRIT_PT_SCALE(C, 0.5f);

    C[0] -= 0.5f;
    C[1] -= 0.5f;
    C[2] -= 0.5f;

    IritMiscMatGenMatUnifScaleCenter(1.0, C, Mat);
    AsTris = IritGeomTransformObjectList(AsTris, Mat);
    AsTris = IritGeomTransformObj2UnitSize(AsTris);
    IritPrsrFreeObject(PObj);

    if (!CountGeom(AsTris, &NumVertices, &NumTris)) {
	IritPrsrFreeObject(PObj);
	return NULL;
    }

    Mesh = ITDParserAllocStruct(NumVertices, NumTris * 3, 1, 0, 0);
    if (!Mesh) {
	IritPrsrFreeObject(AsTris);
	return NULL;
    }

    if (!PopulateGeom(Mesh, AsTris)) {
	IritPrsrFreeObject(AsTris);
	ITDParserFree(Mesh);
	return NULL;
    }
    IritPrsrFreeObject(AsTris);
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
	*Ret = ITD_PRSR_MALLOC(sizeof(*Ret));
    if (!Ret)
	return NULL;

    memset(Ret, 0, sizeof(*Ret));

    /* Written in a way so that NULL is gracefully handled. */

    Ret -> Vertices = ITD_PRSR_MALLOC(sizeof(*Ret -> Vertices) * NumVertices);
    Ret -> TotalVertices = NumVertices;
    ITD_PRSR_ZAP(Ret -> Vertices, Ret -> TotalVertices);

    Ret -> PolygonIndices = ITD_PRSR_MALLOC(sizeof(*Ret -> PolygonIndices) * NumPolygonIndices);
    Ret -> TotalPolygonIndices = NumPolygonIndices;
    ITD_PRSR_ZAP(Ret -> PolygonIndices, Ret -> TotalPolygonIndices);

    Ret -> PolylineIndices = ITD_PRSR_MALLOC(sizeof(*Ret -> PolylineIndices) * NumPolylineIndices);
    Ret -> TotalPolylineIndices = NumPolylineIndices;
    ITD_PRSR_ZAP(Ret -> PolylineIndices, Ret -> TotalPolylineIndices);

    Ret -> PolygonMeshSizes = ITD_PRSR_MALLOC(sizeof(*Ret -> PolygonMeshSizes) * NumPolygonMeshes);
    Ret -> TotalPolygonMeshes = NumPolygonMeshes;
    ITD_PRSR_ZAP(Ret -> PolygonMeshSizes, Ret -> TotalPolygonMeshes);

    Ret -> PolylineMeshSizes = ITD_PRSR_MALLOC(sizeof(*Ret -> PolylineMeshSizes) * NumPolylineMeshes);
    Ret -> TotalPolylineMeshes = NumPolylineMeshes;
    ITD_PRSR_ZAP(Ret -> PolylineMeshSizes, Ret -> TotalPolylineMeshes);

    /* Verify that all allocations passed. */
    if ((Ret -> TotalVertices && !Ret -> Vertices) ||
	(Ret -> TotalPolygonIndices && !Ret -> PolygonIndices) ||
	(Ret -> TotalPolylineIndices && !Ret -> PolylineIndices) ||
	(Ret -> TotalPolygonMeshes && !Ret -> PolygonMeshSizes) ||
	(Ret -> TotalPolylineMeshes && !Ret -> PolylineMeshSizes)) {
	ITDParserFree(Ret);
	return NULL;
    }

    return Ret;
}

static IPObjectStruct *LoadObjectFromFile(const char *FileName)
{
    if (FileName != NULL) {
        int Handler, Compressed,
	    GCode = FALSE,
	    Iges = FALSE,
	    Obj = FALSE,
	    Stl = FALSE,
	    Vtu = FALSE;
        char FullFileName[IRIT_LINE_LEN_VLONG];
	const char
	    *FType = IritPrsrGetDataFileType(FileName, &Compressed);

	strcpy(FullFileName, FileName);

	if (stricmp(FType, "igs") == 0 || stricmp(FType, "iges") == 0) {
	    /* Iges data file is requested. */
	    if (Compressed) {
		return NULL;
	    }
	    Iges = TRUE;
	}
	else if (stricmp(FType, "stl") == 0) {
	    /* STL data file is requested. */
	    if (Compressed) {
		return NULL;
	    }
	    Stl = TRUE;
	}
	else if (stricmp(FType, "vtu") == 0) {
	    /* STL data file is requested. */
	    if (Compressed) {
		return NULL;
	    }
	    Vtu = TRUE;
	}
	else if (stricmp(FType, "obj") == 0) {
	    /* STL data file is requested. */
	    if (Compressed) {
		return NULL;
	    }
	    Obj = TRUE;
	}
	else if (stricmp(FType, "nc") == 0 ||
		 stricmp(FType, "cnc") == 0 ||
		 stricmp(FType, "gcode") == 0) {
	    /* CNC G-code data file is requested. */
	    if (Compressed) {
		return NULL;
	    }
	    GCode = TRUE;
	}
	else if (stricmp(FType, IRIT_TEXT_DATA_FILE) != 0 &&
		 stricmp(FType, IRIT_BINARY_DATA_FILE) != 0 &&
                 stricmp(FType, IRIT_COMPRESSED_DATA_FILE) != 0 &&
		 stricmp(FType, IRIT_MATRIX_DATA_FILE) != 0) {
	    strcat(FullFileName, ".");
	    strcat(FullFileName, IRIT_TEXT_DATA_FILE);
	}

	if (Iges) {
	    IPIgesLoadDfltFileParamsStruct Params;
	    
	    Params = IritPrsrIgesLoadDfltParams;
	    Params.DumpAll = FALSE;
	    Params.InverseProjCrvOnSrfs = TRUE;
	    Params.Messages = 1;
	    return IritPrsrIgesLoadFile(FullFileName, &Params);
	}
	else if (Stl) {
	    IPObjectStruct *PObj;
	    IPSTLLoadDfltFileParamsStruct Params;

	    Params = IritPrsrSTLLoadDfltParams;
	    if ((PObj = IritPrsrSTLLoadFile(FullFileName, &Params)) == NULL ||
		PObj -> U.Pl == NULL) {
		if (PObj != NULL)
		    IritPrsrFreeObject(PObj);

		/* Maybe a binary STL or vice versa - try the other option. */
		Params.BinarySTL = !Params.BinarySTL;  /* Flip text/binary. */

		if ((PObj = IritPrsrSTLLoadFile(FullFileName,
						&Params)) != NULL) {
		}
	    }
	    return PObj;
	}
	else if (Vtu) {
	    IPObjectStruct *PObj = NULL;
	    IritPrsrVTKDataStruct
	        *VTUData = IritPrsrVTULoadFile(FullFileName);

	    if (VTUData != NULL) {
		IritPrsrVTUFetchDataParamStruct VTUFetchParams;

		IRIT_ZAP_MEM(&VTUFetchParams,
			     sizeof(IritPrsrVTUFetchDataParamStruct));
		VTUFetchParams.FetchTetras = FALSE;
		VTUFetchParams.FetchTris = TRUE;
		VTUFetchParams.Messages = TRUE;
		VTUFetchParams.SplitLabels = TRUE;
	        PObj = IritPrsrVTU2IritGeom(VTUData, &VTUFetchParams);

		IritPrsrVTUFree(VTUData);
	    }

	    return PObj;
	}
	else if (Obj) {
	    IPOBJLoadDfltFileParamsStruct Params;

	    Params = IritPrsrOBJLoadDfltParams;
	    Params.WarningMsgs = TRUE;
	    Params.WhiteDiffuseTexture = TRUE;
	    Params.IgnoreFullTransp = TRUE;
	    Params.ForceSmoothing = TRUE;
	    return IritPrsrOBJLoadFile(FullFileName, &Params);
	}
	else if (GCode) {
	    IPGcodeLoadDfltFileParamsStruct Params;

	    Params = IritPrsrGcodeLoadDfltParams;
	    return IritPrsrNCGCodeLoadFile(FullFileName, &Params);
	}
	else {
	    if ((Handler = IritPrsrOpenDataFile(FullFileName, TRUE,
						FALSE)) >= 0) {
	        IPObjectStruct
		    *PObj = IritPrsrGetObjects(Handler);

		IritPrsrCloseStream(Handler, TRUE);

		if (PObj == NULL) {
		    const char *ErrorMsg;

		    if (IritPrsrHasError(&ErrorMsg)) {
		    }
		}
		else {
		    PObj -> RefCount = 0;
		    return PObj;
		}
	    }
	}
    }

    return NULL;
}
static int CountGeom(IPObjectStruct *PObj, int *Vertex, int *Polygon)
{
    IPPolygonStruct *PolyIter;
    IPVertexStruct *VertIter, *Tmp;
    if (!PObj || !Vertex || !Polygon)
	return ITD_PRSR_FAILURE;
    *Vertex = *Polygon = 0;
    if (!IP_IS_POLYGON_OBJ(PObj))
	return ITD_PRSR_FAILURE;
    PolyIter = PObj -> U.Pl;
    while (PolyIter) {
	Tmp = VertIter = PolyIter -> PVertex;
	do {
	    (*Vertex)++;
	    VertIter = VertIter -> Pnext;
	} while (VertIter && VertIter != Tmp);
	(*Polygon)++;
	PolyIter = PolyIter -> Pnext;
    }
    return ITD_PRSR_SUCCESS;
}
static int PopulateGeom(MeshStruct *Mesh, IPObjectStruct *PObj)
{
    IPPolygonStruct *PolyIter;
    IPVertexStruct *VertIter, *Tmp;
    int vk, lk;
    PolyIter = PObj -> U.Pl;
    vk = lk = 0;
    while (PolyIter) {
	Tmp = VertIter = PolyIter -> PVertex;
	do {
	    Mesh -> Vertices[vk++] = (VertexStruct){.x = VertIter -> Coord[0],
	                                            .y = VertIter -> Coord[1],
	                                            .z = VertIter -> Coord[2],
	                                            .nx = PolyIter -> Plane[0] ,
	                                            .ny = PolyIter -> Plane[1],
	                                            .nz = PolyIter -> Plane[2]};
	  VertIter = VertIter -> Pnext;
	} while (VertIter && VertIter != Tmp);
	Mesh -> PolygonIndices[lk + 0] = lk + 0;
	Mesh -> PolygonIndices[lk + 1] = lk + 1;
	Mesh -> PolygonIndices[lk + 2] = lk + 2;
	lk = lk + 3;
	PolyIter = PolyIter -> Pnext;
    }
    Mesh -> PolygonMeshSizes[0] = Mesh -> TotalPolygonIndices;
    return ITD_PRSR_SUCCESS;
}
