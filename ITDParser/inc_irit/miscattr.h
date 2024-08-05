/*****************************************************************************
* Setting attributes for objects.					     *
******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute	of Technology		     *
******************************************************************************
* Written by:  Gershon Elber				Ver 0.2, Mar. 1990   *
*****************************************************************************/

#ifndef IRIT_MISC_ATTR_H
#define IRIT_MISC_ATTR_H

#include "inc_irit/irit_sm.h"
#include "inc_irit/misc_attr_id.h"

typedef enum {
    IP_ATTR_NONE,
    IP_ATTR_INT,
    IP_ATTR_REAL,
    IP_ATTR_REALPTR,
    IP_ATTR_UV,
    IP_ATTR_STR,
    IP_ATTR_OBJ,
    IP_ATTR_PTR,
    IP_ATTR_REFPTR
} IPAttributeType;

#define IP_ATTR_BAD_INT		-2147182588
#define IP_ATTR_BAD_REAL	1e30
#define IP_ATTR_NO_COLOR	999
#define IP_ATTR_NO_WIDTH	1e30

#define IP_ATTR_MAX_STR_LENGTH	100000

#define IP_ATTR_IS_BAD_INT(I)	((I) == IP_ATTR_BAD_INT)
#define IP_ATTR_IS_BAD_REAL(R)	((R) > IP_ATTR_BAD_REAL / 10.0)
#define IP_ATTR_IS_BAD_COLOR(C)	((C) == IP_ATTR_NO_COLOR)
#define IP_ATTR_IS_BAD_WIDTH(W)	((W) > IP_ATTR_NO_WIDTH / 10.0)

#define IP_ATTR_RESET_ATTRS(Attr)  { (Attr) = NULL; }
#define IP_ATTR_FREE_ATTRS(Attr)   { if ((Attr) != NULL) \
					IritMiscAttrFreeAttributes(&(Attr)); }
#define IP_ATTR_COPY_ATTRS(Attr) \
	(Attr) != NULL ? IritMiscAttrCopyAttributes(Attr) : NULL;
#define IP_ATTR_COPY_ATTRS2(NewAttr, OldAttr) { \
	NewAttr = IP_ATTR_COPY_ATTRS(OldAttr); }
#define IP_ATTR_SAFECOPY_ATTRS(NewAttr, OldAttr) { \
	IP_ATTR_FREE_ATTRS(NewAttr); \
	NewAttr = IP_ATTR_COPY_ATTRS(OldAttr); }

// GERSHON - will require Locking
#define IP_ATTR_INIT_UNIQUE_ID_NUM(AttrID, StrID) \
    if (AttrID == ATTRIB_NAME_BAD_NUMBER) { \
        AttrID = IritMiscAttrGetAttribHashNumber(StrID); \
    }

#define IP_ATTR_IRIT_COLOR_TABLE_SIZE	16

typedef unsigned int IPAttrNumType;

#define ATTRIB_NAME_BAD_NUMBER    ((IPAttrNumType) -1)

/*****************************************************************************
* Attributes - an attribute has	a name and can be one of the following:	     *
* an integer, real, string, or a pointer to an Object.			     *
*****************************************************************************/
typedef struct IPAttributeStruct {
    struct IPAttributeStruct *Pnext;
    IPAttributeType Type;
    union {
	char *Str;
   	int I;
	IrtRType R;
	struct {
	    IrtRType *Coord;
	    int Len;
	} Vec;
	float UV[2];
	struct IPObjectStruct *PObj;
	VoidPtr Ptr;
	VoidPtr RefPtr;
    } U;
    IPAttrNumType _AttribNum;           /* Used in string based attributes. */
    IPAttrIDType _AttribID;                  /* Used in Attribute ID attrs. */
} IPAttributeStruct;

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

IRIT_GLOBAL_DATA_HEADER const IPAttrIDDefStruct _IPAttrIDDefTable[];
IRIT_GLOBAL_DATA_HEADER const int IritMiscAttrIritColorTable[][3];
IRIT_GLOBAL_DATA_HEADER const int IritMiscAttrIritColorTableSize;

/* Functions in miscattr.c */

void IritMiscAttrGetIndexColor(int Color, int *Red, int *Green, int *Blue);
int IritMiscAttrGetRGBColor2(const IPAttributeStruct *Attrs, 
			     const char *Name,
			     int *Red, 
			     int *Green, 
			     int *Blue);
void IritMiscAttrSetIntAttrib(IPAttributeStruct **Attrs,
			      const char *Name,
			      int Data);
void IritMiscAttrSetIntAttrib2(IPAttributeStruct **Attrs,
			       IPAttrNumType AttribNum,
			       int Data);
int IritMiscAttrGetIntAttrib(const IPAttributeStruct *Attrs, const char *Name);
int IritMiscAttrGetIntAttrib2(const IPAttributeStruct *Attrs,
			      IPAttrNumType AttrNum);

void IritMiscAttrSetRealAttrib(IPAttributeStruct **Attrs,
			       const char *Name,
			       IrtRType Data);
void IritMiscAttrSetRealAttrib2(IPAttributeStruct **Attrs,
				IPAttrNumType AttribNum,
				IrtRType Data);
IrtRType IritMiscAttrGetRealAttrib(const IPAttributeStruct *Attrs,
				   const char *Name);
IrtRType IritMiscAttrGetRealAttrib2(const IPAttributeStruct *Attrs,
				    IPAttrNumType AttrNum);

void IritMiscAttrSetRealPtrAttrib(IPAttributeStruct **Attrs,
				  const char *Name,
				  IrtRType *Data,
				  int DataLen);
void IritMiscAttrSetRealPtrAttrib2(IPAttributeStruct **Attrs,
				   IPAttrNumType AttribNum,
				   IrtRType *Data,
				   int DataLen);
IrtRType *IritMiscAttrGetRealPtrAttrib(const IPAttributeStruct *Attrs,
				       const char *Name);
IrtRType *IritMiscAttrGetRealPtrAttrib2(const IPAttributeStruct *Attrs,
					IPAttrNumType AttrNum);

void IritMiscAttrSetUVAttrib(IPAttributeStruct **Attrs,
			     const char *Name,
			     IrtRType U,
			     IrtRType V);
void IritMiscAttrSetUVAttrib2(IPAttributeStruct **Attrs,
			      IPAttrNumType AttribNum,
			      IrtRType U,
			      IrtRType V);
float *IritMiscAttrGetUVAttrib(const IPAttributeStruct *Attrs,
			       const char *Name);
float *IritMiscAttrGetUVAttrib2(const IPAttributeStruct *Attrs,
				IPAttrNumType AttrNum);

void IritMiscAttrSetPtrAttrib(IPAttributeStruct **Attrs,
			      const char *Name,
			      VoidPtr Data);
void IritMiscAttrSetPtrAttrib2(IPAttributeStruct **Attrs,
			       IPAttrNumType AttribNum,
			       VoidPtr Data);
VoidPtr IritMiscAttrGetPtrAttrib(const IPAttributeStruct *Attrs,
				 const char *Name);
VoidPtr IritMiscAttrGetPtrAttrib2(const IPAttributeStruct *Attrs,
				  IPAttrNumType AttrNum);

void IritMiscAttrSetRefPtrAttrib(IPAttributeStruct **Attrs,
				 const char *Name,
				 VoidPtr Data);
void IritMiscAttrSetRefPtrAttrib2(IPAttributeStruct **Attrs,
				  IPAttrNumType AttribNum,
				  VoidPtr Data);
VoidPtr IritMiscAttrGetRefPtrAttrib(const IPAttributeStruct *Attrs,
				    const char *Name);
VoidPtr IritMiscAttrGetRefPtrAttrib2(const IPAttributeStruct *Attrs,
				     IPAttrNumType AttrNum);

void IritMiscAttrSetStrAttrib(IPAttributeStruct **Attrs,
			      const char *Name,
			      const char *Data);
void IritMiscAttrSetStrAttrib2(IPAttributeStruct **Attrs,
			       IPAttrNumType AttribNum,
			       const char *Data);
const char *IritMiscAttrGetStrAttrib(const IPAttributeStruct *Attrs,
				     const char *Name);
const char *IritMiscAttrGetStrAttrib2(const IPAttributeStruct *Attrs,
				      IPAttrNumType AttrNum);

const IPAttributeStruct *IritMiscAttrTraceAttributes(
					  const IPAttributeStruct *TraceAttrs,
					  const IPAttributeStruct *FirstAttrs);
char *IritMiscAttr2StringToData(const IPAttributeStruct *Attr,
				int DataFileFormat);
int IritMiscAttrCmpTwoAttrByName(const IPAttributeStruct *AttrList1,
				 const IPAttributeStruct *AttrList2,
				 const char *AttrName);

IPAttributeStruct *IritMiscAttrReverseAttributes(IPAttributeStruct *Attr);

void IritMiscAttrFreeOneAttribute(IPAttributeStruct **Attrs, const char *Name);
void IritMiscAttrFreeAttributes(IPAttributeStruct **Attrs);

IPAttributeStruct *IritMiscAttrFindAttribute(const IPAttributeStruct *Attrs,
					     const char *Name);

IPAttributeStruct *_IritMiscAttrMallocAttributeNameType(const char *Name,
						    IPAttributeType Type);
IPAttributeStruct *_IritMiscAttrMallocAttributeHashNum(
					       IPAttrNumType AttribHashNum, 
					       IPAttributeType Type);
IPAttributeStruct *_IritMiscAttrMallocAttributeIDType(IPAttrNumType AttribID, 
						  IPAttributeType Type);
void _IritMiscAttrFreeAttributeData(IPAttributeStruct *Attr);

const char **IritMiscAttrCopyValidAttrNameList(const char **AttrNames);
IPAttributeStruct *IritMiscAttrCopyAttributes(const IPAttributeStruct *Src);
IPAttributeStruct *IritMiscAttrCopyAttributes2(const IPAttributeStruct *Src,
					       int AllAttrs);
IPAttributeStruct *IritMiscAttrCopyOneAttribute(const IPAttributeStruct *Src);
IPAttributeStruct *IritMiscAttrCopyOneAttribute2(const IPAttributeStruct *Src,
						 int AllAttr);
IPAttributeStruct *IritMiscAttrMergeAttributes(IPAttributeStruct *Orig,
					       const IPAttributeStruct *Src,
					       int Replace);
IPAttributeStruct *IritMiscAttrGetLastAttr(IPAttributeStruct *AList);
IPAttributeStruct *IritMiscAttrGetPrevAttr(IPAttributeStruct *AList,
					   const IPAttributeStruct *A);

/* Functions in miscatt1.c */

const char *IritMiscAttrGetAttribName(const IPAttributeStruct *Attr);
IPAttrNumType IritMiscAttrGetAttribHashNumber(const char *AttribName);
IPAttributeStruct *IritMiscAttrFindAttributeHashNum(
					    const IPAttributeStruct *Attrs, 
				   	    IPAttrNumType AttrNum);
void IritMiscAttrInitHashTbl(void);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* IRIT_MISC_ATTR_H */
