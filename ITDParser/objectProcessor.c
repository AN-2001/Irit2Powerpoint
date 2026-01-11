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

typedef struct ProcessingStruct {
    char *Name;
    IPObjectStruct *PObj;
    LightStruct Lights[I2P_NUM_LIGHTS];
    /* Used for computing the light colours. */
    double Ambient, Diffuse, Specular; /* Since we can't pass the parser directly. */
    int CurrentLight;
} ProcessingStruct;

static void ProcessObjectList(IPObjectStruct *PObj);
static void ApplyGrapSettings(ParserSettingsStruct Settings);
static IPObjectStruct *TriangulateObject(IPObjectStruct *PObj);
static void ProcessMatrix(IPObjectStruct *PObj);
static void ProcessPoint(IPObjectStruct *PObj, int IsVector);
static void ProcessLightSource(IritGrapLightType Pos, IrtVecType Colour);
static IPPolygonStruct *PolyStripToTriangles(IPPolygonStruct *Strip);
static IPObjectStruct *PolyStripToPolyObj(IPObjectStruct *PolyStripObj);

void LoadFromFile(ParserStruct *Parser, const char *FileName)
{
    IPObjectStruct *Loaded;
    ProcessingStruct *Processed;
    

    Processed = malloc(sizeof(*Processed));
    memset( Processed, 0, sizeof(*Processed) );

    WaitForSingleObject(gMutex, INFINITE); 

    Loaded = IritPrsrGetObjects2(FileName);
    if (!Loaded) {
	I2P_LOG_ERROR( "%s: Could not load file at Path = %s", Parser -> SettingsString, FileName);    
	return;
    }
    if (!Loaded) {
	I2P_LOG_ERROR( "%s: Could not resolve instances for file at Path = %s", Parser -> SettingsString, FileName);    
	return;
    }

    ApplyGrapSettings(Parser -> Settings);

    I2P_LOG_TRACE( "%s: Processing object list", Parser -> SettingsString );      

    Processed -> Name = Parser -> SettingsString;
    Processed -> PObj = NULL;
    Processed -> Ambient = Parser -> Settings.Ambient;
    Processed -> Diffuse = Parser -> Settings.Diffuse;
    Processed -> Specular = Parser -> Settings.Specular;

    Processed -> CurrentLight = 0;
    memset(Processed -> Lights, 0, sizeof(Processed -> Lights[0]) * 32);

    TlsSetValue(TLSIndex, Processed);
    ProcessObjectList(Loaded);
    ReleaseMutex(gMutex);

    I2P_LOG_TRACE( "%s: Triangulating object", Parser -> SettingsString);

    Parser -> PObj = TriangulateObject(Processed -> PObj);
    memcpy(Parser -> Lights, Processed -> Lights, sizeof(Processed -> Lights));

    IritPrsrFreeObjectList(Loaded);
    IritPrsrFreeObjectList(Processed -> PObj);
    free( Processed );

    I2P_LOG_TRACE( "%s: Loaded and processed file", Parser -> SettingsString);
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
    IritGrapGraphicStateStruct state;

    IritGrapGraphicsStateResetDefault();
    IritGrapGraphicsStateGet( &state );

    state.DrawVNormal = Settings.DrawVNormal;
    state.DrawPNormal = Settings.DrawPNormal;
    state.NumOfIsolines[0] = Settings.Isolines[0];
    state.NumOfIsolines[1] = Settings.Isolines[1];
    state.NumOfIsolines[2] = Settings.Isolines[2];
    state.PolygonOptiApprox = Settings.PolygonOptiApprox;
    state.PolylineOptiApprox = Settings.PolylineOptiApprox;
    state.DrawSurfacePoly = Settings.DrawSurfacePoly;
    state.DrawSurfaceMesh = Settings.DrawSurfaceMesh;
    state.DrawModelsMonolithic = Settings.DrawModelsMonolithic;
    state.DrawSurfaceOrient = Settings.DrawSurfaceOrient;
    state.FlipNormalOrient = Settings.FlipNormalOrient;
    state.NormalSize = Settings.NormalSize;
    state.PlgnFineness = Settings.PlgnFineness;
    state.PllnFineness = Settings.PllnFineness;
    state.PointSize = Settings.PointSize;

    state.DrawSurfaceWire = Settings.SurfaceWireSetup & 0x01;
    state.DrawSurfaceBndry = Settings.SurfaceWireSetup & 0x02;
    state.DrawSurfaceSilh = Settings.SurfaceWireSetup & 0x04;
    state.DrawSurfaceSketch = Settings.SurfaceWireSetup & 0x08;
    state.DrawSurfaceRflctLns = Settings.SurfaceWireSetup & 0x10;
    state.DrawSurfaceKnotLns = Settings.SurfaceWireSetup & 0x20;
    IritGrapGraphicsStateSet( &state );

}

static void ProcessObjectList(IPObjectStruct *PObj)
{
    IPObjectStruct *Tmp;
    const CagdRType *R;
    IrtPtType Pt;
    IPObjectStruct *PTmp;
    ProcessingStruct *Processed;
    int i;

    Processed = (ProcessingStruct*)TlsGetValue(TLSIndex);
    for (; PObj; PObj = PObj -> Pnext) {
	switch (PObj -> ObjType) {
	    case IP_OBJ_MATRIX:
		I2P_LOG_TRACE( "%s: Detected MATRIX", Processed -> Name );
		ProcessMatrix(PObj);
		break;
	    case IP_OBJ_POLY:
		I2P_LOG_TRACE( "%s: Detected POLY", Processed -> Name );
		ProcessPolygon(PObj);
		break;
	    case IP_OBJ_CTLPT:
		I2P_LOG_TRACE( "%s: Detected CTLPT", Processed -> Name );
		/* Coerce, in place, a control points to a regular point. */
		R = PObj->U.CtlPt.Coords;
		IritCagdCoercePointTo(Pt, CAGD_PT_E3_TYPE,
		    (CagdRType* const*)&R,
		    -1, PObj->U.CtlPt.PtType);
		IRIT_PT_COPY(PObj->U.Pt, Pt);
		PObj->ObjType = IP_OBJ_POINT;
	    case IP_OBJ_POINT:
		I2P_LOG_TRACE( "%s: Detected POINT", Processed -> Name );
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
		    ProcessPoint(PObj, FALSE);
		break;
	    case IP_OBJ_VECTOR:
		I2P_LOG_TRACE( "%s: Detected VECTOR", Processed -> Name );
		ProcessPoint(PObj, TRUE);
		break;
	    case IP_OBJ_CURVE:
		I2P_LOG_TRACE( "%s: Detected CURVE", Processed -> Name );
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
		I2P_LOG_TRACE( "%s: Detected TRIMSRF", Processed -> Name );
		if (CAGD_IS_POWER_SRF(PObj->U.TrimSrfs->Srf)) {
		    CagdSrfStruct
			* Srf = IritCagdCnvrtPwr2BzrSrf(PObj->U.TrimSrfs->Srf);

		    IritCagdSrfFree(PObj->U.TrimSrfs->Srf);
		    PObj->U.TrimSrfs->Srf = Srf;
		}

		if (PObj->U.TrimSrfs->TrimCrvList == NULL) {
		    CagdSrfStruct
			* Srf = PObj->U.TrimSrfs->Srf;

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
		I2P_LOG_TRACE( "%s: Detected SURFACE", Processed -> Name );
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
		I2P_LOG_TRACE( "%s: Detected TRIVAR", Processed -> Name );
		IritGrapDrawTrivar(PObj);
		break;
	    case IP_OBJ_STRING:
		I2P_LOG_TRACE( "%s: Detected STRING", Processed -> Name );
		IritGrapDrawString(PObj); 
		break;
	    case IP_OBJ_MULTIVAR:
		I2P_LOG_TRACE( "%s: Detected MULTIVAR", Processed -> Name );
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
		I2P_LOG_TRACE( "%s: Detected MODEL", Processed -> Name );
		IritGrapDrawModel(PObj, ProcessObjectList);
		break;
	    case IP_OBJ_VMODEL:
		I2P_LOG_TRACE( "%s: Detected VMODEL", Processed -> Name );
		IritGrapDrawVModel(PObj, ProcessObjectList);
		break;
	    case IP_OBJ_TRISRF:
		I2P_LOG_TRACE( "%s: Detected TRISRF", Processed -> Name );
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
		I2P_LOG_TRACE( "%s: Detected LIST", Processed -> Name );
		for (i = 0; i < IritPrsrListObjectLength(PObj); i++) {
		    Tmp = IritPrsrListObjectGet(PObj, i);
		    ProcessObjectList(Tmp);
		}

		break;
	    default:
		I2P_LOG_TRACE( "%s: Detected UNSUPPORTED = %d", Processed -> Name,  PObj -> ObjType );
		break;
	}

    }
}

void ProcessPolygon(IPObjectStruct *PObj)
{
    ProcessingStruct *Processed;
    IPObjectStruct *Cpy;

    if (IP_IS_POLYSTRIP_OBJ(PObj))
	Cpy = PolyStripToPolyObj(PObj);
    else {
	Cpy = IritPrsrCopyObject2(NULL, PObj, FALSE, TRUE);
	Cpy -> Tags = PObj -> Tags;
    }

    Processed = (ProcessingStruct*)TlsGetValue(TLSIndex);
    IRIT_LIST_PUSH(Cpy, Processed -> PObj );

    I2P_LOG_TRACE( "%s: Processed POLYGON", Processed -> Name );
}

static void ProcessMatrix(IPObjectStruct *PObj)
{
    ProcessingStruct *Processed;
    IPObjectStruct *Cpy;

    Cpy = IritPrsrCopyObject2(NULL, PObj, FALSE, TRUE);

    Processed = (ProcessingStruct*)TlsGetValue(TLSIndex);
    IRIT_LIST_PUSH(Cpy, Processed -> PObj );

    I2P_LOG_TRACE( "%s: Processed MATRIX", Processed -> Name );
}

static void ProcessPoint(IPObjectStruct *PObj, int IsVector)
{
    ProcessingStruct *Processed;
    IPObjectStruct *Cpy;
    IPPolygonStruct *Poly;
    IPVertexStruct *Vert;

    Vert = IritPrsrAllocVertex2(NULL);
    Vert -> Coord[0] = PObj -> U.Pt[0];
    Vert -> Coord[1] = PObj -> U.Pt[1];
    Vert -> Coord[2] = PObj -> U.Pt[2];

    Vert -> Attr = IP_ATTR_COPY_ATTRS(PObj -> Attr);

    Poly = IritPrsrAllocPolygon(0, Vert, NULL);
    Poly -> Attr = IP_ATTR_COPY_ATTRS(PObj -> Attr);

    Cpy = IritPrsrGenPOINTLISTObject(Poly);
    IritPrsrCopyObjectAuxInfo(Cpy, PObj);
    IP_SET_POINTLIST_OBJ(Cpy);
    if (IsVector) {
	IP_SET_OBJ_NAME( Cpy, "%s", "VECTOR" );
    } else {
	IP_SET_OBJ_NAME( Cpy, "%s", "POINT" );
    }

    Processed = (ProcessingStruct*)TlsGetValue(TLSIndex);

    IRIT_LIST_PUSH(Cpy, Processed -> PObj );

    I2P_LOG_TRACE( "%s: Processed POINT", Processed -> Name );
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
    ProcessingStruct *Processed;

    Processed = (ProcessingStruct*)TlsGetValue(TLSIndex);

    
    Processed -> Lights[ Processed -> CurrentLight ].IsEnabled = TRUE;

    Processed -> Lights[ Processed -> CurrentLight ].x = Pos[0];
    Processed -> Lights[ Processed -> CurrentLight ].y = Pos[1];
    Processed -> Lights[ Processed -> CurrentLight ].z = Pos[2];

    Processed -> Lights[ Processed -> CurrentLight ].ar = Colour[0] * Processed -> Ambient;
    Processed -> Lights[ Processed -> CurrentLight ].ag = Colour[1] * Processed -> Ambient;
    Processed -> Lights[ Processed -> CurrentLight ].ab = Colour[2] * Processed -> Ambient;

    Processed -> Lights[ Processed -> CurrentLight ].dr = Colour[0] * Processed -> Diffuse;
    Processed -> Lights[ Processed -> CurrentLight ].dg = Colour[1] * Processed -> Diffuse;
    Processed -> Lights[ Processed -> CurrentLight ].db = Colour[2] * Processed -> Diffuse;

    Processed -> Lights[ Processed -> CurrentLight ].sr = Colour[0] * Processed -> Specular;
    Processed -> Lights[ Processed -> CurrentLight ].sg = Colour[1] * Processed -> Specular;
    Processed -> Lights[ Processed -> CurrentLight ].sb = Colour[2] * Processed -> Specular;

    Processed -> CurrentLight++;
    I2P_LOG_TRACE( "%s: Processed LIGHT", Processed -> Name );
}
