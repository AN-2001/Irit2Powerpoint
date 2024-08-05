/*****************************************************************************
* Trng_lib.h - header file for the TRNG	library.			     *
* This header is also the interface header to the world.		     *
******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute	of Technology		     *
******************************************************************************
* Written by Gershon Elber, Aug. 96.					     *
*****************************************************************************/

#ifndef IRIT_TRNG_LIB_H
#define IRIT_TRNG_LIB_H

#include <stdio.h>
#include "inc_irit/irit_sm.h"
#include "inc_irit/miscattr.h"
#include "inc_irit/misc_lib.h"
#include "inc_irit/cagd_lib.h"
#include "inc_irit/symb_lib.h"

typedef enum {
    TRNG_ERR_DIR_NOT_VALID,
    TRNG_ERR_UNDEF_GEOM,
    TRNG_ERR_WRONG_DOMAIN,
    TRNG_ERR_WRONG_ORDER,
    TRNG_ERR_BSPLINE_NO_SUPPORT,
    TRNG_ERR_GREGORY_NO_SUPPORT,
    TRNG_ERR_RATIONAL_NO_SUPPORT,
    TRNG_ERR_RATIONAL_EXPECTED,

    TRNG_ERR_UNDEFINE_ERR
} IritTrngFatalErrorType;

typedef enum {
    TRNG_UNDEF_TYPE = 1260,
    TRNG_TRISRF_BEZIER_TYPE,
    TRNG_TRISRF_BSPLINE_TYPE,
    TRNG_TRISRF_GREGORY_TYPE
} TrngGeomType;

typedef enum {
    TRNG_NO_DIR = CAGD_NO_DIR,
    TRNG_CONST_U_DIR = CAGD_CONST_U_DIR,
    TRNG_CONST_V_DIR = CAGD_CONST_V_DIR,
    TRNG_CONST_W_DIR,
    TRNG_END_DIR
} TrngTriSrfDirType;

typedef struct TrngTriangleStruct {
    struct TrngTriangleStruct *Pnext;
    struct IPAttributeStruct *Attr;
    struct {
	CagdPType Pt;
	CagdVType Nrml;
	CagdUVType UV;
    } T[3];
} TrngTriangleStruct;

typedef struct TrngTriangSrfStruct {
    struct TrngTriangSrfStruct *Pnext;
    struct IPAttributeStruct *Attr;
    TrngGeomType GType;
    CagdPointType PType;
    int Length;		    /* Mesh size (length of edge of triangular mesh. */
    int Order;		      /* Order of triangular surface (Bspline only). */
    CagdRType *Points[CAGD_MAX_PT_SIZE];     /* Pointer on each axis vector. */
    CagdRType *KnotVector;
} TrngTriangSrfStruct;

typedef void (*TrngSetErrorFuncType)(IritTrngFatalErrorType ErrorFunc);

#define TRNG_IS_BEZIER_TRISRF(TriSrf)	((TriSrf) -> GType == TRNG_TRISRF_BEZIER_TYPE)
#define TRNG_IS_BSPLINE_TRISRF(TriSrf)	((TriSrf) -> GType == TRNG_TRISRF_BSPLINE_TYPE)
#define TRNG_IS_GREGORY_TRISRF(TriSrf)	((TriSrf) -> GType == TRNG_TRISRF_GREGORY_TYPE)

#define TRNG_IS_RATIONAL_TRISRF(TriSrf)	CAGD_IS_RATIONAL_PT((TriSrf) -> PType)

/*****************************************************************************
*		    + P00k	   Assuming a domain of	0 <= U,V,W <= 1, and *
*		   / \W	= 1	   an edge of k	control	points,	the drawing  *
*		  /   \		   on the left shows the way the mesh is     *
*	    P10k +     \	   defined.				     *
*		/	\						     *
*	       /	 \						     *
*	      /		  \	   Rows	are order in the Points	slot of	     *
*	     /		   \	   TrngTriangSrfStruct staring from Pk00     *
*	    /		    \	   along the horizontal	line to	P0k0, the    *
*	   /	    P0,k-1,1 +	   second line from Pk-1,0,k-1 to P0,k-1,1,  *
*	  /		      \		   up to the last row of size one of *
*  U = 1 /		       \  V = 1	   P00k.			     *
*  Pk00	+-----------------------+ P0k0					     *
*****************************************************************************/

#define TRNG_TRISRF_MESH_SIZE(TriSrf)  (((TriSrf) -> Length + 1) * \
					(TriSrf) -> Length / 2 + \
					(TRNG_IS_GREGORY_TRISRF(TriSrf) ? 3 : 0))
#define TRNG_LENGTH_MESH_SIZE(Length)	(((Length) + 1) * (Length) / 2)

#define TRNG_TRI_GRG_SRF_MESH_SIZE(TriSrf)  (TRNG_TRISRF_MESH_SIZE(TriSrf) + 3)

/* Index of point Pijk, where k is in fact equal to 'Length - (i+j)' into    */
/* the Points vector of TrngTriangSrfStruct.				     */
/*   Provided k > 0 equal:						     */
/* j + ((Length - (k - 1)) + Length) * (k / 2) =			     */
/* j + (2 * Length - k + 1) * (k / 2)					     */

#define TRNG_MESH_JK(TriSrf, j, k)	((j) + (2 * (TriSrf) -> Length - \
						(k) + 1) * (k) / 2)

#define TRNG_MESH_IJK(TriSrf, i, j, k)	TRNG_MESH_JK(TriSrf, j, k)	

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/*****************************************************************************
* General routines of the triangular surface library:			     *
*****************************************************************************/
CagdRType IritTrngIJChooseN(int i, int j, int N);
TrngTriangSrfStruct *IritTrngTriSrfNew(TrngGeomType GType,
				       CagdPointType PType,
				       int Length);
TrngTriangSrfStruct *IritTrngBspTriSrfNew(int Length,
					  int Order,
					  CagdPointType	PType);
TrngTriangSrfStruct *IritTrngBzrTriSrfNew(int Length, CagdPointType PType);
TrngTriangSrfStruct *IritTrngGrgTriSrfNew(int Length, CagdPointType PType);
TrngTriangSrfStruct *IritTrngTriSrfCopy(const TrngTriangSrfStruct *TriSrf);
TrngTriangSrfStruct *IritTrngTriSrfCopyList(
				       const TrngTriangSrfStruct *TriSrfList);
void IritTrngTriSrfFree(TrngTriangSrfStruct *TriSrf);
void IritTrngTriSrfFreeList(TrngTriangSrfStruct *TriSrfList);

TrngTriangSrfStruct *IritTrngCnvrtBzr2BspTriSrf(
					   const TrngTriangSrfStruct *TriSrf);
TrngTriangSrfStruct *IritTrngCnvrtGregory2BzrTriSrf(
						 TrngTriangSrfStruct *TriSrf);

void IritTrngTriSrfTransform(TrngTriangSrfStruct *TriSrf,
			     CagdRType *Translate,
			     CagdRType Scale);
TrngTriangSrfStruct *IritTrngTriSrfMatTransform(
					    const TrngTriangSrfStruct *TriSrf,
					    CagdMType Mat);

void IritTrngTriSrfMatTransform2(TrngTriangSrfStruct *TriSrf, CagdMType Mat);

TrngTriangSrfStruct *IritTrngCoerceTriSrfsTo(const TrngTriangSrfStruct *TriSrf,
					     CagdPointType PType);
TrngTriangSrfStruct *IritTrngCoerceTriSrfTo(const TrngTriangSrfStruct *TriSrf,
					    CagdPointType PType);

void IritTrngTriSrfDomain(const TrngTriangSrfStruct *TriSrf,
			  CagdRType *UMin,
			  CagdRType *UMax,
			  CagdRType *VMin,
			  CagdRType *VMax,
			  CagdRType *WMin,
			  CagdRType *WMax);
CagdBType IritTrngParamsInDomain(const TrngTriangSrfStruct *TriSrf,
				 CagdRType u,
				 CagdRType v,
				 CagdRType w);
CagdRType *IritTrngTriSrfEvalToData(const TrngTriangSrfStruct *TriSrf,
				    CagdRType u,
				    CagdRType v,
				    CagdRType w,
				    CagdRType *Pt);
CagdRType *IritTrngTriSrfEval2ToData(const TrngTriangSrfStruct *TriSrf,
				     CagdRType u,
				     CagdRType v,
				     CagdRType *Pt);
CagdVecStruct *IritTrngTriSrfNrmlToData(const TrngTriangSrfStruct *TriSrf,
					CagdRType u,
					CagdRType v,
					CagdVecStruct *Normal);
CagdBBoxStruct *IritTrngTriSrfBBox(const TrngTriangSrfStruct *TriSrf,
				   CagdBBoxStruct *BBox);
CagdBBoxStruct *IritTrngTriSrfListBBox(const TrngTriangSrfStruct *TriSrfs,
				       CagdBBoxStruct *BBox);
CagdPolylineStruct *IritTrngTriSrf2CtrlMesh(const TrngTriangSrfStruct *TriSrf);
CagdBType IritTrngBspTriSrfHasOpenEC(const TrngTriangSrfStruct *TriSrf);
TrngTriangSrfStruct *IritTrngBspTriSrfOpenEnd(
					    const TrngTriangSrfStruct *TriSrf);
struct IPPolygonStruct *IritTrngTriSrf2Polygons(
					 const TrngTriangSrfStruct *TriSrf,
					 const CagdSrf2PlsInfoStrct *TessInfo);
CagdPolylineStruct *IritTrngTriSrf2Polylines(const TrngTriangSrfStruct *TriSrf,
					     int NumOfIsocurves[3],
					     CagdRType TolSamples,
					     SymbCrvApproxMethodType Method);
CagdCrvStruct *IritTrngTriSrf2Curves(const TrngTriangSrfStruct *TriSrf,
				     int NumOfIsocurves[2]);
TrngTriangSrfStruct *IritTrngBzrTriSrfDirecDerive(
					     const TrngTriangSrfStruct *TriSrf,
					     CagdVType DirecDeriv);
TrngTriangSrfStruct *IritTrngTriSrfDerive(const TrngTriangSrfStruct *TriSrf,
					  TrngTriSrfDirType Dir);
TrngTriangSrfStruct *IritTrngBzrTriSrfDerive(const TrngTriangSrfStruct *TriSrf,
					     TrngTriSrfDirType Dir);
TrngTriangSrfStruct *IritTrngBspTriSrfDerive(const TrngTriangSrfStruct *TriSrf,
					     TrngTriSrfDirType Dir);
CagdCrvStruct *IritTrngCrvFromTriSrf(const TrngTriangSrfStruct *TriSrf,
				     CagdRType t,
				     TrngTriSrfDirType Dir);
CagdBType IritTrngTriSrfsSame(const TrngTriangSrfStruct *Srf1,
			      const TrngTriangSrfStruct	*Srf2,
			      CagdRType	Eps);

void IritTrngDbg(void *Obj);
void IritTrngDbg1(void *Obj);

void IritTrngGregory2Bezier4(CagdRType **Qt, CagdRType **Pt);
void IritTrngGregory2Bezier5(CagdRType **Qt, CagdRType **Pt);
void IritTrngGregory2Bezier6(CagdRType **Qt, CagdRType **Pt);

/*****************************************************************************
* Error	handling.							     *
*****************************************************************************/
TrngSetErrorFuncType IritTrngSetFatalErrorFunc(TrngSetErrorFuncType ErrorFunc);
void IritTrngFatalError(IritTrngFatalErrorType ErrID);
const char *IritTrngDescribeError(IritTrngFatalErrorType ErrID);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* IRIT_TRNG_LIB_H */
