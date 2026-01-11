/*****************************************************************************
* Trim_lib.h - header file for the TRIMmed surfaces library.		     *
* This header is also the interface header to the world.		     *
******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute	of Technology		     *
******************************************************************************
* Written by Gershon Elber, Oct. 94.					     *
*****************************************************************************/

#ifndef IRIT_TRIM_LIB_H
#define IRIT_TRIM_LIB_H

#include <stdio.h>
#include "inc_irit/irit_sm.h"
#include "inc_irit/miscattr.h"
#include "inc_irit/misc_lib.h"
#include "inc_irit/cagd_lib.h"
#include "inc_irit/symb_lib.h"

typedef enum {
    TRIM_ERR_TRIM_CRV_E2 = 2000,
    TRIM_ERR_BSPLINE_EXPECT,
    TRIM_ERR_BZR_BSP_EXPECT,
    TRIM_ERR_DIR_NOT_CONST_UV,
    TRIM_ERR_ODD_NUM_OF_INTER,
    TRIM_ERR_TCRV_ORIENT,
    TRIM_ERR_INCONSISTENT_CNTRS,
    TRIM_ERR_FAIL_MERGE_TRIM_SEG,
    TRIM_ERR_INVALID_TRIM_SEG,
    TRIM_ERR_INCON_PLGN_CELL,
    TRIM_ERR_TRIM_TOO_COMPLEX,
    TRIM_ERR_TRIMS_NOT_LOOPS,
    TRIM_ERR_LINEAR_TRIM_EXPECT,
    TRIM_ERR_NO_INTERSECTION,
    TRIM_ERR_POWER_NO_SUPPORT,
    TRIM_ERR_UNDEF_SRF,
    TRIM_ERR_TRIM_OPEN_LOOP,
    TRIM_ERR_TRIM_OUT_DOMAIN,
    TRIM_ERR_SINGULAR_TRIM_SEG,
    TRIM_ERR_UNTRIM_FAILED,

    TRIM_ERR_UNDEFINE_ERR
} IritTrimFatalErrorType;

/*****************************************************************************
* A trimmed surface can	have trimming curves that either form a	closed loop  *
* or start and end on the boundary of the surface. A trimming curve will be  *
* defined using	a list of TrimCrvSegStruct, creating a closed loop or a	     *
* curve	that starts and	ends in	the boundary of	the surface.		     *
*   Orientation	of TrimCrvSegStruct should be such that	the trimming curve   *
* tangent direction crossed with the surface normal points into	the inside.  *
*   EucCrv can be either NULL where the	Euclidean location must	be	     *
* computed on the fly from parametric information or, if exist,	must be	     *
* used to prevent from black holes with	adjacent surfaces.		     *
*   The	trimming curves	have no	order what so ever.			     *
*   An outmost loop will always	be present even	if the entire four boundary  *
* curves are untrimmed.							     *
*****************************************************************************/
typedef struct TrimCrvSegStruct {
    struct TrimCrvSegStruct *Pnext;
    IPAttributeStruct *Attr;
    CagdCrvStruct *UVCrv;    /* Trimming crv segment in srf's param. domain. */
    CagdCrvStruct *EucCrv;       /* Trimming curve as an E3 Euclidean curve. */
} TrimCrvSegStruct;

typedef struct TrimCrvStruct {
    struct TrimCrvStruct *Pnext;
    IPAttributeStruct *Attr;
    TrimCrvSegStruct *TrimCrvSegList;    /* List of trimming curve segments. */
} TrimCrvStruct;

typedef struct TrimSrfStruct {
    struct TrimSrfStruct *Pnext;
    IPAttributeStruct *Attr;
    int Tags;
    CagdSrfStruct *Srf;			  /* Surface trimmed by TrimCrvList. */
    TrimCrvStruct *TrimCrvList;		         /* List of trimming curves. */
} TrimSrfStruct;

typedef struct TrimUntrimResultStruct {
    struct TrimUntrimResultStruct *Pnext;
    IPAttributeStruct *Attr;
    CagdSrfStruct *ContainingSrf;
    CagdCrvQuadTileStruct *UVTiles;
    CagdSrfStruct *UntrimmedSrfs;
} TrimUntrimResultStruct; 

/* Subdivision of trimmed surfaces may result in only one surface returned   */
/* as the other is completely trimmed away. This macros should be used to    */
/* define and identify the two parts.					     */
#define TRIM_IS_FIRST_SRF(Srf)		(((Srf) -> Tags & 0x01) == 0)
#define TRIM_IS_SECOND_SRF(Srf)		(((Srf) -> Tags & 0x01) == 1)
#define TRIM_SET_FIRST_SRF(Srf)		((Srf) -> Tags &= ~0x01)
#define TRIM_SET_SECOND_SRF(Srf)	((Srf) -> Tags |= 0x01)


typedef struct TrimIsoInterStruct {      /* Holds intersections of iso curve */
    struct TrimIsoInterStruct *Pnext;		    /* with trimming curves. */
    CagdRType Param;
} TrimIsoInterStruct;

typedef void (*TrimSetErrorFuncType)(IritTrimFatalErrorType ErrorFunc);

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

TrimCrvSegStruct *IritTrimCrvSegNew(CagdCrvStruct *UVCrv,
				    CagdCrvStruct *EucCrv);
TrimCrvSegStruct *IritTrimCrvSegNewList(CagdCrvStruct *UVCrvs,
					CagdCrvStruct *EucCrvs);
TrimCrvSegStruct *IritTrimCrvSegCopy(const TrimCrvSegStruct *TrimCrvSeg);
TrimCrvSegStruct *IritTrimCrvSegCopyList(
				       const TrimCrvSegStruct *TrimCrvSegList);
void IritTrimCrvSegFree(TrimCrvSegStruct *TrimCrvSeg);
void IritTrimCrvSegFreeList(TrimCrvSegStruct *TrimCrvSegList);

#ifdef DEBUG
#define IritTrimCrvSegFree(TrimCrvSeg)     { IritTrimCrvSegFree(TrimCrvSeg); \
					     TrimCrvSeg = NULL; }
#define IritTrimCrvSegFreeList(TrimCrvSegList) { IritTrimCrvSegFreeList(TrimCrvSegList); \
					         TrimCrvSegList = NULL; }
#endif /* DEBUG */

TrimCrvStruct *IritTrimCrvNew(TrimCrvSegStruct *TrimCrvSegList);
TrimCrvStruct *IritTrimCrvCopy(const TrimCrvStruct *TrimCrv);
TrimCrvStruct *IritTrimCrvCopyList(const TrimCrvStruct *TrimCrvList);
void IritTrimCrvFree(TrimCrvStruct *TrimCrv);
void IritTrimCrvFreeList(TrimCrvStruct *TrimCrvList);

#ifdef DEBUG
#define IritTrimCrvFree(TrimCrv)         { IritTrimCrvFree(TrimCrv); \
					   TrimCrv = NULL; }
#define IritTrimCrvFreeList(TrimCrvList) { IritTrimCrvFreeList(TrimCrvList); \
				           TrimCrvList = NULL; }
#endif /* DEBUG */

TrimSrfStruct *IritTrimSrfNew(CagdSrfStruct *Srf,
			      TrimCrvStruct *TrimCrvList,
			      CagdBType	HasTopLvlTrim);
TrimSrfStruct *IritTrimSrfNew2(CagdSrfStruct *Srf,
			       CagdCrvStruct *TrimCrvList,
			       CagdBType HasTopLvlTrim);
TrimSrfStruct *IritTrimSrfNew3(CagdSrfStruct *Srf,
			       CagdCrvStruct *TrimCrvList,
			       CagdBType HasTopLvlTrim);
TrimSrfStruct *IritTrimSrfFromE3TrimmingCurves(TrimCrvStruct *TCrvs,
					       const IrtPlnType	Plane);
int IritTrimSrfVerifyTrimCrvsValidity(TrimSrfStruct *TrimSrf);
TrimSrfStruct *IritTrimSrfCopy(const TrimSrfStruct *TrimSrf);
TrimSrfStruct *IritTrimSrfCopyList(const TrimSrfStruct *TrimSrfList);
void IritTrimSrfFree(TrimSrfStruct *TrimSrf);
void IritTrimSrfFreeList(TrimSrfStruct *TrimSrfList);

#ifdef DEBUG
#define IritTrimSrfFree(TrimSrf)         { IritTrimSrfFree(TrimSrf); \
					   TrimSrf = NULL; }
#define IritTrimSrfFreeList(TrimSrfList) { IritTrimSrfFreeList(TrimSrfList); \
				           TrimSrfList = NULL; }
#endif /* DEBUG */

void IritTrimSrfTransform(TrimSrfStruct *TrimSrf,
			  const	CagdRType *Translate,
			  CagdRType Scale);
void IritTrimSrfMatTransform2(TrimSrfStruct *TrimSrf, CagdMType Mat);
TrimSrfStruct *IritTrimSrfMatTransform(const TrimSrfStruct *TrimSrf,
				       CagdMType Mat);
TrimSrfStruct *IritTrimSrfListMatTransform(const TrimSrfStruct *TrimSrf,
					   CagdMType Mat);
CagdBType IritTrimSrfsSame(const TrimSrfStruct *TSrf1,
			   const TrimSrfStruct *TSrf2,
			   CagdRType Eps);
TrimSrfStruct *IritTrimGetLargestTrimmedSrf(TrimSrfStruct **TSrfs,
					    int Extract);
const TrimCrvSegStruct *IritTrimGetOuterTrimCrv(const TrimSrfStruct *TSrf);
const TrimCrvSegStruct *IritTrimGetFullDomainTrimCrv(const TrimSrfStruct *TSrf);
CagdRType IritTrimFindClosestTrimCurve2UV(TrimCrvStruct *TCrvs,
					  const	CagdUVType UV,
					  TrimCrvSegStruct **ClosestTSeg);
CagdCrvStruct *IritTrimExtendTrimmingDomain(const CagdSrfStruct *Srf,
					    CagdCrvStruct *TrimCrvs,
					    CagdRType Extnt,
					    CagdBType MergeCrvs);
CagdCrvStruct *IritTrimGetTrimmingCurves(const TrimSrfStruct *TrimSrf,
					 CagdBType ParamSpace,
					 CagdBType EvalEuclid);
CagdCrvStruct *IritTrimGetTrimmingCurves2(const TrimCrvStruct *TrimCrvList,
					  const	TrimSrfStruct *TrimSrf,
					  CagdBType ParamSpace,
					  CagdBType EvalEuclid);
TrimSrfStruct *IritTrimManageTrimmingCurvesDegrees(TrimSrfStruct *TrimSrf,
						   int FitOrder,
						   CagdBType EvalEuclid);

TrimCrvStruct *IritTrimLinkTrimmingCurves2Loops(const TrimCrvStruct *TCrvs,
						CagdRType MaxTol,
						CagdBType *ClosedLoops);
TrimCrvStruct *IritTrimLinkTrimmingCurves2Loops1(const TrimCrvSegStruct *TSegs,
						 CagdRType MaxTol,
						 CagdBType *ClosedLoops);
TrimCrvStruct *IritTrimLinkTrimmingCurves2Loops2(TrimCrvStruct *TCrvs,
						 CagdRType Tol,
						 CagdBType *ClosedLoops);

void IritTrimClassifyTrimCrvsOrientation(TrimCrvStruct *TCrvs, CagdRType Tol);
CagdBType IritTrimVerifyClosedTrimLoop(TrimCrvStruct *TCrv,
				       CagdRType Tolerance,
				       CagdBType CoerceIdentical);
int IritTrimCoerceTrimUVCrv2Plane(TrimCrvSegStruct *TSeg);

TrimCrvStruct *IritTrimMergeTrimmingCurves2Loops(const TrimCrvStruct *TrimCrvs);
CagdCrvStruct *IritTrimMergeTrimmingCurves2Loops2(CagdCrvStruct *UVCrvs,
						  CagdRType Tol);

void IritTrimAffineTransTrimCurves(TrimCrvStruct *TrimCrvList,
				   CagdRType OldUMin,
				   CagdRType OldUMax,
				   CagdRType OldVMin,
				   CagdRType OldVMax,
				   CagdRType NewUMin,
				   CagdRType NewUMax,
				   CagdRType NewVMin,
				   CagdRType NewVMax);
TrimSrfStruct *IritTrimAffineTransTrimSrf(const TrimSrfStruct *TrimSrf,
					  CagdRType NewUMin,
					  CagdRType NewUMax,
					  CagdRType NewVMin,
					  CagdRType NewVMax);
CagdPolylineStruct *IritTrimCrvs2Polylines(TrimSrfStruct *TrimSrf,
					   CagdBType ParamSpace,
					   CagdRType TolSamples,
					   SymbCrvApproxMethodType Method);
CagdPolylineStruct *IritTrimCrv2Polyline(const CagdCrvStruct *TrimCrv,
					 CagdRType TolSamples,
					 SymbCrvApproxMethodType Method,
					 CagdBType OptiLin);
CagdCrvStruct *IritTrimEvalTrimCrvToEuclid(const CagdSrfStruct *Srf,
					   const CagdCrvStruct *UVCrv);
CagdCrvStruct *IritTrimEvalTrimCrvToEuclid2(const CagdSrfStruct *Srf,
					    const CagdCrvStruct	*UVCrv,
					    CagdCrvStruct **UVCrvLinear);
void IritTrimSrfFreeEuclideanTrimCrvs(TrimSrfStruct *TrimSrf);
int IritTrimSetEuclidLinearFromUV(int EuclidLinearFromUV);
int IritTrimSetEuclidComposedFromUV(int EuclidComposedFromUV);
void IritTrimRemovEucTrimCrvs(TrimSrfStruct *TSrf);
CagdRType *IritTrimPointInsideTrimmedCrvsToData(TrimCrvStruct *TrimCrvList,
						const TrimSrfStruct *TSrf,
						CagdUVType UVRetVal);
CagdBType IritTrimSrfTrimCrvSquareDomain(const TrimCrvStruct *TrimCrvList,
					 CagdRType *UMin,
					 CagdRType *UMax,
					 CagdRType *VMin,
					 CagdRType *VMax);
CagdBType IritTrimSrfTrimCrvAllDomain(const TrimSrfStruct *TrimSrf);
TrimSrfStruct *IritTrimClipSrfToTrimCrvs(TrimSrfStruct *TrimSrf);
TrimSrfStruct *IritTrimSrfDegreeRaise(const TrimSrfStruct *TrimSrf,
				      CagdSrfDirType Dir);
int IritTrimSrfSetStateTrimCrvsManagement(int TrimmingFitOrder);
TrimSrfStruct *IritTrimSrfSubdivAtParam(const TrimSrfStruct *TrimSrf,
					CagdRType t,
					CagdSrfDirType Dir);
TrimSrfStruct *IritTrimSrfSubdivAtInnerLoops(TrimSrfStruct *TSrf);
TrimCrvStruct *IritTrimSrfSubdivTrimCrvsAtInnerLoops(const TrimCrvStruct *TCrvs);
CagdSrfDirType IritTrimSrfSubdivValAtInnerLoop(const TrimCrvStruct *TCrvs,
					       CagdRType *SubdivVal);

TrimSrfStruct *IritTrimCnvrtBsp2BzrSrf(const TrimSrfStruct *TrimSrf);
TrimSrfStruct *IritTrimSrfCnvrt2BzrTrimSrf(const TrimSrfStruct *TrimSrf);
CagdSrfStruct *IritTrimSrfCnvrt2BzrRglrSrf(const TrimSrfStruct *TrimSrf);
CagdSrfStruct *IritTrimSrfCnvrt2BzrRglrSrf2(const TrimSrfStruct *TSrf,
					    int	ComposeE3,
					    int	OnlyBzrSrfs,
					    CagdRType Eps);
CagdSrfStruct *IritTrimSrfCnvrt2TensorProdSrf(const TrimSrfStruct *TSrf,
					      int ComposeE3,
					      CagdRType Eps);

int IritTrimSrfSubdivTrimmingCrvs(const TrimCrvStruct *TrimCrvs,
				  CagdRType t,
				  CagdSrfDirType Dir,
				  TrimCrvStruct	**TrimCrvs1,
				  TrimCrvStruct	**TrimCrvs2);
TrimSrfStruct *IritTrimSrfRegionFromTrimSrf(TrimSrfStruct *TrimSrf,
					    CagdRType t1,
					    CagdRType t2,
					    CagdSrfDirType Dir);
TrimSrfStruct *IritTrimSrfRefineAtParams(const TrimSrfStruct *Srf,
					 CagdSrfDirType	Dir,
					 CagdBType Replace,
					 CagdRType *t,
					 int n);
TrimSrfStruct *IritTrimSrfReverse(const TrimSrfStruct *TrimSrf);
TrimSrfStruct *IritTrimSrfReverse2(const TrimSrfStruct *TrimSrf);

int IritTrimRemoveCrvSegTrimCrvs(TrimCrvSegStruct *TrimCrvSeg,
				 TrimCrvStruct **TrimCrvs);
int IritTrimRemoveCrvSegTrimCrvSegs(TrimCrvSegStruct *TrimCrvSeg,
				    TrimCrvSegStruct **TrimCrvSegs);

void IritTrimSrfDomain(const TrimSrfStruct *TrimSrf,
		       CagdRType *UMin,
		       CagdRType *UMax,
		       CagdRType *VMin,
		       CagdRType *VMax);
#define IritTrimSrfSetDomain IritTrimAffineTransTrimSrf

int IritTrimCrvSegBBox(const TrimCrvSegStruct *TCrvSeg,
		       int UV,
		       CagdBBoxStruct *BBox);
int IritTrimCrvSegListBBox(const TrimCrvSegStruct *TCrvSegs,
		           int UV,
		           CagdBBoxStruct *BBox);

int IritTrimCrvBBox(const TrimCrvStruct *TCrv, int UV, CagdBBoxStruct *BBox);
int IritTrimCrvListBBox(const TrimCrvStruct *TCrvs,
			int UV,
			CagdBBoxStruct *BBox);

CagdBBoxStruct *IritTrimSrfBBox(const TrimSrfStruct *TSrf,
				CagdBBoxStruct *BBox);
CagdBBoxStruct *IritTrimSrfListBBox(const TrimSrfStruct *TSrfs,
				    CagdBBoxStruct *BBox);

int IritTrimSrfNumOfTrimLoops(const TrimSrfStruct *TSrf);
int IritTrimSrfNumOfTrimCrvSegs(const TrimSrfStruct *TSrf);

void IritTrimSrfEvalToData(const TrimSrfStruct *TrimSrf,
			   CagdRType u,
			   CagdRType v,
			   CagdRType *Pt);
CagdRType *IritTrimSrfEvalMalloc(const TrimSrfStruct *TrimSrf,
			     CagdRType u,
			     CagdRType v);

CagdCrvStruct *IritTrimSrf2Curves(TrimSrfStruct *TrimSrf, 
				  int NumOfIsocurves[2]);
CagdCrvStruct *IritTrimCrvTrimParamList(CagdCrvStruct *Crv,
					TrimIsoInterStruct *InterList);
TrimIsoInterStruct **IritTrimIntersectTrimCrvIsoVals(
						  const TrimSrfStruct *TrimSrf,
						  int Dir,
						  CagdRType *OrigIsoParams,
						  int NumOfIsocurves,
						  CagdBType Perturb);
TrimIsoInterStruct **IritTrimIntersectCrvsIsoVals(const CagdCrvStruct *UVCrvs,
						  int Dir,
						  CagdRType *IsoParams,
						  int NumOfIsocurves);
CagdCrvStruct *IritTrimCrvAgainstTrimCrvs(CagdCrvStruct *UVCrv,
					  const	TrimSrfStruct *TrimSrf,
					  CagdRType Eps);
CagdCrvStruct *IritTrimSrf2KnotCurves(TrimSrfStruct *TrimSrf);
CagdPolylineStruct *IritTrimSrf2Polylines(TrimSrfStruct *TrimSrf,
					  int NumOfIsocurves[2],
					  CagdRType TolSamples,
					  SymbCrvApproxMethodType Method,
					  CagdSrf2PlsInfoStrct *TessInfo);
struct IPPolygonStruct *IritTrimSrfAdap2Polygons(const TrimSrfStruct *TrimSrf,
						 const CagdSrf2PlsInfoStrct
						 *TessInfo);
struct IPPolygonStruct *IritTrimCrvsHierarchy2Polys(TrimCrvStruct *TrimLoops);
void IritTrimMatch2ndCrvLenSpeedAs1stCrv(CagdCrvStruct **Crv1,
					 CagdCrvStruct **Crv2,
					 const CagdSrfStruct *Srf1,
					 const CagdSrfStruct *Srf2);
void IritTrimCrvSegReverse(TrimCrvSegStruct *TSeg);
TrimCrvSegStruct *IritTrimCrvSegListReverse(TrimCrvSegStruct *TSegs);
TrimCrvSegStruct *IritTrimOrderTrimCrvSegsInLoop(TrimCrvSegStruct *TSegs);
int IritTrimEnsureNoSingleTrimCrvLoops(TrimCrvStruct **TrimLoops);
CagdBType IritTrimOrientTrimingCrvs(TrimSrfStruct *TSrf);
CagdBType IritTrimClassifyTrimmingLoops(TrimCrvStruct **TrimLoops);
CagdBType IritTrimClassifyTrimLoopOrient(const TrimCrvSegStruct *TSegs);
void IritTrimCrvFreeListWithSubTrims(TrimCrvStruct *TrimCrv);
void IritTrimCrvFreeWithSubTrims(TrimCrvStruct *TrimCrv);
CagdBType IritTrimClassifyTrimCurveOrient(const CagdCrvStruct *UVCrv);
struct IPPolygonStruct *IritTrimSrf2Polygons2(
					const TrimSrfStruct *Srf,
					const CagdSrf2PlsInfoStrct *TessInfo);
int IritTrimSetNumTrimVrtcsInCell(int NumTrimVrtcsInCell);
SymbCrvApproxMethodType IritTrimSetTrimCrvLinearApprox(
					    CagdRType UVTolSamples,
					    SymbCrvApproxMethodType UVMethod);
CagdRType IritTrimGetTrimCrvLinearApprox(void);

TrimSrfStruct *IritTrimSrfFromSrf(CagdSrfStruct *Srf, int SingleTCrv);
TrimSrfStruct *IritTrimSrfsFromContours(const CagdSrfStruct *Srf,
					const struct IPPolygonStruct *Cntrs);
TrimSrfStruct *IritTrimSrfsFromContours2(const CagdSrfStruct *Srf,
					 const CagdCrvStruct *CCntrs);
struct IPPolygonStruct *IritTrimValidateNewTrimCntrs(
					 const CagdSrfStruct *Srf,
					 const struct IPPolygonStruct *Cntrs);
int IritTrimLoopWeightRelationInside(CagdRType V1, CagdRType V2, CagdRType V);
CagdRType IritTrimLoopUV2Weight(const IrtRType *UV,
				IrtRType *BndryUV,
				CagdRType UMin,
				CagdRType UMax,
				CagdRType VMin,
				CagdRType VMax,
				CagdBType Last);
CagdRType *IritTrimLoopWeight2UV(CagdRType Wgt,
				 CagdRType UMin,
				 CagdRType UMax,
				 CagdRType VMin,
				 CagdRType VMax,
				 CagdUVType	UV);

TrimSrfStruct *IritTrimSrfsFromTrimPlsHierarchy(
					     struct IPPolygonStruct *TopLevel,
					     struct IPPolygonStruct *TrimPls,
					     const CagdSrfStruct *Srf);
TrimCrvStruct *IritTrimPolylines2LinTrimCrvs(
					 const struct IPPolygonStruct *Polys);

CagdBType IritTrimIsPointInsideTrimSrf(const TrimSrfStruct *TrimSrf,
				       CagdUVType UV);
CagdBType IritTrimIsPointInsideTrimCrvs(const TrimCrvStruct *TrimCrvs,
					CagdUVType UV);
int IritTrimIsPointInsideTrimUVCrvs(const CagdCrvStruct *UVCrvs,
				    CagdUVType UV);
int IritTrimIsPointInsideTrimUVCrv(const CagdCrvStruct *UVCrv, CagdUVType UV);

TrimSetErrorFuncType IritTrimSetFatalErrorFunc(TrimSetErrorFuncType ErrorFunc);
const char *IritTrimDescribeError(IritTrimFatalErrorType ErrorNum);
void IritTrimFatalError(IritTrimFatalErrorType ErrID);

#ifdef DEBUG

void IritTrimDbg(const void *Obj);
void IritTrimDbg1(const void *Obj);

void IritTrimDbgTCrvs(const TrimCrvStruct *TrimCrv);
void IritTrimDbgTCrvSegs(const TrimCrvSegStruct *TrimSegs);
int IritTrimDbgVerifyTSeg(const TrimCrvSegStruct *TrimSeg);
int IritTrimDbgVerifyUVCrv(const CagdCrvStruct *TrimCrv);

#endif /* DEBUG */

/*****************************************************************************
* Routines to handle layout (prisa) of trimmed surfaces.		     *
*****************************************************************************/
TrimSrfStruct *IritTrimAllPrisaSrfs(const TrimSrfStruct *TSrfs,
				    int	SamplesPerCurve,
				    CagdRType Epsilon,
				    CagdSrfDirType Dir,
				    CagdVType Space);
TrimSrfStruct *IritTrimPiecewiseRuledSrfApprox(const TrimSrfStruct *TSrf,
					       CagdBType ConsistentDir,
					       CagdRType Epsilon,
					       CagdSrfDirType Dir);
TrimSrfStruct *IritTrimPrisaRuledSrf(const TrimSrfStruct *TSrf,
				     int SamplesPerCurve,
				     CagdRType Space,
				     CagdVType Offset,
				     CagdSrfDirType Dir);

/*****************************************************************************
* Untrimming trimmed surfaces.						     *
*****************************************************************************/
TrimUntrimResultStruct *IritTrimUntrimTrimSrf(
					 const TrimSrfStruct *TSrf,
					 CagdQuadSrfWeightFuncType WeightFunc,
					 CagdBType Compose,
					 int ApproxOrder);
CagdBType IritTrimUntrimSetLineSweepOutputCrvPairs(CagdBType NewValue);
struct IPObjectStruct *IritTrimUntrimmingResultToObj(
				      const TrimUntrimResultStruct *Untrimmed);
void IritTrimUntrimmingResultFree(TrimUntrimResultStruct *Untrim);
void IritTrimUntrimmingResultFreeList(TrimUntrimResultStruct *Untrim);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* IRIT_TRIM_LIB_H */
