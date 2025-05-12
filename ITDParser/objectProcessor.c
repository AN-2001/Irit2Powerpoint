#include "objectProcessor.h"
#include "inc_irit/irit_sm.h"
#include "inc_irit/grap_lib.h"
#include "inc_irit/iritprsr.h"
#include "inc_irit/allocate.h"
#include "inc_irit/ip_cnvrt.h"
#include "inc_irit/geom_lib.h"
#include "inc_irit/cagd_lib.h"
#include "ITDParser.h"
#include <Windows.h>


extern DWORD TLSIndex;
extern HANDLE gMutex;

static void ProcessObjectList(IPObjectStruct *PObj);
static void ApplyGrapSettings(ParserSettingsStruct Settings);
static IPObjectStruct *TriangulateObject(IPObjectStruct *PObj);
static void ProcessMatrix(IPObjectStruct *PObj);
static void ProcessPoint(IPObjectStruct *PObj);
static void ProcessLightSource(IritGrapLightType Pos, IrtVecType Colour);
static IPPolygonStruct *PolyStripToTriangles(IPPolygonStruct *Strip);
static IPObjectStruct *PolyStripToPolyObj(IPObjectStruct *PolyStripObj);

void LoadFromFile(ParserStruct *Parser, const char *FileName)
{
    IPObjectStruct *Loaded, *Processed, *Tris;


    
    WaitForSingleObject(gMutex, INFINITE); 
    IritPrsrSetPolyListCirc(TRUE);
    IritPrsrSetFlattenObjects(FALSE);
    IritPrsrSetPropagateAttrs(FALSE);
    IritPrsrFlattenInvisibleObjects(FALSE);
    IritGrapDSetDrawPolyFunc( ProcessPolygon );
    ReleaseMutex(gMutex);

    Loaded = IritPrsrGetObjects2(FileName);
    Loaded = IritPrsrResolveInstances(Loaded);

    TlsSetValue(TLSIndex, NULL);
    
    WaitForSingleObject(gMutex, INFINITE); 
    ApplyGrapSettings(Parser -> Settings);
    ProcessObjectList(Loaded);
    ReleaseMutex(gMutex);

    Processed = (IPObjectStruct*)TlsGetValue(TLSIndex);

    Parser -> PObj = TriangulateObject(Processed);

    IritPrsrFreeObjectList(Loaded);
    IritPrsrFreeObjectList(Processed);
}

static IPObjectStruct *TriangulateObject(IPObjectStruct *PObj)
{
    IPObjectStruct *Triangulated, *Tmp;

    Triangulated = NULL;
    for (; PObj; PObj = PObj -> Pnext) {
	if (IP_IS_POLY_OBJ(PObj) && IP_IS_POLYGON_OBJ(PObj)) {
	    Tmp = IritGeomConvertPolysToTriangles2(PObj);
	    IritPrsrCopyObjectAuxInfo(Tmp, PObj);
	    IRIT_LIST_PUSH(Tmp, Triangulated);
	} else {
	    Tmp = IritPrsrCopyObject2(NULL, PObj, FALSE, TRUE);
	    IRIT_LIST_PUSH(Tmp, Triangulated);
	}
    }
    return Triangulated;
}

static void ApplyGrapSettings(ParserSettingsStruct Settings)
{
    IritGrapGlblState.DrawVNormal = Settings.DrawVNormal;
    IritGrapGlblState.DrawPNormal = Settings.DrawPNormal;
    IritGrapGlblState.NumOfIsolines[0] = Settings.Isolines[0];
    IritGrapGlblState.NumOfIsolines[1] = Settings.Isolines[1];
    IritGrapGlblState.NumOfIsolines[2] = Settings.Isolines[2];
    IritGrapGlblState.PolygonOptiApprox = Settings.PolygonOptiApprox;
    IritGrapGlblState.PolylineOptiApprox = Settings.PolylineOptiApprox;
    IritGrapGlblState.DrawSurfacePoly = Settings.DrawSurfacePoly;
    IritGrapGlblState.DrawSurfaceMesh = Settings.DrawSurfaceMesh;
    IritGrapGlblState.DrawModelsMonolithic = Settings.DrawModelsMonolithic;
    IritGrapGlblState.DrawSurfaceOrient = Settings.DrawSurfaceOrient;
    IritGrapGlblState.FlipNormalOrient = Settings.FlipNormalOrient;
    IritGrapGlblState.NormalSize = Settings.NormalSize;
    IritGrapGlblState.PlgnFineness = Settings.PlgnFineness;
    IritGrapGlblState.PllnFineness = Settings.PllnFineness;
    IritGrapGlblState.PointSize = Settings.PointSize;
    if (Settings.SurfaceWireSetup) {
        IritGrapGlblState.DrawSurfaceWire = Settings.SurfaceWireSetup & 0x01;
        IritGrapGlblState.DrawSurfaceBndry = Settings.SurfaceWireSetup & 0x02;
        IritGrapGlblState.DrawSurfaceSilh = Settings.SurfaceWireSetup & 0x04;
        IritGrapGlblState.DrawSurfaceSketch = Settings.SurfaceWireSetup & 0x08;
        IritGrapGlblState.DrawSurfaceRflctLns = Settings.SurfaceWireSetup & 0x10;
        IritGrapGlblState.DrawSurfaceKnotLns = Settings.SurfaceWireSetup & 0x20;
    }

}

static void ProcessObjectList(IPObjectStruct *PObj)
{
    IPObjectStruct *Tmp, *Processed;
    const CagdRType *R;
    IrtPtType Pt;
    IPObjectStruct *PTmp;
    int i;

    for (; PObj; PObj = PObj -> Pnext) {
	switch (PObj -> ObjType) {
	    case IP_OBJ_MATRIX:
		ProcessMatrix(PObj);
		break;
	    case IP_OBJ_POLY:
		ProcessPolygon(PObj);
		break;
	    case IP_OBJ_CTLPT:
		/* Coerce, in place, a control points to a regular point. */
		R = PObj->U.CtlPt.Coords;
		IritCagdCoercePointTo(Pt, CAGD_PT_E3_TYPE,
		    (CagdRType* const*)&R,
		    -1, PObj->U.CtlPt.PtType);
		IRIT_PT_COPY(PObj->U.Pt, Pt);
		PObj->ObjType = IP_OBJ_POINT;
	    case IP_OBJ_POINT:
		if (!IP_ATTR_IS_BAD_INT(IritMiscAttrIDGetObjectIntAttrib(PObj,
		    IRIT_ATTR_CREATE_ID(light_source)))) {
		    if (IP_ATTR_IS_BAD_INT(IritMiscAttrIDGetObjectIntAttrib(PObj,
			IRIT_ATTR_CREATE_ID(activated)))) {
			const char* p;
			int i, Red, Green, Blue;
			IritGrapLightType LightPos;
			IrtVecType LightColor;

			for (i = 0; i < 3; i++)
			    LightPos[i] = (float)PObj->U.Pt[i];
			LightPos[3] = (float)
			    !((p = IritMiscAttrIDGetObjectStrAttrib(PObj,
				IRIT_ATTR_CREATE_ID(type))) != NULL &&
				stricmp(p, "POINT_INFTY"));

			if (IritMiscAttrIDGetObjectRGBColor2(PObj, &Red, &Green, &Blue)) {
			    LightColor[0] = Red / 255.0;
			    LightColor[1] = Green / 255.0;
			    LightColor[2] = Blue / 255.0;
			}
			else {
			    LightColor[0] = LightColor[1] = LightColor[2] = 1.0;
			}

			ProcessLightSource(LightPos, LightColor);

			IritMiscAttrIDSetObjectIntAttrib(PObj,
			    IRIT_ATTR_CREATE_ID(activated),
			    TRUE);
		    }
		}
		else
		    ProcessPoint(PObj);
		break;
	    case IP_OBJ_VECTOR:
		ProcessPoint(PObj);
		break;
	    case IP_OBJ_CURVE:
		if (PObj->U.Crvs == NULL)    /* Can occur in the curve editor. */
		    break;
		if (CAGD_IS_POWER_CRV(PObj->U.Crvs)) {
		    CagdCrvStruct
			* Crv = IritCagdCnvrtPwr2BzrCrv(PObj->U.Crvs);

		    IritCagdCrvFree(PObj->U.Crvs);
		    PObj->U.Crvs = Crv;
		}
		if (CAGD_NUM_OF_PT_COORD(PObj->U.Crvs->PType) == 1) {
		    CagdCrvStruct
			* Crv = IritCagdCoerceCrvTo(PObj->U.Crvs,
			    CAGD_IS_RATIONAL_CRV(PObj->U.Crvs) ? CAGD_PT_P2_TYPE
			    : CAGD_PT_E2_TYPE,
			    FALSE);

		    IritCagdCrvFree(PObj->U.Crvs);
		    PObj->U.Crvs = Crv;
		}

		IritGrapDrawCurve(PObj);
		break;
	    case IP_OBJ_TRIMSRF:
		if (CAGD_IS_POWER_SRF(PObj->U.TrimSrfs->Srf)) {
		    CagdSrfStruct
			* Srf = IritCagdCnvrtPwr2BzrSrf(PObj->U.TrimSrfs->Srf);

		    IritCagdSrfFree(PObj->U.TrimSrfs->Srf);
		    PObj->U.TrimSrfs->Srf = Srf;
		}

		if (PObj->U.TrimSrfs->TrimCrvList == NULL) {
		    CagdSrfStruct
			* Srf = PObj->U.TrimSrfs->Srf;

		    if (IritGrapGlblState.MoreInfo)
			IRIT_INFO_MSG_PRINTF("Trimmed surface \"%s\" with no trimming curves detected\n\tand converted to regular surface\n",
			    IP_GET_OBJ_NAME(PObj));

		    PObj->U.TrimSrfs->Srf = NULL;
		    IritTrimSrfFree(PObj->U.TrimSrfs);
		    PObj->U.Srfs = Srf;
		    PObj->ObjType = IP_OBJ_SURFACE;
		}
		else {
		    IritGrapDrawTrimSrf(PObj);
		    break;
		}
	    case IP_OBJ_SURFACE:
		if (PObj->U.Srfs == NULL)  /* Can occur in the surface editor. */
		    break;
		if (CAGD_IS_POWER_SRF(PObj->U.Srfs)) {
		    CagdSrfStruct
			* Srf = IritCagdCnvrtPwr2BzrSrf(PObj->U.Srfs);

		    IritCagdSrfFree(PObj->U.Srfs);
		    PObj->U.Srfs = Srf;
		}
		if (CAGD_NUM_OF_PT_COORD(PObj->U.Srfs->PType) == 1) {
		    CagdSrfStruct
			* Srf = IritCagdCoerceSrfTo(PObj->U.Srfs,
			    CAGD_IS_RATIONAL_SRF(PObj->U.Srfs) ? CAGD_PT_P2_TYPE
			    : CAGD_PT_E2_TYPE,
			    FALSE);

		    IritCagdSrfFree(PObj->U.Srfs);
		    PObj->U.Srfs = Srf;
		}

		IritGrapDrawSurface(PObj);
		break;
	    case IP_OBJ_TRIVAR:
		IritGrapDrawTrivar(PObj);
		break;
	    case IP_OBJ_STRING:
		// IritGrapDrawString(PObj); /* Crashes for some reason? */
		break;
	    case IP_OBJ_MULTIVAR:
		if (PObj->U.MultiVars->Dim < 4) { /* Curve, Surface, Trivar. */
		    if ((PTmp = IritMiscAttrIDGetObjectObjAttrib(PObj,
			IRIT_ATTR_CREATE_ID(_coerced))) == NULL) {
			MvarMVStruct
			    * MV = PObj->U.MultiVars;

			switch (MV->Dim) {
			case 1:
			    PTmp = IritPrsrGenCRVObject(
				IritMvarCnvrtMVToCrv(MV));

			    if (CAGD_IS_POWER_CRV(PTmp->U.Crvs)) {
				CagdCrvStruct
				    * Crv = IritCagdCnvrtPwr2BzrCrv(
					PTmp->U.Crvs);

				IritCagdCrvFree(PTmp->U.Crvs);
				PTmp->U.Crvs = Crv;
			    }
			    break;
			case 2:
			    PTmp = IritPrsrGenSRFObject(
				IritMvarCnvrtMVToSrf(MV));

			    if (CAGD_IS_POWER_SRF(PTmp->U.Srfs)) {
				CagdSrfStruct
				    * Srf = IritCagdCnvrtPwr2BzrSrf(
					PTmp->U.Srfs);

				IritCagdSrfFree(PTmp->U.Srfs);
				PTmp->U.Srfs = Srf;
			    }
			    break;
			case 3:
			    PTmp = IritPrsrGenTRIVARObject(
				IritMvarCnvrtMVToTV(MV));
			    break;
			}
			PTmp->Attr = IP_ATTR_COPY_ATTRS(PObj->Attr);
			IP_SET_OBJ_NAME2(PTmp, IP_GET_OBJ_NAME(PObj));
			IritGrapUpdateObjectBBox(PTmp);

			IritMiscAttrIDSetObjectObjAttrib(PObj,
			    IRIT_ATTR_CREATE_ID(_coerced), PTmp, FALSE);
		    }

		    if (IRIT_GRAP_IS_HIGHLIGHT1_OBJ(PObj))
			IRIT_GRAP_SET_HIGHLIGHT1_OBJ(PTmp);
		    else
			IRIT_GRAP_RST_HIGHLIGHT1_OBJ(PTmp);
		    if (IRIT_GRAP_IS_HIGHLIGHT2_OBJ(PObj))
			IRIT_GRAP_SET_HIGHLIGHT2_OBJ(PTmp);
		    else
			IRIT_GRAP_RST_HIGHLIGHT2_OBJ(PTmp);

		    ProcessObjectList(PTmp);
		}
		break;
	    case IP_OBJ_MODEL:
		IritGrapDrawModel(PObj, ProcessObjectList);
		break;
	    case IP_OBJ_VMODEL:
		IritGrapDrawVModel(PObj, ProcessObjectList);
		break;
	    case IP_OBJ_TRISRF:
		if (TRNG_IS_GREGORY_TRISRF(PObj->U.TriSrfs)) {
		    TrngTriangSrfStruct
			* TriSrf = IritTrngCnvrtGregory2BzrTriSrf(
			    PObj->U.TriSrfs);

		    IritTrngTriSrfFree(PObj->U.TriSrfs);
		    PObj->U.TriSrfs = TriSrf;
		}
		IritGrapDrawTriangSrf(PObj);
		break;
	    case IP_OBJ_LIST_OBJ:
		for (i = 0; i < IritPrsrListObjectLength(PObj); i++) {
		    Tmp = IritPrsrListObjectGet(PObj, i);
		    ProcessObjectList(Tmp);
		}
		break;
	    default:
		break;
	}

    }
}

void ProcessPolygon(IPObjectStruct *PObj)
{
    IPObjectStruct *Processed;
    IPObjectStruct *Cpy;

    if (IP_IS_POLYSTRIP_OBJ(PObj))
	Cpy = PolyStripToPolyObj(PObj);
    else {
	Cpy = IritPrsrCopyObject2(NULL, PObj, FALSE, TRUE);
	Cpy -> Tags = PObj -> Tags;
    }

    Processed = (IPObjectStruct*)TlsGetValue(TLSIndex);

    IRIT_LIST_PUSH(Cpy, Processed);
    TlsSetValue(TLSIndex, Processed);
}

static void ProcessMatrix(IPObjectStruct *PObj)
{
    IPObjectStruct *Processed;
    IPObjectStruct *Cpy;

    Processed = (IPObjectStruct*)TlsGetValue(TLSIndex);
    Cpy = IritPrsrCopyObject2(NULL, PObj, FALSE, TRUE);
    IRIT_LIST_PUSH(Cpy, Processed);
    TlsSetValue(TLSIndex, Processed);

}

static void ProcessPoint(IPObjectStruct *PObj)
{
    IPObjectStruct *Processed;
    IPObjectStruct *Cpy;
    IPPolygonStruct *Poly;
    IPVertexStruct *Vert;


    Vert = IritPrsrAllocVertex2(NULL);
    Vert -> Coord[0] = PObj -> U.Pt[0];
    Vert -> Coord[1] = PObj -> U.Pt[1];
    Vert -> Coord[2] = PObj -> U.Pt[2];

    IP_ATTR_COPY_ATTRS(Vert -> Attr, PObj -> Attr);

    Poly = IritPrsrAllocPolygon(0, Vert, NULL);
    IP_ATTR_COPY_ATTRS(Poly -> Attr, PObj -> Attr);

    Cpy = IritPrsrGenPOINTLISTObject(Poly);
    IritPrsrCopyObjectAuxInfo(Cpy, PObj);
    IP_SET_POINTLIST_OBJ(Cpy);

    Processed = (IPObjectStruct*)TlsGetValue(TLSIndex);
    IRIT_LIST_PUSH(Cpy, Processed);
    TlsSetValue(TLSIndex, Processed);
}

static IPPolygonStruct *PolyStripToTriangles(IPPolygonStruct *Strip)
{
    IPPolygonStruct  *Tmp,
	*Ret = NULL;
    IPVertexStruct *Prev, *PPrev, *Iter, *Tmp0, *Tmp1, *Tmp2;
    int 
	Parity = 0;
    if (!Strip)
	return NULL;

    Prev = PPrev = NULL;
    Iter = Strip -> PVertex;
    if (!Iter)
	return NULL;
    do {
	if (Iter && PPrev && Prev) {
	    Tmp0 = IritPrsrCopyVertex(Parity ? Prev : PPrev);
	    Tmp1 = IritPrsrCopyVertex(Parity ? PPrev : Prev);

	    Tmp2 = IritPrsrCopyVertex(Iter);
	    
	    Tmp0 -> Pnext = Tmp1;
	    Tmp1 -> Pnext = Tmp2;
	    Tmp2 -> Pnext = Tmp0;
	    
	    Tmp = IritPrsrAllocPolygon(0, Tmp0, NULL);
	    Tmp -> Attr = IP_ATTR_COPY_ATTRS(Strip -> Attr);
	    Parity = 1 - Parity;

	    IRIT_LIST_PUSH(Tmp, Ret);
	}

	PPrev = Prev;
	Prev = Iter;
	Iter = Iter -> Pnext;
    } while (Iter && Iter != Strip -> PVertex);

    return Ret;
}

static IPObjectStruct *PolyStripToPolyObj(IPObjectStruct *PolyStripObj)
{
    IPObjectStruct *Ret;
    IPPolygonStruct  *Iter, *Tmp0, *Tmp1,
	*Polys = NULL;

    if (!PolyStripObj)
	return NULL;
    if (IP_IS_POLYGON_OBJ(PolyStripObj))
	return PolyStripObj;
    
    Iter = PolyStripObj -> U.Pl;
    Tmp0 = Tmp1 = NULL;

    for (;Iter; Iter = Iter -> Pnext) {
	Tmp1 = Tmp0 = PolyStripToTriangles(Iter);
	if (!Tmp0)
	    continue;
	for(;Tmp0 && Tmp0 -> Pnext; Tmp0 = Tmp0 -> Pnext);
	Tmp0 -> Pnext = Polys;
	Polys = Tmp1;
    }

    if (!Polys)
	return NULL;

    Ret = IritPrsrGenPOLYObject(Polys);
    IritPrsrCopyObjectAuxInfo(Ret, PolyStripObj);
    IP_SET_POLYGON_OBJ(Ret);
    return Ret;
}

static void ProcessLightSource(IritGrapLightType Pos, IrtVecType Colour)
{

}
