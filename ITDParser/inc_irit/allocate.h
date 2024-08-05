/*****************************************************************************
* Definitions, visible to others, of the dynamic allocator module.	     *
******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute	of Technology		     *
******************************************************************************
* Written by:  Gershon Elber				Ver 0.2, Mar. 1990   *
*****************************************************************************/

#ifndef	IRIT_ALLOCATE_GH
#define	IRIT_ALLOCATE_GH

#include "inc_irit/iritprsr.h"
#include "inc_irit/obj_dpnd.h"
#include "inc_irit/cagd_lib.h"
#include "inc_irit/trim_lib.h"
#include "inc_irit/triv_lib.h"

#define IP_SET_OBJ_NAME2(PObj, Name) { \
    const char *_p = (Name); \
    if ((PObj) -> ObjName != NULL) \
	IritFree((PObj) -> ObjName); \
    (PObj) -> ObjName = (_p && _p[0] != 0 ? IritMiscStrdup(_p) : NULL); }
#define IP_CAT_OBJ_NAME(PObj, Apndx) { \
    char Name[IRIT_LINE_LEN_VLONG]; \
    strcpy(Name, (PObj) -> ObjName ? (PObj) -> ObjName : ""); \
    strcat(Name, Apndx); \
    IP_SET_OBJ_NAME2(PObj, Name); }
#define IP_SET_OBJ_NAME(PObj, Str, Param) { \
    char Name[IRIT_LINE_LEN_VLONG]; \
    sprintf(Name, Str, Param); \
    IP_SET_OBJ_NAME2(PObj, Name); }
#define IP_CLR_OBJ_NAME(PObj) { \
    if ((PObj) -> ObjName != NULL) \
	IritFree((PObj) -> ObjName); \
    (PObj) -> ObjName = NULL; }
#define IP_VALID_OBJ_NAME(PObj) ((PObj) -> ObjName != NULL && \
				 (PObj) -> ObjName[0] != 0)
#define IP_GET_OBJ_NAME(PObj) (IP_VALID_OBJ_NAME(PObj) ? (PObj) -> ObjName : "")

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

const char *IritPrsrObjTypeAsString(const IPObjectStruct *PObj);
void IritPrsrFreeObjectSlots(IPObjectStruct *PObj);
void IritPrsrFreeObjectGeomData(IPObjectStruct *PObj);
void IritPrsrFreeObjectBase(IPObjectStruct *O);
IPVertexStruct *IritPrsrAllocVertex(IrtBType Tags,
				    IPPolygonStruct *PAdj,
				    IPVertexStruct *Pnext);
IPVertexStruct *IritPrsrAllocVertex2(IPVertexStruct *Pnext);
IPPolygonStruct *IritPrsrAllocPolygon(IrtBType Tags,
				      IPVertexStruct *V,
				      IPPolygonStruct *Pnext);
IPObjectStruct *IritPrsrAllocObject(const char *Name,
				    IPObjStructType ObjType,
				    IPObjectStruct *Pnext);
void IritPrsrFreeVertex(IPVertexStruct *V);
void IritPrsrFreePolygon(IPPolygonStruct *P);
void IritPrsrFreeObject(IPObjectStruct *O);
void IritPrsrFreeVertexList(IPVertexStruct *VFirst);
void IritPrsrFreePolygonList(IPPolygonStruct *PPoly);
void IritPrsrFreeObjectList(IPObjectStruct *O);

#ifdef DEBUG
#define IritPrsrFreeVertex(V)          { IritPrsrFreeVertex(V); V = NULL; }
#define IritPrsrFreePolygon(P)         { IritPrsrFreePolygon(P); P = NULL; }
#define IritPrsrFreeObject(O)          { IritPrsrFreeObject(O); O = NULL; }
#define IritPrsrFreeVertexList(VFirst) { IritPrsrFreeVertexList(VFirst); VFirst = NULL; }
#define IritPrsrFreePolygonList(PPoly) { IritPrsrFreePolygonList(PPoly); PPoly = NULL; }
#define IritPrsrFreeObjectList(O)      { IritPrsrFreeObjectList(O); O = NULL; }
#endif /* DEBUG */

IPPolyVrtxArrayStruct *IritPrsrPolyVrtxArrayNew(int NumVrtcs, int NumPlys);
IPPolyVrtxArrayStruct *IritPrsrPolyVrtxArrayNew2(IPObjectStruct *PObj);
void IritPrsrPolyVrtxArrayFree(IPPolyVrtxArrayStruct *PVIdx);

int IritPrsrListObjectLength(const IPObjectStruct *PObj);
int IritPrsrListObjectFind(const IPObjectStruct *PObjList,
			   const IPObjectStruct	*PObj);
void IritPrsrListObjectInsert(IPObjectStruct *PObjList,
			      int Index,
			      IPObjectStruct *PObjItem);
void IritPrsrListObjectInsert2(IPObjectStruct *PObj,
			       int Index,
			       IPObjectStruct *PObjItem);
void IritPrsrListObjectAppend(IPObjectStruct *PObjList,
			      IPObjectStruct *PObjItem);
void IritPrsrListObjectAppendList(IPObjectStruct *PObj,
				  IPObjectStruct *PObjNew);
IPObjectStruct *IritPrsrListObjectDelete(IPObjectStruct *PObj, 
					 int Index,
					 int FreeItem);
void IritPrsrListObjectDelete2(IPObjectStruct *PObj,
			       IPObjectStruct *PObjToDel,
			       int FreeItem);
IPObjectStruct *IritPrsrListObjectGet(const IPObjectStruct *PObjList,
				      int Index);
IPObjectStruct *IritPrsrListObjectFilter(IPObjectStruct *PObj);
void IritPrsrPropagateObjectName(IPObjectStruct *Obj,
				 const char *ObjName,
				 int ForceRename);
void IritPrsrReallocNewTypeObject(IPObjectStruct *PObj,
				  IPObjStructType ObjType);

IPObjectStruct *IritPrsrGenPolyObject(const char *Name,
				      IPPolygonStruct *Pl,
				      IPObjectStruct *Pnext);
IPObjectStruct *IritPrsrGenPOLYObject(IPPolygonStruct *Pl);
IPObjectStruct *IritPrsrGenPolylineObject(const char *Name,
					  IPPolygonStruct *Pl,
					  IPObjectStruct *Pnext);
IPObjectStruct *IritPrsrGenPOLYLINEObject(IPPolygonStruct *Pl);
IPObjectStruct *IritPrsrGenPointListObject(const char *Name,
					   IPPolygonStruct *Pl,
					   IPObjectStruct *Pnext);
IPObjectStruct *IritPrsrGenPOINTLISTObject(IPPolygonStruct *Pl);
IPObjectStruct *IritPrsrGenCrvObject(const char *Name,
				     CagdCrvStruct *Crv,
				     IPObjectStruct *Pnext);
IPObjectStruct *IritPrsrGenCRVObject(CagdCrvStruct *Crv);
IPObjectStruct *IritPrsrGenSrfObject(const char *Name,
				     CagdSrfStruct *Srf,
				     IPObjectStruct *Pnext);
IPObjectStruct *IritPrsrGenSRFObject(CagdSrfStruct *Srf);
IPObjectStruct *IritPrsrGenTrimSrfObject(const char *Name,
					 TrimSrfStruct *TrimSrf,
					 IPObjectStruct	*Pnext);
IPObjectStruct *IritPrsrGenTRIMSRFObject(TrimSrfStruct *TrimSrf);
IPObjectStruct *IritPrsrGenTrivarObject(const char *Name,
					TrivTVStruct *Triv,
					IPObjectStruct *Pnext);
IPObjectStruct *IritPrsrGenTRIVARObject(TrivTVStruct *Triv);
IPObjectStruct *IritPrsrGenTriSrfObject(const char *Name,
					TrngTriangSrfStruct *TriSrf,
					IPObjectStruct *Pnext);
IPObjectStruct *IritPrsrGenTRISRFObject(TrngTriangSrfStruct *TriSrf);
IPObjectStruct *IritPrsrGenModelObject(const char *Name,
				       MdlModelStruct *Model,
				       IPObjectStruct *Pnext);
IPObjectStruct *IritPrsrGenMODELObject(MdlModelStruct *Model);
IPObjectStruct *IritPrsrGenVModelObject(const char *Name,
					VMdlVModelStruct *VModel,
					IPObjectStruct *Pnext);
IPObjectStruct *IritPrsrGenVMODELObject(VMdlVModelStruct *VModel);
IPObjectStruct *IritPrsrGenVxlVMdlObject(const char *Name,
					 VMdlVoxelVModelStruct *VxlVModel,
					 IPObjectStruct	*Pnext);
IPObjectStruct *IritPrsrGenVXLVMDLObject(VMdlVoxelVModelStruct *VxlVModel);
IPObjectStruct *IritPrsrGenMultiVarObject(const char *Name,
					  MvarMVStruct *MultiVar,
					  IPObjectStruct *Pnext);
IPObjectStruct *IritPrsrGenMULTIVARObject(MvarMVStruct *MultiVar);
IPObjectStruct *IritPrsrGenInstncObject(const char *Name,
					const char *InstncName,
					const IrtHmgnMatType *Mat,
					IPObjectStruct *Pnext);
IPObjectStruct *IritPrsrGenINSTNCObject(const char *InstncName,
					const IrtHmgnMatType *Mat);
IPObjectStruct *IritPrsrGenCtlPtObject(const char *Name,
				       CagdPointType PtType,
				       const IrtRType *Coords,
				       IPObjectStruct *Pnext);
IPObjectStruct *IritPrsrGenCTLPTObject(CagdPointType PtType,
				       const IrtRType *Coords);
IPObjectStruct *IritPrsrGenNumObject(const char *Name,
				     const IrtRType *R,
				     IPObjectStruct *Pnext);
IPObjectStruct *IritPrsrGenNUMObject(const IrtRType *R);
IPObjectStruct *IritPrsrGenNUMValObject(IrtRType R);
IPObjectStruct *IritPrsrGenPtObject(const char *Name,
				    const IrtRType *Pt0,
				    const IrtRType *Pt1,
				    const IrtRType *Pt2,
				    IPObjectStruct *Pnext);
IPObjectStruct *IritPrsrGenPTObject(const IrtRType *Pt0,
				    const IrtRType *Pt1,
				    const IrtRType *Pt2);
IPObjectStruct *IritPrsrGenVecObject(const char *Name,
				     const IrtRType *Vec0,
				     const IrtRType *Vec1,
				     const IrtRType *Vec2,
				     IPObjectStruct *Pnext);
IPObjectStruct *IritPrsrGenVECObject(const IrtRType *Vec0,
				     const IrtRType *Vec1,
				     const IrtRType *Vec2);
IPObjectStruct *IritPrsrGenStrObject(const char *Name,
				     const char	*Str,
				     IPObjectStruct *Pnext);
IPObjectStruct *IritPrsrGenSTRObject(const char *Str);
IPObjectStruct *IritPrsrGenWStrObject(const char *Name,
				      const IritFontWChar *Str,
				      IPObjectStruct *Pnext);
IPObjectStruct *IritPrsrGenWSTRObject(const IritFontWChar *Str);
IPObjectStruct *IritPrsrGenListObject(const char *Name,
				      IPObjectStruct *First,
				      IPObjectStruct *Pnext);
IPObjectStruct *IritPrsrGenLISTObject(IPObjectStruct *First);
IPObjectStruct *IritPrsrGenPlaneObject(const char *Name,
				       const IrtRType *Plane0,
				       const IrtRType *Plane1,
				       const IrtRType *Plane2,
				       const IrtRType *Plane3,
				       IPObjectStruct *Pnext);
IPObjectStruct *IritPrsrGenPLANEObject(const IrtRType *Plane0,
				       const IrtRType *Plane1,
				       const IrtRType *Plane2,
				       const IrtRType *Plane3);
IPObjectStruct *IritPrsrGenMatObject(const char *Name,
				     IrtHmgnMatType Mat,
				     IPObjectStruct *Pnext);
IPObjectStruct *IritPrsrGenMATObject(IrtHmgnMatType Mat);

void IritPrsrCopyObjectAuxInfo(IPObjectStruct *Dest,
			       const IPObjectStruct *Src);
IPObjectStruct *IritPrsrCopyObject(IPObjectStruct *Dest,
				   const IPObjectStruct	*Src,
				   int CopyAll);
IPObjectStruct *IritPrsrCopyObject2(IPObjectStruct *Dest,
				    const IPObjectStruct *Src,
				    int	CopyNext,
				    int	CopyName);
IPObjectStruct *IritPrsrCopyObjectGeomData(IPObjectStruct *Dest,
					   const IPObjectStruct	*Src,
					   int CopyNext,
					   int CopyName);
int IritPrsrSetCopyObjectReferenceCount(int RefCount);

IPObjectStruct *IritPrsrCopyObjectList(const IPObjectStruct *PObjs,
				       int CopyAll);
IPPolygonStruct *IritPrsrCopyPolygon(const IPPolygonStruct *Src);
IPPolygonStruct *IritPrsrCopyPolygonList(const IPPolygonStruct *Src);
IPVertexStruct *IritPrsrCopyVertex(const IPVertexStruct *Src);
IPVertexStruct *IritPrsrCopyVertexList(const IPVertexStruct *Src);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif	/* IRIT_ALLOCATE_GH */
