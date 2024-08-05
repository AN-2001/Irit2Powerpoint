/*****************************************************************************
* Setting attributes for geometric objects.				     *
******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute	of Technology		     *
******************************************************************************
* Written by:  Gershon Elber				Ver 0.2, Mar. 1990   *
*****************************************************************************/

#ifndef IRIT_ATTRIBUTE_H
#define IRIT_ATTRIBUTE_H

#include "inc_irit/iritprsr.h"
#include "inc_irit/miscattr.h"

#define ATTR_OBJ_IS_INVISIBLE(PObj) \
	(IritMiscAttrIDGetObjectIntAttrib((PObj), \
			  IRIT_ATTR_CREATE_ID(invisible)) != IP_ATTR_BAD_INT)

#define ATTR_OBJ_ATTR_EXIST(PObj, AttrID) (IritMiscAttrIDFindAttribute(PObj -> Attr, \
							     AttrID) != NULL)

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/* From attribute.c */

void IritMiscAttrSetObjectColor(const IPObjectStruct *PObj, int Color);
int IritMiscAttrGetObjectColor(const IPObjectStruct *PObj);
void IritMiscAttrSetObjectRGBColor(const IPObjectStruct *PObj,
				   int Red,
				   int Green,
				   int Blue);
int IritMiscAttrGetObjectRGBColor(const IPObjectStruct *PObj,
				  int *Red,
				  int *Green,
				  int *Blue);
int IritMiscAttrGetObjectRGBColor2(const IPObjectStruct *PObj,
				   const char *Name,
				   int *Red,
				   int *Green,
				   int *Blue);
void IritMiscAttrSetRGBDoubleColor(const IPAttributeStruct **Attrs,
				   double Red,
				   double Green,
				   double Blue);
int IritMiscAttrGetRGBDoubleColor(const IPAttributeStruct *Attrs,
				  double *Red,
				  double *Green,
				  double *Blue);
void IritMiscAttrSetObjectWidth(const IPObjectStruct *PObj, IrtRType Width);
IrtRType IritMiscAttrGetObjectWidth(const IPObjectStruct *PObj);

void IritMiscAttrSetObjectIntAttrib(const IPObjectStruct *PObj,
				    const char *Name,
				    int	Data);
void IritMiscAttrSetObjectIntAttrib2(const IPObjectStruct *PObj,
				     IPAttrNumType AttribNum,
				     int Data);
int IritMiscAttrGetObjectIntAttrib(const IPObjectStruct *PObj,
				   const char *Name);
int IritMiscAttrGetObjectIntAttrib2(const IPObjectStruct *PObj,
				    IPAttrNumType AttribNum);

void IritMiscAttrSetObjectRealAttrib(const IPObjectStruct *PObj,
				     const char	*Name,
				     IrtRType Data);
void IritMiscAttrSetObjectRealAttrib2(const IPObjectStruct *PObj,
				      IPAttrNumType AttribNum,
				      IrtRType Data);
IrtRType IritMiscAttrGetObjectRealAttrib(const IPObjectStruct *PObj,
					 const char *Name);
IrtRType IritMiscAttrGetObjectRealAttrib2(const IPObjectStruct *PObj,
					  IPAttrNumType	AttribNum);

void IritMiscAttrSetObjectRealPtrAttrib(const IPObjectStruct *PObj,
					const char *Name,
					IrtRType *Data,
					int DataLen);
void IritMiscAttrSetObjectRealPtrAttrib2(const IPObjectStruct *PObj,
					 IPAttrNumType AttribNum,
					 IrtRType *Data,
					 int DataLen);
IrtRType *IritMiscAttrGetObjectRealPtrAttrib(const IPObjectStruct *PObj,
					     const char	*Name);
IrtRType *IritMiscAttrGetObjectRealPtrAttrib2(const IPObjectStruct *PObj,
					      IPAttrNumType AttribNum);

void IritMiscAttrSetObjectUVAttrib(const IPObjectStruct *PObj,
				   const char *Name,
				   IrtRType U,
				   IrtRType V);
void IritMiscAttrSetObjectUVAttrib2(const IPObjectStruct *PObj,
				    IPAttrNumType AttribNum,
				    IrtRType U,
				    IrtRType V);
float *IritMiscAttrGetObjectUVAttrib(const IPObjectStruct *PObj,
				     const char *Name);
float *IritMiscAttrGetObjectUVAttrib2(const IPObjectStruct *PObj,
				      IPAttrNumType AttribNum);

void IritMiscAttrSetObjectPtrAttrib(const IPObjectStruct *PObj,
				    const char *Name,
				    VoidPtr Data);
void IritMiscAttrSetObjectPtrAttrib2(const IPObjectStruct *PObj,
				     IPAttrNumType AttribNum,
				     VoidPtr Data);
VoidPtr IritMiscAttrGetObjectPtrAttrib(const IPObjectStruct *PObj,
				       const char *Name);
VoidPtr IritMiscAttrGetObjectPtrAttrib2(const IPObjectStruct *PObj,
					IPAttrNumType AttribNum);

void IritMiscAttrSetObjectRefPtrAttrib(const IPObjectStruct *PObj,
				       const char *Name,
				       VoidPtr Data);
void IritMiscAttrSetObjectRefPtrAttrib2(const IPObjectStruct *PObj,
					IPAttrNumType AttribNum,
					VoidPtr	Data);
VoidPtr IritMiscAttrGetObjectRefPtrAttrib(const IPObjectStruct *PObj,
					  const	char *Name);
VoidPtr IritMiscAttrGetObjectRefPtrAttrib2(const IPObjectStruct *PObj,
					   IPAttrNumType AttribNum);

void IritMiscAttrSetObjectStrAttrib(const IPObjectStruct *PObj,
				    const char *Name,
				    const char *Data);
void IritMiscAttrSetObjectStrAttrib2(const IPObjectStruct *PObj,
				     IPAttrNumType AttribNum,
				     const char	*Data);
const char *IritMiscAttrGetObjectStrAttrib(const IPObjectStruct *PObj,
					   const char *Name);
const char *IritMiscAttrGetObjectStrAttrib2(const IPObjectStruct *PObj,
					    IPAttrNumType AttribNum);

void IritMiscAttrSetObjectObjAttrib(const IPObjectStruct *PObj,
				    const char *Name,
				    IPObjectStruct *Data,
				    int	CopyData);
void IritMiscAttrSetObjectObjAttrib2(const IPObjectStruct *PObj,
				     IPAttrNumType AttribNum,
				     IPObjectStruct *Data,
				     int CopyData);
void IritMiscAttrSetObjAttrib(IPAttributeStruct **Attrs,
			      const char *Name,
			      IPObjectStruct *Data,
			      int CopyData);
void IritMiscAttrSetObjAttrib2(IPAttributeStruct **Attrs,
			       IPAttrNumType AttribNum,
			       IPObjectStruct *Data,
			       int CopyData);
IPObjectStruct *IritMiscAttrGetObjectObjAttrib(const IPObjectStruct *PObj,
					       const char *Name);
IPObjectStruct *IritMiscAttrGetObjectObjAttrib2(const IPObjectStruct *PObj,
						IPAttrNumType AttribNum);
IPObjectStruct *IritMiscAttrGetObjAttrib(const IPAttributeStruct *Attrs,
					 const char *Name);
IPObjectStruct *IritMiscAttrGetObjAttrib2(const IPAttributeStruct *Attrs,
					  IPAttrNumType	AttribNum);

void IritMiscAttrFreeObjectAttribute(const IPObjectStruct *PObj,
				     const char *Name);

IPAttributeStruct *IritMiscAttrGetObjectGeomAttr(const IPObjectStruct *PObj);
IPObjectStruct *IritMiscAttr2IritObject(const IPAttributeStruct *Attr);
IPAttributeStruct *IritMiscAttrCopyOneAttribute(const IPAttributeStruct *Src);
IPAttributeStruct *IritMiscAttrCopyOneAttribute2(const IPAttributeStruct *Src,
						 int AllAttr);
IPAttributeStruct *IritMiscAttrCopyAttributes(const IPAttributeStruct *Src);
void IritMiscAttrPropagateAttr(const IPObjectStruct *PObj,
			       const char *AttrName);
void IritMiscAttrPropagateRGB2Vrtx(const IPObjectStruct *PObj);
IPObjectStruct const * const *IritMiscAttrFindObjsWithAttr(
					       const IPObjectStruct *PObjs,
					       const char *AttrName,
					       const IPObjectStruct *AttrVal,
					       int LeavesOnly,
					       int Negate);
IPObjectStruct *IritMiscAttrMergeGeomSimilarAttrs(
					       const IPObjectStruct *PObjList,
					       const char *AttrName,
					       int MergeOptions);

/* From attribute_id.c */

void IritMiscAttrIDSetObjectColor(const IPObjectStruct *PObj, int Color);
int IritMiscAttrIDGetObjectColor(const IPObjectStruct *PObj);
void IritMiscAttrIDSetObjectRGBColor(const IPObjectStruct *PObj,
				     int Red,
				     int Green,
				     int Blue);
int IritMiscAttrIDGetObjectRGBColor(const IPObjectStruct *PObj,
				    int	*Red,
				    int	*Green,
				    int	*Blue);
int IritMiscAttrIDGetObjectRGBColor2(const IPObjectStruct *PObj,
				     int *Red,
				     int *Green,
				     int *Blue);
void IritMiscAttrIDSetObjectWidth(const IPObjectStruct *PObj, IrtRType Width);
IrtRType IritMiscAttrIDGetObjectWidth(const IPObjectStruct *PObj);
void IritMiscAttrIDSetObjectIntAttrib(const IPObjectStruct *PObj,
				      IPAttrIDType ID,
				      int Data);
int IritMiscAttrIDGetObjectIntAttrib(const IPObjectStruct *PObj,
				     IPAttrIDType ID);
void IritMiscAttrIDSetObjectPtrAttrib(const IPObjectStruct *PObj,
				      IPAttrIDType ID,
				      VoidPtr Data);
VoidPtr IritMiscAttrIDGetObjectPtrAttrib(const IPObjectStruct *PObj,
					 IPAttrNumType ID);
void IritMiscAttrIDSetObjectRefPtrAttrib(const IPObjectStruct *PObj,
					 IPAttrIDType ID,
					 VoidPtr Data);
VoidPtr IritMiscAttrIDGetObjectRefPtrAttrib(const IPObjectStruct *PObj,
					    IPAttrIDType ID);
void IritMiscAttrIDSetObjectRealAttrib(const IPObjectStruct *PObj,
				       IPAttrIDType ID,
				       IrtRType	Data);
IrtRType IritMiscAttrIDGetObjectRealAttrib(const IPObjectStruct *PObj,
					   IPAttrIDType	ID);
void IritMiscAttrIDSetObjectRealPtrAttrib(const IPObjectStruct *PObj,
				          IPAttrIDType ID,
				          IrtRType *Data,
				          int DataLen);
IrtRType *IritMiscAttrIDGetObjectRealPtrAttrib(const IPObjectStruct *PObj,
					       IPAttrIDType ID);
void IritMiscAttrIDSetObjectUVAttrib(const IPObjectStruct *PObj,
				     IPAttrIDType ID,
				     IrtRType U,
				     IrtRType V);
float *IritMiscAttrIDGetObjectUVAttrib(const IPObjectStruct *PObj,
				       IPAttrIDType ID);
void IritMiscAttrIDSetObjectStrAttrib(const IPObjectStruct *PObj,
				      IPAttrIDType ID,
				      const char *Data);
const char *IritMiscAttrIDGetObjectStrAttrib(const IPObjectStruct *PObj,
					     IPAttrIDType ID);
void IritMiscAttrIDSetObjectObjAttrib(const IPObjectStruct *PObj,
				      IPAttrIDType ID,
				      IPObjectStruct *Data,
				      int CopyData);
void IritMiscAttrIDSetObjAttrib(IPAttributeStruct **Attrs,
				IPAttrIDType ID,
				IPObjectStruct *Data,
				int CopyData);
IPObjectStruct *IritMiscAttrIDGetObjectObjAttrib(const IPObjectStruct *PObj,
						 IPAttrIDType ID);
IPObjectStruct *IritMiscAttrIDGetObjAttrib(const IPAttributeStruct *Attrs,
					   IPAttrIDType	ID);
void IritMiscAttrIDFreeObjectAttribute(const IPObjectStruct *PObj,
				       IPAttrIDType ID);
void IritMiscAttrIDPropagateAttr(const IPObjectStruct *PObj,
				 IPAttrIDType AttrID);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* IRIT_ATTRIBUTE_H */
