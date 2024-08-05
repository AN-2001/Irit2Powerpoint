/*****************************************************************************
* Conversion routines from curves and surfaces to polygons and polylines.    *
******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute	of Technology		     *
******************************************************************************
* Written by:  Gershon Elber				Ver 1.0, Apr 1992    *
*****************************************************************************/

#ifndef IRIT_IP_CONVRT_H
#define IRIT_IP_CONVRT_H

#include "inc_irit/iritprsr.h"
#include "inc_irit/cagd_lib.h"
#include "inc_irit/symb_lib.h"
#include "inc_irit/attribut.h"
#include "inc_irit/grap_lib.h"

#define IP_ATTR_ID_DEGEN_PL	IRIT_ATTR_CREATE_ID(IpDegenPoly)

typedef struct IPFreeformConvStateStruct {
    int	      Talkative;                      /* If TRUE, be more talkative. */
    int	      DumpObjsAsPolylines;  /* Should we dump polylines or polygons. */
    int	      DrawFFGeom;           /* Should we dump the freeform geometry. */
    int	      DrawFFMesh;         /* Should we dump control polygons/meshes. */
    int       NumOfIsolines[3];    /* isolines for surfaces and trivariates. */
    IrtRType  CrvApproxTolSamples;        /* Samples/Tolerance of PL approx. */
    SymbCrvApproxMethodType CrvApproxMethod;     /* Piecewise linear approx. */
    int       ShowInternal;       /* Do we display internal polygonal edges? */
    int	      CubicCrvsAprox;	   /* Do curves should be approx. as cubics. */
    IrtRType  FineNess;         /* Control over the polygonal approximation. */
    int	      ComputeUV;    /* Attach UV attributes to vertices of polygons. */
    int	      ComputeNrml;   /* Attach normal attrs to vertices of polygons. */
    int	      FourPerFlat;          /* Two or four triangles per flat patch. */
    int	      OptimalPolygons;           /* Optimal (or not optimal) approx. */
    int	      BBoxGrid;           /* Compute bbox/grid subdivision for bbox. */
    int	      LinearOnePolyFlag;/* Only one polygonal subdiv. along linears. */
    int	      SrfsToBicubicBzr;      /* Should we convert to bicubic Bezier? */
} IPFreeformConvStateStruct;

IRIT_GLOBAL_DATA_HEADER int IPGlblGenDegenPolys;

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

IRIT_GLOBAL_DATA_HEADER IPFreeformConvStateStruct IritPrsrFFCState;

/* Lower level functions in ip_cnvrt.c */

IPPolygonStruct *IritPrsrGenTriangle(const CagdSrf2PlsInfoStrct *TessInfo,
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
IPPolygonStruct *IritPrsrGenRectangle(const CagdSrf2PlsInfoStrct *TessInfo,
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
IPPolygonStruct *IritPrsrCagdPllns2IritPllns(CagdPolylineStruct *Polys);
CagdPolygonStruct *IritPrsrPlgns2CagdPlgns(IPPolygonStruct *Polys);
IPPolygonStruct *IritPrsrCagdPlgns2IritPlgns(CagdPolygonStruct *Polys,
					     CagdBType ComputeUV);
IPPolygonStruct *IritPrsrCurve2Polylines(const CagdCrvStruct *Crv,
					 CagdRType TolSamples,
					 SymbCrvApproxMethodType Method);
CagdCrvStruct *IritPrsrPolyline2Curve(const IPPolygonStruct *Pl, int Order);
IPPolygonStruct *IritPrsrCurve2CtlPoly(const CagdCrvStruct *Crv);
IPPolygonStruct *IritPrsrSurface2Polylines(const CagdSrfStruct *Srf,
					   int NumOfIsolines[2],
					   CagdRType TolSamples,
					   SymbCrvApproxMethodType Method);
IPPolygonStruct *IritPrsrSurface2KnotPolylines(const CagdSrfStruct *Srf,
					       CagdRType TolSamples,
					       SymbCrvApproxMethodType Method);
IPPolygonStruct *IritPrsrSurface2CtlMesh(const CagdSrfStruct *Srf);
SymbPlErrorFuncType IPSrf2OptPolysSetUserTolFunc(SymbPlErrorFuncType Func);
int IritPrsrSurface2PolygonsGenTriOnly(int OnlyTri);
int IritPrsrSurface2PolygonsGenDegenPolys(int DegenPolys);
CagdPlgErrorFuncType IritPrsrPolygonSetErrFunc(CagdPlgErrorFuncType Func);
IPPolygonStruct *IritPrsrSurface2Polygons(CagdSrfStruct *Srf,
					  const	CagdSrf2PlsInfoStrct *TessInfo);
IPPolygonStruct *IritPrsrTrimSrf2Polylines(TrimSrfStruct *TrimSrf,
					   int NumOfIsolines[2],
					   CagdRType TolSamples,
					   SymbCrvApproxMethodType Method,
					   int TrimmingCurves,
					   int IsoParamCurves);
IPPolygonStruct *IritPrsrTrimSrf2CtlMesh(TrimSrfStruct *TrimSrf);
IPPolygonStruct *IritPrsrTrimSrf2Polygons(
                                          const TrimSrfStruct *TrimSrf,
					  const	CagdSrf2PlsInfoStrct *TessInfo);
IPPolygonStruct *IritPrsrTrivar2Polygons(TrivTVStruct *Trivar,
					 const CagdSrf2PlsInfoStrct *TessInfo);
IPPolygonStruct *IritPrsrTrivar2Polylines(TrivTVStruct *Trivar,
					  int NumOfIsolines[3],
					  CagdRType TolSamples,
					  SymbCrvApproxMethodType Method,
					  IrtBType SrfsOnly);
IPPolygonStruct *IritPrsrTrivar2CtlMesh(TrivTVStruct *Trivar);
IPPolygonStruct *IritPrsrTriSrf2Polygons(TrngTriangSrfStruct *TriSrf,
					 const CagdSrf2PlsInfoStrct *TessInfo);
IPPolygonStruct *IritPrsrTriSrf2Polylines(TrngTriangSrfStruct *TriSrf,
					  int NumOfIsolines[3],
					  CagdRType TolSamples,
					  SymbCrvApproxMethodType Method);
IPPolygonStruct *IritPrsrTriSrf2CtlMesh(TrngTriangSrfStruct *TriSrf);
IrtRType IritPrsrSetCurvesToCubicBzrTol(IrtRType Tolerance);
CagdCrvStruct *IritPrsrCurvesToCubicBzrCrvs(CagdCrvStruct *Crvs,
					    IPPolygonStruct **CtlPolys,
					    CagdBType DrawCurve,
					    CagdBType DrawCtlPoly,
					    CagdRType MaxArcLen);
CagdCrvStruct *IritPrsrSurfacesToCubicBzrCrvs(CagdSrfStruct *Srfs,
					      IPPolygonStruct **CtlMeshes,
					      CagdBType	DrawSurface,
					      CagdBType	DrawMesh,
					      int NumOfIsolines[2],
					      CagdRType	MaxArcLen);
CagdCrvStruct *IritPrsrTrimSrfsToCubicBzrCrvs(TrimSrfStruct *TrimSrfs,
					      IPPolygonStruct **CtlMeshes,
					      CagdBType	DrawTrimSrf,
					      CagdBType	DrawMesh,
					      int NumOfIsolines[2],
					      CagdRType	MaxArcLen);
CagdCrvStruct *IritPrsrTrivarToCubicBzrCrvs(TrivTVStruct *Trivar,
					    IPPolygonStruct **CtlMeshes,
					    CagdBType DrawTrivar,
					    CagdBType DrawMesh,
					    int	NumOfIsolines[2],
					    CagdRType MaxArcLen);
CagdCrvStruct *IritPrsrTriSrfsToCubicBzrCrvs(TrngTriangSrfStruct *TriSrfs,
					     IPPolygonStruct **CtlMeshes,
					     CagdBType DrawSurface,
					     CagdBType DrawMesh,
					     int NumOfIsolines[3],
					     CagdRType MaxArcLen);
CagdSrfStruct *IritPrsrSurfacesToCubicBzrSrfs(CagdSrfStruct *Srfs,
					      CagdSrfStruct **NoConvertionSrfs);
void IritPrsrClosedPolysToOpen(IPPolygonStruct *Pls);
void IritPrsrOpenPolysToClosed(IPPolygonStruct *Pls);
CagdSrf2PlsInfoStrct *IritPrsrTSrf2PlysInitTessInfo(
					CagdSrf2PlsInfoStrct *TessInfo,
					int FourPerFlat,
					IrtRType RelFineNess,
					int ComputeUV,
					int ComputeNrml,
					int Optimal,
					void *EvalNrmlCache,
					CagdSrfMakeRectFuncType MakeRectangle,
					CagdSrfMakeTriFuncType MakeTriangle,
					IritCagdSrfAdapPolyGenFuncType
					AdapPolyGenFunc,
					IritCagdSrfAdapAuxDataFuncType 
					AdapAuxDataFunc);
CagdSrf2PlsInfoStrct *IritPrsrTSrf2PlysInitTessInfo2(
					       CagdSrf2PlsInfoStrct *TessInfo,
					       int FourPerFlat,
					       IrtRType RelFineNess,
					       int ComputeUV,
					       int ComputeNrml,
					       int Optimal,
					       void *EvalNrmlCache);
void IritPrsrTSrf2PlysFreeTessInfo(CagdSrf2PlsInfoStrct *TessInfo);
CagdSrfMakeTriFuncType IritPrsrTSrf2PlyAuxSetTriFunc(
					 const CagdSrf2PlsInfoStrct *TessInfo,
					 CagdSrfMakeTriFuncType	Func);
CagdSrfMakeRectFuncType IritPrsrTSrf2PlyAuxSetRectFunc(
					 const CagdSrf2PlsInfoStrct *TessInfo,
					 CagdSrfMakeRectFuncType Func);
IritCagdSrfAdapAuxDataFuncType IritPrsrTSrf2PlyAuxSetPolyGenData(
					 const CagdSrf2PlsInfoStrct *TessInfo,
					 IritCagdSrfAdapAuxDataFuncType Data);
IritCagdSrfAdapPolyGenFuncType IritPrsrTSrf2PlyAuxSetPolyGenFunc(
					const CagdSrf2PlsInfoStrct *TessInfo,
					IritCagdSrfAdapPolyGenFuncType Func);

IPVertexStruct *IPCopyAllVerticesFromPolys(const IPObjectStruct *PObj);

/* Higher level functions in ff_cnvrt.c */

IPFreeformConvStateStruct IritPrsrFreeForm2PolysSetCState(
				     const IPFreeformConvStateStruct *CState);
IPObjectStruct *IritPrsrFreeForm2Polylines(IPFreeFormStruct *FreeForms,
					   IPFreeformConvStateStruct *State);
IPObjectStruct *IritPrsrFreeForm2CubicBzr(IPFreeFormStruct *FreeForms,
					  IPFreeformConvStateStruct *State);
IPObjectStruct *IritPrsrFreeForm2Polygons(IPFreeFormStruct *FreeForms,
					  IPFreeformConvStateStruct *State);
IPObjectStruct *IritPrsrConvertFreeFormHierachy(
					     IPObjectStruct *PObj,
					     IPFreeformConvStateStruct *State,
					     int TriangleOnly,
					     int Regularize);
IPObjectStruct *IritPrsrConvertFreeForm(IPObjectStruct *PObj,
					IPFreeformConvStateStruct *State);
void IritPrsrMapObjectInPlace(IPObjectStruct *PObj,
			      IrtHmgnMatType Mat,
			      void *AuxData);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* IRIT_IP_CONVRT_H */
