#include "objectProcessor.h"
#include "inc_irit/irit_sm.h"
#include "inc_irit/iritprsr.h"
#include "inc_irit/allocate.h"
#include "inc_irit/ip_cnvrt.h"
#include "inc_irit/geom_lib.h"
#include "inc_irit/cagd_lib.h"



static IPObjectStruct *TriangulateObject(IPObjectStruct *PObj);
static IPObjectStruct *LoadFromFileAux(const char *FileName);

IPObjectStruct *LoadFromFile(const char *FileName)
{
    IPObjectStruct *Loaded, *Tris;


    Loaded = LoadFromFileAux(FileName);
    Loaded = IritPrsrResolveInstances(Loaded);

    Loaded = IritPrsrFlattenForrest(Loaded, TRUE);
    Tris = TriangulateObject(Loaded);
    IritPrsrFreeObjectList(Loaded);

    return Tris;
}


static IPObjectStruct *LoadFromFileAux(const char *FileName)
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
static IPObjectStruct *TriangulateObject(IPObjectStruct *PObj)
{
    IPObjectStruct *Triangulated, *Tmp;

    Triangulated = NULL;
    for (; PObj; PObj = PObj -> Pnext) {
	if (IP_IS_POLY_OBJ(PObj)) {
	    if (IP_IS_POLYGON_OBJ(PObj)) {
		Tmp = IritGeomConvertPolysToTriangles2(PObj);
		IP_SET_OBJ_NAME2(Tmp, IP_GET_OBJ_NAME(PObj));
		IritPrsrCopyObjectAuxInfo(Tmp, PObj);
		IRIT_LIST_PUSH(Tmp, Triangulated);
	    } else if (IP_IS_POLYLINE_OBJ(PObj)) {
		Tmp = IritPrsrCopyObject(NULL, PObj, TRUE);
		IRIT_LIST_PUSH(Tmp, Triangulated);
	    }
	} else if (IP_IS_MAT_OBJ(PObj)) {
	    Tmp = IritPrsrCopyObject(NULL, PObj, TRUE);
	    IRIT_LIST_PUSH(Tmp, Triangulated);
	}
    }
    return Triangulated;
}

IPObjectStruct *IritPrsrProcessFreeForm(IPFreeFormStruct *FreeForms)
{
    IPObjectStruct *Object;
    IPPolygonStruct *Poly;
    CagdSrf2PlsInfoStrct TessInfo;

    for (Object = FreeForms -> CrvObjs;
         Object != NULL;
         Object = Object -> Pnext) {
        CagdCrvStruct *Curves, *Curve;

        Curves = Object -> U.Crvs;
        Object -> U.Pl = NULL;
        Object -> ObjType = IP_OBJ_POLY;
        IP_SET_POLYLINE_OBJ(Object);
        for (Curve = Curves; Curve != NULL; Curve = Curve -> Pnext) {
            Poly = IritPrsrCurve2Polylines(Curve,
                                     32,
                                     SYMB_CRV_APPROX_UNIFORM);
            Object -> U.Pl = IritPrsrAppendPolyLists(Poly, Object -> U.Pl);
        }
        IritCagdCrvFreeList(Curves);
    }

    for (Object = FreeForms -> SrfObjs;
         Object != NULL;
         Object = Object -> Pnext) {
        IrtRType RelativeFineNess;
        CagdSrfStruct *Surfaces, *Surface;

        RelativeFineNess = IritMiscAttrIDGetObjectRealAttrib(Object,
						     IRIT_ATTR_CREATE_ID(resolution));
        if (IP_ATTR_IS_BAD_REAL(RelativeFineNess))
            RelativeFineNess = 1.0;

        IritPrsrTSrf2PlysInitTessInfo2(&TessInfo, FALSE,
			         RelativeFineNess * 32,
			         TRUE, TRUE, FALSE, NULL);

        Surfaces = Object -> U.Srfs;
        Object -> U.Pl = NULL;
        Object -> ObjType = IP_OBJ_POLY;
        IP_SET_POLYGON_OBJ(Object);
        for (Surface = Surfaces; Surface != NULL; Surface = Surface -> Pnext) {
            IrtRType t;

            t = IritMiscAttrIDGetObjectRealAttrib(Object,
					  IRIT_ATTR_CREATE_ID(u_resolution));
            if (!IP_ATTR_IS_BAD_REAL(t))
	        IritMiscAttrIDSetRealAttrib(&Surface -> Attr,
				    IRIT_ATTR_CREATE_ID(u_resolution), t);
            t = IritMiscAttrIDGetObjectRealAttrib(Object,
					  IRIT_ATTR_CREATE_ID(v_resolution));
            if (!IP_ATTR_IS_BAD_REAL(t))
	        IritMiscAttrIDSetRealAttrib(&Surface -> Attr,
				    IRIT_ATTR_CREATE_ID(v_resolution), t);

            Poly = IritPrsrSurface2Polygons(Surface, &TessInfo);
            if (Poly != NULL) {
                IritPrsrGetLastPoly(Poly) -> Pnext = Object -> U.Pl;
                Object -> U.Pl = Poly;
            }
        }
        IritMiscAttrIDSetObjectObjAttrib(Object, IRIT_ATTR_CREATE_ID(OrigSrf),
			         IritPrsrGenSRFObject(Surfaces), FALSE);
    }

    /* Converts models, if any, to freeform trimmed surfaces, in place. */
    IritPrsrProcessModel2TrimSrfs(FreeForms);

    for (Object = FreeForms -> TrimSrfObjs;
         Object != NULL;
         Object = Object -> Pnext) {
        IrtRType RelativeFineNess;
        TrimSrfStruct *TrimSrfs, *TrimSrf;

        RelativeFineNess = IritMiscAttrIDGetObjectRealAttrib(Object,
						     IRIT_ATTR_CREATE_ID(resolution));
        if (IP_ATTR_IS_BAD_REAL(RelativeFineNess))
            RelativeFineNess = 1.0;

        IritPrsrTSrf2PlysInitTessInfo2(&TessInfo, FALSE,
			         RelativeFineNess * 32,
			         TRUE, TRUE, FALSE, NULL);

        TrimSrfs = Object -> U.TrimSrfs;
        Object -> U.Pl = NULL;
        Object -> ObjType = IP_OBJ_POLY;
        IP_SET_POLYGON_OBJ(Object);
        for (TrimSrf = TrimSrfs; TrimSrf != NULL; TrimSrf = TrimSrf -> Pnext) {
            IrtRType t;

            t = IritMiscAttrIDGetObjectRealAttrib(Object,
				          IRIT_ATTR_CREATE_ID(u_resolution));
            if (!IP_ATTR_IS_BAD_REAL(t))
                IritMiscAttrIDSetRealAttrib(&TrimSrf -> Attr,
				    IRIT_ATTR_CREATE_ID(u_resolution), t);
            t = IritMiscAttrIDGetObjectRealAttrib(Object,
				          IRIT_ATTR_CREATE_ID(v_resolution));
            if (!IP_ATTR_IS_BAD_REAL(t))
                IritMiscAttrIDSetRealAttrib(&TrimSrf -> Attr,
				    IRIT_ATTR_CREATE_ID(v_resolution), t);

            Poly = IritPrsrTrimSrf2Polygons(TrimSrf, &TessInfo);
            if (Poly != NULL) {
                IritPrsrGetLastPoly(Poly) -> Pnext = Object -> U.Pl;
                Object -> U.Pl = Poly;
            }
        }
        IritMiscAttrIDSetObjectObjAttrib(Object, IRIT_ATTR_CREATE_ID(OrigSrf),
				 IritPrsrGenSRFObject(IritCagdSrfCopy(TrimSrfs -> Srf)),
				 FALSE);
        IritTrimSrfFreeList(TrimSrfs);
    }

    for (Object = FreeForms -> TrivarObjs;
         Object != NULL;
         Object = Object -> Pnext) {
        IrtRType RelativeFineNess;

        RelativeFineNess = IritMiscAttrIDGetObjectRealAttrib(Object,
					      IRIT_ATTR_CREATE_ID(resolution));
        if (IP_ATTR_IS_BAD_REAL(RelativeFineNess))
            RelativeFineNess = 1.0;

        IritPrsrTSrf2PlysInitTessInfo2(&TessInfo, FALSE,
			         RelativeFineNess * 32,
			         TRUE, TRUE, FALSE, NULL);

	TrivTVStruct *Trivar,
	    *Trivars = Object -> U.Trivars;

	Object -> U.Pl = NULL;
	Object -> ObjType = IP_OBJ_POLY;
	IP_SET_POLYGON_OBJ(Object);
	for (Trivar = Trivars; Trivar != NULL; Trivar = Trivar -> Pnext) {
	    Poly = IritPrsrTrivar2Polygons(Trivar, &TessInfo);
	    if (Poly != NULL) {
		IritPrsrGetLastPoly(Poly) -> Pnext = Object -> U.Pl;
		Object -> U.Pl = Poly;
	    }
	}
	IritTrivTVFreeList(Trivars);
    }

    for (Object = FreeForms -> TriSrfObjs;
         Object != NULL;
         Object = Object -> Pnext) {
        IrtRType RelativeFineNess;
        TrngTriangSrfStruct *TriSrfs, *TriSrf;

        RelativeFineNess = IritMiscAttrIDGetObjectRealAttrib(Object,
					     IRIT_ATTR_CREATE_ID(resolution));
        if (IP_ATTR_IS_BAD_REAL(RelativeFineNess))
            RelativeFineNess = 1.0;

        IritPrsrTSrf2PlysInitTessInfo2(&TessInfo, FALSE,
			         RelativeFineNess * 32,
			         TRUE, TRUE, FALSE, NULL);

        TriSrfs = Object -> U.TriSrfs;
        Object -> U.Pl = NULL;
        Object -> ObjType = IP_OBJ_POLY;
        IP_SET_POLYGON_OBJ(Object);
        for (TriSrf = TriSrfs; TriSrf != NULL; TriSrf = TriSrf -> Pnext) {
            Poly = IritPrsrTriSrf2Polygons(TriSrf, &TessInfo);
            if (Poly != NULL) {
                IritPrsrGetLastPoly(Poly) -> Pnext = Object -> U.Pl;
                Object -> U.Pl = Poly;
            }
        }
        IritTrngTriSrfFreeList(TriSrfs);
    }

    return IritPrsrConcatFreeForm(FreeForms);
}
