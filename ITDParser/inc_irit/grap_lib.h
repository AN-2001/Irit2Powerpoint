/*****************************************************************************
* Grap_lib.h - local header file for the GRAP library.			     *
* This header is also the interface header to the world.		     *
*									     *
* Written by:  Gershon Elber				Ver 1.0, Jan. 1992   *
******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute	of Technology		     *
******************************************************************************
* Global definitions of	Graphics interface.				     *
*****************************************************************************/

#ifndef	IRIT_GRAP_LIB_H	/* Define only once */
#define	IRIT_GRAP_LIB_H

#include "inc_irit/irit_sm.h"
#include "inc_irit/misc_lib.h"
#include "inc_irit/miscattr.h"
#include "inc_irit/iritprsr.h"
#include "inc_irit/geom_lib.h"

#define IRIT_GRAP_IRIT_BLACK		0
#define IRIT_GRAP_IRIT_BLUE		1
#define IRIT_GRAP_IRIT_GREEN		2
#define IRIT_GRAP_IRIT_CYAN		3
#define IRIT_GRAP_IRIT_RED		4
#define IRIT_GRAP_IRIT_MAGENTA		5
#define IRIT_GRAP_IRIT_BROWN		6
#define IRIT_GRAP_IRIT_LIGHTGREY	7
#define IRIT_GRAP_IRIT_DARKGRAY	8
#define IRIT_GRAP_IRIT_LIGHTBLUE	9
#define IRIT_GRAP_IRIT_LIGHTGREEN	10
#define IRIT_GRAP_IRIT_LIGHTCYAN	11
#define IRIT_GRAP_IRIT_LIGHTRED	12
#define IRIT_GRAP_IRIT_LIGHTMAGENTA	13
#define IRIT_GRAP_IRIT_YELLOW		14
#define IRIT_GRAP_IRIT_WHITE		15

#define IRIT_GRAP_MAX_COLOR		IP_ATTR_IRIT_COLOR_TABLE_SIZE
#define IRIT_GRAP_DEFAULT_COLOR	1      /* For objects with no color defined. */

#define IRIT_GRAP_POLYGON_Z_TRANS 0.005   /* Z trans of polygons over pllns. */

/* The current point cross length is divided by scalar to form real length:  */
#define IRIT_GRAP_POINT_DEFAULT_LENGTH	20
#define IRIT_GRAP_POINT_SCALER_LENGTH	1000

#define IRIT_GRAP_SKETCHING_NONE		0
#define IRIT_GRAP_SKETCHING_ISO_PARAM		1
#define IRIT_GRAP_SKETCHING_CURVATURE		2
#define IRIT_GRAP_SKETCHING_ISOCLINES		3
#define IRIT_GRAP_SKETCHING_ORTHOCLINES		4

#define IRIT_GRAP_SKETCH_TYPE_NONE		0
#define IRIT_GRAP_SKETCH_TYPE_SILHOUETTE	1
#define IRIT_GRAP_SKETCH_TYPE_SHADING		2
#define IRIT_GRAP_SKETCH_TYPE_IMPORTANCE	3

#define IRIT_GRAP_DEFAULT_NUM_OF_ISOLINES	8
#define IRIT_GRAP_DEFAULT_SAMPLES_PER_CURVE	64
#define IRIT_GRAP_DEFAULT_PLLN_OPTI_FINENESS	0.005
#define IRIT_GRAP_DEFAULT_POLYGON_FINENESS	20
#define IRIT_GRAP_DEFAULT_IRIT_MAT		"irit.imd"

#define IRIT_GRAP_VIEW_PERSPECTIVE		1	      /* View modes. */
#define IRIT_GRAP_VIEW_ORTHOGRAPHIC		2
#define IRIT_GRAP_DEFAULT_PERSPECTIVE_Z	-5.0	   /* Default Z focal point. */

#define IRIT_GRAP_FREEFORM_ORIENT_BNDRY_EPS  0.004321 /* Very close to start.*/

#define IRIT_GRAP_HIGHLIGHT1_OBJ_TAG	0x20
#define IRIT_GRAP_HIGHLIGHT2_OBJ_TAG	0x40
#define IRIT_GRAP_IS_HIGHLIGHT1_OBJ(Obj)   ((Obj) -> Tags & IRIT_GRAP_HIGHLIGHT1_OBJ_TAG)
#define IRIT_GRAP_SET_HIGHLIGHT1_OBJ(Obj)  ((Obj) -> Tags |= IRIT_GRAP_HIGHLIGHT1_OBJ_TAG)
#define IRIT_GRAP_RST_HIGHLIGHT1_OBJ(Obj)  ((Obj) -> Tags &= ~IRIT_GRAP_HIGHLIGHT1_OBJ_TAG)
#define IRIT_GRAP_IS_HIGHLIGHT2_OBJ(Obj)   ((Obj) -> Tags & IRIT_GRAP_HIGHLIGHT2_OBJ_TAG)
#define IRIT_GRAP_SET_HIGHLIGHT2_OBJ(Obj)  ((Obj) -> Tags |= IRIT_GRAP_HIGHLIGHT2_OBJ_TAG)
#define IRIT_GRAP_RST_HIGHLIGHT2_OBJ(Obj)  ((Obj) -> Tags &= ~IRIT_GRAP_HIGHLIGHT2_OBJ_TAG)

#define IRIT_GRAP_ADD_ORIENT_NRML(P, N) \
				if (IritGrapGlblState.FlipNormalOrient) { \
				    P[0] = P[0] - N[0] * IritGrapGlblState.NormalSize; \
				    P[1] = P[1] - N[1] * IritGrapGlblState.NormalSize; \
				    P[2] = P[2] - N[2] * IritGrapGlblState.NormalSize; \
				} \
				else { \
				    P[0] = P[0] + N[0] * IritGrapGlblState.NormalSize; \
				    P[1] = P[1] + N[1] * IritGrapGlblState.NormalSize; \
				    P[2] = P[2] + N[2] * IritGrapGlblState.NormalSize; \
				}

#define IRIT_GRAP_MAX_LIGHT_SOURCES 10  /* Maximal number of light sources. */

typedef int (*IritGrapDrawUpdateFuncType)(void);
typedef void (*IritGrapDrawObjectFuncType)(IPObjectStruct *PObj);

typedef enum {   /* Note that some device drivers depends on this order. */
    IRIT_GRAP_STATE_NONE,
    IRIT_GRAP_STATE_OOPS,
    IRIT_GRAP_STATE_MOUSE_SENSITIVE,
    IRIT_GRAP_STATE_SCR_OBJ_TGL,
    IRIT_GRAP_STATE_CONT_MOTION,
    IRIT_GRAP_STATE_PERS_ORTHO_TGL,
    IRIT_GRAP_STATE_DEPTH_CUE,
    IRIT_GRAP_STATE_3D_GLASSES,
    IRIT_GRAP_STATE_CACHE_GEOM,
    IRIT_GRAP_STATE_DRAW_STYLE,
    IRIT_GRAP_STATE_SHADING_MODEL,
    IRIT_GRAP_STATE_BACK_FACE_CULL,
    IRIT_GRAP_STATE_DOUBLE_BUFFER,
    IRIT_GRAP_STATE_ANTI_ALIASING,
    IRIT_GRAP_STATE_DRAW_INTERNAL,
    IRIT_GRAP_STATE_DRAW_VNORMAL,
    IRIT_GRAP_STATE_DRAW_PNORMAL,
    IRIT_GRAP_STATE_DRAW_POLYGONS,
    IRIT_GRAP_STATE_DRAW_SRF_MESH,
    IRIT_GRAP_STATE_DRAW_SRF_WIRE,
    IRIT_GRAP_STATE_DRAW_SRF_KNOT_LNS,
    IRIT_GRAP_STATE_DRAW_SRF_BNDRY,
    IRIT_GRAP_STATE_DRAW_SRF_SILH,
    IRIT_GRAP_STATE_DRAW_SRF_POLY,
    IRIT_GRAP_STATE_DRAW_SRF_SKTCH,
    IRIT_GRAP_STATE_DRAW_SRF_RFLCT_LNS,
    IRIT_GRAP_STATE_FOUR_PER_FLAT,
    IRIT_GRAP_STATE_NUM_ISOLINES,
    IRIT_GRAP_STATE_POLYGON_APPROX,
    IRIT_GRAP_STATE_SAMP_PER_CRV_APPROX,
    IRIT_GRAP_STATE_LENGTH_VECTORS,
    IRIT_GRAP_STATE_WIDTH_LINES,
    IRIT_GRAP_STATE_WIDTH_POINTS,
    IRIT_GRAP_STATE_POLYGON_OPTI,
    IRIT_GRAP_STATE_POLYLINE_OPTI,
    IRIT_GRAP_STATE_VIEW_FRONT,
    IRIT_GRAP_STATE_VIEW_SIDE,
    IRIT_GRAP_STATE_VIEW_TOP,
    IRIT_GRAP_STATE_VIEW_ISOMETRY,
    IRIT_GRAP_STATE_VIEW_4,
    IRIT_GRAP_STATE_CLEAR_VIEW,
    IRIT_GRAP_STATE_ANIMATION,

    IRIT_GRAP_STATE_NUM_POLY_COUNT,
    IRIT_GRAP_STATE_NRML_ORIENT,
    IRIT_GRAP_STATE_LIGHT_ONE_SIDE,
    IRIT_GRAP_STATE_POLY_APPROX,

    IRIT_GRAP_STATE_RES_ADAP_ISO,
    IRIT_GRAP_STATE_RES_RULED_SRF,
    IRIT_GRAP_STATE_RULED_SRF_APPROX,
    IRIT_GRAP_STATE_ADAP_ISO_DIR,

    IRIT_GRAP_STATE_LOWRES_RATIO,

    IRIT_GRAP_STATE_SKETCH_ISO_PARAM,
    IRIT_GRAP_STATE_SKETCH_CURVATURE,
    IRIT_GRAP_STATE_SKETCH_SILHOUETTE,

    IRIT_GRAP_STATE_SKETCH_SIL_POWER,
    IRIT_GRAP_STATE_SKETCH_SHADING_POWER,
    IRIT_GRAP_STATE_SKETCH_IMP_DECAY,
    IRIT_GRAP_STATE_SKETCH_IMP_FRNT_SPRT,
    IRIT_GRAP_STATE_SKETCH_IMPORTANCE,

    IRIT_GRAP_STATE_SHADE_PARAM,
    IRIT_GRAP_STATE_SHADE_AMBIENT,
    IRIT_GRAP_STATE_SHADE_DIFFUSE,
    IRIT_GRAP_STATE_SHADE_SPECULAR,
    IRIT_GRAP_STATE_SHADE_SHININESS,
    IRIT_GRAP_STATE_SHADE_EMISSION,

    IRIT_GRAP_STATE_SHADE_LGT_SRC_IDX,
    IRIT_GRAP_STATE_SHADE_LGT_SRC_X,
    IRIT_GRAP_STATE_SHADE_LGT_SRC_Y,
    IRIT_GRAP_STATE_SHADE_LGT_SRC_Z,
    IRIT_GRAP_STATE_SHADE_LGT_SRC_W,

    IRIT_GRAP_STATE_SHADE_RESET,
    IRIT_GRAP_STATE_SHADE_DISMISS,

    IRIT_GRAP_STATE_DRAW_STYLE_SOLID,
    IRIT_GRAP_STATE_DRAW_STYLE_WIREFRAME,
    IRIT_GRAP_STATE_DRAW_STYLE_POINTS,

    IRIT_GRAP_STATE_ANTI_ALIAS_OFF,
    IRIT_GRAP_STATE_ANTI_ALIAS_ON,
    IRIT_GRAP_STATE_ANTI_ALIAS_BLEND,

    IRIT_GRAP_STATE_CRV_EDIT,

    IRIT_GRAP_STATE_SAVE_IMAGE,

    IRIT_GRAP_STATE_FRAME_PER_SEC
} IritGrapGlblStateType;

typedef enum {
    IRIT_GRAP_EVENT_ZERO = 0,
    IRIT_GRAP_EVENT_RESET,
    IRIT_GRAP_EVENT_NONE = 2000,
    IRIT_GRAP_EVENT_DISCONNECT,
    IRIT_GRAP_EVENT_QUIT,
    IRIT_GRAP_EVENT_SCR_OBJ_TGL,
    IRIT_GRAP_EVENT_CONT_MOTION,
    IRIT_GRAP_EVENT_NRML_ORIENT,
    IRIT_GRAP_EVENT_PERS_ORTHO_TGL,
    IRIT_GRAP_EVENT_PERS_ORTHO_Z,
    IRIT_GRAP_EVENT_ROTATE,
    IRIT_GRAP_EVENT_ROTATE_X,
    IRIT_GRAP_EVENT_ROTATE_Y,
    IRIT_GRAP_EVENT_ROTATE_Z,
    IRIT_GRAP_EVENT_TRANSLATE,
    IRIT_GRAP_EVENT_TRANSLATE_X,
    IRIT_GRAP_EVENT_TRANSLATE_Y,
    IRIT_GRAP_EVENT_TRANSLATE_Z,
    IRIT_GRAP_EVENT_SCALE,
    IRIT_GRAP_EVENT_NEAR_CLIP,
    IRIT_GRAP_EVENT_FAR_CLIP,
    IRIT_GRAP_EVENT_DEPTH_CUE,
    IRIT_GRAP_EVENT_3D_GLASSES,
    IRIT_GRAP_EVENT_DBL_BUFFER,
    IRIT_GRAP_EVENT_ACCUM_MATRIX,
    IRIT_GRAP_EVENT_SAVE_MATRIX,
    IRIT_GRAP_EVENT_SUBMIT_MATRIX,
    IRIT_GRAP_EVENT_PUSH_MATRIX,
    IRIT_GRAP_EVENT_POP_MATRIX,
    IRIT_GRAP_EVENT_ANIMATION,
    IRIT_GRAP_EVENT_SHADE_PARAM,
    IRIT_GRAP_EVENT_CRV_EDIT,
    IRIT_GRAP_EVENT_SRF_EDIT,
    IRIT_GRAP_EVENT_PICK_OBJS,
    IRIT_GRAP_EVENT_OBJ_MANIP,
    IRIT_GRAP_EVENT_STATE
} IritGrapGraphicEventType;

#define IRIT_GRAP_MAX_ROTATE_ANGLE	45.0 /*Max. rates used by inter mode.*/
#define IRIT_GRAP_MAX_TRANSLATE_FACTOR	2.0
#define IRIT_GRAP_MAX_SCALE_FACTOR	2.0
#define IRIT_GRAP_MAX_CLIP_FACTOR	0.5

#define IRIT_GRAP_TRANS_SCREEN	1  /* Scrn, Obj coords. transformation mode. */
#define IRIT_GRAP_TRANS_OBJECT	2

#define IRIT_GRAP_STATE_OFF		-32760
#define IRIT_GRAP_STATE_ON		-32761
#define IRIT_GRAP_STATE_DEC		IRIT_GRAP_STATE_OFF
#define IRIT_GRAP_STATE_INC		IRIT_GRAP_STATE_ON
#define IRIT_GRAP_STATE_TGL		-32762

#define IRIT_GRAP_SHADING_NONE		0
#define IRIT_GRAP_SHADING_BACKGROUND	1
#define IRIT_GRAP_SHADING_FLAT		2
#define IRIT_GRAP_SHADING_GOURAUD	3
#define IRIT_GRAP_SHADING_PHONG	4

#define IRIT_GRAP_HAS_ISOLINES(NumOfIsolines) \
		       (NumOfIsolines[0] > 0 || \
		        NumOfIsolines[1] > 0 || \
			NumOfIsolines[2] > 0)

typedef float IritGrapLightType[4];

typedef struct IritGrapSketchParamStruct {
    IrtRType ShadePower;
    IrtRType SilPower;
    int SketchSilType;
    int SketchShdType;
    int SketchImpType;
    int SketchImp;
    int SketchInvShd;
    IrtRType SketchImpDecay;
    IrtRType SketchImpFrntSprt;
} IritGrapSketchParamStruct;

typedef struct IritGrapShadeParamStruct {
    int NumOfLightSrcs;
    IritGrapLightType LightPos[IRIT_GRAP_MAX_LIGHT_SOURCES];
    IritGrapLightType LightAmbient;
    IritGrapLightType LightDiffuse;
    IritGrapLightType LightSpecular;
    IritGrapLightType LightEmissive;
    float Shininess;
} IritGrapShadeParamStruct;

typedef struct IritGrapGraphicStateStruct {
    int BackGroundColor[3],
	AbortKeyPressed,
	AdapIsoDir,
	BackFaceCull,
	CacheGeom,
	CountNumPolys,
	DepthCue,
	DrawInternal,
	DrawPNormal,
	DrawPolygons,
	DrawSurfaceMesh,
	DrawSurfacePoly,
	DrawSurfaceWire,
	DrawSurfaceKnotLns,
	DrawSurfaceOrient,
	DrawSurfaceBndry,
	DrawSurfaceSilh,
	DrawSurfaceDiscont,
	DrawSurfaceContours,
	DrawSurfaceIsophotes,
	DrawSurfaceSketch,
	DrawSurfaceRflctLns,
	DrawModelsMonolithic,
	DrawStyle,
	DrawVNormal,
	FlipNormalOrient,
	PolygonStrips,
	FourPerFlat,
	Highlight1Color[3],
	Highlight2Color[3],
	IntensityHighState,
	LastLowResDraw,
	LineWidth,
	ManipulationActive,
	MoreInfo,
        NumOfIsolines[3],
	NumPolys,
	PolygonOptiApprox,
	QuickLoad,
	ContinuousMotion,
	ViewMode,
	AntiAliasing,
	ShadingModel,
	DoDoubleBuffer,
	Animation,
	TransformMode,
	CountFramePerSec,
	AlwaysGenUV,
	NumOfIsophotes,
	NumOfContours;

    IrtRType
	MinPickDist,
	NormalSize,
	PointSize,
	PlgnFineness,
	PllnFineness,
	RelLowresFineNess,
	FramePerSec,
	ChangeFactor,
	ZMinClip,
	ZMaxClip,
        EyeDistance;

    IrtVecType
	IsophotesDir, ContoursDir;

    SymbCrvApproxMethodType
	PolylineOptiApprox;

    IrtHmgnMatType CrntViewMat, InvCrntViewMat, LastProcessMat,
	PushViewMat, PushPrspMat, IsometryViewMat;

    IPObjectStruct *DisplayList;

    IritGrapDrawUpdateFuncType
	DrawSrfPolysPreFunc,
	DrawSrfPolysPostFunc,
	DrawSrfWirePreFunc,
	DrawSrfWirePostFunc,
	DrawSketchPreFunc,
	DrawSketchPostFunc,
	DrawCtlMeshPreFunc,
	DrawCtlMeshPostFunc;
} IritGrapGraphicStateStruct;

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

IRIT_GLOBAL_DATA_HEADER GMAnimationStruct IritGrapAnimation;
IRIT_GLOBAL_DATA_HEADER IritGrapShadeParamStruct IritGrapShadeParam;
IRIT_GLOBAL_DATA_HEADER IritGrapSketchParamStruct IritGrapSketchParam;

IRIT_GLOBAL_DATA_HEADER IritGrapDrawObjectFuncType IritGrapDrawPolyFuncPtr;

/* These variables are used by C++ code so placed under 'extern "C"'. */
IRIT_GLOBAL_DATA_HEADER IritGrapGraphicStateStruct IritGrapGlblState;

/* Functions that should be supplied by the user of this lib. */
void IritGrapRedrawViewWindow(void);

/*** Function from this point in are fully implemented in grap_lib: ***/
IritGrapDrawObjectFuncType IritGrapDSetDrawPolyFunc(
				     IritGrapDrawObjectFuncType DrawPolyFunc);

/* Grap_lib general functions. */
void IritGrapConfirmConvexPolys(IPObjectStruct *PObj, int Depth);
IPObjectStruct *IritGrapLoadGeometry(const char **FileNames, int NumFiles);
void IritGrapSaveCurrentMat(int ViewMode, char *Name);
void IritGrapActiveListFreePolyIsoAttribute(IPObjectStruct *PObjs,
					    int	FreePolygons,
					    int	FreeIsolines,
					    int	FreeSketches,
					    int	FreeCtlMesh);
void IritGrapActiveListFreeAttrByID(IPObjectStruct *PObjs,
				    IPAttrIDType AttrID);
void IritGrapActiveFreePolyIsoAttribute(IPObjectStruct *PObj,
					int FreePolygons,
					int FreeIsolines,
					int FreeSketches,
					int FreeCtlMesh);
void IritGrapActiveFreeAttrByID(IPObjectStruct *PObj, IPAttrIDType AttrID);
void IritGrapUpdateObjectBBox(IPObjectStruct *PObj);
void IritGrapUpdateViewConsideringScale(IrtHmgnMatType Mat);
IrtRType IritGrapFindMinimalDist(IPObjectStruct *PObj,
				 IPPolygonStruct **MinPl,
				 IrtPtType MinPt,
				 int *MinPlIsPolyline,
				 const IrtPtType LinePos,
				 const IrtVecType LineDir,
				 IrtRType *HitDepth);
void IritGrapDrawPolygonSketches(IPObjectStruct *PObj);
IPObjectStruct *IritGrapGenPolygonSketches(IPObjectStruct *PObj,
					   IrtRType FineNess);
void IritGrapDrawPolySilhBndry(IPObjectStruct *PObj);
void IritGrapDrawPolySilhouette(IPObjectStruct *PObj);
void IritGrapDrawPolyBoundary(IPObjectStruct *PObj);
void IritGrapDrawPolyDiscontinuities(IPObjectStruct *PObj);
void IritGrapDrawPolyKnotLines(IPObjectStruct *PObj);
int IritGrapDrawPolyContoursSetup(IrtRType x, IrtRType y, IrtRType z, int n);
void IritGrapDrawPolyContours(IPObjectStruct *PObj);
int IritGrapDrawPolyIsophotesSetup(IrtRType x, IrtRType y, IrtRType z, int n);
void IritGrapDrawPolyIsophotes(IPObjectStruct *PObj);
IPObjectStruct *IritGrapGetObjIsoLines(const IPObjectStruct *PObj);
IPObjectStruct *IritGrapGetObjPolygons(const IPObjectStruct *PObj);
int IritGrapInitSrfTexture(const IPObjectStruct *PObj);
int IritGrapInitSrfTextureFromImage(const IPObjectStruct *PObj,
				    int	SrfIdx,
				    void *Image,
				    int	ImageWidth,
				    int	ImageHeight,
				    int	ImageAlpha);
int IritGrapDeleteTexture(const IPObjectStruct *PObj);
int IritGrapInitSrfMovie(const IPObjectStruct *PObj);
void IritGrapClearObjTextureMovieAttr(const IPObjectStruct *PObj);
int IritGrapDefaultProcessEvent(IritGrapGraphicEventType Event,
				IrtRType *ChangeFactor);
int IritGrapDefaultStateHandler(int State, int StateStatus, int Refresh);
void IritGrapUpdateFPS(int Start);

/* Generic drawing routines - essentially convert the geometry into a      */
/* displayable form and invoke the drawing routines recursively.	   */
void IritGrapDrawCurve(IPObjectStruct *PObj);
void IritGrapDrawCurveGenPolylines(const IPObjectStruct *PObj);
void IritGrapDrawModel(IPObjectStruct *PObj,
		       IritGrapDrawObjectFuncType DrawTSrfsFuncPtr);
void IritGrapDrawModelGenPolygons(const IPObjectStruct *PObj);
IPObjectStruct *IritGrapDecomposeMdlDue2Graphics(const IPObjectStruct *PMdls);
void IritGrapDrawVModel(IPObjectStruct *PObj,
			IritGrapDrawObjectFuncType DrawTSrfsFuncPtr);
void IritGrapDrawVModelGenPolygons(const IPObjectStruct *PObj);
IPObjectStruct *IritGrapDecomposeVMdlDue2Graphics(const IPObjectStruct *PVMdls);
void IritGrapDrawString(IPObjectStruct *PObj);
unsigned char *IritGrapCnvrtChar2Raster(const char c);
void IritGrapDrawSurface(IPObjectStruct *PObj);
void IritGrapDrawSurfaceGenPolygons(const IPObjectStruct *PObj);
void IritGrapDrawSurfaceAIso(IPObjectStruct *PObj);
void IritGrapDrawTriangSrf(IPObjectStruct *PObj);
void IritGrapDrawTriangGenSrfPolygons(const IPObjectStruct *PObj);
void IritGrapDrawTrimSrf(IPObjectStruct *PObj);
void IritGrapDrawTrimSrfGenPolygons(const IPObjectStruct *PObj);
void IritGrapDrawTrivar(IPObjectStruct *PObj);
void IritGrapDrawTrivarGenSrfPolygons(const IPObjectStruct *PObj);

/* Sketching related routines for freeforms surfaces and polygonal meshes. */
void IritGrapSketchDrawSurface(const IPObjectStruct *PObjSketches);
IPObjectStruct *IritGrapSketchGenSrfSketches(const CagdSrfStruct *InSrf,
					     IrtRType FineNess,
					     const IPObjectStruct *PObj,
					     int Importance);
void IritGrapSketchDrawPolygons(IPObjectStruct *PObjSketches);
IPObjectStruct *IritGrapSketchGenPolySketches(IPObjectStruct *PlObj,
					      IrtRType FineNess,
					      int Importance);
IPObjectStruct *IritGrapSketchGenPolyImportanceSketches(
				       IPObjectStruct *PObj,
				       IritGrapSketchParamStruct *SketchParams,
				       IrtRType FineNess);

/* Allow control over possible pre/post processing of different style draw. */
IritGrapDrawUpdateFuncType IritGrapSetSrfPolysPreFunc(
					      IritGrapDrawUpdateFuncType Func);
IritGrapDrawUpdateFuncType IritGrapSetSrfPolysPostFunc(
					      IritGrapDrawUpdateFuncType Func);
IritGrapDrawUpdateFuncType IritGrapSetSrfWirePreFunc(
					      IritGrapDrawUpdateFuncType Func);
IritGrapDrawUpdateFuncType IritGrapSetSrfWirePostFunc(
					      IritGrapDrawUpdateFuncType Func);
IritGrapDrawUpdateFuncType IritGrapSetSketchPreFunc(
					      IritGrapDrawUpdateFuncType Func);
IritGrapDrawUpdateFuncType IritGrapSetSketchPostFunc(
					      IritGrapDrawUpdateFuncType Func);
IritGrapDrawUpdateFuncType IritGrapSetCtlMeshPreFunc(
					      IritGrapDrawUpdateFuncType Func);
IritGrapDrawUpdateFuncType IritGrapSetCtlMeshPostFunc(
					      IritGrapDrawUpdateFuncType Func);

void IritGrapGraphicsStateSet(const IritGrapGraphicStateStruct *IGS);
void IritGrapGraphicsStateGet(IritGrapGraphicStateStruct *IGS);
void IritGrapGraphicsStateResetDefault(void);
void IritGrapGraphicsStateReset(IritGrapGraphicStateStruct *IGS);
void IritGrapAttrAddOrigGeomRef(IPPolygonStruct *Pls, const void *ProgGeom);

/* Open GL CG hardware GPU support. */
int IritGrapCGDrawDTexture(IPObjectStruct *PObj);
void IritGrapCGFreeDTexture(IPObjectStruct *PObj);
int IritGrapCGFfdDraw(IPObjectStruct *PObj);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* IRIT_GRAP_LIB_H */

