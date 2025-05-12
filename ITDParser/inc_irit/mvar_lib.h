/******************************************************************************
* MVar_lib.h - header file for the multi variate library.		     *
* This header is also the interface header to the world.		     *
*******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute	of Technology		     *
*******************************************************************************
* Written by Gershon Elber, May. 96.					     *
******************************************************************************/

#ifndef IRIT_MVAR_LIB_H
#define IRIT_MVAR_LIB_H

#include <stdio.h>
#include "inc_irit/irit_sm.h"
#include "inc_irit/geom_lib.h"
#include "inc_irit/cagd_lib.h"
#include "inc_irit/trim_lib.h"
#include "inc_irit/triv_lib.h"
#include "inc_irit/mdl_lib.h"

typedef enum {
    MVAR_ERR_DIR_NOT_VALID,
    MVAR_ERR_UNDEF_CRV,
    MVAR_ERR_UNDEF_SRF,
    MVAR_ERR_UNDEF_MVAR,
    MVAR_ERR_UNDEF_GEOM,
    MVAR_ERR_GEOM_NO_SUPPORT,
    MVAR_ERR_PERIODIC_NO_SUPPORT,
    MVAR_ERR_RATIONAL_NO_SUPPORT,
    MVAR_ERR_RATIONAL_EXPECTED,
    MVAR_ERR_WRONG_ORDER,
    MVAR_ERR_KNOT_NOT_ORDERED,
    MVAR_ERR_NUM_KNOT_MISMATCH,
    MVAR_ERR_INDEX_NOT_IN_MESH,
    MVAR_ERR_POWER_NO_SUPPORT,
    MVAR_ERR_WRONG_DOMAIN,
    MVAR_ERR_INCONS_DOMAIN,
    MVAR_ERR_SCALAR_PT_EXPECTED,
    MVAR_ERR_WRONG_PT_TYPE,
    MVAR_ERR_INVALID_AXIS,
    MVAR_ERR_NO_CLOSED_POLYGON,
    MVAR_ERR_TWO_INTERSECTIONS,
    MVAR_ERR_NO_MATCH_PAIR,
    MVAR_ERR_FAIL_READ_FILE,
    MVAR_ERR_INVALID_STROKE_TYPE,
    MVAR_ERR_READ_FAIL,
    MVAR_ERR_MVS_INCOMPATIBLE,
    MVAR_ERR_PT_OR_LEN_MISMATCH,
    MVAR_ERR_TOO_FEW_PARAMS,
    MVAR_ERR_TOO_MANY_PARAMS,
    MVAR_ERR_FAIL_CMPT,
    MVAR_ERR_NO_CROSS_PROD,
    MVAR_ERR_BEZIER_EXPECTED,
    MVAR_ERR_BSPLINE_EXPECTED,
    MVAR_ERR_BEZ_OR_BSP_EXPECTED,
    MVAR_ERR_SAME_GTYPE_EXPECTED,
    MVAR_ERR_SAME_PTYPE_EXPECTED,
    MVAR_ERR_ONE_OR_THREE_EXPECTED,
    MVAR_ERR_POWER_EXPECTED,
    MVAR_ERR_MSC_TOO_FEW_OBJ,
    MVAR_ERR_MSC_FAILED,
    MVAR_ERR_MSS_INCONSISTENT_NUM_OBJ,
    MVAR_ERR_SCALAR_EXPECTED,
    MVAR_ERR_DIM_TOO_HIGH,
    MVAR_ERR_INVALID_MV,
    MVAR_ERR_CANNT_MIX_BSP_BEZ,
    MVAR_ERR_CH_FAILED,
    MVAR_ERR_MSC_CURVES,
    MVAR_ERR_ROUND_CURVE,
    MVAR_ERR_ONLY_2D,
    MVAR_ERR_ONLY_3D,
    MVAR_ERR_2D_OR_3D,
    MVAR_ERR_1D_OR_3D,
    MVAR_ERR_WRONG_INDEX,
    MVAR_ERR_MSC_TOO_FEW_PTS,
    MVAR_ERR_ET_DFRNT_DOMAINS,
    MVAR_ERR_SRF_NOT_ADJ,
    MVAR_ERR_CURVATURE_CONT,
    MVAR_ERR_ZER_PRBLM_CNSTRCT,	
    MVAR_ERR_ZER_ORDER_CNSTRCT,
    MVAR_ERR_ZER_NUMERIC_TOO_EARLY,
    MVAR_ERR_ZER_SOL_CNSTRCT,
    MVAR_ERR_ZER_SCT_TOO_EARLY,
    MVAR_ERR_ZER_GZT_TOO_EARLY,
    MVAR_ERR_TWO_SAME_MVS,
    MVAR_ERR_TWO_SAME_INPUTS,
    MVAR_ERR_ZER_SINGULAR_SOLS,
    MVAR_ERR_ZER_CRT_PTS_NO_SUPPORT,
    MVAR_ERR_EXPR_TREE_NO_SUPPORT,
    MVAR_ERR_INVALID_INPUT,
    MVAR_ERR_INV_PROJ_FAILED,
    MVAR_ERR_INVALID_COMPOSE_INPUT,
    MVAR_ERR_EXPECTED_CLOSED_SRF,
    MVAR_ERR_DIV_BY_ZERO,
    MVAR_ERR_UNDEFINE_ERR
} IritMvarFatalErrorType;

typedef enum {
    MVAR_UNDEF_TYPE = 1240,
    MVAR_BEZIER_TYPE,
    MVAR_BSPLINE_TYPE,
    MVAR_POWER_TYPE
} MvarGeomType;

typedef enum {
    MVAR_CNSTRNT_ZERO = 1320,
    MVAR_CNSTRNT_ZERO_SUBDIV,    /* Examine zeros during subdiv. stage only. */
    MVAR_CNSTRNT_POSITIVE,  /* Examine positivity during subdiv. stage only. */
    MVAR_CNSTRNT_POSITIVE_PARTIAL,/* Partial positivity in subdiv.stage only.*/
    MVAR_CNSTRNT_NEGATIVE,  /* Examine negativity during subdiv. stage only. */
    MVAR_CNSTRNT_NEGATIVE_PARTIAL /* Partial negativity on subdiv.stage only.*/
} MvarConstraintType;

/* This flag determines what to do with domain's center upon reaching subdiv */
/* tolerance, in zero dim solutions.					     */
typedef enum {
    MVAR_ZERO_ALWAYS_PURGE = 0, /* Return nothing when reaching subdiv tol'. */
    MVAR_ZERO_NEVER_PURGE,            /* Return the domain's center, always. */
    MVAR_ZERO_RETURN_VERIFIED,    /* Return the center if L1 error is small. */
    MVAR_ZERO_NUMERIC_STEP,   /* Try to improve numerically from the center. */
    MVAR_ZERO_NUMERIC_STEP_VERIFIED   /* Improve numerically from the center */
                     /* and then verify the L1 error of the answer is small. */
} MvarZeroSubdivTolActionType;

/* The following type should match CagdPointType for the shared domain.      */
typedef enum {		/* Type of control point. The P-types are rationals. */
    MVAR_PT_NONE = CAGD_PT_NONE,
    MVAR_PT_BASE = CAGD_PT_BASE,                  /* Must be an even number. */
    MVAR_PT_E1_TYPE = CAGD_PT_E1_TYPE,
    MVAR_PT_P1_TYPE,
    MVAR_PT_E2_TYPE,
    MVAR_PT_P2_TYPE,
    MVAR_PT_E3_TYPE,
    MVAR_PT_P3_TYPE,
    MVAR_PT_E4_TYPE,
    MVAR_PT_P4_TYPE,
    MVAR_PT_E5_TYPE,
    MVAR_PT_P5_TYPE,
    MVAR_PT_E6_TYPE,
    MVAR_PT_P6_TYPE,
    MVAR_PT_E7_TYPE,
    MVAR_PT_P7_TYPE,
    MVAR_PT_E8_TYPE,
    MVAR_PT_P8_TYPE,
    MVAR_PT_E9_TYPE,
    MVAR_PT_P9_TYPE,
    MVAR_PT_E10_TYPE,
    MVAR_PT_P10_TYPE,
    MVAR_PT_E11_TYPE,
    MVAR_PT_P11_TYPE,
    MVAR_PT_E12_TYPE,
    MVAR_PT_P12_TYPE,
    MVAR_PT_E13_TYPE,
    MVAR_PT_P13_TYPE,
    MVAR_PT_E14_TYPE,
    MVAR_PT_P14_TYPE,
    MVAR_PT_E15_TYPE,
    MVAR_PT_P15_TYPE,
    MVAR_PT_E16_TYPE,
    MVAR_PT_P16_TYPE,
    MVAR_PT_E17_TYPE,
    MVAR_PT_P17_TYPE,
    MVAR_PT_E18_TYPE,
    MVAR_PT_P18_TYPE,
    MVAR_PT_E19_TYPE,
    MVAR_PT_P19_TYPE,
    MVAR_PT_MAX_SIZE_TYPE	     /* See also MVAR_MAX_* constants below. */
} MvarPointType;

#define MVAR_MAX_PT_SIZE		20    /* Rational P19 has 20 coords. */
#define MVAR_MAX_PT_COORD		19		       /* Without w. */

typedef enum {
    MVAR_SK2D_PRIM_POINT,
    MVAR_SK2D_PRIM_LINE,
    MVAR_SK2D_PRIM_ARC,
    MVAR_SK2D_PRIM_CRV
} MvarSkel2DPrimType;

typedef enum {
    MVAR_ET_NODE_NONE,
    MVAR_ET_NODE_LEAF,
    MVAR_ET_NODE_ADD,
    MVAR_ET_NODE_SUB,
    MVAR_ET_NODE_MULT,
    MVAR_ET_NODE_SCALAR_MULT,
    MVAR_ET_NODE_MERGE,
    MVAR_ET_NODE_DOT_PROD,
    MVAR_ET_NODE_CROSS_PROD,
    MVAR_ET_NODE_EXP,
    MVAR_ET_NODE_LOG,
    MVAR_ET_NODE_COS,
    MVAR_ET_NODE_SQRT,
    MVAR_ET_NODE_SQR,
    MVAR_ET_NODE_NPOW,
    MVAR_ET_NODE_RECIP,
    MVAR_ET_NODE_COMMON_EXPR
} MvarExprTreeNodeType;

typedef enum {
    MVAR_ZER_SLVR_UNKNOWN,
    MVAR_ZER_SLVR_MVS,
    MVAR_ZER_SLVR_EXP_TREE,
    MVAR_ZER_SLVR_TRANSCEND,
    MVAR_ZER_SLVR_SOLUTION_POLYLINE,
    MVAR_ZER_SLVR_SOLUTION_PT_LIST,
    MVAR_ZER_SLVR_SOLUTION_TR_LIST,
    MVAR_ZER_SLVR_SOLUTION_DETECT
} MvarZrSlvrRepresentationType;

typedef enum {
    MVAR_MVD_NO_DECOMPOSITION,
    MVAR_MVD_DECOMPOSITION_COMPOSITION,
    MVAR_MVD_DECOMPOSITION_POINT_TRACING
} MvarMVDDecompositionModeType;

typedef int MvarMVDirType;
typedef CagdRType MvarMinMaxType[2];

typedef struct MvarComposedSrfStruct {
    struct MvarComposedSrfStruct *Pnext;
    struct IPAttributeStruct *Attr;
    CagdBType IsTrimmedSurface;
    union {
	CagdSrfStruct *Srf;
	TrimSrfStruct *TSrf;
    } U;
} MvarComposedSrfStruct;

typedef struct MvarComposedTrivStruct {
    struct MvarComposedTrivStruct *Pnext;
    struct IPAttributeStruct *Attr;
    CagdBType IsVMdl;
    union MyUnion {
	TrivTVStruct *TV;
	struct VMdlVModelStruct *VMdl;
    } U;
} MvarComposedTrivStruct;

/*  This struct will hold the result of the constructed surface based       */
/*  kernel. The struct variables described as follows:                      */
/*  ResStatus          0 - Faild to construct a valid surface.              */
/*  		       1 - Successed to construct a valid surface without   */
/*                         add more dofs to the input curves.               */
/*		       2 - Successed to construct a valid surface with add  */
/*                         more dofs to the input curves.		    */
/*  DegRaisItrNum:     Hold the number of degree raising iteration needed   */
/*                     to construct a surface with a valid parameterization.*/
/*                     -1, if not succeeded.                                */
/*  KnotInsItrNum:     Hold the number of knot insertion iteration needed   */
/*                     to construct a surface with a valid parameterization.*/
/*                     -1, if not succeeded.                                */
/*  KrnlSrf:           The kernel surface constructed upon the given input  */
/*                     curves, without add more dofs.                       */
/*  RegularSrf:        The regular surface constructed by using the regular */
/*                     operator on the given input curves.                  */
/*  InitCpStateSrf:    Holds the kernel surface initial state. i.e. before  */
/*		       moving the inner control points toward the kernel    */
/*                     point. The inner control points will be ordered in a */
/*                     unifrom gride. The surface construcetd upon the      */
/*                     given input curves.                                  */
/*  DegRaisSrf:        The kernel surface constructed by using the kernel   */
/*                     operator variation, while adding more dofs by        */
/*                     raising the degree of the input curves.              */
/*  DegRaisRglrSrf:    The regular surface constructed by using the regular */
/*                     operator, while adding more dofs by raising the      */
/*                     degree of the input curves.                          */
/*  DegRaisInitSrf:    Holds the kernel surface initial state. i.e. before  */
/*		       moving the inner control points toward the kernel    */
/*                     point. The inner control points will be ordered in a */
/*                     unifrom gride. The surface construcetd upon the      */
/*                     degree raised input curves.                          */
/*  KnotInsSrf:        The kernel surface constructed by using the kernel   */
/*                     operator variation, while adding more dofs by        */
/*                     inserting unifrom knots into the input curves.       */
/*  KnotInsRglrSrf:    The regular surface constructed by using the regular */
/*                     operator, while adding more dofs by inserting        */
/*                     unifrom knots into the input curves.                 */
/*  KnotInsInitSrf:    Holds the kernel surface initial state. i.e. before  */
/*		       moving the inner control points toward the kernel    */
/*                     point. The inner control points will be ordered in a */
/*                     unifrom gride. The surface construcetd upon the      */
/*                     refined input curves.                                */
/*  CnvxHull:          The convex hull of the evaluated kernel points. NULL */
/*                     If there is no kernel points or the convex hull      */
/*                     provided by the user and not computed.               */
typedef struct MvarPlnrKrnlSrfResStruct {
    int ResStatus;
    int DegRaisItrNum;
    int KnotInsItrNum;
    CagdSrfStruct *KrnlSrf;
    CagdSrfStruct *RegularSrf;
    CagdSrfStruct *InitCpStateSrf;
    CagdSrfStruct *DegRaisSrf;
    CagdSrfStruct *DegRaisRglrSrf;
    CagdSrfStruct *DegRaisInitSrf;
    CagdSrfStruct *KnotInsSrf;
    CagdSrfStruct *KnotInsRglrSrf;
    CagdSrfStruct *KnotInsInitSrf;
    struct IPObjectStruct *CnvxHull;
} MvarPlnrKrnlSrfResStruct;

/*  This struct will hold the result of the constructed trivariate based    */
/*  kernel. The struct variables described as follows:                      */
/*  ResStatus          0 - Faild to construct a valid trivariate.           */
/*  		       1 - Successed to construct a valid trivariate        */
/*                         without add more dofs to the input curves.       */
/*		       2 - Successed to construct a valid trivariate with   */
/*                         add more dofs to the input surfaces.		    */
/*  DegRaisItrNum:     Hold the number of degree raising iteration needed   */
/*                     to construct a trivariate with a valid               */
/*                     parameterization. -1, if not succeeded.              */
/*  KnotInsItrNum:     Hold the number of knot insertion iteration needed   */
/*                     to construct a trivariate with a valid               */
/*                     parameterization. -1, if not succeeded.              */
/*  KrnlTV:            The kernel trivariate constructed upon the given     */
/*                     input curves, without add more dofs.                 */
/*  RegularTV:         The regular trivariate constructed by using the      */
/*                     regular operator on the given input surfaces.        */
/*  InitCpStateTV:     Holds the kernel trivariate initial state. i.e.      */
/*		       before moving the inner control points toward the    */
/*                     kernel point. The inner control points will be       */
/*                     ordered in a unifrom gride. The trivariate           */
/*                     construcetd upon the  given input surfaces.          */
/*  DegRaisTV:         The kernel trivariate constructed by using the       */
/*                     kernel operator variation, while adding more dofs by */
/*                     raising the degree of the input surfaces.            */
/*  DegRaisRglrTV:     The regular trivariate constructed by using the      */
/*                     regular operator, while adding more dofs by raising  */
/*                     the degree of the input surfaces.                    */
/*  DegRaisInitTV:     Holds the kernel trivariate initial state. i.e.      */
/*		       before moving the inner control points toward the    */
/*                     kernel point. The inner control points will be       */
/*                     ordered in a unifrom gride. The trivariate           */
/*                     construcetd upon the  degree raised input surfaces.  */
/*  KnotInsTV:         The kernel trivariate constructed by using the       */
/*                     kernel operator variation, while adding more dofs by */
/*                     inserting unifrom knots into the input surfaces.     */
/*  KnotInsRglrTV:     The regular trivariate constructed by using the      */
/*                     regular operator, while adding more dofs by          */
/*                     inserting unifrom knots into the input surfaces.     */
/*  KnotInsInitTV:     Holds the kernel trivariate initial state. i.e.      */
/*		       before moving the inner control points toward the    */
/*                     kernel point. The inner control points will be       */
/*                     ordered in a unifrom gride. The trivariate           */
/*                     construcetd upon the  refined input surfaces.        */
/*  KrnlPts:           The trivariaet inner kernel points. NULL             */
/*                     If there is no kernel points provided by the user.   */
typedef struct MvarKrnlTVResStruct 
{
    int ResStatus;
    int DegRaisItrNum;
    int KnotInsItrNum;
    TrivTVStruct *KrnlTV;
    TrivTVStruct *RegularTV;
    TrivTVStruct *InitCpStateTV;
    TrivTVStruct *DegRaisTV;
    TrivTVStruct *DegRaisRglrTV;
    TrivTVStruct *DegRaisInitTV;
    TrivTVStruct *KnotInsTV;
    TrivTVStruct *KnotInsRglrTV;
    TrivTVStruct *KnotInsInitTV;
    struct IPObjectStruct *KrnlPts;
    IrtPtType KrnlPt;
} MvarKrnlTVResStruct;

/* This struct will hold the input of the kernel based operators for        */
/* planar surface constructions. The operators can be either Boolan sum,    */
/* one sided Boolean sum, or ruling. The struct variables described as      */
/* follows:                                                                 */
/* Crvs:             The parametric freeform boundary input curves.         */
/* DistRatio:	        For zero to one values: describes the translation   */
/*                   ratio of the inner control points towards the kernel.  */
/*                      For negatives values: organize the inner control    */
/*                   points in a grid which it centered at the kernel       */
/*                   ceneter, and abs(DistRatio) variable describe the      */
/*                   length between two consecutive points at the grid.     */
/* MaxItr:           A limit on the iterative process of adding degrees     */
/*                   of freedom (Amount of degree raisings/refinements).    */
/* DegOfFrdmOp:      If zero, the degree raising operator is used to add    */
/*                   degrees fo freedom. Otherwise, the refinement oprator  */
/*		     is used.                                               */
/* SubEps:           Subdivision epsilon.   For example,                    */
/*		     0.005 - For good kernel results, expensive             */
/*                           computation time.                              */
/*                   0.01  - For resonable kernel results, resonable        */
/*                           computation time.                              */
/* IsSingular:       If FALSE only a surface with one-sign Jacobian will be */
/*                   consider valid. IF TRUE, singular locations (and       */
/*                   negative Jacobians) will be allowed.                   */
/* PCnvxHull:        Optional parameter, The user can provide the kernel    */
/*                   convex hull through it. To save computation time.      */
typedef struct MvarPlnrKrnlSrfInptStruct {
    CagdCrvStruct *Crvs[4];
    IrtRType DistRatio;
    int MaxItr;
    int DegOfFrdmOp;
    IrtRType SubEps;
    int IsSingular;
    struct IPObjectStruct *PCnvxHull;
} MvarPlnrKrnlSrfInptStruct;

/* This struct will hold the input of the kernel based operators for        */
/* trivariate constructions. The operators can be either Boolan sum, one    */
/* sided Boolean sum from {2/3} adjacent surfaces, or ruling. The struct    */
/* variables described as follows:                                          */
/* Crvs:             The parametric freeform boundary input surfaces.       */
/* DistRatio:	        For zero to one values: describes the translation   */
/*                   ratio of the inner control points towards the kernel.  */
/*                       For negatives values: organize the inner control   */
/*                   points in a grid which it centered at the kernel       */
/*                   ceneter, and abs(DistRatio) variable describe the      */
/*                   length between two consecutive points at the grid.     */
/* MaxItr:           A limit on the iterative process of adding degrees     */
/*                   of freedom. (Amount of degree raisings/refinements).   */
/* DegOfFrdmOp:      If zero, the degree raising operator is used to add    */
/*                   degrees fo freedom. Otherwise, the refinement oprator  */
/*		     is used.                                               */
/* SubEps:           Subdivision epsilon.   For example,                    */
/*		     0.005 - For good kernel results, expensive             */
/*                           computation time.                              */
/*                   0.01  - For resonable kernel results, resonable        */
/*                           computation time.                              */
/* IsSingular:       If FALSE only a surface with one-sign Jacobian will be */
/*                   consider valid. IF TRUE, singular locations (and       */
/*                   negative Jacobians) will be allowed.                   */
/* PKrnlPts:         Optional parameter, The user can provide the kernel    */
/*                   points. To save computation time.                      */
typedef struct MvarKrnlTVInptStruct {
    CagdSrfStruct *Srfs[6];
    IrtRType DistRatio;
    int MaxItr;
    int DegOfFrdmOp;
    IrtRType SubEps;
    int IsSingular;
    struct IPObjectStruct *PKrnlPts;
} MvarKrnlTVInptStruct;

#define MVAR_HF_DIST_MAX_PARAM		3

#define MVAR_IS_RATIONAL_PT(PType)  ((int) ((PType) & 0x01))
#define MVAR_IS_RATIONAL_MV(MV)		MVAR_IS_RATIONAL_PT((MV) -> PType)
#define MVAR_NUM_OF_PT_COORD(PType) ((((int) ((PType) - MVAR_PT_BASE)) >> 1) + 1)
#define MVAR_NUM_OF_MV_COORD(MV)    ((((int) (((MV) -> PType) - \
				              MVAR_PT_BASE)) >> 1) + 1)
#define MVAR_MAKE_PT_TYPE(IsRational, NumCoords) \
				    ((MvarPointType) (MVAR_PT_BASE + \
				         ((((IsRational) ? -1 : -2) \
						       + ((NumCoords) << 1)))))

#define MVAR_PREV_DIR(Dir) ((Dir) + 1)
#define MVAR_NEXT_DIR(Dir) ((Dir) - 1)

#define MVAR_PT_RESET(P) IRIT_ZAP_MEM((P) -> Pt, (P) -> Dim * sizeof(CagdRType))
#define MVAR_VEC_RESET(V) IRIT_ZAP_MEM((V) -> Vec, (V) -> Dim * sizeof(CagdRType))
#define MVAR_PLANE_RESET(P) IRIT_ZAP_MEM((P) -> Pln, (P) -> Dim * sizeof(CagdRType))

#define MVAR_PT_COPY(Dst, Src) IRIT_GEN_COPY((Dst) -> Pt, (Src) -> Pt, \
				             (Dst) -> Dim * sizeof(CagdRType))
#define MVAR_VEC_COPY(Dst, Src) IRIT_GEN_COPY((Dst) -> Vec, (Src) -> Vec, \
				              (Dst) -> Dim * sizeof(CagdRType))
#define MVAR_PLANE_COPY(Dst, Src) IRIT_GEN_COPY((Dst) -> Plane, (Src) -> Plane, \
				                (Dst) -> Dim * sizeof(CagdRType))

#define MVAR_MV_EVAL_SCALAR(Mv, Params, PtE1) \
		{ CagdRType _R[MVAR_MAX_PT_SIZE]; \
		  IritMvarMVEvalToData((Mv), (Params), _R); \
		  *(PtE1) = MVAR_IS_RATIONAL_MV((Mv)) ? _R[1] / _R[0] : _R[1]; }
#define MVAR_MV_EVAL_E2(Mv, Params, PtE2) \
		{ CagdRType _R[MVAR_MAX_PT_SIZE], *PR = _R; \
		  IritMvarMVEvalToData((Mv), (Params), _R); \
		  IritCagdCoerceToE2(PtE2, &PR, -1, (CagdPointType) ((Mv) -> PType)); }
#define MVAR_MV_EVAL_E3(Mv, Params, PtE3) \
		{ CagdRType _R[MVAR_MAX_PT_SIZE], *PR = _R; \
		  IritMvarMVEvalToData((Mv), (Params), _R); \
		  IritCagdCoerceToE3(PtE3, &PR, -1, (CagdPointType) ((Mv) -> PType)); }
#define MVAR_MV_EVAL_P2(Mv, Params, PtP2) \
		{ CagdRType _R[MVAR_MAX_PT_SIZE], *PR = _R; \
		  IritMvarMVEvalToData((Mv), (Params), _R); \
		  IritCagdCoerceToP2(PtP2, &PR, -1, (CagdPointType) ((Mv) -> PType)); }
#define MVAR_MV_EVAL_P3(Mv, Params, PtP3) \
		{ CagdRType _R[MVAR_MAX_PT_SIZE], *PR = _R; \
		  IritMvarMVEvalToData((Mv), (Params), _R); \
		  IritCagdCoerceToP3(PtP3, &PR, -1, (CagdPointType) ((Mv) -> PType)); }

#define MVAR_MVS_ZERO_INIT_PROBLEM_SPEC(ZeroProblemSpec, Multivars, \
				        Cnstrs, NumMVs, \
				        SubTol, NumTol, StpTol) { \
    IRIT_ZAP_MEM(&ZeroProblemSpec, sizeof(MvarZeroPrblmSpecStruct)); \
    ZeroProblemSpec.U.MVs = Multivars; \
    ZeroProblemSpec.Constraints = Cnstrs; \
    ZeroProblemSpec.NumOfMVs = NumMVs; \
    ZeroProblemSpec.SubdivTol = SubTol; \
    ZeroProblemSpec.NumericTol = NumTol; \
    ZeroProblemSpec.StepTol = StpTol; \
}

#define MVAR_EQS_ZERO_INIT_PROBLEM_SPEC(ZeroProblemSpec, ExprTreeEqns, \
				        Cnstrs, NumMVs, \
				        SubTol, NumTol, StpTol) { \
    IRIT_ZAP_MEM(&ZeroProblemSpec, sizeof(MvarZeroPrblmSpecStruct)); \
    ZeroProblemSpec.U.MVETs = ExprTreeEqns; \
    ZeroProblemSpec.Constraints = Cnstrs; \
    ZeroProblemSpec.NumOfMVs = NumMVs; \
    ZeroProblemSpec.SubdivTol = SubTol; \
    ZeroProblemSpec.NumericTol = NumTol; \
    ZeroProblemSpec.StepTol = StpTol; \
}

typedef struct MvarPtStruct {
    struct MvarPtStruct *Pnext;
    struct IPAttributeStruct *Attr;
    int Dim;				     /* Number of coordinates in Pt. */
    CagdRType *Pt;	       /* The coordinates of the multivariate point. */
} MvarPtStruct;

typedef struct MvarVecStruct {
    struct MvarVecStruct *Pnext;
    struct IPAttributeStruct *Attr;
    int Dim;				    /* Number of coordinates in Vec. */
    CagdRType *Vec;	      /* The coordinates of the multivariate vector. */
} MvarVecStruct;

typedef struct MvarPolylineStruct {		    /* A polyline structure. */
    struct MvarPolylineStruct *Pnext;
    struct IPAttributeStruct *Attr;
    MvarPtStruct *Pl;
    VoidPtr PAux;
} MvarPolylineStruct;

typedef struct MvarTriangleStruct {		    /* A triangle structure. */
    struct MvarTriangleStruct *Pnext;		    
    int Dim;
    CagdRType *Vrtcs[3];
    CagdRType *Nrmls[3];
} MvarTriangleStruct;

typedef struct MvarPlaneStruct {
    struct MvarPlaneStruct *Pnext;
    struct IPAttributeStruct *Attr;
    int Dim;    /* Number of coordinates in Pln (one above space dimension). */
    CagdRType *Pln;	       /* The coordinates of the multivariate plane. */
} MvarPlaneStruct;


typedef struct GMBBBboxStruct MvarBBoxStruct;

typedef struct MvarNormalConeStruct {        /* Normalized cone axis vector. */
    struct MvarNormalConeStruct *Pnext;
    struct IPAttributeStruct *Attr;
    MvarVecStruct *ConeAxis;
    CagdRType ConeAngleCosine;
    CagdRType AxisMinMax[2];
} MvarNormalConeStruct;

typedef struct MvarMVStruct {
    struct MvarMVStruct *Pnext;
    struct IPAttributeStruct *Attr;
    MvarGeomType GType;
    MvarPointType PType;
    int Dim;		      /* Number of dimensions in this multi variate. */
    int *Lengths;               /* Dimensions of mesh size in multi-variate. */
    int *SubSpaces;	   /* SubSpaces[i] = Prod(i = 0, i-1) of Lengths[i]. */
    int *Orders;                 /* Orders of multi variate (B-spline only). */
    CagdBType *Periodic;           /* Periodicity - valid only for B-spline. */
    CagdRType *Points[MVAR_MAX_PT_SIZE];     /* Pointer on each axis vector. */
    CagdRType **KnotVectors;
    MvarMinMaxType *AuxDomain;		      /* Optional to hold MV domain. */
    VoidPtr PAux;			        /* Auxiliary data structure. */
    VoidPtr PAux2;			        /* Auxiliary data structure. */
} MvarMVStruct;

typedef struct MvarExprTreeStruct {
    MvarExprTreeNodeType NodeType;
    int Dim;
    int PtSize;					    /* vector function size. */
#ifndef SUN4                       /* No support form nameless union/struct. */
    union {
	struct {
#endif /* SUN4 */
	    MvarMVStruct *MV;
	    CagdBType IsRef;       /* TRUE if an MV reference - do not free. */
#ifndef SUN4
	}; /* Leaf node. */
	struct {
#endif /* SUN4 */
	    struct MvarExprTreeStruct *Left, *Right;
#ifndef SUN4
	}; /* Internal node. */
    };
#endif /* SUN4 */
    MvarNormalConeStruct *MVBCone;
    MvarBBoxStruct MVBBox;
    int Val;			             /* Integer value for constants. */
    int IAux, IAux2;		     /* Auxiliary integers for internal use. */
    VoidPtr PAux;		      /* Auxiliary pointer for internal use. */
    char *Info;			   /* Optional info on this expression tree. */
} MvarExprTreeStruct;

typedef struct MvarMVGradientStruct {
    int Dim;
    CagdBType IsRational, HasOrig;
    MvarMVStruct *MV;			       /* The original multivariate. */
    MvarMVStruct *MVGrad;		    /* The gradient if not rational. */
    MvarMVStruct *MVRGrad[MVAR_MAX_PT_COORD + 1];  /* The grad. if rational. */
} MvarMVGradientStruct;

/* Eqns structure - holds a set of expression trees and related info.        */
typedef struct MvarExprTreeEqnsStruct {
    MvarExprTreeStruct **Eqns;		          /* The equations to solve. */
    int NumEqns, NumZeroEqns, NumZeroSubdivEqns;
    MvarExprTreeStruct **CommonExprs;       /* The common expressions found. */
    int NumCommonExprs, MaxNumCommonExprs;
    MvarConstraintType *ConstraintTypes;
} MvarExprTreeEqnsStruct;

/* The general zero finding problem structures. */
typedef struct MvarZeroPrblmStruct {
    struct MvarZeroPrblmStruct *Pnext;
    struct IPAttributeStruct *Attr;
    union {
	MvarMVStruct **MVs;
	MvarExprTreeEqnsStruct *Eqns;
    } U;
    MvarZrSlvrRepresentationType ActiveRepresentation;
    MvarConstraintType *Constraints;
    int NumOfConstraints;
    int NumOfZeroConstraints;
    CagdRType SubdivTol;
    CagdRType NumericTol;
    CagdRType StepTol;
    CagdBType OnlyDtctSol;   /* TRUE to only detect existence of a solution. */
    int *NoSubdivDirs;/* Optional vector of disabled subdivision directions. */
    struct MvarMVZR1DAuxStruct *AS;
    struct MvarZeroPrblmIntrnlStruct *_Internal;
} MvarZeroPrblmStruct;

typedef struct MvarSkel2DPrimPointStruct {
    CagdPType Pt;
} MvarSkel2DPrimPointStruct;

typedef struct MvarSkel2DPrimLineStruct {
    CagdPType Pt1, Pt2;
} MvarSkel2DPrimLineStruct;

typedef struct MvarSkel2DPrimArcStruct {
    CagdPType Center;
    IrtRType StartAngle, EndAngle, Radius;
} MvarSkel2DPrimArcStruct;

typedef struct MvarSkel2DPrimCrvStruct {
    CagdCrvStruct *Crv;
} MvarSkel2DPrimCrvStruct;

typedef struct MvarSkel2DPrimStruct {
    struct MvarSkel2DPrimStruct *Pnext;
    struct IPAttributeStruct *Attr;
    MvarSkel2DPrimType Type;
#ifndef SUN4                       /* No support form nameless union/struct. */
    union {
#endif /* SUN4 */
        MvarSkel2DPrimPointStruct Pt;
        MvarSkel2DPrimLineStruct Ln;
        MvarSkel2DPrimArcStruct Arc;
        MvarSkel2DPrimCrvStruct Crv;
#ifndef SUN4
    };
#endif /* SUN4 */
    int _Index;
    CagdCrvStruct *_CrvRep;
} MvarSkel2DPrimStruct;

typedef struct IritMvarSkel2DInter3PrimsStruct {
    struct IritMvarSkel2DInter3PrimsStruct *Pnext;
    struct IPAttributeStruct *Attr;
    CagdPType PosPrim1, PosPrim2, PosPrim3;
    CagdPType EquadistPoint;
} IritMvarSkel2DInter3PrimsStruct;

typedef struct MvarHFDistParamStruct {
    int NumOfParams;  /* Number of locations where the Hausdorff dist holds. */
    int ManifoldDim;                        /* 1 for curves, 2 for surfaces. */
#ifndef SUN4                       /* No support form nameless union/struct. */
    union {
#endif /* SUN4 */
	CagdRType T[MVAR_HF_DIST_MAX_PARAM];		/* Params of curves. */
	CagdUVType UV[MVAR_HF_DIST_MAX_PARAM];	      /* Params of surfaces. */
#ifndef SUN4
    };
#endif /* SUN4 */
} MvarHFDistParamStruct;

typedef struct MvarHFDistPairParamStruct {
    struct MvarHFDistPairParamStruct *Pnext;
    MvarHFDistParamStruct Param1, Param2;	 /* Param. info of the pair. */
    CagdRType Dist;		     /* Euclidean distance at this location. */
} MvarHFDistPairParamStruct;

/* Matlab interface for equation solving. */
typedef struct MvarMatlabEqStruct {
    int NumOfMonoms;		     /* Number of monomials in the equation. */
    CagdRType *CoeffArr;		     /* The coeffs of each monomial. */
    int *PowersMat;    /* The power of variable j (col) in monomial i (row). */
    int *MaxPowers;	  /* Maximal power of each variable in the equation. */
} MvarMatlabEqStruct;

/* Data structures for Srf-Srf intersection cache. */
typedef struct IritMvarSrfSrfInterCacheDataStruct {
    struct IritMvarSrfSrfInterCacheDataStruct *Pnext;
    int Id1;
    int Id2;
    CagdRType KnotSpan1[4];
    CagdRType KnotSpan2[4];
    MvarPolylineStruct *InterRes;
} IritMvarSrfSrfInterCacheDataStruct;

typedef struct IritMvarSrfSrfInterCacheStruct {
    IritMvarSrfSrfInterCacheDataStruct *CacheData;
    IPAttrIDType AttribID;
    int NextId;
} IritMvarSrfSrfInterCacheStruct;

typedef void (*MvarExprTreePrintFuncType)(const char *s);
typedef void (*MvarSetErrorFuncType)(IritMvarFatalErrorType ErrorFunc);
typedef int (*MvarMVsZerosMVsCBFuncType)(MvarMVStruct **MVs,
					 int n,
					 void *AuxData);
typedef int (*MvarMVsZerosSubdivCBFuncType)(MvarZeroPrblmStruct *p,
					    int i,
					    void *AuxData,
					    int *IsTopoGuarantee);
typedef int (*MvarMVsZerosVerifyOneSolPtCBFuncType)(MvarPtStruct *Pt,
						    void *AuxData);
typedef int (*MvarMVsZerosVerifyAllSolsCBFuncType)(MvarPolylineStruct **MVPls,
						   void *AuxData);
typedef CagdRType *(*MvarMapPrm2EucCBFuncType)(
					    CagdRType *R,
					    int n,
					    CagdRType OutParam[3],
					    void *AuxData);

/* Possible external call backs exposed to end users in the zeros solver. */
typedef struct MvarZeroPrblmExternalCBStruct {
    /* All possible call back functions/data. */
    MvarSetErrorFuncType SetErrorFunc;           /* Not used at this time. */
    void *SetErrorData1;
    MvarExprTreePrintFuncType ExprTreePrintFunc; /* Not used at this time. */
    void *ZerosSubdivCBData;
    /* Alternative termination function for the subdivision process. */
    MvarMVsZerosSubdivCBFuncType MVsZerosSubdivFunc;
    void *MVsZerosSubdivData;
    /* Optional verification/filtering of solution points. */
    MvarMVsZerosVerifyOneSolPtCBFuncType MVsZerosVerifyOneSolPtFunc;
    void *MVsZerosVerifyOneSolPtData;
    /* Optional verification/filtering of general solutions. */
    MvarMVsZerosVerifyAllSolsCBFuncType MVsZerosVerifyAllSolsFunc;
    void *MVsZerosVerifyAllSolsData;
    /* Allows in-place mapping of solutions from MV solver parametric space */
    /* to actual solution space, typically Euclidean.			    */
    MvarMapPrm2EucCBFuncType MapPtParamSpace2EuclidSpace;
    void *MapPtParamSpace2EuclidData;
    /* Allows in-place mapping of solution normals from MV solver           */
    /* parametric space to actual solution space, typically Euclidean.      */
    MvarMapPrm2EucCBFuncType MapPtParamSpace2EuclidNormal;
    void *MapPtParamSpace2EuclidNormalData;
    MvarMVsZerosMVsCBFuncType CnvrtETs2MVsFunc;   /* Not used at this time. */
    void *CnvrtETs2MVsData;
} MvarZeroPrblmExternalCBStruct;

/* The general zero finding problem specification and solution structures. */
typedef struct MvarZeroPrblmSpecStruct {
    union {
	 MvarMVStruct * const *MVs;
	 MvarExprTreeStruct * const *MVETs;
    } U;
    int NumOfMVs;
    MvarConstraintType *Constraints;
    CagdRType SubdivTol;
    CagdRType NumericTol;
    CagdRType StepTol;

    /* All possible call back functions/data. */
    MvarZeroPrblmExternalCBStruct ECB;

    int _HighDimBndry;				        /* Used internally. */
} MvarZeroPrblmSpecStruct;

typedef struct MvarZeroSolutionStruct {
    struct MvarZeroSolutionStruct *Pnext;
    struct IPAttributeStruct *Attr;
    union {
	MvarPtStruct *Pt;
	MvarPolylineStruct *Pl;
	MvarTriangleStruct *Tr;
	CagdBType DtctSol;
    } U;
    MvarZrSlvrRepresentationType ActiveRepresentation;
    struct MvarZeroTJunctionStruct *TJList;/*of current problem, not handled.*/
} MvarZeroSolutionStruct;

#define MVAR_MALLOC_STRUCT_ONCE     /* Faster allocation of MVAR structures. */

#define MVAR_IS_BEZIER_MV(MV)		((MV) -> GType == MVAR_BEZIER_TYPE)
#define MVAR_IS_POWER_MV(MV)		((MV) -> GType == MVAR_POWER_TYPE)
#define MVAR_IS_BSPLINE_MV(MV)		((MV) -> GType == MVAR_BSPLINE_TYPE)

#define MVAR_CTL_MESH_LENGTH(MV)	((MV) -> SubSpaces[(MV) -> Dim])

/******************************************************************************
*  Provides easy access	to multivariates up to dimension six.		     *
******************************************************************************/
#define MVAR_NEXT_U(MV)			(1)         /* == MV -> SubSpaces[0] */
#define MVAR_NEXT_V(MV)			((MV) -> SubSpaces[1])
#define MVAR_NEXT_W(MV)			((MV) -> SubSpaces[2])
#define MVAR_NEXT_FOURTH(MV)		((MV) -> SubSpaces[3])
#define MVAR_NEXT_FIFTH(MV)		((MV) -> SubSpaces[4])
#define MVAR_NEXT_SIXTH(MV)		((MV) -> SubSpaces[5])
#define MVAR_NEXT_DIM(MV, Dim)		((MV) -> SubSpaces[(Dim)])

#define MVAR_MESH_UV(MV, i, j)		((i) + \
					 ((MV) -> SubSpaces[1]) * (j))
#define MVAR_MESH_UVW(MV, i, j, k)	((i) + \
					 ((MV) -> SubSpaces[1]) * (j) + \
					 ((MV) -> SubSpaces[2]) * (k))
#define MVAR_MESH_UVW4(MV, i, j, k, l)  ((i) + \
					 ((MV) -> SubSpaces[1]) * (j) + \
					 ((MV) -> SubSpaces[2]) * (k) + \
					 ((MV) -> SubSpaces[3]) * (l))
#define MVAR_MESH_UVW45(MV, i, j, k, l, m) \
					((i) + \
					 ((MV) -> SubSpaces[1]) * (j) + \
					 ((MV) -> SubSpaces[2]) * (k) + \
					 ((MV) -> SubSpaces[3]) * (l) + \
					 ((MV) -> SubSpaces[4]) * (m))
#define MVAR_MESH_UVW456(MV, i, j, k, l, m, n) \
					((i) + \
					 ((MV) -> SubSpaces[1]) * (j) + \
					 ((MV) -> SubSpaces[2]) * (k) + \
					 ((MV) -> SubSpaces[3]) * (l) + \
					 ((MV) -> SubSpaces[4]) * (m) + \
					 ((MV) -> SubSpaces[5]) * (n))

/* If a mvarariate is periodic, the control polygon/mesh should warp up.     */
/* Length does hold the real allocated length but the virtual periodic       */
/* length is a little larger. Note allocated KV's are larger.                */
#define MVAR_MVAR_UPT_LST_LEN(MV)	((MV) -> Lengths[0] + \
			 ((MV) -> Periodic[0] ? (MV) -> Orders[0] - 1 : 0))
#define MVAR_MVAR_VPT_LST_LEN(MV)	((MV) -> Lengths[1] + \
			 ((MV) -> Periodic[1] ? (MV) -> Orders[1] - 1 : 0))
#define MVAR_MVAR_WPT_LST_LEN(MV)	((MV) -> Lengths[2] + \
			 ((MV) -> Periodic[2] ? (MV) -> Orders[2] - 1 : 0))
#define MVAR_MVAR_FOURTH_PT_LST_LEN(MV)	((MV) -> Lengths[3] + \
			 ((MV) -> Periodic[3] ? (MV) -> Orders[3] - 1 : 0))
#define MVAR_MVAR_FIFTH_PT_LST_LEN(MV)	((MV) -> Lengths[4] + \
			 ((MV) -> Periodic[4] ? (MV) -> Orders[4] - 1 : 0))
#define MVAR_MVAR_SIXTH_PT_LST_LEN(MV)	((MV) -> Lengths[5] + \
			 ((MV) -> Periodic[5] ? (MV) -> Orders[5] - 1 : 0))
#define MVAR_MVAR_ITH_PT_LST_LEN(MV, i)	((MV) -> Lengths[i] + \
			 ((MV) -> Periodic[i] ? (MV) -> Orders[i] - 1 : 0))

#define MVAR_IS_UPERIODIC_MVAR(MV)	((MV) -> Periodic[0])
#define MVAR_IS_VPERIODIC_MVAR(MV)	((MV) -> Periodic[1])
#define MVAR_IS_WPERIODIC_MVAR(MV)	((MV) -> Periodic[2])
#define MVAR_IS_FOURTH_PERIODIC_MVAR(MV) ((MV) -> Periodic[3])
#define MVAR_IS_FIFTH_PERIODIC_MVAR(MV) ((MV) -> Periodic[4])
#define MVAR_IS_SIXTH_PERIODIC_MVAR(MV) ((MV) -> Periodic[5])
#define MVAR_IS_ITH_PERIODIC_MVAR(MV, i) ((MV) -> Periodic[i])

/* Ease the handling of the splitting of a multivariate to scalar fields. */
#define MVAR_CLEAR_SCALARS(MV) { \
	int ii; \
	for (ii = 0; ii < MVAR_MAX_PT_SIZE; ii++) \
	    (MV)[ii] = NULL; \
    } 
#define MVAR_FREE_SCALARS(MV) { \
        int ii; \
        if ((MV)[0] != NULL) \
	    IritMvarMVFree((MV)[0]); \
	for (ii = 1; ii <= MVAR_MAX_PT_COORD; ii++) { \
	    if ((MV)[ii] == NULL) \
	        break; \
	    IritMvarMVFree((MV)[ii]); \
	} \
    }

#define MVAR_INCREMENT_MESH_INDICES(MV, Indices, Index) \
    (++(*Indices) >= MV -> Lengths[0] ? \
	_IritMvarIncrementMeshIndices(MV, Indices, &Index) : ++Index)

#define MVAR_INCREMENT_MESH_ORDER_INDICES(MV, Indices, Index) \
    (++(*Indices) >= MV -> Orders[0] ? \
	_IritMvarIncrementMeshOrderIndices(MV, Indices, &Index) : ++Index)

#define MVAR_INC_SKIP_MESH_INDICES_1ST(MV, Indices) \
    _IritMvarIncSkipMeshIndices1st(MV, Indices)

#define MVAR_INC_SKIP_MESH_INDICES(MV, Indices, SkipDir, Index) \
    (MV -> Dim <= 1 ? (Index = 0) : \
     (++(Indices[SkipDir == 0]) >= MV -> Lengths[SkipDir == 0] ? \
	 _IritMvarIncSkipMeshIndices(MV, Indices, SkipDir, &Index) : \
	 (Index += MVAR_NEXT_DIM(MV, SkipDir == 0))))

#define MVAR_INC_BOUND_MESH_INDICES(MV, Indices, LowBound, UpBound, Index) \
    (++(*Indices) >= UpBound[0] ? \
	_IritMvarIncBoundMeshIndices(MV, Indices, LowBound, UpBound, \
				 &Index) : ++Index)

#define MVAR_BBOX_RESET(BBox)    IRIT_ZAP_MEM(&BBox, sizeof(MvarBBoxStruct));

#define MVAR_BBOX_INIT(BBox, Dimen) { \
    int _m; \
    MVAR_BBOX_RESET(BBox); \
    (BBox).Dim = (Dimen); \
    for (_m = 0; _m < (Dimen); _m++) { \
	(BBox).Min[_m] = IRIT_INFNTY; \
	(BBox).Max[_m] = -IRIT_INFNTY; \
    } \
}
#define MVAR_IS_BBOX_RESET(BBox)  ((BBox).Dim == 0)

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/******************************************************************************
* General routines of the Mvar library:					     *
******************************************************************************/
MvarMVStruct *IritMvarMVNew(int Dim,
			    MvarGeomType GType,
			    MvarPointType PType,
			    const int *Lengths);
MvarMVStruct *IritMvarBspMVNew(int Dim,
			       const int *Lengths,
			       const int *Orders,
			       MvarPointType PType);
MvarMVStruct *IritMvarBzrMVNew(int Dim,
			       const int *Lengths,
			       MvarPointType PType);
MvarMVStruct *IritMvarPwrMVNew(int Dim,
			       const int *Lengths,
			       MvarPointType PType);
MvarMVStruct *MvarBuildParamMV(int Dim,
			       int Dir,
			       CagdRType Min,
			       CagdRType Max);
MvarMVStruct *IritMvarMVCopy(const MvarMVStruct *MV);
MvarMVStruct *IritMvarMVCopyList(const MvarMVStruct *MVList);
void IritMvarMVFree(MvarMVStruct *MV);
void IritMvarMVFreeList(MvarMVStruct *MVList);

#ifdef DEBUG
#define IritMvarMVFree(MV)         { IritMvarMVFree(MV); MV = NULL; }
#define IritMvarMVFreeList(MVList) { IritMvarMVFreeList(MVList); MVList = NULL; }
#endif /* DEBUG */

MvarPtStruct *IritMvarPtNew(int Dim);
MvarPtStruct *IritMvarPtRealloc(MvarPtStruct *Pt, int NewDim);
MvarPtStruct *IritMvarPtCopy(const MvarPtStruct *Pt);
MvarPtStruct *IritMvarPtCopyList(const MvarPtStruct *PtList);
MvarPtStruct *IritMvarPtSortListAxis(MvarPtStruct *PtList, int Axis);
void IritMvarPtFree(MvarPtStruct *Pt);
void IritMvarPtFreeList(MvarPtStruct *PtList);

#ifdef DEBUG
#define IritMvarPtFree(Pt)         { IritMvarPtFree(Pt); Pt = NULL; }
#define IritMvarPtFreeList(PtList) { IritMvarPtFreeList(PtList); PtList = NULL; }
#endif /* DEBUG */

MvarPtStruct *IritMvarPolyReverseList(MvarPtStruct *Pts);

MvarPolylineStruct *IritMvarPolylineNew(MvarPtStruct *Pl);
MvarPolylineStruct *IritMvarPolylineCopy(const MvarPolylineStruct *Poly);
MvarPolylineStruct *IritMvarPolylineCopyList(MvarPolylineStruct *PolyList);
void IritMvarPolylineFree(MvarPolylineStruct *Poly);
void IritMvarPolylineFreeList(MvarPolylineStruct *PolyList);

#ifdef DEBUG
#define IritMvarPolylineFree(Poly)         { IritMvarPolylineFree(Poly); Poly = NULL; }
#define IritMvarPolylineFreeList(PolyList) { IritMvarPolylineFreeList(PolyList); \
				             PolyList = NULL; }
#endif /* DEBUG */

MvarVecStruct *IritMvarVecNew(int Dim);
MvarVecStruct *IritMvarVecArrayNew(int Size, int Dim);
MvarVecStruct *IritMvarVecRealloc(MvarVecStruct *Vec, int NewDim);
MvarVecStruct *IritMvarVecCopy(const MvarVecStruct *Vec);
MvarVecStruct *IritMvarVecCopyList(const MvarVecStruct *VecList);
void IritMvarVecFree(MvarVecStruct *Vec);
void IritMvarVecFreeList(MvarVecStruct *VecList);
void IritMvarVecArrayFree(MvarVecStruct *MVVecArray, int Size);
void IritMvarVecAdd(MvarVecStruct *VRes,
		    const MvarVecStruct *V1,
		    const MvarVecStruct *V2);
void IritMvarVecAddScale(MvarVecStruct *VRes,
			 const MvarVecStruct *V1,
			 const MvarVecStruct *V2,
			 CagdRType Scale2);
void IritMvarVecSub(MvarVecStruct *VRes,
		    const MvarVecStruct *V1,
		    const MvarVecStruct *V2);
CagdRType IritMvarVecDotProd(const MvarVecStruct *V1, const MvarVecStruct *V2);
CagdRType IritMvarVecSqrLength(const MvarVecStruct *V);
CagdRType IritMvarVecSqrLength2(const CagdRType *v, int Dim);
CagdRType IritMvarVecLength(const MvarVecStruct *V);
MvarVecStruct *IritMvarVecScale(MvarVecStruct *V, CagdRType ScaleFactor);
void IritMvarVecBlend(MvarVecStruct *VRes,
		      const MvarVecStruct *V1,
		      const MvarVecStruct *V2,
		      CagdRType t);
int IritMvarVecNormalize(MvarVecStruct *V);
int IritMvarVecOrthogonalize(MvarVecStruct *Dir, const MvarVecStruct *Vec);
int IritMvarVecOrthogonal2(MvarVecStruct *Dir,
			   const MvarVecStruct *Vec1,
			   const MvarVecStruct *Vec2);
int IritMvarVecSetOrthogonalize(const MvarVecStruct **Vecs,
				MvarVecStruct **OrthoVecs,
				int Size);
CagdBType IritMvarVecWedgeProd(MvarVecStruct **Vectors,
			       int Size, 
			       MvarVecStruct **NewVecs,
			       int NewSize, 
			       CagdBType CheckDet,
			       CagdRType *DetVal);
int IritMvarPlaneNormalize(MvarPlaneStruct *Pln);
MvarVecStruct *IritMvarLinePlaneInter(const MvarVecStruct *P,
				      const MvarVecStruct *V,
				      const MvarPlaneStruct *Pln,
				      CagdRType *Param);

#ifdef DEBUG
#define IritMvarVecFree(Vec)         { IritMvarVecFree(Vec); Vec = NULL; }
#define IritMvarVecFreeList(VecList) { IritMvarVecFreeList(VecList); VecList = NULL; }
#define IritMvarVecArrayFree(VecArray, Size) { IritMvarVecArrayFree(VecArray, Size); \
					   VecArray = NULL; }
#endif /* DEBUG */

MvarPlaneStruct *IritMvarPlaneNew(int Dim);
MvarPlaneStruct *IritMvarPlaneCopy(const MvarPlaneStruct *Plane);
MvarPlaneStruct *IritMvarPlaneCopyList(const MvarPlaneStruct *PlaneList);
void IritMvarPlaneFree(MvarPlaneStruct *Plane);
void IritMvarPlaneFreeList(MvarPlaneStruct *PlaneList);

#ifdef DEBUG
#define IritMvarPlaneFree(Plane)         { IritMvarPlaneFree(Plane); Plane = NULL; }
#define IritMvarPlaneFreeList(PlaneList) { IritMvarPlaneFreeList(PlaneList); \
				       PlaneList = NULL; }
#endif /* DEBUG */

MvarNormalConeStruct *IritMvarNormalConeNew(int Dim);
MvarNormalConeStruct *IritMvarNormalConeCopy(const MvarNormalConeStruct
					                         *NormalCone);
MvarNormalConeStruct *IritMvarNormalConeCopyList(const MvarNormalConeStruct
					                        *NormalCones);

void IritMvarNormalConeFree(MvarNormalConeStruct *NormalCone);
void IritMvarNormalConeFreeList(MvarNormalConeStruct *NormalConeList);

#ifdef DEBUG
#define MvarConeFree(Cone)         { MvarConeFree(Cone); PCone = NULL; }
#endif /* DEBUG */

MvarPtStruct *IritMvarGetLastPt(MvarPtStruct *Pts);
int IritMvarPtCmpTwoPoints(const MvarPtStruct *P1,
		       const MvarPtStruct *P2,
		       CagdRType Eps);
int IritMvarVecCmpTwoVectors(const CagdRType *P1,
			     const CagdRType *P2,
			     int Length,
			     CagdRType Eps);
CagdRType IritMvarPtDistTwoPoints(const MvarPtStruct *P1,
				  const MvarPtStruct *P2);
CagdRType IritMvarPtDistSqrTwoPoints(const MvarPtStruct *P1,
				     const MvarPtStruct *P2);
MvarPtStruct *IritMvarPtInBetweenPoint(const MvarPtStruct *Pt1,
				       const MvarPtStruct *Pt2,
				       CagdRType t);
MvarPolylineStruct *IritMvarPolyMergePolylines(MvarPolylineStruct *Polys,
					       IrtRType Eps);
MvarPolylineStruct *IritMvarMatchPointListIntoPolylines(const MvarPtStruct
						                     *PtsList,
							IrtRType MaxTol);
MvarPtStruct *MvarCnvrtCagdPtsToMVPts(const CagdPtStruct *Pts);
MvarPtStruct *MvarCnvrtMVPolysToMVPts(const MvarPolylineStruct *MVPlls);
struct IPObjectStruct *IritMvarCnvrtMVPolysToCtlPts(const MvarPolylineStruct
						                     *MVPlls);
CagdPtStruct *MvarCnvrtMVPtsToCagdPts(const MvarPtStruct *MVPts);
struct IPObjectStruct *IritMvarCnvrtMVPtsToCtlPts(const MvarPtStruct *MVPts,
						  IrtRType MergeTol);
struct IPObjectStruct *IritMvarCnvrtMVPtsToPolys(const MvarPtStruct *MVPts,
						 const MvarMVStruct *MV,
						 IrtRType MergeTol);
struct IPPolygonStruct *IritMvarCnvrtMVPtsToPolys2(const MvarPtStruct *InPts,
						   CagdRType FineNess,
						   int Dim,
						   IrtRType *ParamDomain);
struct IPObjectStruct *IritMvarCnvrtMVPolysToIritPolys(
					     const MvarPolylineStruct *MVPlls);
struct IPObjectStruct *IritMvarCnvrtMVPolysToIritPolys2(
					     const MvarPolylineStruct *MVPlls,
					     int IgnoreIndividualPts);
CagdCrvStruct *IritMvarCnvrtMVPolysToIritCrvs(const MvarPolylineStruct *MVPlls,
					      int Order);
MvarPolylineStruct *IritMvarCnvrtIritLinCrvsToMVPolys(
						   const CagdCrvStruct *Crvs);
struct IPObjectStruct *IritMvarCnvrtMVTrsToIritPolygons(
					      const MvarTriangleStruct *MVTrs,
					      int *Coords);
MvarTriangleStruct *MvarIrit2DTrTo2DMVTrs(struct IPObjectStruct *ObjTrs);

MvarMVStruct *IritMvarCnvrtBzr2BspMV(const MvarMVStruct *MV);
MvarMVStruct *IritMvarCnvrtBsp2BzrMV(const MvarMVStruct *MV);
MvarMVStruct *IritMvarCnvrtPwr2BzrMV(const MvarMVStruct *MV);
MvarMVStruct *IritMvarCnvrtBzr2PwrMV(const MvarMVStruct *MV);

MvarMVStruct *IritMvarBzrLinearInOneDir(int Dim, int Dir, MvarPointType PType);

MvarMVStruct *MvarMVUnitMaxCoef(MvarMVStruct *MV);

void IritMvarMVTransform(MvarMVStruct *MV,
			 CagdRType *Translate,
			 CagdRType Scale);
MvarMVStruct *IritMvarMVMatTransform(const MvarMVStruct *MV, CagdMType Mat);
MvarMVStruct *IritMvarMVListMatTransform(const MvarMVStruct *MVs,
					 CagdMType Mat);
void IritMvarMVMatTransform2(MvarMVStruct *MV, CagdMType Mat);

MvarMVStruct *IritMvarCoerceMVsTo(const MvarMVStruct *MV, MvarPointType PType);
MvarMVStruct *IritMvarCoerceMVTo(const MvarMVStruct *MV, MvarPointType PType);
MvarPointType IritMvarMergeTwoPointTypes(MvarPointType PType1,
					 MvarPointType PType2);

void IritMvarMVDomain(const MvarMVStruct *MV,
		      CagdRType *Min,
		      CagdRType *Max, 
		      int Axis);
void IritMvarMVDomainAlloc(const MvarMVStruct *MV,
			   CagdRType **MinDmn, 
			   CagdRType **MaxDmn);
void IritMvarMVDomainFree(CagdRType *MinDmn, CagdRType *MaxDmn);
MvarMVStruct *IritMvarMVSetDomain(MvarMVStruct *MV,
				  CagdRType Min,
				  CagdRType Max,
				  int Axis,
				  int InPlace);
IrtRType IritMvarMVVolumeOfDomain(MvarMVStruct * const MVs, int Dim);

void IritMvarMVAuxDomainSlotReset(MvarMVStruct *MV);
int IritMvarMVAuxDomainSlotCopy(MvarMVStruct *MVDst,
				const MvarMVStruct *MVSrc);
void IritMvarMVAuxDomainSlotSet(MvarMVStruct *MV,
				CagdRType Min,
				CagdRType Max,
				int Dir);
void IritMvarMVAuxDomainSlotSetRel(MvarMVStruct *MV,
				   CagdRType Min,
				   CagdRType Max,
				   int Dir);
int IritMvarMVAuxDomainSlotGet(const MvarMVStruct *MV,
			       CagdRType *Min,
			       CagdRType *Max,
			       int Dir);

MvarMVStruct *IritMvarMVSetAllDomains(MvarMVStruct *MV,
				      CagdRType *Min,
				      CagdRType *Max,
				      int InPlace);
CagdBType IritMvarParamInDomain(const MvarMVStruct *MV,
				CagdRType t,
				MvarMVDirType Dir);
CagdBType IritMvarParamsInDomain(const MvarMVStruct *MV,
				 const CagdRType *Params);
void IritMvarMVUpdateConstDegDomains(MvarMVStruct **MVs, int NumOfMVs);
MvarPtStruct *MvarMVIntersPtOnBndry(MvarMVStruct *MV, 
				    MvarPtStruct *PointIns, 
				    MvarPtStruct *PointOuts);

CagdRType *IritMvarMVEvalMalloc(const MvarMVStruct *MV,
				const CagdRType *Params);
void IritMvarMVEvalToData(const MvarMVStruct *MV,
			  const CagdRType *Params,
			  CagdRType *Pt);

CagdRType *IritMvarMVEvalGradient2(const MvarMVStruct *MV,
				   const CagdRType *Params,
				   int *HasOrig,
				   CagdRType *Grad);
MvarPlaneStruct *IritMvarMVEvalTanPlane(const MvarMVStruct *MV,
					const CagdRType *Params);

MvarMVStruct *IritMvarMVFromMV(const MvarMVStruct *MV,
			       CagdRType t,
			       MvarMVDirType Dir);
MvarMVStruct *IritMvarMVFromMesh(const MvarMVStruct *MV,
				 int Index,
				 MvarMVDirType Dir);
MvarMVStruct *IritMvarCnvrtCrvToMV(const CagdCrvStruct *Crv);
CagdCrvStruct *IritMvarCnvrtMVToCrv(const MvarMVStruct *MV);
MvarMVStruct *IritMvarCnvrtSrfToMV(const CagdSrfStruct *Srf);
CagdSrfStruct *IritMvarCnvrtMVToSrf(const MvarMVStruct *MV);
MvarMVStruct *IritMvarCnvrtTVToMV(const TrivTVStruct *TV);
TrivTVStruct *IritMvarCnvrtMVToTV(const MvarMVStruct *MV);

MvarMVStruct *IritMvarMVRegionFromMV(const MvarMVStruct *MV,
				     CagdRType t1,
				     CagdRType t2,
				     MvarMVDirType Dir);
MvarMVStruct *IritMvarBzrMVRegionFromMV(const MvarMVStruct *MV,
					CagdRType t1,
					CagdRType t2,
					MvarMVDirType Dir);
MvarMVStruct *IritMvarMVOpenEnd(const MvarMVStruct *MV);
MvarMVStruct *IritMvarMVRefineAtParams(const MvarMVStruct *MV,
				       MvarMVDirType Dir,
				       CagdBType Replace,
				       CagdRType *t,
				       int n);
MvarMVStruct *IritMvarBspMVKnotInsertNDiff(const MvarMVStruct *MV,
					   MvarMVDirType Dir,
					   int Replace,
					   CagdRType *t,
					   int n);
MvarMVStruct *IritMvarMVDerive(const MvarMVStruct *MV, MvarMVDirType Dir);
MvarMVStruct *IritMvarBzrMVDerive(const MvarMVStruct *MV,
				  MvarMVDirType Dir,
				  CagdBType DeriveScalar);
MvarMVStruct *IritMvarBzrMVDeriveScalar(const MvarMVStruct *MV,
					MvarMVDirType Dir);
MvarMVStruct *IritMvarBspMVDerive(const MvarMVStruct *MV,
				  MvarMVDirType Dir,
				  CagdBType DeriveScalar);
MvarMVStruct *IritMvarBspMVDeriveScalar(const MvarMVStruct *MV,
					MvarMVDirType Dir);
void IritMvarMVDeriveBound(const MvarMVStruct *MV,
			   MvarMVDirType Dir,
			   CagdRType MinMax[2]);
void IritMvarBzrMVDeriveBound(const MvarMVStruct *MV,
			      MvarMVDirType Dir,
			      CagdRType MinMax[2]);
void IritMvarBspMVDeriveBound(const MvarMVStruct *MV,
			      MvarMVDirType Dir,
			      CagdRType MinMax[2]);
void IritMvarMVDeriveAllBounds(const MvarMVStruct *MV,
			       CagdMinMaxType *MinMax);
void IritMvarBzrMVDeriveAllBounds(const MvarMVStruct *MV,
				  CagdMinMaxType *MinMax);
void IritMvarBspMVDeriveAllBounds(const MvarMVStruct *MV,
				  CagdMinMaxType *MinMax);
MvarMVGradientStruct *IritMvarMVPrepGradient(const MvarMVStruct *MV,
					     CagdBType Orig);
void IritMvarMVFreeGradient(MvarMVGradientStruct *MV);
CagdRType *IritMvarMVEvalGradient(const MvarMVGradientStruct *MV,
				  const CagdRType *Params,
				  int Axis,
				  CagdRType *Grad);
MvarMVGradientStruct *MvarMVBoundGradient(const MvarMVStruct *MV);
MvarMVStruct *IritMvarMVSubdivAtParam(const MvarMVStruct *MV,
				      CagdRType t,
				      MvarMVDirType Dir);
MvarMVStruct *IritMvarBspMVSubdivAtParam(const MvarMVStruct *MV,
					 CagdRType t,
					 MvarMVDirType Dir);
MvarMVStruct *IritMvarBzrMVSubdivAtParam(const MvarMVStruct *MV,
					 CagdRType t,
					 MvarMVDirType Dir);
MvarMVStruct *IritMvarMVSubdivAtParamOneSide(const MvarMVStruct *MV,
					     CagdRType t,
					     MvarMVDirType Dir,
					     IrtBType LeftSide);
MvarMVStruct *IritMvarBzrMVSubdivAtParamOneSide(const MvarMVStruct *MV,
						CagdRType t,
						MvarMVDirType Dir,
						IrtBType LeftSide);
MvarMVStruct *IritMvarBspMVSubdivAtParamOneSide(const MvarMVStruct *MV,
						CagdRType t,
						MvarMVDirType Dir,
						IrtBType LeftSide);
MvarMVStruct *IritMvarMVDegreeRaise(const MvarMVStruct *MV, MvarMVDirType Dir);
MvarMVStruct *IritMvarMVDegreeRaiseN(const MvarMVStruct *MV, int *NewOrders);
MvarMVStruct *IritMvarMVDegreeRaiseN2(const MvarMVStruct *MV, int *NewOrders);
MvarMVStruct *IritMvarMVPwrDegreeRaise(const MvarMVStruct *MV,
				       int Dir,
				       int IncOrder);
CagdBType IritMvarMakeMVsCompatible(MvarMVStruct **MV1,
				    MvarMVStruct **MV2,
				    CagdBType SameOrders,
				    CagdBType SameKVs);
CagdBType IritMvarMakeMVsCompatible2(MvarMVStruct **MV1,
				     MvarMVStruct **MV2,
				     CagdBType SameOrders,
				     CagdBType SameKVs);
CagdBType IritMvarMakeMVsOneDimCompatible(MvarMVStruct **MV1,
					  MvarMVStruct **MV2,
					  int Dim,
					  CagdBType SameOrders,
					  CagdBType SameKVs);
void IritMvarMVMinMax(const MvarMVStruct *MV,
		      int Axis,
		      CagdRType *Min,
		      CagdRType *Max);
void IritMvarMVCornersMinMax(const MvarMVStruct *MV,
			     int Coord,
			     CagdRType *Min,
			     CagdRType *Max);
MvarBBoxStruct *IritMvarMVBBox(const MvarMVStruct *MV, MvarBBoxStruct *BBox);
void IritMvarMVListBBox(const MvarMVStruct *MVs, MvarBBoxStruct *BBox);
void IritMvarMergeBBox(MvarBBoxStruct *DestBBox, const MvarBBoxStruct *SrcBBox);
int IritMvarMVIsConstant(const MvarMVStruct *MV, IrtRType Eps);
void IritMvarBBoxOfDotProd(const MvarBBoxStruct *BBox1,
			   const MvarBBoxStruct *BBox2,
			   MvarBBoxStruct *DProdBBox);
void IritMvarBBoxOfDotProd2(const MvarBBoxStruct *BBox1,
			    const MvarBBoxStruct *BBox2,
			    MvarBBoxStruct *DProdBBox);
void IritMvarBBoxOfCrossProd(const MvarBBoxStruct *BBox1,
			     const MvarBBoxStruct *BBox2,
			     MvarBBoxStruct *DCrossBBox);

MvarMVStruct **IritMvarBndryMVsFromMV(const MvarMVStruct *MV);
MvarPtStruct *IritMvarMVPreciseBBox(const MvarMVStruct *MV,
				    MvarBBoxStruct *BBox,
				    CagdRType Tol);
void IritMvarMVListPreciseBBox(const MvarMVStruct *MVs,
			       MvarBBoxStruct *BBox,
			       CagdRType Tol);
void IritMvarTrivarPreciseBBox(const TrivTVStruct *TV,
			       MvarBBoxStruct *BBox,
			       CagdRType Tol);
void IritMvarTrivarListPreciseBBox(const TrivTVStruct *Trivars,
				   MvarBBoxStruct *BBox,
				   CagdRType Tol);
void IritMvarSrfPreciseBBox(const CagdSrfStruct *Srf,
			    MvarBBoxStruct *BBox,
			    CagdRType Tol);
void IritMvarSrfListPreciseBBox(const CagdSrfStruct *Srfs,
				MvarBBoxStruct *BBox,
				CagdRType Tol);
void IritMvarMdlTrimSrfPreciseBBox(const MdlTrimSrfStruct *TSrf,
				   MvarBBoxStruct *BBox,
				   CagdRType Tol);
void IritMvarMdlTrimSrfListPreciseBBox(const MdlTrimSrfStruct *TSrfs,
				       MvarBBoxStruct *BBox,
				       CagdRType Tol);
void IritMvarTrimSrfPreciseBBox(const TrimSrfStruct *TSrf,
				MvarBBoxStruct *BBox,
				CagdRType Tol);
void IritMvarTrimSrfListPreciseBBox(const TrimSrfStruct *TSrfs,
				    MvarBBoxStruct *BBox,
				    CagdRType Tol);
void IritMvarCrvPreciseBBox(const CagdCrvStruct *Crv,
			    MvarBBoxStruct *BBox,
			    CagdRType Tol);
void IritMvarCrvListPreciseBBox(const CagdCrvStruct *Crvs,
				MvarBBoxStruct *BBox,
				CagdRType Tol);

int _IritMvarIncrementMeshIndices(const MvarMVStruct *MV,
				  int *Indices,
				  int *Index);
int _IritMvarIncrementMeshOrderIndices(const MvarMVStruct *MV,
				       int *Indices,
				       int *Index);
int _IritMvarIncSkipMeshIndices1st(const MvarMVStruct *MV, int *Indices);
int _IritMvarIncSkipMeshIndices(const MvarMVStruct *MV,
				int *Indices,
				int Dir,
				int *Index);
int _IritMvarIncBoundMeshIndices(const MvarMVStruct *MV,
				 int *Indices,
				 int *LowerBound,
				 int *UpperBound,
				 int *Index);
int IritMvarGetPointsMeshIndices(const MvarMVStruct *MV, int *Indices);
int IritMvarGetPointsPeriodicMeshIndices(const MvarMVStruct *MV, int *Indices);
int IritMvarMeshIndicesFromIndex(int Index,
				 const MvarMVStruct *MV,
				 int *Indices);

MvarMVStruct *IritMvarEditSingleMVPt(MvarMVStruct *MV,
				     CagdCtlPtStruct *CtlPt,
				     int *Indices,
				     CagdBType Write);
CagdBType IritMvarMVsSameSpace(const MvarMVStruct *MV1,
			       const MvarMVStruct *MV2,
			       CagdRType Eps);
CagdBType IritMvarMVsSame(const MvarMVStruct *MV1,
			  const MvarMVStruct *MV2,
			  CagdRType Eps);
CagdBType IritMvarMVsSame3(const MvarMVStruct *MV1,
			   const MvarMVStruct *MV2,
			   CagdRType Eps,
			   int *Modified);
MvarMVStruct *IritMvarPromoteMVToMV(const MvarMVStruct *MV, int Axis);
MvarMVStruct *IritMvarPromoteMVToMV2(const MvarMVStruct *MV,
				     int NewDim, 
				     int StartAxis);
MvarMVStruct *IritMvarCrvMakeCtlPtParam(const CagdCrvStruct *Crv,
					int CtlPtIdx,
					CagdRType Min,
					CagdRType Max);
MvarMVStruct *IritMvarMVShiftAxes(const MvarMVStruct *MV, int Axis);
MvarMVStruct *MvarMVParamShift(const MvarMVStruct *MV,
			       int AxisSrc,
			       int AxisTar);
MvarMVStruct *IritMvarMVReverse(const MvarMVStruct *MV, int Axis1, int Axis2);
MvarMVStruct *IritMvarMVReverseDir(const MvarMVStruct *MV, int Axis);
CagdBType IritMvarAre2MVsPossiblySharingBndry(const MvarMVStruct *MV1,
					      const MvarMVStruct *MV2,
					      int Dir,
					      CagdBType *MV1Rev,
					      CagdBType *MV2Rev,
					      CagdRType Eps);
int IritMvarAre2MVsSharingBndry(const MvarMVStruct *MV1,
				int Dir1,
				CagdBType MaxBndry1,
				const MvarMVStruct *MV2,
				int Dir2,
				CagdBType MaxBndry2,
				IrtRType Tolerance,
				int *Modified);
MvarMVStruct *IritMvarMergeMVList(MvarMVStruct *MVList,
				  int Dir,
				  IrtRType Tolerance,
				  int InterpDiscont);
MvarMVStruct *IritMvarMergeMVMV(const MvarMVStruct *MV1,
				const MvarMVStruct *MV2,
				MvarMVDirType Dir,
				CagdBType Discont);
MvarMVStruct *IritMvarMergeMVMV2(const MvarMVStruct *MV1,
				 const MvarMVStruct *MV2,
				 MvarMVDirType Dir,
				 CagdBType Discont);
CagdRType IritMvarMVAvgArcLenMesh(const MvarMVStruct *MV, MvarMVDirType Dir);

CagdBType IritMvarBspMVHasOpenECInDir(const MvarMVStruct *MV,
				      MvarMVDirType Dir);
CagdBType IritMvarBspMVHasOpenEC(const MvarMVStruct *MV);
CagdBType IritMvarBspMVIsPeriodicInDir(const MvarMVStruct *MV,
				       MvarMVDirType Dir);
CagdBType IritMvarBspMVIsPeriodic(const MvarMVStruct *MV);
int IritMvarBspMVInteriorKnots(const MvarMVStruct *MV, CagdRType *Knot);

MvarMVStruct *IritMvarMVMultiLinearMV(const IrtRType *Min,
				      const IrtRType *Max, 
				      int Dim);

void IritMvarDbg(const void *Obj);
void IritMvarDbg1(const void *Obj);

#ifdef DEBUG
void IritMvarDbgDsp(const void *Obj);
void IritMvarDbgInfo(const void **Objs, int Size);
void IritMvarETDbg(const MvarExprTreeStruct *ET);
#endif /* DEBUG */

MvarMVStruct *IritMvarMVExtension(const MvarMVStruct *OrigMV,
				  const CagdBType *ExtMins,
				  const CagdBType *ExtMaxs,
				  const CagdRType *Epsilons);
CagdBType MvarMVKnotHasC0Discont(const MvarMVStruct *MV,
			         int *Dir,
			         CagdRType *t);
CagdBType MvarMVMeshC0Continuous(const MvarMVStruct *MV,
				 int Dir,
				 int Idx);
CagdBType MvarMVIsMeshC0DiscontAt(const MvarMVStruct *MV,
				  int Dir,
				  CagdRType t);
CagdBType MvarMVKnotHasC1Discont(const MvarMVStruct *MV,
				 int *Dir,
				 CagdRType *t);
CagdBType MvarMVMeshC1Continuous(const MvarMVStruct *MV,
				 int Dir,
				 int Idx);
CagdBType MvarMVIsMeshC1DiscontAt(const MvarMVStruct *MV,
				  int Dir,
				  CagdRType t);

/*****************************************************************************
* Fitting and interpolation						     *
*****************************************************************************/
CagdCrvStruct *IritMvarBspCrvInterpVecs(const MvarVecStruct *PtList,
					int Order,
					int CrvSize,
					CagdParametrizationType ParamType,
					CagdBType Periodic);
MvarVecStruct *IritMvarPtsSortAxis(MvarVecStruct *PtList, int Axis);
void IritMvarPointFromPointLine(const MvarVecStruct *Point,
				const MvarVecStruct *Pl,
				const MvarVecStruct *Vl,
				MvarVecStruct *ClosestPoint);
IrtRType IritMvarDistPointLine(const MvarVecStruct *Point,
			       const MvarVecStruct *Pl,
			       const MvarVecStruct *Vl);
CagdRType IritMvarLineFitToPts(const MvarVecStruct *PtList,
			       MvarVecStruct *LineDir,
			       MvarVecStruct *LinePos);

/*****************************************************************************
* Symbolic computation over multivariates.				     *
*****************************************************************************/
MvarMVStruct *IritMvarMVAdd(const MvarMVStruct *MV1, const MvarMVStruct *MV2);
MvarMVStruct *IritMvarMVSub(const MvarMVStruct *MV1, const MvarMVStruct *MV2);
MvarMVStruct *IritMvarMVMult(const MvarMVStruct *MV1, const MvarMVStruct *MV2);
MvarMVStruct *IritMvarMVInvert(const MvarMVStruct *MV);
MvarMVStruct *IritMvarMVScalarScale(const MvarMVStruct *MV, CagdRType Scale);
MvarMVStruct *IritMvarMVScalarScale2(const MvarMVStruct *CMV,
				     const CagdRType *Scale);
MvarMVStruct *IritMvarMVMultScalar(const MvarMVStruct *MV1,
				   const MvarMVStruct *MV2);
MvarMVStruct *IritMvarMVDotProd(const MvarMVStruct *MV1,
				const MvarMVStruct *MV2);
MvarMVStruct *IritMvarMVVecDotProd(const MvarMVStruct *MV,
				   const CagdRType *Vec);
MvarMVStruct *IritMvarMVCrossProd(const MvarMVStruct *MV1,
				  const MvarMVStruct *MV2);
MvarMVStruct *IritMvarMVCrossProdZ(const MvarMVStruct *MV1,
				   const MvarMVStruct *MV2);
MvarMVStruct *IritMvarMVCrossProd2D(const MvarMVStruct *MV1X,
				    const MvarMVStruct *MV1Y,
				    const MvarMVStruct *MV2X,
				    const MvarMVStruct *MV2Y);
MvarMVStruct *IritMvarMVRtnlMult(const MvarMVStruct *MV1X,
				 const MvarMVStruct *MV1W,
				 const MvarMVStruct *MV2X,
				 const MvarMVStruct *MV2W,
				 CagdBType OperationAdd);
MvarMVStruct *IritMvarMVMultBlend(const MvarMVStruct *MV1,
				  const MvarMVStruct *MV2,
				  CagdRType Blend);
void IritMvarMVSplitScalar(const MvarMVStruct *MV, MvarMVStruct **MVs);
MvarMVStruct *IritMvarMVMergeScalar(MvarMVStruct * const *ScalarMVs);

int IritMvarIritCagdBspMultComputationMethod(int BspMultUsingInter);

void IritMVarBzrMVDivide(const MvarMVStruct *MV1,
			 const MvarMVStruct *MV2,
			 MvarMVStruct **Q,
			 MvarMVStruct **R);
MvarMVStruct *IritMvarBzrMultBrnBasis(const MvarMVStruct *MV,
				      int *MulOrders,
				      int *MulIndices);
MvarMVStruct *IritMvarBzrMVMult(const MvarMVStruct *MV1,
				const MvarMVStruct *MV2);
MvarMVStruct *IritMvarBspMVMult(const MvarMVStruct *MV1,
				const MvarMVStruct *MV2);

MvarMVStruct *IritMvarBzrMVDeriveRational(const MvarMVStruct *MV,
					  MvarMVDirType Dir);
MvarMVStruct *IritMvarBspMVDeriveRational(const MvarMVStruct *MV,
					  MvarMVDirType Dir);

MvarMVStruct *IritMvarMVDeterminant2(const MvarMVStruct *MV11,
				     const MvarMVStruct *MV12,
				     const MvarMVStruct *MV21,
				     const MvarMVStruct *MV22);
MvarMVStruct *IritMvarMVDeterminant3(const MvarMVStruct *MV11,
				     const MvarMVStruct *MV12,
				     const MvarMVStruct *MV13,
				     const MvarMVStruct *MV21,
				     const MvarMVStruct *MV22,
				     const MvarMVStruct *MV23,
				     const MvarMVStruct *MV31,
				     const MvarMVStruct *MV32,
				     const MvarMVStruct *MV33);
MvarMVStruct *IritMvarMVDeterminant4(const MvarMVStruct *MV11,
				     const MvarMVStruct *MV12,
				     const MvarMVStruct *MV13,
				     const MvarMVStruct *MV14,
				     const MvarMVStruct *MV21,
				     const MvarMVStruct *MV22,
				     const MvarMVStruct *MV23,
				     const MvarMVStruct *MV24,
				     const MvarMVStruct *MV31,
				     const MvarMVStruct *MV32,
				     const MvarMVStruct *MV33,
				     const MvarMVStruct *MV34,
				     const MvarMVStruct *MV41,
				     const MvarMVStruct *MV42,
				     const MvarMVStruct *MV43,
				     const MvarMVStruct *MV44);
MvarMVStruct *IritMvarMVDeterminant5(const MvarMVStruct *MV11,
				     const MvarMVStruct *MV12,
				     const MvarMVStruct *MV13,
				     const MvarMVStruct *MV14,
				     const MvarMVStruct *MV15,
				     const MvarMVStruct *MV21,
				     const MvarMVStruct *MV22,
				     const MvarMVStruct *MV23,
				     const MvarMVStruct *MV24,
				     const MvarMVStruct *MV25,
				     const MvarMVStruct *MV31,
				     const MvarMVStruct *MV32,
				     const MvarMVStruct *MV33,
				     const MvarMVStruct *MV34,
				     const MvarMVStruct *MV35,
				     const MvarMVStruct *MV41,
				     const MvarMVStruct *MV42,
				     const MvarMVStruct *MV43,
				     const MvarMVStruct *MV44,
				     const MvarMVStruct *MV45,
				     const MvarMVStruct *MV51,
				     const MvarMVStruct *MV52,
				     const MvarMVStruct *MV53,
				     const MvarMVStruct *MV54,
				     const MvarMVStruct *MV55);
MvarMVStruct *MvarMVDeterminant(const MvarMVStruct * const *MVsMatrix,
				int MatrixSize);

MvarMVStruct *IritMvarBlendMVMV(const MvarMVStruct *MV1,
				const MvarMVStruct *Scalar1,
				const MvarMVStruct *MV2,
				const MvarMVStruct *Scalar2);
MvarMVStruct *IritMvarBlendConvexMVMV(const MvarMVStruct *MV1,
				      const MvarMVStruct *MV2,
				      const MvarMVStruct *MVT);

MvarMVStruct *MvarSrfCalcAsympDirsVolume(const CagdSrfStruct *Srf);
int IritMvarSrfCalcAsympDirsCones(const CagdSrfStruct *Srf,
				  SymbNormalConeStruct *DirsCones);

/******************************************************************************
* Routines to compute zeros of MVs constraints.				     *
******************************************************************************/
MvarZeroSolutionStruct *IritMvarZeroSolveMatlabEqns(
					      MvarMatlabEqStruct **Eqns,
					      int NumOfEqns,
					      int MaxVarsNum,
					      CagdRType *MinDmn,
					      CagdRType *MaxDmn,
					      CagdRType NumericTol,
					      CagdRType SubdivTol,
					      CagdRType StepTol,
					      MvarConstraintType *Constraints);
MvarPtStruct *IritMvarMVsZeros0D(MvarZeroPrblmSpecStruct *ZeroProblemSpec);
MvarPtStruct *IritMvarMVsZeros0DNumeric(MvarMVStruct * const *MVs,
					int NumOfMVs,
					CagdRType NumericTol,
					int SuccessOnDmnErr,
					MvarPtStruct *ZeroPt);
MvarPtStruct *IritMvarZero0DNumeric(MvarPtStruct *ZeroPt,
				    const MvarExprTreeEqnsStruct *Eqns,
				    MvarMVStruct const * const *MVs,
				    int NumMVs,
				    CagdRType NumericTol,
				    int SuccessOnDmnErr,
				    const CagdRType *InputMinDmn,
				    const CagdRType *InputMaxDmn);
MvarPtStruct *IritMvarMVsZeros2DBy0D(MvarZeroPrblmSpecStruct *ZeroProblemSpec);
MvarPtStruct *IritMvarETsZeros0D(MvarZeroPrblmSpecStruct *ZeroProblemSpec);
CagdPtStruct *IritMvarCrvZeroSet(const CagdCrvStruct *Curve,
				 int Axis,
				 CagdRType SubdivTol,
				 CagdRType NumericTol,
				 CagdBType FilterTangencies);
CagdBType IritMvarMVsDetectZeros(MvarMVStruct * const *MVs,
				 MvarConstraintType *Constraints,
				 int NumOfMVs,
				 CagdRType SubdivTol,
				 CagdRType NumericTol,
				 int HighDimBndry);
CagdBType IritMvarMVsZerosSameSpace(MvarMVStruct **MVs, int NumOfMVs);
int IritMvarMVsZerosNormalConeTest(int NormalConeTest);
CagdRType IritMvarMVsZerosDmnExt(CagdRType DmnExt);
MvarZeroSubdivTolActionType IritMvarZerosSubdivTolAction(
				 MvarZeroSubdivTolActionType SubdivTolAction);
int IritMvarMVsZerosGradPreconditioning(int GradPreconditioning);
int IritMvarMVsZeros2DPolylines(int IsPolyLines2DSolution);
int IritMvarMVsZeros2DCornersOnly(int Is2DSolutionCornersOnly);
int IritMvarMVsZerosCrtPts(int CrtPtsDetectionByDim);
int IritMvarMVsZerosNormalizeConstraints(int NormalizeConstraints);
int IritMvarMVsZerosDomainReduction(int DomainReduction);
int IritMvarMVsZerosParallelHyperPlaneTest(int ParallelHPlaneTest);
int IritMvarMVsZerosKantorovichTest(int KantorovichTest);

MvarZeroSolutionStruct *IritMvarZeroSolverSolutionNew(
					    MvarTriangleStruct *Tr,
					    MvarPolylineStruct *Pl,
					    MvarPtStruct *Pt,
					    MvarZrSlvrRepresentationType Rep);
MvarZeroSolutionStruct *IritMvarZeroSolverSolCpy(
					   MvarZeroSolutionStruct const *Sol);
MvarZeroSolutionStruct *IritMvarZeroSolutionCpyList(MvarZeroSolutionStruct 
						    const *SolutionList);
MvarMVDDecompositionModeType IritMvarMVDSetDecompositionMode(
					      MvarMVDDecompositionModeType m);
void IritMvarZeroSolverSolutionFree(MvarZeroSolutionStruct *Solution,
				    CagdBType FreeUnion);

#ifdef DEBUG
void IritMvarMVsZerosVerifier(MvarMVStruct * const *MVs,
			      int NumOfZeroMVs,
			      MvarPtStruct *Sols,
			      CagdRType NumerEps);
#endif /* DEBUG */

/******************************************************************************
* Routines to compute srf-srf intersections and	MV univariate solutions.     *
******************************************************************************/
MvarPolylineStruct *IritMvarSrfSrfInter(const CagdSrfStruct *Srf1, 
					const CagdSrfStruct *Srf2,
					CagdRType Step,
					CagdRType SubdivTol,
					CagdRType NumericTol);
MvarPolylineStruct *IritMvarSrfSrfInterDisc(const CagdSrfStruct *Srf1, 
					const CagdSrfStruct *Srf2,
					CagdRType Step,
					CagdRType SubdivTol,
					CagdRType NumericTol);
CagdRType IritMvarSrfSrfInterNormalizeDomain(CagdRType NewValue);
IrtBType IritMvarSrfSrfInterExamineBBoxes(const CagdSrfStruct *Srf1,
				      const CagdSrfStruct *Srf2);
MvarPolylineStruct *IritMvarMVsZeros1D(MvarZeroPrblmSpecStruct *ZeroProblemSpec);
MvarPolylineStruct *IritMvarMVsZeros1DTrace2Pts(
					      MvarMVStruct * const *MVs,
					      MvarConstraintType *Constraints,
					      int NumOfMVs,
					      MvarPtStruct *StartEndPts,
					      CagdRType Step,
					      CagdRType SubdivTol,
					      CagdRType NumericTol);
int IritMvarMVsZeros1DMergeSingularPts(int MergeSingularPts); 

MvarPolylineStruct *IritMvarSrfZeroSet(const CagdSrfStruct *Surface,
				       int Axis,
				       CagdRType Step,
				       CagdRType SubdivTol,
				       CagdRType NumericTol,
				       MvarMVsZerosVerifyAllSolsCBFuncType
							    OutputPtsFilterCB,
				       void *OutputPtsFilterCBData);
CagdBType IritMvarSrfSrfTestInter(const CagdSrfStruct *Srf1, 
				  const CagdSrfStruct *Srf2,
				  CagdRType Step,
				  CagdRType SubdivTol,
				  CagdRType NumericTol);


/******************************************************************************
* Routines to manage Srf-Srf intersection results cache.		     *
******************************************************************************/
IritMvarSrfSrfInterCacheStruct *IritMvarSrfSrfInterCacheAlloc(
						   IPAttrIDType AttribID,
						   CagdBType ShouldAssignIds);
int IritMvarSrfSrfInterCacheGetSrfId(
			       const IritMvarSrfSrfInterCacheStruct *SSICache, 
			       const CagdSrfStruct *Srf);
IritMvarSrfSrfInterCacheDataStruct *IritMvarSrfSrfInterCacheGetData(
			       const IritMvarSrfSrfInterCacheStruct *SSICache,
			       const CagdSrfStruct *Srf1, 
			       const CagdSrfStruct *Srf2);
CagdBType IritMvarSrfSrfCacheShouldAssignIds(const IritMvarSrfSrfInterCacheStruct 
					                        *DataCache);
IritMvarSrfSrfInterCacheDataStruct *IritMvarSrfSrfInterCacheAddData(
				     IritMvarSrfSrfInterCacheStruct *SSICache, 
				     CagdSrfStruct *Srf1, 
				     CagdSrfStruct *Srf2, 
				     MvarPolylineStruct *Data);
void IritMvarSrfSrfInterCacheClear(IritMvarSrfSrfInterCacheStruct *SSICahce);
void IritMvarSrfSrfInterCacheFree(IritMvarSrfSrfInterCacheStruct *SSICahce);

/******************************************************************************
* Medial axis bi- and tri-tangent circles to freeform (self) curve(s).	     *
******************************************************************************/
MvarPtStruct *IritMvarSCvrCircTanToCrvEndPtCntrOnCrv(
					     const CagdCrvStruct *Crv1,
					     const CagdPType Pt2,
					     const CagdCrvStruct *CntrOnCrv,
					     CagdRType RadiusLB,
					     CagdRType SubdivTol,
					     CagdRType NumericTol);
MvarPtStruct *IritMvarSCvrCircTanTo2CrvsCntrOnCrv(
					     const CagdCrvStruct *Crv,
					     const CagdCrvStruct *CntrOnCrv,
					     CagdRType RadiusLB,
					     CagdRType SubdivTol,
					     CagdRType NumericTol);
MvarPtStruct *IritMvarSCvrCircTanTo3CrvsExprTreeNoDiagonal(
						     const CagdCrvStruct *Crv,
						     CagdRType RadiusLB,
						     CagdRType SubdivTol,
						     CagdRType NumericTol);
MvarPtStruct *IritMvarSCvrCircTanTo2CrvsEndPtNoDiag(const CagdCrvStruct *Crv1,
						    const CagdCrvStruct *Crv2,
						    const CagdPType Pt3,
						    CagdBType ElimDiagonals,
						    CagdRType RadiusLB,
						    CagdRType SubdivTol,
						    CagdRType NumericTol);
MvarPtStruct *IritMvarSCvrCircTanToCrvEndPt(const CagdCrvStruct *Crv1,
					    const CagdPType Pt2,
					    CagdRType RadiusLB,
					    CagdRType SubdivTol,
					    CagdRType NumericTol);
MvarPtStruct *IritMvarSCvrCircTanToCrv2EndPt(const CagdCrvStruct *Crv1,
					     const CagdPType Pt2,
					     const CagdPType Pt3,
					     CagdRType RadiusLB,
					     CagdRType SubdivTol,
					     CagdRType NumericTol);
MvarPtStruct *IritMvarSCvrBiNormals(const CagdCrvStruct *Crv1,
				    const CagdCrvStruct *Crv2,
				    CagdBType ElimDiagonals,
				    CagdRType RadiusLB,
				    CagdRType SubdivTol,
				    CagdRType NumericTol);
CagdSrfStruct *IritMvarSCvrUMinusVSrf(const CagdRType *TMin,
				      const CagdRType *TMax);
CagdSrfStruct *IritMvarSCvrPromoteCrvToSrfWithOtherCrv(
						const CagdCrvStruct *Crv,
						CagdSrfDirType Dir,
						const CagdCrvStruct *OtherCrv);

/******************************************************************************
* Circle packing related routines.					     *
******************************************************************************/
MvarPtStruct *IritMvarCircTanToCircCrv3By3(const CagdCrvStruct *Bndry,
					   const CagdCrvStruct *InNrml,
					   CagdPType XBnd,
					   CagdPType YBnd,
					   const CagdPType Center,
					   CagdRType Radius,
					   CagdBType BndBndryPar,
					   CagdRType BndryPar,
					   CagdRType NumericTol,
					   CagdRType SubdivTol);
MvarPtStruct *IritMvarCircOnLineTangToBdry(const CagdCrvStruct *Bndry,
					   const CagdCrvStruct *InNrml,
					   CagdRType Radius,
					   const CagdPType Dir,
					   const CagdPType Pt,
					   CagdRType NumericTol,
					   CagdRType SubdivTol);
MvarPtStruct *IritMvarCircTanAtTwoPts(const CagdCrvStruct *Bndry,
				      CagdPType XBnd,
				      CagdPType YBnd,
				      CagdRType Radius,
				      CagdRType NumericTol,
				      CagdRType SubdivTol);
MvarPtStruct *IritMvarCircAtDirMax(const CagdCrvStruct *Bndry,
				   CagdPType XBnd,
				   CagdPType YBnd,
				   CagdRType Radius,
				   const CagdPType Dir,
				   CagdRType NumericTol,
				   CagdRType SubdivTol);
MvarPtStruct *IritMvarCircTanToCrvXCoord(const CagdCrvStruct *Bndry,
					 const CagdCrvStruct *InNrml,
					 CagdPType YBnd,
					 CagdRType Radius,
					 CagdRType XCoord,
					 CagdRType NumericTol,
					 CagdRType SubdivTol);
MvarPtStruct *IritMvarCircTanToCrvYCoord(const CagdCrvStruct *Bndry,
					 const CagdCrvStruct *InNrml,
					 CagdPType XBnd,
					 CagdRType Radius,
					 CagdRType YCoord,
					 CagdRType NumericTol,
					 CagdRType SubdivTol);

/******************************************************************************
* Routines to compute zeros of MVs constraints with bivariate solutions.     *
******************************************************************************/
MvarZeroSolutionStruct *IritMvarMVsZeros2D(MvarZeroPrblmSpecStruct
					   *ZeroProblemSpec);
MvarPolylineStruct *IritMvarZeroSolverPolyProject(MvarPolylineStruct *PolyList,
						  int *Coords,
						  int ProjDim);
int IritMvarMinSpanConeAvg(MvarVecStruct *MVVecs,
			   int VecsNormalized,
			   int NumOfVecs,
			   MvarNormalConeStruct *MVCone);
int IritMvarMinSpanCone(MvarVecStruct *MVVecs,
			int VecsNormalized,
			int NumOfVecs,
		    MvarNormalConeStruct *MVCone);
int IritMvarHyperPlaneFromNPoints(MvarPlaneStruct *MVPlane,
				  MvarVecStruct * const *Vecs,
				  int n);
int IritMvarHyperConeFromNPoints(MvarNormalConeStruct *MVCone,
				 MvarVecStruct * const *Vecs,
				 int n);
int IritMvarHyperConeFromNPoints2(MvarNormalConeStruct *MVCone,
				  MvarVecStruct * const *Vecs,
				  int m);
int IritMvarHyperConeFromNPoints3(MvarNormalConeStruct *MVCone,
				  MvarVecStruct * const *Vecs,
				  int m);
MvarNormalConeStruct *IritMVarMVNormalCone(const MvarMVStruct *MV);
MvarNormalConeStruct *IritMVarMVNormalCone2(const MvarMVStruct *MV,
					    CagdRType * const *GradPoints,
					    int TotalLength,
					    int *MaxDevIndex);
MvarNormalConeStruct *IritMVarMVNormalConeMainAxis(const MvarMVStruct *MV,
						   MvarVecStruct **MainAxis);
MvarNormalConeStruct *IritMVarMVNormalConeMainAxis2(
						const MvarMVStruct *MV,
						CagdRType * const *GradPoints,
						int TotalLength,
						MvarVecStruct **MainAxis);
MvarNormalConeStruct *MvarMVNormal2Cones(const MvarMVStruct *MV,
					 CagdRType ExpandingFactor,
					 int NumOfZeroMVs,
					 MvarNormalConeStruct **Cone1,
					 MvarNormalConeStruct **Cone2);
CagdBType IritMvarMVConesOverlap(MvarMVStruct **MVs, int NumOfZeroMVs);

/******************************************************************************
* Routines to handle Minkowski sums.					     *
******************************************************************************/
struct IPObjectStruct *IritMvarSrfSrfMinkowskiSum(const CagdSrfStruct *Srf1, 
						  const	CagdSrfStruct *Srf2,
						  CagdRType SubdivTol,
						  CagdRType CrvTraceStep,
						  CagdRType NumericTol,	
						  int ParallelNrmls, 
						  CagdRType OffsetTrimDist);
struct IPObjectStruct *IritMvarCrvSrfMinkowskiSum(const CagdCrvStruct *Crv, 
						  const CagdSrfStruct *Srf,
						  CagdRType SubdivTol,
						  CagdRType CrvTraceStep,
						  CagdRType NumericTol, 
						  CagdRType OffsetTrimDist);

/******************************************************************************
* Routines to handle surface ray intersections.				     *
******************************************************************************/
MvarPtStruct *IritMvarSurfaceRayIntersection(const CagdSrfStruct *Srf,
					     const IrtPtType RayOrigin,
					     const IrtVecType RayDir,
					     IrtRType SubdivTol);
int IritMvarSrfRayIntersect(const CagdSrfStruct *Srf, 
			const CagdVType RayPt,
			const CagdVType RayDir, 
			CagdUVStruct **InterPntsUV);
int IritMvarTrimSrfRayIntersect(const struct TrimSrfStruct *TrimSrf, 
				const CagdVType RayPt,
				const CagdVType RayDir, 
				CagdUVStruct **InterPntsUV);

/*****************************************************************************
* Routines to compute 2contact motion curves.				     *
*****************************************************************************/
MvarPolylineStruct *IritMvar2CntctCompute2CntctMotion(
						  const CagdCrvStruct *CCrvA,
						  const CagdCrvStruct *CCrvB,
						  CagdRType Step,
						  CagdRType SubdivTol,
						  CagdRType NumericTol);

/******************************************************************************
* Multivariate expression trees.					     *
******************************************************************************/

/* Conversions between expression trees and multivariates. */
MvarExprTreeStruct *IritMvarExprTreeFromCrv(const CagdCrvStruct *Crv,
					    int NewDim,
					    int StartAxis);
  MvarExprTreeStruct *IritMvarExprTreeFromSrf(const CagdSrfStruct *Srf,
					      int NewDim,
					      int StartAxis);
MvarExprTreeStruct *IritMvarExprTreeFromMV(const MvarMVStruct *MV,
					   int NewDim,
					   int StartAxis);
MvarExprTreeStruct *IritMvarExprTreeFromMV2(const MvarMVStruct *MV);
MvarMVStruct *IritMvarExprTreeToMV(const MvarExprTreeStruct *ET);

/* Maintenance function on multivariate expression trees. */
MvarExprTreeStruct *IritMvarExprTreeLeafNew(CagdBType IsRef,
					    MvarMVStruct *MV,
					    int NewDim,
					    int StartAxis,
					    MvarNormalConeStruct *MVBCone,
					    const MvarBBoxStruct *MVBBox);
MvarExprTreeStruct *IritMvarExprTreeIntrnlNew(MvarExprTreeNodeType NodeType,
					      MvarExprTreeStruct *Left,
					      MvarExprTreeStruct *Right,
					      const MvarBBoxStruct *MVBBox);
MvarExprTreeStruct *IritMvarExprTreeCopy(const MvarExprTreeStruct *ET,
					 CagdBType ThisNodeOnly,
					 CagdBType DuplicateMVs);
void IritMvarExprTreeFreeSlots(MvarExprTreeStruct *ET, CagdBType ThisNodeOnly);
void IritMvarExprTreeFree(MvarExprTreeStruct *ET, CagdBType ThisNodeOnly);
int IritMvarExprTreeSize(MvarExprTreeStruct *ET);
CagdBType IritMvarExprTreesSame(const MvarExprTreeStruct *ET1,
				const MvarExprTreeStruct *ET2,
				CagdRType Eps);
void IritMvarExprTreePrintInfo(const MvarExprTreeStruct *ET,
			   CagdBType CommonExprIdx,
			   CagdBType PrintMVInfo,
			   MvarExprTreePrintFuncType PrintFunc);

/* Constraint constructors using simple math operations. */
MvarExprTreeStruct *IritMvarExprTreeAdd(MvarExprTreeStruct *Left,
					MvarExprTreeStruct *Right);
MvarExprTreeStruct *IritMvarExprTreeSub(MvarExprTreeStruct *Left,
					MvarExprTreeStruct *Right);
MvarExprTreeStruct *IritMvarExprTreeMult(MvarExprTreeStruct *Left,
					 MvarExprTreeStruct *Right);
MvarExprTreeStruct *IritMvarExprTreeMultScalar(MvarExprTreeStruct *Left,
					       MvarExprTreeStruct *Right);
MvarExprTreeStruct *MvarExprTreeMergeScalar(MvarExprTreeStruct *Left,
					    MvarExprTreeStruct *Right);
MvarExprTreeStruct *IritMvarExprTreeDotProd(MvarExprTreeStruct *Left,
					    MvarExprTreeStruct *Right);
MvarExprTreeStruct *IritMvarExprTreeCrossProd(MvarExprTreeStruct *Left,
					  MvarExprTreeStruct *Right);
MvarExprTreeStruct *MvarExprTreeExp(MvarExprTreeStruct *Left);
MvarExprTreeStruct *MvarExprTreeLog(MvarExprTreeStruct *Left);
MvarExprTreeStruct *MvarExprTreeCos(MvarExprTreeStruct *Left);
MvarExprTreeStruct *MvarExprTreeSqrt(MvarExprTreeStruct *Left);
MvarExprTreeStruct *MvarExprTreeSqr(MvarExprTreeStruct *Left);
MvarExprTreeStruct *MvarExprTreeNPow(MvarExprTreeStruct *Left, int Power);
MvarExprTreeStruct *MvarExprTreeRecip(MvarExprTreeStruct *Left);

/* Operations on multivariate expression trees. */
int IritMvarExprTreeSubdivAtParam(const MvarExprTreeStruct *ET,
				  CagdRType t,
				  MvarMVDirType Dir,
				  MvarExprTreeStruct **Left,
				  MvarExprTreeStruct **Right);
const MvarBBoxStruct *IritMvarExprTreeBBox(MvarExprTreeStruct *ET);
MvarBBoxStruct *MvarExprTreeCompositionDerivBBox(MvarExprTreeStruct *ET,
						 MvarBBoxStruct *BBox);
int IritMvarETDomain(const MvarExprTreeStruct *ET,
		     CagdRType *Min,
		     CagdRType *Max,
		     int Axis);
int IritMvarExprTreeLeafDomain(MvarExprTreeStruct *ET,
			       CagdRType *Min,
			       CagdRType *Max,
			       int Axis);
int IritMvarETUpdateConstDegDomains(MvarExprTreeStruct **MVETs, int n);
int IritMvarExprTreesVerifyDomain(MvarExprTreeStruct *ET1,
				  MvarExprTreeStruct *ET2);
int IritMvarExprTreesMakeConstDomainsUpdate(MvarExprTreeStruct **MVETs, int n);
void IritMvarExprAuxDomainReset(MvarExprTreeStruct *ET);
int IritMvarExprTreeCnvrtBsp2BzrMV(MvarExprTreeStruct *ET,
				   MvarMinMaxType *Domain);
int IritMvarExprTreeCnvrtBzr2BspMV(MvarExprTreeStruct *ET);

int IritMvarExprTreeInteriorKnots(const MvarExprTreeStruct *ET,
				  CagdRType *Knot);
CagdRType *MvarExprTreeEval(const MvarExprTreeStruct *ET,
			    CagdRType *Params,
			    CagdRType *Pt);
CagdRType *IritMvarExprTreeGradient(const MvarExprTreeStruct *ET,
				    CagdRType *Params,
				    int *Dim,
				    CagdRType *GradData,
				    CagdRType *V);
MvarPlaneStruct *MvarExprTreeEvalTanPlane(const MvarExprTreeStruct *ET,
					  CagdRType *Params);

/* Zero finding over multivariate expression trees. */
int IritMvarExprTreeZerosUseCommonExpr(int UseCommonExpr);
int IritMvarExprTreeZerosCnvrtBezier2MVs(int Bezier2MVs);
MvarMVsZerosMVsCBFuncType IritMvarExprTreeZerosCnvrtBezier2MVsCBFunc(
					 MvarMVsZerosMVsCBFuncType MVsCBFunc);
MvarPtStruct *IritMvarExprTreesZeros(MvarExprTreeStruct * const *MVETs,
				     MvarConstraintType *Constraints,
				     int NumOfMVETs,
				     CagdRType SubdivTol,
				     CagdRType NumericTol);
MvarPtStruct *MvarExprTreeEqnsZeros(MvarExprTreeEqnsStruct *Eqns,
				    CagdRType SubdivTol,
				    CagdRType NumericTol);
MvarNormalConeStruct *IritMVarExprTreeNormalCone(MvarExprTreeStruct *Eqn);
CagdBType IritMvarExprTreeConesOverlap(MvarExprTreeEqnsStruct *Eqns);

/******************************************************************************
* Intersections/contacts/antipodal points in crvs and srfs.		     *
******************************************************************************/
MvarPtStruct *IritMvarCrvCrvInter(const CagdCrvStruct *Crv1,
				  const	CagdCrvStruct *Crv2,
				  CagdRType SubdivTol,
				  CagdRType NumericTol,
				  CagdBType UseExprTree);
MvarPtStruct *IritMvarCrvCrvContact(const CagdCrvStruct *Crv1,
				    const CagdCrvStruct *Crv2,
				    const CagdCrvStruct *MotionCrv1,
				    const CagdCrvStruct *ScaleCrv1,
				    CagdRType SubdivTol,
				    CagdRType NumericTol,
				    CagdBType UseExprTree);
MvarPtStruct *IritMvarSrfSrfSrfInter(const CagdSrfStruct *Srf1,
				     const CagdSrfStruct *Srf2,
				     const CagdSrfStruct *Srf3,
				     CagdRType SubdivTol,
				     CagdRType NumericTol,
				     CagdBType UseExprTree);
MvarPtStruct *IritMvarSrfSrfContact(const CagdSrfStruct *Srf1,
				    const CagdSrfStruct *Srf2,
				    const CagdCrvStruct *Srf1Motion,
				    const CagdCrvStruct *Srf1Scale,
				    CagdRType SubdivTol,
				    CagdRType NumericTol,
				    CagdBType UseExprTree);

MvarMVStruct *IritMvarMVFactorUMinusV(const MvarMVStruct *MV);
MvarMVStruct *IritMvarMVFactorRMinusT(const MvarMVStruct *MV,
				      int RIdx,
				      int TIdx);
MvarPtStruct *IritMvarCrvAntipodalPoints(const CagdCrvStruct *Crv,
					 CagdRType SubdivTol,
					 CagdRType NumericTol);
MvarPtStruct *IritMvarSrfAntipodalPoints(const CagdSrfStruct *Srf,
					 CagdRType SubdivTol,
					 CagdRType NumericTol);
MvarPtStruct *IritMvarCrvSelfInterDiagFactor(const CagdCrvStruct *Crv,
					     CagdRType SubdivTol,
					     CagdRType NumericTol);
MvarPtStruct *IritMvarCrvSelfInterNrmlDev(const CagdCrvStruct *Crv,
					  CagdRType SubdivTol,
					  CagdRType NumericTol,
					  CagdRType MinNrmlDeviation);
void IritMvarBzrSelfInter4VarDecomp(const CagdSrfStruct *Srf,
				    MvarMVStruct **U1MinusU3Factor,
				    MvarMVStruct **U2MinusU4Factor);
MvarPolylineStruct *IritMvarBzrSrfSelfInterDiagFactor(const CagdSrfStruct *Srf,
						      CagdRType SubdivTol,
						      CagdRType NumericTol);
MvarPolylineStruct *IritMvarBspSrfSelfInterDiagFactor(const CagdSrfStruct *Srf,
						      CagdRType SubdivTol,
						      CagdRType NumericTol);
MvarPolylineStruct *IritMvarAdjacentSrfSrfInter(const CagdSrfStruct *Srf1,
						const CagdSrfStruct *Srf2,
						CagdSrfBndryType Srf1Bndry,
						CagdRType SubdivTol,
						CagdRType NumericTol);
MvarPolylineStruct *IritMvarSrfSelfInterDiagFactor(const CagdSrfStruct *Srf,
						   CagdRType SubdivTol,
						   CagdRType NumericTol);
MvarPolylineStruct *IritMvarSrfSelfInterNrmlDev(const CagdSrfStruct *Srf,
						CagdRType SubdivTol,
						CagdRType NumericTol,
						CagdRType MinNrmlDeviation);
MvarPtStruct *IritMvarFindSrfMiterPointsPartial(const CagdSrfStruct *Srf, 
						const CagdSrfStruct *NrmlSrf,
						int Axis1, 
						int Axis2,
						CagdRType SubdivTol,
						CagdRType NumericTol,
						CagdRType UVDiffTol,
						CagdBType Euclidean);
MvarPtStruct *IritMvarFindSrfMiterPoints(const CagdSrfStruct *Srf,
					 int Axis,
					 CagdRType SubdivTol,
					 CagdRType NumericTol,
					 CagdBType Euclidean);
MvarPtStruct *IritMvarCntctTangentialCrvCrvC1(const CagdCrvStruct *Crv1,
					      const CagdCrvStruct *Crv2,
					      CagdRType Epsilon);

/******************************************************************************
* Bisectors and	Trisectors of multivariates.				     *
******************************************************************************/
MvarMVStruct *IritMvarMVsBisector(const MvarMVStruct *MV1,
				  const	MvarMVStruct *MV2);
MvarMVStruct *IritMvarCrvSrfBisector(const MvarMVStruct *MV1,
				     const MvarMVStruct *MV2);
MvarMVStruct *IritMvarSrfSrfBisector(const MvarMVStruct *MV1,
				     const MvarMVStruct *MV2);
VoidPtr IritMvarCrvSrfBisectorApprox2(const MvarMVStruct *MV1,
				      const MvarMVStruct *MV2,
				      int OutputType,
				      CagdRType SubdivTol,
				      CagdRType NumericTol);
MvarZeroSolutionStruct *IritMvarCrvSrfBisectorApprox(const MvarMVStruct *CMV1,
						     const MvarMVStruct *CMV2,
						     CagdRType SubdivTol,
						     CagdRType NumericTol);
MvarZeroSolutionStruct *IritMvarSrfSrfBisectorApprox(const MvarMVStruct *CMV1,
						     const MvarMVStruct *CMV2,
						     CagdRType SubdivTol,
						     CagdRType NumericTol);
VoidPtr IritMvarSrfSrfBisectorApprox2(const MvarMVStruct *MV1,
				      const MvarMVStruct *MV2,
				      int OutputType,
				      CagdRType SubdivTol,
				      CagdRType NumericTol);
CagdCrvStruct *IritMvarCrvCrvBisector2D(const CagdCrvStruct *Crv1,
					const CagdCrvStruct *Crv2, 
					CagdRType Step, 
					CagdRType SubdivTol,
					CagdRType NumericTol, 
					CagdRType *BBoxMin,
					CagdRType *BBoxMax,
					CagdBType SupportPrms);

MvarMVStruct **IritMvarTrisector3DCreateMVs(VoidPtr FF1, 
					    VoidPtr FF2,
					    VoidPtr FF3,
					    CagdRType *BBoxMin,
					    CagdRType *BBoxMax,
					    int *Eqns);
MvarPolylineStruct *IritMvarTrisectorCrvs(VoidPtr FF1,
					  VoidPtr FF2,
					  VoidPtr FF3,
					  CagdRType Step, 
					  CagdRType SubdivTol,
					  CagdRType NumericTol,
					  CagdRType *BBoxMin,
					  CagdRType *BBoxMax);

/******************************************************************************
* Voronoi cell computation						     *
******************************************************************************/
CagdCrvStruct *IritMvarCrv2DMAT(const CagdCrvStruct *OCrv,
				CagdRType SubdivTol,
				CagdRType NumericTol,
				CagdBType InvertOrientation);
struct IPObjectStruct *IritMvarComputeVoronoiCell(CagdCrvStruct *Crv);
CagdCrvStruct *IritMvarBsctTrimCrvPt(CagdCrvStruct *Crv, 
				     CagdRType *Pt, 
				     CagdRType Alpha,
				     CagdCrvStruct *BaseCrv);
void IritMvarUniFuncsComputeLowerEnvelope(CagdCrvStruct *InputCurves, 
					  CagdCrvStruct **LowerEnvelope);

/******************************************************************************
* Bitangents/Tritangents.						     *
******************************************************************************/

MvarPtStruct *IritMvarMVBiTangentLine(const CagdCrvStruct *Crv1,
				      const CagdCrvStruct *Crv2,
				      CagdRType SubdivTol,
				      CagdRType NumericTol);
MvarPolylineStruct *IritMvarMVBiTangents(const MvarMVStruct *MV1,
					 const MvarMVStruct *MV2,
					 CagdRType SubdivTol,
					 CagdRType NumericTol);
MvarPtStruct *IritMvarMVTriTangents(const MvarMVStruct *MV1,
				    const MvarMVStruct *MV2,
				    const MvarMVStruct *MV3,
				    int Orientation,
				    CagdRType SubdivTol,
				    CagdRType NumericTol);
MvarPtStruct *IritMvarCircTanTo2Crvs(const CagdCrvStruct *Crv1,
				     const CagdCrvStruct *Crv2,
				     CagdRType Radius,
				     CagdRType Tol);
MvarPtStruct *IritMvarCircTanTo3Crvs(const CagdCrvStruct *Crv1,
				     const CagdCrvStruct *Crv2,
				     const CagdCrvStruct *Crv3,
				     CagdRType SubdivTol,
				     CagdRType NumericTol,
				     CagdBType OneSideOrientation);
void IritMvarMVTriTangentLineCreateMVs(const MvarMVStruct *CMV1,
				       const MvarMVStruct *CMV2,
				       const MvarMVStruct *CMV3,
				       MvarMVStruct **MVs,
				       MvarConstraintType *Constraints);
void IritMvarMVTriTangentLineCreateETs(const MvarMVStruct *CMV1,
				       const MvarMVStruct *CMV2,
				       const MvarMVStruct *CMV3,
				       MvarExprTreeStruct **ETs,
				       MvarConstraintType *Constraints);
CagdCrvStruct *IritMvarMVTriTangentLine(const CagdSrfStruct *Srf1,
					const CagdSrfStruct *Srf2,
					const CagdSrfStruct *Srf3,
					CagdRType StepSize,
					CagdRType SubdivTol,
					CagdRType NumericTol,
					int Euclidean);

CagdCrvStruct *IritMvarRoundChamferCrvAtC1DiscontArc(
						 CagdCrvStruct **TCrv1,
						 CagdCrvStruct **TCrv2,
						 CagdCrvCornerType CornerType,
						 CagdRType Radius);
CagdCrvStruct *IritMvarRoundChamferCrvAtC1Discont(const CagdCrvStruct *Crv,
						  CagdCrvCornerType CornerType,
						  CagdRType Radius);

/******************************************************************************
 * Kernel and related analysis of curves.				      *
******************************************************************************/
MvarMVStruct *IritMvarCrvKernel(const CagdCrvStruct *Crv);
MvarMVStruct *IritMvarCrvGammaKernel(const CagdCrvStruct *Crv, CagdRType Gamma);
MvarMVStruct *IritMvarCrvGammaKernelSrf(const CagdCrvStruct *Crv,
					CagdRType ExtentScale,
					CagdRType GammaMax);
MvarPolylineStruct *IritMvarCrvKernelSilhouette(const CagdCrvStruct *Crv,
						CagdRType Gamma,
						CagdRType SubEps,
						CagdRType NumEps);
struct IPObjectStruct *IritMvarCrvDiameter(const CagdCrvStruct *Crv,
					   CagdRType SubEps,
					   CagdRType NumEps);
struct IPObjectStruct *IritMvarCrvKernelPoint(const CagdCrvStruct *Crvs,
					      CagdRType SubEps,
					      int OneKernelPoint,
					      IrtRType PreciseBBoundTol,
					      CagdRType Gamma, 
					      int NumTanSamples,
					      CagdBType CnvxHull);
struct IPObjectStruct *IritMvarCrvArtGalleryPoint(const CagdCrvStruct *Crv,
						  CagdRType SubEps,
						  int NumGuards,
						  int BBoxSubdivDepth);

/******************************************************************************
 * Kernel and related analysis and construction of surfaces.		      *
******************************************************************************/
struct IPObjectStruct *IritMvarSrfKernelPointIneql(const CagdSrfStruct *Srfs,
						   CagdRType SubEps,
						   CagdRType ZeroEps,
						   CagdRType BoxScale,
						   CagdRType Gamma,
						   int NumTanSamples,
						   CagdBType DmnBoxOutput);
struct IPObjectStruct *IritMvarEvalCnvxHullRegion4Curves(
						   const CagdCrvStruct *Left,
						   const CagdCrvStruct *Right,
						   const CagdCrvStruct *Top,
						   const CagdCrvStruct *Bottom,
					           IrtRType SubEps);
void IritMvarPlnrKrnlSrfResStructFree(MvarPlnrKrnlSrfResStruct *Res);
MvarPlnrKrnlSrfResStruct *IritMvarPlnrKrnlBooleanSumSrf(
                                              MvarPlnrKrnlSrfInptStruct *Inpt);
MvarPlnrKrnlSrfResStruct *IritMvarPlnrKrnlRuledSrf(
                                              MvarPlnrKrnlSrfInptStruct *Inpt);
MvarPlnrKrnlSrfResStruct *IritMvarPlnrKrnlOneSidedBSumSrf(
                                              MvarPlnrKrnlSrfInptStruct *Inpt);
MvarPlnrKrnlSrfInptStruct *IritMvarPlnrKrnlSrfInptStructAlloc();
void IritMvarPlnrKrnlSrfInptStructFree(MvarPlnrKrnlSrfInptStruct *KrnlInpt);
CagdSrfStruct *IritMvarCreatePlnrKrnlSrf(MvarPlnrKrnlSrfInptStruct *KrnlInpt,
					 int Op,
					 int AllowInvldSrf);

/******************************************************************************
 * Kernel trivariate methods.						      *
******************************************************************************/
struct IPObjectStruct *IritMvarEvalKrnlPtsTV(TrivTVStruct *TV, IrtRType SubEps);
void IritMvarTvTransInnerCtlPts2Pt(TrivTVStruct *TV, 
			       const IrtPtType Pt, 
			       IrtRType Ratio);
MvarKrnlTVResStruct *IritMvarKrnlTVResStructNew();
void IritMvarKrnlTVResStructFree(MvarKrnlTVResStruct *Res);
void IritMvarPtListGetCenter(const struct IPObjectStruct *Pts, IrtPtType *Pt);
MvarKrnlTVResStruct *IritMvarKrnlBooleanSumTV(MvarKrnlTVInptStruct *Inpt);
MvarKrnlTVResStruct *IritMvarKrnlRuledTV(MvarKrnlTVInptStruct *Inpt);
MvarKrnlTVResStruct *IritMvarKrnlTVOneSidedBSum2Srfs(MvarKrnlTVInptStruct *Inpt);
MvarKrnlTVResStruct *IritMvarKrnlTVOneSidedBSum3Srfs(MvarKrnlTVInptStruct *Inpt);
MvarKrnlTVInptStruct *IritMvarKrnlTVInptStructAlloc();
void IritMvarKrnlTVInptStructFree(MvarKrnlTVInptStruct *KrnlInpt);
TrivTVStruct *IritMvarCreateKrnlTV(MvarKrnlTVInptStruct *Inpt,
				   int Op,
				   int AllowInvldSrf);
						   
/*****************************************************************************
* Distances between manifolds as multivariates.				     *
*****************************************************************************/
CagdRType *IritMvarDistSrfPoint(const CagdSrfStruct *Srf,
				void *SrfPtPrepHandle,
				const CagdPType	Pt,
				MvarPtStruct **InitialSol,
				CagdBType MinDist,
				CagdRType SubdivTol,
				CagdRType NumericTol,
				MvarPtStruct **ExtremePts,
				CagdRType *ExtremeDistUV,
				CagdPointType DistSpace);
void *IritMvarDistSrfPointPrep(const CagdSrfStruct *CSrf);
void IritMvarDistSrfPointFree(void *SrfPtPrepHandle);
MvarPtStruct *IritMvarLclDistSrfPoint(const CagdSrfStruct *Srf,
				      void *SrfPtPrepHandle,
				      const CagdPType Pt,
				      MvarPtStruct **InitialSol,
				      CagdRType SubdivTol,
				      CagdRType NumericTol);
CagdRType *IritMvarDistSrfLine(const CagdSrfStruct *Srf,
			       const CagdPType LnPt,
			       const CagdVType LnDir,
			       CagdBType MinDist,
			       CagdRType SubdivTol,
			       CagdRType NumericTol,
			       CagdUVType ExtremeDistUV);
MvarPtStruct *IritMvarLclDistSrfLine(const CagdSrfStruct *Srf,
				     const CagdPType LnPt,
				     const CagdVType LnDir,
				     CagdRType SubdivTol,
				     CagdRType NumericTol);
MvarMVStruct *IritMvarMVDistCrvSrf(const CagdCrvStruct *Crv1,
				   const CagdSrfStruct *Srf2,
				   int DistType);
MvarMVStruct *IritMvarMVDistSrfSrf(const CagdSrfStruct *Srf1,
				   const CagdSrfStruct *Srf2,
				   int DistType);

int IritMvarIsCrvOnBndryOfSrf(const CagdCrvStruct *Crv,
			      const CagdSrfStruct *Srf,
			      int CrvOnBndry[4],
			      int *CrvClosed);
void *IritMvarInverseCrvOnSrfProjPrep(const CagdSrfStruct *Srf);
void IritMvarInverseCrvOnSrfProjFree(void *SrfPrepHandle);
CagdCrvStruct *IritMvarInverseCrvOnSrfProj(const CagdCrvStruct *E3Crv,
					   const CagdSrfStruct *Srf,
					   void *SrfPrepHandle,
					   CagdRType ApproxTol,
					   const CagdRType *PrevUVPt,
					   int *RedundantSol,
					   int IncrSol);
CagdCrvStruct *IritMvarProjCrvOnSrf(const CagdCrvStruct *Crv,
				    const CagdSrfStruct *Srf,
				    CagdRType Step, 
				    CagdRType SubdivTol,
				    CagdRType NumericTol,
				    void *SrfPrepHandle,
				    CagdBType GenUVCrv);
MvarPolylineStruct *IritMvarMapTUV2UV(MvarPolylineStruct *MVPlls);
MvarPolylineStruct *IritMvarProjCrvOnSrf1(const CagdCrvStruct *Crv1,
					  const CagdSrfStruct *Srf2,
					  int Axes,
					  CagdRType Step, 
					  CagdRType SubdivTol,
					  CagdRType NumericTol);
MvarPolylineStruct *IritMvarProjCrvOnSrf2(const CagdCrvStruct *Crv1,
					  const CagdSrfStruct *Srf2,
					  CagdRType Step, 
					  CagdRType SubdivTol,
					  CagdRType NumericTol);

int IritMvarNumericImporveSharedPoints(const CagdSrfStruct *Srf1,
				       void *DistSrfPointPreps1,
				       CagdRType *UV1,
				       const CagdSrfStruct *Srf2,
				       void *DistSrfPointPreps2,
				       CagdRType *UV2);
CagdCrvStruct *IritMvarProjUVCrvOnE3CrvMatchSpeed(
					      const CagdCrvStruct *UVLinCrv1,
					      const CagdSrfStruct *Srf1,
					      const CagdCrvStruct *UVCrv2,
					      const CagdSrfStruct *Srf2);
CagdCrvStruct *IritMvarEucCrvOffsetOnSrf(const CagdCrvStruct *Crv,
					 const CagdSrfStruct *Srf,
					 CagdRType Dist,
					 CagdRType Step,
					 CagdRType SubdivTol,
					 CagdRType NumerTol,
					 int Orient,
					 int LstSqrFit);
CagdCrvStruct *IritMvarSpiralCrvOnSrf(const CagdSrfStruct *Srf,
				      CagdSrfDirType Dir,
				      CagdRType Pitch,
				      CagdRType Step,
				      CagdRType SubdivTol,
				      CagdRType NumerTol,
				      int OutputType);
CagdCrvStruct *IritMvarDistCrvOfPtFromSrf(const CagdRType *Pt,
					  const CagdSrfStruct *Srf,
					  CagdRType Dist,
					  CagdRType Step,
					  CagdRType SubdivTol,
					  CagdRType NumerTol,
					  int LstSqrFit);

/*****************************************************************************
* Metamorphosis	of multivariates.					     *
*****************************************************************************/
MvarMVStruct *IritMvarTwoMVsMorphing(const MvarMVStruct *MV1,
				     const MvarMVStruct	*MV2,
				     CagdRType Blend);

/*****************************************************************************
* Packing problems.							     *
*****************************************************************************/
MvarPtStruct *IritMvar3CircsInTriangles(const CagdPType Pts[3],
					CagdRType SubdivTol,
					CagdRType NumericTol);
MvarPtStruct *IritMvar6CircsInTriangles(const CagdPType Pts[3],
				    CagdRType SubdivTol,
				    CagdRType NumericTol);

/*****************************************************************************
* Steward Platform problem.						     *
*****************************************************************************/
MvarMVStruct **IritMvarStewartPlatformGenEqns(
				       const CagdPType BottomBase[3],
				       const CagdRType TopBaseEdgeLengths[6],
				       const CagdPType WorkDomain[2]);
MvarPtStruct *IritMvarStewartPlatformSolve(const MvarMVStruct **AllCnstrnts,
				       const CagdRType BaseConnectLengths[6],
				       const CagdPType WorkDomain[2],
				       CagdRType SubdivTol,
				       CagdRType NumericTol);
MvarPtStruct *IritMvarStewartPlatform2Solve(
					const CagdPType BottomBasePoints[3],
					const CagdRType BotTopEdgeLengths[6],
					const CagdRType TopEdgeLengths[3],
					CagdBType Rational,
					CagdRType SubdivTol,
					CagdRType NumericTol);

/*****************************************************************************
* Light	ray traps between n curves/surface.				     *
*****************************************************************************/
MvarPtStruct *IritMvarComputeRayTraps(const CagdCrvStruct *Crvs,
				      int Orient,
				      CagdRType	SubdivTol,
				      CagdRType	NumerTol,
				      CagdBType	UseExprTree);
MvarPtStruct *IritMvarComputeRayTraps3D(const CagdSrfStruct *Srfs,
				    int Orient,
				    CagdRType SubdivTol,
				    CagdRType NumerTol,
				    CagdBType UseExprTree);

/*****************************************************************************
* Control related analysis.						     *
*****************************************************************************/
MvarPtStruct *IritMvarCtrlComputeCrvNCycle(const CagdCrvStruct *Crv,
					   int CycLen,
					   CagdRType SubdivTol,
					   CagdRType NumerTol);
MvarPtStruct *IritMvarCtrlComputeSrfNCycle(const CagdSrfStruct *Srf,
					   int CycLen,
					   CagdRType SubdivTol,
					   CagdRType NumerTol);

/*****************************************************************************
* Surface/Check	surface	accessibility analysis.				     *
*****************************************************************************/
MvarPolylineStruct *IritMvarSrfAccessibility(const CagdSrfStruct *PosSrf,
					     const CagdSrfStruct *OrientSrf,
					     const CagdSrfStruct *CheckSrf,
					     CagdRType SubdivTol,
					     CagdRType NumerTol);
MvarPtStruct *IritMvarSrfSilhInflections(const CagdSrfStruct *Srf,
					 const CagdVType ViewDir,
					 CagdRType SubdivTol,
					 CagdRType NumerTol);
MvarMVStruct **IritMvarFlecnodalCrvsCreateMVCnstrnts(
						    const CagdSrfStruct *CSrf);
MvarPolylineStruct *IritMvarSrfFlecnodalCrvs(const CagdSrfStruct *Srf, 
					     CagdRType Step, 
					     CagdRType SubdivTol, 
					     CagdRType NumerTol);
MvarPtStruct *IritMvarSrfFlecnodalPts(const CagdSrfStruct *Srf,
				      CagdRType SubdivTol,
				      CagdRType NumerTol);
MvarMVStruct *IritMVarProjNrmlPrmt2MVScl(const CagdSrfStruct *Srf,
					 const CagdSrfStruct *NrmlSrf,
					 const MvarMVStruct *MVScl);

/*****************************************************************************
* Freeform curvature analysis.						     *
*****************************************************************************/
MvarPtStruct *IritMvarSrfRadialCurvature(const CagdSrfStruct *Srf,
					 const CagdVType ViewDir,
					 CagdRType SubdivTol,
					 CagdRType NumerTol);
MvarMVStruct *IritMvarCrvCrvtrByOneCtlPt(const CagdCrvStruct *Crv,
					 int CtlPtIdx,
					 CagdRType Min,
					 CagdRType Max);

/*****************************************************************************
* Freeform topology analysis.						     *
*****************************************************************************/
MvarPtStruct *IritMvarImplicitCrvExtreme(const CagdSrfStruct *Srf,
					 const CagdSrfDirType Dir,
					 CagdRType SubdivTol,
					 CagdRType NumerTol);

/*****************************************************************************
* Freeform poles treatments.						     *
*****************************************************************************/
MvarPolylineStruct *IritMvarRationalSrfsPoles(const CagdSrfStruct *Srf,
					      CagdRType	SubdivTol,
					      CagdRType	NumericTol);
struct TrimSrfStruct *IritMvarSrfSplitPoleParams(const CagdSrfStruct *Srf,
						 CagdRType SubdivTol,
						 CagdRType NumericTol,
						 CagdRType OutReach);

/*****************************************************************************
* Silhouette (and related curves') tracing.				     *
*****************************************************************************/
struct IPObjectStruct *IritMvarSrfSilhouette(const CagdSrfStruct *Srf,
					     const CagdVType VDir,
					     CagdRType Step,
					     CagdRType SubdivTol,
					     CagdRType NumericTol,
					     CagdBType Euclidean);
struct IPObjectStruct *IritMvarSrfSilhouetteThroughPoint(
						     const CagdSrfStruct *Srf,
					             const CagdPType VPoint,
					             CagdRType Step,
					             CagdRType SubdivTol,
					             CagdRType NumericTol,
					             CagdBType Euclidean);
struct IPObjectStruct *IritMvarSrfSilhouetteThroughPoint2(
						   MvarMVStruct *SrfMv,
						   const MvarMVStruct *NrmlMv,
						   const CagdPType VPoint,
						   CagdRType Step,
						   CagdRType SubdivTol,
						   CagdRType NumericTol);

/*****************************************************************************
* Routines to handle the computation of	2D skeletons and minimum spanning    *
* circles.								     *
*****************************************************************************/
CagdRType IritMvarSkel2DSetEpsilon(CagdRType NewEps);
CagdRType IritMvarSkel2DSetFineNess(CagdRType NewFineNess);
CagdRType IritMvarSkel2DSetMZeroTols(CagdRType SubdivTol, CagdRType NumerTol);
CagdRType IritMvarSkel2DSetOuterExtent(CagdRType NewOutExtent);
IritMvarSkel2DInter3PrimsStruct *IritMvarSkel2DInter3Prims(
						  MvarSkel2DPrimStruct *Prim1,
						  MvarSkel2DPrimStruct *Prim2,
						  MvarSkel2DPrimStruct *Prim3);
void IritMvarSkel2DInter3PrimsFree(IritMvarSkel2DInter3PrimsStruct *SK2DInt);
void IritMvarSkel2DInter3PrimsFreeList(
				IritMvarSkel2DInter3PrimsStruct *SK2DIntList);

int IritMvarMSCircOfTwoCurves(const CagdCrvStruct *Crv1,
			      const CagdCrvStruct *Crv2,
			      CagdRType Center[2],
			      CagdRType *Radius,
			      CagdRType SubdivTol,
			      CagdRType NumerTol);
int IritMvarMSCircOfThreeCurves(const CagdCrvStruct *Crv1,
				const CagdCrvStruct *Crv2,
				const CagdCrvStruct *Crv3,
				CagdRType Center[2],
				CagdRType *Radius,
				CagdRType SubdivTol,
				CagdRType NumerTol);
int IritMVarIsCrvInsideCirc(const CagdCrvStruct *Crv,
			    CagdRType Center[2],
			    CagdRType Radius,
			    CagdRType Tolerance);
int IritMvarMinSpanCirc(struct IPObjectStruct *Objs,
			CagdRType *Center,
			CagdRType *Radius,
			CagdRType SubdivTol,
			CagdRType NumerTol);
MvarPtStruct *IritMvarTanHyperSpheresofNManifolds(MvarMVStruct **MVs,
						  int NumOfMVs,
						  CagdRType SubdivTol,
						  CagdRType NumerTol,
						  CagdBType UseExprTree);

/*****************************************************************************
* Routines to handle offsets.						     *
*****************************************************************************/
CagdCrvStruct *IritMvarCrvTrimGlblOffsetSelfInter(CagdCrvStruct *Crv,
						  const	CagdCrvStruct *OffCrv,
						  CagdRType TrimAmount,
						  CagdRType SubdivTol,
						  CagdRType NumericTol);
CagdCrvStruct *IritMvarCrvTrimGlblOffsetSelfInter2(const CagdCrvStruct *Crv,
						   CagdRType OffsetDist,
						   int Operation,
						   CagdRType SubdivTol,
						   CagdRType NumericTol);

MvarPtStruct *IritMvarCrvDtctSelfInterLocations(const CagdCrvStruct *Crv,
						CagdRType OffsetDist,
						CagdRType SubdivTol,
						CagdRType NumericTol);
CagdCrvStruct *IritMvarCrvTrimSelfInterLocations(const CagdCrvStruct *Crv,
						 CagdRType OffsetDist,
						 CagdRType SubdivTol,
						 CagdRType NumericTol,
						 CagdBType PurgeSelfInters);

struct IPObjectStruct *IritMvarSrfTrimGlblOffsetSelfInter(
						   CagdSrfStruct *Srf,
						   const CagdSrfStruct *OffSrf,
						   CagdRType TrimAmount,
						   int Validate,
						   int Euclidean,
						   CagdRType SubdivTol,
						   CagdRType NumerTol,
						   CagdBType NumerImp);
struct IPObjectStruct *IritMvarSrfTrimGlblOffsetSelfInterNI(
					 	struct IPPolygonStruct *Plls, 
						const CagdSrfStruct *OffSrf, 
						CagdRType SubdivTol, 
						CagdRType NumerTol,
						int Euclidean,
						CagdRType SameUVTol);
struct IPObjectStruct *IritMvarSrfFindOffsetSelfInter(
						  const CagdSrfStruct *Srf,
					          CagdRType OffsetDist,
					          CagdRType SubdivTol,
					          CagdRType NumericTol, 
					          int OutputType);

/*****************************************************************************
* Routines to handle Hausdorff/minimal/maximal distances between freeforms.  *
*****************************************************************************/

CagdRType IritMvarDistPointCrvC1(CagdPType P,
				 const CagdCrvStruct *Crv,
				 MvarHFDistParamStruct *Param,
				 CagdBType MinDist,
				 CagdRType Epsilon);
CagdRType IritMvarHFExtremeLclDistPointCrvC1(CagdPType P,
					     const CagdCrvStruct *Crv1,
					     const CagdCrvStruct *Crv2,
					     MvarHFDistParamStruct *Param2,
					     CagdRType Epsilon);
MvarPtStruct *IritMvarHFDistAntipodalCrvCrvC1(const CagdCrvStruct *Crv1,
					      const CagdCrvStruct *Crv2,
					      CagdRType Epsilon);
MvarHFDistPairParamStruct *IritMvarHFDistInterBisectCrvCrvC1(
						     const CagdCrvStruct *Crv1,
						     const CagdCrvStruct *Crv2,
						     CagdRType Epsilon);
CagdRType IritMvarHFDistFromCrvToCrvC1(const CagdCrvStruct *Crv1,
				       const CagdCrvStruct *Crv2,
				       MvarHFDistParamStruct *Param1,
				       MvarHFDistParamStruct *Param2,
				       CagdRType Epsilon);
CagdRType IritMvarHFDistCrvCrvC1(const CagdCrvStruct *Crv1,
				 const CagdCrvStruct *Crv2,
				 MvarHFDistParamStruct *Param1,
				 MvarHFDistParamStruct *Param2,
				 CagdRType Epsilon);

CagdRType IritMvarHFDistPointSrfC1(const CagdPType P,
				   const CagdSrfStruct *Srf,
				   MvarHFDistParamStruct *Param,
				   CagdBType MinDist);
CagdRType IritMvarHFExtremeLclDistPointSrfC1(const CagdPType P,
					     const CagdSrfStruct *Srf1,
					     const CagdSrfStruct *Srf2,
					     MvarHFDistParamStruct *Param2);
MvarPtStruct *IritMvarHFDistAntipodalCrvSrfC1(const CagdSrfStruct *Srf1,
					      const CagdCrvStruct *Crv2);
CagdRType IritMvarHFDistFromCrvToSrfC1(const CagdCrvStruct *Crv1,
				       const CagdSrfStruct *Srf2,
				       MvarHFDistParamStruct *Param1,
				       MvarHFDistParamStruct *Param2);
CagdRType IritMvarHFDistFromSrfToCrvC1(const CagdSrfStruct *Srf1,
				       const CagdCrvStruct *Crv2,
				       MvarHFDistParamStruct *Param1,
				       MvarHFDistParamStruct *Param2);
CagdRType IritMvarHFDistSrfCrvC1(const CagdSrfStruct *Srf1,
				 const CagdCrvStruct *Crv2,
				 MvarHFDistParamStruct *Param1,
				 MvarHFDistParamStruct *Param2);
MvarPtStruct *IritMvarHFDistAntipodalSrfSrfC1(const CagdSrfStruct *Srf1,
					      const CagdSrfStruct *Srf2);
MvarHFDistPairParamStruct *IritMvarHFDistBisectSrfSrfC1(
						    const CagdSrfStruct *Srf1,
						    const CagdSrfStruct *Srf2);
MvarHFDistPairParamStruct *IritMvarHFDistInterBisectSrfSrfC1(
						    const CagdSrfStruct *Srf1,
						    const CagdSrfStruct *Srf2);
CagdRType IritMvarHFDistFromSrfToSrfC1(const CagdSrfStruct *Srf1,
				       const CagdSrfStruct *Srf2,
				       MvarHFDistParamStruct *Param1,
				       MvarHFDistParamStruct *Param2);
CagdRType IritMvarHFDistSrfSrfC1(const CagdSrfStruct *Srf1,
				 const CagdSrfStruct *Srf2,
				 MvarHFDistParamStruct *Param1,
				 MvarHFDistParamStruct *Param2);

MvarPtStruct *IritMvarCrvCrvMinimalDist(const CagdCrvStruct *Crv1,
					const CagdCrvStruct *Crv2,
					CagdRType *MinDist,
					CagdBType ComputeAntipodals,
					CagdRType Eps,
					CagdRType OutEps);
MvarPtStruct *IritMvarCrvSrfMinimalDist(const CagdSrfStruct *Srf1,
					const CagdCrvStruct *Crv2,
					CagdRType *MinDist);
MvarPtStruct *IritMvarSrfSrfMinimalDist(const CagdSrfStruct *Srf1,
					const CagdSrfStruct *Srf2,
					CagdRType *MinDist);
CagdRType IritMvarCrvMaxXYOriginDistance(const CagdCrvStruct *Crv,
					 CagdRType Epsilon,
					 CagdRType *Param);
CagdRType IritMvarSrfLineOneSidedMaxDist(const CagdSrfStruct *Srf,
					 const CagdUVType UV1,
					 const CagdUVType UV2,
					 CagdSrfDirType ClosedDir,
					 CagdRType Epsilon);
MvarPtStruct *IritMvarLineSrfInter(const CagdPType LinePt,
				   const CagdVType LineDir,
				   const CagdSrfStruct *Srf,
				   CagdRType SubdivTol,
				   CagdRType NumericTol);
MvarPtStruct *IritMvarCrvSrfInter(const CagdCrvStruct *Crv,
				  const CagdSrfStruct *Srf,
				  CagdRType SubdivTol,
				  CagdRType NumericTol);

/*****************************************************************************
* Routines to handle curve on surface projections.			     *
*****************************************************************************/
MvarPolylineStruct *IritMvarMVOrthoCrvProjOnSrf(const CagdCrvStruct *Crv,
						const CagdSrfStruct *Srf,
						CagdRType Tol);
MvarPolylineStruct *IritMvarMVOrthoCrvProjOnTrimSrf(const CagdCrvStruct *Crv,
						    const TrimSrfStruct *TSrf,
						    CagdRType Tol);
MvarPolylineStruct *IritMvarMVOrthoCrvProjOnModel(const CagdCrvStruct *Crv,
						  const MdlModelStruct *Mdl,
						  CagdRType Tol,
						  TrimSrfStruct **TSrfs);
MvarPolylineStruct *IritMvarMVOrthoIsoCrvProjOnSrf(const CagdSrfStruct *Srf1,
						   const CagdRType RVal,
						   const CagdRType CrvT0,
						   const CagdRType CrvT1,
						   CagdSrfDirType Dir,
						   const CagdSrfStruct *Srf2,
						   CagdRType Tol);

/*****************************************************************************
* Routines to handle basis function conversions.			     *
*****************************************************************************/
MvarMVStruct *IritMvarCnvrtPeriodic2FloatMV(const MvarMVStruct *MV);
MvarMVStruct *IritMvarCnvrtFloat2OpenMV(const MvarMVStruct *MV);

/*****************************************************************************
* Routines to handle ruled surface approximation over freeform surfaces.     *
* Piecewise ruled surface approximation	can be used for, for example flank   *
* milling, wire	EDM, hot wire cutting).					     *
*****************************************************************************/
CagdCrvStruct *IritMvarPiecewiseRuledAlgApproxLineAnalyze(
					       const CagdSrfStruct *Srf,
					       CagdRType Tolerance, 
					       CagdCrvStruct **StripBoundries,
					       int CrvSizeReduction,
					       CagdRType SubdivTol,
					       CagdRType NumericTol);
struct IPObjectStruct *IritMvarPiecewiseRuledAlgApproxBuildRuledSrfs(
					   const struct IPObjectStruct *Srf,
					   const struct IPObjectStruct *TPath);

CagdSrfStruct *IritMvarPiecewiseDvlpAlgApproxLineAnalyze(
					      const CagdSrfStruct *Srf,
					      CagdRType Tolerance,
					      CagdCrvStruct **StripBoundriesUV,
					      int CrvSizeReduction,
					      CagdRType SubdivTol,
					      CagdRType NumericTol,
					      CagdRType SrfExtent,
					      int DvlpSteps);
struct IPObjectStruct *IritMvarDevelopSrfFromCrvSrf(
					      const CagdSrfStruct *Srf,
					      const CagdCrvStruct *Crv,
					      const CagdCrvStruct *OrientField,
					      int CrvSizeReduction,
					      CagdRType SubdivTol,
					      CagdRType NumericTol,
					      CagdBType Euclidean);
CagdSrfStruct *IritMvarDevelopSrfFromCrvSrfMakeSrfs(
						const CagdCrvStruct *Crv,
						const CagdSrfStruct *Srf,
						const CagdCrvStruct *UVTCrvs,
						int CrvSizeReduction);

/******************************************************************************
* Routines to multivariate algebraic symbolic manipulation as strings.	     *
******************************************************************************/
void *IritMvarZrAlgCreate();
void IritMvarZrAlgDelete(void *MVZrAlg);
int IritMvarZrAlgAssignExpr(void *MVZrAlg, const char *Name, const char *Expr);
int IritMvarZrAlgAssignNumVar(void *MVZrAlg, const char *Name, CagdRType Val);
int IritMvarZrAlgAssignMVVar(void *MVZrAlg,
			     const char *Name,
			     CagdRType DmnMin,
			     CagdRType DmnMax,
			     const MvarMVStruct *MV);
int IritMvarZrAlgGenMVCode(void *MVZrAlg, const char *Expr, FILE *f);

/******************************************************************************
* Routines to manipulate trivariate functions.				     *
******************************************************************************/
TrivTVStruct *IritMvarTrivarBoolOne(const CagdSrfStruct *Srf);
TrivTVStruct *IritMvarTrivarBoolSum(const CagdSrfStruct *Srf1,
				    const CagdSrfStruct *Srf2,
				    const CagdSrfStruct *Srf3,
				    const CagdSrfStruct *Srf4,
				    const CagdSrfStruct *Srf5,
				    const CagdSrfStruct *Srf6);
TrivTVStruct *IritMvarTrivarBoolSum2(const CagdSrfStruct *UMin,
				     const CagdSrfStruct *UMax,
				     const CagdSrfStruct *VMin,
				     const CagdSrfStruct *VMax,
				     const CagdSrfStruct *WMin,
				     const CagdSrfStruct *WMax);
TrivTVStruct *IritMvarTrivarBoolSum3(const CagdSrfStruct *Srf1,
				     const CagdSrfStruct *Srf2,
				     const CagdSrfStruct *Srf3,
				     const CagdSrfStruct *Srf4,
				     const CagdSrfStruct *Srf5,
				     const CagdSrfStruct *Srf6);
TrivTVStruct *IritMvarTrivarBoolSumRtnl(const CagdSrfStruct *Srf1,
					const CagdSrfStruct *Srf2,
					const CagdSrfStruct *Srf3,
					const CagdSrfStruct *Srf4,
					const CagdSrfStruct *Srf5,
					const CagdSrfStruct *Srf6);
TrivTVStruct *IritMvarTrivarHalfBoolSum(const CagdSrfStruct *Srf1,
					const CagdSrfStruct *Srf2);
int MvarGetSrfsCommonCrvs(const CagdSrfStruct *Srf1, 
			  const CagdSrfStruct *Srf2, 
			  CagdCrvStruct **CommonCrv);
TrivTVStruct *IritMvarTrivarOneSidedBoolSum2Srfs(const CagdSrfStruct *Srf1,
						 const CagdSrfStruct *Srf2);
TrivTVStruct *IritMvarTrivarOneSidedBoolSum3Srfs(const CagdSrfStruct *Srf1,
						 const CagdSrfStruct *Srf2,
						 const CagdSrfStruct *Srf3);
TrivTVStruct *MvarTrivarCubicTVFit(const TrivTVStruct *TV);
TrivTVStruct *MvarTrivarQuadraticTVFit(const TrivTVStruct *TV);
MvarPtStruct *IritMvarCalculateExtremePoints(const MvarMVStruct *MV);
void IritMvarTrivJacobianImprove(TrivTVStruct *TV,
				 CagdRType StepSize,
				 int NumIters);
MvarMVStruct *IritMvarCalculateTVJacobian(const TrivTVStruct *TV);
void MvarMakeUniquePointsList(MvarPtStruct **PtList, CagdRType Tol);
TrivTVStruct *IritMvarMergeTVList(const TrivTVStruct *TVList,
				  int Dir,
				  IrtRType Tolerance,
				  int InterpDiscont);
int IritMvarTVRglrIsNegJacobian(const TrivTVStruct *TV);
TrivTVStruct *IritMvarTVsRglrCorrectJacobian(TrivTVStruct *TVs);
void IritMvarTVsHierRglrCorrectJacobian(struct IPObjectStruct *TVObjs);

/*****************************************************************************
* Routines to handle multivariate compositions.				     *
*****************************************************************************/
MvarMVStruct *IritMvarMVCompose(const MvarMVStruct *TargetMV,
				const MvarMVStruct *SrcMV,
				const int *DimMapping,
				CagdBType DoMerge);
struct IPObjectStruct *IritMvarMVCompose2(const MvarMVStruct *MVMapping,
					  const MvarMVStruct *MVToCompose,
					  int HandleCrossKVLines);
struct IPObjectStruct *IritMvarMVCompose3(const struct IPObjectStruct
				                             *MappingFunction,
					  const struct IPObjectStruct *Obj,
					  int HandleCrossKVLines);
CagdBType IritMvarMVSetCompositionCheckDomains(CagdBType NewValue);
CagdBType IritMvarMVSetCompositionPropagateHigherDims(CagdBType NewValue);

MvarComposedSrfStruct *IritMvarTrimComposeMVSrf(const MvarMVStruct *MV,
						const CagdSrfStruct *Srfs);
CagdSrfStruct *IritMvarUntrimComposeMVSrf(
				   const MvarMVStruct *MV,
				   const CagdSrfStruct *Srfs,
				   CagdQuadSrfWeightFuncType UntrimWeightFunc);
MdlModelStruct *IritMvarComposeMVMdl(const MvarMVStruct *MV,
				 const MdlModelStruct *Models);
struct VMdlVModelStruct *IritMvarComposeMVVModel(
					  const MvarMVStruct *MV,
					  const struct VMdlVModelStruct *Vmdl);
MvarComposedTrivStruct *IritMvarTrimComposeMVTV(const MvarMVStruct *MV,
				            const TrivTVStruct *TV);

MvarComposedSrfStruct *IritMvarComposedSrfAssumeSrf(CagdSrfStruct *Srf);
MvarComposedSrfStruct *IritMvarComposedSrfAssumeTSrf(TrimSrfStruct *TSrf);
MvarComposedSrfStruct *IritMvarComposedSrfCopy(
					 const MvarComposedSrfStruct *CompSrf);
MvarComposedSrfStruct *IritMvarComposedSrfCopyList(
					const MvarComposedSrfStruct *CompSrfs);
void IritMvarComposedSrfFree(MvarComposedSrfStruct *CompSrf);
void IritMvarComposedSrfFreeList(MvarComposedSrfStruct *CompSrfs);

MvarComposedTrivStruct *IritMvarComposedTrivAssumeTV(TrivTVStruct *TV);
MvarComposedTrivStruct *IritMvarComposedTrivAssumeVMdl(
					        struct VMdlVModelStruct *VMdl);
void IritMvarComposedTrivFree(MvarComposedTrivStruct *TVP);
void IritMvarComposedTrivFreeList(MvarComposedTrivStruct *TVPs);
MvarComposedTrivStruct *IritMvarComposedTrivCopy(
					    const MvarComposedTrivStruct *TVP);
MvarComposedTrivStruct *IritMvarComposedTrivCopyList(
					    const MvarComposedTrivStruct *TVP);

/*****************************************************************************
* Hyperbolic analysis  of tangent line accessibility.			     *
*****************************************************************************/
CagdBType IritMvarSrfHyperbolicLocallyAccessibleDirs(
					const CagdSrfStruct *Srf,
					const CagdVType SrfVAxis,
					SymbNormalConeStruct *AccessibleCones);
CagdBType IritMvarSrfHyperbolicLocallyAccessibleDirs2(
					const CagdSrfStruct *Srf,
					SymbNormalConeStruct *AccessibleCones);

/*****************************************************************************
* Error	handling.							     *
*****************************************************************************/
MvarSetErrorFuncType IritMvarSetFatalErrorFunc(MvarSetErrorFuncType ErrorFunc);
void IritMvarFatalError(IritMvarFatalErrorType ErrID);
const char *IritMvarDescribeError(IritMvarFatalErrorType ErrID);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* IRIT_MVAR_LIB_H */
