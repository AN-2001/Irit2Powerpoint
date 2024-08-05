/*****************************************************************************
* Symb_lib.h - header file for the SYMBolic library.			     *
* This header is also the interface header to the world.		     *
* This library is closely related to symb_lib and should be linked with	it.  *
******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute	of Technology		     *
******************************************************************************
* Written by Gershon Elber, Mar. 90.					     *
*****************************************************************************/

#ifndef IRIT_SYMB_LIB_H
#define IRIT_SYMB_LIB_H

#include <stdio.h>
#include "inc_irit/irit_sm.h"
#include "inc_irit/cagd_lib.h"

#define SYMB_CRV_APRX_C1_DISCONT_ATTR_ID IRIT_ATTR_CREATE_ID(C1Discont)

typedef enum {
    SYMB_ERR_WRONG_INPUT,
    SYMB_ERR_WRONG_SRF,
    SYMB_ERR_BZR_CRV_EXPECT,
    SYMB_ERR_BZR_SRF_EXPECT,
    SYMB_ERR_BSP_CRV_EXPECT,
    SYMB_ERR_BSP_SRF_EXPECT,
    SYMB_ERR_RATIONAL_EXPECTED,
    SYMB_ERR_NO_CROSS_PROD,
    SYMB_ERR_POWER_NO_SUPPORT,
    SYMB_ERR_CRV_FAIL_CMPT,
    SYMB_ERR_SRF_FAIL_CMPT,
    SYMB_ERR_UNDEF_CRV,
    SYMB_ERR_UNDEF_SRF,
    SYMB_ERR_INVALID_CRV,
    SYMB_ERR_INVALID_SRF,
    SYMB_ERR_UNDEF_GEOM,
    SYMB_ERR_OUT_OF_RANGE,
    SYMB_ERR_DIR_NOT_CONST_UV,
    SYMB_ERR_REPARAM_NOT_MONOTONE,
    SYMB_ERR_BSPLINE_NO_SUPPORT,
    SYMB_ERR_WRONG_PT_TYPE,
    SYMB_ERR_ONLY_2D_OR_3D,
    SYMB_ERR_ONLY_2D,
    SYMB_ERR_ONLY_3D,
    SYMB_ERR_RATIONAL_NO_SUPPORT,
    SYMB_ERR_SRFS_INCOMPATIBLE,
    SYMB_ERR_CRVS_INCOMPATIBLE,
    SYMB_ERR_CANNOT_COMP_NORMAL,
    SYMB_ERR_TOO_COMPLEX,
    SYMB_ERR_UNSUPPORT_PT,
    SYMB_ERR_W_NOT_SAME,
    SYMB_ERR_SCALAR_EXPECTED,
    SYMB_ERR_POLY_CONST_SRF,
    SYMB_ERR_COPLANAR_GEOMETRY,
    SYMB_ERR_ILLEGAL_PARAMETERS,
    SYMB_ERR_INCONSIST_EDGE_BHOLE,
    SYMB_ERR_BIARC_FIT_FAIL,
    SYMB_ERR_SPL_PROD_FAILED,
    SYMB_ERR_MATCH_FAILED,
    SYMB_ERR_MINIMUM_LINEAR,
    SYMB_ERR_DIV_ZERO,
    SYMB_ERR_INVALID_AXIS,
    SYMB_ERR_WRONG_KNOT_INDEX,
    SYMB_ERR_SUBDIV_TREE_BEZ_ONLY,
    SYMB_ERR_IDENTICAL_ZERO_DATA,
    SYMB_ERR_PERODIC_NOT_SUPPORTED,
    SYMB_ERR_MISMATCH_DOMAIN,

    SYMB_ERR_UNDEFINE_ERR
} IritSymbFatalErrorType;

typedef enum {
    SYMB_SUBDIV_STRAT_ALTERNATE = 1,
    SYMB_SUBDIV_STRAT_MIN_MAX,
    SYMB_SUBDIV_STRAT_MIN_MIN
} SymbPlSubdivStrategyType;

typedef enum {
    SYMB_CRV_APPROX_UNIFORM = 0,
    SYMB_CRV_APPROX_TOLERANCE,
    SYMB_CRV_APPROX_CURVATURE
} SymbCrvApproxMethodType;

typedef enum {                       /* Type of relation between two curves. */
    SYMB_CRVREL_NO_RELATION = 0,
    SYMB_CRVREL_SAME_CRVS,
    SYMB_CRVREL_OVERLAPPING_CRVS,		
    SYMB_CRVREL_DISTINCT_CRVS
} SymbCrvRelType;

typedef enum {
    SYMB_APPROX_LOW_DEG_NO_TREE = 0,
    SYMB_APPROX_LOW_DEG_CREATE_TREE,
    SYMB_APPROX_LOW_DEG_TREE_COMPLETE,
    SYMB_APPROX_LOW_DEG_FREE_TREE
} SymbApproxLowDegStateType;

typedef enum {
    BSP_MULT_BEZ_DECOMP,
    BSP_MULT_INTERPOL,
    BSP_MULT_BLOSSOMING
} IritCagdBspMultComputationMethodType;

typedef enum { 
    SYMB_CRV_LS_APX_ERROR_PRECISE,
    SYMB_CRV_LS_APX_ERROR_PRECISE_FAST,
    SYMB_CRV_LS_APX_ERROR_FAST,
} SymbCrvLSErrorMeasureType;

typedef enum {
    SYMB_ASYMP_BOUNDS_B,
    SYMB_ASYMP_BOUNDS_A,
    SYMB_ASYMP_BOUNDS_T
} SymbSrfAsympBoundsMethod;

typedef struct SymbMultiResCrvStruct {
    struct SymbMultiResCrvStruct *Pnext;
    CagdCrvStruct **HieCrv;
    int RefineLevel, Levels, Periodic;
} SymbMultiResCrvStruct;

typedef struct SymbNormalConeStruct {
    CagdVType ConeAxis;
    CagdRType ConeAngle;
} SymbNormalConeStruct;

typedef struct SymbArcStruct {
    struct SymbArcStruct *Pnext;
    struct IPAttributeStruct *Attr;
    int Arc;
    CagdPType Pt1, Cntr, Pt2;
} SymbArcStruct;

#if defined(_WIN64) || defined(LINUX386)
#define SYMB_MAX_CRV_SUBDIV_LEN  100000        /* No subdiv. of larger crvs. */
#else
#define SYMB_MAX_CRV_SUBDIV_LEN    5000        /* No subdiv. of larger crvs. */
#endif /* _WIN64 || LINUX386 */

typedef void (*SymbSetErrorFuncType)(IritSymbFatalErrorType ErrorFunc);
typedef CagdCrvStruct *(*SymbAdapIsoDistSqrFuncType)(int Level,
						     CagdCrvStruct *Crv1,
						     CagdCrvStruct *NCrv1,
						     CagdCrvStruct *Crv2,
						     CagdCrvStruct *NCrv2);
typedef CagdRType (*SymbPlErrorFuncType)(CagdSrfStruct *Srf,
					 CagdSrfDirType Dir,
					 int SubdivDepth);
typedef CagdCrvStruct *(*SymbOffCrvFuncType)(const CagdCrvStruct *Crv,
					     CagdRType Off,
					     CagdBType B);
typedef CagdCrvStruct *(*SymbVarOffCrvFuncType)(const CagdCrvStruct *Crv,
						const CagdCrvStruct *VarOff,
						CagdBType B);
typedef int (*SymbUniformAprxSrfPtImportanceFuncType)(const CagdSrfStruct *Srf,
						      CagdRType u,
						      CagdRType v);
typedef CagdBType (*IritSymbCrv2PolylineTlrncErrorFuncType)(const CagdCrvStruct
							                 *Crv);
typedef IrtRType (*SymbDistEnergy1DFuncType)(IrtRType e, 
				             IrtRType CrvtrCrvTMin,
				             IrtRType CrvtrCrvTMax,
				             CagdCrvStruct *CrvtrCrv,
				             CagdCrvStruct *Deriv1MagSqrCrv);

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/******************************************************************************
* Routines to handle objects generically.				     *
******************************************************************************/
SymbArcStruct *IritSymbArcArrayNew(int Size);
SymbArcStruct *IritSymbArcNew(int Arc);
SymbArcStruct *IritSymbArcCopy(SymbArcStruct *Arc);
SymbArcStruct *IritSymbArcCopyList(SymbArcStruct *ArcList);
void IritSymbArcFree(SymbArcStruct *Arc);
void IritSymbArcFreeList(SymbArcStruct *ArcList);
void IritSymbArcArrayFree(SymbArcStruct *ArcArray, int Size);

/*****************************************************************************
* Routines to handle curves generically.				     *
*****************************************************************************/
CagdPolylineStruct *IritSymbCrv2Polyline(const CagdCrvStruct *Crv,
					 CagdRType TolSamplesPerCurve,
					 SymbCrvApproxMethodType Method,
					 CagdBType OptiLin);
CagdPtStruct *SymbHugeCrv2Polyline(const CagdCrvStruct *Crv,
				   int Samples,
				   CagdBType AddFirstPt,
				   CagdBType AddLastPt,
				   CagdBType AddParamVals);
IritSymbCrv2PolylineTlrncErrorFuncType IritSymbCrv2PolylineSetTlrncErrorFunc(
			     IritSymbCrv2PolylineTlrncErrorFuncType ErrorFunc);

CagdCrvStruct *IritSymbCrvAdd(const CagdCrvStruct *Crv1,
			      const CagdCrvStruct *Crv2);
CagdCrvStruct *IritSymbCrvSub(const CagdCrvStruct *Crv1,
			      const CagdCrvStruct *Crv2);
CagdCrvStruct *IritSymbCrvMult(const CagdCrvStruct *Crv1,
			       const CagdCrvStruct *Crv2);
CagdCrvStruct *IritSymbCrvInvert(const CagdCrvStruct *Crv);
CagdCrvStruct *IritSymbCrvScalarScale(const CagdCrvStruct *Crv,
				      CagdRType	Scale);
CagdCrvStruct *IritSymbCrvDotProd(const CagdCrvStruct *Crv1,
				  const	CagdCrvStruct *Crv2);
CagdCrvStruct *IritSymbCrvVecDotProd(const CagdCrvStruct *Crv,
				     const CagdRType *Vec);
CagdCrvStruct *IritSymbCrvMultScalar(const CagdCrvStruct *Crv1,
				     const CagdCrvStruct *Crv2);
CagdCrvStruct *IritSymbCrvCrossProd(const CagdCrvStruct *Crv1,
				    const CagdCrvStruct	*Crv2);
CagdCrvStruct *IritSymbCrvVecCrossProd(const CagdCrvStruct *Crv,
				       const CagdVType Vec);
CagdCrvStruct *IritSymbCrvRtnlMult(const CagdCrvStruct *Crv1X,
				   const CagdCrvStruct *Crv1W,
				   const CagdCrvStruct *Crv2X,
				   const CagdCrvStruct *Crv2W,
				   CagdBType OperationAdd);
CagdCrvStruct *IritSymbCrvDeriveRational(const CagdCrvStruct *Crv);
CagdCrvStruct *IritSymbCrvEnclosedArea(const CagdCrvStruct *Crv);
CagdRType IritSymbCrvEnclosedAreaEval(const CagdCrvStruct *Crv);
CagdCrvStruct *IritSymbCrv2DCurvatureSqr(const CagdCrvStruct *Crv);
CagdCrvStruct *IritSymbCrv3DCurvatureSqr(const CagdCrvStruct *Crv);
CagdCrvStruct *IritSymbCrv3DRadiusNormal(const CagdCrvStruct *Crv);
CagdCrvStruct *IritSymbCrv2DUnnormNormal(const CagdCrvStruct *Crv);
CagdCrvStruct *IritSymbCrv3DCurvatureNormal(const CagdCrvStruct *Crv);
CagdCrvStruct *IritSymbCrv2DCurvatureSign(const CagdCrvStruct *Crv);
CagdCrvStruct *IritSymbCrvGenSignedCrvtr(const CagdCrvStruct *Crv,
					 int Samples,
					 int Order,
					 int ArcLen);
CagdCrvStruct *IritSymbSignedCrvtrGenCrv(const CagdCrvStruct *Crvtr,
					 CagdRType Tol,
					 int Order,
					 CagdBType Periodic);
CagdCrvStruct *IritSymbMakePosCrvCtlPolyPos(const CagdCrvStruct *OrigCrv);
CagdPtStruct *IritSymbCrv2DInflectionPts(const CagdCrvStruct *Crv,
					 CagdRType Epsilon);
CagdPtStruct *IritSymbCrvExtremCrvtrPts(const CagdCrvStruct *Crv,
					CagdRType Epsilon,
					CagdBType Crv2D);
CagdPtStruct *IritSymbCrvExtremCrvtrPts2(const CagdCrvStruct *Crv,
					 CagdRType Epsilon,
					 CagdBType Crv2D);
void IritSymbCrvSplitScalarN(const CagdCrvStruct *Crv, CagdCrvStruct **Crvs);
void IritSymbCrvSplitScalar(const CagdCrvStruct *Crv,
			    CagdCrvStruct **CrvW,
			    CagdCrvStruct **CrvX,
			    CagdCrvStruct **CrvY,
			    CagdCrvStruct **CrvZ);
CagdCrvStruct *IritSymbCrvMergeScalarN(CagdCrvStruct * const *CrvVec, 
				       int NumCrvs);
CagdCrvStruct *IritSymbCrvMergeScalar(const CagdCrvStruct *CrvW,
				      const CagdCrvStruct *CrvX,
				      const CagdCrvStruct *CrvY,
				      const CagdCrvStruct *CrvZ);
CagdCrvStruct *IritSymbCrvUnitLenScalar(const CagdCrvStruct *OrigCrv,
					CagdBType Mult,
					CagdRType Epsilon);
CagdCrvStruct *IritSymbCrvUnitLenCtlPts(const CagdCrvStruct *Crv);
CagdCrvStruct *IritSymbCrvSqrtScalar(const CagdCrvStruct *OrigCrv,
				     CagdRType Epsilon);
CagdCrvStruct *IritSymbCrvArcLenSclrCrv(const CagdCrvStruct *Crv,
					CagdRType Epsilon);
CagdCrvStruct *IritSymbCrvArcLenCrv(const CagdCrvStruct *Crv,
				    CagdRType Fineness,
				    int	Order);

CagdRType IritSymbCrvArcLen(const CagdCrvStruct *Crv, CagdRType Epsilon);
CagdRType IritSymbCrvArcLen2(const CagdCrvStruct *Crv, CagdRType Epsilon);
CagdPtStruct *IritSymbCrvArcLenSteps(const CagdCrvStruct *Crv,
				     CagdRType Length,
				     CagdRType Epsilon);
CagdRType *IritSymbCrvArcLenSteps2(const CagdCrvStruct *Crv,
				   int NumSteps,
				   CagdRType Epsilon);
int IritSymbCrvMonotoneCtlPt(const CagdCrvStruct *Crv, int Axis);
CagdCrvStruct *IritSymbComposeCrvCrv(const CagdCrvStruct *Crv1,
				     const CagdCrvStruct *Crv2);
CagdCrvStruct *IritSymbComposePeriodicCrvCrv(const CagdCrvStruct *Crv1,
					     const CagdCrvStruct *Crv2,
					     CagdRType Epsilon);
int IritSymbComposeSrfSetCache(int Cache);
void IritSymbComposeSrfClrCache(const CagdSrfStruct *Srf);
CagdCrvStruct *IritSymbComposeSrfCrv(const CagdSrfStruct *Srf,
				     const CagdCrvStruct *Crv,
				     int EvalLinCrvsDirectly,
				     CagdRType FilterTinysegs);
CagdCrvStruct *IritSymbComposeSrfCrv2(const CagdSrfStruct *Srf,
				      const CagdCrvStruct *Crv,
				      CagdRType	FilterTinysegs);
CagdSrfStruct *IritSymbComposeSrfPatch(const CagdSrfStruct *Srf,
				       const CagdUVType	UV00,
				       const CagdUVType	UV01,
				       const CagdUVType	UV10,
				       const CagdUVType	UV11);
CagdCrvStruct *IritSymbComposePeriodicSrfCrv(const CagdSrfStruct *Srf,
					     const CagdCrvStruct *Crv,
					     CagdRType Epsilon);
CagdSrfStruct *IritSymbComposeSrfSrf(const CagdSrfStruct *Srf1,
				     const CagdSrfStruct *Srf2);
CagdCrvStruct *IritSymbMapUVCrv2E3(const CagdCrvStruct *Crv,
				   const CagdSrfStruct *Srf,
				   CagdBType Compose);

CagdCrvStruct **IritSymbComputeCurvePowers(const CagdCrvStruct *Crv, int Order);
CagdSrfStruct **IritSymbComputeSurfacePowers(const CagdSrfStruct *CSrf, 
					     int Order);
CagdCrvStruct *IritSymbDecomposeCrvCrv(CagdCrvStruct *Crv);
struct IPObjectStruct *IritSymbComposeTileObjectInSrf(
						    const struct IPObjectStruct
						    *PTile,
						    const CagdSrfStruct
						    *DeformSrf,
						    IrtRType UTimes,
						    IrtRType VTimes,
						    int FitObj);
CagdRType IritSymbDistCrvPoint(const CagdCrvStruct *Crv,
			       void *CrvPtPrepHandle,
			       const CagdPType Pt,
			       CagdBType MinDist,
			       CagdRType Epsilon,
			       CagdPointType DistSpace);
void *IritSymbDistCrvPointPrep(const CagdCrvStruct *CCrv);
void IritSymbDistCrvPointFree(void *CrvPtPrepHandle);
CagdPtStruct *IritSymbLclDistCrvPoint(const CagdCrvStruct *Crv,
				      void *CrvPtPrepHandle,
				      const CagdRType *Pt,
				      CagdRType	Epsilon);
CagdRType IritSymbDistCrvLine(const CagdCrvStruct *Crv,
			      const CagdLType Line,
			      CagdBType	MinDist,
			      CagdRType	Epsilon);
CagdPtStruct *IritSymbLclDistCrvLine(const CagdCrvStruct *Crv,
				     const CagdLType Line,
				     CagdRType Epsilon,
				     CagdBType InterPos,
				     CagdBType ExtremPos);
int IritSymbCrvPointInclusion(const CagdCrvStruct *Crv,
			      const CagdPType Pt,
			      CagdRType	Epsilon);
CagdPtStruct *IritSymbCrvRayInter(const CagdCrvStruct *Crv,
				  const	CagdPType RayPt,
				  const	CagdVType RayDir,
				  CagdRType Epsilon);
CagdRType IritSymbDistBuildMapToCrv(const CagdCrvStruct *Crv,
				    CagdRType Tolerance,
				    CagdRType *XDomain,
				    CagdRType *YDomain,
				    CagdRType **DiscMap,
				    CagdRType DiscMapXSize,
				    CagdRType DiscMapYSize);
CagdPtStruct *IritSymbCrvZeroSet(const CagdCrvStruct *Crv,
				 int Axis,
				 CagdRType Epsilon,
				 CagdBType NoSolsOnEndPts);
CagdPtStruct *IritSymbCrvExtremSet(const CagdCrvStruct *Crv,
				   int Axis,
				   CagdRType Epsilon,
				   CagdBType NoSolsOnEndPts);
CagdPtStruct *IritSymbCrvConstSet(const CagdCrvStruct *Crv,
				  int Axis,
				  CagdRType Epsilon,
				  CagdRType ConstVal,
				  CagdBType NoSolsOnEndPts);
CagdCrvStruct *IritSymbCrvSliceCrvsByPrllLines(const CagdCrvStruct *Crvs,
					       int Axis,
					       CagdRType Epsilon,
					       CagdVType Range);
CagdPtStruct *IritSymbScalarCrvLowDegZeroSet(CagdCrvStruct *Crv);
CagdBType IritSymbCrvPosNegWeights(const CagdCrvStruct *Crv);
CagdPtStruct *IritSymbSplitRationalCrvsPoles(const CagdCrvStruct *Crv,
					     CagdRType Epsilon);
CagdCrvStruct *IritSymbCrvSplitPoleParams(const CagdCrvStruct *Crv,
					  CagdRType Eps,
					  CagdRType OutReach);
CagdCrvStruct *IritSymbCrvsSplitPoleParams(const CagdCrvStruct *Crvs,
					   CagdRType Eps,
					   CagdRType OutReach);
CagdCrvStruct *IritSymbCrvOffset(const CagdCrvStruct *Crv,
				 CagdRType OffsetDist,
				 CagdBType BezInterp);
CagdCrvStruct *IritSymbCrvVarOffset(const CagdCrvStruct *Crv,
				    const CagdCrvStruct	*VarOffsetDist,
				    CagdBType BezInterp);
CagdCrvStruct *IritSymbCrvSubdivOffset(const CagdCrvStruct *Crv,
				       CagdRType OffsetDist,
				       CagdRType Tolerance,
				       CagdBType BezInterp);
CagdSrfStruct *IritSymbSrfCloseParallelSrfs2Shell(const CagdSrfStruct *Srf1,
						  const	CagdSrfStruct *Srf2);
CagdCrvStruct *IritSymbCrvAdapOffset(const CagdCrvStruct *OrigCrv,
				     CagdRType OffsetDist,
				     CagdRType OffsetError,
				     SymbOffCrvFuncType	OffsetAprxFunc,
				     CagdBType BezInterp);
CagdCrvStruct *IritSymbCrvAdapVarOffset(const CagdCrvStruct *OrigCrv,
					const CagdCrvStruct *VarOffsetDist,
					CagdRType OffsetError,
					SymbVarOffCrvFuncType VarOffsetAprxFunc,
					CagdBType BezInterp);
CagdCrvStruct *IritSymbCrvAdapOffsetTrim(const CagdCrvStruct *OrigCrv,
					 CagdRType OffsetDist,
					 CagdRType OffsetError,
					 SymbOffCrvFuncType OffsetAprxFunc,
					 CagdBType BezInterp);
CagdCrvStruct *IritSymbCrvLeastSquarOffset(const CagdCrvStruct *Crv,
					   CagdRType OffsetDist,
					   int NumOfSamples,
					   int NumOfDOF,
					   int Order,
					   CagdRType *Tolerance);
CagdCrvStruct *IritSymbCrvMatchingOffset(CagdCrvStruct *Crv,
					 CagdRType OffsetDist,
					 CagdRType Tolerance);
CagdCrvStruct *IritSymbCrvTrimGlblOffsetSelfInter(const CagdCrvStruct *Crv,
						  const	CagdCrvStruct *OffCrv,
						  CagdRType SubdivTol,
						  CagdRType TrimAmount,
						  CagdRType NumerTol);
CagdBType IritSymbIsOffsetLclSelfInters(CagdCrvStruct *Crv,
					CagdCrvStruct *OffCrv,
					CagdPtStruct **SIDmns);
CagdCrvStruct *IritSymbCrvCrvConvolution(CagdCrvStruct *Crv1,
					 CagdCrvStruct *Crv2,
					 CagdRType OffsetDist,
					 CagdRType Tolerance);
CagdSrfStruct *IritSymbEnvOffsetFromCrv(const CagdCrvStruct *Crv,
					CagdRType Height,
					CagdRType Tolerance);
CagdCrvStruct *IritSymbCrvOffset2CrvsJoint(CagdCrvStruct *OrigCrv1,
					   CagdCrvStruct *OrigCrv2,
					   CagdCrvStruct **OffCrv1,
					   CagdCrvStruct **OffCrv2);
CagdCrvStruct *IritSymbCrvCrvtrTrim(const CagdCrvStruct *Crv,
				    CagdRType Dist,
				    CagdRType Eps);

CagdRType *IritSymbUniformAprxPtOnCrvDistrib(const CagdCrvStruct *Crv,
					     CagdBType ParamUniform,
					     int n);
IrtRType *IritSymbDistPoint1DWithEnergy(int N,
					IrtRType XMin,
					IrtRType XMax,
					CagdCrvStruct *CrvtrCrv,
					CagdCrvStruct *Deriv1MagSqrCrv,
					int Resolution,
					SymbDistEnergy1DFuncType EnergyFunc);
CagdPtStruct *IritSymbInsertNewParam2(CagdPtStruct *PtList, CagdRType t);
void IritSymbEvalCrvCurvPrep(const CagdCrvStruct *Crv, CagdBType Init);
CagdBType IritSymbEvalCrvCurvature(const CagdCrvStruct *Crv,
				   CagdRType t,
				   CagdRType *k,
				   CagdVType Tan,
				   CagdVType BiNrml);
void IritSymbEvalCrvCurvTN(CagdVType Nrml,
			   CagdVType Tan,
			   CagdVType BiNrml,
			   int Normalize);

/******************************************************************************
* Routines to handle surfaces generically.				     *
******************************************************************************/
struct IPPolygonStruct *IritSymbSrf2Polygons(const CagdSrfStruct *Srf,
					     int FineNess,
					     CagdBType ComputeNormals,
					     CagdBType FourPerFlat,
					     CagdBType ComputeUV);
CagdPolylineStruct *IritSymbSrf2Polylines(const CagdSrfStruct *Srf,
					  int NumOfIsocurves[2],
					  CagdRType TolSamples,
					  SymbCrvApproxMethodType Method);
CagdCrvStruct *IritSymbSrf2Curves(const CagdSrfStruct *Srf,
				  int NumOfIsocurves[2]);
CagdSrfStruct *IritSymbSrfAdd(const CagdSrfStruct *Srf1,
			      const CagdSrfStruct *Srf2);
CagdSrfStruct *IritSymbSrfSub(const CagdSrfStruct *Srf1,
			      const CagdSrfStruct *Srf2);
CagdSrfStruct *IritSymbSrfMult(const CagdSrfStruct *Srf1, 
			       const CagdSrfStruct *Srf2);
CagdSrfStruct *IritSymbSrfInvert(const CagdSrfStruct *Srf);
CagdSrfStruct *IritSymbSrfScalarScale(const CagdSrfStruct *Srf,
				      CagdRType	Scale);
CagdSrfStruct *IritSymbSrfDotProd(const CagdSrfStruct *Srf1,
				  const	CagdSrfStruct *Srf2);
CagdSrfStruct *IritSymbSrfVecDotProd(const CagdSrfStruct *Srf,
				     const CagdVType Vec);
CagdSrfStruct *IritSymbSrfMultScalar(const CagdSrfStruct *Srf1,
				     const CagdSrfStruct *Srf2);
CagdSrfStruct *IritSymbSrfCrossProd(const CagdSrfStruct *Srf1,
				    const CagdSrfStruct	*Srf2);
CagdSrfStruct *IritSymbSrfVecCrossProd(const CagdSrfStruct *Srf,
				       const CagdVType Vec);
CagdSrfStruct *IritSymbSrfRtnlMult(const CagdSrfStruct *Srf1X,
				   const CagdSrfStruct *Srf1W,
				   const CagdSrfStruct *Srf2X,
				   const CagdSrfStruct *Srf2W,
				   CagdBType OperationAdd);
CagdSrfStruct *IritSymbSrfDeriveRational(const CagdSrfStruct *Srf,
					 CagdSrfDirType	Dir);
CagdSrfStruct *IritSymbSrfNormalSrf(const CagdSrfStruct *Srf);
CagdSrfStruct *IritSymbSrfNormalSrfReversed(const CagdSrfStruct *Srf);
CagdSrfStruct *IritSymb2DSrfJacobian(const CagdSrfStruct *Srf);
void IritSymbMeshAddSub(CagdRType **DestPoints,
			CagdRType * const *Points1,
			CagdRType * const *Points2,
			CagdPointType PType,
			int Size,
			CagdBType OperationAdd);
void IritSymbMeshAddSubTo(CagdRType **DestPoints,
			  CagdRType * const *Points2,
			  CagdPointType	PType,
			  int Size,
			  CagdBType OperationAdd);
void IritSymbSrfSplitScalarN(const CagdSrfStruct *Srf,
			     CagdSrfStruct *Srfs[CAGD_MAX_PT_SIZE]);
void IritSymbSrfSplitScalar(const CagdSrfStruct *Srf,
			    CagdSrfStruct **SrfW,
			    CagdSrfStruct **SrfX,
			    CagdSrfStruct **SrfY,
			    CagdSrfStruct **SrfZ);
CagdSrfStruct *IritSymbSrfMergeScalarN(CagdSrfStruct * const *SrfVec, 
				       int NumSrfs);
CagdSrfStruct *IritSymbSrfMergeScalar(const CagdSrfStruct *SrfW,
				      const CagdSrfStruct *SrfX,
				      const CagdSrfStruct *SrfY,
				      const CagdSrfStruct *SrfZ);
CagdCrvStruct *IritSymbPrmtSclrCrvTo2D(const CagdCrvStruct *Crv,
				       CagdRType Min,
				       CagdRType Max);
CagdSrfStruct *IritSymbPrmtSclrSrfTo3D(const CagdSrfStruct *Srf,
				       CagdRType UMin, CagdRType UMax,
				       CagdRType VMin, CagdRType VMax);
CagdRType *IritSymbExtremumCntPtValsToData(CagdRType * const *Points,
					   int Length,
					   CagdBType FindMinimum,
					   CagdRType *Extremum);
CagdRType *IritSymbExtremumCntPtValsMalloc(CagdRType * const *Points,
					   int Length,
					   CagdBType FindMinimum);

/* Adaptive iso synthsis modeul: */
typedef struct SymbAdapIsoGenInfoStruct *SymbAdapIsoGenInfoStructPtr;

SymbAdapIsoGenInfoStructPtr IritSymbAdapIsoExtractInit(void);
void IritSymbAdapIsoExtractFree(SymbAdapIsoGenInfoStructPtr GI);
CagdCrvStruct *IritSymbAdapIsoSkewDistSqr(int Level,
					  CagdCrvStruct	*Crv1,
					  CagdCrvStruct	*NCrv1,
					  CagdCrvStruct	*Crv2,
					  CagdCrvStruct	*NCrv2);
CagdCrvStruct *IritSymbAdapIsoExtract(
				    SymbAdapIsoGenInfoStructPtr GI,
				    const CagdSrfStruct *Srf,
				    const CagdSrfStruct *NSrf,
				    SymbAdapIsoDistSqrFuncType AdapIsoDistFunc,
				    CagdSrfDirType Dir,
				    CagdRType	Eps,
				    CagdBType	FullIso,
				    CagdBType	SinglePath);
struct IPObjectStruct *IritSymbAdapIsoExtractRectRgns(
					       SymbAdapIsoGenInfoStructPtr GI,
					       const CagdSrfStruct *Srf,
					       CagdSrfDirType Dir,
					       CagdRType Size,
					       int Smoothing,
					       int OutputType);
void IritSymbAdapIsoSetWeightPt(SymbAdapIsoGenInfoStructPtr GI,
				CagdRType *Pt,
				CagdRType Scale,
				CagdRType Width);
int IritSymbAdapIsoSetExtractMinLevel(SymbAdapIsoGenInfoStructPtr GI,
				      int MinLevel);

CagdSrfStruct *IritSymbSrfVolume1Srf(const CagdSrfStruct *Srf,
				     CagdBType Integrate);
CagdRType IritSymbSrfVolume1(const CagdSrfStruct *Srf);
CagdSrfStruct *IritSymbSrfVolume2Srf(const CagdSrfStruct *Srf,
				     CagdBType Integrate);
CagdRType IritSymbSrfVolume2(const CagdSrfStruct *Srf);
CagdSrfStruct *IritSymbSrfFirstMomentSrf(const CagdSrfStruct *Srf,
					 int Axis,
					 CagdBType Integrate);
CagdRType IritSymbSrfFirstMoment(const CagdSrfStruct *Srf, int Axis);
CagdSrfStruct *IritSymbSrfSecondMomentSrf(const CagdSrfStruct *Srf,
					  int Axis1,
					  int Axis2,
					  CagdBType Integrate);
CagdRType IritSymbSrfSecondMoment(const CagdSrfStruct *Srf,
				  int Axis1,
				  int Axis2);

void IritSymbSrfFff(const CagdSrfStruct *Srf,
		    CagdSrfStruct **DuSrf,
		    CagdSrfStruct **DvSrf,
		    CagdSrfStruct **FffG11,
		    CagdSrfStruct **FffG12,
		    CagdSrfStruct **FffG22);
void IritSymbSrfSff(const CagdSrfStruct *DuSrf,
		    const CagdSrfStruct	*DvSrf,
		    CagdSrfStruct **SffL11,
		    CagdSrfStruct **SffL12,
		    CagdSrfStruct **SffL22,
		    CagdSrfStruct **SNormal);
void IritSymbSrfTff(const CagdSrfStruct *Srf,
		    CagdSrfStruct **TffL11,
		    CagdSrfStruct **TffL12,
		    CagdSrfStruct **TffL22);
CagdSrfStruct *IritSymbSrfDeterminant2(const CagdSrfStruct *Srf11,
				       const CagdSrfStruct *Srf12,
				       const CagdSrfStruct *Srf21,
				       const CagdSrfStruct *Srf22);
CagdSrfStruct *IritSymbSrfDeterminant3(const CagdSrfStruct *Srf11,
				       const CagdSrfStruct *Srf12,
				       const CagdSrfStruct *Srf13,
				       const CagdSrfStruct *Srf21,
				       const CagdSrfStruct *Srf22,
				       const CagdSrfStruct *Srf23,
				       const CagdSrfStruct *Srf31,
				       const CagdSrfStruct *Srf32,
				       const CagdSrfStruct *Srf33);
CagdCrvStruct *IritSymbCrvDeterminant2(const CagdCrvStruct *Crv11,
				       const CagdCrvStruct *Crv12,
				       const CagdCrvStruct *Crv21,
				       const CagdCrvStruct *Crv22);
CagdCrvStruct *IritSymbCrvDeterminant3(const CagdCrvStruct *Crv11,
				       const CagdCrvStruct *Crv12,
				       const CagdCrvStruct *Crv13,
				       const CagdCrvStruct *Crv21,
				       const CagdCrvStruct *Crv22,
				       const CagdCrvStruct *Crv23,
				       const CagdCrvStruct *Crv31,
				       const CagdCrvStruct *Crv32,
				       const CagdCrvStruct *Crv33);
CagdSrfStruct *IritSymbSrfGaussCurvature(const CagdSrfStruct *Srf,
					 CagdBType NumerOnly);
CagdSrfStruct *IritSymbSrfMeanNumer(const CagdSrfStruct *Srf);
CagdSrfStruct *IritSymbSrfMeanCurvatureSqr(const CagdSrfStruct *Srf);
CagdSrfStruct *IritSymbSrfMeanEvolute(const CagdSrfStruct *Srf);
CagdSrfStruct *IritSymbSrfIsoFocalSrf(const CagdSrfStruct *Srf,
				      CagdSrfDirType Dir);
CagdSrfStruct *IritSymbSrfCurvatureUpperBound(const CagdSrfStruct *Srf);
CagdSrfStruct *IritSymbSrfIsoDirNormalCurvatureBound(const CagdSrfStruct *Srf,
						     CagdSrfDirType Dir);

/* Surfaces's normal curvatrue bounding routines. */
typedef struct SymbSrfCrvtrBndsInfoStruct *SymbSrfCrvtrBndsInfoStructPtr;

SymbSrfCrvtrBndsInfoStructPtr IritSymbSrfCrvtrBndsInfoCreate(
						     const CagdSrfStruct *Srf);
void IritSymbSrfCrvtrBndsSplitInfo(const SymbSrfCrvtrBndsInfoStructPtr Info,
				   SymbSrfCrvtrBndsInfoStructPtr Ret,
				   CagdSrfDirType Dir);
SymbSrfCrvtrBndsInfoStructPtr IritSymbSrfCrvtrBndsSubInfo(
				      const SymbSrfCrvtrBndsInfoStructPtr Info,
				      IrtRType UMin,
				      IrtRType UMax,
				      IrtRType VMin,
				      IrtRType VMax);

void IritSymbSrfCrvtrBndsCalcBnds(const SymbSrfCrvtrBndsInfoStructPtr Info,
				  IrtRType *K1Limits,
				  IrtRType *K2Limits);

void IritSymbSrfCrvtrBndsCalcBnds2(const SymbSrfCrvtrBndsInfoStructPtr Info,
				   int Count,
				   IrtRType ValMin,
				   IrtRType ValMax,
				   IrtRType ValRes,
				   IrtRType *K1Limits,
				   IrtRType *K2Limits);
void IritSymbSrfCrvtrBndsInfoClear(SymbSrfCrvtrBndsInfoStructPtr Info);
void IritSymbSrfCrvtrBndsInfoFree(SymbSrfCrvtrBndsInfoStructPtr Info);

int IritSymbSrfCalcAsympDirsCoeffs(const CagdSrfStruct *SffMatrix[3],
				   IrtRType *AsympLimits,
				   SymbSrfAsympBoundsMethod *ChosenMethod);

/**************************/

CagdSrfStruct *IritSymbSrfDistCrvCrv(const CagdCrvStruct *Crv1,
				     const CagdCrvStruct *Crv2,
				     int DistType);
CagdPtStruct *IritSymbSrfDistFindPoints(const CagdSrfStruct *Srf,
					CagdRType Epsilon,
					CagdBType SelfInter);
CagdPtStruct *IritSymbCrvCrvInter(const CagdCrvStruct *Crv1,
				  const	CagdCrvStruct *Crv2,
				  CagdRType CCIEpsilon,
				  CagdBType SelfInter);
CagdBType IritSymbIntersectCrvWithLine(const CagdCrvStruct *OrigCrv,
				       int Axis,
				       CagdRType Val,
				       CagdCrvStruct **CrvsBelow,
				       CagdCrvStruct **CrvsAbove);
CagdSrfStruct *IritSymbConicDistCrvCrv(const CagdCrvStruct *Crv1,
				       const CagdCrvStruct *Crv2,
				       CagdRType Dist);
CagdRType IritSymbHausDistOfSampleOfPts(CagdPType * const V1,
				        CagdPType * const V2,
				        int V1Size,
				        int V2Size,
				        int HDistSide);
CagdRType IritSymbHausDistBySamplesCrvCrv(const CagdCrvStruct *Crv1,
					  const	CagdCrvStruct *Crv2,
					  int Samples,
					  int HDistSide);
CagdRType IritSymbHausDistBySamplesCrvCrv2(const CagdCrvStruct *Crv1,
					   const CagdCrvStruct *Crv2,
					   int Samples,
					   int HDistSide);
CagdRType IritSymbHausDistBySamplesCrvSrf(const CagdCrvStruct *Crv1,
					  const	CagdSrfStruct *Srf2,
					  int Samples,
					  int HDistSide);
CagdRType IritSymbHausDistBySamplesSrfSrf(const CagdSrfStruct *Srf1,
					  const	CagdSrfStruct *Srf2,
					  int Samples,
					  int HDistSide);
CagdCrvStruct *IritSymbTwoCrvsMorphing(const CagdCrvStruct *Crv1,
				       const CagdCrvStruct *Crv2,
				       CagdRType Blend);
CagdCrvStruct *IritSymbTwoCrvsMorphingCornerCut(const CagdCrvStruct *Crv1,
						const CagdCrvStruct *Crv2,
						CagdRType MinDist,
						CagdBType SameLength,
						CagdBType FilterTangencies);
CagdCrvStruct *IritSymbTwoCrvsMorphingMultiRes(const CagdCrvStruct *Crv1,
					       const CagdCrvStruct *Crv2,
					       CagdRType BlendStep);
CagdSrfStruct *IritSymbTwoSrfsMorphing(const CagdSrfStruct *Srf1,
				       const CagdSrfStruct *Srf2,
				       CagdRType Blend);
CagdSrfStruct *IritSymbSrfOffset(const CagdSrfStruct *Srf,
				 CagdRType OffsetDist);
CagdSrfStruct *IritSymbSrfVarOffset(const CagdSrfStruct *CSrf,
				    const CagdSrfStruct	*VarOffsetDist);
CagdSrfStruct *IritSymbSrfSubdivOffset(const CagdSrfStruct *Srf,
				       CagdRType OffsetDist,
				       CagdRType Tolerance);
CagdUVType *IritSymbUniformAprxPtOnSrfDistrib(
		        const CagdSrfStruct *Srf,
			CagdBType ParamUniform,
			int n,
			SymbUniformAprxSrfPtImportanceFuncType EvalImportance);
void IritSymbUniformAprxPtOnSrfPrepDistrib(const CagdSrfStruct *Srf,
					   CagdUVType **DistUV,
					   CagdRType **DistProb,
					   int *DistSize,
					   int n);
CagdUVType *IritSymbUniformAprxPtOnSrfGetDistrib(const CagdSrfStruct *Srf,
						 CagdUVType *DistUV,
						 CagdRType *DistProb,
						 int DistSize,
						 int *n);
void IritSymbEvalSrfCurvPrep(const CagdSrfStruct *Srf, CagdBType Init);
CagdBType IritSymbEvalSrfCurvature(const CagdSrfStruct *Srf,
				   CagdRType U,
				   CagdRType V,
				   CagdBType DirInUV,
				   CagdRType *K1,
				   CagdRType *K2,
				   CagdVType D1,
				   CagdVType D2);
int IritSymbEvalSrfAsympDir(const CagdSrfStruct *Srf,
			    CagdRType U,
			    CagdRType V,
			    CagdBType DirInUV,
			    CagdVType AsympDir1,
			    CagdVType AsympDir2);

/*****************************************************************************
* Routines to handle Bezier curves.					     *
*****************************************************************************/
CagdCrvStruct *IritSymbBzrCrvMult(const CagdCrvStruct *Crv1,
				  const	CagdCrvStruct *Crv2);
void IritSymbBzrCrvMultPtsVecs(const CagdRType *Pts1,
			       int Order1,
			       const CagdRType *Pts2,
			       int Order2,
			       CagdRType *ProdPts);
CagdCrvStruct *IritSymbBzrCrvMultList(const CagdCrvStruct *Crv1Lst,
				      const CagdCrvStruct *Crv2Lst);

CagdCrvStruct *IritSymbBzrCrvFactorT(const CagdCrvStruct *Crv);
CagdCrvStruct *IritSymbBzrCrvFactor1MinusT(const CagdCrvStruct *Crv);

CagdCrvStruct *IritSymbApproxCrvAsBzrCubics(const CagdCrvStruct *Crv,
					    CagdRType Tol,
					    CagdRType MaxLen);
CagdCrvStruct *IritSymbApproxCrvAsBzrQuadratics(const CagdCrvStruct *Crv,
						CagdRType Tol,
						CagdRType MaxLen);
CagdCrvStruct *IritCagdBzrComposeCrvCrv(const CagdCrvStruct *Crv1,
					const CagdCrvStruct *Crv2);
CagdCrvStruct *IritCagdBzrComposeSrfCrv(const CagdSrfStruct *Srf,
					const CagdCrvStruct *Crv);
CagdSrfStruct *IritCagdBzrComposeSrfSrf(const CagdSrfStruct *Srf1,
					const CagdSrfStruct *Srf2);
CagdSrfStruct *IritCagdBzrSrfSubdivAtCurve(const CagdSrfStruct *Srf,
					   const CagdCrvStruct *DivCrv);

/*****************************************************************************
* Routines to handle B-spline curves.					     *
*****************************************************************************/
IritCagdBspMultComputationMethodType IritCagdBspMultComputationMethod(
		           IritCagdBspMultComputationMethodType BspMultMethod);
CagdCrvStruct *IritCagdBspCrvMult(const CagdCrvStruct *Crv1,
				  const	CagdCrvStruct *Crv2);
CagdCrvStruct *IritCagdBspCrvBlossomMult(const CagdCrvStruct *Crv1,
					 const CagdCrvStruct *Crv2);
void IritSymbBspBasisInnerProdPrep(const CagdRType *KV,
				   int Len,
				   int Order1,
				   int Order2,
				   CagdCrvStruct **InnerProdCrv1,
				   CagdCrvStruct **InnerProdCrv2);
void IritSymbBspBasisInnerProdPrep2(const CagdRType *KV1,
				    const CagdRType *KV2,
				    int	Len1,
				    int	Len2,
				    int	Order1,
				    int	Order2,
				    CagdCrvStruct **InnerProdCrv1,
				    CagdCrvStruct **InnerProdCrv2);
CagdRType IritSymbBspBasisInnerProd(int Index1, int Index2,
				    CagdCrvStruct *InnerProdCrv1,
				    CagdCrvStruct *InnerProdCrv2);

CagdRType **IritSymbBspBasisInnerProdMat(const CagdRType *KV,
					 int Len,
					 int Order1,
					 int Order2);
CagdRType IritSymbBspBasisInnerProd2(const CagdRType *KV,
				     int Len,
				     int Order1,
				     int Order2,
				     int Index1,
				     int Index2);

/*****************************************************************************
* Routines to handle Bezier surfaces.					     *
*****************************************************************************/
CagdSrfStruct *IritCagdBzrSrfMult(const CagdSrfStruct *Srf1,
				  const	CagdSrfStruct *Srf2);
CagdSrfStruct *IritCagdBzrSrfFactorBilinear(const CagdSrfStruct *Srf,
					    const CagdRType *A);
CagdSrfStruct *IritCagdBzrSrfFactorUMinusV(const CagdSrfStruct *Srf);
void IritCagdBzrSrfFactorLowOrders(const CagdSrfStruct *Srf,
				   CagdSrfStruct **S11,
				   CagdSrfStruct **S12,
				   CagdSrfStruct **S21,
				   CagdSrfStruct **S22);
CagdSrfStruct *IritCagdBzrSrfFactorExtremeRowCol(const CagdSrfStruct *Srf,
						 CagdSrfBndryType Bndry);

/*****************************************************************************
* Routines to handle B-spline surfaces.					     *
*****************************************************************************/
CagdSrfStruct *IritCagdBspSrfMult(const CagdSrfStruct *Srf1,
				  const	CagdSrfStruct *Srf2);
CagdSrfStruct *IritCagdBspSrfBlossomMult(const CagdSrfStruct *Srf1,
					 const CagdSrfStruct *Srf2);
CagdSrfStruct *IritCagdBspSrfFactorBilinear(const CagdSrfStruct *Srf,
					    const CagdRType *A);
CagdSrfStruct *IritCagdBspSrfFactorUMinusV(const CagdSrfStruct *Srf);

/*****************************************************************************
* Routines to handle layout (prisa) of surfaces.			     *
*****************************************************************************/
CagdSrfStruct *IritSymbAllPrisaSrfs(const CagdSrfStruct *Srfs,
				    int	SamplesPerCurve,
				    CagdRType Epsilon,
				    CagdSrfDirType Dir,
				    const CagdVType Space);
CagdSrfStruct *IritSymbPiecewiseRuledSrfApprox(const CagdSrfStruct *Srf,
					       CagdBType ConsistentDir,
					       CagdRType Epsilon,
					       CagdSrfDirType Dir);
CagdSrfStruct *IritSymbPrisaRuledSrf(const CagdSrfStruct *Srf,
				     int SamplesPerCurve,
				     CagdRType Space,
				     CagdVType Offset);
CagdCrvStruct *IritSymbPrisaGetCrossSections(const CagdSrfStruct *RSrfs,
					     CagdSrfDirType Dir,
					     const CagdVType Space);
CagdCrvStruct *IritSymbPrisaGetOneCrossSection(const CagdSrfStruct *RSrf,
					       CagdSrfDirType Dir,
					       CagdBType Starting,
					       CagdBType Ending);

/*****************************************************************************
* Routines to handle Multi Resolution Bspline curves.			     *
*****************************************************************************/
CagdBType IritSymbCrvMultiResKVBuild(const CagdCrvStruct *Crv,
				     CagdBType Discont,
				     CagdRType ***KVList,
				     int **KVListSizes,
				     int *KVListSize);
SymbMultiResCrvStruct *IritSymbCrvMultiResDecomp(const CagdCrvStruct *Crv,
						 CagdBType Discont);
SymbMultiResCrvStruct *IritSymbCrvMultiResDecomp2(const CagdCrvStruct *Crv,
						  CagdBType Discont,
						  CagdBType SameSpace);
CagdCrvStruct *IritSymbCrvMultiResCompos(const SymbMultiResCrvStruct *MRCrv);
CagdCrvStruct *IritSymbCrvMultiResComposAtT(const SymbMultiResCrvStruct *MRCrv,
					    CagdRType T);
void IritSymbCrvMultiResEdit(const SymbMultiResCrvStruct *MRCrv,
			     CagdRType t,
			     const CagdVType TransDir,
			     CagdRType Level,
			     CagdRType FracLevel);
CagdRType *IritSymbCrvMultiResRefineLevelToData(SymbMultiResCrvStruct *MRCrv,
						CagdRType T,
						CagdBType SpanDiscont,
						CagdRType *Domain);
CagdRType *IritSymbCrvMultiResRefineLevelMalloc(SymbMultiResCrvStruct *MRCrv,
						CagdRType T,
						CagdBType SpanDiscont);
CagdCrvStruct *IritSymbCrvMultiResBWavelet(CagdRType *KV,
					   int Order,
					   int Len,
					   int KnotIndex);
void IritSymbCrvMultiResFree(SymbMultiResCrvStruct *MRCrv);
SymbMultiResCrvStruct *IritSymbCrvMultiResNew(int Levels, CagdBType Periodic);
SymbMultiResCrvStruct *IritSymbCrvMultiResCopy(const SymbMultiResCrvStruct
					       *MRCrvOrig);

/*****************************************************************************
* Routines to compute the convex hull of piecewise freeforms		     *
*****************************************************************************/
CagdCrvStruct *IritSymbCrvCnvxHull(const CagdCrvStruct *Crv,
				   CagdRType SubdivTol);
CagdCrvStruct *IritSymbCrvListCnvxHull(CagdCrvStruct *Crvs,
				       CagdRType SubdivTol);
struct IPPolygonStruct *IritSymbCrvDiameter(const CagdCrvStruct *Crv,
					    CagdRType SubdivTol);
CagdRType *IritSymbCrvDiameterMinMaxToData(const CagdCrvStruct *Crv,
					   struct IPPolygonStruct *Cntrs,
					   int Min,
					   CagdRType *Params);
CagdRType *IritSymbCrvDiameterMinMaxMalloc(const CagdCrvStruct *Crv,
					   struct IPPolygonStruct *Cntrs,
					   int Min);
CagdPtStruct *IritSymbCrvPtTangents(const CagdCrvStruct *Crv,
				    const CagdPType Pt,
				    CagdRType Tolerance);
CagdPtStruct *IritSymbTangentToCrvAtTwoPts(const CagdCrvStruct *Crv,
					   CagdRType SubdivTol);
CagdPtStruct *IritSymbCircTanTo2Crvs(const CagdCrvStruct *Crv1,
				     const CagdCrvStruct *Crv2,
				     CagdRType Radius,
				     CagdRType Tol);

/*****************************************************************************
* Routines to compute the bisector of piecewise	freeforms		     *
*****************************************************************************/
CagdRType *SymbBsctComputeInterMidPointToData(const CagdCrvStruct *Crv1,
					      CagdRType	t1,
					      const CagdCrvStruct *Crv2,
					      CagdRType	t2,
					      CagdPType	*Inter);
CagdRType *SymbBsctComputeInterMidPointMalloc(const CagdCrvStruct *Crv1,
					      CagdRType	t1,
					      const CagdCrvStruct *Crv2,
					      CagdRType	t2);
CagdCrvStruct *IritSymbCrvBisectors(const CagdCrvStruct *Crv,
				    int	BisectFunc,
				    CagdRType SubdivTol,
				    CagdBType NumerImprove,
				    CagdBType SameNormal,
				    CagdBType SupportPrms);
CagdSrfStruct *IritSymbCrvBisectorsSrf(const CagdCrvStruct *Crv,
				       int BisectFunc);
CagdSrfStruct *IritSymbCrvBisectorsSrf2(const CagdCrvStruct *Crv);
CagdSrfStruct *IritSymbCrvBisectorsSrf3(const CagdCrvStruct *Crv);
CagdSrfStruct *IritSymbCrvCrvBisectorSrf3D(const CagdCrvStruct *Crv1,
					   const CagdCrvStruct *Crv2,
					   CagdRType Alpha);
CagdSrfStruct *IritSymbCrvBisectorsSrf3D(CagdSrfStruct *Srf1,
					 CagdSrfStruct *Srf2,
					 CagdSrfStruct *DSrf1,
					 CagdSrfStruct *DSrf2,
					 CagdRType Alpha);
CagdCrvStruct *IritSymbCrvPtBisectorCrv2D(const CagdCrvStruct *Crv,
					  const	CagdPType Pt,
					  CagdRType Alpha);
CagdSrfStruct *IritSymbCrvPtBisectorSrf3D(const CagdCrvStruct *Crv,
					  const	CagdPType Pt,
					  CagdRType RulingScale);
CagdSrfStruct *IritSymbSrfPtBisectorSrf3D(const CagdSrfStruct *Srf,
					  const	CagdPType Pt);

CagdCrvStruct *IritSymbPtCrvBisectOnSphere(const CagdPType Pt,
					   const CagdCrvStruct *Crv);
CagdCrvStruct *IritSymbPtCrvBisectOnSphere2(const CagdPType Pt,
					    const CagdCrvStruct	*Crv,
					    CagdRType SubdivTol);
CagdSrfStruct *IritSymbCrvCrvBisectOnSphere(const CagdCrvStruct *Crv1,
					    const CagdCrvStruct	*Crv2);
CagdCrvStruct *IritSymbCrvCrvBisectOnSphere2(const CagdCrvStruct *Crv1,
					     const CagdCrvStruct *Crv2,
					     CagdRType SubdivTol);
CagdSrfStruct *IritSymbCrvCrvBisectOnSphere3(const CagdCrvStruct *Crv1,
					     const CagdCrvStruct *Crv2);
CagdSrfStruct *IritSymbPlanePointBisect(const CagdPType Pt, CagdRType Size);
CagdSrfStruct *IritSymbCylinPointBisect(const CagdPType CylPt,
					const CagdVType	CylDir,
					CagdRType CylRad,
					const CagdPType	Pt,
					CagdRType Size);
CagdSrfStruct *IritSymbConePointBisect(const CagdPType ConeApex,
				       const CagdVType ConeDir,
				       CagdRType ConeAngle,
				       const CagdPType Pt,
				       CagdRType Size);
CagdSrfStruct *IritSymbSpherePointBisect(const CagdPType SprCntr,
					 CagdRType SprRad,
					 const CagdPType Pt);
CagdSrfStruct *IritSymbTorusPointBisect(const CagdPType TrsCntr,
					const CagdVType	TrsDir,
					CagdRType TrsMajorRad,
					CagdRType TrsMinorRad,
					const CagdPType	Pt);
CagdSrfStruct *IritSymbPlaneLineBisect(const CagdVType LineDir,
				       CagdRType Size);
CagdSrfStruct *IritSymbConeLineBisect(const CagdVType ConeDir,
				      CagdRType	ConeAngle,
				      const CagdVType LineDir,
				      CagdRType	Size);
CagdSrfStruct *IritSymbSphereLineBisect(const CagdPType SprCntr,
					CagdRType SprRad,
					CagdRType Size);
CagdSrfStruct *IritSymbSpherePlaneBisect(const CagdPType SprCntr,
					 CagdRType SprRad,
					 CagdRType Size);
CagdSrfStruct *IritSymbCylinPlaneBisect(const CagdPType CylPt,
					const CagdVType	CylDir,
					CagdRType CylRad,
					CagdRType Size);
CagdSrfStruct *IritSymbConePlaneBisect(const CagdPType ConeApex,
				       const CagdVType ConeDir,
				       CagdRType ConeAngle,
				       CagdRType Size);
CagdSrfStruct *IritSymbCylinSphereBisect(const CagdPType CylPt,
					 const CagdVType CylDir,
					 CagdRType CylRad,
					 const CagdPType SprCntr,
					 CagdRType SprRad,
					 CagdRType Size);
CagdSrfStruct *IritSymbSphereSphereBisect(const CagdPType SprCntr1,
					  CagdRType SprRad1,
					  const	CagdPType SprCntr2,
					  CagdRType SprRad2);
CagdSrfStruct *IritSymbConeSphereBisect(const CagdPType ConeApex,
					const CagdVType	ConeDir,
					CagdRType ConeAngle,
					const CagdPType	SprCntr,
					CagdRType SprRad,
					CagdRType Size);
CagdSrfStruct *IritSymbConeConeBisect(const CagdVType Cone1Dir,
				      CagdRType	Cone1Angle,
				      const CagdVType Cone2Dir,
				      CagdRType	Cone2Angle,
				      CagdRType	Size);
CagdSrfStruct *IritSymbTorusSphereBisect(const CagdPType TrsCntr,
					 const CagdVType TrsDir,
					 CagdRType TrsMajorRad,
					 CagdRType TrsMinorRad,
					 const CagdPType SprCntr,
					 CagdRType SprRad);
CagdSrfStruct *IritSymbTorusTorusBisect(const CagdVType Trs1Cntr,
					const CagdVType	Trs1Dir,
					CagdRType Trs1MajorRad,
					const CagdVType	Trs2Cntr,
					const CagdVType	Trs2Dir,
					CagdRType Trs2MajorRad,
					CagdRType Alpha);
CagdSrfStruct *IritSymbCylinCylinBisect(const CagdVType Cyl1Pos,
					const CagdVType	Cyl1Dir,
					CagdRType Cyl1Rad,
					const CagdVType	Cyl2Pos,
					const CagdVType	Cyl2Dir,
					CagdRType Cyl2Rad);
CagdSrfStruct *IritSymbConeConeBisect2(const CagdVType Cone1Pos,
				       const CagdVType Cone1Dir,
				       CagdRType Cone1Angle,
				       const CagdVType Cone2Pos,
				       const CagdVType Cone2Dir,
				       CagdRType Cone2Angle);
CagdSrfStruct *IritSymbConeCylinBisect(const CagdVType Cone1Pos,
				       const CagdVType Cone1Dir,
				       CagdRType Cone1Angle,
				       const CagdVType Cyl2Pos,
				       const CagdVType Cyl2Dir,
				       CagdRType Cyl2Rad);

/*****************************************************************************
* Routines to handle Normal Cones.					     *
*****************************************************************************/
const SymbNormalConeStruct *IritSymbTangentConeForCrvToData(
					   const CagdCrvStruct *Crv,
					   CagdBType Planar,
					   SymbNormalConeStruct *TangentCone);
const SymbNormalConeStruct *IritSymbTangentConeForCrvMalloc(
						    const CagdCrvStruct *Crv,
						    CagdBType Planar);
const SymbNormalConeStruct *IritSymbNormalConeForSrfToData(
					    const CagdSrfStruct *Srf,
					    SymbNormalConeStruct *NormalCone);
const SymbNormalConeStruct *IritSymbDirectionsConeAvgToData(
					    const CagdRType * const *Pts,
					    int	NumPts,
					    CagdPointType PType,
					    SymbNormalConeStruct *NormalCone);
const SymbNormalConeStruct *IritSymbDirectionsConeOptToData(
					    const CagdRType * const *Pts,
					    int	NumPts,
					    CagdPointType PType,
					    SymbNormalConeStruct *NormalCone);
const SymbNormalConeStruct *IritSymbDirectionsConeToData(
					    const CagdRType * const *Pts,
					    int NumPts,
					    CagdPointType PType,
					    SymbNormalConeStruct *NormalCone);
int IritSymbNormalConeForSrfDoOptimal(int Optimal);
const SymbNormalConeStruct *IritSymbNormalConeForSrfAvgToData(
					    const CagdSrfStruct *Srf,
					    SymbNormalConeStruct *NormalCone);
const SymbNormalConeStruct *IritSymbNormalConeForSrfOptToData(
					    const CagdSrfStruct *Srf,
					    SymbNormalConeStruct *NormalCone);
const SymbNormalConeStruct *IritSymbNormalConeForSrfMainAxisToData(
				                 const CagdSrfStruct *Srf,
						 CagdVType MainAxis,
						 SymbNormalConeStruct *Cone);
CagdBType IritSymbNormal2ConesForSrf(const CagdSrfStruct *Srf,
				     CagdRType ExpandingFactor,
				     SymbNormalConeStruct *Cone1,
				     SymbNormalConeStruct *Cone2);
CagdBType IritSymbNormalConeOverlap(const SymbNormalConeStruct *NormalCone1,
				    const SymbNormalConeStruct *NormalCone2);
SymbNormalConeStruct *IritSymbNormalConvexHullConeForSrf(
						     const CagdSrfStruct *Srf,
						     CagdRType ***CH,
						     int *NPts);
CagdBType IritSymbNormalConvexHullConeOverlap(
				      const SymbNormalConeStruct *NormalCone1,
				      const CagdRType **CH1,
				      int NPts1,
				      const SymbNormalConeStruct *NormalCone2,
				      const CagdRType **CH2,
				      int NPts2);

/*****************************************************************************
* Routines to handle Reflection	Lines.					     *
*****************************************************************************/
void IritSymbRflctLnPrepSrf(CagdSrfStruct *Srf,
			    const CagdVType ViewDir,
			    const CagdVType LnDir);
CagdSrfStruct *IritSymbRflctLnGen(CagdSrfStruct *Srf,
				  const	CagdVType ViewDir,
				  const	CagdPType LnPt,
				  const	CagdVType LnDir);
void IritSymbRflctLnFree(CagdSrfStruct *Srf);
void IritSymbRflctCircPrepSrf(CagdSrfStruct *Srf,
			      const CagdVType ViewDir,
			      const CagdPType SprCntr);
CagdSrfStruct *IritSymbRflctCircGen(CagdSrfStruct *Srf,
				    const CagdVType ViewDir,
				    const CagdPType SprCntr,
				    CagdRType ConeAngle);
void IritSymbRflctCircFree(CagdSrfStruct *Srf);
void IritSymbHighlightLnPrepSrf(CagdSrfStruct *Srf,
				const CagdVType	LnDir);
CagdSrfStruct *IritSymbHighlightLnGen(CagdSrfStruct *Srf,
				      const CagdPType LnPt,
				      const CagdVType LnDir);
void IritSymbHighlightLnFree(CagdSrfStruct *Srf);

/*****************************************************************************
* Routines to handle K-orthotomics, silhouettes, and isoclines.		     *
*****************************************************************************/
CagdCrvStruct *IritSymbCrvOrthotomic(const CagdCrvStruct *Crv,
				     const CagdPType P,
				     CagdRType K);
CagdSrfStruct *IritSymbSrfOrthotomic(const CagdSrfStruct *Srf,
				     const CagdPType P,
				     CagdRType K);
struct IPPolygonStruct *IritSymbSrfSilhouette(const CagdSrfStruct *Srf,
					      const CagdVType VDir,
					      CagdRType	SubdivTol,
					      CagdBType	Euclidean);
struct IPPolygonStruct *IritSymbSrfPolarSilhouette(const CagdSrfStruct *Srf,
						   const CagdVType VDir,
						   CagdRType SubdivTol,
						   CagdBType Euclidean);
struct IPPolygonStruct *IritSymbSrfIsocline(const CagdSrfStruct *Srf,
					    const CagdVType VDir,
					    CagdRType Theta,
					    CagdRType SubdivTol,
					    CagdBType Euclidean);

/*****************************************************************************
* Routines to construct	surfaces algebraically.				     *
*****************************************************************************/
CagdSrfStruct *IritSymbAlgebraicSumSrf(const CagdCrvStruct *Crv1,
				       const CagdCrvStruct *Crv2);
CagdSrfStruct *IritSymbAlgebraicProdSrf(const CagdCrvStruct *Crv1,
					const CagdCrvStruct *Crv2);
CagdSrfStruct *IritSymbSwungAlgSumSrf(const CagdCrvStruct *Crv1,
				      const CagdCrvStruct *Crv2);
CagdSrfStruct *IritSymbOrthoNetSrf(const CagdCrvStruct *Crv1,
				   const CagdCrvStruct *Crv2,
				   int FrontSamples,
				   int LayerSamples);

/******************************************************************************
* Routines to construct	dual curves and	surfaces.			     *
******************************************************************************/
CagdCrvStruct *IritSymbCrvDual(const CagdCrvStruct *Crv);
CagdSrfStruct *IritSymbSrfDual(const CagdSrfStruct *Srf);


/******************************************************************************
* Routines to construct	developable surface.				     *
******************************************************************************/
CagdSrfStruct *IritSymbSrfDevelopableCrvOnSrf(const CagdSrfStruct *Srf,
					      const CagdCrvStruct *Crv,
					      IrtRType Scale);
CagdSrfStruct *IritSymbSrfDevelopableSrfBetweenFrames(
						    const CagdVType Frame1Pos,
						    const CagdVType Frame1Tan,
						    const CagdVType Frame1Nrml,
						    const CagdVType Frame2Pos,
						    const CagdVType Frame2Tan,
						    const CagdVType Frame2Nrml,
						    CagdRType OtherScale,
						    CagdRType Tension);
CagdSrfStruct *IritSymbSrfDevelopableSrfBetweenFrames2(
						    const CagdVType Frame1Pos,
						    const CagdVType Frame1Tan,
						    const CagdVType Frame1Nrml,
						    const CagdVType Frame2Pos,
						    const CagdVType Frame2Tan,
						    const CagdVType Frame2Nrml,
						    CagdRType OtherScale,
						    CagdRType Tension,
						    int DOFs);

/*****************************************************************************
* Routines to handle surface surface intersections.			     *
*****************************************************************************/
CagdSrfStruct *IritSymbRuledRuledZeroSetFunc(CagdCrvStruct *C1,
					     CagdCrvStruct *C2,
					     CagdCrvStruct *D1,
					     CagdCrvStruct *D2);
CagdCrvStruct *IritSymbRuledRuledIntersection(CagdCrvStruct *C1,
					      CagdCrvStruct *C2,
					      CagdCrvStruct *D1,
					      CagdCrvStruct *D2,
					      CagdRType	SubdivTol,
					      CagdCrvStruct **PCrvs1,
					      CagdCrvStruct **PCrvs2);
CagdCrvStruct *IritSymbRingRingIntersection(CagdCrvStruct *C1,
					    CagdCrvStruct *r1,
					    CagdCrvStruct *C2,
					    CagdCrvStruct *r2,
					    CagdRType SubdivTol,
					    CagdCrvStruct **PCrvs1,
					    CagdCrvStruct **PCrvs2);
CagdSrfStruct *IritSymbRingRingZeroSetFunc(CagdCrvStruct *C1,
					   CagdCrvStruct *r1,
					   CagdCrvStruct *C2,
					   CagdCrvStruct *r2);

/*****************************************************************************
* Routines to handle knot removal of B-spline curves.			     *
*****************************************************************************/
CagdCrvStruct *IritSymbRmKntBspCrvRemoveKnots(const CagdCrvStruct *Crv,
					      CagdRType	Tolerance);
CagdCrvStruct *IritSymbRmKntBspCrvCleanKnots(const CagdCrvStruct *Crv);
CagdSrfStruct *IritSymbRmKntBspSrfRemoveKnots(const CagdSrfStruct *CSrf,
					      CagdRType	Tolerance);
CagdSrfStruct *IritSymbRmKntBspSrfRemoveKnotsDir(const CagdSrfStruct *CSrf,
						 CagdSrfDirType	Dir,
						 CagdRType Tolerance);
CagdSrfStruct *IritSymbRmKntBspSrfCleanKnots(const CagdSrfStruct *Srf);


/*****************************************************************************
* Routines to handle reverse engineering and surface recognition.	     *
*****************************************************************************/
CagdBType IritSymbIsConstCrv(const CagdCrvStruct *Crv,
			     CagdCtlPtStruct *ConstVal,
			     CagdRType Eps);
CagdBType IritSymbIsZeroCrv(const CagdCrvStruct *Crv, CagdRType Eps);
CagdBType IritSymbIsCircularCrv(const CagdCrvStruct *Crv,
				CagdPType Center,
				CagdRType *Radius,
				CagdRType Eps);
CagdBType IritSymbIsLineCrv(const CagdCrvStruct *Crv,
			    CagdPType LnPos,
			    CagdVType LnDir,
			    CagdRType Eps);
CagdBType IritSymbIsConstSrf(const CagdSrfStruct *Srf,
			     CagdCtlPtStruct *ConstVal,
			     CagdRType Eps);
CagdBType IritSymbIsZeroSrf(const CagdSrfStruct *Srf, CagdRType Eps);
CagdBType IritSymbIsSphericalSrf(const CagdSrfStruct *Srf,
				 CagdPType Center,
				 CagdRType *Radius,
				 CagdRType Eps);
int IritSymbIsExtrusionSrf(const CagdSrfStruct *Srf,
			   CagdCrvStruct **Crv,
			   CagdVType ExtDir,
			   CagdRType Eps);
CagdBType IritSymbIsDevelopSrf(const CagdSrfStruct *Srf, CagdRType Eps);
int IritSymbIsRuledSrf(const CagdSrfStruct *Srf,
		       CagdCrvStruct **Crv1,
		       CagdCrvStruct **Crv2,
		       CagdRType Eps);
CagdBType IritSymbIsSrfOfRevSrf(const CagdSrfStruct *Srf,
				CagdCrvStruct **CrossSec,
				CagdPType AxisPos,
				CagdVType AxisDir,
				CagdRType Eps);
CagdBType IritSymbIsPlanarSrf(const CagdSrfStruct *Srf,
			      IrtPlnType Plane,
			      CagdRType	Eps);

/*****************************************************************************
* Routines to handle injectivity testing of Bspline surfaces.		     *
*****************************************************************************/
CagdBType IritSymbCubicBspInjective(CagdRType x[4][4], CagdRType y[4][4]);

/*****************************************************************************
* Routines to handle blending of Bspline surfaces.			     *
*****************************************************************************/
CagdCrvStruct *IritSymbClipCrvToSrfDomain(const CagdSrfStruct *Srf,
					  const	CagdCrvStruct *UVCrv);
CagdSrfStruct *IritSymbShapeBlendOnSrf(CagdSrfStruct *Srf,
				       CagdCrvStruct *UVCrv,
				       const CagdCrvStruct *CrossSecShape,
				       CagdRType TanScale,
				       CagdRType Width,
				       const CagdCrvStruct *WidthField,
				       CagdRType Height,
				       const CagdCrvStruct *HeightField);
CagdSrfStruct *IritSymbShapeBlendSrf(const CagdCrvStruct *Pos1Crv,
				     const CagdCrvStruct *Pos2Crv,
				     const CagdCrvStruct *Dir1Crv,
				     const CagdCrvStruct *Dir2Crv,
				     const CagdCrvStruct *CrossSecShape,
				     const CagdCrvStruct *Normal);

/*****************************************************************************
* Routines to handle bi	arc approximations of curves.			     *
*****************************************************************************/
SymbArcStruct *IritSymbCrvBiArcApprox(const CagdCrvStruct *Crv,
				      CagdRType	Tolerance,
				      CagdRType	MaxAngle);
CagdCrvStruct *IritSymbArcs2Crvs(const SymbArcStruct *Arcs);

/*****************************************************************************
* Routines to handle cubic approximations of curves.			     *
*****************************************************************************/
CagdCrvStruct *IritSymbCrvCubicApprox(const CagdCrvStruct *CCrv,
				      CagdRType	Tolerance);

/******************************************************************************
* Routines to compare the similarity of	freeforms.			     *
******************************************************************************/
SymbCrvRelType IritSymbCrvsCompare(const CagdCrvStruct *Crv1,
				   const CagdCrvStruct *Crv2,
				   CagdRType Eps,
				   CagdRType *StartOvrlpPrmCrv1,
				   CagdRType *EndOvrlpPrmCrv1,
				   CagdRType *StartOvrlpPrmCrv2,
				   CagdRType *EndOvrlpPrmCrv2);
CagdCrvStruct *IritSymbCanonicBzrCrv(const CagdCrvStruct *Crv, CagdRType Eps);
CagdCrvStruct *IritSymbBzrDegReduce(const CagdCrvStruct *Crv, CagdRType Eps);

/*****************************************************************************
* Routines to handle lower envelopes of	curve arrangments.		     *
*****************************************************************************/
CagdCrvStruct *IritSymbCrvsLowerEnvelop(const CagdCrvStruct *Crvs,
					CagdRType *Pt,
					CagdRType Eps);
CagdCrvStruct *IritSymbSplitCrvsAtExtremums(const CagdCrvStruct *Crvs,
					    int	Axis,
					    const CagdPType Pt,
					    CagdRType Eps);

/*****************************************************************************
* Routines to handle parameterization of regions enclosed in planar curve.   *
*****************************************************************************/
CagdSrfStruct *IritSymb2DCrvParameterizing2Crvs(const CagdCrvStruct *Crv1,
						const CagdCrvStruct *Crv2,
						int Dir,
						CagdBType ForceMatch);
CagdSrfStruct *IritSymb2DCrvParamerize2Prms(const CagdCrvStruct *Crv,
					    CagdRType T1,
					    CagdRType T2,
					    int	Dir,
					    CagdRType *Error);
CagdSrfStruct *IritSymb2DCrvParameterizeDomain(const CagdCrvStruct *UVCrv,
					       CagdRType Eps);
void IritSymbSrfSmoothInternalCtlPts(CagdSrfStruct *Srf, CagdRType Weight);
CagdRType IritSymbSrfJacobianImprove(CagdSrfStruct *Srf,
				     CagdRType StepSize,
				     int MaxIter);

/*****************************************************************************
* Routines to handle curv curve	intersection related functionality	     *
*****************************************************************************/
CagdCrvStruct *IritSymbGet2CrvsIntersectionRegions(const CagdCrvStruct *Crv1,
						   const CagdCrvStruct *Crv2,
						   CagdRType Eps);
CagdRType IritSymbGet2CrvsIntersectionAreas(const CagdCrvStruct *Crv1,
					    const CagdCrvStruct	*Crv2,
					    CagdRType Eps);
int IritSymbGet2CrvsInterDAreaDCtlPts(CagdCrvStruct *Crv1,
				      CagdCrvStruct *Crv2,
				      CagdRType	Eps,
				      CagdRType	**InterDomains,
				      CagdRType	**dAreadPts);
CagdRType *SymbGetCrvSubRegionAlphaMatrix(const CagdCrvStruct *Crv,
					  CagdRType t1,
					  CagdRType t2,
					  int *Dim);

/*****************************************************************************
* Routines to handle approximating a polyline by a curve		     *
*****************************************************************************/

CagdCrvStruct *IritSymbCrvLstSqrAprxPlln(const CagdCtlPtStruct *Polyline,
					 CagdRType ExpectedError,
					 int Order,
					 CagdRType NumericTol,
					 CagdBType ForceC1Continuity,
					 CagdRType C1DiscontThresh,
					 SymbCrvLSErrorMeasureType ErrMeasure);

/*****************************************************************************
* Error	handling.							     *
*****************************************************************************/
SymbSetErrorFuncType IritSymbSetFatalErrorFunc(SymbSetErrorFuncType ErrorFunc);
const char *IritSymbDescribeError(IritSymbFatalErrorType ErrorNum);
void IritSymbFatalError(IritSymbFatalErrorType ErrID);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* IRIT_SYMB_LIB_H */
