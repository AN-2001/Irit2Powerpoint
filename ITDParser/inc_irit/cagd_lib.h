/*****************************************************************************
* Cagd_lib.h - header file for the CAGD	library.			     *
* This header is also the interface header to the world.		     *
******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute	of Technology		     *
******************************************************************************
* Written by Gershon Elber, Mar. 90.					     *
*****************************************************************************/

#ifndef IRIT_CAGD_LIB_H
#define IRIT_CAGD_LIB_H

#include <stdio.h>

#include "inc_irit/irit_sm.h"
#include "inc_irit/miscattr.h"
#include "inc_irit/misc_lib.h"
#include "inc_irit/geom_lib.h"

typedef int CagdBType;
typedef IrtRType CagdRType;
typedef IrtMinMaxType CagdMinMaxType;
typedef IrtUVType CagdUVType;
typedef IrtPtType CagdPType;
typedef IrtVecType CagdVType;
typedef IrtHmgnMatType CagdMType;
typedef IrtLnType CagdLType;
typedef GMBBBboxStruct CagdBBoxStruct;

typedef enum {
    CAGD_ERR_180_ARC = 1000,
    CAGD_ERR_AFD_NO_SUPPORT,
    CAGD_ERR_ALLOC_ERR,
    CAGD_ERR_BSPLINE_NO_SUPPORT,
    CAGD_ERR_BZR_CRV_EXPECT,
    CAGD_ERR_BZR_SRF_EXPECT,
    CAGD_ERR_BSP_CRV_EXPECT,
    CAGD_ERR_BSP_SRF_EXPECT,
    CAGD_ERR_CRV_FAIL_CMPT,
    CAGD_ERR_CRVS_INCOMPATIBLE,
    CAGD_ERR_CUBIC_EXPECTED,
    CAGD_ERR_DEGEN_ALPHA,
    CAGD_ERR_DIR_NOT_CONST_UV,
    CAGD_ERR_DIR_NOT_VALID,
    CAGD_ERR_INDEX_NOT_IN_MESH,
    CAGD_ERR_KNOT_NOT_ORDERED,
    CAGD_ERR_LIN_NO_SUPPORT,
    CAGD_ERR_BELOW_LIN_NO_SUPPORT,
    CAGD_ERR_NO_CROSS_PROD,
    CAGD_ERR_NOT_ENOUGH_MEM,
    CAGD_ERR_NOT_IMPLEMENTED,
    CAGD_ERR_NUM_KNOT_MISMATCH,
    CAGD_ERR_OUT_OF_RANGE,
    CAGD_ERR_PARSER_STACK_OV,
    CAGD_ERR_POWER_NO_SUPPORT,
    CAGD_ERR_PT_OR_LEN_MISMATCH,
    CAGD_ERR_POLYNOMIAL_EXPECTED,
    CAGD_ERR_RATIONAL_EXPECTED,
    CAGD_ERR_SCALAR_EXPECTED,
    CAGD_ERR_SRF_FAIL_CMPT,
    CAGD_ERR_SRFS_INCOMPATIBLE,
    CAGD_ERR_UNDEF_CRV,
    CAGD_ERR_UNDEF_SRF,
    CAGD_ERR_UNDEF_GEOM,
    CAGD_ERR_UNSUPPORT_PT,
    CAGD_ERR_T_NOT_IN_CRV,
    CAGD_ERR_U_NOT_IN_SRF,
    CAGD_ERR_V_NOT_IN_SRF,
    CAGD_ERR_WRONG_DOMAIN,
    CAGD_ERR_W_NOT_SAME,
    CAGD_ERR_W_ZERO,
    CAGD_ERR_WRONG_CRV,
    CAGD_ERR_WRONG_INDEX,
    CAGD_ERR_WRONG_ORDER,
    CAGD_ERR_WRONG_SRF,
    CAGD_ERR_WRONG_PT_TYPE,
    CAGD_ERR_CANNOT_COMP_VEC_FIELD,
    CAGD_ERR_CANNOT_COMP_NORMAL,
    CAGD_ERR_REPARAM_NOT_MONOTONE,
    CAGD_ERR_RATIONAL_NO_SUPPORT,
    CAGD_ERR_NO_SOLUTION,
    CAGD_ERR_TOO_COMPLEX,
    CAGD_ERR_REF_LESS_ORIG,
    CAGD_ERR_ONLY_2D_OR_3D,
    CAGD_ERR_ONLY_1D_TO_3D,
    CAGD_ERR_ONLY_2D,
    CAGD_ERR_DOMAIN_TOO_SMALL,
    CAGD_ERR_PERIODIC_EXPECTED,
    CAGD_ERR_PERIODIC_NO_SUPPORT,
    CAGD_ERR_OPEN_EC_EXPECTED,
    CAGD_ERR_POLYGON_EXPECTED,
    CAGD_ERR_POLYSTRIP_EXPECTED,
    CAGD_ERR_SWEEP_AXIS_TOO_COMPLEX,
    CAGD_ERR_INVALID_CONIC_COEF,
    CAGD_ERR_HYPERBOLA_NO_SUPPORT,
    CAGD_ERR_WRONG_DERIV_ORDER,
    CAGD_ERR_NO_TOL_TEST_FUNC,
    CAGD_ERR_NO_KV_FOUND,
    CAGD_ERR_WRONG_SIZE,
    CAGD_ERR_INVALID_KV,
    CAGD_ERR_INVALID_CRV,
    CAGD_ERR_INVALID_SRF,
    CAGD_ERR_C0_KV_DETECTED,
    CAGD_ERR_NO_EXTRA_KNOTS,
    CAGD_ERR_INTER_SAME_GEOM,

    CAGD_ERR_UNDEFINE_ERR
} IritCagdFatalErrorType;

/* The following type should match MvarPointType for the shared domain.      */
typedef enum {		/* Type of control point. The P-types are rationals. */
    CAGD_PT_NONE = 0,
    CAGD_PT_BASE = 1100,			  /* Must be an even number. */
    CAGD_PT_E1_TYPE = 1100,
    CAGD_PT_P1_TYPE,
    CAGD_PT_E2_TYPE,
    CAGD_PT_P2_TYPE,
    CAGD_PT_E3_TYPE,
    CAGD_PT_P3_TYPE,
    CAGD_PT_E4_TYPE,
    CAGD_PT_P4_TYPE,
    CAGD_PT_E5_TYPE,
    CAGD_PT_P5_TYPE,
    CAGD_PT_E6_TYPE,
    CAGD_PT_P6_TYPE,
    CAGD_PT_E7_TYPE,
    CAGD_PT_P7_TYPE,
    CAGD_PT_E8_TYPE,
    CAGD_PT_P8_TYPE,
    CAGD_PT_E9_TYPE,
    CAGD_PT_P9_TYPE,
    CAGD_PT_E10_TYPE,
    CAGD_PT_P10_TYPE,
    CAGD_PT_E11_TYPE,
    CAGD_PT_P11_TYPE,
    CAGD_PT_E12_TYPE,
    CAGD_PT_P12_TYPE,
    CAGD_PT_E13_TYPE,
    CAGD_PT_P13_TYPE,
    CAGD_PT_E14_TYPE,
    CAGD_PT_P14_TYPE,
    CAGD_PT_E15_TYPE,
    CAGD_PT_P15_TYPE,
    CAGD_PT_E16_TYPE,
    CAGD_PT_P16_TYPE,
    CAGD_PT_E17_TYPE,
    CAGD_PT_P17_TYPE,
    CAGD_PT_E18_TYPE,
    CAGD_PT_P18_TYPE,
    CAGD_PT_MAX_SIZE_TYPE	     /* See also CAGD_MAX_* constants below. */
} CagdPointType;

#define CAGD_MAX_PT_SIZE		19    /* Rational P18 has 19 coords. */
#define CAGD_MAX_PT_COORD		18		       /* Without w. */
#define CAGD_MAX_E_POINT		CAGD_PT_E18_TYPE
#define CAGD_MAX_P_POINT		CAGD_PT_P18_TYPE

typedef enum {
    CAGD_POLY_APPROX_ERR_CENTER = 1,
    CAGD_POLY_APPROX_ERR_SAMPLES_MAX,
    CAGD_POLY_APPROX_ERR_SAMPLES_AVG
} CagdPolyErrEstimateType;

typedef enum {
    CAGD_END_COND_GENERAL = 0,
    CAGD_END_COND_OPEN = 1,
    CAGD_END_COND_FLOAT,
    CAGD_END_COND_PERIODIC
} CagdEndConditionType;

typedef enum {
    CAGD_PRIM_CAPS_NONE = 0,
    CAGD_PRIM_CAPS_BOTTOM = 1,
    CAGD_PRIM_CAPS_TOP = 2,
    CAGD_PRIM_CAPS_BOTH = 3
} CagdPrimCapsType;

typedef enum {
    CAGD_CONST_X_SYMMETRY = 1,
    CAGD_CONST_Y_SYMMETRY = 2,
    CAGD_CONST_Z_SYMMETRY = 4,
    CAGD_CONST_C_SYMMETRY = 32,
    CAGD_CONST_X_AREA = 64,
    CAGD_CONST_Y_AREA = 128
} CagdConstraintType;

typedef enum {
    CAGD_GEOM_CONST,
    CAGD_GEOM_LINEAR,
    CAGD_GEOM_CIRCULAR,
    CAGD_GEOM_CONIC_SEC,
    CAGD_GEOM_PLANAR,
    CAGD_GEOM_SPHERICAL,
    CAGD_GEOM_TORUS,
    CAGD_GEOM_CYLINDRICAL,
    CAGD_GEOM_CONICAL,
    CAGD_GEOM_SRF_OF_REV,
    CAGD_GEOM_BILINEAR,
    CAGD_GEOM_BOOL_SUM,
    CAGD_GEOM_EXTRUSION,
    CAGD_GEOM_RULED_SRF,
    CAGD_GEOM_DEVELOP_SRF,
    CAGD_GEOM_SWEEP_SRF
} CagdIsGeometryType;

typedef enum {
    CAGD_CORNER_NONE = 0,
    CAGD_CORNER_REGULAR,
    CAGD_CORNER_ROUND,
    CAGD_CORNER_CHAMFER,
    CAGD_CORNER_MITER
} CagdCrvCornerType;

typedef enum {
    CAGD_BSP_BASIS_FUNC_EVAL_MULT_VALUE,
    CAGD_BSP_BASIS_FUNC_EVAL_MULT_DER1ST,
    CAGD_BSP_BASIS_FUNC_EVAL_MULT_DER2ND,
    CAGD_BSP_BASIS_FUNC_EVAL_MULT_DER3RD
} CagdIritCagdBspBasisFuncMultEvalType;

typedef enum {
    CAGD_CRV_INC_CNSTRCT_INIT,
    CAGD_CRV_INC_CNSTRCT_XY,
    CAGD_CRV_INC_CNSTRCT_X,
    CAGD_CRV_INC_CNSTRCT_Y,
    CAGD_CRV_INC_CNSTRCT_DELTA_XY,
    CAGD_CRV_INC_CNSTRCT_DELTA_X,
    CAGD_CRV_INC_CNSTRCT_DELTA_Y,
    CAGD_CRV_INC_CNSTRCT_DELTA_X_AND_Y,
    CAGD_CRV_INC_CNSTRCT_DELTA_Y_AND_X,
    CAGD_CRV_INC_CNSTRCT_DIST_ALPHA,
    CAGD_CRV_INC_CNSTRCT_DIST_DELTA_ALPHA,
    CAGD_CRV_INC_CNSTRCT_DELTA_X_DELTA_ALPHA,
    CAGD_CRV_INC_CNSTRCT_DELTA_Y_DELTA_ALPHA,
    CAGD_CRV_INC_CNSTRCT_ALPHA_LINE,
    CAGD_CRV_INC_CNSTRCT_ARC,
    CAGD_CRV_INC_CNSTRCT_ADD_INTERMEDIATE,
    CAGD_CRV_INC_CNSTRCT_ROUND_LAST,
    CAGD_CRV_INC_CNSTRCT_CHAMFER_LAST,
    CAGD_CRV_INC_CNSTRCT_CURVE_PARAMS,
    CAGD_CRV_INC_CNSTRCT_CLOSE,

    CAGD_CRV_INC_CNSTRCT_BUILD,                          /* Used internally. */
    CAGD_CRV_INC_CNSTRCT_DEBUG                           /* Used internally. */
} IritCagdCrvIncCnstrctType;

#define CAGD_MALLOC_STRUCT_ONCE     /* Faster allocation of CAGD structures. */

#define CAGD_IS_RATIONAL_PT(PType)  ((int) ((PType) & 0x01))
#define CAGD_NUM_OF_PT_COORD(PType) ((((int) ((PType) - CAGD_PT_BASE)) >> 1) + 1)
#define CAGD_MAKE_PT_TYPE(IsRational, NumCoords) \
				    ((CagdPointType) (CAGD_PT_BASE + \
				         ((((IsRational) ? -1 : -2) \
						       + ((NumCoords) << 1)))))

#define CAGD_IS_RATIONAL_CRV(Crv)	CAGD_IS_RATIONAL_PT((Crv) -> PType)
#define CAGD_IS_RATIONAL_SRF(Srf)	CAGD_IS_RATIONAL_PT((Srf) -> PType)

/* Bezier curves may be evaluated using a predefined cache. The cache must   */
/* be of size (FINENESS) which is power of 2 up to the maximum order below.  */
/* See IritCagdBzrCrvSetCache routine below.				     */
#ifdef CAGD_LARGE_BEZIER_CACHE
#define CAGD_MAX_BEZIER_CACHE_ORDER	99    /* See cbzr_tbl.c in cagd_lib. */
#else
#define CAGD_MAX_BEZIER_CACHE_ORDER	28    /* See cbzr_tbl.c in cagd_lib. */
#endif /* CAGD_LARGE_BEZIER_CACHE */

#define CAGD_MAX_BEZIER_CACHE_FINENESS	1024

#define CAGD_MAX_BEZIER_CACHE_ORDER2	15    /* See cbzr2tbl.c in cagd_lib. */

/* If a curve or a surface is periodic, their control polygon/mesh should    */
/* warp up. Length does hold the real allocated length but the virtual       */
/* periodic length is a little larger. Note allocated KV's are larger.       */
#define CAGD_CRV_PT_LST_LEN(Crv) ((Crv) -> Length + \
				  ((Crv) -> Periodic ? (Crv) -> Order - 1 : 0))
#define CAGD_SRF_UPT_LST_LEN(Srf)	((Srf) -> ULength + \
					 ((Srf) -> UPeriodic ? \
					      (Srf) -> UOrder - 1 : 0))
#define CAGD_SRF_VPT_LST_LEN(Srf)	((Srf) -> VLength + \
					 ((Srf) -> VPeriodic ? \
					      (Srf) -> VOrder - 1 : 0))
#define CAGD_IS_PERIODIC_CRV(Crv)	((Crv) -> Periodic)
#define CAGD_IS_UPERIODIC_SRF(Srf)	((Srf) -> UPeriodic)
#define CAGD_IS_VPERIODIC_SRF(Srf)	((Srf) -> VPeriodic)
#define CAGD_IS_PERIODIC_SRF(Srf)	(CAGD_IS_UPERIODIC_SRF(Srf) || \
					 CAGD_IS_VPERIODIC_SRF(Srf))

#define CAGD_BBOX_RESET(BBox)     GM_BBOX_RESET(BBox)

#define CAGD_BBOX_INIT_3D(BBox) { \
    CAGD_BBOX_RESET(BBox) \
    BBox.Min[0] = BBox.Min[1] = BBox.Min[2] = IRIT_INFNTY; \
    BBox.Max[0] = BBox.Max[1] = BBox.Max[2] = -IRIT_INFNTY; }

#define CAGD_DEL_GEOM_TYPE(Obj) \
         IritMiscAttrIDFreeOneAttribute(&(Obj) -> Attr, IRIT_ATTR_CREATE_ID(GeomType))
#define CAGD_SET_GEOM_TYPE(Obj, Geom) \
       IritMiscAttrIDSetIntAttrib(&(Obj) -> Attr, IRIT_ATTR_CREATE_ID(GeomType), Geom)
#define CAGD_PROPAGATE_ATTR(NewObj, Obj) { \
		  IP_ATTR_FREE_ATTRS((NewObj) -> Attr); \
                  if ((Obj) -> Attr != NULL) { \
		      (NewObj) -> Attr = IritMiscAttrCopyAttributes((Obj) -> Attr); } }

#define CAGD_EPS_ROUND_KNOT		1e-12
#define CAGD_MESH_CONT_ANGULAR_EPS	0.99999
#define CAGD_MESH_CONT_LENRATIO_EPS	IRIT_EPS

#define CAGD_MAX_DOMAIN_EPS	(IRIT_UEPS * 10)
#define CAGD_VALIDATE_MIN_MAX_DOMAIN(t, TMin, TMax) { \
    if (t >= TMax - CAGD_MAX_DOMAIN_EPS) { \
	t = TMax - CAGD_MAX_DOMAIN_EPS; \
	if (t >= TMax) { \
	    t = TMax - CAGD_MAX_DOMAIN_EPS * TMax; \
	    assert(t < TMax); \
	} \
    } \
    if (t < TMin) \
	t = TMin; }

typedef enum {
    CAGD_UNDEF_TYPE = 1200,
    CAGD_CBEZIER_TYPE,
    CAGD_CBSPLINE_TYPE,
    CAGD_CPOWER_TYPE,
    CAGD_SBEZIER_TYPE,
    CAGD_SBSPLINE_TYPE,
    CAGD_SPOWER_TYPE
} CagdGeomType;

typedef enum {
    CAGD_NO_DIR = 1300,
    CAGD_CONST_U_DIR,
    CAGD_CONST_V_DIR,
    CAGD_BOTH_DIR
} CagdSrfDirType;

typedef enum {
    CAGD_UV_BNDRY_MASK = 0x600,                       /* 8 lower bits are 0. */
    CAGD_NO_BNDRY = CAGD_UV_BNDRY_MASK,
    CAGD_U_MIN_BNDRY = CAGD_UV_BNDRY_MASK + 1,
    CAGD_U_MAX_BNDRY = CAGD_UV_BNDRY_MASK + 2,
    CAGD_V_MIN_BNDRY = CAGD_UV_BNDRY_MASK + 4,
    CAGD_V_MAX_BNDRY = CAGD_UV_BNDRY_MASK + 8,
    CAGD_U_MINMAX_BNDRY = CAGD_U_MIN_BNDRY | CAGD_U_MAX_BNDRY,
    CAGD_V_MINMAX_BNDRY = CAGD_V_MIN_BNDRY | CAGD_V_MAX_BNDRY
} CagdSrfBndryType;

#define CAGD_SRF_BNDRY_FILTER_MASK(B) ((B) & ~CAGD_UV_BNDRY_MASK)

#define CAGD_OTHER_DIR(Dir) ((Dir) == CAGD_CONST_U_DIR ? CAGD_CONST_V_DIR \
						       : CAGD_CONST_U_DIR)

typedef enum {
    CAGD_REG_POLY_PER_LIN = 1400,
    CAGD_ONE_POLY_PER_LIN,
    CAGD_ONE_POLY_PER_COLIN
} CagdLin2PolyType;

typedef enum {
    CAGD_GENERAL_PARAM = 1500,
    CAGD_UNIFORM_PARAM,
    CAGD_CENTRIPETAL_PARAM,
    CAGD_CHORD_LEN_PARAM,
    CAGD_NIELSON_FOLEY_PARAM,
    CAGD_KV_NODAL_PARAM,
} CagdParametrizationType;

typedef enum {
    CAGD_POLYGON_TYPE_TRIANGLE,
    CAGD_POLYGON_TYPE_RECTANGLE,
    CAGD_POLYGON_TYPE_POLYSTRIP
} CagdPolygonType;

typedef enum {
    CAGD_PDM_FITTING,
    CAGD_SDM_FITTING
} CagdBspFittingType;

typedef struct CagdGenericStruct {
    struct CagdGenericStruct *Pnext;
    struct IPAttributeStruct *Attr;
} CagdGenericStruct;

typedef struct CagdUVStruct {
    struct CagdUVStruct *Pnext;
    struct IPAttributeStruct *Attr;
    CagdUVType UV;
} CagdUVStruct;

typedef struct CagdPtStruct {
    struct CagdPtStruct *Pnext;
    struct IPAttributeStruct *Attr;
    CagdRType Pt[CAGD_MAX_PT_SIZE];
} CagdPtStruct;

typedef struct CagdSrfPtStruct {
    struct CagdSrfPtStruct *Pnext;
    struct IPAttributeStruct *Attr;
    CagdUVType Uv;
    CagdPType Pt;
    CagdVType Nrml;
} CagdSrfPtStruct;

typedef struct CagdSrfAdapRectStruct {
    struct CagdSrfAdapRectStruct *Pnext;
    int UIndexBase;
    int UIndexSize;
    int VIndexBase;
    int VIndexSize;
    VoidPtr AuxSrfData;
    CagdRType Err;
} CagdSrfAdapRectStruct;

typedef struct CagdCtlPtStruct {
    struct CagdCtlPtStruct *Pnext;
    struct IPAttributeStruct *Attr;
    CagdRType Coords[CAGD_MAX_PT_SIZE];
    CagdPointType PtType;
    int align8;
} CagdCtlPtStruct;

typedef struct CagdSrfAdjInfoStruct {
    /* Pointers to the surfaces that neighbor (the boundary curves) of a    */
    /* surface.  If no neighboring surface, slot will be set to NULL.       */
    /* The pointers are stored in the order: UMin, UMax, VMin, VMax.        */
    struct CagdSrfStruct *AdjSrfs[4];
    /* Indicator which boundary curve in the neighbor surfaces is similar   */
    /* our corresponding boundary curve.		                    */
    /* These values are interpreted according to this template:             */
    /* CagdSrfBndryType + 0x600 +                                           */
    /*        (The Curves are the same but flipped) * 0x1000, otherwise 0.  */
    /* Stored in  order: UMin, UMax, VMin, VMax.			    */
    int Flag[4];
    /* Unique Id of the Srf, It's determined according to its order in the  */
    /* surfaces list.							    */
    int Id;
} CagdSrfAdjInfoStruct;

typedef struct CagdVecStruct {
    struct CagdVecStruct *Pnext;
    struct IPAttributeStruct *Attr;
    CagdRType Vec[CAGD_MAX_PT_SIZE];
} CagdVecStruct;

typedef struct CagdPlaneStruct {
    struct CagdPlaneStruct *Pnext;
    struct IPAttributeStruct *Attr;
    CagdRType Plane[4];
} CagdPlaneStruct;

typedef struct CagdCrvStruct {
    struct CagdCrvStruct *Pnext;
    struct IPAttributeStruct *Attr;
    CagdGeomType GType;
    CagdPointType PType;
    int Length;           /* Number of control points (== order in Bezier). */
    int Order;	  /* Order of curve (only for B-spline, ignored in Bezier). */
    CagdBType Periodic;			 /* Valid only for B-spline curves. */
    CagdRType *Points[CAGD_MAX_PT_SIZE];    /* Pointer on each axis vector. */
    CagdRType *KnotVector;
} CagdCrvStruct;

typedef struct CagdSrfStruct {
    struct CagdSrfStruct *Pnext;
    struct IPAttributeStruct *Attr;
    CagdGeomType GType;
    CagdPointType PType;
    int ULength, VLength;	/* Mesh size in the tensor product surface. */
    int UOrder, VOrder; /* Order in tensor product surface (B-spline only). */
    CagdBType UPeriodic, VPeriodic;    /* Valid only for B-spline surfaces. */
    CagdRType *Points[CAGD_MAX_PT_SIZE];    /* Pointer on each axis vector. */
    CagdRType *UKnotVector, *VKnotVector;
} CagdSrfStruct;

typedef struct CagdPolygonStruct {
    struct CagdPolygonStruct *Pnext;
    struct IPAttributeStruct *Attr;
    CagdPolygonType PolyType;
    union {
	struct {
	    CagdPType Pt;	/* Polygon is either triangle or rectangle. */
	    CagdVType Nrml;
	    CagdUVType UV;
	} Polygon[4];
	struct {
	    /* Polygonal strip can have arbitrary # of polygons. */
	    CagdPType FirstPt[2];	     /* Base line - the first edge. */
	    CagdVType FirstNrml[2];
	    CagdUVType FirstUV[2];
	    CagdPType *StripPt;		      /* Arrays of size NumOfPolys. */
	    CagdVType *StripNrml;
	    CagdUVType *StripUV;
	    int NumOfPolys;
	} PolyStrip;
    } U;
} CagdPolygonStruct;

#define CAGD_MAX_POLYLINE_PT_SIZE CAGD_MAX_PT_SIZE
typedef struct CagdPolylnStruct {
    CagdRType Pt[CAGD_MAX_POLYLINE_PT_SIZE];
} CagdPolylnStruct;

typedef struct CagdPolylineStruct {
    struct CagdPolylineStruct *Pnext;
    struct IPAttributeStruct *Attr;
    CagdPolylnStruct *Polyline; /* Polyline length is defined using Length. */
    int Length, PtDim;
} CagdPolylineStruct;

typedef struct BspKnotAlphaCoeffStruct {
    int Order, Length, RefLength, Periodic;  /* Dimensions of alpha matrix. */
    CagdRType *Matrix;
    CagdRType *MatrixTransp;
    CagdRType **Rows;		    /* A column of pointers to Matrix rows. */
    CagdRType **RowsTransp;	    /* A row of pointers to Matrix columns. */
    int *ColIndex;	/* A row of indices of first non zero value in col. */
    int *ColLength;	     /* A row of lengths of non zero values in col. */
    CagdRType *_CacheKVT;	   /* To compare input/output kvs in cache. */
    CagdRType *_CacheKVt;
} BspKnotAlphaCoeffStruct;

typedef struct CagdBlsmAlphaCoeffStruct {
    int Order, Length,
	NewOrder, NewLength, Periodic;/* Dimensions of blossom alpha matrix.*/
    CagdRType *Matrix;
    CagdRType **Rows;		    /* A column of pointers to Matrix rows. */
    int *ColIndex;	/* A row of indices of first non zero value in col. */
    int *ColLength;	     /* A row of lengths of non zero values in col. */
    CagdRType *KV;	    /* Knot sequences before and after the blossom. */
    CagdRType *NewKV;
} CagdBlsmAlphaCoeffStruct;

typedef struct CagdBspBasisFuncEvalStruct {
    int FirstBasisFuncIndex;
    CagdRType *BasisFuncsVals;
} CagdBspBasisFuncEvalStruct;

typedef struct IritCagdCrvIncCnstrctInputStruct {
    IritCagdCrvIncCnstrctType Op;
    CagdRType Params[4];
} IritCagdCrvIncCnstrctInputStruct;

/* Partition of a curve to quads: */
typedef enum {
    CAGD_CRV_QUAD_TILE_LIMIT_CRVS,
    CAGD_CRV_QUAD_TILE_SRF
} CagdCrvQuadTileRepresentationType;

typedef struct CagdCrvQuadTileStruct {
    struct CagdCrvQuadTileStruct *Pnext;
    struct IPAttributeStruct *Attr;
    union {
	CagdCrvStruct *LimitCrvs;
	CagdSrfStruct *Srf;
    } U;
    CagdCrvQuadTileRepresentationType ActiveRepresentation;
} CagdCrvQuadTileStruct;

typedef const struct CagdSrf2PlsInfoStrct *CagdSrf2PlsInfoStrctConstPtr;

typedef void (*CagdSetErrorFuncType)(IritCagdFatalErrorType ErrorFunc);
typedef void (*CagdPrintfFuncType)(char *Line);
typedef int (*CagdCompFuncType)(const VoidPtr P1, const VoidPtr P2);
typedef CagdCrvStruct *(*CagdCrvFuncType)(CagdCrvStruct *Crv, CagdRType R);
typedef CagdRType (*CagdMatchNormFuncType)(const CagdVType T1,
					   const CagdVType T2,
					   const CagdVType P1,
					   const CagdVType P2);
typedef CagdRType (*IritCagdSrfErrorFuncType)(const CagdSrfStruct *Srf,
					      CagdRType Tol,
					      void *AuxData);
typedef int (*IritCagdSrfAdapAuxDataFuncType)(const CagdSrfStruct *Srf,
					      VoidPtr AuxSrfData,
					      CagdRType t,
					      CagdSrfDirType Dir,
					      CagdSrfStruct *Srf1,
					      VoidPtr *AuxSrf1Data,
					      CagdSrfStruct *Srf2,
					      VoidPtr *AuxSrf2Data);
typedef int (*CagdCrvAdjCmpFuncType)(const CagdCrvStruct *Crv1,
				     const CagdCrvStruct *Crv2,
				     CagdRType Eps,
				     CagdBType *Reversed);
typedef struct IPPolygonStruct *(*CagdSrfMakeTriFuncType)(
				   CagdSrf2PlsInfoStrctConstPtr TessInfo,
				   const CagdRType *Pt1,
				   const CagdRType *Pt2,
				   const CagdRType *Pt3,
				   const CagdRType *Nl1,
				   const CagdRType *Nl2,
				   const CagdRType *Nl3,
				   const CagdRType *UV1,
				   const CagdRType *UV2,
				   const CagdRType *UV3,
				   CagdBType *GenPoly);
typedef struct IPPolygonStruct *(*CagdSrfMakeRectFuncType)(
				   CagdSrf2PlsInfoStrctConstPtr TessInfo,
				   const CagdRType *Pt1,
				   const CagdRType *Pt2,
				   const CagdRType *Pt3,
				   const CagdRType *Pt4,
				   const CagdRType *Nl1,
				   const CagdRType *Nl2,
				   const CagdRType *Nl3,
				   const CagdRType *Nl4,
				   const CagdRType *UV1,
				   const CagdRType *UV2,
				   const CagdRType *UV3,
				   const CagdRType *UV4,
				   CagdBType *GenPoly);
typedef CagdRType (*CagdPlgErrorFuncType)(const CagdPType P1,
					  const CagdPType P2,
					  const CagdPType P3);

typedef CagdBType (*CagdCrvTestingFuncType)(const CagdCrvStruct *Crv,
					    CagdRType *t);
typedef CagdBType(*CagdSrfTestingFuncType)(const CagdSrfStruct *Srf,
					   CagdSrfDirType Dir,
					   CagdRType *t);

typedef struct IPPolygonStruct *(*IritCagdSrfAdapPolyGenFuncType)(
				 const CagdSrfStruct *Srf,
				 CagdSrfPtStruct *SrfPtList,
				 const CagdSrfAdapRectStruct *Rect,
				 const struct CagdSrf2PlsInfoStrct *TessInfo);

typedef struct CagdSrf2PlsInfoStrct { 
    int FourPerFlat;   /*  If TRUE, four triangle per flat surface patch are */
    /* created, otherwise two. Only for uniform, non optimal, sampling. */
    IrtRType FineNess; /* Fineness control on polygonal approximation.       */
    /* If !Optimal, uniform sampling in the order of FineNess Sampling  */
    /* is used.  If Optimal, FineNess prescribes the maximal distance   */
    /* between the surface and its polygonal approximation.	     */
    int ComputeUV; /* TRUE for UV parameter values in vertices of triangles. */
    int ComputeNrml;	       /* TRUE for normals in vertices of triangles. */
    int Optimal; /* If FALSE, then parametric space of Srf is sampled        */
    /* uniformly, order of FineNess samples per direction.		     */
    /* If TRUE, the adaptively created polygonal approximation is        */
    /* guaranteed to be within FineNess distance to the surface.	     */

    /* The data/functions below is optional and can be NULL.		     */
    void *EvalNrmlCache; /* Optional cache for efficient normal evaluations. */

    CagdSrfMakeRectFuncType MakeRectFunc; /* Generator func. for rectangles. */
    CagdSrfMakeTriFuncType MakeTriFunc;    /* Generator func. for triangles. */
    IritCagdSrfAdapPolyGenFuncType AdapPolyGenFunc; /* Gen. func. for polys. */
    IritCagdSrfAdapAuxDataFuncType AdapAuxDataFunc;/*Aux optnl data in tess. */
} CagdSrf2PlsInfoStrct;

#define CAGD_IS_BEZIER_CRV(Crv)		((Crv) -> GType == CAGD_CBEZIER_TYPE)
#define CAGD_IS_BEZIER_SRF(Srf)		((Srf) -> GType == CAGD_SBEZIER_TYPE)
#define CAGD_IS_BSPLINE_CRV(Crv)	((Crv) -> GType == CAGD_CBSPLINE_TYPE)
#define CAGD_IS_BSPLINE_SRF(Srf)	((Srf) -> GType == CAGD_SBSPLINE_TYPE)
#define CAGD_IS_POWER_CRV(Crv)		((Crv) -> GType == CAGD_CPOWER_TYPE)
#define CAGD_IS_POWER_SRF(Srf)		((Srf) -> GType == CAGD_SPOWER_TYPE)

/******************************************************************************
*		U -->			    The	mesh is	ordered	row after row*
*	+-----------------------+	or the increments along	U are 1	while*
*   V |	|P0		    Pi-1|	the increment along V is full row.   *
*     v	|Pi		   P2i-1|	    To encapsulate it, NEXTU/V are   *
*	|			|	defined	below.			     *
*	|Pn-i		    Pn-1|					     *
*	+-----------------------+					     *
******************************************************************************/
#define CAGD_NEXT_U(Srf)	(1)
#define CAGD_NEXT_V(Srf)	((Srf) -> ULength)
#define CAGD_MESH_UV(Srf, i, j)	((i) + ((Srf) -> ULength) * (j))

#define CAGD_GEN_COPY(Dst, Src, Size) memcpy((char *) (Dst), (char *) (Src), \
					     (Size))

#define CAGD_GEN_COPY_STEP(Dst, Src, Size, DstStep, SrcStep, Type) \
			{ \
			    int _ii; \
			    Type *_DstType = (Type *) (Dst), \
				 *_SrcType = (Type *) (Src); \
 \
			    for (_ii = 0; _ii < Size; _ii++) { \
				*_DstType = *_SrcType; \
				_DstType += DstStep; \
				_SrcType += SrcStep; \
			    } \
			}

/******************************************************************************
* Some points/vectors simplifying operators.				     *
******************************************************************************/
#define	CAGD_COPY_POINT(DstPt, SrcPt)	{ (DstPt) = (SrcPt); }
#define	CAGD_ADD_POINT(DstPt, SrcPt)    { (DstPt).Pt[0] += (SrcPt).Pt[0]; \
					  (DstPt).Pt[1] += (SrcPt).Pt[1]; \
					  (DstPt).Pt[2] += (SrcPt).Pt[2]; }
#define	CAGD_SUB_POINT(DstPt, SrcPt)    { (DstPt).Pt[0] -= (SrcPt).Pt[0]; \
					  (DstPt).Pt[1] -= (SrcPt).Pt[1]; \
					  (DstPt).Pt[2] -= (SrcPt).Pt[2]; }
#define	CAGD_MULT_POINT(DstPt, Scaler)  { (DstPt).Pt[0] *= (Scaler); \
					  (DstPt).Pt[1] *= (Scaler); \
					  (DstPt).Pt[2] *= (Scaler); }

#define	CAGD_COPY_VECTOR(DstVec, SrcVec) { (DstVec) = (SrcVec); }
#define	CAGD_ADD_VECTOR(DstVec, SrcVec) { (DstVec).Vec[0] += (SrcVec).Vec[0]; \
					  (DstVec).Vec[1] += (SrcVec).Vec[1]; \
					  (DstVec).Vec[2] += (SrcVec).Vec[2]; }
#define	CAGD_SUB_VECTOR(DstVec, SrcVec) { (DstVec).Vec[0] -= (SrcVec).Vec[0]; \
					  (DstVec).Vec[1] -= (SrcVec).Vec[1]; \
					  (DstVec).Vec[2] -= (SrcVec).Vec[2]; }
#define	CAGD_MULT_VECTOR(DstVec, Scaler){ (DstVec).Vec[0] *= (Scaler); \
					  (DstVec).Vec[1] *= (Scaler); \
					  (DstVec).Vec[2] *= (Scaler); }
#define	CAGD_DIV_VECTOR(DstVec, Scaler) { (DstVec).Vec[0] /= (Scaler); \
					  (DstVec).Vec[1] /= (Scaler); \
					  (DstVec).Vec[2] /= (Scaler); }
#define CAGD_SQR_LEN_VECTOR(V)		(IRIT_SQR((V).Vec[0]) + \
					     IRIT_SQR((V).Vec[1]) + \
					     IRIT_SQR((V).Vec[2]))
#define CAGD_LEN_VECTOR(V)		sqrt(CAGD_SQR_LEN_VECTOR(V))
#define CAGD_NORMALIZE_VECTOR(V)	{ CagdRType __t = CAGD_LEN_VECTOR(V); \
					  { \
					      __t = 1.0 / __t; \
					      CAGD_MULT_VECTOR((V), __t); \
					  } \
				        }
#define CAGD_NORMALIZE_VECTOR_MSG_ZERO(V) \
					{ CagdRType __t = CAGD_LEN_VECTOR(V); \
					  _IRIT_PT_NORMALIZE_MSG_ZERO(__t) \
					  { \
					      __t = 1.0 / __t; \
					      CAGD_MULT_VECTOR((V), __t); \
					  } \
				        }


#define	CAGD_COPY_UVVAL(DstUV, SrcUV) { (DstUV) = (SrcUV); }

#define CAGD_PT_ON_BNDRY(u, v, UMin, UMax, VMin, VMax, Eps) \
	(IRIT_APX_EQ_EPS(u, UMin, Eps) || IRIT_APX_EQ_EPS(u, UMax, Eps) || \
	 IRIT_APX_EQ_EPS(v, VMin, Eps) || IRIT_APX_EQ_EPS(v, VMax, Eps))

#define CAGD_CRV_EVAL_SCALAR(Crv, t, PtE1) \
	     { CagdRType _R[CAGD_MAX_PT_SIZE]; \
	       IritCagdCrvEvalToData((Crv), (t), _R); \
	       PtE1 = CAGD_IS_RATIONAL_CRV((Crv)) ? _R[1] / _R[0] : _R[1]; }
#define CAGD_CRV_EVAL_E2(Crv, t, PtE2) \
		{ CagdRType _R[CAGD_MAX_PT_SIZE], *PR = _R; \
		  IritCagdCrvEvalToData((Crv), (t), _R); \
		  IritCagdCoerceToE2(PtE2, &PR, -1, (Crv) -> PType); }
#define CAGD_CRV_EVAL_E3(Crv, t, PtE3) \
		{ CagdRType _R[CAGD_MAX_PT_SIZE], *PR = _R; \
		  IritCagdCrvEvalToData((Crv), (t), _R); \
		  IritCagdCoerceToE3(PtE3, &PR, -1, (Crv) -> PType); }
#define CAGD_CRV_EVAL_P2(Crv, t, PtP2) \
		{ CagdRType _R[CAGD_MAX_PT_SIZE], *PR = _R; \
		  IritCagdCrvEvalToData((Crv), (t), _R); \
		  IritCagdCoerceToP2(PtP2, &PR, -1, (Crv) -> PType); }
#define CAGD_CRV_EVAL_P3(Crv, t, PtP3) \
		{ CagdRType _R[CAGD_MAX_PT_SIZE], *PR = _R; \
		  IritCagdCrvEvalToData((Crv), (t), _R); \
		  IritCagdCoerceToP3(PtP3, &PR, -1, (Crv) -> PType); }
#define CAGD_CRV_EVAL_EUC(Crv, t, PtE) /* Coerce to same dim as Crv dim. */ \
    { CagdPointType \
	  _NewPType = CAGD_MAKE_PT_TYPE(FALSE, \
		                        CAGD_NUM_OF_PT_COORD(Crv -> PType)); \
      CagdRType _PtTmp[CAGD_MAX_PT_SIZE], \
          *_R = _PtTmp; \
      IritCagdCrvEvalToData(Crv, t, _PtTmp); \
      IritCagdCoercePointTo(PtE, _NewPType, &_R, -1, Crv -> PType); }

#define CAGD_SRF_EVAL_SCALAR(Srf, u, v, PtE1) \
	     { CagdRType _R[CAGD_MAX_PT_SIZE]; \
	       IritCagdSrfEvalToData((Srf), (u), (v), _R); \
	       *PtE1 = CAGD_IS_RATIONAL_SRF((Srf)) ? _R[1] / _R[0] : _R[1]; }
#define CAGD_SRF_EVAL_E2(Srf, u, v, PtE2) \
		{ CagdRType _R[CAGD_MAX_PT_SIZE], *PR = _R; \
		  IritCagdSrfEvalToData((Srf), (u), (v), _R); \
		  IritCagdCoerceToE2(PtE2, &PR, -1, (Srf) -> PType); }
#define CAGD_SRF_EVAL_E3(Srf, u, v, PtE3) \
		{ CagdRType _R[CAGD_MAX_PT_SIZE], *PR = _R; \
		  IritCagdSrfEvalToData((Srf), (u), (v), _R); \
		  IritCagdCoerceToE3(PtE3, &PR, -1, (Srf) -> PType); }
#define CAGD_SRF_EVAL_P2(Srf, u, v, PtP2) \
		{ CagdRType _R[CAGD_MAX_PT_SIZE], *PR = _R; \
		  IritCagdSrfEvalToData((Srf), (u), (v), _R); \
		  IritCagdCoerceToP2(PtP2, &PR, -1, (Srf) -> PType); }
#define CAGD_SRF_EVAL_P3(Srf, u, v, PtP3) \
		{ CagdRType _R[CAGD_MAX_PT_SIZE], *PR = _R; \
		  IritCagdSrfEvalToData((Srf), (u), (v), _R); \
		  IritCagdCoerceToP3(PtP3, &PR, -1, (Srf) -> PType); }
#define CAGD_SRF_EVAL_EUC(Srf, u, v, PtE)/* Coerce to same dim as Srf dim.*/ \
    { CagdPointType \
	  _NewPType = CAGD_MAKE_PT_TYPE(FALSE, \
		                        CAGD_NUM_OF_PT_COORD(Srf -> PType)); \
      CagdRType _PtTmp[CAGD_MAX_PT_SIZE], \
          *_R = _PtTmp; \
      IritCagdSrfEvalToData(Srf, u, v, _PtTmp); \
      IritCagdCoercePointTo(PtE, _NewPType, &_R, -1, Srf -> PType); }

/******************************************************************************
* This macro is	called when the	library	has detected an	unrecoverable error. *
* Default action is to call IritCagdFatalError,	but you	may want to reroute  *
* this to invoke your handler and recover yourself (by long jump for example).*
******************************************************************************/
#define CAGD_FATAL_ERROR(MsgID)	IritCagdFatalError(MsgID)

/******************************************************************************
* Macros to verify the validity	of the parametric domain.		     *
******************************************************************************/
#ifdef IRIT_DOUBLE
#define CAGD_DOMAIN_IRIT_EPS 1e-13
#else
#define CAGD_DOMAIN_IRIT_EPS 1e-5
#endif /* IRIT_DOUBLE */

#define CAGD_DOMAIN_T_VERIFY(t, TMin, TMax) \
    { \
	if (t < TMin) \
	    t += CAGD_DOMAIN_IRIT_EPS; \
	if (t > TMax) \
	    t -= CAGD_DOMAIN_IRIT_EPS; \
	if (t < TMin || t > TMax) \
	    CAGD_FATAL_ERROR(CAGD_ERR_T_NOT_IN_CRV); \
    }

#define CAGD_DOMAIN_GET_AND_VERIFY_CRV(t, Crv, TMin, TMax) \
    { \
	IritCagdCrvDomain(Crv, &TMin, &TMax); \
	CAGD_DOMAIN_T_VERIFY(t, TMin, TMax); \
    }

#define CAGD_DOMAIN_GET_AND_VERIFY_SRF(u, v, Srf, UMin, UMax, VMin, VMax) \
    { \
	IritCagdSrfDomain(Srf, &UMin, &UMax, &VMin, &VMax); \
	CAGD_DOMAIN_T_VERIFY(u, UMin, UMax); \
	CAGD_DOMAIN_T_VERIFY(v, VMin, VMax); \
    }

/******************************************************************************
* A fast macro to blend	the original control polygon with the Alpha matrix.  *
******************************************************************************/

#define CAGD_ALPHA_LOOP_BLEND_NOT_PERIODIC_OLD(A, IMin, IMax, OrigPts, RefPts) \
{ \
    int _i, \
	*_ColLength = &A -> ColLength[IMin], \
	*_ColIndex = &A -> ColIndex[IMin]; \
    CagdRType \
	**_Rows = A -> Rows; \
 \
    IRIT_ZAP_MEM(RefPts, sizeof(CagdRType) * (IMax - IMin)); \
 \
    for (_i = IMin; _i < IMax; _i++) { \
	if (*_ColLength++ == 1) \
	    *RefPts++ = OrigPts[*_ColIndex++]; \
	else { \
	    int _Len = _ColLength[-1], \
	        _Idx = *_ColIndex++ + --_Len; \
	    CagdRType \
		*_Pts = &OrigPts[_Idx]; \
 \
	    for ( ; _Len-- >= 0; _Idx--) { \
		*RefPts += *_Pts-- * _Rows[_Idx][_i]; \
	    } \
 \
	    RefPts++; \
	} \
    } \
}

#define CAGD_ALPHA_LOOP_BLEND_PERIODIC_OLD(A, IMin, IMax, OrigPts, OrigLen, RefPts) \
{ \
    int _i, \
	*_ColLength = &A -> ColLength[IMin], \
	*_ColIndex = &A -> ColIndex[IMin]; \
    CagdRType \
	**_Rows = A -> Rows; \
 \
    IRIT_ZAP_MEM(RefPts, sizeof(CagdRType) * (IMax - IMin)); \
 \
    for (_i = IMin; _i < IMax; _i++) { \
	if (*_ColLength++ == 1) { \
	    int _Idx = *_ColIndex++; \
	    *RefPts++ = OrigPts[_Idx >= OrigLen ? _Idx - OrigLen \
			                        : _Idx]; \
	} \
	else { \
	    int _Len = _ColLength[-1], \
	        _Idx = *_ColIndex++ + --_Len; \
	    CagdRType \
		*_Pts = &OrigPts[_Idx]; \
 \
	    for ( ; _Len-- >= 0; _Idx--) { \
		*RefPts += *_Pts-- * _Rows[_Idx >= OrigLen ? _Idx - OrigLen \
			                                   : _Idx][_i]; \
	    } \
 \
	    RefPts++; \
	} \
    } \
}

#define CAGD_ALPHA_LOOP_BLEND_STEP_OLD(A, IMin, IMax, OrigPts, OrigPtsStep, \
				   OrigLen, RefPts, RefPtsStep) \
{ \
    int _i, \
	*_ColLength = &A -> ColLength[IMin], \
	*_ColIndex = &A -> ColIndex[IMin]; \
    CagdRType \
	**_Rows = A -> Rows; \
 \
    for (_i = IMin; _i < IMax; _i++) { \
	if (*_ColLength++ == 1) { \
	    *RefPts = OrigPts[*_ColIndex++ * OrigPtsStep]; \
	} \
	else { \
	    int _Len = _ColLength[-1], \
	        _Idx = *_ColIndex++ + --_Len; \
	    CagdRType \
		*_Pts = &OrigPts[_Idx * OrigPtsStep]; \
 \
	    for (*RefPts = 0.0; _Len-- >= 0; _Idx--, _Pts -= OrigPtsStep) { \
		*RefPts += *_Pts * _Rows[_Idx][_i]; \
	    } \
	} \
	RefPts += RefPtsStep; \
    } \
}

#define CAGD_ALPHA_LOOP_BLEND_NOT_PERIODIC(A, IMin, IMax, OrigPts, RefPts) \
{ \
    int _i, \
	*_ColLength = &A -> ColLength[IMin], \
	*_ColIndex = &A -> ColIndex[IMin]; \
    CagdRType *_p, *_r; \
  \
    IRIT_ZAP_MEM(RefPts, sizeof(CagdRType) * (IMax - IMin)); \
  \
    for (_i = IMin; _i < IMax; _i++) { \
	switch (*_ColLength++) { \
 	    case 1: \
		*RefPts++ = OrigPts[*_ColIndex++]; \
		break; \
 	    case 2: \
		_p = &OrigPts[*_ColIndex]; \
		_r = &A -> RowsTransp[_i][*_ColIndex++]; \
		*RefPts++ = _p[0] * _r[0] + \
		            _p[1] * _r[1]; \
		break; \
 	    case 3: \
		_p = &OrigPts[*_ColIndex]; \
		_r = &A -> RowsTransp[_i][*_ColIndex++]; \
		*RefPts++ = _p[0] * _r[0] + \
		            _p[1] * _r[1] + \
		            _p[2] * _r[2]; \
		break; \
 	    case 4: \
		_p = &OrigPts[*_ColIndex]; \
		_r = &A -> RowsTransp[_i][*_ColIndex++]; \
		*RefPts++ = _p[0] * _r[0] + \
		            _p[1] * _r[1] + \
		            _p[2] * _r[2] + \
		            _p[3] * _r[3]; \
		break; \
	    default: \
		{ \
		    int _Len = _ColLength[-1]; \
 \
		    _p = &OrigPts[*_ColIndex]; \
		    _r = &A -> RowsTransp[_i][*_ColIndex++]; \
 \
		    for (*RefPts = 0.0; _Len-- > 0; ) { \
			*RefPts += *_p++ * *_r++; \
		    } \
 \
		    RefPts++; \
	        } \
		break; \
	} \
    } \
}

#define CAGD_ALPHA_LOOP_BLEND_PERIODIC(A, IMin, IMax, OrigPts, OrigLen, RefPts) \
{ \
    int _i, \
	*_ColLength = &A -> ColLength[IMin], \
	*_ColIndex = &A -> ColIndex[IMin]; \
    CagdRType *_p, *_r; \
 \
    for (_i = IMin; _i < IMax; _i++) { \
        int _Len = *_ColLength++, \
	    _Idx = *_ColIndex++; \
 \
	if (_Idx + _Len <= OrigLen) { \
	    switch (_Len) { \
 	        case 1: \
		    *RefPts++ = OrigPts[_Idx]; \
		    break; \
 	        case 2: \
		    _p = &OrigPts[_Idx]; \
		    _r = &A -> RowsTransp[_i][_Idx]; \
		    *RefPts++ = _p[0] * _r[0] + \
		                _p[1] * _r[1]; \
		    break; \
 	        case 3: \
		    _p = &OrigPts[_Idx]; \
		    _r = &A -> RowsTransp[_i][_Idx]; \
		    *RefPts++ = _p[0] * _r[0] + \
		                _p[1] * _r[1] + \
		                _p[2] * _r[2]; \
		    break; \
 	        case 4: \
		    _p = &OrigPts[_Idx]; \
		    _r = &A -> RowsTransp[_i][_Idx]; \
		    *RefPts++ = _p[0] * _r[0] + \
		                _p[1] * _r[1] + \
		                _p[2] * _r[2] + \
		                _p[3] * _r[3]; \
		    break; \
	        default: \
	  	    { \
		        if (_Len == 1) { \
			    *RefPts++ = OrigPts[_Idx]; \
			} \
			else { \
			    _p = &OrigPts[_Idx]; \
			    _r = &A -> RowsTransp[_i][_Idx]; \
 \
			    for (*RefPts = 0.0 ; _Len-- > 0; ) { \
				*RefPts += *_p++ * *_r++; \
			    } \
 \
			    RefPts++; \
			} \
		    } \
	    } \
	} \
	else { \
	    if (_Len == 1) { \
		*RefPts++ = OrigPts[_Idx >= OrigLen ? _Idx - OrigLen : _Idx]; \
	    } \
	    else { \
		_p = &OrigPts[_Idx]; \
		_r = A -> RowsTransp[_i]; \
 \
		for (*RefPts = 0.0 ; _Len-- > 0; _Idx++) { \
		    *RefPts += *_p++ * _r[_Idx >= OrigLen ? _Idx - OrigLen : \
			                                    _Idx]; \
		} \
 \
		RefPts++; \
	    } \
	} \
    } \
}

#define CAGD_ALPHA_LOOP_BLEND_STEP(A, IMin, IMax, OrigPts, OrigPtsStep, \
				   OrigLen, RefPts, RefPtsStep) \
{ \
    int _i, \
	*_ColLength = &A -> ColLength[IMin], \
	*_ColIndex = &A -> ColIndex[IMin]; \
    CagdRType \
	**_Rows = A -> Rows; \
 \
    for (_i = IMin; _i < IMax; _i++) { \
	if (*_ColLength++ == 1) { \
	    *RefPts = OrigPts[*_ColIndex++ * OrigPtsStep]; \
	} \
	else { \
	    int _Len = _ColLength[-1], \
	        _Idx = *_ColIndex++ + --_Len; \
	    CagdRType \
		*_Pts = &OrigPts[_Idx * OrigPtsStep]; \
 \
	    for (*RefPts = 0.0; _Len-- >= 0; _Idx--, _Pts -= OrigPtsStep) { \
		*RefPts += *_Pts * _Rows[_Idx][_i]; \
	    } \
	} \
	RefPts += RefPtsStep; \
    } \
}

/******************************************************************************
* A fast macro to evaluate a point out of a control points' mesh/vector.     *
******************************************************************************/

#define BSP_CRV_EVAL_VEC_AT_PARAM(Res, Pts, Inc, Order, Len, \
				  t, BasisFuncs, IndexFirst) \
    { \
	int _i, \
	    _IFirst = IndexFirst; \
	CagdRType \
            *_B = BasisFuncs; \
 \
	*Res = 0.0; \
	if (Inc == 1) { \
	    for (_i = 0; _i < Order; _i++) { \
		*Res += *_B++ * Pts[_IFirst]; \
		if (++_IFirst >= Len) \
		    _IFirst -= Len; \
	    } \
        } \
	else { \
	    int _IFirstInc = _IFirst; \
 \
	    _IFirstInc *= Inc; \
	    for (_i = 0; _i < Order; _i++) { \
		*Res += *_B++ * Pts[_IFirstInc]; \
	        _IFirstInc += Inc; \
	        if (++_IFirst >= Len) { \
	            _IFirst -= Len; \
		    _IFirstInc -= Len * Inc; \
	        } \
	    } \
        } \
    }

IRIT_GLOBAL_DATA_HEADER CagdBType _IritCagdSrfMakeOnlyTri;
IRIT_GLOBAL_DATA_HEADER CagdSrfMakeTriFuncType _IritCagdSrfMakeTriFunc;
IRIT_GLOBAL_DATA_HEADER CagdSrfMakeRectFuncType _IritCagdSrfMakeRectFunc;

/******************************************************************************
* General global variable of the Cagd library:				     *
******************************************************************************/
IRIT_GLOBAL_DATA_HEADER const CagdRType
    IritCagdIChooseKTable[CAGD_MAX_BEZIER_CACHE_ORDER + 1]
	             [CAGD_MAX_BEZIER_CACHE_ORDER + 1];


IRIT_GLOBAL_DATA_HEADER const CagdRType 
    IritCagdIcKJcMIJcKMTable[CAGD_MAX_BEZIER_CACHE_ORDER2 + 1]
                            [CAGD_MAX_BEZIER_CACHE_ORDER2 + 1]
                            [CAGD_MAX_BEZIER_CACHE_ORDER2 + 1]
			    [CAGD_MAX_BEZIER_CACHE_ORDER2 + 1];

/******************************************************************************
* Routines prototypes. Routines	are prefixed as	follows:		     *
* Cagd	  - General routines such as dynamic memory handlers etc.	     *
* BzrCrv  - Bezier curves routines.					     *
* BzrSrf  - Bezier surface routines.					     *
* BspKnot - B-spline knot vector routines.				     *
* BspCrv  - B-spline curves routines.					     *
* BspSrf  - B-spline surface routines.					     *
* CagdCnvrt   -	Conversion routines such as Bezier to Power basis.	     *
******************************************************************************/

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/******************************************************************************
* General routines of the Cagd library:					     *
******************************************************************************/

CagdUVStruct *IritCagdUVNew(void);
CagdPtStruct *IritCagdPtNew(void);
CagdSrfPtStruct *IritCagdSrfPtNew(void);
CagdCtlPtStruct *IritCagdCtlPtNew(CagdPointType PtType);
CagdVecStruct *IritCagdVecNew(void);
CagdPlaneStruct *IritCagdPlaneNew(void);
CagdBBoxStruct *IritCagdBBoxNew(void);
CagdCrvStruct *IritCagdCrvNew(CagdGeomType GType,
			      CagdPointType PType,
			      int Length);
CagdCrvStruct *IritCagdPeriodicCrvNew(CagdGeomType GType,
				      CagdPointType PType,
				      int Length,
				      CagdBType	Periodic);
CagdSrfStruct *IritCagdSrfNew(CagdGeomType GType,
			      CagdPointType PType,
			      int ULength,
			      int VLength);
CagdSrfStruct *IritCagdPeriodicSrfNew(CagdGeomType GType,
				      CagdPointType PType,
				      int ULength,
				      int VLength,
				      CagdBType	UPeriodic,
				      CagdBType	VPeriodic);
CagdPolygonStruct *IritCagdPolygonNew(int Len);
CagdPolygonStruct *IritCagdPolygonStripNew(int Len);
CagdPolylineStruct *IritCagdPolylineNew(int Length, int PtDim);

CagdUVStruct *IritCagdUVArrayNew(int Size);
CagdPtStruct *IritCagdPtArrayNew(int Size);
CagdCtlPtStruct *IritCagdCtlPtArrayNew(CagdPointType PtType, int Size);
CagdVecStruct *IritCagdVecArrayNew(int Size);
CagdPlaneStruct *IritCagdPlaneArrayNew(int Size);
CagdBBoxStruct *IritCagdBBoxArrayNew(int Size);
CagdPolygonStruct *IritCagdPolygonArrayNew(int Size);
CagdPolylineStruct *IritCagdPolylineArrayNew(int Length, int Size);

CagdCrvStruct *IritCagdCrvCopy(const CagdCrvStruct *Crv);
CagdSrfStruct *IritCagdSrfCopy(const CagdSrfStruct *Srf);
CagdUVStruct *IritCagdUVCopy(const CagdUVStruct *UV);
CagdPtStruct *IritCagdPtCopy(const CagdPtStruct *Pt);
CagdSrfPtStruct *IritCagdSrfPtCopy(const CagdSrfPtStruct *Pt);
CagdCtlPtStruct *IritCagdCtlPtCopy(const CagdCtlPtStruct *CtlPt);
CagdVecStruct *IritCagdVecCopy(const CagdVecStruct *Vec);
CagdPlaneStruct *IritCagdPlaneCopy(const CagdPlaneStruct *Plane);
CagdBBoxStruct *IritCagdBBoxCopy(const CagdBBoxStruct *BBoc);
CagdPolygonStruct *IritCagdPolygonCopy(const CagdPolygonStruct *Poly);
CagdPolylineStruct *IritCagdPolylineCopy(const CagdPolylineStruct *Poly);

CagdCrvStruct *IritCagdCrvCopyList(const CagdCrvStruct *CrvList);
CagdSrfStruct *IritCagdSrfCopyList(const CagdSrfStruct *SrfList);
CagdUVStruct *IritCagdUVCopyList(const CagdUVStruct *UVList);
CagdPtStruct *IritCagdPtCopyList(const CagdPtStruct *PtList);
CagdSrfPtStruct *IritCagdSrfPtCopyList(const CagdSrfPtStruct *SrfPtList);
CagdCtlPtStruct *IritCagdCtlPtCopyList(const CagdCtlPtStruct *CtlPtList);
CagdVecStruct *IritCagdVecCopyList(const CagdVecStruct *VecList);
CagdPlaneStruct *IritCagdPlaneCopyList(const CagdPlaneStruct *PlaneList);
CagdPolylineStruct *IritCagdPolylineCopyList(const CagdPolylineStruct *PolyList);
CagdPolygonStruct *IritCagdPolygonCopyList(const CagdPolygonStruct *PolyList);

void IritCagdCrvFree(CagdCrvStruct *Crv);
void IritCagdCrvFreeList(CagdCrvStruct *CrvList);
void IritCagdSrfFree(CagdSrfStruct *Srf);
void IritCagdSrfFreeList(CagdSrfStruct *SrfList);
void IritCagdUVFree(CagdUVStruct *UV);
void IritCagdUVFreeList(CagdUVStruct *UVList);
void IritCagdUVArrayFree(CagdUVStruct *UVArray, int Size);
void IritCagdPtFree(CagdPtStruct *Pt);
void IritCagdPtFreeList(CagdPtStruct *PtList);
void IritCagdSrfPtFree(CagdSrfPtStruct *SrfPt);
void IritCagdSrfPtFreeList(CagdSrfPtStruct *SrfPtList);
void IritCagdPtArrayFree(CagdPtStruct *PtArray, int Size);
void IritCagdCtlPtFree(CagdCtlPtStruct *CtlPt);
void IritCagdCtlPtFreeList(CagdCtlPtStruct *CtlPtList);
void IritCagdCtlPtArrayFree(CagdCtlPtStruct *CtlPtArray, int Size);
void IritCagdVecFree(CagdVecStruct *Vec);
void IritCagdVecFreeList(CagdVecStruct *VecList);
void IritCagdVecArrayFree(CagdVecStruct *VecArray, int Size);
void IritCagdPlaneFree(CagdPlaneStruct *Plane);
void IritCagdPlaneFreeList(CagdPlaneStruct *PlaneList);
void IritCagdPlaneArrayFree(CagdPlaneStruct *PlaneArray, int Size);
void IritCagdBBoxFree(CagdBBoxStruct *BBox);
void IritCagdBBoxArrayFree(CagdBBoxStruct *BBoxArray, int Size);
void IritCagdPolylineFree(CagdPolylineStruct *Poly);
void IritCagdPolylineFreeList(CagdPolylineStruct *PolyList);
void IritCagdPolygonFree(CagdPolygonStruct *Poly);
void IritCagdPolygonFreeList(CagdPolygonStruct *PolyList);

#ifdef DEBUG
#define IritCagdCrvFree(Crv)         { IritCagdCrvFree(Crv); Crv = NULL; }
#define IritCagdCrvFreeList(CrvList) { IritCagdCrvFreeList(CrvList); CrvList = NULL; }
#define IritCagdSrfFree(Srf)         { IritCagdSrfFree(Srf); Srf = NULL; }
#define IritCagdSrfFreeList(SrfList) { IritCagdSrfFreeList(SrfList); SrfList = NULL; }
#define IritCagdUVFree(UV)           { IritCagdUVFree(UV); UV = NULL; }
#define IritCagdUVFreeList(UVList)   { IritCagdUVFreeList(UVList); UVList = NULL; }
#define IritCagdUVArrayFree(UVArray, \
			    Size)    { IritCagdUVArrayFree(UVArray, Size); \
			               UVArray = NULL; }
#define IritCagdPtFree(Pt)           { IritCagdPtFree(Pt); Pt = NULL; }
#define IritCagdPtFreeList(PtList)   { IritCagdPtFreeList(PtList); PtList = NULL; }
#define IritCagdSrfPtFree(SrfPt)     { IritCagdSrfPtFree(SrfPt); SrfPt = NULL; }
#define IritCagdSrfPtFreeList(SrfPtList) { IritCagdSrfPtFreeList(SrfPtList); \
			                   SrfPtList = NULL; }
#define IritCagdPtArrayFree(PtArray, \
			    Size)    { IritCagdPtArrayFree(PtArray, Size); \
				        PtArray = NULL; }
#define IritCagdCtlPtFree(CtlPt)     { IritCagdCtlPtFree(CtlPt); CtlPt = NULL; }
#define IritCagdCtlPtFreeList(CtlPtList) { IritCagdCtlPtFreeList(CtlPtList); \
			                   CtlPtList = NULL; }
#define IritCagdCtlPtArrayFree(CtlPtArray, \
			       Size) { IritCagdCtlPtArrayFree(CtlPtArray, Size); \
			               CtlPtArray = NULL; }
#define IritCagdVecFree(Vec)         { IritCagdVecFree(Vec); Vec = NULL; }
#define IritCagdVecFreeList(VecList) { IritCagdVecFreeList(VecList); VecList = NULL; }
#define IritCagdVecArrayFree(VecArray, \
			     Size)   { IritCagdVecArrayFree(VecArray, Size); \
			               VecArray = NULL; }
#define IritCagdPlaneFree(Plane)     { IritCagdPlaneFree(Plane); Plane = NULL; }
#define IritCagdPlaneFreeList(PlaneList) { IritCagdPlaneFreeList(PlaneList); \
			                   PlaneList = NULL; }
#define IritCagdPlaneArrayFree(PlaneArray, \
			       Size) { IritCagdPlaneArrayFree(PlaneArray, Size); \
			               PlaneArray = NULL; }
#define IritCagdBBoxFree(BBox)       { IritCagdBBoxFree(BBox); BBox = NULL; }
#define IritCagdBBoxArrayFree(BBoxArray, \
			      Size)  { IritCagdBBoxArrayFree(BBoxArray, Size); \
			              BBoxArray = NULL;	}
#define IritCagdPolylineFree(Poly)   { IritCagdPolylineFree(Poly); Poly = NULL; }
#define IritCagdPolylineFreeList(PolyList) { IritCagdPolylineFreeList(PolyList); \
			                     PolyList = NULL; }
#define IritCagdPolylineArrayFree(PolyArray, \
			          Size)  { \
				      IritCagdPolylineArrayFree(PolyArray, Size); \
			              PolyArray = NULL;	}
#define IritCagdPolygonFree(Poly)    { IritCagdPolygonFree(Poly); Poly = NULL; }
#define IritCagdPolygonFreeList(PolyList) { IritCagdPolygonFreeList(PolyList); \
			                    PolyList = NULL; }
#define IritCagdPolygonArrayFree(PolyArray, \
			     Size) { IritCagdPolygonArrayFree(PolyArray, Size); \
			             PolyArray = NULL; }
#endif /* DEBUG */
int IritCagdListLength(const VoidPtr List);
VoidPtr IritCagdListAppend(VoidPtr List1, VoidPtr List2);
VoidPtr IritCagdListReverse(VoidPtr List);
VoidPtr IritCagdListLast(const VoidPtr List);
VoidPtr IritCagdListPrev(const VoidPtr List, const VoidPtr Item);
IrtBType IritCagdListFind(const VoidPtr List, const VoidPtr Elem);
VoidPtr IritCagdListNth(const VoidPtr List, int n);
VoidPtr IritCagdListDelNth(VoidPtr *List, int n);
VoidPtr IritCagdListSort(VoidPtr List,
			 CagdBType Ascending,
			 CagdCompFuncType SortCmprFunc);
void IritCagdCoerceToE2(CagdRType *E2Point,
			CagdRType * const Points[CAGD_MAX_PT_SIZE],
			int Index,
			CagdPointType PType);
void IritCagdCoerceToE3(CagdRType *E3Point,
			CagdRType * const Points[CAGD_MAX_PT_SIZE],
			int Index,
			CagdPointType PType);
void IritCagdCoerceToP2(CagdRType *P2Point,
			CagdRType * const Points[CAGD_MAX_PT_SIZE],
			int Index,
			CagdPointType PType);
void IritCagdCoerceToP3(CagdRType *P3Point,
			CagdRType * const Points[CAGD_MAX_PT_SIZE],
			int Index,
			CagdPointType PType);
void IritCagdCoercePointTo(CagdRType *NewPoint,
			   CagdPointType NewPType,
			   CagdRType * const *Points,
			   int Index,
			   CagdPointType OldPType);
void IritCagdCoercePointsTo(CagdRType *Points[],
			    int	Len,
			    CagdPointType OldPType,
			    CagdPointType NewPType);
VoidPtr IritCagdStructOnceCoercePointsTo(CagdRType * const OldPoints[],
					 const VoidPtr OldStruct,
					 int OldStructLen,
					 int ExtraMem,
					 int PtsLen,
					 CagdPointType OldPType,
					 CagdPointType NewPType);
CagdRType IritCagdDistTwoCtlPt(CagdRType * const *Pt1,
			       int Index1,
			       CagdRType * const *Pt2,
			       int Index2,
			       CagdPointType PType);
CagdRType IritCagdDistTwoCtlPt2(CagdRType * const *Pts,
				int Index1,
				int Index2,
				CagdPointType PType);
CagdBType IritCagdRealVecSame(CagdRType const *Vec1,
			      CagdRType const *Vec2,
			      int Len,
			      CagdRType Eps);
CagdBType IritCagdCtlMeshsSame(CagdRType * const Mesh1[],
			       CagdRType * const Mesh2[],
			       int Len,
			       CagdRType Eps);
CagdBType IritCagdCtlMeshsSameUptoRigidScl2D(CagdRType * const Mesh1[],
					     CagdRType * const Mesh2[],
					     int Len,
					     IrtPtType Trans,
					     CagdRType *Rot,
					     CagdRType *Scl,
					     CagdRType Eps);
CagdCrvStruct *IritCagdCoerceCrvsTo(const CagdCrvStruct *Crv,
				    CagdPointType PType,
				    CagdBType AddParametrization);
CagdCrvStruct *IritCagdCoerceCrvTo(const CagdCrvStruct *Crv,
				   CagdPointType PType,
				   CagdBType AddParametrization);
CagdSrfStruct *IritCagdCoerceSrfsTo(const CagdSrfStruct *Srf,
				    CagdPointType PType,
				    CagdBType AddParametrization);
CagdSrfStruct *IritCagdCoerceSrfTo(const CagdSrfStruct *Srf,
				   CagdPointType PType,
				   CagdBType AddParametrization);
CagdPointType IritCagdMergePointTypes(CagdPointType PType1,
				      CagdPointType PType2);
void IritCagdDbg(const void *Obj);
void IritCagdDbg1(const void *Obj);
#ifdef DEBUG
void IritCagdDbgDsp(const void *Obj);
#endif /* DEBUG */
void IritCagdSetLinear2Poly(CagdLin2PolyType Lin2Poly);
CagdBType IritCagdIgnoreNonPosWeightBBox(CagdBType IgnoreNonPosWeightBBox);
void IritCagdCtlPtBBox(const CagdCtlPtStruct *CtlPt, CagdBBoxStruct *BBox);
CagdBType IritCagdCtlPtListBBox(const CagdCtlPtStruct *CtlPts,
				CagdBBoxStruct *BBox);
void IritCagdPointsBBox2(CagdRType * const *Points,
			 int Min,
			 int Max,
			 int Dim,
			 CagdRType *BBoxMin,
			 CagdRType *BBoxMax);
void IritCagdPointsBBox(CagdRType * const *Points,
			int Length,
			int Dim,
			CagdRType *BBoxMin,
			CagdRType *BBoxMax);
CagdRType IritCagdIChooseK(int i, int k);
CagdRType IritCagdIcKJcMIJcKM(int i, int j, int k, int m);
void IritCagdTransform(CagdRType **Points,
		       int Len,
		       int MaxCoord,
		       CagdBType IsNotRational,
		       const CagdRType *Translate,
		       CagdRType Scale);
void IritCagdTransform2(CagdRType **Points,
			int Len,
			int MaxCoord,
			CagdBType IsNotRational,
			const CagdRType	*Translate,
			CagdRType Scale);
void IritCagdScale(CagdRType **Points,
		   int Len,
		   int MaxCoord,
		   const CagdRType *Scale);
void IritCagdScaleCenter(CagdRType **Points,
			 int Len,
			 int MaxCoord,
			 const CagdRType *Scale);
void IritCagdMatTransCenter(CagdRType **Points,
			    int	Len,
			    int	MaxCoord,
			    int	IsNotRational,
			    IrtHmgnMatType Mat);
void IritCagdMatTransform(CagdRType **Points,
			  int Len,
			  int MaxCoord,
			  CagdBType IsNotRational,
			  CagdMType Mat);
void IritCagdMatTransform2(CagdRType **NewPoints,
			   const CagdRType **OldPoints,
			   int Len,
			   int MaxCoord,
			   CagdBType IsNotRational,
			   CagdMType Mat);
CagdBType IritCagdPointsHasPoles(CagdRType * const *Points,
				 int Len,
				 int *Sign);
CagdBType IritCagdAllWeightsNegative(CagdRType * const *Points,
				     CagdPointType PType,
				     int Len,
				     CagdBType Flip);
CagdBType IritCagdScaleWeights(CagdRType * const *Points,
			       CagdPointType PType,
			       int Len,
			       CagdRType WScale);
CagdBType IritCagdAllWeightsSame(CagdRType * const *Points, int Len);
CagdPlgErrorFuncType IritCagdPolygonSetErrFunc(CagdPlgErrorFuncType Func);
CagdSrfMakeTriFuncType IritCagdSrfSetMakeTriFunc(CagdSrfMakeTriFuncType Func);
CagdSrfMakeRectFuncType IritCagdSrfSetMakeRectFunc(
						CagdSrfMakeRectFuncType Func);
CagdBType IritCagdSrfSetMakeOnlyTri(CagdBType OnlyTri);
CagdPolylineStruct *IritCagdPtPolyline2EkPolyline(
				  CagdRType * const Polyline[CAGD_MAX_PT_SIZE],
				  int n,
				  int MaxCoord,
				  CagdBType IsRational);
CagdPtStruct *IritCagdPtsSortAxis(CagdPtStruct *PtList, int Axis);
CagdBType IritCagdCrvOnOneSideOfLine(const CagdCrvStruct *Crv,
				     CagdRType X1,
				     CagdRType Y1,
				     CagdRType X2,
				     CagdRType Y2);

void IritCagdPolygonBBox(const CagdPolygonStruct *Poly, CagdBBoxStruct *BBox);
void IritCagdPolygonListBBox(const CagdPolygonStruct *Polys,
			     CagdBBoxStruct *BBox);

/*****************************************************************************
* Blossoming								     *
*****************************************************************************/

CagdBlsmAlphaCoeffStruct *IritCagdBlsmAllocAlphaCoef(int Order,
						     int Length,
						     int NewOrder,
						     int NewLength,
						     CagdBType Periodic);
CagdBlsmAlphaCoeffStruct *IritCagdBlsmCopyAlphaCoef(
					   const CagdBlsmAlphaCoeffStruct *A);
void IritCagdBlsmFreeAlphaCoef(CagdBlsmAlphaCoeffStruct *A);
void IritCagdBlsmAddRowAlphaCoef(CagdBlsmAlphaCoeffStruct *A,
				 CagdRType *Coefs,
				 int ARow,
				 int ColIndex,
				 int ColLength);
void IritCagdBlsmSetDomainAlphaCoef(CagdBlsmAlphaCoeffStruct *A);
void IritCagdBlsmScaleAlphaCoef(CagdBlsmAlphaCoeffStruct *A, CagdRType Scl);

CagdRType *IritCagdBlsmEvalSymb(int Order,
				const CagdRType	*Knots,
				int KnotsLen,
				const CagdRType	*BlsmVals,
				int BlsmLen,
				int *RetIdxFirst,
				int *RetLength,
				CagdRType *RetVec,
				void *Cache);
void *IritCagdBlsmEvalSymbAllocCache();
void IritCagdBlsmEvalSymbFreeCache(void *Cache);
CagdRType IritCagdBlossomEval(const CagdRType *Pts,
			      int PtsStep,
			      int Order,
			      const CagdRType *Knots,
			      int KnotsLen,
			      const CagdRType *BlsmVals,
			      int BlsmLen);
CagdRType *IritCagdCrvBlossomEvalToData(const CagdCrvStruct *Crv,
					const CagdRType	*BlsmVals,
					int BlsmLen,
					CagdRType *BlossomVals,
					void *BlsmCache);
CagdRType *IritCagdCrvBlossomEvalMalloc(const CagdCrvStruct *Crv,
					const CagdRType	*BlsmVals,
					int BlsmLen);
CagdRType *IritCagdSrfBlossomEvalToData(const CagdSrfStruct *Srf,
					const CagdRType	*BlsmUVals,
					int BlsmULen,
					const CagdRType	*BlsmVVals,
					int BlsmVLen,
					CagdRType *BlossomVals,
					void *BlsmCache);
CagdRType *IritCagdSrfBlossomEvalMalloc(const CagdSrfStruct *Srf,
					const CagdRType	*BlsmUVals,
					int BlsmULen,
					const CagdRType	*BlsmVVals,
					int BlsmVLen);
CagdCrvStruct *IritCagdSrfBlossomEvalU(const CagdSrfStruct *Srf,
				       const CagdRType *BlsmUVals,
				       int BlsmULen,
				       void *BlsmCache);

CagdCrvStruct *IritCagdCrvBlossomDegreeRaiseN(const CagdCrvStruct *Crv,
					      int NewOrder);
CagdCrvStruct *IritCagdCrvBlossomDegreeRaise(const CagdCrvStruct *Crv);
CagdSrfStruct *IritCagdSrfBlossomDegreeRaiseN(const CagdSrfStruct *Srf,
					      int NewUOrder,
					      int NewVOrder);
CagdSrfStruct *IritCagdSrfBlossomDegreeRaise(const CagdSrfStruct *Srf,
					     CagdSrfDirType Dir);

CagdBlsmAlphaCoeffStruct *IritCagdDegreeRaiseMatProd(
						 CagdBlsmAlphaCoeffStruct *A1,
						 CagdBlsmAlphaCoeffStruct *A2);
CagdBlsmAlphaCoeffStruct *IritCagdBlossomDegreeRaiseMat(const CagdRType *KV,
							int Order,
							int Len);
CagdBlsmAlphaCoeffStruct *IritCagdBlossomDegreeRaiseNMat(const CagdRType *KV,
							 int Order,
							 int NewOrder,
							 int Len);

/*****************************************************************************
* Matrix/Vector/Plane/Transformation routines:				     *
*****************************************************************************/
CagdRType IritCagdFitPlaneThruCtlPts(CagdPlaneStruct *Plane,
				     CagdPointType PType,
				     CagdRType * const *Points,
				     int Index1,
				     int Index2,
				     int Index3,
				     int Index4);
CagdRType IritCagdDistPtPlane(const CagdPlaneStruct *Plane,
			      CagdRType	* const	*Points,
			      int Index,
			      int MaxDim);
CagdRType IritCagdDistPtLine(const CagdVecStruct *LineDir,
			     CagdRType * const *Points,
			     int Index,
			     int MaxDim);
CagdCrvStruct *IritCagdCrvMatTransform(const CagdCrvStruct *Crv,
				       CagdMType Mat);
CagdCrvStruct *IritCagdCrvListMatTransform(const CagdCrvStruct *Crvs,
					   CagdMType Mat);
CagdSrfStruct *IritCagdSrfMatTransform(const CagdSrfStruct *Srf,
				       CagdMType Mat);
CagdSrfStruct *IritCagdSrfListMatTransform(const CagdSrfStruct *Srfs,
					   CagdMType Mat);
void IritCagdCrvScale(CagdCrvStruct *Crv, const CagdRType *Scale);
void IritCagdCrvScaleCenter(CagdCrvStruct *Crv, const CagdRType *Scale);
void IritCagdCrvMatTransCenter(CagdCrvStruct *Crv, IrtHmgnMatType Mat);
void IritCagdCrvTransform(CagdCrvStruct *Crv,
			  const	CagdRType *Translate,
			  CagdRType Scale);
void IritCagdSrfScale(CagdSrfStruct *Srf, const CagdRType *Scale);
void IritCagdSrfScaleCenter(CagdSrfStruct *Srf, const CagdRType *Scale);
void IritCagdSrfMatTransCenter(CagdSrfStruct *Srf, IrtHmgnMatType Mat);
void IritCagdSrfTransform(CagdSrfStruct *Srf,
			  const	CagdRType *Translate,
			  CagdRType Scale);
CagdCrvStruct *IritCagdCrvUnitMaxCoef(CagdCrvStruct *Crv);
CagdSrfStruct *IritCagdSrfUnitMaxCoef(CagdSrfStruct *Srf);
CagdCrvStruct *IritCagdCrvRotateToXY(const CagdCrvStruct *Crv);
CagdBType IritCagdCrvRotateToXYMat(const CagdCrvStruct *Crv,
				   IrtHmgnMatType Mat);
void IritCagdSrfTransInnerCtlPts2Pt(CagdSrfStruct *Srf,
				    const IrtPtType Pt,	
				    IrtRType Ratio);

/*****************************************************************************
* Routines to handle curves generically.				     *
*****************************************************************************/
typedef struct IritCagdCrvIncCnstrctStruct *IritCagdCrvIncCnstrctStructPtr;
void IritCagdCrvIncCnstrctPrint(IritCagdCrvIncCnstrctStructPtr CrvIncCnstrct);
int IritCagdCrvIncCnstrctSize(IritCagdCrvIncCnstrctStructPtr CrvIncCnstrct);
CagdCrvStruct *IritCagdCrvIncCnstrct(
				IritCagdCrvIncCnstrctType Op,
				CagdRType *Params,
				IritCagdCrvIncCnstrctStructPtr *CrvIncCnstrct);
const char *IritCagdCrvIncCnstrctError(
				 IritCagdCrvIncCnstrctStructPtr CrvIncCnstrct);
CagdCrvStruct *IritCagdCrvIncCnstrctList(
				 IritCagdCrvIncCnstrctInputStruct *CrvIncInput,
				 char **ErrorStr);
CagdRType *IritCagdCrvNodes(const CagdCrvStruct *Crv);
CagdRType IritCagdEstimateCrvCollinearity(const CagdCrvStruct *Crv);
void IritCagdCrvDomain(const CagdCrvStruct *Crv,
		       CagdRType *TMin,
		       CagdRType *TMax);
CagdCrvStruct *IritCagdCrvSetDomain(CagdCrvStruct *Crv,
				    CagdRType TMin,
				    CagdRType TMax);
void IritCagdCrvEvalToData(const CagdCrvStruct *Crv, CagdRType t, CagdRType *R);
CagdRType *IritCagdCrvEvalMalloc(const CagdCrvStruct *Crv, CagdRType t);
void IritCagdCrvEvalEndPtsE3(const CagdCrvStruct *Crv,
			     CagdPType Start,
			     CagdPType End);
CagdCrvStruct *IritCagdCrvDerive(const CagdCrvStruct *Crv);
CagdCrvStruct *IritCagdCrvDeriveScalar(const CagdCrvStruct *Crv);
void IritCagdCrvScalarCrvSlopeBounds(const CagdCrvStruct *Crv,
				     CagdRType *MinSlope,
				     CagdRType *MaxSlope);
CagdCrvStruct *IritCagdCrvIntegrate(const CagdCrvStruct *Crv);
CagdCrvStruct *IritCagdCrv2DNormalField(const CagdCrvStruct *Crv);
CagdCrvStruct *IritCagdCrvMoebiusTransform(const CagdCrvStruct *Crv,
					   CagdRType c);
int IritCagdCrvIsCtlPolyMonotone(const CagdCrvStruct *Crv,
				 int Axis, CagdRType Eps);
CagdCrvStruct *IritCagdCrvSubdivAtParam(const CagdCrvStruct *Crv, CagdRType t);
CagdCrvStruct *IritCagdCrvSubdivAtParams(const CagdCrvStruct *Crv,
					 const CagdPtStruct *Pts,
					 CagdRType Eps,
					 CagdBType PurgeTooSmallSegs,
					 int *Proximity);
CagdCrvStruct *IritCagdCrvSubdivAtParams2(const CagdCrvStruct *CCrv,
					  const	CagdPtStruct *Pts,
					  int Idx,
					  CagdRType Eps,
					  CagdBType PurgeTooSmallSegs,
					  int *Proximity);
CagdCrvStruct *IritCagdCrvSubdivAtParams3(const CagdCrvStruct *CCrv,
					  CagdRType *Prms,
					  int PrmsLen,
					  CagdRType Eps,
					  CagdBType PurgeTooSmallSegs,
					  int *Proximity);
CagdCrvStruct *IritCagdCrvRegionFromCrv(const CagdCrvStruct *Crv,
					CagdRType t1,
					CagdRType t2);
CagdCrvStruct *IritCagdCrvRegionFromCrvWrap(const CagdCrvStruct *Crv,
					    CagdRType t1,
					    CagdRType t2);
CagdCrvStruct *IritCagdCrvRefineAtParams(const CagdCrvStruct *Crv,
					 CagdBType Replace,
					 CagdRType *t,
					 int n);
CagdCrvStruct *IritCagdCrvRefineUniformly(const CagdCrvStruct *Crv,
					  int RefLevel);
CagdCrvStruct *IritCagdCrvRefineUniformly2(const CagdCrvStruct *Crv, int n);
CagdVecStruct *IritCagdCrvTangentToData(const CagdCrvStruct *Crv,
					CagdRType t,
					CagdBType Normalize,
					CagdVecStruct *Tan);
CagdVecStruct *IritCagdCrvBiNormalToData(const CagdCrvStruct *Crv,
					 CagdRType t,
					 CagdBType Normalize,
					 CagdVecStruct *Vec);
CagdVecStruct *IritCagdCrvBiNormalMalloc(const CagdCrvStruct *Crv,
					 CagdRType t,
					 CagdBType Normalize);
CagdVecStruct *IritCagdCrvNormalToData(const CagdCrvStruct *Crv,
				       CagdRType t,
				       CagdBType Normalize,
				       CagdVecStruct *N);
CagdVecStruct *IritCagdCrvNormalMalloc(const CagdCrvStruct *Crv,
				       CagdRType t,
				       CagdBType Normalize);
CagdVecStruct *IritCagdCrvNormalXYToData(const CagdCrvStruct *Crv,
					 CagdRType t,
					 CagdBType Normalize,
					 CagdVecStruct *Vec);
CagdPolylineStruct *IritCagdCrv2Polyline(const CagdCrvStruct *Crv,
					 int SamplesPerCurve,
					 CagdBType OptiLin);
CagdCrvStruct *IritCagdCrvReverse(const CagdCrvStruct *Crv);
CagdCrvStruct *IritCagdCrvReverseUV(const CagdCrvStruct *Crv);
CagdCrvStruct *IritCagdCrvSubdivAtAllC0Discont(const CagdCrvStruct *Crv,
					       IrtBType	EuclideanC1Discont,
					       IrtRType	Tolerance);
CagdPtStruct *IritCagdBspCrvAllEuclideanC1Discont(const CagdCrvStruct *Crv,
						  IrtBType EuclideanC1Discont,
						  IrtRType Tolerance);
CagdCrvStruct *IritCagdCrvSubdivAtAllC1Discont(const CagdCrvStruct *Crv,
					       IrtBType	EuclideanC1Discont,
					       IrtRType	Tolerance);
CagdCrvStruct *IritCagdBspCrvsSubdivAtAllDetectedLocations(
				   	   const CagdCrvStruct *Crv,
				           CagdCrvTestingFuncType CrvTestFunc);
CagdCrvStruct *IritCagdCrvDegreeRaise(const CagdCrvStruct *Crv);
CagdCrvStruct *IritCagdCrvDegreeRaiseN(const CagdCrvStruct *Crv, int NewOrder);
CagdCrvStruct *IritCagdCrvDegreeReduce(const CagdCrvStruct *Crv);
CagdCrvStruct *IritCagdCrvCreateArc(const CagdPtStruct *Center,
				    CagdRType Radius,
				    CagdRType StartAngle,
				    CagdRType EndAngle);
CagdCrvStruct *IritCagdCrvCreateArcCCW(const CagdPtStruct *Start,
				       const CagdPtStruct *Center,
				       const CagdPtStruct *End);
CagdCrvStruct *IritCagdCrvCreateArcCW(const CagdPtStruct *Start,
				      const CagdPtStruct *Center,
				      const CagdPtStruct *End);
CagdCrvStruct *IritCagdCreateConicCurve(CagdRType A,
					CagdRType B,
					CagdRType C,
					CagdRType D,
					CagdRType E,
					CagdRType F,
					CagdRType ZLevel,
					CagdBType RationalEllipses);
CagdCrvStruct *IritCagdCreateConicCurve2(CagdRType A,
					 CagdRType B,
					 CagdRType C,
					 CagdRType D,
					 CagdRType E,
					 CagdRType F,
					 CagdRType ZLevel,
					 const CagdRType *PStartXY,
					 const CagdRType *PEndXY,
					 CagdBType RationalEllipses);
CagdBType IritCagdEllipse3Points(CagdPType Pt1,
				 CagdPType Pt2,
				 CagdPType Pt3,
				 CagdRType *A,
				 CagdRType *B,
				 CagdRType *C,
				 CagdRType *D,
				 CagdRType *E,
				 CagdRType *F);
CagdBType IritCagdEllipse4Points(CagdPType Pt1,
				 CagdPType Pt2,
				 CagdPType Pt3,
				 CagdPType Pt4,
				 CagdRType *A,
				 CagdRType *B,
				 CagdRType *C,
				 CagdRType *D,
				 CagdRType *E,
				 CagdRType *F);
CagdBType IritCagdEllipseOffset(CagdRType *A,
				CagdRType *B,
				CagdRType *C,
				CagdRType *D,
				CagdRType *E,
				CagdRType *F,
				CagdRType Offset);
CagdBType IritCagdConicMatTransform(CagdRType *A,
				    CagdRType *B,
				    CagdRType *C,
				    CagdRType *D,
				    CagdRType *E,
				    CagdRType *F,
				    CagdMType Mat);
CagdBType IritCagdQuadricMatTransform(CagdRType *A,
				      CagdRType	*B,
				      CagdRType	*C,
				      CagdRType	*D,
				      CagdRType	*E,
				      CagdRType	*F,
				      CagdRType	*G,
				      CagdRType	*H,
				      CagdRType	*I,
				      CagdRType	*J,
				      CagdMType	Mat);
CagdBType IritCagdConic2Quadric(CagdRType *A,
				CagdRType *B,
				CagdRType *C,
				CagdRType *D,
				CagdRType *E,
				CagdRType *F,
				CagdRType *G,
				CagdRType *H,
				CagdRType *I,
				CagdRType *J);
CagdSrfStruct *IritCagdCreateQuadricSrf(CagdRType A,
					CagdRType B,
					CagdRType C,
					CagdRType D,
					CagdRType E,
					CagdRType F,
					CagdRType G,
					CagdRType H,
					CagdRType I,
					CagdRType J);
CagdCrvStruct *IritCagdMergeCrvCrv(const CagdCrvStruct *Crv1,
				   const CagdCrvStruct *Crv2,
				   CagdBType InterpDiscont,
				   CagdRType MergeEps);
CagdCrvStruct *IritCagdMergeCrvList(const CagdCrvStruct *CrvList,
				    CagdBType InterpDiscont,
				    CagdRType MergeEps);
CagdCrvStruct *IritCagdMergeCrvList2(CagdCrvStruct *CrvList,
				     IrtRType Tolerance,
				     CagdBType InterpDiscont);
CagdCrvStruct *IritCagdMergeCrvList3(CagdCrvStruct *CrvList,
				     IrtRType Tolerance,
				     CagdBType InterpDiscont);
void IritCagdCrvCrvMakeJoinMatch(CagdCrvStruct **Crv1,
				 CagdCrvStruct **Crv2,
				 IrtRType Tolerance,
				 CagdBType G1Continuity,
				 CagdBType ClosedLoop);
CagdCrvStruct *IritCagdCrvListMakeJoinMatch(const CagdCrvStruct *CrvList,
					    IrtRType Tolerance,
					    CagdBType G1Continuity,
					    CagdBType ClosedLoop);
CagdCrvStruct *IritCagdMergeCrvPt(const CagdCrvStruct *Crv,
				  const	CagdPtStruct *Pt);
CagdCrvStruct *IritCagdMergePtCrv(const CagdPtStruct *Pt,
				  const	CagdCrvStruct *Crv);
CagdCrvStruct *IritCagdMergeCrvCtlPt(const CagdCrvStruct *Crv,
				     const CagdCtlPtStruct *CtlPt);
CagdCrvStruct *IritCagdMergeCtlPtCrv(const CagdCtlPtStruct *CtlPt,
				     const CagdCrvStruct *Crv);
CagdCrvStruct *IritCagdMergePtPt(const CagdPtStruct *Pt1,
				 const CagdPtStruct *Pt2);
CagdCrvStruct *IritCagdMergePtPtLen(const CagdPtStruct *Pt1,
				    const CagdPtStruct *Pt2,
				    int	Len);
CagdCrvStruct *IritCagdMergePtPt2(const CagdPType Pt1, const CagdPType Pt2);
CagdCrvStruct *IritCagdMergeUvUv(const CagdUVType UV1, const CagdUVType UV2);
CagdCrvStruct *IritCagdMergeCtlPtCtlPt(const CagdCtlPtStruct *Pt1,
				       const CagdCtlPtStruct *Pt2,
				       int MinDim);
CagdRType IritCagdCrvAreaPoly(const CagdCrvStruct *Crv);
CagdRType IritCagdCrvArcLenPoly(const CagdCrvStruct *Crv);
CagdCrvStruct *IritCagdLimitCrvArcLen(const CagdCrvStruct *Crv,
				      CagdRType MaxLen);
CagdPolylineStruct *IritCagdCrv2CtrlPoly(const CagdCrvStruct *Crv);
CagdCrvStruct *IritCagdEditSingleCrvPt(const CagdCrvStruct *Crv,
				       CagdCtlPtStruct *CtlPt,
				       int Index,
				       CagdBType Write);
CagdCrvStruct *IritCagdCrvDeletePoint(const CagdCrvStruct *Crv, int Index);
CagdCrvStruct *IritCagdCrvInsertPoint(const CagdCrvStruct *Crv,
				      int Index,
				      const CagdPType Pt);
CagdBType IritCagdMakeCrvsCompatible(CagdCrvStruct **Crv1,
				     CagdCrvStruct **Crv2,
				     CagdBType SameOrder,
				     CagdBType SameKV);
CagdBType IritCagdMakeCrvsCompatible2(CagdCrvStruct **Crv1,
				      CagdCrvStruct **Crv2,
				      CagdBType	SameOrder,
				      CagdBType	SameKV);
CagdBBoxStruct *IritCagdCrvBBox(const CagdCrvStruct *Crv,
				CagdBBoxStruct *BBox);
CagdBBoxStruct *IritCagdCrvListBBox(const CagdCrvStruct *Crvs,
				    CagdBBoxStruct *BBox);
CagdBType IritCagdCrvIsConstant(const CagdCrvStruct *Crv, IrtRType Eps);
void IritCagdCrvMinMax(const CagdCrvStruct *Crv,
		       int Axis,
		       CagdRType *Min,
		       CagdRType *Max);
CagdRType IritCagdCtlMeshAverageValue(CagdRType * const *Pts,
				      int Length,
				      int Axis);
CagdRType IritCagdCrvAverageValue(const CagdCrvStruct *Crv, int Axis);
CagdBType IritCagdIsCrvInsideCirc(const CagdCrvStruct *Crv,
				  const	CagdRType Center[2],
				  CagdRType Radius);
int IritCagdIsCrvInsideCH(const CagdCrvStruct *Crv,
			  const	IrtE2PtStruct *CHPts,
			  int NumCHPts);
int IritCagdCrvEvalToPolyline(const CagdCrvStruct *Crv,
				  int FineNess,
				  CagdRType *Points[],
				  BspKnotAlphaCoeffStruct *A,
				  CagdBType OptiLin);
void IritCagdCrvFirstMoments(const CagdCrvStruct *Crv,
			     int n,
			     CagdPType Pt,
			     CagdVType Dir);
CagdCrvStruct *IritCagdCubicHermiteCrv(const CagdPType Pt1,
				       const CagdPType Pt2,
				       const CagdVType Dir1,
				       const CagdVType Dir2);
CagdCrvStruct *IritCagdCubicHermiteCrv2(const CagdRType *Pt1,
					const CagdRType	*Pt2,
					const CagdRType	*Dir1,
					const CagdRType	*Dir2,
					int Dim);
CagdCrvStruct *IritCagdCubicCrvFit(const CagdCrvStruct *Crv);
CagdCrvStruct *IritCagdQuadraticCrvFit(const CagdCrvStruct *Crv);
CagdSrfStruct *IritCagdQuinticHermiteSrf(const CagdCrvStruct *CPos1Crv,
					 const CagdCrvStruct *CPos2Crv,
					 const CagdCrvStruct *CDir1Crv,
					 const CagdCrvStruct *CDir2Crv,
					 const CagdCrvStruct *C2Dir1Crv,
					 const CagdCrvStruct *C2Dir2Crv);
CagdSrfStruct *IritCagdCubicSrfFit(const CagdSrfStruct *Srf);
CagdSrfStruct *IritCagdQuadraticSrfFit(const CagdSrfStruct *Srf);
CagdRType IritCagdMatchDistNorm(const CagdVType T1,
				const CagdVType	T2,
				const CagdVType	P1,
				const CagdVType	P2);
CagdRType IritCagdMatchRuledNorm(const CagdVType T1,
				 const CagdVType T2,
				 const CagdVType P1,
				 const CagdVType P2);
CagdRType IritCagdMatchRuled2Norm(const CagdVType T1,
				  const	CagdVType T2,
				  const	CagdVType P1,
				  const	CagdVType P2);
CagdRType IritCagdMatchBisectorNorm(const CagdVType T1,
				    const CagdVType T2,
				    const CagdVType P1,
				    const CagdVType P2);
CagdRType IritCagdMatchMorphNorm(const CagdVType T1,
				 const CagdVType T2,
				 const CagdVType P1,
				 const CagdVType P2);

void IritCagdMatchingFixVector(int *OldVec, CagdRType *NewVec, int Len);
void IritCagdMatchingFixCrv(CagdCrvStruct *Crv);
void IritCagdMatchingPolyTransform(CagdRType **Poly,
				   int Len,
				   CagdRType NewBegin,
				   CagdRType NewEnd);
void IritCagdMatchingVectorTransform(CagdRType *Vec,
				     CagdRType NewBegin,
				     CagdRType NewEnd,
				     int Len);
CagdCrvStruct *IritCagdMatchingTwoCurves(const CagdCrvStruct *Crv1,
					 const CagdCrvStruct *Crv2,
					 int Reduce,
					 int SampleSet,
					 int ReparamOrder,
					 int RotateFlag,
					 int AllowNegativeNorm,
					 int ReturnReparamFunc,
					 CagdBType MinimizeMaxError,
					 CagdMatchNormFuncType MatchNormFunc);
CagdBType IritCagdCrvTwoCrvsOrient(CagdCrvStruct *Crv1,
				   CagdCrvStruct *Crv2,
				   int n);
CagdBType IritCagdIsClosedCrv(const CagdCrvStruct *Crv);
void IritCagdForceClosedCrv(CagdCrvStruct *Crv);
CagdBType IritCagdAreClosedCrvs(const CagdCrvStruct *Crvs,
				const CagdSrfStruct *Srf);
CagdBType IritCagdIsZeroLenCrv(const CagdCrvStruct *Crv, CagdRType Eps);
CagdBType IritCagdCrvTanAngularSpan(const CagdCrvStruct *Crv,
				    CagdVType ConeDir,
				    CagdRType *AngularSpan);
CagdRType IritCagdDistCrvLine(const CagdCrvStruct *Crv, CagdLType Line);
CagdPtStruct *IritCagdCrvCrvInter(const CagdCrvStruct *Crv1,
				  const	CagdCrvStruct *Crv2,
				  CagdRType Eps);
CagdPtStruct *IritCagdInsertInterPoints(CagdRType t1,
					CagdRType t2,
					CagdRType Eps,
					CagdPtStruct **InterList);
CagdCrvStruct *IritCagdCrvCrvInterArrangment(const CagdCrvStruct *ArngCrvs,
					     CagdBType SplitCrvs,
					     CagdRType Eps);
CagdBType IritCagdCrvsSameFuncSpace(const CagdCrvStruct *Crv1,
				    const CagdCrvStruct	*Crv2,
				    CagdRType Eps);
CagdBType IritCagdCrvsSame(const CagdCrvStruct *Crv1,
			   const CagdCrvStruct *Crv2,
			   CagdRType Eps);
CagdBType IritCagdCrvsSame2(const CagdCrvStruct *Crv1,
			    const CagdCrvStruct	*Crv2,
			    CagdRType Eps);
CagdBType IritCagdCrvsSame3(const CagdCrvStruct *Crv1,
			    const CagdCrvStruct	*Crv2,
			    CagdRType Eps,
			    CagdBType *Reversed);
int IritCagdCrvsRelation(const CagdCrvStruct *Crv1,
			 const CagdCrvStruct *Crv2,
			 CagdRType ParialOverlapRatio,
			 int *Crv1StartOn2,
			 int *Crv1EndOn2,
			 int *Crv2StartOn1,
			 int *Crv2EndOn1,
			 CagdRType *t1Start2,
			 CagdRType *t1End2,
			 CagdRType *t2Start1,
			 CagdRType *t2End1,
			 CagdRType *RelOverlap,
			 CagdRType Eps);
int IritCagdSrfsAddAdjAttributes(CagdSrfStruct *Srfs, 
				 CagdCrvAdjCmpFuncType CrvCmpFuncPtr, 
				 CagdRType Eps);
void IritCagdSrfsFreeAdjAttributes(CagdSrfStruct *Srfs);
CagdCrvStruct *IritCagdOrientCrvAsCrv2EndPts(const CagdCrvStruct *OrntCrv,
					     const CagdCrvStruct *Crv);
CagdBType IritCagdCrvsSameUptoRigidScl2D(const CagdCrvStruct *Crv1,
					 const CagdCrvStruct *Crv2,
					 IrtPtType Trans,
					 CagdRType *Rot,
					 CagdRType *Scl,
					 CagdRType Eps);
CagdPtStruct *IritCagdCrvZeroSet(const CagdCrvStruct *Crv,
				 int Axis,
				 int NRInit,
				 CagdRType NumericTol,
				 CagdRType SubdivTol);
CagdPtStruct *IritCagdCrvZeroSetC0(const CagdCrvStruct *Crv,
				   int Axis,
				   int NRInit,
				   CagdRType NumericTol,
				   CagdRType SubdivTol);
CagdBType IritCagdCrvZeroNumericStep(const CagdCrvStruct *Crv,
				     CagdCrvStruct *DCrv,
				     CagdRType Seed,
				     CagdBType LeftOut,	
				     CagdBType RightOut,
				     CagdBType CheckEndPts,
				     CagdRType *Solution,
				     CagdRType NumericTol);

CagdSrfStruct *IritCagdSrfExtensionDup(const CagdSrfStruct *Srf,
				       CagdSrfBndryType	Bndry);

/*****************************************************************************
* Routines to handle surfaces generically.				     *
*****************************************************************************/
CagdRType *IritCagdSrfNodes(const CagdSrfStruct *Srf, CagdSrfDirType Dir);
CagdRType IritCagdEstimateSrfPlanarity(const CagdSrfStruct *Srf);
void IritCagdSrfDomain(const CagdSrfStruct *Srf,
		       CagdRType *UMin,
		       CagdRType *UMax,
		       CagdRType *VMin,
		       CagdRType *VMax);
CagdSrfStruct *IritCagdSrfSetDomain(CagdSrfStruct *Srf,
				    CagdRType UMin,
				    CagdRType UMax,
				    CagdRType VMin,
				    CagdRType VMax);
void IritCagdSrfEvalToData(const CagdSrfStruct *Srf,
			   CagdRType u,
			   CagdRType v,
			   CagdRType *R);
CagdRType *IritCagdSrfEvalMalloc(const CagdSrfStruct *Srf,
				 CagdRType u,
				 CagdRType v);
void IritCagdSrfEstimateCurveness(const CagdSrfStruct *Srf,
				  CagdRType *UCurveness,
				  CagdRType *VCurveness);
struct IPPolygonStruct *IritCagdSrf2Polygons(const CagdSrfStruct *Srf,
					     const CagdSrf2PlsInfoStrct	*TessInfo);
struct IPPolygonStruct *IritCagdSrf2PolygonsN(const CagdSrfStruct *Srf,
					      int Nu,
					      int Nv,
					      CagdBType	ComputeNrmls,
					      CagdBType	FourPerFlat,
					      CagdBType	ComputeUVs);
IritCagdSrfErrorFuncType IritCagdSrf2PolyAdapSetErrFunc(
					  IritCagdSrfErrorFuncType Func,
					  void *Data);
IritCagdSrfAdapAuxDataFuncType IritCagdSrf2PolyAdapSetAuxDataFunc(
					  IritCagdSrfAdapAuxDataFuncType Func);
IritCagdSrfAdapPolyGenFuncType IritCagdSrf2PolyAdapSetPolyGenFunc(
					  IritCagdSrfAdapPolyGenFuncType Func);
CagdRType IritCagdSrfAdap2PolyDefErrFunc(const CagdSrfStruct *Srf,
					 CagdRType Tolerance,
					 void *AuxData);
CagdRType IritCagdSrfIsLinearCtlMeshOneRowCol(const CagdSrfStruct *Srf,
					      int Idx,
					      CagdSrfDirType Dir);
CagdRType IritCagdSrfIsLinearCtlMesh(const CagdSrfStruct *Srf,
				     CagdBType Interior);
CagdRType IritCagdSrfIsLinearBndryCtlMesh(const CagdSrfStruct *Srf);
CagdRType IritCagdSrfIsCoplanarCtlMesh(const CagdSrfStruct *Srf);
struct IPPolygonStruct *IritCagdSrfAdap2Polygons(const CagdSrfStruct *Srf,
						 VoidPtr AuxSrfData,
						 const CagdSrf2PlsInfoStrct
						 *TessInfo);
struct IPPolygonStruct *IritCagdSrf2PolygonsGenPolys(const CagdSrfStruct *Srf,
						     CagdRType *PtWeights,
						     CagdPtStruct *PtMesh,
						     CagdVecStruct *PtNrml,
						     CagdUVStruct *UVMesh,
						     int FineNessU,
						     int FineNessV, 
						     const CagdSrf2PlsInfoStrct
						     *TessInfo);
struct IPPolygonStruct *IritCagdSrfAdapRectPolyGen(
					     const CagdSrfStruct *Srf,
					     CagdSrfPtStruct *SrfPtList,
					     const CagdSrfAdapRectStruct *Rect,
					     const CagdSrf2PlsInfoStrct
					     *TessInfo);
CagdRType *IritCagdSrfAdap2PolyEvalNrmlBlendedUV(const CagdRType *UV1,
						 const CagdRType *UV2,
						 const CagdRType *UV3,
						 void *NrmlEvalCache,
						 CagdRType *Nrml);
CagdBType IritCagdSrf2PolygonStrip(CagdBType PolygonStrip);
CagdBType IritCagdSrf2PolygonFast(CagdBType PolygonFast);
CagdBType IritCagdSrf2PolygonMergeCoplanar(CagdBType MergeCoplanarPolys);
CagdBType IritCagd2PolyClipPolysAtPoles(CagdBType ClipPolysAtPoles);
CagdPolylineStruct *IritCagdSrf2Polylines(const CagdSrfStruct *Srf,
					  int NumOfIsocurves[2],
					  int SamplesPerCurve);
CagdCrvStruct *IritCagdSrf2Curves(const CagdSrfStruct *Srf,
				  int NumOfIsocurves[2]);
CagdPolylineStruct *IritCagdSrf2KnotPolylines(const CagdSrfStruct *Srf,
					      int SamplesPerCurve,
					      BspKnotAlphaCoeffStruct *A);
void IritCagdEvaluateSurfaceVecField(CagdVType Vec,
				     CagdSrfStruct *VecFieldSrf,
				     CagdRType U,
				     CagdRType V);
CagdSrfStruct *IritCagdSrfDerive(const CagdSrfStruct *Srf, CagdSrfDirType Dir);
CagdSrfStruct *IritCagdSrfDeriveScalar(const CagdSrfStruct *Srf,
				       CagdSrfDirType Dir);
CagdSrfStruct *IritCagdSrfIntegrate(const CagdSrfStruct *Srf,
				    CagdSrfDirType Dir);
CagdBType IritCagdSrfIsSingular(const CagdSrfStruct *Srf);
CagdSrfStruct *IritCagdSrfsFilterSingular(CagdSrfStruct *Srfs);
CagdSrfStruct *IritCagdSrfMoebiusTransform(const CagdSrfStruct *Srf,
					   CagdRType c,
					   CagdSrfDirType Dir);
CagdCrvStruct *IritCagdCrvFromSrf(const CagdSrfStruct *Srf,
				  CagdRType t,
				  CagdSrfDirType Dir);
CagdCrvStruct *IritCagdCrvFromMesh(const CagdSrfStruct *Srf,
				   int Index,
				   CagdSrfDirType Dir);
CagdCrvStruct **IritCagdBndryCrvsFromSrf(const CagdSrfStruct *Srf,
					 CagdCrvStruct *Crvs[4]);
CagdCrvStruct *IritCagdBndryCrvFromSrf(const CagdSrfStruct *Srf,
				       CagdSrfBndryType	Bndry);
CagdCrvStruct *IritCagdBndryAsOneCrvFromSrf(const CagdSrfStruct *Srf);

void IritCagdCrvToMesh(const CagdCrvStruct *Crv,
		       int Index,
		       CagdSrfDirType Dir,
		       CagdSrfStruct *Srf);
CagdSrfStruct *IritCagdSrfSubdivAtParam(const CagdSrfStruct *Srf,
					CagdRType t,
					CagdSrfDirType Dir);
CagdSrfStruct *IritCagdSrfSubdivAtAllC0Discont(const CagdSrfStruct *Srf);
CagdSrfStruct *IritCagdSrfsSubdivAtAllC0Discont(const CagdSrfStruct *Srfs);
CagdSrfStruct *IritCagdSrfSubdivAtAllC1Discont(const CagdSrfStruct *Srf);
CagdSrfStruct *IritCagdSrfsSubdivAtAllC1Discont(const CagdSrfStruct *Srfs);
CagdSrfStruct *IritCagdSrfSubdivAtAllCnDiscont(const CagdSrfStruct *Srf, int n);
CagdSrfStruct *IritCagdBspSrfsSubdivAtAllDetectedLocations(
						      const CagdSrfStruct *Srf,
						      CagdSrfTestingFuncType
						      SrfTestFunc);
CagdSrfStruct *IritCagdSrfSubdivAtPoles(const CagdSrfStruct *Srf,
					CagdRType Tol);
CagdSrfStruct *IritCagdSrfRegionFromSrf(const CagdSrfStruct *Srf,
					CagdRType t1,
					CagdRType t2,
					CagdSrfDirType Dir);
CagdSrfStruct *IritCagdSrfRegionFromSrf2(const CagdSrfStruct *Srf,
					 CagdRType UMin,
					 CagdRType UMax,
					 CagdRType VMin,
					 CagdRType VMax);
CagdSrfStruct *IritCagdSrfRefineAtParams(const CagdSrfStruct *Srf,
					 CagdSrfDirType	Dir,
					 CagdBType Replace,
					 CagdRType *t,
					 int n);
CagdVecStruct *IritCagdSrfTangentToData(const CagdSrfStruct *Srf,
					CagdRType u,
					CagdRType v,
					CagdSrfDirType Dir,
					CagdBType Normalize,
					CagdVecStruct *Tan);
CagdVecStruct *IritCagdSrfNormalToData(const CagdSrfStruct *Srf,
					   CagdRType u,
					   CagdRType v,
					   CagdBType Normalize,
					   CagdVecStruct *Normal);
CagdVecStruct *IritCagdSrfNormalMalloc(const CagdSrfStruct *Srf,
					   CagdRType u,
					   CagdRType v,
					   CagdBType Normalize);
CagdUVType *IritCagdSrfUVDirOrthoE3ToData(const CagdSrfStruct *Srf,
					  const	CagdUVType *UV,
					  const	CagdUVType *UVDir,
					  CagdUVType *OrthoUVDir);
CagdUVType *IritCagdSrfUVDirOrthoE3Malloc(const CagdSrfStruct *Srf,
					  const	CagdUVType *UV,
					  const	CagdUVType *UVDir);
CagdSrfStruct *IritCagdSrfDegreeRaise(const CagdSrfStruct *Srf,
				      CagdSrfDirType Dir);
CagdSrfStruct *IritCagdSrfDegreeRaiseN(const CagdSrfStruct *Srf,
				       int NewUOrder,
				       int NewVOrder);
CagdSrfStruct *IritCagdSrfReverse(const CagdSrfStruct *Srf);
CagdSrfStruct *IritCagdSrfReverseDir(const CagdSrfStruct *Srf,
				     CagdSrfDirType Dir);
CagdSrfStruct *IritCagdSrfReverse2(const CagdSrfStruct *Srf);
CagdSrfStruct *IritCagdSrfMakeBoundryIndexUMin(const CagdSrfStruct *Srf,
					       int BndryIdx);
CagdPolylineStruct *IritCagdSrf2CtrlMesh(const CagdSrfStruct *Srf);
CagdPolylineStruct *IritCagdSrf2KnotLines(const CagdSrfStruct *Srf);
int IritCagdSrf2KnotCurves(const CagdSrfStruct *Srf,
			   CagdCrvStruct **UKnotLines,
			   CagdCrvStruct **VKnotLines);
CagdSrfStruct *IritCagdMergeSrfSrf(const CagdSrfStruct *CSrf1,
				   const CagdSrfStruct *CSrf2,
				   CagdSrfDirType Dir,
				   CagdBType SameEdge,
				   CagdBType InterpolateDiscont);
CagdSrfStruct *IritCagdMergeSrfList(const CagdSrfStruct *SrfList,
				    CagdSrfDirType Dir,
				    CagdBType SameEdge,
				    CagdBType InterpolateDiscont);
CagdSrfStruct *IritCagdMergeSrfList2(CagdSrfStruct *SrfList,
				     CagdSrfDirType Dir,
				     IrtRType Tolerance,
				     CagdBType InterpDiscont);
CagdSrfStruct *IritCagdMergeSrfList3U(CagdSrfStruct *SrfList,
				      IrtRType Tolerance,
				      CagdBType	InterpDiscont);
CagdSrfStruct *IritCagdMergeSrfList3V(CagdSrfStruct *SrfList,
				      IrtRType Tolerance,
				      CagdBType	InterpDiscont);
CagdBType IritCagdSrfSrfMakeJoinMatch(CagdSrfStruct **Srf1,
				      CagdSrfStruct	**Srf2,
				      CagdSrfDirType Dir,
				      CagdRType Tolerance,
				      CagdBType PreserveOrientation);
CagdRType IritCagdSrfAvgArgLenMesh(const CagdSrfStruct *Srf,
				   CagdRType *AvgULen,
				   CagdRType *AvgVLen);
CagdSrfStruct *IritCagdExtrudeSrf(const CagdCrvStruct *Crv,
				  const	CagdVecStruct *Vec);
CagdSrfStruct *IritCagdExtrudeSrfList(const CagdCrvStruct *Crvs,
				      const CagdVecStruct *Vec);
CagdSrfStruct *IritCagdZTwistExtrudeSrf(const CagdCrvStruct *CCrv,
					CagdBType Rational,
					CagdRType ZPitch);
CagdSrfStruct *IritCagdSurfaceRev(const CagdCrvStruct *Crv);
CagdSrfStruct *IritCagdSurfaceRevAxis(const CagdCrvStruct *Crv,
				      CagdVType Axis);
CagdSrfStruct *IritCagdSurfaceRev2(const CagdCrvStruct *Crv,
				   CagdBType PolyApprox,
				   CagdRType StartAngle,
				   CagdRType EndAngle);
CagdSrfStruct *IritCagdSurfaceRev2Axis(const CagdCrvStruct *Crv,
				       CagdBType PolyApprox,
				       CagdRType StartAngle,
				       CagdRType EndAngle,
				       const CagdVType Axis);
CagdSrfStruct *IritCagdSurfaceRevPolynomialApprox(const CagdCrvStruct *Crv);
CagdBType IritCagdCrvOrientationFrame(const CagdCrvStruct *Crv,
				      CagdRType	CrntT,
				      CagdVecStruct *Tangent,
				      CagdVecStruct *Normal,
				      CagdVecStruct **MallocedNrmls,
				      int *MallocedNrmlsVecSize,
				      CagdBType	FirstTime);
CagdSrfStruct *IritCagdSweepSrf(const CagdCrvStruct *CrossSection,
				const CagdCrvStruct *Axis,
				const CagdCrvStruct *ScalingCrv,
				CagdRType Scale,
				const VoidPtr Frame,
				int FrameOption,
				CagdRType *LastBiNormal);
CagdBType IritCagdSweepComputeNormalOrientation(const CagdCrvStruct *Axis,
						const CagdVType	FrameVec,
						CagdCrvStruct *FrameCrv,
						CagdRType *PrevT,
						CagdRType CrntT,
						const CagdVecStruct *Tangent,
						CagdVecStruct *Normal,
						CagdBType FirstTime);
CagdRType IritCagdSweepCosineHalfAngle(CagdRType **Points, int Index);
void IritCagdSweepGenTransformMatrix(CagdMType Mat,
				     const CagdRType *Trans,
				     const CagdVecStruct *Normal,
				     const CagdVecStruct *Tangent,
				     const CagdRType *Scale,
				     CagdRType NormalScale);
CagdBType IritCagdSweepSrfC1AdjSrfsInterDmn(const CagdSrfStruct *PrevSrf,
					    const CagdSrfStruct	*NextSrf,
					    CagdRType C1DiscontCropTol,
					    CagdRType *PrevSrfVMax,
					    CagdRType *NextSrfVMin);
CagdSrfStruct *IritCagdSweepSrfC1(const CagdCrvStruct *CrossSection,
				  const	CagdCrvStruct *Axis,
				  const	CagdCrvStruct *ScalingCrv,
				  CagdRType Scale,
				  const	VoidPtr	Frame,
				  CagdBType FrameIsCrv,
				  CagdCrvCornerType CornerType,
				  CagdRType C1DiscontCropTol);
CagdCrvStruct *IritCagdSweepAxisRefine(const CagdCrvStruct *Axis,
				       const CagdCrvStruct *ScalingCrv,
				       int RefLevel);
CagdSrfStruct *IritCagdSweepSrfError(const CagdSrfStruct *SweepSrf,
				     const CagdCrvStruct *CrossSection,
				     const CagdCrvStruct *Axis,
				     const CagdCrvStruct *ScalingCrv,
				     CagdRType Scale);
CagdSrfStruct *IritCagdBoolSumSrf(const CagdCrvStruct *CrvLeft,
				  const	CagdCrvStruct *CrvRight,
				  const	CagdCrvStruct *CrvTop,
				  const	CagdCrvStruct *CrvBottom);
CagdSrfStruct *IritCagdBoolSumSrf2(const CagdCrvStruct *Crv1,
				   const CagdCrvStruct *Crv2,
				   const CagdCrvStruct *Crv3,
				   const CagdCrvStruct *Crv4);
CagdSrfStruct *IritCagdBoolSumSrfRtnl(const CagdCrvStruct *CrvLeft,
				      const CagdCrvStruct *CrvRight,
				      const CagdCrvStruct *CrvTop,
				      const CagdCrvStruct *CrvBottom);
CagdSrfStruct *IritCagdOneBoolSumSrf(const CagdCrvStruct *BndryCrv);
int IritCagdGetCrvsCommonPt(const CagdCrvStruct *Crv1,
			    const CagdCrvStruct *Crv2, 
			    CagdPType P);
CagdSrfStruct *IritCagdOneSidedBoolSumSrf(const CagdCrvStruct *CCrvLeft,
					  const	CagdCrvStruct *CCrvBottom);
CagdCrvStruct *IritCagdReorderCurvesInLoop(CagdCrvStruct *UVCrvs);
CagdSrfStruct *IritCagdSrfFromNBndryCrvs(const CagdCrvStruct *Crvs,
					 CagdBType MinimizeSize);
CagdSrfStruct *IritCagdRuledSrf(const CagdCrvStruct *Crv1,
				const CagdCrvStruct *Crv2,
				int OtherOrder,
				int OtherLen);
CagdSrfStruct *IritCagdBilinearSrf(const CagdPtStruct *Pt00,
				   const CagdPtStruct *Pt01,
				   const CagdPtStruct *Pt10,
				   const CagdPtStruct *Pt11,
				   CagdPointType PType);
CagdSrfStruct *IritCagdCnvrtCrvToSrf(const CagdCrvStruct *Crv,
				     CagdSrfDirType Dir);
CagdSrfStruct *IritCagdSrfFromCrvs(const CagdCrvStruct *CrvList,
				   int OtherOrder,
				   CagdEndConditionType	OtherEC,
				   IrtRType *OtherParamVals);
CagdRType *IritCagdSrfInterpolateCrvsChordLenParams(
						 const CagdCrvStruct *CrvList);
CagdSrfStruct *IritCagdSrfInterpolateCrvs(const CagdCrvStruct *CrvList,
					  int OtherOrder,
					  CagdEndConditionType OtherEC,
					  CagdParametrizationType OtherParam,
					  IrtRType *OtherParamVals);
CagdBType IritCagdMakeSrfsCompatible(CagdSrfStruct **Srf1,
				     CagdSrfStruct **Srf2,
				     CagdBType SameUOrder,
				     CagdBType SameVOrder,
				     CagdBType SameUKV,
				     CagdBType SameVKV);
CagdBType IritCagdMakeSrfsCompatible2(CagdSrfStruct **Srf1,
				      CagdSrfStruct **Srf2,
				      CagdBType	SameUOrder,
				      CagdBType	SameVOrder,
				      CagdBType	SameUKV,
				      CagdBType	SameVKV);
CagdSrfStruct *IritCagdEditSingleSrfPt(const CagdSrfStruct *Srf,
				       CagdCtlPtStruct *CtlPt,
				       int UIndex,
				       int VIndex,
				       CagdBType Write);
CagdBBoxStruct *IritCagdSrfBBox(const CagdSrfStruct *Srf,
				CagdBBoxStruct *BBox);
CagdBBoxStruct *IritCagdSrfListBBox(const CagdSrfStruct *Srfs,
				    CagdBBoxStruct *BBox);
CagdBType IritCagdSrfBelowPlane(const CagdSrfStruct* Srf,
				const CagdRType Pln[4]);
CagdBType IritCagdSrfIsConstant(const CagdSrfStruct *Srf, IrtRType Eps);
void IritCagdSrfMinMax(const CagdSrfStruct *Srf,
		       int Axis,
		       CagdRType *Min,
		       CagdRType *Max);
CagdRType IritCagdSrfAverageValue(const CagdSrfStruct *Srf, int Axis);
CagdRType IritCagdPolyApproxMaxErr(const CagdSrfStruct *Srf,
				   const CagdPolygonStruct *Polys);
CagdRType *IritCagdPolyApproxErrs(const CagdSrfStruct *Srf,
				  const	CagdPolygonStruct *Polys);
int IritCagdPolyApproxErrEstimate(const CagdPolyErrEstimateType Method,
				  int Samples);
CagdSrfStruct *IritCagdCubicHermiteSrf(const CagdCrvStruct *CPos1Crv,
				       const CagdCrvStruct *CPos2Crv,
				       const CagdCrvStruct *CDir1Crv,
				       const CagdCrvStruct *CDir2Crv);
CagdSrfStruct *IritCagdBlendTwoSurfaces(const CagdSrfStruct *Srf1,
					const CagdSrfStruct *Srf2,
					int BlendDegree,
					CagdRType TanScale);
CagdBType IritCagdIsClosedSrf(const CagdSrfStruct *Srf, CagdSrfDirType Dir);
CagdBType IritCagdIsZeroLenSrfBndry(const CagdSrfStruct *Srf,
				    CagdSrfBndryType Bdnry,
				    CagdRType Eps);
CagdBType IritCagdSrfsSameCorners(const CagdSrfStruct *Srf1,
				  const	CagdSrfStruct *Srf2,
				  CagdRType Eps);
CagdBType IritCagdSrfsSameFuncSpace(const CagdSrfStruct *Srf1,
				    const CagdSrfStruct	*Srf2,
				    CagdRType Eps);
CagdBType IritCagdSrfsSame(const CagdSrfStruct *Srf1,
			   const CagdSrfStruct *Srf2,
			   CagdRType Eps);
CagdBType IritCagdSrfsSame2(const CagdSrfStruct *Srf1,
			    const CagdSrfStruct	*Srf2,
			    CagdRType Eps,
			    int	*Modified);
CagdBType IritCagdSrfsSame3(const CagdSrfStruct *Srf1,
			    const CagdSrfStruct	*Srf2,
			    CagdRType Eps,
			    int	*Modified);
CagdBType IritCagdSrfsSame4(const CagdSrfStruct *Srf1,
			    const CagdSrfStruct	*Srf2,
			    CagdRType Eps,
			    int	*Modified);
CagdSrfStruct *IritCagdSrfsFilterDuplicated(CagdSrfStruct *Srfs,
					    CagdRType Eps);
void IritCagdSrfEval4Corners(const CagdSrfStruct *Srf,
				 CagdPType P00,
				 CagdPType P01, 
				 CagdPType P10,
				 CagdPType P11);
CagdSrfStruct *IritCagdOrientSrfAsSrf4Corners(const CagdSrfStruct *OrntSrf,
					      const CagdSrfStruct *Srf);
CagdCrvStruct *IritCagdCrvUpdateLength(CagdCrvStruct *Crv, int NewLength);
CagdSrfStruct *IritCagdSrfUpdateLength(CagdSrfStruct *Srf,
				       int NewLength,
				       CagdSrfDirType Dir);
CagdSrfBndryType IritCagdSrfIsPtIndexBoundary(CagdSrfStruct *Srf, int PtIdx);
CagdSrfBndryType IritCagdCrvonSrfBndry(const CagdCrvStruct *Crv,
				       const CagdSrfStruct *Srf);
CagdBType IritCagdSrfsSameUptoRigidScl2D(const CagdSrfStruct *Srf1,
					 const CagdSrfStruct *Srf2,
					 IrtPtType Trans,
					 CagdRType *Rot,
					 CagdRType *Scl,
					 CagdRType Eps);
void *IritCagdSrfEffiNrmlPrelude(const CagdSrfStruct *Srf);
CagdVecStruct *IritCagdSrfEffiNrmlEval(CagdRType u,
				       CagdRType v,
				       CagdBType Normalize,
				       CagdVecStruct *RetVec,
				       void *EvalCache);
void IritCagdSrfEffiNrmlPostlude(void *EvalCache);
CagdRType IritCagdSrfArea(const CagdSrfStruct *Srf, CagdRType Tol);

/*****************************************************************************
* Routines to handle Power basis curves.				     *
*****************************************************************************/
CagdCrvStruct *IritCagdPwrCrvNew(int Length, CagdPointType PType);
void IritCagdPwrCrvEvalAtParamToData(const CagdCrvStruct *Crv,
				     CagdRType t,
				     CagdRType*	Buf);
CagdRType *PwrCrvEvalAtParamMalloc(const CagdCrvStruct *Crv, CagdRType t);
CagdCrvStruct *IritCagdPwrCrvDerive(const CagdCrvStruct *Crv,
				    CagdBType DeriveScalar);
CagdCrvStruct *IritCagdPwrCrvDeriveScalar(const CagdCrvStruct *Crv);
CagdCrvStruct *IritCagdPwrCrvIntegrate(const CagdCrvStruct *Crv);
CagdCrvStruct *IritCagdPwrCrvDegreeRaiseN(const CagdCrvStruct *Crv,
					  int NewOrder);
CagdCrvStruct *IritCagdPwrCrvDegreeRaise(const CagdCrvStruct *Crv);

/*****************************************************************************
* Routines to handle Power basis surfaces.				     *
*****************************************************************************/
CagdSrfStruct *IritCagdPwrSrfNew(int ULength,
				 int VLength,
				 CagdPointType PType);
CagdSrfStruct *IritCagdPwrSrfDegreeRaiseN(const CagdSrfStruct *Srf,
					  int NewUOrder,
					  int NewVOrder);
CagdSrfStruct *IritCagdPwrSrfDegreeRaise(const CagdSrfStruct *Srf,
					 CagdSrfDirType Dir);

/*****************************************************************************
* Routines to handle Bezier curves.					     *
*****************************************************************************/
CagdCrvStruct *IritCagdBzrCrvNew(int Length, CagdPointType PType);
CagdRType *IritCagdBzrCrvEvalBasisFuncs(int k,
					CagdRType t,
					CagdRType *Vec);
CagdRType IritCagdBzrCrvEvalVecAtParam(const CagdRType *Vec,
				       int VecInc,
				       int Order,
				       CagdRType t,
				       CagdRType *BasisFuncs);
void IritCagdBzrCrvEvalAtParamToData(const CagdCrvStruct *Crv, 
				     CagdRType t,
				     CagdRType *Pt);
CagdRType *IritCagdBzrCrvEvalAtParamMalloc(const CagdCrvStruct *Crv,
					   CagdRType t);
void IritCagdBzrCrvEvalToPolyline(const CagdCrvStruct *Crv,
				  int FineNess,
				  CagdRType *Points[]);
CagdCrvStruct *IritCagdBzrCrvCreateArc(const CagdPtStruct *Start,
				       const CagdPtStruct *Center,
				       const CagdPtStruct *End);
void IritCagdBzrCrvSubdivCtlPoly(CagdRType * const *Points,
				 CagdRType **LPoints,
				 CagdRType **RPoints,
				 int Length,
				 CagdPointType PType,
				 CagdRType t);
void IritCagdBzrCrvSubdivCtlPolyStep(CagdRType * const *Points,
				     CagdRType **LPoints,
				     CagdRType **RPoints,
				     int Length,
				     CagdPointType PType,
				     CagdRType t,
				     int Step);
CagdCrvStruct *IritCagdBzrCrvSubdivAtParam(const CagdCrvStruct *Crv,
					   CagdRType t);
CagdCrvStruct *IritCagdBzrCrvDegreeRaise(const CagdCrvStruct *Crv);
CagdCrvStruct *IritCagdBzrCrvDegreeRaiseN(const CagdCrvStruct *Crv,
					  int NewOrder);
CagdCrvStruct *IritCagdBzrCrvDegreeReduce(const CagdCrvStruct *Crv);
CagdCrvStruct *IritCagdBzrCrvDerive(const CagdCrvStruct *Crv,
				    CagdBType DeriveScalar);
CagdCrvStruct *IritCagdBzrCrvDeriveScalar(const CagdCrvStruct *Crv);
CagdCrvStruct *IritCagdBzrCrvIntegrate(const CagdCrvStruct *Crv);
CagdCrvStruct *IritCagdBzrCrvMoebiusTransform(const CagdCrvStruct *Crv,
					      CagdRType c);
CagdVecStruct *IritCagdBzrCrvTangentToData(const CagdCrvStruct *Crv,
					   CagdRType t,
					   CagdBType Normalize,
					   CagdVecStruct *Tan);
CagdVecStruct *IritCagdBzrCrvBiNormalToData(const CagdCrvStruct *Crv,
					    CagdRType t,
					    CagdBType Normalize,
					    CagdVecStruct *Vec);
CagdVecStruct *IritCagdBzrCrvBiNormalMalloc(const CagdCrvStruct *Crv,
					    CagdRType t,
					    CagdBType Normalize);
CagdVecStruct *IritCagdBzrCrvNormalToData(const CagdCrvStruct *Crv,
					  CagdRType t,
					  CagdBType Normalize,
					  CagdVecStruct	*N);
CagdVecStruct *IritCagdBzrCrvNormalMalloc(const CagdCrvStruct *Crv,
					  CagdRType t,
					  CagdBType Normalize);
CagdPolylineStruct *IritCagdBzrCrv2Polyline(const CagdCrvStruct *Crv,
					    int	SamplesPerCurve);
CagdBType IritCagdBzrCrvInterp2(IrtRType *Result,
				const IrtRType *Input,
				int Size);
CagdPtStruct *IritCagdBzrZeroSetNoSubdiv(const CagdCrvStruct *Crv,
					 int Axis,
					 CagdRType NumericTol,
					 CagdRType SubdivTol);

/*****************************************************************************
* Routines to handle Bezier surfaces.					     *
*****************************************************************************/
CagdSrfStruct *IritCagdBzrSrfNew(int ULength,
				 int VLength,
				 CagdPointType PType);
void IritCagdBzrSrfEvalAtParamToData(const CagdSrfStruct *Srf,
				     CagdRType u,
				     CagdRType v,
				     CagdRType *R);
CagdRType *IritCagdBzrSrfEvalAtParamMalloc(const CagdSrfStruct *Srf,
					   CagdRType u,
					   CagdRType v);
CagdCrvStruct *IritCagdBzrSrfCrvFromSrf(const CagdSrfStruct *Srf,
					CagdRType t,
					CagdSrfDirType Dir);
CagdCrvStruct *IritCagdBzrSrfCrvFromMesh(const CagdSrfStruct *Srf,
					 int Index,
					 CagdSrfDirType	Dir);
void IritCagdBzrSrfSubdivCtlMesh(CagdRType * const *Points,
				 CagdRType **LPoints,
				 CagdRType **RPoints,
				 int ULength,
				 int VLength,
				 CagdPointType PType,
				 CagdRType t,
				 CagdSrfDirType	Dir);
CagdSrfStruct *IritCagdBzrSrfSubdivAtParam(const CagdSrfStruct *Srf,
					   CagdRType t,
					   CagdSrfDirType Dir);
CagdSrfStruct *IritCagdBzrSrfDegreeRaise(const CagdSrfStruct *Srf,
					 CagdSrfDirType Dir);
CagdSrfStruct *IritCagdBzrSrfDegreeRaiseN(const CagdSrfStruct *Srf,
					  int NewUOrder,
					  int NewVOrder);
CagdSrfStruct *IritCagdBzrSrfDerive(const CagdSrfStruct *Srf,
				    CagdSrfDirType Dir,
				    CagdBType DeriveScalar);
CagdSrfStruct *IritCagdBzrSrfDeriveScalar(const CagdSrfStruct *Srf,
					  CagdSrfDirType Dir);
CagdSrfStruct *IritCagdBzrSrfIntegrate(const CagdSrfStruct *Srf,
				       CagdSrfDirType Dir);
CagdSrfStruct *IritCagdBzrSrfMoebiusTransform(const CagdSrfStruct *Srf,
					      CagdRType	c,
					      CagdSrfDirType Dir);
CagdVecStruct *IritCagdBzrSrfTangentToData(const CagdSrfStruct *Srf,
					   CagdRType u,
					   CagdRType v,
					   CagdSrfDirType Dir,
					   CagdBType Normalize,
					   CagdVecStruct *Tan);
CagdVecStruct *IritCagdBzrSrfNormalToData(const CagdSrfStruct *Srf,
					  CagdRType u,
					  CagdRType v,
					  CagdBType Normalize,
					  CagdVecStruct	*Normal);
CagdVecStruct *IritCagdBzrSrfNormalMalloc(const CagdSrfStruct *Srf,
					  CagdRType u,
					  CagdRType v,
					  CagdBType Normalize);
CagdVecStruct *IritCagdBzrSrfMeshNormals(const CagdSrfStruct *Srf,
					 int UFineNess,
					 int VFineNess);
CagdBType IritCagdBzrSrf2PolygonsSamples(const CagdSrfStruct *Srf,
					 int FineNess,
					 CagdBType ComputeNrmls,
					 CagdBType ComputeUVs,
					 CagdRType **PtWeights,
					 CagdPtStruct **PtMesh,
					 CagdVecStruct **PtNrml,
					 CagdUVStruct **UVMesh,
					 int *FineNessU,
					 int *FineNessV);
CagdBType IritCagdBzrSrf2PolygonsSamplesNuNv(const CagdSrfStruct *Srf,
					     int Nu,
					     int Nv,
					     CagdBType ComputeNrmls,
					     CagdBType ComputeUVs,
					     CagdRType **PtWeights,
					     CagdPtStruct **PtMesh,
					     CagdVecStruct **PtNrml,
					     CagdUVStruct **UVMesh);
struct IPPolygonStruct *IritCagdBzrSrf2Polygons(
					 const CagdSrfStruct *Srf,
					 const CagdSrf2PlsInfoStrct *TessInfo);
struct IPPolygonStruct *IritCagdBzrSrf2PolygonsN(const CagdSrfStruct *Srf,
						 int Nu,
						 int Nv,
						 CagdBType ComputeNrmls,
						 CagdBType FourPerFlat,
						 CagdBType ComputeUVs);
CagdPolylineStruct *IritCagdBzrSrf2Polylines(const CagdSrfStruct *Srf,
					     int NumOfIsocurves[2],
					     int SamplesPerCurve);
CagdCrvStruct *IritCagdBzrSrf2Curves(const CagdSrfStruct *Srf,
				     int NumOfIsocurves[2]);

/*****************************************************************************
* Routines to handle Bspline knot vectors.				     *
*****************************************************************************/
CagdBType IritCagdBspCrvHasBezierKV(const CagdCrvStruct *Crv);
CagdBType IritCagdBspSrfHasBezierKVs(const CagdSrfStruct *Srf);
CagdBType IritCagdBspKnotHasBezierKV(const CagdRType *KnotVector,
				     int Len,
				     int Order);
CagdBType IritCagdBspCrvHasOpenEC(const CagdCrvStruct *Crv);
CagdBType IritCagdBspSrfHasOpenEC(const CagdSrfStruct *Srf);
CagdBType IritCagdBspSrfHasOpenECDir(const CagdSrfStruct *Srf,
				     CagdSrfDirType Dir);
CagdBType IritCagdBspKnotHasOpenEC(const CagdRType *KnotVector,
				   int Len,
				   int Order);
CagdBType IritCagdBspKnotParamInDomain(const CagdRType *KnotVector,
				       int Len,
				       int Order,
				       CagdBType Periodic,
				       CagdRType t);
int IritCagdBspKnotLastIndexLE(const CagdRType *KnotVector,
			       int Len,
			       CagdRType t,
			       CagdRType Tol);
int IritCagdBspKnotLastIndexL(const CagdRType *KnotVector,
			      int Len,
			      CagdRType	t,
			      CagdRType	Tol);
int IritCagdBspKnotFirstIndexG(const CagdRType *KnotVector,
			       int Len,
			       CagdRType t,
			       CagdRType Tol);
int BspKnotMultiplicity(const CagdRType *KnotVector, int Len, int Idx);
CagdRType *IritCagdBspKnotUniformPeriodic(int Len,
					  int Order,
					  CagdRType *KnotVector);
CagdRType *IritCagdBspKnotUniformFloat(int Len,
				       int Order,
				       CagdRType *KnotVector);
CagdRType *IritCagdBspKnotUniformOpen(int Len,
				      int Order,
				      CagdRType *KnotVector);
CagdRType *IritCagdBspKnotDiscontUniformOpen(int Len,
					     int Order,
					     CagdRType *KnotVector);
CagdEndConditionType IritCagdBspIsKnotUniform(int Len,
					      int Order,
					      const CagdRType *KnotVector);
CagdEndConditionType IritCagdBspIsKnotDiscontUniform(
						 int Len,
						 int Order,
						 const CagdRType *KnotVector);
CagdRType *IritCagdBspKnotDegreeRaisedKV(const CagdRType *KV,
					 int Len,
					 int Order,
					 int NewOrder,
					 int *NewLen);
CagdRType *IritCagdBspKnotSubtrTwo(const CagdRType *KnotVector1,
				   int Len1,
				   const CagdRType *KnotVector2,
				   int Len2,
				   int *NewLen);
CagdRType *IritCagdBspKnotMergeTwo(const CagdRType *KnotVector1,
				   int Len1,
				   const CagdRType *KnotVector2,
				   int Len2,
				   int Mult,
				   int *NewLen);
CagdRType *IritCagdBspKnotContinuityMergeTwo(const CagdRType *KnotVector1,
					     int Len1,
					     int Order1,
					     const CagdRType *KnotVector2,
					     int Len2,
					     int Order2,
					     int ResOrder,
					     int *NewLen);
CagdRType *IritCagdBspKnotDoubleKnots(const CagdRType *KnotVector, 
				      int *Len,
				      int Order);
CagdRType *IritCagdBspKnotAverage(const CagdRType *KnotVector,
				  int Len,
				  int Ave);
CagdRType *IritCagdBspKnotNodes(const CagdRType *KnotVector,
				int Len,
				int Order);
CagdRType IritCagdBspKnotNode(const CagdRType *KnotVector, int i, int Order);
CagdRType *IritCagdBspKnotPeriodicNodes(const CagdRType *KnotVector,
					int Len,
					int Order);
CagdRType IritCagdBspCrvMaxCoefParam(const CagdCrvStruct *Crv,
				     int Axis,
				     CagdRType *MaxVal);
CagdRType *IritCagdBspSrfMaxCoefParamToData(const CagdSrfStruct *Srf,
					    int	Axis,
					    CagdRType *MaxVal,
					    CagdRType *UV);
CagdRType *IritCagdBspSrfMaxCoefParamMalloc(const CagdSrfStruct *Srf,
					    int	Axis,
					    CagdRType *MaxVal);
CagdRType *IritCagdBspKnotPrepEquallySpaced(const CagdRType *KV,
					    int	KVLen,
					    int	KVOrder,
					    int	*n,
					    CagdRType Tmin,
					    CagdRType Tmax);
CagdRType *IritCagdBspKnotReverse(const CagdRType *KnotVector, int Len);
void IritCagdBspKnotScale(CagdRType *KnotVector, int Len, CagdRType Scale);
void IritCagdBspKnotTranslate(CagdRType *KnotVector, int Len, CagdRType Trans);
void IritCagdBspKnotAffineTrans(CagdRType *KnotVector,
				int Len,
				CagdRType Translate,
				CagdRType Scale);
void IritCagdBspKnotAffineTrans2(CagdRType *KnotVector,
				 int Len,
				 CagdRType MinVal,
				 CagdRType MaxVal);
void IritCagdBspKnotAffineTransOrder(CagdRType *KnotVector,
				     int Order,
				     int Len,
				     CagdRType Translate,
				     CagdRType Scale);
void IritCagdBspKnotAffineTransOrder2(CagdRType *KnotVector,
				      int Order,
				      int Len,
				      CagdRType	MinVal,
				      CagdRType	MaxVal);
CagdRType *IritCagdBspKnotCopy(CagdRType *DstKV,
			       const CagdRType *SrcKV,
			       int Len);
BspKnotAlphaCoeffStruct *IritCagdBspKnotEvalAlphaCoef(int k,
						      CagdRType	*KVT,
						      int LengthKVT,
						      CagdRType	*KVt,
						      int LengthKVt,
						      int Periodic);
BspKnotAlphaCoeffStruct *IritCagdBspKnotEvalAlphaCoefMerge(int k,
							   CagdRType *KVT,
							   int LengthKVT,
							   CagdRType *NewKV,
							   int LengthNewKV,
							   int Periodic);
BspKnotAlphaCoeffStruct *IritCagdBspKnotCopyAlphaCoef(
					     const BspKnotAlphaCoeffStruct *A);
void IritCagdBspKnotFreeAlphaCoef(BspKnotAlphaCoeffStruct *A);
void IritCagdBspKnotAlphaLoopBlendNotPeriodic(const BspKnotAlphaCoeffStruct *A,
					      int IMin,
					      int IMax,
					      const CagdRType *OrigPts,
					      CagdRType	*RefPts);
void IritCagdBspKnotAlphaLoopBlendPeriodic(const BspKnotAlphaCoeffStruct *A,
					   int IMin,
					   int IMax,
					   const CagdRType *OrigPts,
					   int OrigLen,	 
					   CagdRType *RefPts);
void IritCagdBspKnotAlphaLoopBlendStep(const BspKnotAlphaCoeffStruct *A,
				       int IMin,
				       int IMax,
				       const CagdRType *OrigPts,
				       int OrigPtsStep,
				       int OrigLen,  
				       CagdRType *RefPts,
				       int RefPtsStep);
CagdRType *IritCagdBspKnotInsertOne(const CagdRType *KnotVector,
				    int	Order,
				    int	Len,
				    CagdRType t);
CagdRType *IritCagdBspKnotInsertMult(const CagdRType *KnotVector,
				     int Order,
				     int *Len,
				     CagdRType t,
				     int Mult);
int IritCagdBspKnotFindMult(const CagdRType *KnotVector,
			    int	Order,
			    int	Len,
			    CagdRType t);
int IritCagdBspKnotsMultiplicityVector(const CagdRType *KnotVector,
				       int Len,
				       CagdRType *KnotValues,
				       int *KnotMultiplicities,
				       CagdRType Eps);
int IritCagdBspKnotMinDmnBzrIdx(const CagdRType *KnotVector,
				int Len,
				int Order,
				CagdRType MinDmn,
				CagdRType Eps);
void IritCagdBspKnotsGetIntervals(const CagdRType *KV,
				  int KVLen,
				  CagdRType **KnotIntervals,
				  int *IntervalCount);
CagdBType IritCagdBspKnotC0Discont(const CagdRType *KnotVector,
				   int Order,
				   int Length,
				   CagdRType *t);
CagdBType IritCagdBspKnotC1Discont(const CagdRType *KnotVector,
				   int Order,
				   int Length,
				   CagdRType *t);
CagdBType IritCagdBspKnotC2Discont(const CagdRType *KnotVector,
				   int Order,
				   int Length,
				   CagdRType *t);
CagdBType IritCagdBspKnotCnDiscont(const CagdRType *KnotVector,
				   int Order,
				   int Length,
				   int n,
				   CagdRType *t);
CagdRType *IritCagdBspKnotAllC0Discont(const CagdRType *KnotVector,
				       int Order,
				       int Length,
				       int *n);
CagdRType *IritCagdBspKnotAllC1Discont(const CagdRType *KnotVector,
				       int Order,
				       int Length,
				       int *n);
CagdRType *IritCagdBspKnotParamValues(CagdRType PMin,
				      CagdRType	PMax,
				      int NumSamples,
				      CagdRType	*C1Disconts,
				      int NumC1Disconts);
CagdBType IritCagdBspKnotMakeRobustKV(CagdRType *KV, int Len);
CagdBType IritCagdBspKnotVectorsSame(const CagdRType *KV1,
				     const CagdRType *KV2,
				     int Len,
				     CagdRType Eps);
void IritCagdBspKnotVerifyPeriodicKV(CagdRType *KV, int Order, int Len);
int IritCagdBspKnotVerifyKVValidity(CagdRType *KV,
				    int Order,
				    int Len, CagdRType Tol);
void IritCagdBspKnotSortKVMonotone(CagdRType *KV, int Len);
CagdBType IritCagdBspVecSpreadEqualItems(CagdRType *Vec,
					 int Len,
					 CagdRType MinDist);
CagdCrvStruct *IritCagdBspGenBasisFuncsAsCurves(int Order,
						int Length,
						const CagdRType	*KV);
CagdCrvStruct *IritCagdBspGenKnotsGeometryAsCurves(int Order,
						   int Length,
						   const CagdRType *KV,
						   CagdRType SizeOfKnot);

/*****************************************************************************
* Routines to handle B-spline curves.					     *
*****************************************************************************/
CagdCrvStruct *IritCagdBspCrvNew(int Length, int Order, CagdPointType PType);
CagdCrvStruct *IritCagdBspPeriodicCrvNew(int Length,
					 int Order,
					 CagdBType Periodic,
					 CagdPointType PType);
void IritCagdBspCrvDomain(const CagdCrvStruct *Crv,
			  CagdRType *TMin,
			  CagdRType *TMax);

#define BSP_CRV_COX_DB_BASIS(KnotVector, Order, Len, Periodic, t, IndexFirst) \
	IritCagdBspCrvCoxDeBoorBasis(KnotVector, Order, Len, Periodic, t, IndexFirst, \
				     (CagdRType *) IritAlloca(Order * \
						           sizeof(CagdRType)))
#define BSP_CRV_COX_DB_BASIS_ALLOC(KnotVector, Order, Len, Periodic, t, \
				   IndexFirst, Data) \
	IritCagdBspCrvCoxDeBoorBasis(KnotVector, Order, Len, Periodic, t, IndexFirst, \
				     Data)
CagdRType *IritCagdBspCrvCoxDeBoorBasis(const CagdRType *KnotVector,
					int Order,
					int Len,
					CagdBType Periodic,
					CagdRType t,
					int *IndexFirst,
					CagdRType *Basis);
int IritCagdBspCrvCoxDeBoorIndexFirst(const CagdRType *KnotVector,
				      int Order,
				      int Len,
				      CagdRType	t);
void IritCagdBspCrvEvalCoxDeBoorToData(const CagdCrvStruct *Crv,
				       CagdRType t,
				       CagdRType *Pt);
CagdRType *IritCagdBspCrvEvalCoxDeBoorMalloc(const CagdCrvStruct *Crv,
					     CagdRType t);
CagdBspBasisFuncEvalStruct *IritCagdBspBasisFuncMultEval(
			     const CagdRType *KnotVector,
			     int KVLength,
			     int Order,
			     CagdBType Periodic,
			     CagdRType *Params,
			     int NumOfParams,
			      CagdIritCagdBspBasisFuncMultEvalType EvalType);
void IritCagdBspBasisFuncMultEvalPrint(const CagdBspBasisFuncEvalStruct *Evals,
				       int Order,
				       CagdRType *Params,
				       int NumOfParams);
void IritCagdBspBasisFuncMultEvalFree(CagdBspBasisFuncEvalStruct *Evals,
				      int NumOfEvals);
CagdRType IritCagdBspCrvEvalVecAtParam(const CagdRType *Vec,
				       int VecInc,
				       const CagdRType *KnotVector,
				       int Order,
				       int Len,
				       CagdBType Periodic,
				       CagdRType t);
void IritCagdBspCrvEvalAtParamToData(const CagdCrvStruct *Crv,
				     CagdRType t,
				     CagdRType *R);
CagdRType *IritCagdBspCrvEvalAtParamMalloc(const CagdCrvStruct *Crv,
					   CagdRType t);
CagdCrvStruct *IritCagdBspCrvCreateCircle(const CagdPtStruct *Center,
					  CagdRType Radius);
CagdCrvStruct *IritCagdBspCrvCreateUnitCircle(void);
CagdCrvStruct *IritCagdBspCrvCreatePCircle(const CagdPtStruct *Center,
					   CagdRType Radius);
CagdCrvStruct *IritCagdBspCrvCreateUnitPCircle(void);
CagdCrvStruct *IritCagdBspCrvCreateUnitPCircleQuadTol(CagdRType Tol, int Cont);
CagdCrvStruct *IritCagdBspCrvCreateUnitPCircleCubicTol(CagdRType Tol, int Cont);
CagdCrvStruct *IritCagdBspCrvCreatePCircleTol(const CagdPtStruct *Center,
					      CagdRType	Radius,
					      int Order,
					      int Cont,
					      CagdRType	Tol);
CagdCrvStruct *IritCagdBspCrvCreateApproxSpiral(CagdRType NumOfLoops,
						CagdRType Pitch,
						int Sampling,
						int CtlPtsPerLoop);
CagdCrvStruct *IritCagdBspCrvCreateApproxHelix(CagdRType NumOfLoops,
					       CagdRType Pitch,
					       CagdRType Radius,
					       int Sampling,
					       int CtlPtsPerLoop);
CagdCrvStruct *IritCagdBspCrvCreateApproxSine(CagdRType NumOfCycles,
					      int Sampling,
					      int CtlPtsPerCycle);
CagdCrvStruct *IritCagdBspCrvCreateZigZagCrv(CagdRType NumOfCycles,
					     CagdRType ArcsRadius,
					     CagdRType EndSpacings);
CagdCrvStruct *IritCagdBspCrvCreateApproxZigZagCrv(CagdRType NumOfCycles,
						   CagdRType ArcsRadius,
						   CagdRType EndSpacings);
CagdCrvStruct *IritCagdBspCrvCreateRectangle(CagdRType X1,
					     CagdRType Y1,
					     CagdRType X2,
					     CagdRType Y2);
CagdCrvStruct *IritCagdBspCrvKnotInsert(const CagdCrvStruct *Crv, CagdRType t);
CagdCrvStruct *IritCagdBspCrvKnotInsertNSame(const CagdCrvStruct *Crv,
					     CagdRType t,
					     int n);
CagdCrvStruct *IritCagdBspCrvKnotInsertNDiff(const CagdCrvStruct *Crv,
					     CagdBType Replace,
					     CagdRType *t,
					     int n);
void IritCagdBspCrvSubdivCtlPoly(const CagdCrvStruct *Crv,
				 CagdRType **LPoints,
				 CagdRType **RPoints,
				 int LLength,
				 int RLength,
				 CagdRType t,
				 int Mult);
CagdCrvStruct *IritCagdBspCrvSubdivAtParam(const CagdCrvStruct *Crv,
					   CagdRType t);
CagdCrvStruct *IritCagdBspCrvOpenEnd(const CagdCrvStruct *Crv);
CagdBType IritCagdBspCrvKnotC0Discont(const CagdCrvStruct *Crv, CagdRType *t);
CagdBType IritCagdBspCrvKnotC1Discont(const CagdCrvStruct *Crv, CagdRType *t);
CagdBType IritCagdBspCrvKnotC2Discont(const CagdCrvStruct *Crv, CagdRType *t);
CagdBType IritCagdBspMeshC1PtsCollinear(const CagdPType Pt0,
					const CagdPType	Pt1,
					const CagdPType	Pt2,
					CagdRType *LenRatio);
CagdBType IritCagdBspCrvMeshC0Continuous(const CagdCrvStruct *Crv,
					 int Idx,
					 CagdRType Tol);
CagdBType IritCagdBspCrvMeshC1Continuous(const CagdCrvStruct *Crv,
					 int Idx,
					 CagdRType *CosAngle);
CagdBType IritCagdBspCrvIsC1DiscontAt(const CagdCrvStruct *Crv, CagdRType t);
CagdCrvStruct *IritCagdBspCrvDegreeRaise(const CagdCrvStruct *Crv);
CagdCrvStruct *IritCagdBspCrvDegreeRaiseN(const CagdCrvStruct *Crv,
					  int NewOrder);
CagdCrvStruct *IritCagdBspCrvDerive(const CagdCrvStruct *Crv,
				    CagdBType DeriveScalar);
CagdCrvStruct *IritCagdBspCrvDeriveScalar(const CagdCrvStruct *Crv);
CagdCrvStruct *IritCagdBspCrvIntegrate(const CagdCrvStruct *Crv);
CagdCrvStruct *IritCagdBspCrvMoebiusTransform(const CagdCrvStruct *Crv,
					      CagdRType c);
CagdVecStruct *IritCagdBspCrvTangentToData(const CagdCrvStruct *Crv,
					   CagdRType t,
					   CagdBType Normalize,
					   CagdVecStruct *Tan);
CagdVecStruct *IritCagdBspCrvBiNormalToData(const CagdCrvStruct *Crv,
					    CagdRType t,
					    CagdBType Normalize,
					    CagdVecStruct *Vec);
CagdVecStruct *IritCagdBspCrvBiNormalMalloc(const CagdCrvStruct *Crv,
					    CagdRType t,
					    CagdBType Normalize);
CagdVecStruct *IritCagdBspCrvNormalToData(const CagdCrvStruct *Crv,
					  CagdRType t,
					  CagdBType Normalize,
					  CagdVecStruct	*N);
CagdVecStruct *IritCagdBspCrvNormalMalloc(const CagdCrvStruct *Crv,
					  CagdRType t,
					  CagdBType Normalize);
CagdPolylineStruct *IritCagdBspCrv2Polyline(const CagdCrvStruct *Crv,
					    int	SamplesPerCurve,
					    BspKnotAlphaCoeffStruct *A,
					    CagdBType OptiLin);
CagdCrvStruct *IritCagdBspCrvInterpPts(const CagdPtStruct *PtList,
				       int Order,
				       int CrvSize,
				       CagdParametrizationType ParamType,
				       CagdBType Periodic);
CagdCrvStruct *IritCagdBspCrvInterpPts2(const CagdCtlPtStruct *PtList,
					int Order,
					int CrvSize,
					CagdParametrizationType	ParamType,
					CagdBType Periodic,
					CagdBType EndPtInterp);
CagdCrvStruct *IritCagdBspCrvInterpPts3(const CagdCtlPtStruct *PtList,
					const CagdRType	*Params,
					const CagdRType	*KV,
					int Length,
					int Order,
					CagdBType Periodic);
void IritCagdBspCrvInterpBuildKVs(const CagdCtlPtStruct *PtList,
				  int Order,
				  int CrvSize,
				  CagdParametrizationType ParamType,
				  CagdBType Periodic,
				  CagdRType **RetPtKnots,
				  CagdRType **RetKV);
CagdCrvStruct *IritCagdBspCrvInterpolate(const CagdCtlPtStruct *PtList,
					 const CagdRType *Params,
					 const CagdRType *KV,
					 int Length,
					 int Order,
					 CagdBType Periodic);
CagdCrvStruct *IritCagdBspCrvFitLstSqr(const CagdCrvStruct *Crv,
				       int Order,
				       int Size,
				       CagdBType Periodic,
				       CagdParametrizationType ParamType,
				       CagdBType EndPtInterp,
				       CagdBType EvalPts,
				       CagdRType *Err);
CagdRType *IritCagdBspPtSamplesToKV(const CagdRType *PtsSamples,
				    int	NumPts,
				    int	CrvOrder,
				    int	CrvLength);
CagdRType IritCagdBspCrvInterpPtsError(const CagdCrvStruct *Crv,
				       const CagdPtStruct *PtList,
				       CagdParametrizationType ParamType,
				       CagdBType Periodic);
CagdCrvStruct *IritCagdCrvQuadDirectInterp(const CagdCrvStruct *Crv,
					   CagdRType InflectStretch);
CagdCrvStruct *IritCagdBspMakeReparamCurve(const CagdPtStruct *PtsList,
					   int Order,
					   int DegOfFreedom);
CagdRType IritCagdLineFitToPts(CagdPtStruct *PtList,
			       CagdVType LineDir,
			       CagdPType LinePos);
CagdRType IritCagdPlaneFitToPts(const CagdPtStruct *PtList,
				IrtPlnType Pln,
				IrtVecType MVec,
				IrtPtType Cntr,
				IrtRType *CN);
CagdRType IritCagdPlaneFitToPts2(CagdRType * const *Points,
				 int NumPts,
				 CagdPointType PType,
				 IrtPlnType Pln,
				 IrtVecType MVec,
				 IrtPtType Cntr,
				 IrtRType *CN);
CagdRType IritCagdPlaneFitToPts3(CagdPType * const Points,
				 int NumPts,
				 IrtPlnType Pln,
				 IrtVecType MVec,
				 IrtPtType Cntr,
				 IrtRType *CN);
void IritCagdBspReparameterizeCrv(CagdCrvStruct *Crv,
				  CagdParametrizationType ParamType);
CagdCrvStruct *IritCagdBspCrvExtensionOneSide(const CagdCrvStruct *OrigCrv,
					      CagdBType	MinDmn,
					      CagdRType	Epsilon,
					      CagdRType	ExtntScl,
					      CagdBType	RemoveExtraKnots);
CagdCrvStruct *IritCagdBspCrvExtension(const CagdCrvStruct *OrigCrv,
				       const CagdBType *ExtDirs,
				       CagdRType Epsilon,
				       CagdBType RemoveExtraKnots);

CagdCrvStruct *IritCagdBspCrvExtraKnotRmv(const CagdCrvStruct *Crv,
					  int RmvIndex);

/*****************************************************************************
* Routines to handle Bspline surfaces.					     *
*****************************************************************************/
CagdSrfStruct *IritCagdBspSrfNew(int ULength,
				 int VLength,
				 int UOrder,
				 int VOrder,
				 CagdPointType PType);
CagdSrfStruct *IritCagdBspPeriodicSrfNew(int ULength,
					 int VLength,
					 int UOrder,
					 int VOrder,
					 CagdBType UPeriodic,
					 CagdBType VPeriodic,
					 CagdPointType PType);
void IritCagdBspSrfDomain(const CagdSrfStruct *Srf,
			  CagdRType *UMin,
			  CagdRType *UMax,
			  CagdRType *VMin,
			  CagdRType *VMax);
void IritCagdBspSrfEvalAtParamToData(const CagdSrfStruct *Srf,
				     CagdRType u,
				     CagdRType v,
				     CagdRType *R);
CagdRType *IritCagdBspSrfEvalAtParamMalloc(const CagdSrfStruct *Srf,
					   CagdRType u,
					   CagdRType v,
					   void	**Cache);
CagdCrvStruct *IritCagdBspSrfCrvFromSrf(const CagdSrfStruct *Srf,
					CagdRType t,
					CagdSrfDirType Dir);
CagdCrvStruct *IritCagdBspSrfC1DiscontCrvs(const CagdSrfStruct *Srf);
CagdBType IritCagdBspSrfHasC1Discont(const CagdSrfStruct *Srf, int E3C1Discont);
CagdBType IritCagdBspSrfIsC1DiscontAt(const CagdSrfStruct *Srf,
				      CagdSrfDirType Dir,
				      CagdRType	t);
CagdCrvStruct *IritCagdBspSrfCrvFromMesh(const CagdSrfStruct *Srf,
					 int Index,
					 CagdSrfDirType	Dir);
CagdSrfStruct *IritCagdBspSrfKnotInsert(const CagdSrfStruct *Srf,
					CagdSrfDirType Dir,
					CagdRType t);
CagdSrfStruct *IritCagdBspSrfKnotInsertNSame(const CagdSrfStruct *Srf,
					     CagdSrfDirType Dir,
					     CagdRType t, int n);
CagdSrfStruct *IritCagdBspSrfKnotInsertNDiff(const CagdSrfStruct *Srf,
					     CagdSrfDirType Dir,
					     CagdBType Replace,
					     CagdRType *t,
					     int n);
CagdSrfStruct *IritCagdBspSrfSubdivAtParam(const CagdSrfStruct *Srf,
					   CagdRType t,
					   CagdSrfDirType Dir);
CagdSrfStruct *IritCagdBspSrfOpenEnd(const CagdSrfStruct *Srf);
CagdBType IritCagdBspSrfKnotC0Discont(const CagdSrfStruct *Srf,
				      CagdSrfDirType Dir,
				      CagdRType	*t);
CagdBType IritCagdBspSrfKnotC1Discont(const CagdSrfStruct *Srf,
				      CagdSrfDirType Dir,
				      CagdRType	*t);
CagdBType IritCagdBspSrfMeshC1Continuous(const CagdSrfStruct *Srf,
					 CagdSrfDirType	Dir,
					 int Idx);
CagdSrfStruct *IritCagdBspSrfDegreeRaise(const CagdSrfStruct *Srf,
					 CagdSrfDirType Dir);
CagdSrfStruct *IritCagdBspSrfDegreeRaiseN(const CagdSrfStruct *Srf,
					  int NewUOrder,
					  int NewVOrder);
CagdSrfStruct *IritCagdBspSrfDerive(const CagdSrfStruct *Srf,
				    CagdSrfDirType Dir,
				    CagdBType DeriveScalar);
CagdSrfStruct *IritCagdBspSrfDeriveScalar(const CagdSrfStruct *Srf,
					  CagdSrfDirType Dir);
CagdSrfStruct *IritCagdBspSrfIntegrate(const CagdSrfStruct *Srf,
				       CagdSrfDirType Dir);
CagdSrfStruct *IritCagdBspSrfMoebiusTransform(const CagdSrfStruct *Srf,
					      CagdRType	c,
					      CagdSrfDirType Dir);
CagdVecStruct *IritCagdBspSrfTangentToData(const CagdSrfStruct *Srf,
					   CagdRType u,
					   CagdRType v,
					   CagdSrfDirType Dir,
					   CagdBType Normalize,
					   CagdVecStruct *Tan);
CagdVecStruct *IritCagdBspSrfNormalToData(const CagdSrfStruct *Srf,
					  CagdRType u,
					  CagdRType v,
					  CagdBType Normalize,
					  CagdVecStruct	*Normal);
CagdVecStruct *IritCagdBspSrfNormalMalloc(const CagdSrfStruct *Srf,
					  CagdRType u,
					  CagdRType v,
					  CagdBType Normalize);
CagdVecStruct *IritCagdBspSrfMeshNormals(const CagdSrfStruct *Srf,
					 int UFineNess,
					 int VFineNess);
IritCagdSrfErrorFuncType IritCagdBspSrf2PolygonSetErrFunc(
					       IritCagdSrfErrorFuncType Func,
					       void *Data);
struct IPPolygonStruct *IritCagdBspSrf2Polygons(
					const CagdSrfStruct *Srf,
					const CagdSrf2PlsInfoStrct *TessInfo);
struct IPPolygonStruct *IritCagdBspSrf2PolygonsN(const CagdSrfStruct *Srf,
						 int Nu,
						 int Nv,
						 CagdBType ComputeNrmsals,
						 CagdBType FourPerFlat,
						 CagdBType ComputeUVs);
CagdBType IritCagdBspSrf2PolygonsSamplesNuNv(const CagdSrfStruct *Srf,
					     int Nu,
					     int Nv,
					     CagdBType ComputeNrmls,
					     CagdBType ComputeUVs,
					     CagdRType **PtWeights,
					     CagdPtStruct **PtMesh,
					     CagdVecStruct **PtNrml,
					     CagdUVStruct **UVMesh);
CagdBType IritCagdBspC1Srf2PolygonsSamples(const CagdSrfStruct *Srf,
					   int FineNess,
					   CagdBType ComputeNrmls,
					   CagdBType ComputeUVs,
					   CagdRType **PtWeights,
					   CagdPtStruct	**PtMesh,
					   CagdVecStruct **PtNrml,
					   CagdUVStruct	**UVMesh,
					   int *FineNessU,
					   int *FineNessV);
CagdPolylineStruct *IritCagdBspSrf2Polylines(const CagdSrfStruct *Srf,
					     int NumOfIsocurves[2],
					     int SamplesPerCurve);
CagdCrvStruct *IritCagdBspSrf2Curves(const CagdSrfStruct *Srf,
				     int NumOfIsocurves[2]);
CagdSrfStruct *IritCagdBspSrfInterpPts(const CagdPtStruct **PtList,
				       int UOrder,
				       int VOrder,
				       int SrfUSize,
				       int SrfVSize,
				       CagdParametrizationType ParamType);
CagdSrfStruct *IritCagdBspSrfInterpolate(const CagdCtlPtStruct *PtList,
					 int NumUPts,
					 int NumVPts,
					 const CagdRType *UParams,
					 const CagdRType *VParams,
					 const CagdRType *UKV,
					 const CagdRType *VKV,
					 int ULength,
					 int VLength,
					 int UOrder,
					 int VOrder);
CagdSrfStruct *IritCagdBspSrfFitLstSqr(const CagdSrfStruct *Srf,
				       int UOrder,
				       int VOrder,
				       int USize,
				       int VSize, 
				       CagdParametrizationType ParamType,
				       CagdRType *Err);
CagdSrfStruct *IritCagdBspSrfInterpScatPts(const CagdCtlPtStruct *PtList,
					   int UOrder,
					   int VOrder,
					   int USize,
					   int VSize,
					   CagdRType *UKV,
					   CagdRType *VKV);
CagdSrfStruct *IritCagdBspSrfInterpScatPtsC0Bndry(
						const CagdCtlPtStruct *PtList,
						const CagdCrvStruct *UMinCrv,
						const CagdCrvStruct *UMaxCrv,
						const CagdCrvStruct *VMinCrv,
						const CagdCrvStruct *VMaxCrv);
CagdSrfStruct *IritCagdBspSrfInterpScatPts2(const CagdCtlPtStruct *PtList,
					    int	UOrder,
					    int	VOrder,
					    int	USize,
					    int	VSize,
					    CagdRType *UKV,
					    CagdRType *VKV,
					    CagdRType *MatrixCondition);
void IritCagdBspReparameterizeSrf(CagdSrfStruct *Srf,
				  CagdSrfDirType Dir,
				  CagdParametrizationType ParamType);
CagdSrfStruct *IritCagdBspSrfExtension(const CagdSrfStruct *OrigSrf,
				       const CagdBType *ExtDirs,
				       CagdRType EpsilonU,
				       CagdRType EpsilonV,
				       CagdBType RemoveExtraKnots);

/*****************************************************************************
* Routines to construct	primitive objects.				     *
*****************************************************************************/
CagdCrvStruct *IritCagdPrimRectangleCrv(CagdRType MinX,
					CagdRType MinY,
					CagdRType MaxX,
					CagdRType MaxY,
					CagdRType ZLevel);
CagdCrvStruct *IritCagdPrimLinCrvFrom4Pts(const CagdPType P1,
					  const	CagdPType P2,
					  const	CagdPType P3,
					  const	CagdPType P4,
					  CagdBType Closed);
CagdSrfStruct *IritCagdPrimPlaneSrf(CagdRType MinX,
				    CagdRType MinY,
				    CagdRType MaxX,
				    CagdRType MaxY,
				    CagdRType ZLevel);
CagdSrfStruct *IritCagdPrimPlaneSrf1(CagdRType P00X,
				     CagdRType P00Y,
				     CagdRType P01X,
				     CagdRType P01Y,
				     CagdRType P10X,
				     CagdRType P10Y,
				     CagdRType P11X,
				     CagdRType P11Y,
				     CagdRType ZLevel);
CagdSrfStruct *IritCagdPrimPlaneSrf2(CagdPType Cntr,
				     CagdVType Vec1,
				     CagdVType Vec2);
CagdSrfStruct *IritCagdPrimPlaneFromE3Crv(const CagdCrvStruct *Crv,
					  const	IrtPlnType Plane);
CagdSrfStruct *IritCagdPrimPlaneSrfOrderLen(CagdRType MinX,
					    CagdRType MinY,
					    CagdRType MaxX,
					    CagdRType MaxY,
					    CagdRType ZLevel,
					    int	Order,
					    int	Len);
CagdSrfStruct *IritCagdPrimPlaneXZSrf(CagdRType MinX,
				      CagdRType MinZ,
				      CagdRType MaxX,
				      CagdRType MaxZ,
				      CagdRType YLevel);
CagdSrfStruct *IritCagdPrimPlaneYZSrf(CagdRType MinY,
				      CagdRType MinZ,
				      CagdRType MaxY,
				      CagdRType MaxZ,
				      CagdRType XLevel);
CagdSrfStruct *IritCagdPrimPlanePlaneSpanBBox(const CagdBBoxStruct *BBox,
					      const IrtPlnType Pln);
CagdSrfStruct *IritCagdPrimBoxSrf(CagdRType MinX,
				  CagdRType MinY,
				  CagdRType MinZ,
				  CagdRType MaxX,
				  CagdRType MaxY,
				  CagdRType MaxZ);
CagdSrfStruct *IritCagdPrimSphereSrf(const CagdVType Center,
				     CagdRType Radius,
				     CagdBType Rational);
CagdSrfStruct *IritCagdPrimCubeSphereSrf(const CagdVType Center,
					 CagdRType Radius,
					 CagdBType Rational);
CagdSrfStruct *IritCagdPrimTorusSrf(const CagdVType Center,
				    CagdRType MajorRadius,
				    CagdRType MinorRadius,
				    CagdBType Rational);
CagdSrfStruct *IritCagdPrimCone2Srf(const CagdVType Center,
				    CagdRType MajorRadius,
				    CagdRType MinorRadius,
				    CagdRType Height,
				    CagdBType Rational,
				    CagdPrimCapsType Caps);
CagdSrfStruct *IritCagdPrimConeSrf(const CagdVType Center,
				   CagdRType Radius,
				   CagdRType Height,
				   CagdBType Rational,
				   CagdPrimCapsType Caps);
CagdSrfStruct *IritCagdPrimCylinderSrf(const CagdVType Center,
				       CagdRType Radius,
				       CagdRType Height,
				       CagdBType Rational,
				       CagdPrimCapsType	Caps);

/*****************************************************************************
* Routines to handle basis function conversions.			     *
*****************************************************************************/
CagdCrvStruct *IritCagdCnvrtPwr2BzrCrv(const CagdCrvStruct *Crv);
CagdCrvStruct *IritCagdCnvrtBzr2PwrCrv(const CagdCrvStruct *Crv);
CagdCrvStruct *IritCagdCnvrtBsp2BzrCrv(const CagdCrvStruct *Crv);
CagdCrvStruct *IritCagdCnvrtBzr2BspCrv(const CagdCrvStruct *Crv);

CagdSrfStruct *IritCagdCnvrtPwr2BzrSrf(const CagdSrfStruct *Srf);
CagdSrfStruct *IritCagdCnvrtBzr2PwrSrf(const CagdSrfStruct *Srf);
CagdSrfStruct *IritCagdCnvrtBzr2BspSrf(const CagdSrfStruct *Srf);
CagdSrfStruct *IritCagdCnvrtBsp2BzrSrf(const CagdSrfStruct *CSrf);

CagdCrvStruct *IritCagdCnvrtPeriodic2FloatCrv(const CagdCrvStruct *Crv);
CagdCrvStruct *IritCagdCnvrtFloat2OpenCrv(const CagdCrvStruct *Crv);
CagdCrvStruct *IritCagdCnvrtBsp2OpenCrv(const CagdCrvStruct *Crv);
CagdSrfStruct *IritCagdCnvrtPeriodic2FloatSrf(const CagdSrfStruct *Srf);
CagdSrfStruct *IritCagdCnvrtFloat2OpenSrf(const CagdSrfStruct *Srf);
CagdSrfStruct *IritCagdCnvrtBsp2OpenSrf(const CagdSrfStruct *Srf);

CagdCtlPtStruct *IritCagdCnvrtCrvToCtlPts(const CagdCrvStruct *Crv);

CagdPolylineStruct *IritCagdCnvrtPtList2Polyline(const CagdPtStruct *Pts,
						 CagdPolylineStruct **Params);
CagdPtStruct *IritCagdCnvrtPolyline2PtList(const CagdPolylineStruct *Poly);
CagdCrvStruct *IritCagdCnvrtPolyline2LinBspCrv(const CagdPolylineStruct *Poly,
					       CagdPointType PType);
CagdCrvStruct *IritCagdCnvrtPolyline2LinBspCrv2(const CagdPolylineStruct *Poly,
						CagdPointType PType,
						CagdBType SkipIdenticalPoints);
CagdPolylineStruct *IritCagdCnvrtLinIritCagdBspCrv2Polyline(
						    const CagdCrvStruct *Crv,
						    CagdBType FilterIdentical);

/*****************************************************************************
* Routines to handle adaptive forward differencing basis functions.	     *
*****************************************************************************/
void IritCagdAfdCnvrtCubicBzrToAfd(CagdRType Coef[4]);
void IritCagdAfdApplyEStep(CagdRType Coef[4]);
void IritCagdAfdApplyLn(CagdRType Coef[4], int n);
void IritCagdAfdApplyLStep(CagdRType Coef[4]);
void IritCagdAfdApplyAntiLStep(CagdRType Coef[4]);
void IritCagdAfdComputePolyline(CagdRType Coef[4],
				CagdRType *Poly,
				int Log2Step,
				CagdBType NonAdaptive);
void AfdIritCagdBzrCrvEvalToPolyline(const CagdCrvStruct *Crv,
				     int FineNess,
				     CagdRType *Points[]);

/*****************************************************************************
* Routines to handle Bezier clipping based ray tracing.			     *
*****************************************************************************/
CagdBType IritCagdRayTraceBzrSrf(CagdPType StPt,
				 CagdVType Dir,
				 const CagdSrfStruct *BzrSrf,
				 CagdUVStruct **IntrPrm,
				 CagdPtStruct **IntrPt);

/*****************************************************************************
* B-Spline SDM fitting algorithm interface functions			     *
*****************************************************************************/
CagdCrvStruct *IritCagdBsplineCrvFittingWithInitCrv(
				         CagdPType *PtList, /* Points cloud. */
					 int NumOfPoints,
					 CagdCrvStruct *InitCrv,
					 CagdBspFittingType AgorithmType,
					 int MaxIter,
					 CagdRType ErrorLimit,
					 CagdRType ErrorChangeLimit,
					 CagdRType Lambda);

CagdCrvStruct *IritCagdBsplineCrvFitting(CagdPType *PtList,    /* Pts cloud. */
					 int NumOfPoints,
					 int Length, 
					 int Order,
					 CagdBType IsPeriodic,
					 CagdBspFittingType AgorithmType,
					 int MaxIter,
					 CagdRType ErrorLimit,
					 CagdRType ErrorChangeLimit,
					 CagdRType Lambda);

/*****************************************************************************
* Curve	untrimming: converting closed curves into tensor product surfaces.   *
*****************************************************************************/

/* Functions from a freeform curve to tensor-product surfaces. */
typedef IrtRType (*CagdQuadSrfWeightFuncType)(
				     const CagdSrfStruct *QuadSrf, 
				     const CagdCrvStruct *BoundaryCrv,
				     const CagdPolylineStruct *SampledPolygon, 
				     const int *VIndices,
				     int numV);
CagdSrfStruct *IritCagdQuadCurveWeightedQuadrangulation(
					 const struct CagdCrvStruct *Crv,
					 CagdQuadSrfWeightFuncType WeightFunc,
					 int ApproxOrder);
CagdSrfStruct *IritCagdQuadCurveListWeightedQuadrangulation(
					  const CagdCrvStruct *CrvList,
					  CagdQuadSrfWeightFuncType WeightFunc,
					  int ApproxOrder);
void IritCagdGetPlnrSrfJacobianMinMax(const CagdSrfStruct *Srf, 
				      CagdRType	*JMin,
				      CagdRType	*JMax, 
				      int ComputePrecisely);
struct MvarMVStruct *IritCagdGetPlnrSrfJacobian(const CagdSrfStruct *Srf);

/* Proposed weight functions */
IrtRType IritCagdQuadSrfJacobianWeight(
				     const CagdSrfStruct *QuadSrf, 
				     const CagdCrvStruct *BoundaryCrv,
				     const CagdPolylineStruct *SampledPolygon, 
				     const int *VIndices, int numV);
IrtRType IritCagdQuadSrfConformWeight(
				     const CagdSrfStruct *QuadSrf, 
				     const CagdCrvStruct *BoundaryCrv,
				     const CagdPolylineStruct *SampledPolygon,
				     const int *VIndices,
				     int numV);
IrtRType IritCagdQuadSrfRegularPolyWeight(
				      const CagdSrfStruct *QuadSrf, 
				      const CagdCrvStruct *BoundaryCrv,
				      const CagdPolylineStruct *SampledPolygon,
				      const int *VIndices,
				      int numV);

/* A weight function that combines the above three functions. */
IrtRType IritCagdQuadSrfCombinedWeight(
				      const CagdSrfStruct *QuadSrf, 
				      const CagdCrvStruct *BoundaryCrv,
				      const CagdPolylineStruct *SampledPolygon,
				      const int *VIndices,
				      int numV);
void IritCagdQuadSetQuadSrfCombinedWeightFactors(IrtRType JacobianFactor,
						 IrtRType ConformFactor,
						 IrtRType RegularityFactor);
void IritCagdQuadGetQuadSrfCombinedWeightFactors(IrtRType *JacobianFactor,
						 IrtRType *ConformFactor,
						 IrtRType *RegularityFactor);

/* Line-sweep algorithm for partitioning into quads: */
CagdCrvQuadTileStruct *IritCagdQuadCrvToQuadsLineSweep(
						    const CagdCrvStruct *Crv,
						    CagdBType OutputSrfs);
CagdCrvQuadTileStruct *IritCagdCrvQuadTileCopy(
					   const CagdCrvQuadTileStruct *Tile);
CagdCrvQuadTileStruct *IritCagdCrvQuadTileCopyList(
					   const CagdCrvQuadTileStruct *Tiles);
void IritCagdCrvQuadTileFree(CagdCrvQuadTileStruct *Tile);
void IritCagdCrvQuadTileFreeList(CagdCrvQuadTileStruct *Tiles);
CagdCrvQuadTileStruct *IritCagdCrvQuadTileAssumeSrf(CagdSrfStruct *Srf);
CagdCrvQuadTileStruct *IritCagdCrvQuadTileFromSrf(const CagdSrfStruct *Srf);

/*****************************************************************************
* Error	handling.							     *
*****************************************************************************/
CagdSetErrorFuncType IritCagdSetFatalErrorFunc(CagdSetErrorFuncType ErrorFunc);
void IritCagdFatalError(IritCagdFatalErrorType ErrID);
const char *IritCagdDescribeError(IritCagdFatalErrorType ErrID);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* IRIT_CAGD_LIB_H */
