/*****************************************************************************
* Mdl_lib.h - header file for the Model	library.			     *
* This header is also the interface header to the world.		     *
******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute	of Technology		     *
******************************************************************************
* Written by Gershon Elber and Bogdanov	Alexander   July, 1996		     *
*****************************************************************************/

#ifndef IRIT_MDL_LIB_H
#define IRIT_MDL_LIB_H

#include "inc_irit/cagd_lib.h" 
#include "inc_irit/trim_lib.h" 

typedef enum {
    MDL_BOOL_UNKNOWN = 6000,
    MDL_BOOL_UNION,
    MDL_BOOL_INTERSECTION,
    MDL_BOOL_SUBTRACTION,
    MDL_BOOL_CUT,
    MDL_BOOL_INTER_CRVS,
    MDL_BOOL_DTCT_INTER
} MdlBooleanType;

typedef enum {
    MDL_ERR_NO_ERROR = 0,

    MDL_ERR_PTR_REF = 1000,
    MDL_ERR_TSEG_NO_SRF,
    MDL_ERR_BOOL_MERGE_FAIL,
    MDL_ERR_TSEG_NOT_FOUND,
    MDL_ERR_EXPECTED_LINEAR_TSEG,
    MDL_ERR_TSRF_NOT_FOUND,
    MDL_ERR_FP_ERROR,
    MDL_ERR_BOOL_DISJOINT,
    MDL_ERR_BOOL_GET_REF,
    MDL_ERR_BOOL_CLASSIFY_FAIL,
    MDL_ERR_BOOL_UVMATCH_FAIL,

    MDL_ERR_UNDEFINE_ERR
} IritMdlFatalErrorType;

struct MdlModelStruct;
struct MdlIntersectionCBStruct;
struct IritMvarSrfSrfInterCacheStruct;

typedef void *MdlIntersectionCBData;
typedef struct MvarPolylineStruct * (*MdlIntersectionCBFunc)(
 					   const CagdSrfStruct *Srf1,
					   const CagdSrfStruct *Srf2, 
					   CagdSrfStruct **ModifiedSrf1, 
					   CagdSrfStruct **ModifiedSrf2,
					   MdlIntersectionCBData InterCBData);
typedef void (*MdlPreIntersectionCBFunc)(struct MdlModelStruct *Mdl1,
				         struct MdlModelStruct *Mdl2,
				         MdlIntersectionCBData InterCBData);
typedef void (*MdlPostIntersectionCBFunc)(
				        struct MdlModelStruct *Mdl,
				        MdlIntersectionCBData InterCBData);
					
typedef struct MdlIntersectionCBStruct {
    MdlIntersectionCBFunc InterCBFunc;
    MdlPreIntersectionCBFunc PreInterCBFunc;
    MdlIntersectionCBData InterCBData;
    MdlPostIntersectionCBFunc PostInterCBFunc;
} MdlIntersectionCBStruct;

typedef struct MdlBoolTolStruct {
    CagdRType SubdivTol;
    CagdRType Numerictol;
    CagdRType TraceTol;
    CagdRType VerifyUVTol;
    CagdRType VerifyEucTol;
} MdlBoolTolStruct;

typedef struct MdlBopsParamsStruct {
    CagdBType PertubrateSecondModel;
    CagdBType ExtendUVSrfResult;
    /* If IntersectedSurfacesAttrib != NULL then each surface in both models */
    /* will have an integer attribute named IntersectedSurfacesAttrib that   */
    /* will have value of 1 if the surface participated is trimmed           */
    /* (intersects) as result of the boolean operation.			     */
    IPAttrIDType IntersectedSurfacesAttrID;
    MdlIntersectionCBStruct *SSICBData;
    MdlBoolTolStruct Tolerances;
} MdlBopsParamsStruct;

typedef struct MdlTrimSegStruct {
    struct MdlTrimSegStruct *Pnext;
    struct IPAttributeStruct *Attr;
    struct MdlTrimSrfStruct *SrfFirst;
    struct MdlTrimSrfStruct *SrfSecond;
    CagdCrvStruct *UVCrvFirst;   /* Trim crv segment in srf's param. domain. */
    CagdCrvStruct *UVCrvSecond;  /* Trim crv segment in srf's param. domain. */
    CagdCrvStruct *EucCrv;       /* Trimming curve as an E3 Euclidean curve. */
    IrtBType Tags;
} MdlTrimSegStruct;

typedef struct MdlTrimSegRefStruct {
    struct MdlTrimSegRefStruct *Pnext;
    struct IPAttributeStruct *Attr;
    MdlTrimSegStruct *TrimSeg;
    IrtBType Reversed;
    IrtBType Tags;
} MdlTrimSegRefStruct;

typedef struct MdlLoopStruct {
    struct MdlLoopStruct *Pnext;
    struct IPAttributeStruct *Attr;
    MdlTrimSegRefStruct *SegRefList;
} MdlLoopStruct;

typedef struct MdlTrimSrfStruct {
    struct MdlTrimSrfStruct *Pnext;
    struct IPAttributeStruct *Attr;
    CagdSrfStruct *Srf;                /* Surface trimmed by MdlTrimSegList. */
    MdlLoopStruct *LoopList;
} MdlTrimSrfStruct;

typedef struct MdlModelStruct {
    struct MdlModelStruct *Pnext;
    struct IPAttributeStruct *Attr;
    MdlTrimSrfStruct *TrimSrfList;
    MdlTrimSegStruct *TrimSegList;       /* List of trimming curve segments. */
} MdlModelStruct;

typedef void (*MdlSetErrorFuncType)(IritMdlFatalErrorType ErrorFunc);

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

void IritMdlTrimSegFree(MdlTrimSegStruct *MTSeg);
void IritMdlTrimSegFreeList(MdlTrimSegStruct *MTSegList);
void IritMdlTrimSegRefFree(MdlTrimSegRefStruct *MTSegRef);
void IritMdlTrimSegRefFreeList(MdlTrimSegRefStruct *MTSegRefList);
void IritMdlLoopFree(MdlLoopStruct *MdlLoop);
void IritMdlLoopFreeList(MdlLoopStruct *MdlLoopList);
void IritMdlTrimSrfFree(MdlTrimSrfStruct *TrimSrf);
void IritMdlTrimSrfFreeList(MdlTrimSrfStruct *MdlTrimSrfList);
void IritMdlModelFree(MdlModelStruct *Model);
void IritMdlModelFreeList(MdlModelStruct *Model);


#ifdef DEBUG
#define IritMdlTrimSegFree(MTSeg)         { IritMdlTrimSegFree(MTSeg); MTSeg = NULL; }
#define IritMdlTrimSegFreeList(MTSegList) { IritMdlTrimSegFreeList(MTSegList); \
					    MTSegList = NULL; }
#define IritMdlTrimSegRefFree(MTSegRef)   { IritMdlTrimSegRefFree(MTSegRef); \
					    MTSegRef = NULL; }
#define IritMdlTrimSegRefFreeList(MTSegRefList) { IritMdlTrimSegRefFreeList(MTSegRefList); \
					    MTSegRefList = NULL; }
#define IritMdlLoopFree(MdlLoop)          { IritMdlLoopFree(MdlLoop); MdlLoop = NULL; }
#define IritMdlTrimSrfFree(TrimSrf)       { IritMdlTrimSrfFree(TrimSrf); \
					    TrimSrf = NULL; }
#define IritMdlTrimSrfFreeList(MdlTrimSrfList) { IritMdlTrimSrfFreeList(MdlTrimSrfList); \
					     MdlTrimSrfList = NULL; }
#define IritMdlModelFree(Model)           { IritMdlModelFree(Model); Model = NULL; }
#define IritMdlModelFreeList(Model)       { IritMdlModelFreeList(Model); Model = NULL; }
#endif /* DEBUG */

MdlTrimSegStruct *IritMdlTrimSegCopy(const MdlTrimSegStruct *MdlTrimSeg,
				     const MdlTrimSrfStruct *TrimSrfList);
MdlTrimSegStruct *IritMdlTrimSegCopyList(const MdlTrimSegStruct *MdlTrimSegList,
					 const MdlTrimSrfStruct	*TrimSrfList);
MdlTrimSegRefStruct *IritMdlTrimSegRefCopy(
					 const MdlTrimSegRefStruct *SegRefList,
					 const MdlTrimSegStruct *TrimSegList);
MdlTrimSegRefStruct *IritMdlTrimSegRefCopyList(
					 const MdlTrimSegRefStruct *SegRefList,
					 const MdlTrimSegStruct *TrimSegList);
MdlLoopStruct *IritMdlLoopNew(MdlTrimSegRefStruct *MdlTrimSegRefList);
MdlLoopStruct *IritMdlLoopCopy(const MdlLoopStruct *MdlLoop, 
			       const MdlTrimSegStruct *TrimSegList);
MdlLoopStruct *IritMdlLoopCopyList(const MdlLoopStruct *MdlLoopList, 
				   const MdlTrimSegStruct *TrimSegList);

MdlTrimSrfStruct *IritMdlTrimSrfNew(CagdSrfStruct *Srf,
				    MdlLoopStruct *LoopList, 
				    CagdBType HasTopLvlTrim,
				    CagdBType UpdateBackTSrfPtrs);
MdlTrimSrfStruct *IritMdlTrimSrfNew2(CagdSrfStruct *Srf,
				     CagdCrvStruct **LoopList,
				     int NumLoops,
				     CagdBType HasTopLvlTrim);
MdlTrimSrfStruct *IritMdlTrimSrfCopy(const MdlTrimSrfStruct *MdlTrimSrf, 
				     const MdlTrimSegStruct *TrimSegList);
MdlTrimSrfStruct *IritMdlTrimSrfCopyList(
				       const MdlTrimSrfStruct *MdlTrimSrfList, 
				       const MdlTrimSegStruct	*TrimSegList);

MdlTrimSegStruct *IritMdlTrimSegNew(CagdCrvStruct *UVCrv1,
				    CagdCrvStruct *UVCrv2,
				    CagdCrvStruct *EucCrv1,
				    MdlTrimSrfStruct *SrfFirst,
				    MdlTrimSrfStruct *SrfSecond);
int IritMdlTrimSegRemove(const MdlTrimSegStruct *TSeg,
			 MdlTrimSegStruct **SegList);
int IritMdlTrimSegRemove2(MdlTrimSegStruct *TSeg, MdlModelStruct *Mdl);

MdlTrimSegRefStruct *IritMdlTrimSegRefNew(MdlTrimSegStruct *MdlTrimSeg);
int IritMdlTrimSegRefRemove(const MdlTrimSegStruct *TSeg,
			    MdlTrimSegRefStruct **TSegRefList,
			    int FreeRef);
int IritMdlTrimSegRefRemove2(const MdlTrimSegStruct *TSeg,
			     MdlLoopStruct *Loops,
			     int FreeRef);
MdlTrimSegRefStruct *IritMdlGetOtherSegRef(const MdlTrimSegRefStruct *SegRef,
					   const MdlTrimSrfStruct *TSrf);
MdlTrimSegRefStruct *IritMdlGetOtherSegRef2(const MdlTrimSegRefStruct *SegRef,
					    const MdlTrimSrfStruct *TSrf,
					    MdlTrimSrfStruct **OtherTSrf,
					    MdlLoopStruct **OtherLoop);
MdlTrimSegRefStruct *IritMdlGetSrfTrimSegRef(const MdlTrimSrfStruct *TSrf,
					     const MdlTrimSegStruct *TSeg);
int IritMdlGetModelTrimSegRef(const MdlModelStruct *Mdl,
			      const MdlTrimSegStruct *TSeg,
			      MdlTrimSegRefStruct **TSegRef1,
			      MdlTrimSrfStruct **TSrf1,
			      MdlTrimSegRefStruct **TSegRef2,
			      MdlTrimSrfStruct **TSrf2);
  
MdlTrimSegStruct *IritMdlTrimSrfChainTrimSegs(MdlTrimSrfStruct *TSrfs);

MdlModelStruct *IritMdlModelNew(CagdSrfStruct *Srf,
				CagdCrvStruct **LoopList,
				int NumLoops,
				CagdBType HasTopLvlTrim);
MdlModelStruct *IritMdlModelNew2(MdlTrimSrfStruct *TrimSrfs,
				 MdlTrimSegStruct *TrimSegs);
MdlModelStruct *IritMdlModelCopy(const MdlModelStruct *Model);
MdlModelStruct *IritMdlModelCopyList(const MdlModelStruct *ModelList);
void IritMdlModelTransform(MdlModelStruct *Model,
			   const CagdRType *Translate,
			   CagdRType Scale);
void IritMdlModelMatTransform(MdlModelStruct *Model, CagdMType Mat);
CagdBType IritMdlModelsSame(const MdlModelStruct *Model1,
			    const MdlModelStruct *Model2,
			    CagdRType Eps);

void IritMdlPatchTrimmingSegPointers(MdlModelStruct *Model);
IritIntPtrSizeType IritMdlGetLoopSegIndex(const MdlTrimSegRefStruct *TrimSeg,
					  const	MdlTrimSegStruct *TrimSegList);
IritIntPtrSizeType IritMdlGetSrfIndex(const MdlTrimSrfStruct *Srf,
				      const MdlTrimSrfStruct *TrimSrfList);


/*****************************************************************************
* Bounding boxes routines.						     *
*****************************************************************************/

CagdBBoxStruct *IritMdlModelBBox(const MdlModelStruct *Mdl,
				 CagdBBoxStruct *BBox);
CagdBBoxStruct *IritMdlModelListBBox(const MdlModelStruct *Mdls,
				     CagdBBoxStruct *BBox);
CagdBBoxStruct *IritMdlModelTSrfTCrvsBBox(const MdlTrimSrfStruct *TSrf,
					  CagdBBoxStruct *BBox);

/*****************************************************************************
* Primitives routines.							     *
*****************************************************************************/

int IritMdlTwoTrimSegsSameEndPts(const MdlTrimSegStruct *TSeg1,
				 const MdlTrimSegStruct *TSeg2,
				 CagdBType BackProjTest,
				 CagdRType Tol);
CagdCrvStruct *IritMdlGetTrimmingCurves(const MdlTrimSrfStruct *TrimSrf,
					CagdBType MergeLoops,
					CagdBType ParamSpace,
					CagdBType EvalEuclid);
int IritMdlStitchModel(MdlModelStruct *Mdl,
		       CagdBType BackProjTest,
		       CagdRType StitchTol);
MdlModelStruct *IritMdlPrimPlane(CagdRType MinX,
				 CagdRType MinY,
				 CagdRType MaxX,
				 CagdRType MaxY,
				 CagdRType ZLevel);
MdlModelStruct *IritMdlPrimPlaneSrfOrderLen(CagdRType MinX,
					    CagdRType MinY,
					    CagdRType MaxX,
					    CagdRType MaxY,
					    CagdRType ZLevel,
					    int	Order,
					    int	Len);
MdlModelStruct *IritMdlPrimListOfSrfs2Model(CagdSrfStruct *Srfs,
					    int	*n,
					    int	StitchModel);
MdlModelStruct *IritMdlPrimListOfSrfs2Model2(
					   const CagdSrfStruct *InSrfs, 
					   CagdCrvAdjCmpFuncType CrvCmpFuncPtr,
					   int *Is2ManifoldP,	
					   int *StitchedCrvsP);
MdlModelStruct *IritMdlPrimBox(CagdRType MinX,
			       CagdRType MinY,
			       CagdRType MinZ,
			       CagdRType MaxX,
			       CagdRType MaxY,
			       CagdRType MaxZ);
MdlModelStruct *IritMdlPrimSphere(const CagdVType Center,
				  CagdRType Radius,
				  CagdBType Rational);
MdlModelStruct *IritMdlPrimTorus(const CagdVType Center,
				 CagdRType MajorRadius,
				 CagdRType MinorRadius,
				 CagdBType Rational);
MdlModelStruct *IritMdlPrimCone2(const CagdVType Center,
				 CagdRType MajorRadius,
				 CagdRType MinorRadius,
				 CagdRType Height,
				 CagdBType Rational,
				 CagdPrimCapsType Caps);
MdlModelStruct *IritMdlPrimCone(const CagdVType Center,
				CagdRType Radius,
				CagdRType Height,
				CagdBType Rational,
				CagdPrimCapsType Caps);
MdlModelStruct *IritMdlPrimCylinder(const CagdVType Center,
				    CagdRType Radius,
				    CagdRType Height,
				    CagdBType Rational,
				    CagdPrimCapsType Caps);
int IritMdlStitchSelfSrfPrims(int Stitch);
int IritMdlCreateCubeSpherePrim(int CubeTopoSphere);

/*****************************************************************************
* Boolean operations.							     *
*****************************************************************************/

CagdCrvStruct *IritMdlExtructReversUVCrv(const MdlTrimSrfStruct *MdlSrf, 
					 const MdlTrimSegStruct *MdlSeg);

CagdRType IritMdlBooleanSetTolerances(CagdRType SubdivTol,
				      CagdRType	NumerTol,
				      CagdRType	TraceTol);
CagdBType IritMdlBooleanSetClip2Trim(CagdBType Clip2Trim);

struct IPObjectStruct *IritMdlBooleanUnion(
			      const MdlModelStruct *Model1, 
			      const MdlModelStruct *Model2,
			      struct IritMvarSrfSrfInterCacheStruct *SSICache, 
			      MdlBopsParamsStruct *BopsParams);
struct IPObjectStruct *IritMdlBooleanIntersection(
			      const MdlModelStruct *Model1,	
			      const MdlModelStruct *Model2,
			      struct IritMvarSrfSrfInterCacheStruct *SSICache,
			      MdlBopsParamsStruct *BopsParams);
struct IPObjectStruct *IritMdlBooleanSubtraction(
			      const MdlModelStruct *Model1, 
			      const MdlModelStruct *Model2,
			      struct IritMvarSrfSrfInterCacheStruct *SSICache,
			      MdlBopsParamsStruct *BopsParams);
struct IPObjectStruct *IritMdlBooleanCut(
			      const MdlModelStruct *Model1,
			      const MdlModelStruct *Model2,
			      struct IritMvarSrfSrfInterCacheStruct *SSICache,
			      MdlBopsParamsStruct *BopsParams);
struct IPObjectStruct *IritMdlBooleanMerge(const MdlModelStruct *Model1,
					   const MdlModelStruct	*Model2,
					   CagdBType StitchBndries);
MdlModelStruct *IritMdlBooleanMerge2(const MdlModelStruct *Model1,
				     const MdlModelStruct *Model2,
				     CagdBType StitchBndries);
CagdCrvStruct *IritMdlBooleanInterCrv(const MdlModelStruct *Model1,
				      const MdlModelStruct *Model2,
				      int InterType,
				      MdlModelStruct **InterModel,
				      MdlBopsParamsStruct *BopsParams);
int IritMdlBooleanDtctModelSrfInter(MdlModelStruct *Model1,
				    const CagdSrfStruct	*Srf2);
CagdBType IritMdlBooleanDetectInter(
			       const MdlModelStruct *Model1, 
			       const MdlModelStruct *Model2,
			       struct IritMvarSrfSrfInterCacheStruct *SSICache,
			       MdlBopsParamsStruct *BopsParams);
int IritMdlBoolSetOutputInterCrv(int OutputInterCurve);
int IritMdlBoolSetOutputInterCrvType(int OutputInterCurveType);
int IritMdlBoolSetHandleInterDiscont(int HandleInterDiscont);
MdlModelStruct *IritMdlModelNegate(const MdlModelStruct *Model);

int IritMdlBoolCleanRefsToTSrf(MdlModelStruct *Model, MdlTrimSrfStruct *TSrf);
void IritMdlBoolResetAllTags(MdlModelStruct *Model);
int IritMdlBoolCleanUnusedTrimCrvsSrfs(MdlModelStruct *Model);
int IritMdlBoolCleanUnusedTrimCrvsSrfs2(MdlModelStruct *Model);
void IritMdlBoolClipTSrfs2TrimDomain(MdlModelStruct *Model, 
				     CagdBType ExtendSrfDomain);

CagdCrvStruct *IritMdlInterSrfByPlane(const CagdSrfStruct *Trf,
				      const IrtPlnType Pln);
CagdCrvStruct *IritMdlInterModelByPlane(const MdlModelStruct *Mdl,
					const IrtPlnType Pln);
CagdPtStruct *IritMdlInterModelByCurve(const MdlModelStruct *Mdl,
				       const CagdCrvStruct *Crv,
				       CagdRType Eps,
				       int DtctInters);
TrimSrfStruct *IritMdlClipSrfByPlane(const CagdSrfStruct *Srf,
				     const IrtPlnType Pln);
TrimSrfStruct *IritMdlClipTrimmedSrfByPlane(const TrimSrfStruct *TSrf,
					     const IrtPlnType Pln);
MdlModelStruct *IritMdlClipModelByPlane(const MdlModelStruct *Mdl,
					const IrtPlnType Pln,
					MdlBooleanType BoolOp);

struct MdlBopsParamsStruct *IritMdlBoolOpParamsAlloc(
				    CagdBType PertubrateSecondModel,
				    CagdBType ExtendUVSrfResult,
				    IPAttrIDType IntersectedSurfacesAttrID,
				    MdlIntersectionCBFunc InterCBFunc,
				    MdlPreIntersectionCBFunc PreInterCBFunc,
				    MdlPostIntersectionCBFunc PostInterCBFunc);
void IritMdlBoolOpParamsFree(struct MdlBopsParamsStruct *BopsParams);

/*****************************************************************************
* Model	maintenance routines.						     *
*****************************************************************************/

int IritMdlSplitTrimCrv(MdlTrimSegStruct *Seg,
			const CagdPtStruct *Pts,
			int Idx,
			CagdRType Eps,
			int *Proximity);
MdlTrimSegStruct *IritMdlDivideTrimCrv(MdlTrimSegStruct *Seg,
				       const CagdPtStruct *Pts,
				       int Idx,
				       CagdRType Eps,
				       int *Proximity);
MdlTrimSegStruct *IritMdlFilterOutCrvs(MdlTrimSegStruct *TSegs);
CagdBType IritMdlIsPointInsideTrimSrf(const MdlTrimSrfStruct *TSrf,
				      CagdUVType UV);
int IritMdlIsPointInsideTrimLoop(const MdlTrimSrfStruct *TSrf,
				 const MdlLoopStruct *Loop,
				 CagdUVType	UV);
CagdBType IritMdlIsPointInsideModel(const CagdPType Pnt,
				    const MdlModelStruct *Mdl);
int IritMdlIsLoopNested(const MdlLoopStruct *L, const MdlTrimSrfStruct *TSrf);
int IritMdlGetUVLocationInLoop(const MdlLoopStruct *L,
			       const MdlTrimSrfStruct *TSrf,
			       CagdUVType UV);
void IritMdlEnsureMdlTrimCrvsPrecision(MdlModelStruct *Mdl);
void IritMdlEnsureTSrfTrimCrvsPrecision(MdlTrimSrfStruct *MdlTrimSrf);
int IritMdlEnsureTSrfTrimLoopPrecision(MdlLoopStruct *Loop,
				       MdlTrimSrfStruct *MdlTrimSrf,
				       CagdRType Tol);
int IritMdlModelIsClosed(const MdlModelStruct *Model);
CagdPType *IritMdlGetTrimmingCurvesEndPts(MdlModelStruct *Mdl, int *N);

/*****************************************************************************
* Conversion routines.							     *
*****************************************************************************/

TrimSrfStruct *IritMdlCnvrtMdl2TrimmedSrfs(const MdlModelStruct *Model,
					   CagdRType TrimCrvStitchTol);
TrimSrfStruct *IritMdlCnvrtMdls2TrimmedSrfs(const MdlModelStruct *Models,
					    CagdRType TrimCrvStitchTol);
MdlModelStruct *IritMdlCnvrtSrfs2Mdls(const CagdSrfStruct *Srfs,
				      int StitchModel);
MdlModelStruct *IritMdlCnvrtSrf2Mdl(const CagdSrfStruct *Srf, int StitchModel);
MdlModelStruct *IritMdlCnvrtTrimmedSrfs2Mdls(const TrimSrfStruct *TSrfs,
					     int MergeTCrvsIntoLoops);
MdlModelStruct *IritMdlCnvrtTrimmedSrf2Mdl(const TrimSrfStruct *TSrf,
					   int MergeTCrvsIntoLoops);
MdlModelStruct *IritMdlCnvrtTrimmedSrfs2Mdl(const TrimSrfStruct *TSrfs,
					    int	MergeTCrvsIntoLoops,
					    int	StitchMdl);
MdlModelStruct *IritMdlAddSrf2Mdl(const MdlModelStruct *Mdl,
				  const	CagdSrfStruct *Srf,
				  int StitchModel);
MdlModelStruct *IritMdlAddTrimmedSrf2Mdl(const MdlModelStruct *Mdl,
					 const TrimSrfStruct *TSrf,
					 int MergeTCrvsIntoLoops,
					 int StitchModel);
CagdCrvStruct *IritMdlExtractUVCrv(const MdlTrimSrfStruct *Srf,
				   const MdlTrimSegStruct *Seg);
void IritMdlRemovEucTrimCrvs(MdlModelStruct *Mdl);
MdlModelStruct *IritMdlSplitDisjointComponents(const MdlModelStruct *Mdl);

/*****************************************************************************
* Error	handling.							     *
*****************************************************************************/

MdlSetErrorFuncType IritMdlSetFatalErrorFunc(MdlSetErrorFuncType ErrorFunc);
void IritMdlFatalError(IritMdlFatalErrorType ErrID);
const char *IritMdlDescribeError(IritMdlFatalErrorType ErrID);

void IritMdlDbg(void *Obj);
void IritMdlDbg1(void *Obj);

#ifdef DEBUG

void IritMdlDbg2(void *Obj);
void IritMdlDbgDsp(void *Obj, CagdRType Trans, int Clear);
void IritMdlDbgDsp2(void *Obj, CagdRType Trans, int Clear);
int IritMdlDbgMC(const MdlModelStruct *Mdl, int Format);
int IritMdlDbgTC(const MdlTrimSegStruct *TSegs, 
		 const MdlTrimSrfStruct *TSrf,
		 int Format);
int IritMdlDbgSC(const MdlTrimSrfStruct *TSrf, int Format);
int IritMdlDbgRC(const MdlTrimSegRefStruct *Refs, int Format);
int IritMdlDbgRC2(const MdlTrimSegRefStruct *Refs,
		  const MdlTrimSrfStruct *TSrf,
		  int Format);

int IritMdlDebugHandleTCrvLoops(const MdlTrimSrfStruct *TSrf,
				const MdlLoopStruct *Loops,
				const CagdPType Trans,
				int Display,
				int TrimEndPts);
int IritMdlDebugHandleTSrfCrvs(const MdlTrimSegStruct *TCrvs,
			       const MdlTrimSrfStruct *TSrf,
			       const CagdPType Trans,
			       int Display,
			       int TrimEndPts);
int IritMdlDebugHandleTSrfRefCrvs(const MdlTrimSegRefStruct *Refs,
				  const MdlTrimSrfStruct *TSrf,
				  const CagdPType Trans,
				  int Loop,
				  int Display,
				  int TrimEndPts);
int IritMdlDebugWriteTrimSegs(const MdlTrimSegStruct *TSegs,
			      const MdlTrimSrfStruct *TSrf,
			      const CagdPType Trans);

int IritMdlDebugVerifyTrimSeg(const MdlTrimSegStruct *TSeg,
			      int VerifyBackPtrs);
int IritMdlDebugVerifyTrimSegEps(const MdlTrimSegStruct *TSeg,
				 int VerifyBackPtrs,
				 CagdRType TCrvTol);
int IritMdlDebugVerifyTrimSegsArcLen(const MdlTrimSegStruct *TSegs);
int IritMdlDebugVerifyTrimSrf(const MdlTrimSrfStruct *MdlTrimSrf,
			      int Complete,
			      int TestLoops);
int IritMdlDebugVerifyTrimSrfEps(const MdlTrimSrfStruct *MdlTrimSrf,
				 int Complete,
				 int TestLoops,
				 CagdRType TCrvTol,
				 CagdRType UVTol);
int IritMdlDebugVerify(const MdlModelStruct *Model,
		       int Complete,
		       int TestLoops);
int IritMdlDebugVerifyEps(const MdlModelStruct *Model,
			  int Complete,
			  int TestLoops,
			  CagdRType TCrvTol,
			  CagdRType TCrvEndPtTol,
			  CagdRType VertexTol);
struct IPObjectStruct *IritMdlDebugVisual(const MdlModelStruct *Model,
					  CagdBType TCrvs,
					  CagdBType TSrfs,
					  CagdBType Srfs);
void IritMdlDbgVsl(const MdlModelStruct *Model,
		   CagdBType TCrvs,
		   CagdBType TSrfs,
		   CagdBType Srfs);
#endif /* DEBUG */

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* IRIT_MDL_LIB_H */
