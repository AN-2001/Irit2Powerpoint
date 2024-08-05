/******************************************************************************
* VMdl_lib.h - header file for the V-Model library.			     *
* This header is also the interface header to the world.		     *
*******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute	of Technology		     *
*******************************************************************************
* Written by Fady Massarwi and Gershon Elber   July, 2016		     *
******************************************************************************/

#ifndef IRIT_VOL_MDL_H
#define IRIT_VOL_MDL_H

#include "inc_irit/phashmap.h"
#include "inc_irit/cagd_lib.h"
#include "inc_irit/triv_lib.h"
#include "inc_irit/trim_lib.h"

#define VMDL_BLEND_NO_VALUE			   0
#define VMDL_SLICER_DEFAULT_PRLL_SLICE_BATCH_SIZE  50

typedef enum VMdlInterBndryType {
    VMDL_INTER_UMIN = 0,
    VMDL_INTER_UMAX,
    VMDL_INTER_VMIN,
    VMDL_INTER_VMAX,
    VMDL_INTER_WMIN,
    VMDL_INTER_WMAX,
    VMDL_INTER_INTERNAL
} VMdlInterBndryType;

typedef enum VMdlBoolOpType {
    VMDL_BOOL_OP_INTERSECTION = 0,
    VMDL_BOOL_OP_UNION,
    VMDL_BOOL_OP_SUBTRACTION
} VMdlBoolOpType;

typedef enum VMdlSliceOpType {
    VMDL_SLICE_NONE = -1,
    VMDL_SLICE_HETEROGENEOUS = 0,
    VMDL_SLICE_OUTLINE_CURVES,
    VMDL_SLICE_OUTLINE_IMAGE,
    VMDL_SLICE_COVERAGE_CURVES,
    VMDL_SLICE_COVERAGE_IMAGE,
    VMDL_SLICE_IMPLICIT,
} VMdlSliceOpType;

struct VMdlInterTrimCurveSegStruct;
struct VMdlInterTrimCurveSegRefStruct;
struct VMdlInterTrimSrfStruct;
struct VMdlInterTrimSrfRefStruct;
struct VMdlVolumeElementStruct;
struct VMdlVolumeElementRefStruct;

/* A generic reference structure, to be used in overloaded operations. */
typedef struct VMdlGenericRefStruct {
    struct VMdlGenericRefStruct *Pnext;
    struct IPAttributeStruct *Attr;
    void *ObjRef;
} VMdlGenericRefStruct;

/* Intersection point - holds information about trimmed intersecting      */
/* curves. From curves it is possible to reach the intersecting surfaces. */
typedef struct VMdlInterTrimPointStruct  {
    struct VMdlInterTrimPointStruct *Pnext;
    struct IPAttributeStruct *Attr;
    /* curves that pass through this point. */
    struct VMdlInterTrimCurveSegRefStruct *InterCurveSegRefList; 
    CagdPType E3Pnt;
} VMdlInterTrimPointStruct;

/* List of intersection points. */
typedef struct VMdlInterTrimPointRefStruct  {
    struct VMdlInterTrimPointRefStruct *Pnext;
    struct IPAttributeStruct *Attr;
    VMdlInterTrimPointStruct *Obj;
} VMdlInterTrimPointRefStruct;

typedef struct VMdlInterTrimCurveSegStruct  {
    struct VMdlInterTrimCurveSegStruct *Pnext;
    struct IPAttributeStruct *Attr;
    /* List of trimmed surfaces sharing this boundary, Typically  */
    /* two surfaces.                                              */
    struct VMdlInterTrimSrfRefStruct *IncidentSrfsRefList;

    /* The actual trimmed curve. */
    /* Euclidean curve */
    CagdCrvStruct *TrimSeg;
    /*MdlTrimSegStruct *TrimSeg; TODO - MdlTrimSegStruct or CagdCrvStruct */

    /* Start & End intersection points. If the end point is NULL, then */
    /* the curve seg. is closed.                                       */
    VMdlInterTrimPointRefStruct *StartPntRef;
    VMdlInterTrimPointRefStruct *EndPntRef;
} VMdlInterTrimCurveSegStruct;

/* Add a bit that states that the curve is a boundary or from SSI. */

/* Trimming loops in one trimmed surface, including the boundary loop. */
typedef struct VMdlInterTrimCurveSegLoopInSrfStruct {
    /* Next curve segment in loop. */
    struct VMdlInterTrimCurveSegLoopInSrfStruct *Pnext;
    struct IPAttributeStruct *Attr;
    /* The reference curve. */
    struct VMdlInterTrimCurveSegRefStruct *CrvRef;
    /* Next curve segment in loop. */
    /* struct VMdlInterTrimCurveSegStruct *PnextCrv; */
    /* Previous curve segment in loop. */
    struct VMdlInterTrimCurveSegLoopInSrfStruct *PprevCrv;    
    /* Surface holding this trimming loop. */
    struct VMdlInterTrimSrfRefStruct *IncidentSrfRef;

    /* The trimming curve in UV space of this Incident Srf. */
    CagdCrvStruct *UVCrv;
    CagdBType IsCrvInverted;
} VMdlInterTrimCurveSegLoopInSrfStruct;

/* Trimmed surface, a boundary face of volumetric element */
typedef struct VMdlInterTrimSrfStruct {
    struct VMdlInterTrimSrfStruct *Pnext;
    struct IPAttributeStruct *Attr;
    /* The volumetric element this is one of its faces. */
    struct VMdlVolumeElementRefStruct *TrimmedTVRef;  
    /* The adjacent Srf to this one in the adjacent cell, if any.  Can be   */
    /* NULL if none (on the boundary of the whole VModel).  If exists, that */
    /* adjacent surface better point to this one.			    */
    struct VMdlInterTrimSrfRefStruct *OppositeSrfRef;   
    /* List of trimming loops. */
    VMdlInterTrimCurveSegLoopInSrfStruct **BoundaryTrimmingCurveLoops;
    unsigned int NumOfBoundaryLoops;
    /* The real surface geometry. */
    CagdSrfStruct *UVWSrf;
    VMdlInterBndryType BndryType;
} VMdlInterTrimSrfStruct;

/* List of trimmed surfaces. */
typedef struct VMdlInterTrimSrfRefStruct {
    struct VMdlInterTrimSrfRefStruct *Pnext;
    struct IPAttributeStruct *Attr;
    VMdlInterTrimSrfStruct *Obj;
} VMdlInterTrimSrfRefStruct;

/* List of trivariates. */
typedef struct VMdlInterTrivTVRefStruct {
    struct VMdlInterTrivTVRefStruct *Pnext;
    struct IPAttributeStruct *Attr;
    TrivTVStruct *Obj; 
} VMdlInterTrivTVRefStruct;

typedef struct VMdlInterTrimCurveSegRefStruct {
    struct VMdlInterTrimCurveSegRefStruct *Pnext;
    struct IPAttributeStruct *Attr;
    VMdlInterTrimCurveSegStruct *Obj;
} VMdlInterTrimCurveSegRefStruct;

/* Volumetric element, encapsulates the minimal volumetric entity. */
typedef struct VMdlVolumeElementStruct {
    struct VMdlVolumeElementStruct *Pnext;
    struct IPAttributeStruct *Attr;
    /* Boundary surfaces. */
    VMdlInterTrimSrfRefStruct *BoundarySrfRefList;
    /* Associated trimmed curve segments. Maybe redundancy !? */
    VMdlInterTrimCurveSegRefStruct *TrimCurveSegRefList;
    /* All trimming curves' end points. Maybe redundancy !? */
    VMdlInterTrimPointRefStruct *TrimPointRefList;
    /* All TVs that their intersection created this element. */
    VMdlInterTrivTVRefStruct  *TVRefList;
    /* A 2D model that represents the boundaries of this element. */
    MdlModelStruct *__BoundaryModel;
} VMdlVolumeElementStruct;

typedef struct VMdlVolumeElementRefStruct {
    struct VMdlVolumeElementRefStruct *Pnext;
    struct IPAttributeStruct *Attr;
    VMdlVolumeElementStruct *Obj;
} VMdlVolumeElementRefStruct;

/* Volumetric model. */
typedef struct VMdlVModelStruct {
    struct VMdlVModelStruct *Pnext;
    struct IPAttributeStruct *Attr;
    /* List of volumetric elements in the model. */
    VMdlVolumeElementStruct *VolumeElements;
    /* All surfaces in the model in the model. */
    VMdlInterTrimSrfStruct *InterSrfList;
    /* All trimming curve segments in the model. */
    VMdlInterTrimCurveSegStruct *InterCurveSegList;  
    /* All trimming curves end points in the model. */
    VMdlInterTrimPointStruct *InterPointList;
    /* All TV's in the entire model in the model. */
    TrivTVStruct *TVList;
} VMdlVModelStruct;

typedef struct VMdlSlicerParamsStruct {
    CagdRType ZRes;       /* Z size of a single pixel. */
    CagdRType XYRes[2];   /* XY size of a single pixel. */
    CagdRType Max[2];     /* XY bounding box of domain to slice. */
    CagdRType Min[2];
    TrivImagePixelType PixelType;
    int AntiAliasing;	  /* Anti-aliasing factor. */  
} VMdlSlicerParamsStruct;

/* Structures that govern tolerances and constants: */
typedef struct VMdlSubdivParamsStruct {
    CagdRType UVOpTol;
    CagdRType IsoSrfExtendAmount;
    int IsoCrvNumSamples;
    CagdRType SrfRegionMargin;
    CagdRType SrfSubdivTol;
    CagdRType SrfNumericTol;
    CagdRType BspSrfFitSampleRatio;
    CagdRType TCrvSimplifyTol;
    CagdRType DistSubdivTol;
    CagdRType DistNumericTol;
    int MaxErrSamples;
} VMdlSubdivParamsStruct;

typedef struct VMdlBoolParamsStruct {
    CagdRType SpeedTol;
    CagdRType SubdivTol;
    CagdRType NumericTol;
    CagdRType TraceTol;
    CagdRType BRepVerifyE3Tol;
    CagdRType SrfDiffEps;
    CagdBType TCrvLinAprx;
    CagdRType TCrvLinAprxTol;
    int SpeedTestNumSamples;
    CagdRType GeomCompEps;
    struct IPObjectStruct *LeftObj;
    struct IPObjectStruct *RightObj;
} VMdlBoolParamsStruct;

typedef struct VMdlSweepParamsStruct {
    CagdRType WTol;
} VMdlSweepParamsStruct;

typedef struct VMdlFilletParamsStruct {
    CagdRType FilletNumericTol;
} VMdlFilletParamsStruct;

typedef struct VMdlParamsStruct {
    CagdRType GeneralTol;
    VMdlSubdivParamsStruct SubdivParams;
    VMdlBoolParamsStruct BoolParams;
    VMdlSlicerParamsStruct SlicerParams;
    VMdlSweepParamsStruct SweepParams;
    VMdlFilletParamsStruct FilletParams;
} VMdlParamsStruct;

typedef void (*IritVMdlVModelBoolOpCBFuncType)(const VMdlParamsStruct *Params,
					   const VMdlVModelStruct *VMdl1,
					   const VMdlVModelStruct *VMdl2,
					   VMdlVModelStruct *ResVMdl,
					   VMdlBoolOpType BoolOp,
					   void *AuxData);

/* V-model blending of fields structures/typedefs. */

/* Properties */

typedef struct VMdlSlicerPropertyStruct {
    CagdPointType PType;	      /* Dimension of the property function. */
    CagdRType Property[CAGD_MAX_PT_SIZE];            /* Array of properties. */
} VMdlSlicerPropertyStruct;

typedef struct VMdlBlendFilletStruct {
    struct VMdlBlendFilletStruct *PNext;
    TrivTVStruct *FilletTV;                            /* Fillet trivariate. */
    struct IritTrivInverseQueryStruct *TVQueryStruct;   /* Handle for inversion. */
    int NumSols;					/* For TV inversion. */
    CagdRType UVWParams[3];				/* For TV inversion. */
    CagdSrfStruct *BndrSrf1;                      /* First boundary surface. */
    CagdSrfStruct *BndrSrf2;                     /* Second boundary surface. */
    void *SrfPointPrep1;                        /* Prep handle for BndrSrf1. */
    void *SrfPointPrep2;                        /* Prep handle for BndrSrf2. */
    CagdUVType ExtremeDistUV1;           /* UV coordinates of the projection */
                                                           /* over BndrSrf1. */
    CagdUVType ExtremeDistUV2;           /* UV coordinates of the projection */
                                                           /* over BndrSrf2. */
} VMdlBlendFilletStruct;

/* Topology structure of the V-model for fast access and pre-computations. */
typedef struct VMdlBlendTopologyStruct {
    struct VMdlBlendTopologyStruct *Pnext;
    TrivTVStruct *TVs;			  /* TV list associated to the VMdl. */
    unsigned int SizeTVs;			   /* Number of TVs in VMdl. */
    CagdSrfStruct **Srfs;	         /* Array of internal boundary SRFs.
					    that belong to the VMdl.         */
    unsigned int SizeSrfs;			  /* Size of previous array. */
    void **SrfPtDistPreps;		    /* Handle for PT - SRF distance. */
    size_t Index;		     /* Unique index associated to the VMdl. */
    int ObjID;
} VMdlBlendTopologyStruct;

/* Handle for computation of the blend. */
typedef struct VMdlBlendPrepStruct {
    struct VMdlBlendTopologyStruct *TopolStruct;   /* Stores V-primitives in */
						                 /* V-model. */
    VMdlBlendFilletStruct* Fillet;                  /* Additional handle for */
                                                   /* blends inside fillets. */
    struct VMdlBlendVolElemStruct *VElems;   /* Stores info about V-cells in */
						                /* V-model.  */
    CagdBBoxStruct *VMdlBBox;		     /* Bounding box of the V-model. */
    size_t NumVPrims;				   /* Number of V-primitives */
    int NumTVs;					    /* Number of trivariates */
    struct VMdlBlendPointStruct *Point;              /* Structure containing */
					        /* point-related quantities. */
    struct VMdlVModelStruct *VMdl;		   /* Corresponding V-model. */
    struct VMdlParamsStruct Params;   /* Parameters for blending operations. */
} VMdlBlendPrepStruct;

/* Structure containing useful quantities related to V-cells for blending and
encoding and avoiding multiple dynamic allocations */
typedef struct VMdlBlendVolElemStruct {
    struct VMdlBlendVolElemStruct *Pnext;
    VMdlVolumeElementStruct *VolumeElement;	    /* Corresponding V-cell. */
    struct IritTrivInverseQueryStruct **TVQueryStruct; /* Handles for inversion. */
						         /* of V-cell's TVs. */
    int *NumSols;					/* For TV inversion. */
    CagdRType **UVWParams;				/* For TV inversion. */
    size_t SizeTVs;				 /* Number of TVs in V-cell. */
} VMdlBlendVolElemStruct;

/* Structure containing useful quantities related to points for blending and */
/* encoding and avoiding multiple dynamic allocations.			     */
typedef struct VMdlBlendPointStruct {
    struct VMdlBlendPointStruct *Pnext;
    struct IPAttributeStruct *Attr;			 /* Point attribute. */
    CagdRType Coords[CAGD_MAX_PT_SIZE];	   /* Euclidean coords of the point. */
    CagdRType *Params[3];	  /* Parameter values (if image of some TV). */
    CagdBType *InclusionVPrims;		       /* Inclusion in V-primitives. */
    CagdBType Discretized;      /* TRUE if the point is defined by sampling  */
						          /* toward fitting. */
    int *TrivIDs;	             /* If the pt is defined by sampling of  */
				     /* a TV domain these are the TV indices */
    size_t NumberOfTrivariates;	 /* Number of TVs having the point as image. */
    CagdRType *BlendWeights;        /* Blending weights wrt internal surface */
			 	                              /* boundaries. */
    struct VMdlVolumeElementRefStruct *VolumeElement;      /* V-cell holding */
							       /* the point. */
} VMdlBlendPointStruct;

/* Structure storing the property values for a grid of points. */
typedef struct VMdlSlicerOutputImageStruct {
    void *Points;
    int Size[2];
    TrivImagePixelType PixelType;
} VMdlSlicerOutputImageStruct;

typedef struct VMdlVoxelVModelStruct {
    struct VMdlVoxelVModelStruct *Pnext;
    struct IPAttributeStruct *Attr;	
    void **Images;		    /* The different Z levels in the volume. */
    int Size[3];
    int Origin[3];					   /* Object origin. */			    
    int ImageSize;  /* Size of one image: Size[0] * Size[1] * VoxelByteSize. */
    GMBBBboxStruct BBox;		      /* Bounding box of the volume. */
    TrivImagePixelType VoxelType;
} VMdlVoxelVModelStruct;

typedef void (*VMdlBlendCoordinatesFuncType)(VMdlBlendPrepStruct
                                                               *BlendCoordPrep,
				             const CagdRType *Pt);
typedef CagdPointType (*VMdlBlendPropertyFuncType)(
				struct VMdlBlendPrepStruct *BlendCoordPrep,
				const int ObjIndex,
				void *Output);
typedef void *(*VMdlBlendRuleFuncType)(
				VMdlBlendPrepStruct *BlendCoordPrep,
				const IrtRType Pt[4],
				void *OutputPtr,
				VMdlBlendCoordinatesFuncType Coordinates,
				VMdlBlendPropertyFuncType PropertyFunction);
typedef size_t(*VMdlEncVectorFieldFuncType)(VMdlBlendPrepStruct
                                                               *BlendCoordPrep,
				            VMdlBlendPointStruct *Pt,
				            VMdlSlicerPropertyStruct *Value);

typedef TrimSrfStruct *(*VMdlTSrfs2StitchedTSrfsFuncType)(TrimSrfStruct *,
							  CagdRType,
							  void *);

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

VMdlVModelStruct *IritVMdlVModelCopy(const VMdlParamsStruct *Params,
				     const VMdlVModelStruct *VMdl);
VMdlVModelStruct *IritVMdlVModelCopyList(const VMdlParamsStruct *Params,
					 const VMdlVModelStruct	*Mdls);

void IritVMdlVModelFree(VMdlVModelStruct *Mdl);
void IritVMdlVModelFreeList(VMdlVModelStruct *VMdls);

VMdlVModelStruct *VMdlGlueVModels(VMdlVModelStruct *Mdl1,
				  VMdlVModelStruct *Mdl2);
CagdBType VMdlGlueVModelsAppend(VMdlVModelStruct **Mdl1, 
				const VMdlVModelStruct *Mdl2, 
				CagdRType SrfDiffEps,
				CagdBType CalculateConnectivity,
				IritMiscPHashMap TVHMap);
void VMdlCalcMdlEuclCrvs(const VMdlParamsStruct *Params,
			 MdlModelStruct	*BMdl);
void VMdlEvalEuclidCrvsForTrimCrvs(TrimSrfStruct* TrSrf);
void IritVMdlDefaultParams(VMdlParamsStruct *Params);

/******************************* Debugging operations ***********************/

#ifdef DEBUG
void IritVMdlDbg(void *Obj);
void IritVMdlDbg1(void *Obj);
#endif /* DEBUG */

void IritVMdlPrintVModel(VMdlVModelStruct *VM);
void IritVMdlPrintVE(VMdlVModelStruct *VM);
void IritVMdlSplitVModelInDir(const VMdlParamsStruct *Params,
			      VMdlVModelStruct *VM,
			      CagdRType Dx,
			      CagdRType Dy,
			      CagdRType Dz);

/********************************** Alloc and free **************************/

VMdlVModelStruct *IritVMdlAllocVModel();
VMdlVolumeElementStruct *IritVMdlAllocTrimVolumeElem();
VMdlVolumeElementRefStruct *IritVMdlAllocTrimVolumeElemRef();
VMdlInterTrimPointStruct *IritVMdlAllocTrimInterPoint();
VMdlInterTrimSrfStruct *IritVMdlAllocInterTrimSrf();
VMdlInterTrimCurveSegStruct *IritVMdlAllocInterTrimCurveSeg();
VMdlInterTrimCurveSegRefStruct *IritVMdlAllocInterTrimCurveSegRef();
VMdlInterTrivTVRefStruct *IritVMdlAllocInterTrivTVRef();
VMdlInterTrimSrfRefStruct *IritVMdlAllocInterTrimSrfRef();
VMdlInterTrimCurveSegLoopInSrfStruct *IritVMdlAllocInterTrimCurveSegLoopInSrf();
VMdlInterTrimPointRefStruct *IritVMdlAllocInterTrimPointRef();

VMdlVModelStruct *IritVMdlVModelFromVElement(
					 const VMdlVolumeElementStruct *VElem, 
					 CagdBType UseVElemInPlace);

void IritVMdlFreeTrimVolElem(VMdlVolumeElementStruct *TrimVolElem);
void IritVMdlFreeTrimVolumeElemRef(VMdlVolumeElementRefStruct *TrimVolElemRef);
void IritVMdlFreeInterTrimPnt(VMdlInterTrimPointStruct *IntJunctionList);
void IritVMdlFreeInterTrimSrf(VMdlInterTrimSrfStruct *TrimSrfList);
void IritVMdlFreeInterTrimCurveSeg(VMdlInterTrimCurveSegStruct *CurveSegList);
void IritVMdlFreeInterTrimCurveSegRef(
			            VMdlInterTrimCurveSegRefStruct *CrvSegRef);
void IritVMdlFreeInterTrivTVRef(VMdlInterTrivTVRefStruct *TrivTVRef);
void IritVMdlFreeInterTrimSrfRef(VMdlInterTrimSrfRefStruct *SrfRef);
void IritVMdlFreeInterTrimCurveSegLoopInSrf(
			     VMdlInterTrimCurveSegLoopInSrfStruct *CrvSegLoop);
void IritVMdlFreeInterTrimPointRef(VMdlInterTrimPointRefStruct *Ref);

/********************************** queries *********************************/

CagdBBoxStruct *IritVMdlVModelBBox(const VMdlParamsStruct *Params,
				   const VMdlVModelStruct *VMdl,
				   CagdBBoxStruct *BBox,
				   int OnlyGeom);
CagdBBoxStruct *IritVMdlVModelListBBox(const VMdlParamsStruct *Params,
				       const VMdlVModelStruct *Mdls,
				       CagdBBoxStruct *CagdBbox,
				       int OnlyGeom);

MdlModelStruct *IritVMdlGetBoundaryVModel(const VMdlParamsStruct *Params,
					  const VMdlVModelStruct *Mdl);
struct IPObjectStruct *IritVMdlGetBoundarySurfaces2(
						const VMdlParamsStruct *Params,
						const VMdlVModelStruct *Mdl);
MdlModelStruct *IritVMdlGetOuterBoundarySurfacesVModel(
					       const VMdlParamsStruct *Params,
					       const VMdlVModelStruct *VMdl);
MdlModelStruct *IritVMdlGetBndryVElement(const VMdlParamsStruct *Params,
					 VMdlVolumeElementStruct *VCell);
CagdCrvStruct *IritVMdlGetBoundaryCurves(const VMdlVModelStruct *Mdl);
TrivTVStruct *IritVMdlIsNonTrimmedVModel(const VMdlParamsStruct *Params,
					 const VMdlVModelStruct	*VMdl);
CagdBType IritVMdlIsTVBoundaryVSrf(const VMdlInterTrimSrfStruct *VSrf, 
				   const TrivTVStruct *TV, 
				   TrivTVDirType *IsoDir, 
				   CagdRType *IsoVal);
const TrivTVStruct *IritVMdlGetTVBoundarySrf(const VMdlVModelStruct *VMdl,
					     const CagdSrfStruct *Srf,
					     TrivTVBndryType *TVBndry,
					     int *Reversed);
CagdBType IritVMdlVModelsSame(const VMdlParamsStruct *Params,
			      const VMdlVModelStruct *VMdl1,
			      const VMdlVModelStruct *VMdl2);

/************************** primitives **************************************/

VMdlVModelStruct *IritVMdlVolElementFromBoundaryModel(
				 const VMdlParamsStruct *Params,
				 const MdlModelStruct *InBMdl, 
				 VMdlInterTrivTVRefStruct *ElementTVsRefList);
VMdlVModelStruct *IritVMdlFromBoundaryModel(const VMdlParamsStruct *Params,
					    const MdlModelStruct *BMdl,
					    const TrivTVStruct *TV);

VMdlVModelStruct *IritVMdlPrimBoxVMdl(CagdRType MinX,
				      CagdRType MinY,
				      CagdRType MinZ,
				      CagdRType MaxX,
				      CagdRType MaxY,
				      CagdRType MaxZ);
VMdlVModelStruct *IritVMdlPrimBoxVMdl2(const VMdlParamsStruct *Params,
				       CagdRType MinX,
				       CagdRType MinY,
				       CagdRType MinZ,
				       CagdRType MaxX,
				       CagdRType MaxY,
				       CagdRType MaxZ);
VMdlVModelStruct *IritVMdlPrimCubeSphereVMdl(const CagdVType Center,
					     CagdRType Radius,
					     CagdBType Rational,
					     CagdRType InternalCubeEdge);
VMdlVModelStruct *IritVMdlPrimCubeSphereVMdl2(const VMdlParamsStruct *Params,
					      const CagdVType Center,
					      CagdRType	Radius,
					      CagdBType	Rational,
					      CagdRType	InternalCubeEdge);
VMdlVModelStruct *IritVMdlPrimTorusVMdl(const CagdVType Center,
					CagdRType MajorRadius,
					CagdRType MinorRadius,
					CagdBType Rational,
					CagdRType InternalCubeEdge);
VMdlVModelStruct *IritVMdlPrimTorusVMdl2(const VMdlParamsStruct *Params,
					 const CagdVType Center,
					 CagdRType MajorRadius,
					 CagdRType MinorRadius,
					 CagdBType Rational,
					 CagdRType InternalCubeEdge);
VMdlVModelStruct *IritVMdlPrimConeVMdl(const CagdVType Center,
				       CagdRType Radius,
				       CagdRType Height,
				       CagdBType Rational,
				       CagdRType InternalCubeEdge);
VMdlVModelStruct *IritVMdlPrimConeVMdl2(const VMdlParamsStruct *Params,
					const CagdVType Center,
					CagdRType Radius,
					CagdRType Height,
					CagdBType Rational,
					CagdRType InternalCubeEdge);
VMdlVModelStruct *IritVMdlPrimCone2VMdl(const CagdVType Center,
					CagdRType MajorRadius,
					CagdRType MinorRadius,
					CagdRType Height,
					CagdBType Rational,
					CagdRType InternalCubeEdge);
VMdlVModelStruct *IritVMdlPrimCone2VMdl2(const VMdlParamsStruct *Params,
					 const CagdVType Center,
					 CagdRType MajorRadius,
					 CagdRType MinorRadius,
					 CagdRType Height,
					 CagdBType Rational,
					 CagdRType InternalCubeEdge);
VMdlVModelStruct *IritVMdlPrimCylinderVMdl(const CagdVType Center,
					   CagdRType Radius,
					   CagdRType Height,
					   CagdBType Rational,
					   CagdRType InternalCubeEdge);
VMdlVModelStruct *IritVMdlPrimCylinderVMdl2(const VMdlParamsStruct *Params,
					    const CagdVType Center,
					    CagdRType Radius,
					    CagdRType Height,
					    CagdBType Rational,
					    CagdRType InternalCubeEdge);
VMdlVModelStruct *IritVMdlRuledTrimSrf(const TrimSrfStruct *TSrf1,
				       const CagdSrfStruct *Srf2,
				       int OtherOrder,
				       int OtherLen);
VMdlVModelStruct *IritVMdlRuledTrimSrfExtra(const VMdlParamsStruct *Params,
					    const TrimSrfStruct	*TSrf1,
					    const CagdSrfStruct	*Srf2,
					    int	OtherOrder,
					    int	OtherLen,
					    void *ExtraData);
VMdlVModelStruct *IritVMdlExtrudeTrimSrf(const TrimSrfStruct *Section,
					 CagdVecStruct *Dir);
VMdlVModelStruct *IritVMdlExtrudeTrimSrfExtra(const VMdlParamsStruct *Params,
					      const TrimSrfStruct *Section,
					      CagdVecStruct *Dir,
					      void *ExtraData);
VMdlVModelStruct *IritVMdlOfRevTrimSrf(const TrimSrfStruct *Section,
				       CagdRType StartAngle,
				       CagdRType EndAngle,
				       CagdBType PolyApprox);
VMdlVModelStruct *IritVMdlOfRevTrimSrfExtra(const VMdlParamsStruct* Params,
					    const TrimSrfStruct	*Section,
					    CagdRType StartAngle,
					    CagdRType EndAngle,
					    CagdBType Rational,
					    void *ExtraData);
VMdlVModelStruct *IritVMdlOfRevAxisTrimSrf(const TrimSrfStruct *Section,
					   const TrivV4DType AxisPoint,
					   const TrivV4DType AxisVector,
					   CagdRType StartAngle,
					   CagdRType EndAngle,
					   CagdBType PolyApprox);
VMdlVModelStruct *IritVMdlOfRevAxisTrimSrfExtra(const VMdlParamsStruct *Params,
						const TrimSrfStruct *Section,
						const TrivV4DType AxisPoint,
						const TrivV4DType AxisVector,
						CagdRType StartAngle,
						CagdRType EndAngle,
						CagdBType Rational,
						void *ExtraData);
VMdlVModelStruct *IritVMdlSweepTrimSrf(const TrimSrfStruct *TSrf,
				       const CagdCrvStruct *Axis,
				       const CagdCrvStruct *ScalingCrv,
				       CagdRType Scale,
				       const VoidPtr Frame,
				       int FrameOption);
VMdlVModelStruct *IritVMdlSweepTrimSrfExtra(const VMdlParamsStruct *Params,
					    const TrimSrfStruct	*TSrf,
					    const CagdCrvStruct	*Axis,
					    const CagdCrvStruct	*ScalingCrv,
					    CagdRType Scale,
					    const VoidPtr Frame,
					    int	FrameOption,
					    void* ExtraData);

/***************************  operations ************************************/

CagdBType IritVMdlTrimVModelBySurface(const VMdlVModelStruct *VMdl,
				      const CagdSrfStruct *Srf);
CagdBType IritVMdlRemoveTrimmingSurface(const VMdlVModelStruct *VMdl,
					const CagdSrfStruct	*Srf);
void IritVMdlVModelTransform(const VMdlParamsStruct *Params,
			     VMdlVModelStruct *VMdl,
			     IrtHmgnMatType	Mat);
TrivTVStruct *IritVMdlFetchTrivar(const VMdlVolumeElementStruct *VMTrimmedTV);
TrimSrfStruct *IritVMdlFetchTrimmingSrfs(
				  const VMdlVolumeElementStruct *VMTrimmedTV);
CagdBType IritVMdlVModelReplaceTV(VMdlVModelStruct *VMdl, 
				  TrivTVStruct *OldTV,
				  const	TrivTVStruct *NewTV);

/*************************** Boolean operations *****************************/

VMdlVModelStruct *IritVMdlVModelBoolOp(const VMdlParamsStruct *Params,
				       const VMdlVModelStruct *VMdlA,
				       const VMdlVModelStruct *VMdlB,
				       VMdlBoolOpType OpType);
VMdlVModelStruct *IritVMdlVModelIntersect(const VMdlParamsStruct *Params,
					  const	VMdlVModelStruct *VMdlA,
					  const	VMdlVModelStruct *VMdlB);
VMdlVModelStruct *IritVMdlVModelUnion(const VMdlParamsStruct *Params,
				      const VMdlVModelStruct *VMdlA,
				      const VMdlVModelStruct *VMdlB);
VMdlVModelStruct *IritVMdlVModelSubtract(const VMdlParamsStruct *Params,
					 const VMdlVModelStruct	*VMdlA,
					 const VMdlVModelStruct	*VMdlB);
VMdlVModelStruct *IritVMdlVModelSymDiff(const VMdlParamsStruct *Params,
					const VMdlVModelStruct *VMdlA,
					const VMdlVModelStruct *VMdlB);
VMdlVModelStruct *IritVMdlVModelNegate(const VMdlParamsStruct *Params,
				       const VMdlVModelStruct *VMdl);
VMdlVModelStruct *IritVMdlClipVModelByPlane(const VMdlParamsStruct *Params,
					    const VMdlVModelStruct *Mdl,
					    const IrtPlnType Pln,
					    VMdlBoolOpType BoolOp);
IritVMdlVModelBoolOpCBFuncType IritVMdlSetBoolOpCBFunc(
					IritVMdlVModelBoolOpCBFuncType CBFunc);

/************************** Subdivision *************************************/

VMdlVModelStruct *IritVMdlSubdivideVElement(
					const VMdlParamsStruct *Params,
					VMdlVolumeElementStruct	*VElem,
					const TrivTVStruct *TV,
					TrivTVDirType Dir,
					IrtRType t,
					IrtRType *OtherParamAttribVals,
					CagdBType HandleKnotLineIntersections);
VMdlVModelStruct *IritVMdlSubdivideVModel(const VMdlParamsStruct *Params,
					  VMdlVModelStruct *VMdl,
					  TrivTVDirType	Dir,
					  IrtRType t);
VMdlVModelStruct *IritVMdlSubdivideVElemToBezierVElements(
					 const VMdlParamsStruct *Params,
					 const VMdlVolumeElementStruct *VElem,
					 const TrivTVStruct *TV);
VMdlVModelStruct *IritVMdlSubdivideVMdlToBezierVElements(
					       const VMdlParamsStruct *Params,
					       const VMdlVModelStruct *VMdl);
TrivTVStruct *IritVMdlUntrimVModel(const VMdlParamsStruct *Params,
				   const VMdlVModelStruct *VMdl, 
				   const TrivTVStruct *TV,
				   const TrivTVStruct *OriginalTV,
				   CagdBType InParamSpace,
				   int InvSrfApproxOrder,
				   CagdRType InvApproxErr);

/************************** Properties **************************************/

IPAttributeStruct *IritVMdlGetPointVMdlAttribute(const VMdlVModelStruct *VMdl,
						 const CagdPType *UVW,
						 int AttributeID);

/*************************** Conversion functions ************************** */

int IritVMdlStitchMdlModel(const VMdlParamsStruct *Params, MdlModelStruct *Mdl);
VMdlVModelStruct *IritVMdlCnvrtSrf2VMdl(const CagdSrfStruct *Srf);
VMdlVModelStruct *IritVMdlCnvrtTrimmedSrf2VMdl(const TrimSrfStruct *TSrf);
VMdlVModelStruct *IritVMdlCnvrtTrivar2VMdl(const VMdlParamsStruct *Params,
					   const TrivTVStruct *TV,
					   int PrimID);
VMdlVModelStruct *IritVMdlCnvrtTrivarList2VMdl(const VMdlParamsStruct *Params,
					       TrivTVStruct *TVList,
					       int PrimID);
TrimSrfStruct *IritVMdlCnvrtVMdls2TrimmedSrfs(const VMdlParamsStruct *Params,
					      const VMdlVModelStruct *VMdls);
TrimSrfStruct *IritVMdlCnvrtVMdl2TrimmedSrfs(const VMdlParamsStruct *Params,
					     const VMdlVModelStruct *VMdl);
MdlModelStruct *IritVMdlCnvrtVMdls2Mdls(const VMdlParamsStruct *Params,
					const VMdlVModelStruct *VMdls);
MdlModelStruct *IritVMdlCnvrtVMdl2Mdl(const VMdlParamsStruct *Params,
				      const VMdlVModelStruct *VMdl);
TrimSrfStruct *IritVMdlCnvrtVSrf2TrimmedSrf(
					  const VMdlInterTrimSrfStruct *VSrf);
void IritVMdlAddTrimSrfToVMdl(VMdlVModelStruct *VMdl,
			      const TrimSrfStruct *TSrf,
			      CagdRType Tol);
CagdBType IritVMdlSetSplitPeriodicTV(CagdBType Split);
VMdlVModelStruct *IritVMdlExtractVElements(const VMdlVModelStruct *VMdl);
CagdCrvStruct **IritVMdlExtractTrimCrvLoop(
				  const VMdlInterTrimSrfStruct *VTrimSrfStruct,
				  int *NumLoops);

/*************************** Volumetric slicing ***************************/

struct VMdlSlicerInfoStruct *IritVMdlSlicerPlanarSlicerInitInfo(
					   const struct IPObjectStruct *Model,
					   const struct IPObjectStruct *AuxMdl,
					   VMdlParamsStruct *Params,
					   TrivTVStruct **TV,
					   int VCellIdx,
					   TrivImagePixelType PixelType,
					   const CagdRType *SliceDimension);
struct MdlModelStruct *IritVMdlBuildModelFromTVBndries(
						  const TrivTVStruct *Trivar);
struct VMdlSlicerInfoStruct *IritVMdlSlicerInitVElement(
				       const VMdlParamsStruct *Params,
				       VMdlVolumeElementStruct *VolumeElement);
struct VMdlSlicerInfoStruct *IritVMdlSlicerInitVModelVElement(
				       const VMdlParamsStruct *Params,
				       struct VMdlSlicerInfoStruct *RetIn,
				       const VMdlVModelStruct *VModel,
				       VMdlVolumeElementStruct *VolumeElement);
struct VMdlSlicerInfoStruct *IritVMdlSlicerInitVModel(
					       const VMdlParamsStruct *Params,
					       const VMdlVModelStruct *VModel);
struct VMdlSlicerInfoStruct *IritVMdlSlicerInitTrivar(
					 const TrivTVStruct *Trivar, 
					 const VMdlSlicerParamsStruct *Params);
struct VMdlSlicerInfoStruct *IritVMdlSlicerInitTrivMdl(
				        const TrivTVStruct *Trivar,
					const struct IPObjectStruct *BMdl,
					const VMdlSlicerParamsStruct *Params);
struct VMdlSlicerInfoStruct *IritVMdlSlicerInitTrivPoly(
					const TrivTVStruct *Trivar,
					const struct IPObjectStruct *BPls,
					const VMdlSlicerParamsStruct *Params);
struct VMdlSlicerInfoStruct *IritVMdlSlicerInitModel(
					const MdlModelStruct *BMdl,
					const VMdlSlicerParamsStruct *Params);
int IritVMdlSlicerAddGeom(const VMdlParamsStruct *Params,
			  struct VMdlSlicerInfoStruct *Info,
			  const	struct IPObjectStruct *Geom,
			  int Priority);
void IritVMdlSlicerFree(struct VMdlSlicerInfoStruct *Info);
void IritVMdlSlicerSliceAtZLevel(const VMdlParamsStruct *Params,
				 struct	VMdlSlicerInfoStruct *Info,
				 CagdRType z,
				 int BatchSize);
struct IPObjectStruct *IritVMdlSlicerSliceAtZLevelCoverage(
					   const VMdlParamsStruct *Params,
					   VMdlSliceOpType SliceOper,
					   struct VMdlSlicerInfoStruct *Info,
					   CagdRType z);
VMdlSlicerOutputImageStruct *IritVMdlSlicerSliceAtZBatch(
					    const VMdlParamsStruct *Params,
					    VMdlSliceOpType SliceOper,
					    struct VMdlSlicerInfoStruct *Info,
					    CagdRType *ZVals,
					    int BatchSz);
VMdlSlicerOutputImageStruct *IritVMdlSlicerSliceAtZBatch2(
					    const VMdlParamsStruct *Params,
					    VMdlSliceOpType SliceOper,
					    struct VMdlSlicerInfoStruct *Info,
					    CagdRType ZMin,
					    CagdRType ZMax,
					    CagdRType ZStep);
void IritVMdlSlicerSliceAtZBatchToFiles(const VMdlParamsStruct *Params,
					VMdlSliceOpType	SliceOper,
					struct VMdlSlicerInfoStruct *Info,
					CagdRType *ZVals,
					int BatchSz,
					const char *FileNameBase);
void IritVMdlSlicerSliceAtZBatchToFiles2(const VMdlParamsStruct *Params,
					 VMdlSliceOpType SliceOper,
					 struct	VMdlSlicerInfoStruct *Info,
					 CagdRType ZMin,
					 CagdRType ZMax,
					 CagdRType ZStep,
					 const char *FileNameBase);
int IritVMdlSlicerGetCurrSliceXY(struct VMdlSlicerInfoStruct *Info,
				 int x,
				 int y,
				 CagdRType *Params,
				 CagdRType *Pos,
				 const TrivTVStruct **TV);
void IritVMdlSlicerGetSliceSize(struct VMdlSlicerInfoStruct *Info, int *Size);
void IritVMdlSlicerAssignValueToDataXY(struct VMdlSlicerInfoStruct *Info,
				       int x,
				       int y,
				       CagdRType *ValueBuf);
void IritVMdlSlicerAssignValueToImgXY(struct VMdlSlicerInfoStruct *Info,
				      int x,
				      int y,
				      VMdlSlicerOutputImageStruct *Img);
void IritVMdlSlicerSaveImage(VMdlSlicerOutputImageStruct *Image,
			     const char	*Output);
void IritVMdlSlicerOutputImageFree(VMdlSlicerOutputImageStruct *Item);
void IritVMdlSlicerOutputImageFreeArr(VMdlSlicerOutputImageStruct *Items,
				      const int	NumItems);

/************************ Implicit tiling slicing ****************************/

int IritVMdlSlicerImplicitTileInitInfo(
			        struct VMdlSlicerInfoStruct *SlicerInfo,
			        const struct IPObjectStruct *ImplctMicroModel);
void IritVMdlSlicerImplicitTileInfoFree(
				      struct VMdlSlicerInfoStruct *SlicerInfo);
VMdlSlicerOutputImageStruct *IritVMdlSlicerImplicitSliceAtZLevel(
					    struct VMdlSlicerInfoStruct *Info,
					    CagdRType z);

/**************************** Voxelization module ****************************/

/* This data structure stores for each projection direction, its meta data.  */
/* The image should be a squared gray scale image and its pixels should be   */
/* in [0, 255]. The image values should be stored in a continuous unsigned   */
/* char vector pointed to by ImageData. The width/height of the image should */
/* be in OneSideImage. Finally, the view angle, in degrees, or projection    */
/* direction, is assigned to Theta (in [0, 360) range).		             */

typedef struct VMdlVxlRadonProjStruct {
    unsigned char *ImageData;
    int OneSideImage;
    double Theta;
} VMdlVxlRadonProjStruct;

/* Union to Get & Set different pixel values. */
typedef union TrivImagePixelTypeUnion {
    unsigned char CPixel;
    float FPixel;
    double DPixel;
    char CPixels[3];
    float FPixels[3];
    double DPixels[3];
    int _IPixel; /* Used internally. */
} TrivImagePixelTypeUnion;

int IritVMdlVxlGetPixelInPimage(const VMdlVoxelVModelStruct* VxlVMdl,
				int i,
				int j,
				int k,
				TrivImagePixelTypeUnion *Value);
int IritVMdlVxlGetPixelInPimage2(const VMdlVoxelVModelStruct *VxlVMdl,
				 int ImgIdx,
				 int VxlIdx,
				 TrivImagePixelTypeUnion *Value);
int IritVMdlVxlSetPixelInPimage(const VMdlVoxelVModelStruct *VxlVMdl,
				int i,
				int j,
				int k,
				TrivImagePixelTypeUnion *Value);
int IritVMdlVxlSetPixelInPimage2(const VMdlVoxelVModelStruct *VxlVMdl,
				 int ImgIdx,
				 int VxlIdx,
			     TrivImagePixelTypeUnion *Value);
int IritVMdlVxlInPixelOnBndry(const VMdlVoxelVModelStruct *VxlVMdl,
			      int i,
			      int j,
			      int k);
int IritVMdlVxlOutPixelOnBndry(const VMdlVoxelVModelStruct *VxlVMdl,
			       int i,
			       int j,
			       int k);

void IritVMdlVxlCalculateAverage(const VMdlVoxelVModelStruct* VxlVMdl,
				 int Dim,
				 int i,
				 int j,
				 int k,
			     TrivImagePixelTypeUnion *Avg);
void IritVMdlVxlFindMinNeighbor(const VMdlVoxelVModelStruct *VxlVMdl,
				int i,
				int j,
				int k,
			    TrivImagePixelTypeUnion *Min);
void IritVMdlVxlFindMaxNeighbor(const VMdlVoxelVModelStruct *VxlVMdl,
				int i,
				int j,
				int k,
			    TrivImagePixelTypeUnion *Max);
CagdRType IritVMdlVxlGetRealVoxelVolume(const VMdlVoxelVModelStruct *VxlVMdl);

void IritVMdlVxlEdge2VoxelVModel(VMdlVoxelVModelStruct *VxlVMdl,
				 const CagdRType *Pt1,
				 const CagdRType *Pt2);
VMdlVoxelVModelStruct *IritVMdlVxlCrv2VoxelVModel(
					     const struct IPObjectStruct *Crv,
					     CagdRType Tol,
					     const int *Sizes,
					     TrivImagePixelType PixelType);
VMdlVoxelVModelStruct *IritVMdlVxlMdl2VoxelVModel(
					     const struct IPObjectStruct *Mdl,
					     VMdlSliceOpType SliceOper,
					     const CagdRType *Dims,
					     TrivImagePixelType PixelType,
					     int AntiAliasing);
VMdlVoxelVModelStruct *IritVMdlVxlMdl2VoxelVModel2(
					     const struct IPObjectStruct *Mdl,
					     VMdlSliceOpType SliceOper,
					     const CagdRType *Dims,
					     TrivImagePixelType PixelType,
					     int AntiAliasing);
struct IPObjectStruct *IritVMdlVxlMrchCubeVxlVMdl(
					const VMdlVoxelVModelStruct *VxlVMdl,
					CagdRType IsoLevel,
					CagdRType ExtractBndry);
CagdRType IritVMdlVxlAreaVxlVMdl(const VMdlVoxelVModelStruct *VxlVMdl,
				 CagdRType IsoLevel,
				 CagdRType ExtractBndry);
CagdRType IritVMdlVxlVolumeVxlVMdl(const VMdlVoxelVModelStruct *VxlVMdl);
VMdlVoxelVModelStruct *IritVMdlVxlVoxelOffsetBW(
				         const VMdlVoxelVModelStruct *VxlVMdl,
					 int NumPixels);
VMdlVoxelVModelStruct *IritVMdlVxlVoxelOffsetGray(
					  const	VMdlVoxelVModelStruct *VxlVMdl,
					  int NumPixels);
VMdlVoxelVModelStruct *IritVMdlVxlCopyVoxelVModel(
					 const VMdlVoxelVModelStruct *VxlVMdl);
void IritVMdlVxlFreeVoxelVModel(VMdlVoxelVModelStruct *VxlVMdl);
VMdlVoxelVModelStruct *VMdlVxlAllocVoxelVModel(const int *Sizes, 
					       TrivImagePixelType VoxelType);
void IritVMdlVxlPrintVoxelVModel(VMdlVoxelVModelStruct *VxlVMdl,
				 const char *VxlFormat,
				 FILE *f,
				 int Row);
void IritVMdlVxlSaveVoxelVModel(VMdlVoxelVModelStruct *VxlVMdl,
				const char *FileName,
				IrtRType Scale,
				int Row);
VMdlVoxelVModelStruct *IritVMdlVxlHeuristicRadon(
				       VMdlVxlRadonProjStruct *ProjectionList,
				       int ProjCount);

/**************************** Blending attributes ****************************/

void IritVMdlBlendComputeDistCoordinates(VMdlBlendPrepStruct *BlendCoordPrep,
					 const CagdRType Pt[3]);
VMdlSlicerOutputImageStruct *IritVMdlBlendPropertiesSlice(
				  VMdlBlendPrepStruct *BlendCoordPrep,
				  const	double z,
				  int Resolution[2],
				  VMdlBlendCoordinatesFuncType Coordinates,
				  VMdlBlendPropertyFuncType PropertyFunction,
				  VMdlBlendRuleFuncType	BlendingRule,
				  unsigned int SizePixelProperty);
VMdlSlicerOutputImageStruct *IritVMdlBlendPropertiesSliceOneBatch(
				  VMdlBlendPrepStruct *BlendCoordPrep,
				  int BatchSz,
				  const	double *ZVals,
				  const	int Resolution[2],
				  VMdlBlendCoordinatesFuncType Coordinates,
				  VMdlBlendPropertyFuncType PropertyFunction,
				  VMdlBlendRuleFuncType	BlendingRule,
				  unsigned int SizePixelProperty);
void *IritVMdlBlendPropertiesPerPoint(
				  VMdlBlendPrepStruct *BlendCoordPrep,
				  const IrtRType Pt[4],
				  void *OutputPtr,
				  VMdlBlendCoordinatesFuncType Coordinates,
				  VMdlBlendPropertyFuncType PropertyFunction);
VMdlBlendPrepStruct *IritVMdlBlendCoordPrep(const VMdlParamsStruct *Params,
					    VMdlVModelStruct *VMdl);
int IritVMdlBlendInIntersection(VMdlBlendPrepStruct *BlendCoordPrep,
				int *VMdlIndex);
void IritVMdlBlendCoordPrepFree(VMdlBlendPrepStruct *Item);
void IritVMdlBlendBoolTrivariates(VMdlBlendPrepStruct *BlendCoordPrep,
				  const	IrtRType Pt[3],
				  CagdBType UseRayCasting);
void IritVMdlBlendPointListFree(VMdlBlendPointStruct *Item);
VMdlBlendPointStruct *VMdlBlendPointStructInit(const int SizeTopologyStruct,
					       const int NumTVs);
void IritVMdlBlendPointFree(VMdlBlendPointStruct *Item);
CagdPointType IritVMdlBlendDfltRGBPropertyFunction(VMdlBlendPrepStruct
						   *BlendCoordPrep,
						   const int ObjIndex,
						   void	*Output);
void IritVMdlBlendBoolTrivariatesVElem(VMdlBlendPrepStruct *BlendCoordPrep,
				       IrtRType	Pt[3],
				       VMdlBlendVolElemStruct *VElem);
CagdPointType IritVMdlBlendDflt3DPropertyFunction(VMdlBlendPrepStruct
						  *BlendCoordPrep,
						  const	int ObjIndex,
						  void *Output);
CagdPointType IritVMdlBlendDflt1DPropertyFunction(VMdlBlendPrepStruct
						  *BlendCoordPrep,
						  const	int ObjIndex,
						  void *Output);
#ifdef VMDL_BLEND_DISCONT
void IritVMdlBlendVElemComputeDistCoordinates(
					   VMdlBlendPrepStruct *BlendCoordPrep,
					   const CagdRType Pt[3]);
VMdlSlicerOutputImageStruct *IritVMdlBlendVCellPropertiesSlice(
					   VMdlBlendPrepStruct *BlendCoordPrep,
					   const double	z,
					   const int *Resolution);
VMdlSlicerPropertyStruct IritVMdlBlendVCellPropertiesPerPoint(
					   VMdlBlendPrepStruct *BlendCoordPrep,
					   IrtRType Pt[3]);
#endif /* VMDL_BLEND_DISCONT */

/*************************** Filleting attributes ****************************/

void IritVMdlBlendFilletComputeCoordinates(VMdlBlendPrepStruct *BlendCoordPrep,
					   const CagdRType *Pt);
void *IritVMdlBlendFilletPropertiesPerPoint(
				   VMdlBlendPrepStruct *BlendCoordPrep,
				   const IrtRType Pt[3],
				   void *OutputPtr,
				   VMdlBlendCoordinatesFuncType Coordinates,
				   VMdlBlendPropertyFuncType PropertyFunction);
VMdlSlicerOutputImageStruct *IritVMdlBlendFilletPropertiesSlice(
				   const VMdlParamsStruct *Params,
				   VMdlBlendPrepStruct	*BlendCoordPrep,
				   const double z,
				   int	Resolution[2],
				   VMdlBlendCoordinatesFuncType Coordinates,
				   VMdlBlendPropertyFuncType PropertyFunction,
				   VMdlBlendRuleFuncType BlendingRule,
				   unsigned int SizePixelProperty);
CagdPointType IritVMdlBlendFilletDflt1DPropertyFunction(
				           VMdlBlendPrepStruct *BlendCoordPrep,
					   const int ObjIndex,
					   void *Output);
CagdPointType IritVMdlBlendFilletDflt3DPropertyFunction(
					   VMdlBlendPrepStruct *BlendCoordPrep,
					   const int ObjIndex,
					   void *Output);
CagdPointType IritVMdlBlendFilletDfltRGBPropertyFunction(
					   VMdlBlendPrepStruct *BlendCoordPrep,
					   const int ObjIndex,
					   void *Output);
VMdlBlendPrepStruct *IritVMdlBlendFilletCoordPrep(
					      const VMdlParamsStruct *Params,
					      VMdlVModelStruct *VMdl,
					      TrivTVStruct *FilletTV,
					      CagdSrfStruct *BndrSrf1,
					      CagdSrfStruct *BndrSrf2);
void IritVMdlBlendFilletFree(VMdlBlendFilletStruct *Item);

void IritVMdlCreateFillet(const VMdlParamsStruct *Params,
			  const	CagdSrfStruct *Srf1List,
			  const	CagdSrfStruct *Srf2List,
			  CagdRType RailDist,
			  int R1Orient,
			  int R2Orient,
			  CagdRType TanScale,
			  int ApproxCrvsCtlPts,
			  TrivFilletingMethodType FilletingMethod,
			  CagdBType PreciseFillet,
			  TrivTVStruct **FilletTVs,
			  MdlModelStruct **FilletMdls,
			  CagdSrfStruct	**SubSrfs1,
			  CagdSrfStruct	**SubSrfs2);
VMdlVModelStruct *IritVMdlCreateFillet2(const VMdlParamsStruct *Params,
					const CagdSrfStruct *Srf1List,
					const CagdSrfStruct *Srf2List,
					CagdRType RailDist,
					int R1Orient,
					int R2Orient,
					CagdRType TanScale,
					int ApproxCrvsCtlPts,
					TrivFilletingMethodType	FilletingMethod,
					CagdBType PreciseFillet);

/**************************** Encoding attributes ****************************/

TrivTVStruct *IritVMdlEncRetrieveProperties(
				    VMdlBlendPrepStruct	*BlendCoordPrep,
				    const int NumSamplePoints,
				    CagdBType CreateNewTrivStruct,
				    const int AddedRefinedKnots,
				    VMdlEncVectorFieldFuncType F,
				    VMdlBlendCoordinatesFuncType Coordinates);
size_t IritVMdlEncDfltRGBVectorField(VMdlBlendPrepStruct *BlendCoordPrep,
				     VMdlBlendPointStruct *Pt,
				     VMdlSlicerPropertyStruct *PropertyOut);
size_t IritVMdlEncDflt3DVectorField(VMdlBlendPrepStruct *BlendCoordPrep,
				    VMdlBlendPointStruct *Pt,
				    VMdlSlicerPropertyStruct *Value);
size_t IritVMdlEncDflt1DVectorField(VMdlBlendPrepStruct *BlendCoordPrep,
				    VMdlBlendPointStruct *Pt,
				    VMdlSlicerPropertyStruct *Value);
IrtRType IritVMdlEncPropertyFunctionError(
			      const VMdlParamsStruct *Params,
			      VMdlBlendPrepStruct *BlendCoordPrep,
			      const int	Resolution[3],
			      VMdlBlendPropertyFuncType	PropertyFunction,
			      VMdlBlendPropertyFuncType	ReconPropertyFunction,
			      VMdlBlendCoordinatesFuncType Coordinates);
struct VMdlSlicerInfoStruct *IritVMdlEncInitSlicerVElementInIntersection(
				       VMdlVolumeElementStruct *VolumeElement,
				       const VMdlSlicerParamsStruct *Params,
				       const int TrivID);

#ifdef IRIT_NOT_USED
IrtRType IritVMdlEncVModelVectorFieldError(
			       VMdlBlendPrepStruct *BlendCoordPrep,
			       const int Resolution[3],
			       VMdlEncVectorFieldFuncType VectorField,
			       VMdlBlendPropertyFuncType ReconPropertyFunction,
			       VMdlBlendCoordinatesFuncType Coordinates,
			       VMdlBlendRuleFuncType BlendingRule);
#endif /* IRIT_NOT_USED */

/***************** Stitching and Boolean Operation Callbacks *****************/

VMdlTSrfs2StitchedTSrfsFuncType IritVMdlSetStitchCallback(
					 VMdlTSrfs2StitchedTSrfsFuncType Func);
VMdlTSrfs2StitchedTSrfsFuncType IritVMdlGetStitchCallback();

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* IRIT_VOL_MDL_H */
