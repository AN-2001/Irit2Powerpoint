/*****************************************************************************
* Triv_lib.h - header file for the TRIV	library.			     *
* This header is also the interface header to the world.		     *
******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute	of Technology		     *
******************************************************************************
* Written by Gershon Elber, Sep. 94.					     *
*****************************************************************************/

#ifndef IRIT_TRIV_LIB_H
#define IRIT_TRIV_LIB_H

#include <stdio.h>
#include "inc_irit/irit_sm.h"
#include "inc_irit/miscattr.h"
#include "inc_irit/misc_lib.h"
#include "inc_irit/cagd_lib.h"

#define TV_PT_COPY(PtDest, PtSrc) \
            IRIT_GEN_COPY((char *) (PtDest), (char *) (PtSrc), \
						    4 * sizeof(CagdRType))
#define TV_PLANE_COPY(PlDest, PlSrc) \
	    IRIT_GEN_COPY((char *) (PlDest), (char *) (PlSrc),  \
						    5 * sizeof(CagdRType))
#define TV_PT_SQR_LENGTH(Pt)	(IRIT_SQR(Pt[0]) + IRIT_SQR(Pt[1]) + \
				 IRIT_SQR(Pt[2]) + IRIT_SQR(Pt[3]))

#define TV_PT_LENGTH(Pt)	sqrt(TV_PT_SQR_LENGTH(Pt))

#define TV_PT_RESET(Pt)		IRIT_ZAP_MEM((Pt), 4 * sizeof(IrtRType))

#define TV_PT_NORMALIZE(Pt)	{    CagdRType Size = TV_PT_LENGTH(Pt); \
				     _IRIT_PT_NORMALIZE_MSG_ZERO(Size) \
				     { \
					 Pt[0] /= Size; \
					 Pt[1] /= Size; \
				         Pt[2] /= Size; \
				         Pt[3] /= Size; \
				     } \
				}

#define TV_PT_BLEND(Res, Pt1, Pt2, t) \
				{ Res[0] = Pt1[0] * t + Pt2[0] * (1 - t); \
				  Res[1] = Pt1[1] * t + Pt2[1] * (1 - t); \
				  Res[2] = Pt1[2] * t + Pt2[2] * (1 - t); \
				  Res[3] = Pt1[3] * t + Pt2[3] * (1 - t); \
			        }

#define TV_PT_ADD(Res, Pt1, Pt2) { Res[0] = Pt1[0] + Pt2[0]; \
				   Res[1] = Pt1[1] + Pt2[1]; \
				   Res[2] = Pt1[2] + Pt2[2]; \
				   Res[3] = Pt1[3] + Pt2[3]; \
			         }

#define TV_PT_SUB(Res, Pt1, Pt2) { Res[0] = Pt1[0] - Pt2[0]; \
				   Res[1] = Pt1[1] - Pt2[1]; \
				   Res[2] = Pt1[2] - Pt2[2]; \
				   Res[3] = Pt1[3] - Pt2[3]; \
				 }

#define TV_PT_SWAP(Pt1, Pt2)	{ IRIT_SWAP(CagdRType, Pt1[0], Pt2[0]); \
				  IRIT_SWAP(CagdRType, Pt1[1], Pt2[1]); \
				  IRIT_SWAP(CagdRType, Pt1[2], Pt2[2]); \
				  IRIT_SWAP(CagdRType, Pt1[3], Pt2[3]); \
				}

#define TV_PT_PT_DIST(Pt1, Pt2) sqrt(IRIT_SQR(Pt1[0] - Pt2[0]) + \
				     IRIT_SQR(Pt1[1] - Pt2[1]) + \
				     IRIT_SQR(Pt1[2] - Pt2[2]) + \
				     IRIT_SQR(Pt1[3] - Pt2[3]))

#define TV_PT_PT_DIST_SQR(Pt1, Pt2) (IRIT_SQR(Pt1[0] - Pt2[0]) + \
				     IRIT_SQR(Pt1[1] - Pt2[1]) + \
				     IRIT_SQR(Pt1[2] - Pt2[2]) + \
				     IRIT_SQR(Pt1[3] - Pt2[3]))

#define TV_DOT_PROD(Pt1, Pt2)	(Pt1[0] * Pt2[0] + \
				 Pt1[1] * Pt2[1] + \
				 Pt1[2] * Pt2[2] + \
				 Pt1[3] * Pt2[3])

typedef CagdRType TrivP4DType[4];
typedef CagdRType TrivV4DType[4];
typedef CagdRType TrivUVWType[3];
typedef CagdRType TrivPln4DType[5];

typedef enum {
    TRIV_ERR_DIR_NOT_VALID,
    TRIV_ERR_UNDEF_CRV,
    TRIV_ERR_UNDEF_SRF,
    TRIV_ERR_UNDEF_TRIVAR,
    TRIV_ERR_UNDEF_GEOM,
    TRIV_ERR_UNSUPPORT_PT,
    TRIV_ERR_RATIONAL_NO_SUPPORT,
    TRIV_ERR_RATIONAL_EXPECTED,
    TRIV_ERR_WRONG_ORDER,
    TRIV_ERR_KNOT_NOT_ORDERED,
    TRIV_ERR_NUM_KNOT_MISMATCH,
    TRIV_ERR_INDEX_NOT_IN_MESH,
    TRIV_ERR_POWER_NO_SUPPORT,
    TRIV_ERR_WRONG_DOMAIN,
    TRIV_ERR_INCONS_DOMAIN,
    TRIV_ERR_DIR_NOT_CONST_UVW,
    TRIV_ERR_SCALAR_PT_EXPECTED,
    TRIV_ERR_INVALID_AXIS,
    TRIV_ERR_NO_CLOSED_POLYGON,
    TRIV_ERR_TWO_INTERSECTIONS,
    TRIV_ERR_NO_MATCH_PAIR,
    TRIV_ERR_2_OR_4_INTERS,
    TRIV_ERR_FAIL_FIND_PT,
    TRIV_ERR_FAIL_READ_FILE,
    TRIV_ERR_INVALID_STROKE_TYPE,
    TRIV_ERR_READ_FAIL,
    TRIV_ERR_TVS_INCOMPATIBLE,
    TRIV_ERR_PT_OR_LEN_MISMATCH,
    TRIV_ERR_BZR_TV_EXPECT,
    TRIV_ERR_BSP_TV_EXPECT,
    TRIV_ERR_PERIODIC_EXPECTED,
    TRIV_ERR_PERIODIC_NOT_SUPPORTED,
    TRIV_ERR_CRV_OR_SRF_EXPECTED,
    TRIV_ERR_CRV_SRF_TV_EXPECTED,
    TRIV_ERR_WRONG_NUM_BNDRY_TILES,
    TRIV_ERR_UNSUPPORT_DERIV,
    TRIV_ERR_INVALID_VALUE,
    TRIV_ERR_NO_INTERSECTION,
    TRIV_ERR_SWEEP_AXIS_TOO_COMPLEX,
    TRIV_ERR_FILLET_UNSUPPORTED_TOPOLOGY,
    TRIV_ERR_FILLET_UNSUPPORTED_SRF_TYPE,

    TRIV_ERR_UNDEFINE_ERR
} IritTrivFatalErrorType;

typedef enum {
    TRIV_UNDEF_TYPE = 1220,
    TRIV_TVBEZIER_TYPE,
    TRIV_TVBSPLINE_TYPE,
    TRIV_TVPOWER_TYPE
} TrivGeomType;

typedef enum {
    TRIV_NO_DIR = CAGD_NO_DIR,
    TRIV_CONST_U_DIR = CAGD_CONST_U_DIR,
    TRIV_CONST_V_DIR = CAGD_CONST_V_DIR,
    TRIV_CONST_W_DIR,
    TRIV_END_DIR
} TrivTVDirType;

#define TRIV_IMAGE_VAR_BYTE 0x100000
#define TRIV_IMAGE_VAR_FLOAT 0x200000
#define TRIV_IMAGE_VAR_DOUBLE 0x300000

typedef enum  {
    TRIV_IMAGE_UNDEFINED_TYPE = 0x000000,
    TRIV_IMAGE_BYTE_SCALAR_TYPE =   TRIV_IMAGE_VAR_BYTE   + 0x001000 + sizeof(unsigned char),
    TRIV_IMAGE_FLOAT_SCALAR_TYPE =  TRIV_IMAGE_VAR_FLOAT  + 0x001000 + sizeof(float),
    TRIV_IMAGE_DOUBLE_SCALAR_TYPE = TRIV_IMAGE_VAR_DOUBLE + 0x001000 + sizeof(double),
    TRIV_IMAGE_BYTE_ARGB_TYPE =     TRIV_IMAGE_VAR_BYTE   + 0x004000 + sizeof(unsigned char) * 4,
    TRIV_IMAGE_FLOAT_ARGB_TYPE =    TRIV_IMAGE_VAR_FLOAT  + 0x004000 + sizeof(float) * 4,
    TRIV_IMAGE_DOUBLE_ARGB_TYPE =   TRIV_IMAGE_VAR_DOUBLE + 0x004000 + sizeof(double) * 4
} TrivImagePixelType;

#define TRIV_IMAGE_PIXEL_SIZE(Type)  ((int) ((Type) & 0xfff))
#define TRIV_IMAGE_PIXEL_NUM_COORDS(Type)  ((int) (((Type) & 0x0ff000) >> 12))
#define TRIV_IMAGE_PIXEL_VAR_TYPE(Type)  ((int) ((Type) & 0xf00000))
#define TRIV_IMAGE_PIXEL_CREATE_BYTE_TYPE(N) (TRIV_IMAGE_VAR_BYTE + ((N) << 12) + sizeof(unsigned char) * (N))
#define TRIV_IMAGE_PIXEL_CREATE_FLOAT_TYPE(N) (TRIV_IMAGE_VAR_FLOAT + ((N) << 12) + sizeof(float) * (N))
#define TRIV_IMAGE_PIXEL_CREATE_DOUBLE_TYPE(N) (TRIV_IMAGE_VAR_DOUBLE + ((N) << 12) + sizeof(double) * (N))

#define TRIV_PREV_DIR(Dir) (((int) Dir) + 1 > ((int) TRIV_CONST_W_DIR) ? \
			TRIV_CONST_U_DIR : (TrivTVDirType) ((int) Dir) + 1)
#define TRIV_NEXT_DIR(Dir) (((int) Dir) - 1 < ((int) TRIV_CONST_U_DIR) ? \
			TRIV_CONST_W_DIR : (TrivTVDirType) ((int) Dir) - 1)
#define TRIV_DIR_TO_INT(Dir) \
		  Dir == TRIV_CONST_W_DIR ? 2 : \
                                            (Dir == TRIV_CONST_V_DIR ? 1 : 0))
#define TRIV_INT_TO_DIR(IDir) \
		  IDir == 2 ? TRIV_CONST_W_DIR \
		            : (IDir == 1 ? TRIV_CONST_V_DIR : TRIV_CONST_U_DIR)

typedef enum {
    TRIV_UVW_BNDRY_MASK = CAGD_UV_BNDRY_MASK,     /* 8 lower bits must be 0. */
    TRIV_NO_BNDRY = TRIV_UVW_BNDRY_MASK,
    TRIV_U_MIN_BNDRY = CAGD_U_MIN_BNDRY,
    TRIV_U_MAX_BNDRY = CAGD_U_MAX_BNDRY,
    TRIV_V_MIN_BNDRY = CAGD_V_MIN_BNDRY,
    TRIV_V_MAX_BNDRY = CAGD_V_MAX_BNDRY,
    TRIV_W_MIN_BNDRY = TRIV_UVW_BNDRY_MASK + 16,
    TRIV_W_MAX_BNDRY = TRIV_UVW_BNDRY_MASK + 32,
    TRIV_U_MINMAX_BNDRY = CAGD_U_MINMAX_BNDRY,
    TRIV_V_MINMAX_BNDRY = CAGD_V_MINMAX_BNDRY,
    TRIV_W_MINMAX_BNDRY = TRIV_W_MIN_BNDRY | TRIV_W_MAX_BNDRY,
    TRIV_ALL_BNDRY = TRIV_U_MINMAX_BNDRY | TRIV_V_MINMAX_BNDRY |
		     TRIV_W_MINMAX_BNDRY,
} TrivTVBndryType;

#define TRIV_TV_BNDRY_FILTER_MASK(B) ((B) & ~TRIV_UVW_BNDRY_MASK)

typedef enum {
    TRIV_GEOM_CONST,
    TRIV_GEOM_LINEAR,
    TRIV_GEOM_TV_OF_REV,
    TRIV_GEOM_TRILINEAR,
    TRIV_GEOM_EXTRUSION,
    TRIV_GEOM_RULED_TV,
    TRIV_GEOM_SWEEP_TV
} TrivIsGeometryType;

typedef enum {
    FILLETING_METHOD_RLD_VLM,
    FILLETING_METHOD_BOOL_SUM
} TrivFilletingMethodType;

typedef struct TrivTriangleStruct {
    struct TrivTriangleStruct *Pnext;
    struct IPAttributeStruct *Attr;
    struct {
	CagdPType Pt;
	CagdVType Nrml;
	TrivUVWType UVW;
    } T[3];
} TrivTriangleStruct;

typedef struct TrivTVStruct {
    struct TrivTVStruct *Pnext;
    struct IPAttributeStruct *Attr;
    TrivGeomType GType;
    CagdPointType PType;
    int ULength, VLength, WLength;/* Mesh size in tri-variate tensor product.*/
    int UVPlane;	  /* Should equal ULength * VLength for fast access. */
    int UOrder, VOrder, WOrder;      /* Order in trivariate (B-spline only). */
    CagdBType UPeriodic, VPeriodic, WPeriodic;   /* Valid only for B-spline. */
    CagdRType *Points[CAGD_MAX_PT_SIZE];     /* Pointer on each axis vector. */
    CagdRType *UKnotVector, *VKnotVector, *WKnotVector;
} TrivTVStruct;

typedef struct TrivTVBlockEvalStruct {
    CagdPType Pos;
    CagdRType Jcbn[3][3];
} TrivTVBlockEvalStruct;

typedef struct TrivTVBlockEvalGenInfoStruct *TrivTVBlockEvalGenInfoStructPtr;
typedef struct TrivTVCurvEvalGenInfoStruct *TrivTVCurvEvalGenInfoStructPtr;

typedef void (*TrivSetErrorFuncType)(IritTrivFatalErrorType ErrorFunc);
typedef CagdBType (*TrivTVTestingFuncType)(const TrivTVStruct *TV,
					   TrivTVDirType *Dir,
					   CagdRType *t);
typedef CagdBType (*TrivTVAdjSrfCmpFuncType)(const CagdSrfStruct *Srf1,
					     const CagdSrfStruct *Srf2,
					     CagdRType Eps,
					     int *Modified);

#define TRIV_IS_BEZIER_TV(TV)		((TV) -> GType == TRIV_TVBEZIER_TYPE)
#define TRIV_IS_BSPLINE_TV(TV)		((TV) -> GType == TRIV_TVBSPLINE_TYPE)
#define TRIV_IS_POWER_TV(TV)		((TV) -> GType == TRIV_TVPOWER_TYPE)

#define TRIV_IS_RATIONAL_PT(PType)	CAGD_IS_RATIONAL_PT(PType)
#define TRIV_IS_RATIONAL_TV(TV)		CAGD_IS_RATIONAL_PT((TV) -> PType)
#define TRIV_NUM_OF_PT_COORD(PType)	CAGD_NUM_OF_PT_COORD(PType)
#define TRIV_NUM_OF_TV_COORD(TV)	CAGD_NUM_OF_PT_COORD((TV) -> PType)

#define TRIV_PARAM_IN_DOMAIN(Prm, UMin, UMax, VMin, VMax, WMin, WMax) \
    (Prm[0] >= UMin && Prm[0] <= UMax && \
     Prm[1] >= VMin && Prm[1] <= VMax && \
     Prm[2] >= WMin && Prm[2] <= WMax)

#define TRIV_COERCE_TO_DOMAIN(Pt, UMin, UMax, VMin, VMax, WMin, WMax) \
	if (Pt[0] < UMin) \
	    Pt[0] = UMin; \
	if (Pt[0] > UMax) \
	    Pt[0] = UMax; \
	if (Pt[1] < VMin) \
	    Pt[1] = VMin; \
	if (Pt[1] > VMax) \
	    Pt[1] = VMax; \
	if (Pt[2] < WMin) \
	    Pt[2] = WMin; \
	if (Pt[2] > WMax) \
	    Pt[2] = WMax;

/*****************************************************************************
*	    +-----------------------+					     *
*	W  /			   /|					     *
*      /  /			  / |					     *
*     /	 /	U -->		 /  |	    The	mesh is	ordered	raw after raw*
*	+-----------------------+   |	or the increments along	U are 1	while*
*   V |	|P0		    Pi-1|   +	the increment along V is full raw.   *
*     v	|Pi		   P2i-1|  /	    Once a full	UV plane is complete *
*	|			| /	W is incremented by 1.		     *
*	|Pn-i		    Pn-1|/	    To encapsulate it, NEXTU/V/W are *
*	+-----------------------+	defined	below.			     *
*****************************************************************************/
#define TRIV_NEXT_U(TV)			(1)
#define TRIV_NEXT_V(TV)			(TV -> ULength)
#define TRIV_NEXT_W(TV)			(TV -> UVPlane)
#define TRIV_MESH_UVW(TV, i, j, k)	((i) + (TV -> ULength) * (j) + (TV -> UVPlane) * (k))

#define TRIV_CTL_MESH_LENGTH(TV)	((TV) -> UVPlane * (TV) -> WLength)

/* If a trivariate is periodic, the control polygon/mesh should warp up.     */
/* Length does hold the real allocated length but the virtual periodic       */
/* length is a little larger. Note allocated KV's are larger.                */
#define TRIV_TV_UPT_LST_LEN(TV)	((TV) -> ULength + \
				 ((TV) -> UPeriodic ? (TV) -> UOrder - 1 : 0))
#define TRIV_TV_VPT_LST_LEN(TV)	((TV) -> VLength + \
				 ((TV) -> VPeriodic ? (TV) -> VOrder - 1 : 0))
#define TRIV_TV_WPT_LST_LEN(TV)	((TV) -> WLength + \
				 ((TV) -> WPeriodic ? (TV) -> WOrder - 1 : 0))
#define TRIV_IS_UPERIODIC_TV(TV)	((TV) -> UPeriodic)
#define TRIV_IS_VPERIODIC_TV(TV)	((TV) -> VPeriodic)
#define TRIV_IS_WPERIODIC_TV(TV)	((TV) -> WPeriodic)
#define TRIV_IS_PERIODIC_TV(TV)	(TRIV_IS_UPERIODIC_TV(TV) || \
				 TRIV_IS_VPERIODIC_TV(TV) || \
				 TRIV_IS_WPERIODIC_TV(TV))

#define TRIV_DEL_GEOM_TYPE(Obj)		 CAGD_DEL_GEOM_TYPE(Obj)
#define TRIV_SET_GEOM_TYPE(Obj, Geom)	 CAGD_SET_GEOM_TYPE(Obj, Geom)
#define TRIV_PROPAGATE_ATTR(NewObj, Obj) CAGD_PROPAGATE_ATTR(NewObj, Obj)

#define TRIV_TV_EVAL_SCALAR(Trivar, u, v, w, PtE1) \
	     { CagdRType _R[CAGD_MAX_PT_SIZE]; \
	       IritTrivTVEvalToData((Trivar), (u), (v), (w), _R); \
	       *PtE1 = TRIV_IS_RATIONAL_TV((Trivar)) ? _R[1] / _R[0] : _R[1]; }
#define TRIV_TV_EVAL_E3(Trivar, u, v, w, PtE3) \
		{ CagdRType _R[CAGD_MAX_PT_SIZE], *PR = _R; \
		  IritTrivTVEvalToData((Trivar), (u), (v), (w), _R); \
		  IritCagdCoerceToE3(PtE3, &PR, -1, (Trivar) -> PType); }

/*****************************************************************************
* This macro is	called when the	library	has detected an	unrecoverable error. *
* Default action is to call IritTrivFatalError,	but you	may want to reroute  *
* this to invoke your handler & recover yourself (by long jump for example). *
*****************************************************************************/
#define TRIV_FATAL_ERROR(Msg)	IritTrivFatalError(Msg)

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/*****************************************************************************
* General routines of the Triv library:					     *
*****************************************************************************/
TrivTVStruct *IritTrivTVNew(TrivGeomType GType,
			    CagdPointType PType,
			    int	ULength,
			    int	VLength,
			    int	WLength);
TrivTVStruct *IritTrivBspTVNew(int ULength,
			       int VLength,
			       int WLength,
			       int UOrder,
			       int VOrder,
			       int WOrder,
			       CagdPointType PType);
TrivTVStruct *IritTrivBspPeriodicTVNew(int ULength,
				       int VLength,
				       int WLength,
				       int UOrder,
				       int VOrder,
				       int WOrder,
				       CagdBType UPeriodic,
				       CagdBType VPeriodic,
				       CagdBType WPeriodic,
				       CagdPointType PType);
TrivTVStruct *IritTrivBzrTVNew(int ULength,
			       int VLength,
			       int WLength,
			       CagdPointType PType);
TrivTVStruct *IritTrivPwrTVNew(int ULength,
			       int VLength,
			       int WLength,
			       CagdPointType PType);
TrivTVStruct *IritTrivTVCopy(const TrivTVStruct *TV);
TrivTVStruct *IritTrivTVCopyList(const TrivTVStruct *TVList);
void IritTrivTVFree(TrivTVStruct *TV);
void IritTrivTVFreeList(TrivTVStruct *TVList);

#ifdef DEBUG
#define IritTrivTVFree(TV)         { IritTrivTVFree(TV); TV = NULL; }
#define IritTrivTVFreeList(TVList) { IritTrivTVFreeList(TVList); TVList = NULL; }
#endif /* DEBUG */

TrivTriangleStruct *IritTrivTriangleNew(void);
TrivTriangleStruct *IritTrivTriangleCopy(const TrivTriangleStruct *Triangle);
TrivTriangleStruct *IritTrivTriangleCopyList(
				      const TrivTriangleStruct *TriangleList);
void IritTrivTriangleFree(TrivTriangleStruct *Triangle);
void IritTrivTriangleFreeList(TrivTriangleStruct *TriangleList);

#ifdef DEBUG
#define IritTrivTriangleFree(Triangle)     { IritTrivTriangleFree(Triangle); \
					     Triangle = NULL; }
#define IritTrivTriangleFreeList(TriangleList) { IritTrivTriangleFreeList(TriangleList); \
					         TriangleList = NULL; }
#endif /* DEBUG */

TrivTVStruct *IritTrivPrimCylinder(const CagdVType Center,
				   CagdRType Radius,
				   CagdRType Height,
				   CagdBType Rational);
TrivTVStruct *IritTrivPrimCone(const CagdVType Center,
			       CagdRType Radius,
			       CagdRType Height,
			       CagdBType Rational);
TrivTVStruct *IritTrivPrimCone2(const CagdVType Center,
				CagdRType MajorRadius,
				CagdRType MinorRadius,
				CagdRType Height,
				CagdBType Rational);
TrivTVStruct *IritTrivPrimSphere(const CagdVType Center,
				 CagdRType Radius,
				 CagdBType Rational);
TrivTVStruct *IritTrivPrimSphere2(const CagdVType Center,
				  CagdRType Radius,
				  CagdBType Rational);
TrivTVStruct *IritTrivPrimTorus(const CagdVType Center,
				CagdRType MajorRadius,
				CagdRType MinorRadius,
				CagdBType Rational);

TrivTVStruct *IritTrivNSPrimBox(CagdRType MinX,
				CagdRType MinY,
				CagdRType MinZ,
				CagdRType MaxX,
				CagdRType MaxY,
				CagdRType MaxZ);
TrivTVStruct *IritTrivNSPrimGenBox(const CagdPType P000,
				   const CagdPType P001,
				   const CagdPType P010,
				   const CagdPType P011,
				   const CagdPType P100,
				   const CagdPType P101,
				   const CagdPType P110,
				   const CagdPType P111);
TrivTVStruct *IritTrivNSPrimGenBox2(const CagdVType VecUMin,
				    const CagdVType VecUMax,
				    const CagdVType VecVMin,
				    const CagdVType VecVMax,
				    const CagdVType VecWMin,
				    const CagdVType VecWMax,
				    int	Normalize);
TrivTVStruct *IritTrivNSPrimCylinder(const CagdVType Center,
				     CagdRType Radius,
				     CagdRType Height,
				     CagdBType Rational,
				     CagdRType InternalCubeEdge);
TrivTVStruct *IritTrivNSPrimCone(const CagdVType Center,
				 CagdRType Radius,
				 CagdRType Height,
				 CagdBType Rational,
				 CagdRType InternalCubeSize);
TrivTVStruct *IritTrivNSPrimCone2(const CagdVType Center,
				  CagdRType MajorRadius,
				  CagdRType MinorRadius,
				  CagdRType Height,
				  CagdBType Rational,
				  CagdRType InternalCubeEdge);
TrivTVStruct *IritTrivNSPrimSphere(const CagdVType Center,
				   CagdRType Radius,
				   CagdBType Rational,
				   CagdRType InternalCubeEdge);
TrivTVStruct *IritTrivNSPrimTorus(const CagdVType Center,
				  CagdRType MajorRadius,
				  CagdRType MinorRadius,
				  CagdBType Rational,
				  CagdRType InternalCubeEdge);

TrivTVStruct *IritTrivCnvrtBzr2BspTV(const TrivTVStruct *TV);
TrivTVStruct *IritTrivCnvrtBsp2BzrTV(const TrivTVStruct *TV);
TrivTVStruct *IritTrivCnvrtBsp2BzrTVList(const TrivTVStruct *TVs);

void IritTrivTVTransform(TrivTVStruct *TV,
			 const CagdRType *Translate,
			 CagdRType Scale);
TrivTVStruct *IritTrivTVMatTransform(const TrivTVStruct *TV, CagdMType Mat);
TrivTVStruct *IritTrivTVListMatTransform(const TrivTVStruct *TVs,
					 CagdMType Mat);
void IritTrivTVMatTransform2(TrivTVStruct *TV, CagdMType Mat);

TrivTVStruct *IritTrivCoerceTVsTo(const TrivTVStruct *TV, CagdPointType PType);
TrivTVStruct *IritTrivCoerceTVTo(const TrivTVStruct *TV, CagdPointType PType);

void IritTrivTVDomain(const TrivTVStruct *TV,
		      CagdRType	*UMin,
		      CagdRType	*UMax,
		      CagdRType	*VMin,
		      CagdRType	*VMax,
		      CagdRType	*WMin,
		      CagdRType	*WMax);
TrivTVStruct *IritTrivTVSetDomain(TrivTVStruct *TV,
				  CagdRType UMin,
				  CagdRType UMax,
				  CagdRType VMin,
				  CagdRType VMax,
				  CagdRType WMin,
				  CagdRType WMax);
TrivTVStruct *IritTrivTVSetDomain2(TrivTVStruct *TV,
				   CagdRType Min,
				   CagdRType Max,
				   TrivTVDirType Dir);
CagdBType IritTrivParamInDomain(const TrivTVStruct *TV,
				CagdRType t,
				TrivTVDirType Dir);
CagdBType IritTrivParamsInDomain(const TrivTVStruct *TV,
				 CagdRType u,
				 CagdRType v,
				 CagdRType w);

void IritTrivTVEvalToData(const TrivTVStruct *TV,
			  CagdRType u,
			  CagdRType v,
			  CagdRType w,
			  CagdRType *Pt);
CagdRType *IritTrivTVEvalMalloc(const TrivTVStruct *TV,
				CagdRType u,
				CagdRType v,
				CagdRType w);
void IritTrivTVEval2ToData(const TrivTVStruct *TV,
			   CagdRType u,
			   CagdRType v,
			   CagdRType w,
			   CagdRType *Pt);
CagdRType *IritTrivTVEval2Malloc(const TrivTVStruct *TV,
				 CagdRType u,
				 CagdRType v,
				 CagdRType w);
CagdSrfStruct *IritTrivSrfFromTV(const TrivTVStruct *TV,
				 CagdRType t,
				 TrivTVDirType Dir,
				 int OrientBoundary);
CagdSrfStruct *IritTrivBndrySrfFromTV(const TrivTVStruct *TV,
				      TrivTVBndryType TVBndry,  
				      int OrientBndry);
CagdSrfStruct **IritTrivBndrySrfsFromTVToData(const TrivTVStruct *TV,
					      int OrientBoundary,
					      int FilterSimilar,
					      CagdSrfStruct **Srfs);
CagdSrfStruct *IritTrivBndrySrfsFromTVs(const TrivTVStruct *Trivars,
					CagdRType Eps,
					int OrientBoundary,
					int FilterSelfSimilar,
					int FilterDupSrfs);
CagdCrvStruct *IritTrivBndryEdgesFromTV(const TrivTVStruct *TV);
CagdPtStruct *IritTrivBndryCrnrsFromTV(const TrivTVStruct *TV);

CagdSrfStruct *IritTrivSrfFromMesh(const TrivTVStruct *TV,
				   int Index,
				   TrivTVDirType Dir);
void IritTrivSrfToMesh(const CagdSrfStruct *Srf,
		       int Index,
		       TrivTVDirType Dir,
		       TrivTVStruct *TV);
CagdRType *IritTrivTVMultEval(CagdRType *UKnotVector,
			      CagdRType	*VKnotVector,
			      CagdRType	*WKnotVector,
			      int ULength,
			      int VLength,
			      int WLength,
			      int UOrder,
			      int VOrder,
			      int WOrder,
			      CagdPType	*Mesh,
			      CagdPType	*Params,
			      int NumOfParams,
			      int *RetSize,
			      CagdIritCagdBspBasisFuncMultEvalType EvalType);
TrivTVBlockEvalGenInfoStructPtr IritTrivTVBlockEvalInit(CagdRType *UKnotVector,
							CagdRType *VKnotVector,
							CagdRType *WKnotVector,
							int Lengths[3],
							int Orders[3],
							int BlockSizes[3],
							CagdPType *Params,
							int NumOfParams[3]);
void IritTrivTVBlockEvalSetMesh(TrivTVBlockEvalGenInfoStructPtr TVBlock,
				CagdPType *Mesh);
TrivTVBlockEvalStruct *IritTrivTVBlockEvalOnce(
					       TrivTVBlockEvalGenInfoStructPtr TVBlock,
					       int i, int j, int k);
void IritTrivTVBlockEvalDone(TrivTVBlockEvalGenInfoStructPtr TVBlock);
TrivTVStruct *IritTrivTVRegionFromTV(const TrivTVStruct *TV,
				     CagdRType t1,
				     CagdRType t2,
				     TrivTVDirType Dir);
TrivTVStruct *IritTrivTVRefineAtParams(const TrivTVStruct *TV,
				       TrivTVDirType Dir,
				       CagdBType Replace,
				       CagdRType *t,
				       int n);
TrivTVStruct *IritTrivBspTVKnotInsertNDiff(const TrivTVStruct *TV,
					   TrivTVDirType Dir,
					   int Replace,
					   const CagdRType *t,
					   int n);
TrivTVStruct *IritTrivTVDerive(const TrivTVStruct *TV, TrivTVDirType Dir);
TrivTVStruct *IritTrivTVDeriveScalar(const TrivTVStruct *TV, TrivTVDirType Dir);
TrivTVStruct *IritTrivBzrTVDerive(const TrivTVStruct *TV, 
				  TrivTVDirType	Dir,
				  CagdBType DeriveScalar);
TrivTVStruct *IritTrivBzrTVDeriveScalar(const TrivTVStruct *TV,
					TrivTVDirType Dir);
TrivTVStruct *IritTrivBspTVDerive(const TrivTVStruct *TV,
				  TrivTVDirType	Dir,
				  CagdBType DeriveScalar);
TrivTVStruct *IritTrivBspTVDeriveScalar(const TrivTVStruct *TV,
					TrivTVDirType Dir);
CagdRType IritTrivTVEvalJacobian(const TrivTVStruct *TV,
				 CagdRType u,
				 CagdRType v,
				 CagdRType w);
CagdRType IritTrivTVEvalJacobianApprox(const TrivTVStruct *TV,
				       CagdRType u,
				       CagdRType v,
				       CagdRType w,
				       CagdRType Eps);
TrivTVStruct *IritTrivTVSubdivAtParam(const TrivTVStruct *TV,
				      CagdRType	t,
				      TrivTVDirType Dir);
TrivTVStruct *IritTrivTVSubdivAtParams(const TrivTVStruct *TV,
				       const CagdPtStruct *Pts,
				       TrivTVDirType Dir,
				       CagdRType Eps,
				       CagdBType PurgeTooSmallSegs,
				       int *Proximity);
TrivTVStruct *IritTrivTVsSubdivAtAllDetectedLocations(const TrivTVStruct *TV,
						      TrivTVTestingFuncType
						      TVTestFunc);
TrivTVStruct *IritTrivTVSubdivAtAllC0Discont(const TrivTVStruct *TV);
TrivTVStruct *IritTrivTVsSubdivAtAllC1Discont(const TrivTVStruct *TVs);
TrivTVStruct *IritTrivTVSubdivAtAllC0Discont(const TrivTVStruct *TV);
TrivTVStruct *IritTrivTVSubdivAtAllC1Discont(const TrivTVStruct *TV);
TrivTVStruct *IritTrivTVDegreeRaise(const TrivTVStruct *TV, TrivTVDirType Dir);
TrivTVStruct *IritTrivTVDegreeRaiseN(const TrivTVStruct *TV,
				     TrivTVDirType Dir,
				     int NewOrder);
TrivTVStruct *IritTrivBspTVDegreeRaise(const TrivTVStruct *TV,
				       TrivTVDirType Dir);
TrivTVStruct *IritTrivBzrTVDegreeRaise(const TrivTVStruct *TV,
				       TrivTVDirType Dir);
TrivTVStruct *IritTrivTVBlossomDegreeRaise(const TrivTVStruct *TV,
					   TrivTVDirType Dir);
TrivTVStruct *IritTrivTVBlossomDegreeRaiseN(const TrivTVStruct *TV,
					    int	NewUOrder,
					    int	NewVOrder,
					    int	NewWOrder);
TrivTVStruct *IritTrivTVReverseDir(const TrivTVStruct *TV, TrivTVDirType Dir);
TrivTVStruct *IritTrivTVReverse2Dirs(const TrivTVStruct *TV,
				     TrivTVDirType Dir1,
				     TrivTVDirType Dir2);
TrivTVStruct *IritTrivMergeTVTV(const TrivTVStruct *CTV1,
				const TrivTVStruct *CTV2,
				TrivTVDirType Dir,
				CagdBType Discont);
int IritTrivUpdateBndrySrfinTV(TrivTVStruct *TV,
			       const CagdSrfStruct *Srf,
			       TrivTVBndryType TVBndry);
int IritTrivMakeTVsCompatibleDomain(const TrivTVStruct *TV1,
				    TrivTVStruct **TV2);
CagdBType IritTrivMakeTVsCompatible(TrivTVStruct **TV1,
				    TrivTVStruct **TV2,
				    CagdBType SameUOrder,
				    CagdBType SameVOrder,
				    CagdBType SameWOrder,
				    CagdBType SameUKV,
				    CagdBType SameVKV,
				    CagdBType SameWKV);
CagdBBoxStruct *IritTrivTVBBox(const TrivTVStruct *TV, CagdBBoxStruct *BBox);
CagdBBoxStruct *IritTrivTVListBBox(const TrivTVStruct *TVs,
				   CagdBBoxStruct *BBox);
CagdPolylineStruct *IritTrivTV2CtrlMesh(const TrivTVStruct *Trivar);
CagdRType IritTrivTVVolume(const TrivTVStruct *TV, CagdBType VolType);
CagdRType IritTrivSrfArea(const CagdSrfStruct *Srf, CagdBType VolType);

void IritTrivTVPointInclusionPrep(TrivTVStruct *TV, int n);
CagdBType IritTrivTVPointInclusion(TrivTVStruct *TV, const IrtPtType Pt);
void IritTrivTVPointInclusionFree(TrivTVStruct *TV);

TrivTVStruct *IritTrivInterpTrivar(const TrivTVStruct *TV);
TrivTVStruct *IritTrivTVInterpPts(const TrivTVStruct *PtGrid,
				  int UOrder,
				  int VOrder,
				  int WOrder,
				  int TVUSize,
				  int TVVSize,
				  int TVWSize);
TrivTVStruct *IritTrivTVInterpolate(const TrivTVStruct *PtGrid,
				    int	ULength,
				    int	VLength,
				    int	WLength,
				    int	UOrder,
				    int	VOrder,
				    int	WOrder);
TrivTVStruct *IritTrivTVInterpScatPts(const CagdCtlPtStruct *PtList,
				      int USize,
				      int VSize,
				      int WSize,
				      int UOrder,
				      int VOrder,
				      int WOrder,
				      CagdRType	*UKV,
				      CagdRType	*VKV,
				      CagdRType	*WKV);
TrivTVStruct *IritTrivCnvrtCrvToTV(const CagdCrvStruct *Crv, TrivTVDirType Dir);
TrivTVStruct *IritTrivCnvrtSrfToTV(const CagdSrfStruct *Srf, TrivTVDirType Dir);
TrivTVStruct *IritTrivTVFromSrfs(const CagdSrfStruct *SrfList,
				 int OtherOrder,
				 CagdEndConditionType OtherEC,
				 IrtRType *OtherParamVals);
CagdRType *IritTrivTVInterpolateSrfsChordLenParams(
						 const CagdSrfStruct *SrfList);
TrivTVStruct *IritTrivTVInterpolateSrfs(const CagdSrfStruct *SrfList,
					int OtherOrder,
					CagdEndConditionType OtherEC,
					CagdParametrizationType	OtherParam,
					IrtRType *OtherParamVals);
TrivTVStruct *IritTrivRuledTV(const CagdSrfStruct *Srf1,
			      const CagdSrfStruct *Srf2,
			      int OtherOrder,
			      int OtherLen);
TrivTVStruct *TrivTrilinearSrf(const CagdPtStruct *Pt000,
			       const CagdPtStruct *Pt001,
			       const CagdPtStruct *Pt010,
			       const CagdPtStruct *Pt011,
			       const CagdPtStruct *Pt100,
			       const CagdPtStruct *Pt101,
			       const CagdPtStruct *Pt110,
			       const CagdPtStruct *Pt111,
			       CagdPointType PType);
TrivTVStruct *IritTrivExtrudeTV(const CagdSrfStruct *Srf,
				const CagdVecStruct *Vec);
TrivTVStruct *IritTrivExtrudeTV2(const CagdSrfStruct *Srf,
				 const CagdCrvStruct *Crv);
TrivTVStruct *IritTrivZTwistExtrudeSrf(const CagdSrfStruct *Srf,
				       CagdBType Rational,
				       CagdRType ZPitch);
TrivTVStruct *IritTrivTVOfRev(const CagdSrfStruct *Srf);
TrivTVStruct *IritTrivTVOfRev2(const CagdSrfStruct *Srf,
			       CagdBType PolyApprox,
			       CagdRType StartAngle,
			       CagdRType EndAngle);
TrivTVStruct *IritTrivTVOfRevPolynomialApprox(const CagdSrfStruct *Srf);
TrivTVStruct *IritTrivTVOfRevAxis(const CagdSrfStruct *Srf,
				  const	TrivP4DType AxisPoint,
				  const	TrivV4DType AxisVector,
				  CagdBType PolyApprox);
CagdSrfStruct *IritTrivExtractSleeveSrf(const TrivTVStruct *TV);
TrivTVStruct *IritTrivSweepTV(const CagdSrfStruct *CrossSection,
			      const CagdCrvStruct *Axis,
			      const CagdCrvStruct *ScalingCrv,
			      CagdRType	Scale,
			      const VoidPtr Frame,
			      int FrameOption);
TrivTVStruct *IritTrivSweepTVC1(const CagdSrfStruct *CrossSection,
				const CagdCrvStruct *Axis,
				const CagdCrvStruct *ScalingCrv,
				CagdRType Scale,
				const VoidPtr Frame,
				int FrameOption,
				CagdCrvCornerType CornerType,
				CagdRType C1DiscontCropTol);
CagdSrfStruct *IritTrivSweepTVError(const TrivTVStruct *SweepTV,
				    const CagdSrfStruct	*CrossSection,
				    const CagdCrvStruct	*Axis,
				    const CagdCrvStruct	*ScalingCrv,
				    CagdRType Scale);
TrivTVStruct *IritTrivEditSingleTVPt(const TrivTVStruct *TV,
				     CagdCtlPtStruct *CtlPt,
				     int UIndex,
				     int VIndex,
				     int WIndex,
				     CagdBType Write);
CagdBType IritTrivTVsSame(const TrivTVStruct *Tv1,
			  const	TrivTVStruct *Tv2,
			  CagdRType Eps);

CagdBType IritTrivTVKnotHasC0Discont(const TrivTVStruct *TV,
				     TrivTVDirType *Dir,
				     CagdRType *t);
CagdBType IritTrivTVMeshC0Continuous(const TrivTVStruct *TV,
				     TrivTVDirType Dir,
				     int Idx);
CagdBType IritTrivTVIsMeshC0DiscontAt(const TrivTVStruct *TV,
				      int Dir,
				      CagdRType t);
CagdBType IritTrivTVKnotHasC1Discont(const TrivTVStruct *TV,
				     TrivTVDirType *Dir,
				     CagdRType *t);
CagdBType IritTrivTVMeshC1Continuous(const TrivTVStruct *TV,
				     TrivTVDirType Dir,
				     int Idx);
CagdBType TrivTVIsMeshC1DiscontAt(const TrivTVStruct *TV,
				  int Dir,
				  CagdRType t);
CagdBType IritTrivBspTVHasBezierKVs(const TrivTVStruct *TV);
CagdBType IritTrivBspTVHasOpenEC(const TrivTVStruct *TV);
CagdBType IritTrivBspTVHasOpenECDir(const TrivTVStruct *TV, TrivTVDirType Dir);
CagdBType IritTrivIsTVClosed(const TrivTVStruct *TV, int Dim);
void IritTrivTVGenAdjacencyInfo(TrivTVStruct *TVList,
				TrivTVAdjSrfCmpFuncType	SrfCmpFuncPtr,
				TrivTVAdjSrfCmpFuncType	SrfCmpApxFuncPtr,
				CagdRType SrfGapTol,
				CagdBType MarkUVWBndry);
void IritTrivTVGenAdjacencyInfo2(TrivTVStruct **TVVec,
				 int NumTVs,
				 TrivTVAdjSrfCmpFuncType SrfCmpFuncPtr,
				 TrivTVAdjSrfCmpFuncType SrfCmpApxFuncPtr,
				 CagdRType SrfGapTol,
				 CagdBType MarkUVWBndry);
TrivTVStruct *IritTrivMakeTVArrngmntCompatible(const TrivTVStruct *TVList);

void IritTrivDbg(const void *Obj);
void IritTrivDbg1(const void *Obj);

#ifdef DEBUG
void IritTrivDbgDsp(const void *Obj);
#endif /* DEBUG */

/*****************************************************************************
* Routines to handle basis function conversions.			     *
*****************************************************************************/
TrivTVStruct *IritTrivCnvrtPeriodic2FloatTV(const TrivTVStruct *TV);
TrivTVStruct *IritTrivCnvrtFloat2OpenTV(const TrivTVStruct *TV);
TrivTVStruct *IritTrivTVOpenEnd(const TrivTVStruct *TV);

/*****************************************************************************
* Metamorphosis	and FFD	of trivariates.					     *
*****************************************************************************/
TrivTVStruct *IritTrivTwoTVsMorphing(const TrivTVStruct *TV1,
				     const TrivTVStruct	*TV2,
				     CagdRType Blend);
void IritTrivFFDCtlMeshUsingTV(CagdRType **Points,
			       int Length,
			       CagdPointType PType,
			       const TrivTVStruct *DeformTV);
struct IPObjectStruct *IritTrivFFDObjectTV(struct IPObjectStruct *PObj,
					   const TrivTVStruct *DeformTV);
struct IPObjectStruct *IritTrivFFDTileObjectInTV(
					     const struct IPObjectStruct *PObj,
					     const TrivTVStruct *DeformTV,
					     IrtRType UTimes,
					     IrtRType VTimes,
					     IrtRType WTimes,
					     int FitObj,
					     IrtRType CropBoundaries,
					     IrtRType MaxEdgeLen,
					     const IrtRType *TriLinearMap);
int IritTrivFFDTileCropBndries(struct IPObjectStruct *BndryTiles[3][3][3],
			       const struct IPObjectStruct *Tile,
			       IrtHmgnMatType Mat,
			       IrtRType CropBoundaries);
void IritTrivFFDTileFreeBndries(struct IPObjectStruct *BndryTiles[3][3][3]);

/*****************************************************************************
* Local	curvature processing.						     *
*****************************************************************************/
TrivTVCurvEvalGenInfoStructPtr IritTrivEvalTVCurvaturePrelude(
						      const TrivTVStruct *TV);
CagdBType IritTrivEvalCurvature(TrivTVCurvEvalGenInfoStructPtr TrivTVCurvature,
				CagdPType Pos,
				CagdRType *PCurv1,
				CagdRType *PCurv2,
				CagdVType PDir1,
				CagdVType PDir2);
CagdBType IritTrivEvalGradient(TrivTVCurvEvalGenInfoStructPtr TrivTVCurvature,
			       CagdPType Pos,
			       CagdVType Gradient);
CagdBType IritTrivEvalHessian(TrivTVCurvEvalGenInfoStructPtr TrivTVCurvature,
			      CagdPType	Pos,
			      CagdVType	Hessian[3]);
void IritTrivEvalTVCurvaturePostlude(TrivTVCurvEvalGenInfoStructPtr
				     TrivTVCurvature);

/*****************************************************************************
* Geometry in R^4.							     *
*****************************************************************************/
int IritTrivPlaneFrom4Points(const TrivP4DType Pt1,
			     const TrivP4DType Pt2,
			     const TrivP4DType Pt3,
			     const TrivP4DType Pt4,
			     TrivPln4DType Plane);
void IritTrivVectCross3Vecs(const TrivV4DType A,
			    const TrivV4DType B,
			    const TrivV4DType C,
			    TrivV4DType	Res);

/*****************************************************************************
* Routines to compute trivariate compositions.				     *
*****************************************************************************/
struct IPObjectStruct *IritTrivComposeTileObjectInTV(
					     const struct IPObjectStruct *PObj,
					     const TrivTVStruct	*DeformTV,
					     IrtRType UTimes,
					     IrtRType VTimes,
					     IrtRType WTimes,
					     int FitObj,
					     IrtRType CropBoundaries);
struct IPObjectStruct *IritTrivComposeTileObjectInTVBzr(
					     const struct IPObjectStruct *PObj,
					     const TrivTVStruct *DeformTV,
					     IrtRType UTimes,
					     IrtRType VTimes,
					     IrtRType WTimes,
					     int FitObj);
struct IPObjectStruct *TrivComposeOneObjectInTVBzr(
					     const struct IPObjectStruct *PObj,
					     const TrivTVStruct *DeformTV);
CagdCrvStruct *IritTrivComposeTVCrv(const TrivTVStruct *TV,
				    const CagdCrvStruct	*Crv);
CagdCrvStruct *TrivBzrComposeTVCrv(const TrivTVStruct *TV,
				   const CagdCrvStruct *Crv);
CagdSrfStruct *IritTrivComposeTVSrf(const TrivTVStruct *TV,
				    const CagdSrfStruct	*Srf);
CagdSrfStruct *TrivBzrComposeTVSrf(const TrivTVStruct *TV,
				   const CagdSrfStruct *Srf);
TrivTVStruct *IritTrivComposeTVTV(const TrivTVStruct *TV1,
				  const	TrivTVStruct *TV2);

/*****************************************************************************
* Routines to handle adaptive extraction of surfaces and curves	from trivars.*
*****************************************************************************/
struct TrimSrfStruct *IritTrivAdapIsoExtractSrfs(const TrivTVStruct *Trivar,
						 TrivTVDirType Dir,
						 CagdRType Epsilon,
						 int InitialDiv,
						 CagdRType CntrEps);
CagdCrvStruct *IritTrivAdapIsoExtractCrvs(const TrivTVStruct *Trivar,
					  TrivTVDirType	SrfDir,
					  CagdRType Epsilon,
					  int InitialDiv,
					  CagdSrfDirType CrvDir,
					  CagdRType CntrEps);

/*****************************************************************************
* Routines to handle inverse queries over trivariates.			     *
*****************************************************************************/
struct IritTrivInverseQueryStruct *IritTrivPrepInverseQueries(
						  const TrivTVStruct *Trivar);
int IritTrivInverseQuery(struct IritTrivInverseQueryStruct *Handle,
			 const CagdRType *XYZPos,
			 CagdRType *UVWParams,
			 int InitialGuess);
void IritTrivFreeInverseQueries(struct IritTrivInverseQueryStruct *Handle);
struct IritTrivInverseQueryStruct *IritTrivCopyInverseQueries(
			     const struct IritTrivInverseQueryStruct *Handle);
int IritTrivInverseQueryPolys(struct IPObjectStruct *PlObj,
			      const TrivTVStruct *TV);

/*****************************************************************************
* Symbolic computations	routines.					     *
*****************************************************************************/
TrivTVStruct *IritTrivTVAdd(const TrivTVStruct *TV1, const TrivTVStruct *TV2);
TrivTVStruct *IritTrivTVSub(const TrivTVStruct *TV1, const TrivTVStruct *TV2);
TrivTVStruct *IritTrivTVMult(const TrivTVStruct *TV1, const TrivTVStruct *TV2);
TrivTVStruct *IritTrivTVInvert(const TrivTVStruct *TV);
TrivTVStruct *IritTrivTVMultScalar(const TrivTVStruct *TV1,
				   const TrivTVStruct *TV2);
TrivTVStruct *IritTrivTVDotProd(const TrivTVStruct *TV1,
				const TrivTVStruct *TV2);
TrivTVStruct *IritTrivTVVecDotProd(const TrivTVStruct *TV, const CagdVType Vec);
TrivTVStruct *IritTrivTVCrossProd(const TrivTVStruct *TV1,
				  const TrivTVStruct *TV2);
TrivTVStruct *IritTrivTVRtnlMult(const TrivTVStruct *TV1X,
				 const TrivTVStruct *TV1W,
				 const TrivTVStruct *TV2X,
				 const TrivTVStruct *TV2W,
				 CagdBType OperationAdd);

TrivTVStruct **IritTrivTVSplitScalarNToData(const TrivTVStruct *TV,
					    TrivTVStruct **Tvs);
void IritTrivTVSplitScalar(const TrivTVStruct *TV,
			   TrivTVStruct	**TVW,
			   TrivTVStruct	**TVX,
			   TrivTVStruct	**TVY,
			   TrivTVStruct	**TVZ);
TrivTVStruct *IritTrivTVMergeScalarN(TrivTVStruct * const *TVVec, int NumTVs);
TrivTVStruct *IritTrivTVMergeScalar(const TrivTVStruct *TVW,
				    const TrivTVStruct *TVX,
				    const TrivTVStruct *TVY,
				    const TrivTVStruct *TVZ);

/*****************************************************************************
* Special trivariate constructors.					     *
*****************************************************************************/
TrivTVStruct *IritTrivAlgebraicSumTV(const CagdCrvStruct *Crv,
				     const CagdSrfStruct *Srf);
TrivTVStruct *IritTrivAlgebraicProdTV(const CagdCrvStruct *Crv,
				      const CagdSrfStruct *Srf);
TrivTVStruct *IritTrivSwungAlgSumTV(const CagdCrvStruct *Crv,
				    const CagdSrfStruct	*Srf);
TrivTVStruct *IritTrivImplicitTVFromDistFFs(
					  int Order,
					  int Length,
					  const CagdCrvStruct *Crvs,
					  const CagdSrfStruct *Srfs,
					  const CagdRType *CrvsDistThresholds,
					  const CagdRType *SrfsDistThresholds,
					  int NumDistThresholds,
					  const CagdRType CornerBias[2]);

/*****************************************************************************
* Fillets methods.							     *
*****************************************************************************/
TrivTVStruct *IritTrivTVFillet(const CagdSrfStruct *Srf1List,
			       const CagdSrfStruct *Srf2List,
			       CagdRType RailDist,
			       int R1Orient,
			       int R2Orient,
			       CagdRType TanScale,
			       int ApproxCrvsCtlPts,
			       CagdRType Tol,
			       CagdRType NumerTol,
			       TrivFilletingMethodType FilletingMethod,
			       CagdBType PreciseFillet,
			       CagdSrfStruct **PrimSrfs1,
			       CagdSrfStruct **PrimSrfs2);

void IritTrivBlendFilletProperties(TrivTVStruct **FilletTV,
				   const CagdSrfStruct *PrimSrf1,
				   const CagdSrfStruct *PrimSrf2);

/*****************************************************************************
* Error	handling.							     *
*****************************************************************************/
TrivSetErrorFuncType IritTrivSetFatalErrorFunc(TrivSetErrorFuncType ErrorFunc);
void IritTrivFatalError(IritTrivFatalErrorType ErrID);
const char *IritTrivDescribeError(IritTrivFatalErrorType ErrID);

/*****************************************************************************
* Trivariate fitting methods.						     *
*****************************************************************************/
TrivTVStruct *IritTrivFitTV2PolyMesh(struct IPObjectStruct *MeshObj, 
				     struct IPObjectStruct *MedialAxisObj,
				     IrtRType MeanErrThr, 
				     IrtRType HausdorffErrThr);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* IRIT_TRIV_LIB_H */
