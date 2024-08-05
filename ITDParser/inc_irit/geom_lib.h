/******************************************************************************
* Geom_lib.h - header file of the geometry library.			     *
*******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute	of Technology		     *
*******************************************************************************
* Written by Gershon Elber, June 1996.					     *
******************************************************************************/

#ifndef	IRIT_GEOM_LIB_H
#define IRIT_GEOM_LIB_H

#include "inc_irit/irit_sm.h"
#include "inc_irit/misc_lib.h"
#include "inc_irit/miscattr.h"

typedef enum {
    GEOM_ERR_NO_OGL_SUPPORT,
    GEOM_ERR_OGL_NO_X_SERVER,
    GEOM_ERR_X_NO_OGL_SERVER,
    GEOM_ERR_X_NO_VISUAL,
    GEOM_ERR_X_NO_GCONTEXT,
    GEOM_ERR_CH_STACK_OVERFLOW,
    GEOM_ERR_CH_STACK_UNDERFLOW,
    GEOM_ERR_NO_INSTANCE_ORIGIN,
    GEOM_ERR_ANIM_MAT_OR_CRV,
    GEOM_ERR_UNKNOWN_ANIM_CRVS,
    GEOM_ERR_NO_ANIM_CRVS,
    GEOM_ERR_UNEQUAL_NUM_OF_POLYS,
    GEOM_ERR_UNEQUAL_NUM_OF_VRTXS,
    GEOM_ERR_TOO_MANY_ADJACENCIES,
    GEOM_ERR_NO_IRIT_PATH,
    GEOM_ERR_INVALID_FONT,
    GEOM_ERR_MSC_TOO_FEW_PTS,
    GEOM_ERR_MSC_COLIN_CIRC,
    GEOM_ERR_TRIANGLES_ONLY,
    GEOM_ERR_TRI_RECT_ONLY,
    GEOM_ERR_INVALID_POLYGON,
    GEOM_ERR_VRTX_MTCH_FAILED,
    GEOM_ERR_EXPCT_POLYHEDRA,
    GEOM_ERR_EXPCT_POLYLINE,
    GEOM_ERR_EXPCT_LIST_OBJ,
    GEOM_ERR_EXPCT_TWO_PTS,
    GEOM_ERR_PROJ_FAILED,
    GEOM_ERR_DECIM_BDRY_FAILED,
    GEOM_ERR_OPEN_OBJ_VOL_COMP,
    GEOM_ERR_NO_INV_MAT,
    GEOM_ERR_NO_POLY_PLANE,
    GEOM_ERR_NO_VRTX_NRML,
    GEOM_ERR_REGULAR_POLY,
    GEOM_ERR_REORIENT_STACK_OF,
    GEOM_ERR_DISJOINT_PARTS,
    GEOM_ERR_VRTX_MUST_HAVE_NRML,
    GEOM_ERR_MISS_VRTX_IDX,
    GEOM_ERR_CMPLX_T_JUNC,
    GEOM_ERR_TOL_INVALID,
    GEOM_ERR_TEXTURE_NOT_FOUND, 
    GEOM_ERR_MULTIPLE_GEOMETRIES, 
    GEOM_ERR_TEXTURE_MIXED_ALPHA,
    GEOM_ERR_POLY_NOT_IN_MESH,

    GEOM_ERR_UNDEFINE_ERR
} IritGeomFatalErrorType;

typedef enum {
    GM_FIT_OTHER = -1,
    GM_FIT_PLANE = 0,
    GM_FIT_SPHERE,
    GM_FIT_CYLINDER,
    GM_FIT_CIRCLE,
    GM_FIT_CONE,
    GM_FIT_TORUS
} GMFittingModelType;

typedef enum {
    GM_GEN_PRIM_POLYS = 0,
    GM_GEN_PRIM_SRFS,
    GM_GEN_PRIM_MDLS,
    GM_GEN_PRIM_TVS,
    GM_GEN_PRIM_TVS_SNGLR,
    GM_GEN_PRIM_VMDLS,
    GM_GEN_PRIM_VMDLS_SNGLR
} GMGenPrimType;

#define GM_FIT_MODEL_MAX_PARAM 10

/* Used by the Ray & Polygon intersection (Jordan theorem): */
typedef enum {
    GM_GEOM_BELOW_RAY = -1,
    GM_GEOM_ON_RAY = 0,
    GM_GEOM_ABOVE_RAY = 1
} GMGeomRayRelationType;

#define GM_ANIM_DEFAULT_FILE_NAME	"IAnim"
#define PRIM_MIN_RESOLUTION		3

#define GM_ANIM_NO_DEFAULT_TIME IRIT_INFNTY

#define GM_QUAT_COPY(SrcQ, DstQ) IRIT_GEN_COPY(DstQ, SrcQ, sizeof(GMQuatType))

typedef struct GMAnimationStruct {
    IrtRType
	StartT,		                     /* Starting time of animation. */
	FinalT,		                  /* Termination time of animation. */
	Dt,		                         /* Step size pf animation. */
	RunTime;		              /* Current time of animation. */
    int TwoWaysAnimation,   /* Should the animation bounce back and forth!? */
	SaveAnimationGeom,          /* Save animation geometry into files!? */
	SaveAnimationImage,           /* Save animation images into files!? */
	BackToOrigin,	           /* Should we terminate at the beginning? */
	NumOfRepeat,			            /* How many iterations? */
	StopAnim,		   /* If TRUE, must stop the animation now. */
	SingleStep,			     /* Are we in single step mode? */
	TextInterface,		/* Are we communicating using a textual UI? */
	MiliSecSleep,	  /* How many milliseconds to sleep between frames. */
	_Count;						/* Used internally. */
    const char *ExecEachStep;	      /* Program to execute each iteration. */
    char BaseFileName[IRIT_LINE_LEN];/* Base name of animation files saved. */
} GMAnimationStruct;

#define GM_BBOX_MAX_DIM 19

typedef struct GMBBBboxStruct {
    IrtRType Min[GM_BBOX_MAX_DIM];
    IrtRType Max[GM_BBOX_MAX_DIM];
    int Dim;                  /* Actual number of valid dimensions in bbox. */
} GMBBBboxStruct;

#define GM_BBOX_RESET(BBox)     IRIT_ZAP_MEM(&(BBox), sizeof(GMBBBboxStruct));

#define GM_BBOX3D_INIT(BBox) { \
    GM_BBOX_RESET((BBox)); \
    (BBox).Min[0] = (BBox).Min[1] = (BBox).Min[2] = IRIT_INFNTY; \
    (BBox).Max[0] = (BBox).Max[1] = (BBox).Max[2] = -IRIT_INFNTY; \
    (BBox).Dim = 3; }

#define GM_BBOX3D_MAX_SIZE(BBox) \
    IRIT_MAX((BBox).Max[0] - (BBox).Min[0], \
	     IRIT_MAX((BBox).Max[1] - (BBox).Min[1], \
		      (BBox).Max[2] - (BBox).Min[2]))

#define GM_BBOX3D_HOLD_PT(Pt, BBox, Eps) /* Point in BBox to within Eps. */ \
    ((Pt)[0] >= (BBox) -> Min[0] - (Eps) && \
     (Pt)[0] <= (BBox) -> Max[0] + (Eps) && \
     (Pt)[1] >= (BBox) -> Min[1] - (Eps) && \
     (Pt)[1] <= (BBox) -> Max[1] + (Eps) && \
     (Pt)[2] >= (BBox) -> Min[2] - (Eps) && \
     (Pt)[2] <= (BBox) -> Max[2] + (Eps))

#define GM_BBOX3D_SAME(BBox1, BBox2, Eps)  /* Check for BBox similarity. */ \
     ((IRIT_FABS((BBox1).Max[0] - (BBox2).Max[0]) < Eps) && \
      (IRIT_FABS((BBox1).Max[1] - (BBox2).Max[1]) < Eps) && \
      (IRIT_FABS((BBox1).Max[2] - (BBox2).Max[2]) < Eps) && \
      (IRIT_FABS((BBox1).Min[0] - (BBox2).Min[0]) < Eps) && \
      (IRIT_FABS((BBox1).Min[1] - (BBox2).Min[1]) < Eps) && \
      (IRIT_FABS((BBox1).Min[2] - (BBox2).Min[2]) < Eps)) \

#define GM_BBOX3D_OVERLAP_EPS(BBox1, BBox2, Eps)/* Checks if BBs overlap.*/ \
    (!((BBox1) -> Max[0] < (BBox2) -> Min[0] - Eps || \
       (BBox1) -> Max[1] < (BBox2) -> Min[1] - Eps || \
       (BBox1) -> Max[2] < (BBox2) -> Min[2] - Eps || \
       (BBox2) -> Max[0] < (BBox1) -> Min[0] - Eps || \
       (BBox2) -> Max[1] < (BBox1) -> Min[1] - Eps || \
       (BBox2) -> Max[2] < (BBox1) -> Min[2] - Eps))

#define GM_BBOX3D_BBOX2_IN_BBOX1_EPS(BBox1, BBox2, Eps)/* Cntnmnt of BBs. */ \
    ((BBox1) -> Min[0] < (BBox2) -> Min[0] + Eps && \
     (BBox1) -> Min[1] < (BBox2) -> Min[1] + Eps && \
     (BBox1) -> Min[2] < (BBox2) -> Min[2] + Eps && \
     (BBox1) -> Max[0] > (BBox2) -> Max[0] - Eps && \
     (BBox1) -> Max[1] > (BBox2) -> Max[1] - Eps && \
     (BBox1) -> Max[2] > (BBox2) -> Max[2] - Eps)

#define GM_BBOX2D_OVERLAP_EPS(BBox1, BBox2, Eps)/* Checks if BBs overlap.*/ \
    (!((BBox1) -> Max[0] < (BBox2) -> Min[0] - Eps || \
       (BBox1) -> Max[1] < (BBox2) -> Min[1] - Eps || \
       (BBox2) -> Max[0] < (BBox1) -> Min[0] - Eps || \
       (BBox2) -> Max[1] < (BBox1) -> Min[1] - Eps))

#define GM_BBOX2D_BBOX2_IN_BBOX1_EPS(BBox1, BBox2, Eps)/* Cntnmnt of BBs. */ \
    ((BBox1) -> Min[0] < (BBox2) -> Min[0] + Eps && \
     (BBox1) -> Min[1] < (BBox2) -> Min[1] + Eps && \
     (BBox1) -> Max[0] > (BBox2) -> Max[0] - Eps && \
     (BBox1) -> Max[1] > (BBox2) -> Max[1] - Eps)


typedef IrtRType GMLsPoint[3];   /* The Z component is pretty much ignored. */

typedef struct GMLsLineSegStruct {
    struct GMLsLineSegStruct *Pnext;
    GMLsPoint Pts[2];
    long Id;			   /* Lines with unique ID never intersect. */
    VoidPtr PAux;	  /* Auxiliary back pointer - not used by ln_sweep. */
    struct GMLsIntersectStruct *Inters;
    GMLsPoint _MinVals;			        /* Bounding box on the line */
    GMLsPoint _MaxVals;
    GMLsPoint _Vec;		    /* A vector from first point to second. */
    IrtRType _ABC[3];			   /* Line equation as Ax + By + C. */
} GMLsLineSegStruct;

typedef struct GMLsIntersectStruct {
    struct GMLsIntersectStruct *Pnext;
    IrtRType t;
    IrtRType OtherT;
    struct GMLsLineSegStruct *OtherSeg;
    long Id;				      /* Unique ID of intersection. */
} GMLsIntersectStruct;

typedef struct GMBoxBVHInfoStruct {
    IrtRType Max[3];
    IrtRType Min[3];
    int Id;
} GMBoxBVHInfoStruct;

typedef struct GMFrustumSideStruct {
    IrtRType Corners[4][3];
    IrtRType Normal[3];
} GMFrustumSideStruct;

typedef struct GMGeneralFrustumInfoStruct {
    GMFrustumSideStruct Sides[6];
} GMGeneralFrustumInfoStruct;

/* Plane struct used to store the plane divsion metadata. */
typedef struct GMBspPtsSliceInfoStruct {
    struct GMBspPtsSliceInfoStruct *Pnext;
    struct IPAttributeStruct *Attr;
    IrtPlnType Pln;
    IrtPtType Pt;
    IrtVecType Normal, Tangent;
} GMBspPtsSliceInfoStruct;

/* The node of the Binary space partition tree.  */
typedef struct GMBspPtsNodeStruct {
    struct GMBspPtsNodeStruct *Pnext;
    struct IPAttributeStruct *Attr;
    struct IPVertexStruct *PC;
    struct GMBspPtsNodeStruct *Left, *Right;
    GMBspPtsSliceInfoStruct *DivisionSlice; 
} GMBspPtsNodeStruct;

/* Ogl depth peeling. */
typedef struct IritGeomOglZPeelParamStruct {
    unsigned Width, Height;				      /* In pixels. */
    struct GMBBBboxStruct XYDomain; /* Specefies the XY domain of the image.*/
    IrtRType Tolerance;				 /* The clipping tolerance. */
    IrtBType _CustomDomain;				     /* Inner data. */
} IritGeomOglZPeelParamStruct;

typedef struct IritGeomOglZPeelBufferStruct {
    struct IritGeomOglZPeelBufferStruct *Pnext;
    float *Buffer;
    int Width, Height;
} IritGeomOglZPeelBufferStruct;

typedef enum {            /* Predefined indices for the TransformIrtVecType */
    GM_QUAT_ROT_X = 0,
    GM_QUAT_ROT_Y,
    GM_QUAT_ROT_Z, 
    GM_QUAT_TRANS_X,
    GM_QUAT_TRANS_Y,
    GM_QUAT_TRANS_Z, 
    GM_QUAT_SCALE
} GMQuatTransformationsType;

typedef enum {
    GM_ZBUF_Z_LARGER,
    GM_ZBUF_Z_LARGER_EQUAL,
    GM_ZBUF_Z_SMALLER,
    GM_ZBUF_Z_SMALLER_EQUAL,
    GM_ZBUF_Z_ALWAYS,
    GM_ZBUF_Z_NEVER
} GMZTestsType;

/* Dummy declarations to prevent compiler warnings later on. */
typedef struct IPVertexStruct *IPVertexStructGMRef;
typedef struct IPPolygonStruct *IPPolygonStructGMRef;
typedef struct IPObjectStruct *IPObjectStructGMRef;
typedef struct IPPolyVrtxArrayStruct *IPPolyVrtxArrayStructGMRef;
typedef struct CagdPolylineStruct *CagdPolylineStructGMRef;

typedef	IrtRType GMQuatType[4];                            /* A Quaternion. */
typedef IrtRType GMQuatTransVecType[7];       /* Transformation parameters. */

typedef IrtRType (*IritGeomPolyOffsetAmountFuncType)(IrtRType *Coord);
typedef void (*IritGeomSetErrorFuncType)(IritGeomFatalErrorType ErrorFunc);
typedef void (*GMZBufferUpdateFuncType)(VoidPtr ZbufferID, int x, int y);
typedef void (*GMScanConvertApplyFuncType)(int x, int y);
typedef void (*GMTransObjUpdateFuncType)(const struct IPObjectStruct *OldPObj,
					 struct IPObjectStruct *NewPObj,
					 IrtHmgnMatType Mat,
					 int AnimUpdate);
typedef IrtRType (*GMFetchVertexPropertyFuncType)(struct IPVertexStruct *V,
						  struct IPPolygonStruct *Pl,
						  void *AuxData);
typedef void (*GMSphConeQueryCallBackFuncType)(struct IPVertexStruct *V);
typedef int (*GMSphConeQueryDirFuncType)(IrtVecType Vec, IrtRType ConeAngle);
typedef void (*IritGeomPolyAdjacncyVertexFuncType)(struct IPVertexStruct *V1,
					     struct IPVertexStruct *V2,
					     struct IPPolygonStruct *Pl1,
					     struct IPPolygonStruct *Pl2);
typedef struct IPObjectStruct *(*IritGeomTransObjUpdateAnimCrvsFuncType)(
						struct IPObjectStruct *PAnim,
						IrtHmgnMatType Mat);
typedef int (*GMMergePolyVrtxCmpFuncType)(struct IPVertexStruct *V1,
					  struct IPVertexStruct *V2,
					  IrtRType Eps);
typedef void (*GMMergeGeomInitFuncType)(VoidPtr GenericData, VoidPtr Entty);
typedef IrtRType (*GMMergeGeomDistFuncType)(VoidPtr GenericData,
					    VoidPtr Entty1,
					    VoidPtr Entty2);
typedef IrtRType (*GMMergeGeomKeyFuncType)(VoidPtr GenericData, VoidPtr Entty);
typedef int (*GMMergeGeomMergeFuncType)(VoidPtr GenericData, 
					void **Entty1,
					void **Entty2);
typedef void (*GMIdentifyTJunctionFuncType)(struct IPVertexStruct *V0,
					    struct IPVertexStruct *V1,
					    struct IPVertexStruct *V2,
					    struct IPPolygonStruct *Pl0,
					    struct IPPolygonStruct *Pl1,
					    struct IPPolygonStruct *Pl2);
typedef IrtRType *(*GMPointDeformVrtxDirFuncType)(const struct IPVertexStruct
						                           *V,
						  void *UserData);
typedef IrtRType (*GMPointDeformVrtxFctrFuncType)(struct IPVertexStruct *V,
						  void *UserData);
typedef IrtRType (*GMQuadWeightFuncType)(const struct CagdPolylineStruct *P,
					 int *VIndices, 
					 int numV,
					 void *UserData);

/* Points equally spread over a sphere. */
#define GM_DFLT_SPHERE_CONE_DENSITY 300

IRIT_GLOBAL_DATA_HEADER IrtVecType const
    GMSphereCoverVectors4[],
    GMSphereCoverVectors20[],
    GMSphereCoverVectors50[],
    GMSphereCoverVectors100[],
    GMSphereCoverVectors130[];

IRIT_GLOBAL_DATA_HEADER int _IritPrimGlblResolution;

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/* And prototypes of the functions: */
IrtRType IritGeomBasicSetEps(IrtRType Eps);
void IritGeomVecCopy(IrtVecType Vdst, const IrtVecType Vsrc);
void IritGeomVecNormalize(IrtVecType V);
IrtRType IritGeomVecLength(const IrtVecType V);
int IritGeomVecMinAbsValueIndex(const IrtVecType V);
int IritGeomVecMaxAbsValueIndex(const IrtVecType V);
void IritGeomVecCrossProd(IrtVecType Vres,
			  const IrtVecType V1,
			  const IrtVecType V2);
IrtRType IritGeomVecVecAngle(const IrtVecType V1,
			     const IrtVecType V2,
			     int Normalize);
void IritGeomSLerp(const IrtVecType V1,
		   const IrtVecType V2,
		   IrtRType t,
		   IrtVecType VOut);
IrtRType IritGeomPlanarVecVecAngle(const IrtVecType V1,
				   const IrtVecType V2,
				   int Normalize);
int IritGeomOrthogonalVector(const IrtVecType V, IrtVecType OV, int UnitLen);
int IritGeomCollinear3Pts(const IrtPtType Pt1,
			  const	IrtPtType Pt2,
			  const	IrtPtType Pt3);
int IritGeomCollinear3PtsInside(const IrtPtType Pt1,
				const IrtPtType	Pt2,
				const IrtPtType	Pt3);
int IritGeomCoplanar4Pts(const IrtPtType Pt1,
			 const IrtPtType Pt2,
			 const IrtPtType Pt3,
			 const IrtPtType Pt4);
IrtRType IritGeomVecDotProd(const IrtVecType V1, const IrtVecType V2);
IrtRType IritGeomVecDotProdLen(const IrtRType *V1,
			       const IrtRType *V2,
			       int Len);
void IritGeomVecReflectPlane(IrtVecType Dst,
			     IrtVecType Src,
			     IrtVecType PlaneNormal);

struct IPObjectStruct *IritGeomGenMatObjectRotX(const IrtRType *Degree);
struct IPObjectStruct *IritGeomGenMatObjectRotY(const IrtRType *Degree);
struct IPObjectStruct *IritGeomGenMatObjectRotZ(const IrtRType *Degree);
struct IPObjectStruct *IritGeomGenMatObjectTrans(const IrtVecType Vec);
struct IPObjectStruct *IritGeomGenMatObjectScale(const IrtVecType Vec);
struct IPObjectStruct *IritGeomGetMatTransPortion(
					  const	struct IPObjectStruct *MatObj,
					  int TransPortion);
struct IPPolygonStruct *IritGeomTransformPolyList(
					    const struct IPPolygonStruct *Pls,
					    IrtHmgnMatType Mat,
					    int IsPolygon);
GMTransObjUpdateFuncType IritGeomTransObjSetUpdateFunc(
				         GMTransObjUpdateFuncType UpdateFunc);
IritGeomTransObjUpdateAnimCrvsFuncType IritGeomTransObjSetAnimCrvUpdateFunc(
			IritGeomTransObjUpdateAnimCrvsFuncType AnimUpdateFunc);
struct IPObjectStruct *IritGeomTransformObj2UnitSize(
					    const struct IPObjectStruct *PObj);
struct IPObjectStruct *IritGeomTransformObjectInPlace(
						  struct IPObjectStruct *PObj,
						  IrtHmgnMatType Mat);
struct IPObjectStruct *IritGeomTransformObject(
					    const struct IPObjectStruct *PObj,
					    IrtHmgnMatType Mat);
struct IPObjectStruct *IritGeomTransformObjectList(
					    const struct IPObjectStruct *PObj,
					    IrtHmgnMatType Mat);
struct IPObjectStruct *IritGeomTransObjUpdateAnimCrvs(
						 struct IPObjectStruct *PAnim,
						 IrtHmgnMatType Mat);
struct IPObjectStruct *IritGeomObjExplodeObject(
					   const struct IPObjectStruct *PObj,
					   int ExplosionType,
					   const IrtVecType CenterOfExplosion,
					   IrtRType ExplosionAmount,
					   const IrtVecType LineExplodeDir,
					   int AnimCrvs);

int IritGeomObjectNumCoordinates(const struct IPObjectStruct *Obj);
struct IPObjectStruct *IritGeomGenMatObjectZ2Dir(const IrtVecType Dir);
struct IPObjectStruct *IritGeomGenMatObjectZ2Dir2(const IrtVecType Dir,
						  const	IrtVecType Dir2);
struct IPObjectStruct *IritGeomGenMatObjectRotVec(const IrtVecType Vec,
						  const	IrtRType *Degree);
struct IPObjectStruct *IritGeomGenMatObjectV2V(const IrtVecType V1,
					       const IrtVecType	V2);
struct IPObjectStruct *IritGeomGenMatrix3Pts2EqltrlTri(const IrtPtType Pt1,
						       const IrtPtType Pt2,
						       const IrtPtType Pt3);
IritGeomOglZPeelBufferStruct *IritGeomOglZPeel(
					  const struct IPObjectStruct *Obj,
					  IritGeomOglZPeelParamStruct *Params);
struct IPObjectStruct *IritGeomOglZPeelTesselate(
				    struct IritGeomOglZPeelBufferStruct *Peel,
				    const GMBBBboxStruct *Domain,
				    int LayerIndex);

/* General basic computational geometry routines: */
IrtRType IritGeomDistPointPoint(const IrtPtType P1, const IrtPtType P2);
int IritGeomFindLinComb2Vecs(const IrtVecType V1,
			     const IrtVecType V2,
			     const IrtVecType V,
			     IrtRType w[2]);
int IritGeomLineFrom2Points(IrtLnType Line,
			    const IrtPtType Pt1, 
			    const IrtPtType Pt2);
void IritGeomPointVecFromLine(const IrtLnType Line,
			      IrtPtType Pt,
			      IrtVecType Dir);
int IritGeomPlaneFrom3Points(IrtPlnType Plane,
			     const IrtPtType Pt1,
			     const IrtPtType Pt2,
			     const IrtPtType Pt3);
IrtRType IritGeomPointFromPointLine(const IrtPtType Point,
				    const IrtPtType Pl,
				    const IrtPtType Vl,
				    IrtPtType ClosestPoint);
IrtRType IritGeomDistPointLine(const IrtPtType Point,
			       const IrtPtType Pl,
			       const IrtPtType Vl);
IrtRType IritGeomDistPointPlane(const IrtPtType Point, const IrtPlnType Plane);
int IritGeomPointFromPointPlane(const IrtPtType Pt,
				const IrtPlnType Plane,
				IrtPtType ClosestPoint);
int IritGeomVectorFromVectorPlane(const IrtVecType Vec,
				  const	IrtVecType PlaneN,
				  IrtPtType ProjVec);
int IritGeomPointFromLinePlane(const IrtPtType Pl,
			       const IrtPtType Vl,
			       const IrtPlnType	Plane,
			       IrtPtType InterPoint,
			       IrtRType	*t);
int IritGeomPointFromLinePlane01(const IrtPtType Pl,
				 const IrtPtType Vl,
				 const IrtPlnType Plane,
				 IrtPtType InterPoint,
				 IrtRType *t);
int IritGeomPointFromPlanarLineLine(const IrtPtType Pl1,
				    const IrtPtType Vl1,
				    const IrtPtType Pl2,
				    const IrtPtType Vl2,
				    IrtPtType Pi,
				    IrtRType *t1,
				    IrtRType *t2);
int IritGeom2PointsFromLineLine(const IrtPtType Pl1,
				const IrtPtType	Vl1,
				const IrtPtType	Pl2,
				const IrtPtType	Vl2,
				IrtPtType Pt1,
				IrtRType *t1,
				IrtPtType Pt2,
				IrtRType *t2);
IrtRType IritGeomDistLineLine(const IrtPtType Pl1,
			      const IrtPtType Vl1,
			      const IrtPtType Pl2,
			      const IrtPtType Vl2);
int IritGeomPointFrom3Planes(const IrtPlnType Pl1,
			     const IrtPlnType Pl2,
			     const IrtPlnType Pl3,
			     IrtPtType Pt);
int IritGeomLineFrom2Planes(const IrtPlnType Pl1,
			    const IrtPlnType Pl2,
			    IrtPtType Pt,
			    IrtVecType Dir);

IrtRType IritGeomDistPolyPt(const struct IPPolygonStruct *Pl,
			    IrtPtType Pt,
			    const struct IPVertexStruct	**ExtremeV,
			    int	MaxDist);
IrtRType IritGeomDistPolyPt2(const struct IPPolygonStruct *Pl,
			     int IsPolyline,
			     IrtPtType Pt,
			     struct IPVertexStruct **ExtremeV,
			     int MaxDist);
IrtRType IritGeomDistPolyPoly(const struct IPPolygonStruct *Pl1,
			      const struct IPPolygonStruct *Pl2,
			      struct IPVertexStruct **V1,
			      struct IPVertexStruct **V2,
			      int TagIgnoreV);

int IritGeomApproxGeodesicDist(struct IPPolygonStruct *PlMesh,
			       const struct IPPolygonStruct *HitPl,
			       const IrtPtType HitPt,
			       IrtRType	MaxDist);

int IritGeomPolygonPlaneInter(const struct IPPolygonStruct *Pl,
			      const IrtPlnType Pln,
			      IrtRType *MinDist);
int IritGeomPolygonListPlaneInter(const struct IPPolygonStruct *Pl,
				  const IrtPlnType Pln);
int IritGeomSplitPolygonAtPlane(struct IPPolygonStruct *Pl,
				const IrtPlnType Pln);
void IritGeomPolygonGetCentroid(const struct IPPolygonStruct *Polyline,
				IrtPtType *Pt);
int IritGeomSegmentRayInter(const IrtPtType v1, 
			    const IrtPtType v2, 
			    const IrtPtType Pt, 
			    const IrtPtType Dir, 
			    IrtPtType	InterPt,
			    IrtRType Eps);

IrtRType IritGeomPolyPlaneClassify(const struct IPPolygonStruct *Pl,
				   const IrtPlnType Pln);
int IritGeomTrianglePointInclusion(const IrtRType *V1,
				   const IrtRType *V2,
				   const IrtRType *V3,
				   const IrtPtType Pt);
int IritGeomPolygonPointInclusion(const struct IPPolygonStruct *Pl,
				  const	IrtPtType Pt,
				  IrtRType OnBndryEps);
IrtRType IritGeomAreaSphericalTriangle(const IrtVecType Dir1,
				       const IrtVecType	Dir2,
				       const IrtVecType	Dir3);
IrtRType IritGeomAngleSphericalTriangle(const IrtVecType Dir,
					const IrtVecType ODir1,
					const IrtVecType ODir2);
int IritGeomPolygonPointInclusion3D(const struct IPPolygonStruct *Pl,
				    const IrtPtType Pt);

int IritGeomPolygonXYRayInter(const struct IPPolygonStruct *Pl,
			      const IrtPtType PtRay,
			      int RayAxes);
int IritGeomPolygonXYRayInter2(const struct IPPolygonStruct *Pl,
			       const IrtPtType PtRay,
			       int RayAxes,
			       struct IPVertexStruct **FirstInterV,
			       IrtRType	*FirstInterP,
			       IrtRType	*AllInters);
int IritGeomPolygonXYRayInter3D(const struct IPPolygonStruct *Pl,
				const IrtPtType	PtRay,
				int RayAxes);
int IritGeomPolylineRayInter3D(const struct IPPolygonStruct *Polyline,
			       const IrtPtType Pt,
			       const IrtVecType Dir,
			       IrtPtType InterPt);
struct IPPolygonStruct *IritGeomPolyHierarchy2SimplePoly(
					      struct IPPolygonStruct *Root,
					      struct IPPolygonStruct *Islands);
void IritGeomGenTransMatrixZ2Dir(IrtHmgnMatType Mat,
				 const IrtVecType Trans,
				 const IrtVecType Dir,
				 IrtRType Scale);
void IritGeomGenMatrixX2Dir(IrtHmgnMatType Mat, const IrtVecType Dir);
void IritGeomGenMatrixY2Dir(IrtHmgnMatType Mat, const IrtVecType Dir);
void IritGeomGenMatrixZ2Dir(IrtHmgnMatType Mat, const IrtVecType Dir);
void IritGeomGenTransMatrixZ2Dir2(IrtHmgnMatType Mat,
				  const	IrtVecType Trans,
				  const	IrtVecType Dir,
				  const	IrtVecType Dir2,
				  IrtRType Scale);
void IritGeomGenMatrixZ2Dir2(IrtHmgnMatType Mat,
			     const IrtVecType Dir,
			     const IrtVecType Dir2);
int IritGeomMatFromPosDir(const IrtPtType Pos,
			  const	IrtVecType Dir,
			  const	IrtVecType UpDir,
			  IrtHmgnMatType M);
void IritGeomGenMatrixRotVec(IrtHmgnMatType Mat,
			     const IrtVecType Vec,
			     IrtRType Angle);
void IritGeomGenMatrixRotV2V(IrtHmgnMatType Mat,
			     const IrtVecType V1,
			     const IrtVecType V2);
int IritGeomGenMatrix4Pts2Affine4Pts(const IrtPtType P0,
				     const IrtPtType P1,
				     const IrtPtType P2,
				     const IrtPtType P3,
				     const IrtPtType Q0,
				     const IrtPtType Q1,
				     const IrtPtType Q2,
				     const IrtPtType Q3,
				     IrtHmgnMatType Trans);
void IritGeomGenProjectionMat(const IrtPlnType ProjPlane,
			      const IrtRType EyePos[4],
			      IrtHmgnMatType Mat);
void IritGeomGenReflectionMat(const IrtPlnType ReflectPlane,
			      IrtHmgnMatType Mat);
int IritGeom3Pts2EqltrlTriMat(const IrtPtType Pt1Orig,
			      const IrtPtType Pt2Orig,
			      const IrtPtType Pt3Orig,
			      IrtHmgnMatType Mat);
IrtRType *IritGeomBaryCentric3Pts2DToData(const IrtPtType Pt1,
					  const	IrtPtType Pt2,
					  const	IrtPtType Pt3,
					  const	IrtPtType Pt,
					  IrtVecType RetVal);
IrtRType *IritGeomBaryCentric3PtsToData(const IrtPtType Pt1,
					const IrtPtType	Pt2,
					const IrtPtType	Pt3,
					const IrtPtType	Pt,
					IrtVecType RetVal);
int IritGeom2PointsFromCircCirc(const IrtPtType Center1,
				IrtRType Radius1,
				const IrtPtType	Center2,
				IrtRType Radius2,
				IrtPtType Inter1,
				IrtPtType Inter2);
int IritGeom2PointsFromCircCirc3D(const IrtPtType Cntr1,
				  const	IrtVecType Nrml1,
				  IrtRType Rad1,
				  const	IrtPtType Cntr2,
				  const	IrtVecType Nrml2,
				  IrtRType Rad2,
				  IrtPtType Inter1,
				  IrtPtType Inter2);
int IritGeomCircleFrom3Points(IrtPtType Center,
			      IrtRType *Radius,
			      const IrtPtType Pt1,
			      const IrtPtType Pt2,
			      const IrtPtType Pt3);
int IritGeomCircleFrom2Pts2Tans(IrtPtType Center,
				IrtRType *Radius,
				const IrtPtType	Pt1,
				const IrtPtType	Pt2,
				const IrtVecType Tan1,
				const IrtVecType Tan2);
int IritGeomCircleFromLstSqrPts(IrtPtType Center,
				IrtRType *Radius,
				const IrtPtType	*Pts,
				int PtsSize);
int IritGeom2BiTansFromCircCirc(const IrtPtType Center1,
				IrtRType Radius1,
				const IrtPtType	Center2,
				IrtRType Radius2,
				int OuterTans,
				IrtPtType TanPts[2][2]);
int IritGeom2TanLinesFromCircCirc(const IrtPtType Center1,
				  IrtRType Radius1,
				  const	IrtPtType Center2,
				  IrtRType Radius2,
				  int OuterTans,
				  IrtLnType Tans[2]);
int IritGeomIsPtInsideCirc(const IrtRType *Point,
			   const IrtRType *Center,
			   IrtRType Radius);
int IritGeomIsPtOnCirc(const IrtRType *Point,
		       const IrtRType *Center,
		       IrtRType	Radius);
IrtRType IritGeomAreaOfTriangle(const IrtRType *Pt1,
				const IrtRType *Pt2,
				const IrtRType *Pt3);

/* Convex polygon - ray intersections in R3. */

int IritGeomRayCnvxPolygonInter(const IrtPtType RayOrigin,
				const IrtVecType RayDir,
				const struct IPPolygonStruct *Pl,
				IrtPtType InterPoint);
int IritGeomPointInsideCnvxPolygon(const IrtPtType Pt,
				   const struct	IPPolygonStruct	*Pl);
int IritGeomPointOnPolygonBndry(const IrtPtType Pt,
				const struct IPPolygonStruct *Pl,
				IrtRType Eps);

/* List of convex polygon - ray intersections in R3. */
int IritGeomRayCnvxPolygonListInter(const IrtPtType RayOrigin,
				    const IrtVecType RayDir,
				    const struct IPPolygonStruct *Plgns,
				    IrtPtType InterPoint);
int IritGeomRayCnvxPolygonListInter2(
				  const IrtPtType RayOrigin,
				  const IrtVecType RayDir,
				  const struct IPPolygonStruct *Plgns,
				  IrtPtType *AllInterPts,
				  const struct IPPolygonStruct **AllInterPls);

/* Polynomial solvers. */
int IritGeomSolveQuadraticEqn(IrtRType A, IrtRType B, IrtRType *Sols);
int IritGeomSolveQuadraticEqn2(IrtRType B,
			       IrtRType	C,
			       IrtRType	*RSols,
			       IrtRType	*ISols);
int IritGeomSolveCubicEqn(IrtRType A, IrtRType B, IrtRType C, IrtRType *Sols);
int IritGeomSolveCubicEqn2(IrtRType A,
			   IrtRType B,
			   IrtRType C, 
			   IrtRType *RSols,
			   IrtRType *ISols);
int IritGeomSolveQuarticEqn(IrtRType A,
			    IrtRType B,
			    IrtRType C,
			    IrtRType D,	
			    IrtRType *Sols);
void IritGeomComplexRoot(IrtRType RealVal,
			 IrtRType ImageVal,
			 IrtRType *RealRoot,
			 IrtRType *ImageRoot);

/* Geometric properties routines: */
IrtRType IritGeomPolyLength(const struct IPPolygonStruct *Pl);
int IritGeomPolyCentroid(const struct IPPolygonStruct *Pl, IrtPtType Centroid);
IrtRType IritGeomPolyObjectArea(const struct IPObjectStruct *PObj,
				int SingedArea);
IrtRType IritGeomPolyOnePolyArea(const struct IPPolygonStruct *Pl,
				 int SingedArea);
IrtRType IritGeomPolyOnePolyXYArea(const struct IPVertexStruct *VHead,
				   int SingedArea);

IrtRType IritGeomPolyObjectVolume(const struct IPObjectStruct *PObj);
int IritGeomInverseBilinearMap(const IrtPtType P00,
			       const IrtPtType P01,
			       const IrtPtType P10,
			       const IrtPtType P11,
			       const IrtPtType PtInt,
			       IrtPtType UV[2]);

/* Box BVH routines. */
struct GMBoxBVHStruct *IritGeomBoxBVHCreate(const GMBoxBVHInfoStruct **Boxes,
					    int	BoxNum,	
					    int	Size);
void IritGeomBoxBVHFree(struct GMBoxBVHStruct *BVH);
int IritGeomBoxBVHGetBoxNum(const struct GMBoxBVHStruct *BVH);
int IritGeomBoxBVHGetBoxIntersection(const struct GMBoxBVHStruct *BVH,
				     const IrtRType *Max,
				     const IrtRType *Min,
				     int *Res);
int IritGeomBoxBVHGetFrustumIntersection(const struct GMBoxBVHStruct *BVH,
					 const IrtRType	*Max,
					 const IrtRType	*Min,
					 IrtRType Angle,
					 int *Res);
int IritGeomBoxBVHTestFrustumIntersection(const struct GMBoxBVHStruct *BVH,
					  const	IrtRType *Max,
					  const	IrtRType *Min,
					  IrtRType Angle);
void GMBoxBVHConstructGeneralAlignedFrustum(
					 const IrtRType Center[3],
					 IrtRType SizeX,
					 IrtRType SizeY,
					 const IrtRType XAxis[3],
					 const IrtRType HeightAxis[3],
					 IrtRType Height,
					 IrtRType XAngle,
					 IrtRType YAngle,
					 GMGeneralFrustumInfoStruct *Frustum);
int IritGeomBoxBVHGetGeneralAlignedFrustumIntersection(
					     const struct GMBoxBVHStruct *BVH,
					     const IrtRType Center[3],
					     IrtRType SizeX,
					     IrtRType SizeY,
					     const IrtRType XAxis[3],
					     const IrtRType HeightAxis[3],
					     IrtRType Height,
					     IrtRType XAngle,
					     IrtRType YAngle,
					     int *Res);
struct IPPolygonStruct *IritGeomBoxBVHDbgConstructGeneralAlignedFrustum(
						  const	IrtRType Center[3],
						  IrtRType SizeX,
						  IrtRType SizeY,
						  const	IrtRType XAxis[3],
						  const	IrtRType HeightAxis[3],
						  IrtRType Height,
						  IrtRType XAngle,
						  IrtRType YAngle);
int IritGeomBoxBVHGetGeneralAlignedFrustumIntersection2(
				    const struct GMBoxBVHStruct *BVH,
				    const GMGeneralFrustumInfoStruct *Frustum,
				    int *Res);
IrtBType IritGeomGetGeneralAlignedFrustumPtsIntersection2(
				    const GMGeneralFrustumInfoStruct *Frustum,
				    const IrtRType **Pts,
				    int	NumPts,
				    IrtBType RationalPts,
				    IrtRType Tolerance);

/* Functions from sphere's cone distribution - Sph_Cone.c. */
const IrtVecType *IritGeomSphConeGetPtsDensity(int *n);
VoidPtr IritGeomSphConeQueryInit(struct IPObjectStruct *PObj, int n);
void IritGeomSphConeQueryFree(VoidPtr SphCone);
void IritGeomSphConeQueryGetVectors(VoidPtr SphConePtr,
				    IrtVecType Dir,
				    IrtRType Angle,
				    GMSphConeQueryCallBackFuncType SQFunc);
void IritGeomSphConeQuery2GetVectors(VoidPtr SphConePtr,
				     GMSphConeQueryDirFuncType SQQuery,
				     GMSphConeQueryCallBackFuncType SQFunc);

/* Functions from the convex hull computation package. */

int IritGeomConvexHull(IrtE2PtStruct *DTPts, int *NumOfPoints);
int IritGeomMonotonePolyConvex(struct IPVertexStruct *VHead, int Cnvx);

/* Functions from the minimum spanning circle/sphere packages. */
int IritGeomMinSpanCirc(IrtE2PtStruct *DTPts,
			int NumOfPoints,
			IrtE2PtStruct *Center,
			IrtRType *Radius);
int IritGeomMinSpanConeAvg(IrtVecType *DTVecs,
			   int VecsNormalized,
			   int NumOfPoints,
			   IrtVecType Center,
			   IrtRType *Angle);
int IritGeomMinSpanCone(IrtVecType *DTVecs,
			int VecsNormalized,
			int NumOfPoints,
			IrtVecType Center,
			IrtRType *Angle);
int IritGeomMinSpanSphere(IrtE3PtStruct *DTPts,
			  int NumOfPoints,
			  IrtE3PtStruct	*Center,
			  IrtRType *Radius);
int IritGeomSphereWith3Pts(IrtE3PtStruct *Pts,
			   IrtRType *Center,
			   IrtRType *RadiusSqr);
int IritGeomSphereWith4Pts(IrtE3PtStruct *Pts,
			   IrtRType *Center,
			   IrtRType *RadiusSqr);

/* Functions to extract silhouette and boundary curves from polygonal data. */
VoidPtr IritGeomSilPreprocessPolys(struct IPObjectStruct *PObj, int n);
int IritGeomSilPreprocessRefine(VoidPtr PrepSils, int n);
struct IPObjectStruct *IritGeomSilExtractSilDirect(struct IPObjectStruct *PObj,
						   IrtHmgnMatType ViewMat);
struct IPObjectStruct *IritGeomSilExtractSilDirect2(
					       struct IPObjectStruct *PObjReg,
					       IrtHmgnMatType ViewMat);
struct IPObjectStruct *IritGeomSilExtractSil(VoidPtr PrepSils,
					     IrtHmgnMatType ViewMat);
struct IPObjectStruct *IritGeomSilExtractDiscont(
					       struct IPObjectStruct *PObjReg,
					       IrtRType MinAngle);
struct IPObjectStruct *IritGeomSilExtractBndry(struct IPObjectStruct *PObj);
void IritGeomSilProprocessFree(VoidPtr PrepSils);
int IritGeomSilOrigObjAlive(int ObjAlive);

/* Functions from the animate package. */
void IritGeomAnimResetAnimStruct(GMAnimationStruct *Anim);
void IritGeomAnimGetAnimInfoText(GMAnimationStruct *Anim);
int IritGeomAnimHasAnimation(const struct IPObjectStruct *PObjs);
int IritGeomAnimHasAnimationOne(const struct IPObjectStruct *PObj);
int IritGeomAnimAffineTransAnimation(const struct IPObjectStruct *PObjs,
				     IrtRType	Trans,
				     IrtRType	Scale);
int IritGeomAnimAffineTransAnimationOne(const struct IPObjectStruct *PObj,
					IrtRType Trans,
					IrtRType Scale);
int IritGeomAnimAffineTransAnimation2(const struct IPObjectStruct *PObjs,
				      IrtRType Min,
				      IrtRType Max);
int IritGeomAnimAffineTransAnimationOne2(const struct IPObjectStruct *PObj,
					 IrtRType Min,
					 IrtRType Max);
void IritGeomAnimFindAnimationTimeOne(GMAnimationStruct *Anim,
				      const struct IPObjectStruct *PObj);
void IritGeomAnimFindAnimationTime(GMAnimationStruct *Anim,
				   const struct	IPObjectStruct *PObjs);
void IritGeomAnimSaveIterationsToFiles(GMAnimationStruct *Anim,
				       struct IPObjectStruct *PObjs);
void IritGeomAnimSaveIterationsAsImages(GMAnimationStruct *Anim,
					struct IPObjectStruct *PObjs);
IrtRType IritGeomExecuteAnimationEvalMat(struct IPObjectStruct *AnimationP,
					 IrtRType Time,
					 IrtHmgnMatType	ObjMat);
void IritGeomAnimDoAnimation(GMAnimationStruct *Anim,
			     struct IPObjectStruct *PObjs);
int IritGeomAnimSetReverseHierarchyMatProd(int ReverseHierarchyMatProd);
int IritGeomAnimSetAnimInternalNodes(int AnimInternalNodes);
int IritGeomAnimHasDispPropAttrib(const struct IPObjectStruct *PObj);
void IritGeomAnimEvalAnimation(IrtRType t, struct IPObjectStruct *PObj);
void IritGeomAnimEvalAnimationList(IrtRType t, struct IPObjectStruct *PObjList);
struct IPObjectStruct *IritGeomAnimEvalObjAtTime(IrtRType t,
						 struct	IPObjectStruct *PObj);
void IritGeomAnimDoSingleStep(GMAnimationStruct *Anim,
			      struct IPObjectStruct *PObjs);
int IritGeomAnimCheckInterrupt(GMAnimationStruct *Anim);

/* Functions from the bbox package. */
int IritGeomBBSetBBoxPrecise(int Precise);
GMBBBboxStruct *IritGeomBBComputeBboxObject(const struct IPObjectStruct *PObj,
					    GMBBBboxStruct *Bbox,
					    int	HandleInvisibelObjs);
GMBBBboxStruct *IritGeomBBComputeBboxObjectList(
					    const struct IPObjectStruct *PObj,
					    GMBBBboxStruct *Bbox,
					    int HandleInvisibelObjs);
const struct IPObjectStruct *IritGeomBBSetGlblBBInstncObjList(
				  const struct IPObjectStruct *BBInstObjList);
GMBBBboxStruct *IritGeomBBComputeOnePolyBbox(
					  const struct IPPolygonStruct *PPoly,
					  GMBBBboxStruct *Bbox);
GMBBBboxStruct *IritGeomBBComputePolyListBbox(
					   const struct IPPolygonStruct *PPoly,
					   GMBBBboxStruct *Bbox);
GMBBBboxStruct *IritGeomBBComputePointBbox(
				     const IrtRType *Pt, GMBBBboxStruct *Bbox);
GMBBBboxStruct *IritGeomBBMergeBboxTo(const GMBBBboxStruct *Bbox1,
				      const GMBBBboxStruct *Bbox2,
				      GMBBBboxStruct *MergedBbox);
GMBBBboxStruct *IritGeomBBMergeBbox(GMBBBboxStruct *MergedBbox,
				    const GMBBBboxStruct *Bbox);

/* Functions from the convex polygons package. */
int IritGeomConvexRaysToVertices(int RaysToVertices);
int IritGeomConvexNormalizeNormal(int NormalizeNormals);
struct IPObjectStruct *IritGeomConvexPolyObjectN(
					     const struct IPObjectStruct *PObj,
					     int HandleNormals);
void IritGeomConvexPolyObject(struct IPObjectStruct *PObj,
			      int HandleNormals);
int IritGeomIsConvexPolygon2(const struct IPPolygonStruct *Pl);
int IritGeomIsConvexPolygon(struct IPPolygonStruct *Pl);

struct IPPolygonStruct *IritGeomSplitNonConvexPoly(struct IPPolygonStruct *Pl,
						   int HandleNormals);
void IritGeomGenRotateMatrix(IrtHmgnMatType Mat, const IrtVecType Dir);

/* Functions from general polygons to triangles package. */
struct IPPolygonStruct *IritGeomTriangulatePolygon(
					  const struct CagdPolylineStruct *Pl);
struct IPPolygonStruct *IritGeomTriangulatePolygon2(
					    const struct IPPolygonStruct *Pl);
struct IPPolygonStruct *IritGeomTriangulatePolygonList(
				       const struct IPPolygonStruct *PlgnList);
/* Functions from general polygons to quads. */
struct IPPolygonStruct *IritGeomQuadrangulatePolygon(const struct
					       CagdPolylineStruct *Pl,
					       GMQuadWeightFuncType WF,
					       void *UserData);
struct IPPolygonStruct *IritGeomQuadrangulatePolygon2(const struct 
						IPPolygonStruct	*Pl,
						GMQuadWeightFuncType WF,
						void *UserData);
struct IPPolygonStruct *IritGeomQuadrangulatePolygonList(const struct 
						   IPPolygonStruct *PlgnList,
						   GMQuadWeightFuncType	WF,
						   void	*UserData);
IrtRType IritGeomQuadAreaPerimeterRatioWeightFunc(
					   const struct CagdPolylineStruct *P,
					   const int *VIndices, 
					   int	numV);

/* Functions from the normal/uv/rgb/etc. interpolation package. */
void IritGeomUpdateVerticesByInterp(struct IPPolygonStruct *PlList,
				    const struct IPPolygonStruct *OriginalPl);
void IritGeomUpdateVertexByInterp(struct IPVertexStruct *VUpdate,
				  const	struct IPVertexStruct *V,
				  const	struct IPVertexStruct *VNext,
				  int DoRgb,
				  int DoUV,
				  int DoNrml);
int IritGeomCollinear3Vertices(const struct IPVertexStruct *V1,
			       const struct IPVertexStruct *V2,
			       const struct IPVertexStruct *V3);
int IritGeomEvalWeightsVFromPl(const IrtRType *Coord,
			       const struct IPPolygonStruct *Pl,
			       IrtRType *Wgt);
void IritGeomInterpVrtxNrmlBetweenTwo(struct IPVertexStruct *V,
				      const struct IPVertexStruct *V1,
				      const struct IPVertexStruct *V2);
void IritGeomInterpVrtxNrmlBetweenTwo2(IrtPtType Pt,
				       IrtVecType Normal,
				       const struct IPVertexStruct *V1,
				       const struct IPVertexStruct *V2,
				       int Normalize);
int IritGeomInterpVrtxNrmlFromPl(struct IPVertexStruct *V,
				 const struct IPPolygonStruct *Pl);
int IritGeomInterpVrtxRGBBetweenTwo(struct IPVertexStruct *V,
				    const struct IPVertexStruct	*V1,
				    const struct IPVertexStruct	*V2);
int IritGeomInterpVrtxRGBFromPl(struct IPVertexStruct *V,
				const struct IPPolygonStruct *Pl);
int IritGeomInterpVrtxUVBetweenTwo(struct IPVertexStruct *V,
				   const struct	IPVertexStruct *V1,
				   const struct	IPVertexStruct *V2);
int IritGeomInterpVrtxUVFromPl(struct IPVertexStruct *V,
			       const struct IPPolygonStruct *Pl);
void IritGeomBlendNormalsToVertices(struct IPPolygonStruct *PlList,
				    IrtRType MaxAngle);
void IritGeomFixOrientationOfPolyModel(struct IPPolygonStruct *Pls);
void IritGeomFixNormalsOfPolyModel(struct IPPolygonStruct *PlList,
				   int TrustFixedPt);
void IritGeomFixPolyNormals(struct IPObjectStruct *PObj, int TrustFixPt);

/* Functions from the line sweep package. */
void IritGeomLineSweep(GMLsLineSegStruct **Lines);

/* Functions from the polygonal cleaning package. */
int IritGeomTwoPolySameGeom(const struct IPPolygonStruct *Pl1,
			    const struct IPPolygonStruct *Pl2,
			    IrtRType Eps);
struct IPPolygonStruct *IritGeomCleanUpDupPolys(
					   struct IPPolygonStruct **PPolygons,
					   IrtRType Eps);
struct IPPolygonStruct *IritGeomCleanUpPolygonList(
					   struct IPPolygonStruct **PPolygons,
					   IrtRType Eps);
struct IPPolygonStruct *IritGeomCleanUpPolylineList(
					  struct IPPolygonStruct **PPolylines,
					  IrtRType Eps);
struct IPPolygonStruct *IritGeomCleanUpPolylineList2(
					   struct IPPolygonStruct *PPolylines);
int IritGeomIsPolygonPlanar(const struct IPPolygonStruct *Pl, IrtRType Tol);
struct IPPolygonStruct *IritGeomVerifyPolygonsPlanarity(
						 struct IPPolygonStruct *Pls,
						 IrtRType Tol);
void IritGeomVrtxListToCircOrLin(struct IPPolygonStruct *Pls, int DoCirc);
void IritGeomVrtxListToCircOrLinDup(struct IPPolygonStruct *Pls, int DoCirc);
struct IPVertexStruct *IritGeomFilterInteriorVertices(
						 struct IPVertexStruct *VHead,
						 IrtRType MinTol,
						 int n);
struct IPPolygonStruct *IritGeomClipPolysAgainstPlane(
					     struct IPPolygonStruct *PHead,
					     struct IPPolygonStruct **PClipped,
					     struct IPPolygonStruct **PInter,
					     IrtPlnType Plane);
struct IPVertexStruct *IritGeomFindThirdPointInTriangle(
					  const struct IPPolygonStruct *Pl,
					  const struct IPVertexStruct *V,
					  const struct IPVertexStruct *VNext);

/* Functions from the points on polygonal objects package. */
int IritGeomGetMaxNumVrtcsPoly(struct IPObjectStruct *PolyObj);
int IritGeomGetObjNumOfPolys(struct IPObjectStruct *PObj);
int IritGeomPolyHasCollinearEdges(const struct IPPolygonStruct *Pl);
int IritGeomPolysMaxSize(const struct IPPolygonStruct *Pls);
struct IPPolygonStruct *IritGeomConvertPolyToTriangles(
						  struct IPPolygonStruct *Pl);
struct IPObjectStruct *IritGeomConvertPolysToNGons(
					 const struct IPObjectStruct *PolyObj,
					 int n,
					 int HandleNormals);
struct IPObjectStruct *IritGeomConvertPolysToTriangles(
				        const struct IPObjectStruct *PolyObj);
struct IPObjectStruct *IritGeomConvertPolysToTriangles2(
					const struct IPObjectStruct *PolyObj);
struct IPObjectStruct *IritGeomConvertPolysToTrianglesIntrrPt(
					      struct IPObjectStruct *PolyObj);
struct IPObjectStruct *IritGeomConvertPolysToRectangles(
					      struct IPObjectStruct *PolyObj);
struct IPPolygonStruct *IritGeomLimitTrianglesEdgeLen(
				        const struct IPPolygonStruct *OrigPls,
					IrtRType MaxLen);
void IritGeomAffineTransUVVals(struct IPObjectStruct *PObj,
			       const IrtRType Scale[2],
			       const IrtRType Trans[2]);
void IritGeomGenUVValsForPolys(const struct IPObjectStruct *PObj,
			       const IrtRType *UVWTextureScale,
			       const IrtRType *UVWTextureTrans,
			       int HasZScaleTrans);
int IritGeomMergeSameGeometry(void **GeomEntities,
			      int NumOfGEntities,
			      IrtRType IdenticalEps,
			      VoidPtr GenericData,
			      GMMergeGeomInitFuncType InitFunc,
			      GMMergeGeomDistFuncType DistSqrFunc,
			      GMMergeGeomKeyFuncType *KeyFuncs,
			      GMMergeGeomMergeFuncType MergeFunc);
int IritGeomMergeGeometry(void **GeomEntities,
			  int NumOfGEntities,
			  IrtRType Eps,
			  IrtRType IdenticalEps,
			  VoidPtr GenericData,
			  GMMergeGeomInitFuncType InitFunc,
			  GMMergeGeomDistFuncType DistSqrFunc,
			  GMMergeGeomKeyFuncType *KeyFuncs,
			  GMMergeGeomMergeFuncType MergeFunc);
int *IritGeomMergeFindSimilarPoints(IrtRType *VecPts,
				    int	PtLen,
				    int	VecLen,
				    IrtRType Tolerance);
struct IPPolygonStruct *IritGeomMergeClosedLoopHoles(
					   struct IPPolygonStruct *PlMain,
					   struct IPPolygonStruct *PClosedPls);
struct IPPolygonStruct *IritGeomMergePolylines(struct IPPolygonStruct *Polys,
					       IrtRType	Eps);
struct IPPolygonStruct *IritGeomMatchPointListIntoPolylines(
						struct IPObjectStruct *PtsList,
						IrtRType MaxTol);
struct IPObjectStruct *IritGeomPointCoverOfPolyObj(
						struct IPObjectStruct *PolyObj,
						int n,
						IrtRType *Dir,
						char *PlAttr);
struct IPObjectStruct *IritGeomRegularizePolyModel(
					     const struct IPObjectStruct *PObj,
					     int SplitCollinear,
					     IrtRType MinRefineDist);
struct IPPolygonStruct *IritGeomSplitPolysAtCollinearVertices(
						  struct IPPolygonStruct *Pls);
struct IPPolygonStruct *IritGeomSplitPolyInPlaceAtVertex(
						 struct IPPolygonStruct *Pl,
						 struct IPVertexStruct *VHead);
struct IPPolygonStruct *IritGeomSplitPolyInPlaceAt2Vertices(
						   struct IPPolygonStruct *Pl,
						   struct IPVertexStruct *V1,
						   struct IPVertexStruct *V2);

/* Functions from the polygonal offsets package. */
IrtRType IritGeomPolyOffsetAmountDepth(const IrtRType *Coord);
struct IPPolygonStruct *IritGeomPolyOffset(
				  const struct IPPolygonStruct *Poly,
				  int IsPolygon,
				  IrtRType Ofst,
				  IritGeomPolyOffsetAmountFuncType AmountFunc);
struct IPPolygonStruct *IritGeomPolyOffset3D(
				 const struct IPPolygonStruct *Poly,
				 IrtRType Ofst,
				 int ForceSmoothing,
				 IrtRType MiterEdges,
				 IritGeomPolyOffsetAmountFuncType AmountFunc);

/* Functions from the primitive constructions' package. */
int IritGeomPrimSetGeneratePrimType(int PolygonalPrimitive);
int IritGeomPrimSetSurfacePrimitiveRational(int SurfaceRational);

struct IPObjectStruct *IritGeomPrimGenBOXObject(const IrtVecType Pt,
						IrtRType WidthX,
						IrtRType WidthY,
						IrtRType WidthZ);
struct IPObjectStruct *IritGeomPrimGenBOXWIREObject(const IrtVecType Pt,
						    IrtRType WidthX,
						    IrtRType WidthY,
						    IrtRType WidthZ);
struct IPObjectStruct *IritGeomPrimGenGBOXObject(const IrtVecType Pt,
						 const IrtVecType Dir1,
						 const IrtVecType Dir2,
						 const IrtVecType Dir3);
struct IPObjectStruct *IritGeomPrimGenCONEObject(const IrtVecType Pt,
						 const IrtVecType Dir,
						 IrtRType R,
						 int Bases);
struct IPObjectStruct *IritGeomPrimGenCONE2Object(const IrtVecType Pt,
						  const	IrtVecType Dir,
						  IrtRType R1,
						  IrtRType R2,
						  int Bases);
struct IPObjectStruct *IritGeomPrimGenCYLINObject(const IrtVecType Pt,
						  const	IrtVecType Dir,
						  IrtRType R,
						  int Bases);
struct IPObjectStruct *IritGeomPrimGenSPHEREObject(const IrtVecType Center,
						   IrtRType R);
struct IPObjectStruct *IritGeomPrimGenTORUSObject(const IrtVecType Center,
						  const	IrtVecType Normal,
						  IrtRType Rmajor,
						  IrtRType Rminor);
struct IPObjectStruct *IritGeomPrimGenPOLYDISKObject(const IrtVecType Nrml,
						     const IrtVecType Trns, 
						     IrtRType R);
struct IPObjectStruct *IritGeomPrimGenPOLYGONObject(
					      struct IPObjectStruct *PObjList,
					      int IsPolyline);
struct IPObjectStruct *IritGeomPrimGenObjectFromPolyList(
					      struct IPObjectStruct *PObjList);
struct IPObjectStruct *IritGeomPrimGenCROSSECObject(
					   const struct IPObjectStruct *PObj);
struct IPObjectStruct *IritGeomPrimGenSURFREVObject(
					   const struct IPObjectStruct *Cross);
struct IPObjectStruct *IritGeomPrimGenSURFREVAxisObject(
						 struct IPObjectStruct *Cross,
						 const IrtVecType Axis);
struct IPObjectStruct *IritGeomPrimGenSURFREV2Object(
					    const struct IPObjectStruct *Cross,
					    IrtRType StartAngle,
					    IrtRType EndAngle);
struct IPObjectStruct *IritGeomPrimGenSURFREV2AxisObject(
						 struct IPObjectStruct *Cross,
						 IrtRType StartAngle,
						 IrtRType EndAngle,
						 const IrtVecType Axis);
struct IPObjectStruct *IritGeomPrimGenEXTRUDEObject(
					   const struct IPObjectStruct *Cross,
					   const IrtVecType Dir,
					   int	Bases);
struct IPObjectStruct *IritGeomPrimGenRULEDObject(
					  const struct IPObjectStruct *Cross1,
					  const	struct IPObjectStruct *Cross2);

struct IPPolygonStruct *IritGeomPrimGenPolygon4Vrtx(
					       const IrtVecType V1,
					       const IrtVecType V2,
					       const IrtVecType V3,
					       const IrtVecType V4,
					       const IrtVecType Vin,
					       int *VrtcsRvrsd,
					       struct IPPolygonStruct *Pnext);
struct IPPolygonStruct *IritGeomPrimGenPolygon4Vrtx2(
					       const IrtVecType V1,
					       const IrtVecType V2,
					       const IrtVecType V3,
					       const IrtVecType V4,
					       const IrtVecType Vin,
					       int *VrtcsRvrsd,
					       int *Singular,
					       struct IPPolygonStruct *Pnext);
struct IPPolygonStruct *IritGeomPrimGenPolygon3Vrtx(
					       const IrtVecType V1,
					       const IrtVecType V2,
					       const IrtVecType V3,
					       const IrtVecType Vin,
					       int *VrtcsRvrsd,
					       struct IPPolygonStruct *Pnext);

struct IPObjectStruct *IritGeomPrimGenTransformController2D(
						   const GMBBBboxStruct *BBox,
						   int	HasRotation,
						   int	HasTranslation,
						   int	HasScale);
struct IPObjectStruct *IritGeomPrimGenTransformController2DCrvs(
						   const GMBBBboxStruct *BBox);
struct IPObjectStruct *IritGeomPrimGenTransformControllerSphere(
						   const GMBBBboxStruct *BBox, 
						   int HasRotation,
						   int HasTranslation,
						   int HasUniformScale,
						   IrtRType BoxOpacity,
						   IrtRType RelTesalate);
struct IPObjectStruct *IritGeomPrimGenTransformControllerBox(
						   const GMBBBboxStruct *BBox, 
						   int HasRotation,
						   int HasTranslation,
						   int HasUniformScale,
						   IrtRType BoxOpacity,
						   IrtRType RelTesalate);
struct IPObjectStruct *IritGeomPrimGenFrameController(IrtRType BBoxLen, 
						      IrtRType NLeverLen, 
						      IrtRType TLeverLen, 
						      const char *HandleName);
struct IPPolygonStruct *IritGeomPrimGenPolyline4Vrtx(
						const IrtVecType V1,
						const IrtVecType V2,
						const IrtVecType V3,
						const IrtVecType V4,
						struct IPPolygonStruct *Pnext);

int IritGeomPrimSetResolution(int Resolution);

/* Functions from the quaternions package. */
void IritGeomQuatToMat(GMQuatType q, IrtHmgnMatType Mat);
void IritGeomQuatMatToQuat(IrtHmgnMatType Mat, GMQuatType q);
void IritGeomQuatRotationToQuat(IrtRType Xangle,
				IrtRType Yangle, 
				IrtRType Zangle,
				GMQuatType q);
void IritGeomQuatToRotation(GMQuatType q,
			    IrtVecType *Angles,
			    int *NumSolutions);
void IritGeomQuatMul(GMQuatType q1, GMQuatType q2, GMQuatType QRes);
void IritGeomQuatAdd(GMQuatType q1, GMQuatType q2, GMQuatType QRes);
int IritGeomQuatIsUnitQuat(GMQuatType q);
void IritGeomQuatNormalize(GMQuatType q);
void IritGeomQuatInverse(GMQuatType SrcQ, GMQuatType DstQ);
void IritGeomQuatRotateVec(IrtVecType OrigVec,
			   GMQuatType RotQ,
			   IrtVecType DestVec);
void IritGeomQuatLog(GMQuatType SrcQ, IrtVecType DstVec);
void IritGeomQuatExp(IrtVecType SrcVec, GMQuatType DstQ);
void IritGeomQuatPow(GMQuatType MantisQ, IrtRType Expon, GMQuatType DstQ);
int IritGeomQuatMatrixToAngles(IrtHmgnMatType Mat, IrtVecType *Vec);
void IritGeomQuatMatrixToTranslation(IrtHmgnMatType Mat, IrtVecType Vec);
IrtRType IritGeomQuatMatrixToScale(IrtHmgnMatType Mat);
int IritGeomQuatMatrixToVector(IrtHmgnMatType Mat,
			       GMQuatTransVecType TransVec);
void IritGeomQuatVectorToMatrix(GMQuatTransVecType TransVec,
				IrtHmgnMatType Mat);
void IritGeomQuatVecToScaleMatrix(GMQuatTransVecType TransVec,
				  IrtHmgnMatType ScaleMatrix);
void IritGeomQuatVecToRotMatrix(GMQuatTransVecType TransVec,
				IrtHmgnMatType RotMatrix);
void IritGeomQuatVecToTransMatrix(GMQuatTransVecType TransVec,
				  IrtHmgnMatType TransMatrix);
void IritGeomMatrixToTransform(IrtHmgnMatType Mat, 
			       IrtVecType S,
			       GMQuatType R,
			       IrtVecType T);

/* Functions from the spherical coverage package. */
struct IPObjectStruct *IritGeomPointCoverOfUnitHemiSphere(
						      IrtRType HoneyCombSize);

/* Functions from the software z buffer. */
VoidPtr IritGeomZBufferInit(int Width, int Height);
void IritGeomZBufferFree(VoidPtr ZbufferID);
void IritGeomZBufferClear(VoidPtr ZbufferID);
void IritGeomZBufferClearSet(VoidPtr ZbufferID, IrtRType Depth);
GMZTestsType IritGeomZBufferSetZTest(VoidPtr ZbufferID, GMZTestsType ZTest);
GMZBufferUpdateFuncType IritGeomZBufferSetUpdateFunc(VoidPtr ZbufferID,
						     GMZBufferUpdateFuncType
						     UpdateFunc);
VoidPtr IritGeomZBufferInvert(VoidPtr ZbufferID);
VoidPtr IritGeomZBufferRoberts(VoidPtr ZbufferID);
VoidPtr IritGeomZBufferLaplacian(VoidPtr ZbufferID);
IrtRType IritGeomZBufferQueryZ(VoidPtr ZbufferID, int x, int y);
VoidPtr IritGeomZBufferQueryInfo(VoidPtr ZbufferID, int x, int y);
IrtRType IritGeomZBufferUpdatePt(VoidPtr ZbufferID, int x, int y, IrtRType z);
VoidPtr IritGeomZBufferUpdateInfo(VoidPtr ZbufferID,
				  int x,
				  int y,
				  VoidPtr Info);
void IritGeomZBufferUpdateHLn(VoidPtr ZbufferID,
			      int x1,
			      int x2,
			      int y,
			      IrtRType z1,
			      IrtRType z2);
void IritGeomZBufferUpdateLine(VoidPtr ZbufferID,
			       int x1,
			       int y1,
			       int x2,
			       int y2,
			       IrtRType	z1,
			       IrtRType	z2);
void IritGeomZBufferUpdateTri(VoidPtr ZbufferID,
			      int x1,
			      int y1,
			      IrtRType z1,
			      int x2,
			      int y2,
			      IrtRType z2,
			      int x3,
			      int y3,
			      IrtRType z3);

/* Functions from the z buffer based on Open GL package. */
IritIntPtrSizeType IritGeomZBufferOGLInit(int Width,
					  int Height,
					  IrtRType ZMin,
					  IrtRType ZMax,
					  int OffScreen);
void IritGeomZBufferOGLClear(void);
void IritGeomZBufferOGLSetColor(int Red, int Green, int Blue);
void IritGeomZBufferOGLMakeActive(IritIntPtrSizeType Id);
IrtRType IritGeomZBufferOGLQueryZ(IrtRType x, IrtRType y);
void IritGeomZBufferOGLQueryColor(IrtRType x,
				  IrtRType y,
				  int *Red,
				  int *Green,
				  int *Blue);
void IritGeomZBufferOGLFlush(void);

/* Functions to fit analytic functions to point data sets. */
IrtPtType *IritGeomSrfBilinearFit(IrtPtType *ParamDomainPts,
				  IrtPtType *EuclideanPts,
				  int FirstAtOrigin,
				  int NumPts,
				  IrtPtType *FitPts);
IrtPtType *IritGeomSrfQuadricFit(IrtPtType *ParamDomainPts,
				 IrtPtType *EuclideanPts,
				 int FirstAtOrigin,
				 int NumPts,
				 IrtPtType *FitPts);
IrtPtType *IritGeomSrfQuadricQuadOnly(IrtPtType *ParamDomainPts,
				      IrtPtType	*EuclideanPts,
				      int FirstAtOrigin,
				      int NumEucDim,
				      int NumPts,
				      IrtPtType	*QuadData);
IrtPtType *IritGeomSrfCubicQuadOnly(IrtPtType *ParamDomainPts,
				    IrtPtType *EuclideanPts,
				    int	FirstAtOrigin,
				    int	NumEucDim,
				    int	NumPts,
				    IrtPtType *CubicData);

/* Metamorphosis of polygonal objects. */
struct IPPolygonStruct *IritGeomPolygonalMorphosis(
					     const struct IPPolygonStruct *Pl1,
					     const struct IPPolygonStruct *Pl2,
					     IrtRType t);

/* Scan conversion of polygons. */
void IritGeomScanConvertTriangle(int Pt1[2],
				 int Pt2[2],
				 int Pt3[2],
				 GMScanConvertApplyFuncType ApplyFunc);

/* Text and string data sets. */
int IritGeomLoadTextFont(const char *FName);
struct IPObjectStruct *IritGeomMakeTextGeometry(const char *Str,
						const IrtVecType Spacing,
						const IrtRType *Scaling);

/* Curvature analysis over polygonal meshes. */
void IritGeomPlCrvtrSetCurvatureAttr(struct IPPolygonStruct *PolyList,
				     int NumOfRings,
				     int EstimateNrmls);
int IritGeomPlCrvtrSetFitDegree(int UseCubic);

/* Importance analysis over polygonal meshes. */
void IritGeomPlSilImportanceAttr(struct IPPolygonStruct *PolyList);
struct IPPolygonStruct *IritGeomPlSilImportanceRange(struct IPPolygonStruct
						     *PolyList);


/* Extraction of properties from polygonal meshes. */
struct IPPolygonStruct *IritGeomPolyPropFetchAttribute(
						  struct IPPolygonStruct *Pls,
						  IPAttrIDType AttrID,
						  IrtRType Value);
struct IPPolygonStruct *IritGeomPolyPropFetchIsophotes(
						  struct IPPolygonStruct *Pls,
						  const IrtVecType ViewDir,
						  IrtRType InclinationAngle);
struct IPPolygonStruct *IritGeomPolyPropFetchCurvature(
						  struct IPPolygonStruct *Pls,
						  int CurvatureProperty,
						  int NumOfRings,
						  IrtRType CrvtrVal);
struct IPPolygonStruct *IritGeomPolyPropFetch(
				 struct IPPolygonStruct *Pls,
				 GMFetchVertexPropertyFuncType VertexProperty,
				 IrtRType ConstVal,
				 void *AuxData);
struct IPPolygonStruct *IritGeomGenPolyline2Vrtx(IrtVecType V1,
						 IrtVecType V2,
						 struct	IPPolygonStruct	*Pnext);

/* Function for primitive fitting to point clouds. */
IrtRType IritGeomFitData(IrtRType **PointData,
			 unsigned int NumberOfPointsToFit,
			 GMFittingModelType FittingModel,
			 IrtRType ModelParams[],
			 IrtRType Tolerance);
IrtRType IritGeomFitDataWithOutliers(IrtRType **PointData,
				     unsigned int NumberOfPointsToFit,
				     GMFittingModelType	FittingModel,
				     IrtRType ModelParams[],
				     IrtRType Tolerance,
				     unsigned int NumOfChecks);
IrtRType IritGeomFitObjectWithOutliers(struct IPPolygonStruct *PPoly,
				       GMFittingModelType FittingModel,
				       IrtRType	ModelExtParams[],
				       IrtRType	Tolerance,
				       unsigned	int NumOfChecks);     
IrtRType IritGeomFitEstimateRotationAxis(IrtPtType *PointsOnObject,
					 IrtVecType *Normals,
					 unsigned int NumberOfPoints, 
					 IrtPtType PointOnRotationAxis,
					 IrtVecType RotationAxisDirection);

/* Functions to construct an adjacency data structure for polygonal meshes. */
VoidPtr IritGeomPolyAdjacncyGen(struct IPObjectStruct *PObj, IrtRType EqlEps);
void IritGeomPolyAdjacncyVertex(
			    struct IPVertexStruct *V,
			    VoidPtr PolyAdj,
			    IritGeomPolyAdjacncyVertexFuncType AdjVertexFunc);
void IritGeomPolyAdjacncyFree(VoidPtr PolyAdj);
int IritGeomIdentifyTJunctions(struct IPObjectStruct *PolyObj,
			 GMIdentifyTJunctionFuncType TJuncCB,
			 IrtRType Eps);
int IritGeomRefineDeformedTriangle(
			      struct IPPolygonStruct *Pl,
			      GMPointDeformVrtxFctrFuncType DeformVrtxFctrFunc,
			      GMPointDeformVrtxDirFuncType DeformVrtxDirFunc,
			      void *UserData,
			      IrtRType DeviationTol,
			      IrtRType MaxEdgeLen);
int IritGeomRefineDeformedTriangle2(
			      struct IPPolygonStruct *Pl,
			      GMPointDeformVrtxFctrFuncType DeformVrtxFctrFunc,
			      void *UserData,
			      IrtBType Ref12,
			      IrtBType Ref23,
			      IrtBType Ref31);

/* Functions to smooth poly data. */
struct IPObjectStruct *IritGeomPolyMeshSmoothing(
				 struct	IPObjectStruct *PolyObj,
				 const struct IPPolygonStruct *VerticesToRound,
				 int AllowBndryMove,
				 IrtRType RoundingRadius,
				 int NumIters,
				 IrtRType BlendFactor,
				 int CurvatureLimits);
void IritGeomFindUnConvexPolygonNormal(const struct IPVertexStruct *VL,
				       IrtVecType Nrml);
int IritGeomFindPtInsidePoly(const struct IPPolygonStruct *Pl,
			     IrtPtType PtInside);
int IritGeomFindPtInsidePolyKernel(const struct IPVertexStruct *VE,
				   IrtPtType KrnlPt);
int IritGeomIsVertexBoundary(int Index,
			     const struct IPPolyVrtxArrayStruct *PVIdx);
int IritGeomIsInterLinePolygon2D(const struct IPVertexStruct *VS, 
				 const IrtPtType V1, 
				 const IrtPtType V2, 
				 IrtRType *t);
int IritGeomAllIntersLinePolygon2D(const struct IPVertexStruct *VS, 
				   const IrtPtType V1, 
				   const IrtPtType V2, 
				   IrtRType *AllPrms);
int IritGeomComputeAverageVertex(const struct IPVertexStruct *VS, 
				 IrtPtType CenterPoint,	
				 IrtRType BlendFactor);
int IritGeomComputeAverageVertex2(const int *NS, 
				  const	struct IPPolyVrtxArrayStruct *PVIdx,
				  IrtPtType CenterPoint, 
				  int CenterIndex,
				  IrtRType BlendFactor,
				  IrtRType DesiredRadius);

  /* Subdivision surfaces functions. */
struct IPObjectStruct *IritGeomSubCatmullClark(const struct IPObjectStruct
					       *OriginalObj);
struct IPObjectStruct *IritGeomSubLoop(
				    const struct IPObjectStruct *OriginalObj);
struct IPObjectStruct *IritGeomSubButterfly(
				    const struct IPObjectStruct *OriginalObj, 
				    IrtRType ButterflyWCoef);

/* Error handling. */
IritGeomSetErrorFuncType IritGeomSetFatalErrorFunc(IritGeomSetErrorFuncType
						   ErrorFunc);
void IritGeomFatalError(IritGeomFatalErrorType ErrID);
const char *IritGeomDescribeError(IritGeomFatalErrorType ErrID);


/* Polygon mesh handling functions using BVH. */
struct IritGeomPolyBVHStruct *IritGeomPolyBVHCreate(const struct IPPolygonStruct *Pl);
void IritGeomPolyBVHFree(struct IritGeomPolyBVHStruct *PolyBVH);

int IritGeomPolyBVHGetClosestPoint(
				 const IrtPtType query, 
				 const struct IritGeomPolyBVHStruct *PolyBVH,
				 IrtPtType ClosestPt,
				 int *ClosestPlIdx);
int IritGeomPolyBVHGetClosestPoint2(
				  const IrtPtType QueryPt,
				  const struct IritGeomPolyBVHStruct *PolyBVH,
				  IrtPtType	ClosestPt,
				  int *ClosestPlIdx);
int IritGeomPolyBVHGetClosestPointNrml(
				  const IrtPtType QueryPt,
				  const struct IritGeomPolyBVHStruct *PolyBVH,
				  IrtPtType ClosestPt,
				  IrtNrmlType ClosestNrml);
int IritGeomPolyBVHGetRayBVHIntersectionPt(
				   const IrtPtType RayPt, 
				   const IrtVecType RayDir, 
				   const struct IritGeomPolyBVHStruct *PolyBVH,
				   IrtPtType *ResPt,
				   const struct IPPolygonStruct **ResPl);
int IritGeomPolyBVHPointInsidePolys(
				  const IrtPtType Pt,
				  const struct IritGeomPolyBVHStruct *PolyBVH);
int IritGeomPolyBVHSrfsPolyInter(const struct IPObjectStruct *PSrfs,
				 const struct IritGeomPolyBVHStruct *PolyBVH);
int IritGeomPolyBVHCrvsPolyInter(const struct IPObjectStruct *PCrvs,
				 const struct IritGeomPolyBVHStruct *PolyBVH);
int IritGeomPolyBVHBoxPolyInter(const GMBBBboxStruct *BBox,
				const	struct IritGeomPolyBVHStruct *PolyBVH);
int IritGeomPolyBVHPolysInter(const struct IritGeomPolyBVHStruct *PolyBVH,
			      const struct IPPolygonStruct *Polys);
int IritGeomPolyBVHGetClosestPointNrml(
				   const IrtPtType QueryPt,
				   const struct IritGeomPolyBVHStruct *PolyBVH,
				   IrtPtType ClosestPt,
				   IrtNrmlType ClosestNrml);

/* BSP tree API, for 3D points. */
GMBspPtsSliceInfoStruct *GMBspPtsAllocSliceStruct();
void IritGeomBspPtsFreeSliceStruct(GMBspPtsSliceInfoStruct *Slice);
void IritGeomBspPtsFreeSliceStructList(GMBspPtsSliceInfoStruct *SliceList);
GMBspPtsNodeStruct *GMBspPtsCreateTree(struct IPVertexStruct *PC, 
				       GMBspPtsSliceInfoStruct *Slices,	
				       int StartIdx, 
				       int EndIdx);
void IritGeomBspPtsFreeTree(GMBspPtsNodeStruct *Head);
struct IPVertexStruct *GMBspPtsGetVertices(GMBspPtsNodeStruct *Head, 
					   GMBspPtsSliceInfoStruct *LeftSlc, 
					   GMBspPtsSliceInfoStruct *RightSlc);
void IritGeomBspPtsAddSlices(GMBspPtsNodeStruct *Head, 
			     GMBspPtsSliceInfoStruct *AddedSlices);

/* Process geometry and unite all detected textures into one large image. */
struct IPObjectStruct *IritGeomUniteTextures(const struct IPObjectStruct *PObj,
					     const char	*UnitedTextureFName);
int IritGeomReparamTexture(struct IPPolygonStruct *Pls,
			   const IrtRType UVCrntDmn[4],
			   const char *UVNewDmnStr);
int IritGeomReparamTexture2(struct IPPolygonStruct *Pls,
			    const IrtRType UVCrntDmn[4],
			    const IrtRType UVNewDmn[4]);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif	/* IRIT_GEOM_LIB_H */
