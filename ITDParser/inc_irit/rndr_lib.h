/******************************************************************************
* Rndr_lib.h - header file for the RNDR	library.			     *
* This header is also the interface header to the world.		     *
*******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute	of Technology		     *
*******************************************************************************
* Written by Michael Plavnik and Dimitri Beserb, Mar. 95.		     *
* Modified by David Shafrir and	Alex Reicher, Mar 2004.			     *
******************************************************************************/

#ifndef IRIT_RNDR_LIB_H
#define IRIT_RNDR_LIB_H

#include "inc_irit/irit_sm.h"
#include "inc_irit/iritprsr.h"

#define IRNDR_PURGED_POLY_ATTR_ID       IRIT_ATTR_CREATE_ID(IRndrPurgedPoly)
#define IRNDR_VM_BEFORE_PRSP_X_ATTR_ID  IRIT_ATTR_CREATE_ID(IRndrVMBeforePrspX)
#define IRNDR_VM_BEFORE_PRSP_Y_ATTR_ID  IRIT_ATTR_CREATE_ID(IRndrVMBeforePrspY)
#define IRNDR_VM_BEFORE_PRSP_Z_ATTR_ID  IRIT_ATTR_CREATE_ID(IRndrVMBeforePrspZ)

typedef enum IRndrLightType {
    IRNDR_LIGHT_POINT,
    IRNDR_LIGHT_VECTOR,
    IRNDR_LIGHT_LAST
} IRndrLightType;

typedef enum IRndrShadingType {
    IRNDR_SHADING_FLAT,
    IRNDR_SHADING_GOURAUD,
    IRNDR_SHADING_PHONG,
    IRNDR_SHADING_NONE,
    IRNDR_SHADING_LAST
} IRndrShadingType;

typedef enum IRndrZBufferCmpType {
    IRNDR_ZBUFFER_NEVER,
    IRNDR_ZBUFFER_LESS,
    IRNDR_ZBUFFER_LEQUAL,
    IRNDR_ZBUFFER_GREATER,
    IRNDR_ZBUFFER_GEQUAL,
    IRNDR_ZBUFFER_NOTEQUAL,
    IRNDR_ZBUFFER_ALWAYS
} IRndrZBufferCmpType;

typedef enum IRndrStencilCmpType {
    IRNDR_STENCIL_NEVER,
    IRNDR_STENCIL_LESS,
    IRNDR_STENCIL_LEQUAL,
    IRNDR_STENCIL_GREATER,
    IRNDR_STENCIL_GEQUAL,
    IRNDR_STENCIL_EQUAL,
    IRNDR_STENCIL_NOTEQUAL,
    IRNDR_STENCIL_ALWAYS
} IRndrStencilCmpType;

typedef enum IritRndrStencilOpType {
    IRNDR_STENCIL_KEEP,
    IRNDR_STENCIL_ZERO,
    IRNDR_STENCIL_REPLACE,
    IRNDR_STENCIL_INCR,
    IRNDR_STENCIL_DECR,
    IRNDR_STENCIL_INVERT
} IritRndrStencilOpType;

typedef enum IRndrVisibleFillType {
    IRNDR_VISMAP_FILL_EMPTY = 0,
    IRNDR_VISMAP_FILL_MAPPED,
    IRNDR_VISMAP_FILL_VISIBLE,
    IRNDR_VISMAP_FILL_DILATE_MAPPED,
    IRNDR_VISMAP_FILL_DILATE_VISIBLE
} IRndrVisibleFillType;

typedef enum IRndrVisibleValidityType {
    IRNDR_VISMAP_VALID_OK,
    IRNDR_VISMAP_VALID_TANGENT,
    IRNDR_VISMAP_VALID_POOR_AR,
    IRNDR_VISMAP_VALID_DEGEN,
    IRNDR_VISMAP_VALID_NOT_RENDERED,
    IRNDR_VISMAP_VALID_COUNT /* Number of validity types. */
} IRndrVisibleValidityType;

typedef IrtRType IRndrColorType[4];			  /* Space for RGBA. */
typedef float IRndrZDepthType;
typedef struct IRndrStruct *IRndrPtrType;
typedef struct INCZBufferStruct *INCZBufferPtrType;
typedef struct IRndrZBuffer1DStruct *IRndrZBuffer1DPtrType;
typedef IrtBType (*IRndrZCmpPolicyFuncType)(int x,
					    int y,
					    IrtRType OldZ,
					    IrtRType NewZ);
typedef void (*IRndrPixelClbkFuncType)(int x,
				       int y,
				       IRndrColorType Color,
				       IrtRType Z,
				       VoidPtr ClbkData);
typedef IrtImgImageType (*IRndrImgGetTypeFuncType)(const char *ImageType);
typedef MiscWriteGenInfoStructPtr (*IRndrImgOpenFuncType)(
						      const char **argv,
						      const char *FName,
						      IrtImgImageType ImgType,
						      int Alpha,
						      int XSize,
						      int YSize);
typedef void (*IRndrImgWriteLineFuncType)(MiscWriteGenInfoStructPtr GI,
					  IrtBType *Alpha,
					  IrtImgPixelStruct *Pixels);
typedef void (*IRndrImgCloseFuncType)(MiscWriteGenInfoStructPtr GI);

/* Colors for visibility map. */
extern const IrtImgPixelStruct RNDR_VISMAP_EMPTY_COLOR;
extern const IrtImgPixelStruct RNDR_VISMAP_MAPPED_COLOR;
extern const IrtImgPixelStruct RNDR_VISMAP_VISIBLE_COLOR;
extern const IrtImgPixelStruct RNDR_VISMAP_TANGENT_COLOR;
extern const IrtImgPixelStruct RNDR_VISMAP_POOR_AR_COLOR;
extern const IrtImgPixelStruct RNDR_VISMAP_DEGEN_COLOR;
extern const IrtImgPixelStruct RNDR_VISMAP_NOT_RENDERED_COLOR;

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/* Construct a new IRndrer. */
IRndrPtrType IritRndrInitialize(int SizeX,
				int SizeY,
				int SuperSampSize,
				int ColorQuantization,
				IrtBType UseTransparency,
				IrtBType BackfaceCulling,
				IRndrColorType BackgrCol,
				IrtRType AmbientLight,
				int VisMap);

/* Dispose of an IRender. */
void IritRndrDestroy(IRndrPtrType Rend);

/* Clearing functions. */
void IritRndrClearDepth(IRndrPtrType Rend, IRndrZDepthType ClearZ);
void IritRndrClearStencil(IRndrPtrType Rend);
void IritRndrClearColor(IRndrPtrType Rend);

/* Specify light sources (must be called only before any scan conversion
			  takes	place).	*/
void IritRndrAddLightSource(IRndrPtrType Rend,
			    IRndrLightType Type,
			    IrtPtType Where,
			    IRndrColorType Color);

/* Configure IRndrer (at initialization or on-the-fly). */
void IritRndrSetFilter(IRndrPtrType Rend,
		       char *FilterName);
IRndrShadingType IritRndrSetShadeModel(IRndrPtrType Rend,
				       IRndrShadingType	ShadeModel);
void IritRndrSetViewPrsp(IRndrPtrType Rend,
			 IrtHmgnMatType	ViewMat,
			 IrtHmgnMatType	PrspMat,
			 IrtHmgnMatType	ScrnMat);
void IRndrGetViewPrsp(IRndrPtrType Rend,
		      IrtHmgnMatType ViewMat,
		      IrtHmgnMatType PrspMat,
		      IrtHmgnMatType ScrnMat);
void IritRndrSetPllParams(IRndrPtrType Rend,
			  IrtRType MinWidth,
			  IrtRType MaxWidth,
			  IrtRType ZNear,
			  IrtRType ZFar);
IrtBType IritRndrSetRawMode(IRndrPtrType Rend, IrtBType UseRawMode);
IRndrZCmpPolicyFuncType IritRndrSetZCmpPolicy(IRndrPtrType Rend,
					      IRndrZCmpPolicyFuncType ZCmpPol);
IRndrZBufferCmpType IritRndrSetZCmp(IRndrPtrType Rend,
				    IRndrZBufferCmpType ZCmp);
IRndrPixelClbkFuncType IritRndrSetPreZCmpClbk(IRndrPtrType Rend,
					      IRndrPixelClbkFuncType PixelClbk);
void IritRndrSetPostZCmpClbk(IRndrPtrType Rend,
			     IRndrPixelClbkFuncType ZPassClbk,
			     IRndrPixelClbkFuncType ZFailClbk);

/* Stencil operations, OpenGL-style. */
void IritRndrStencilCmpFunc(IRndrPtrType Rend,
			    IRndrStencilCmpType	SCmp,
			    int	Ref,
			    unsigned int Mask);
void IritRndrStencilOp(IRndrPtrType Rend,
		       IritRndrStencilOpType Fail,
		       IritRndrStencilOpType ZFail,
		       IritRndrStencilOpType ZPass);

/* Object scan conversion. */
void IritRndrBeginObject(IRndrPtrType Rend,
			 IPObjectStruct	*Object,
			 int NoShading);
void IritRndrPutTriangle(IRndrPtrType Rend, IPPolygonStruct *Triangle);
void IritRndrEndObject(IRndrPtrType Rend);

/* Polyline scan conversion. */
void IritRndrBeginPll(IRndrPtrType Rend);
void IritRndrPutPllVertex(IRndrPtrType Rend, IPVertexStruct *Vertex);
void IritRndrEndPll(IRndrPtrType Rend);

/* "Manual" pixel rendering. */
void IritRndrPutPixel(IRndrPtrType Rend,
		      int x,
		      int y,
		      IrtRType z,
		      IrtRType Transparency,
		      IRndrColorType Color,
		      IPPolygonStruct *Triangle);

/* Z Buffer Access - Pixel Resolution. */
void IritRndrGetPixelColorAlpha(IRndrPtrType Rend,
				int x,
				int y,
				IRndrColorType *Result);
void IritRndrGetPixelDepth(IRndrPtrType Rend,
			   int x,
			   int y,
			   IrtRType *Result);
void IritRndrGetPixelStencil(IRndrPtrType Rend,
			     int x,
			     int y,
			     int *Result);

/* Z Buffer Access - Line Resolution. */
void IritRndrGetLineColorAlpha(IRndrPtrType Rend,
			       int y,
			       IRndrColorType *Result);
void IritRndrGetLineDepth(IRndrPtrType Rend, int y, IrtRType *Result);
void IritRndrGetLineStencil(IRndrPtrType Rend, int y, int *Result);

/* Clipping support. */
void IritRndrGetClippingPlanes(IRndrPtrType Rend, IrtPlnType *ClipPlanes);
void IritRndrSetZBounds(IRndrPtrType Rend, IrtRType ZNear, IrtRType ZFar);

/* Dump z-buffer contents to a file. */
void IritRndrSaveFileCB(IRndrPtrType Rend,
			IRndrImgGetTypeFuncType	ImgSetType,
			IRndrImgOpenFuncType ImgOpen,
			IRndrImgWriteLineFuncType ImgWriteLine,
			IRndrImgCloseFuncType ImgClose);
void IritRndrSaveFile(IRndrPtrType Rend,
		      const char *BaseDirectory,
		      const char *OutFileName,
		      const char *Type);
void IritRndrSaveFileDepth(IRndrPtrType Rend,
			   const char *BaseDirectory,
			   const char *OutFileName,
			   const char *Type);
void IritRndrSaveFileStencil(IRndrPtrType Rend,
			     const char	*BaseDirectory,
			     const char	*OutFileName,
			     const char	*Type);
void IritRndrSaveFileVisMap(IRndrPtrType Rend,
			    const char *BaseDirectory,
			    const char *OutFileName,
			    const char *Type);
struct IRndrSceneStruct *IritRndrGetScene(IRndrPtrType Rend); 

/* The 1D Z Buffer implementation. */

IRndrZBuffer1DPtrType IritRndr1DInitialize(int ZBuf1DSize,
					   IrtRType XMin,
					   IrtRType XMax,
					   IrtRType ZMin,
					   IrtRType ZMax,
					   int BottomMaxZ);
void IritRndr1DClearDepth(IRndrZBuffer1DPtrType Rend, IrtRType ClearZ);
IRndrZBufferCmpType IritRndr1DSetZCmp(IRndrZBuffer1DPtrType Rend,
				      IRndrZBufferCmpType ZCmp);
void IritRndr1DDestroy(IRndrZBuffer1DPtrType Rend);
void IritRndr1DPutPolyline(IRndrZBuffer1DPtrType Rend, IPPolygonStruct *Pl);
void IritRndr1DPutLine(IRndrZBuffer1DPtrType Rend,
		       IrtRType	x1,
		       IrtRType	z1,
		       IrtRType	x2,
		       IrtRType	z2);
void IritRndr1DPutPixel(IRndrZBuffer1DPtrType Rend, int x, IrtRType z);
void IritRndr1DGetPixelDepth(IRndrZBuffer1DPtrType Rend,
			     int x,
			     IrtRType *Result);
void IritRndr1DGetLineDepth(IRndrZBuffer1DPtrType Rend,
			    int	x1,
			    int	x2,
			    IrtRType *ZValues);
IPPolygonStruct *IritRndr1DUpperEnvAsPolyline(IRndrZBuffer1DPtrType Rend,
					      int MergeInters);
IPPolygonStruct *IRndr1DFilterCollinearEdges(IRndrZBuffer1DPtrType Rend,
					     IPPolygonStruct *Pl,
					     int MergeInters);

/* The NC Z Buffer implementation. */

INCZBufferPtrType IritRndrIncInitialize(int ZBufSizeX,
					int ZBufSizeY,
					int GridSizeX,
					int GridSizeY,
					IrtPtType XYZMin,
					IrtPtType XYZMax,
					int BottomMaxZ);
IRndrZBufferCmpType IritRndrIncSetZCmp(INCZBufferPtrType Rend,
				       IRndrZBufferCmpType ZCmp);
void IritRndrIncDestroy(INCZBufferPtrType Rend);
void IritRndrIncBeginObject(INCZBufferPtrType Rend, IPObjectStruct *Object);
void IritRndrIncPutTriangle(INCZBufferPtrType Rend, IPPolygonStruct *Triangle);
IrtRType IritRndrIncPutMask(INCZBufferPtrType Rend,
			    int	*PosXY,
			    IrtRType PosZ,
			    IrtRType *Mask,
			    int	MaskXSize,
			    int	MaskYSize);
void IritRndrIncEndObject(INCZBufferPtrType Rend);
void IritRndrIncPutPixel(INCZBufferPtrType Rend, int x, int y, IrtRType z);
void IritRndrIncGetPixelDepth(INCZBufferPtrType Rend,
			      int x,
			      int y,
			      IrtRType *Result);
void IritRndrIncGetLineDepth(INCZBufferPtrType Rend,
			     int x1,
			     int x2,
			     int y,
			     IrtRType *ZValues);
void IritRndrIncGetZbufferGridCellMaxSize(INCZBufferPtrType Rend,
					  int *GridSizeX,
					  int *GridSizeY,
					  int *GridCellXSize,
					  int *GridCellYSize);
int IritRndrIncGetZbufferGridCell(INCZBufferPtrType Rend,
				  int GridCellX,
				  int GridCellY,
				  IrtRType *ZValues,
				  int *XMin,
				  int *YMin,
				  int *XMax,
				  int *YMax);
int IritRndrIncMapPixelsToCells(INCZBufferPtrType Rend, int *X, int *Y);
int IritRndrIncGetActiveCells(INCZBufferPtrType Rend,
			      int *MinCellX,
			      int *MinCellY,
			      int *MaxCellX,
			      int *MaxCellY,
			      IrtRType *ZPixelsRemoved);
int IRndrVisMapEnable(IRndrPtrType Rend,
		      IPObjectStruct* Objects,
		      int SuperSize,
		      int UVBackfaceCulling);
void IRndrVisMapScan(IRndrPtrType Rend);
void IRndrVisMapSetTanAngle(IRndrPtrType Rend, IrtRType CosAng);
void IRndrVisMapSetCriticAR(IRndrPtrType Rend, IrtRType CriticAR);
void IRndrVisMapSetDilation(IRndrPtrType Rend, int Dilation);
int IritRndrVisMapGetObjDomain(IPObjectStruct *PObj, 
			       IrtRType	*UMin, 
			       IrtRType	*UMax, 
			       IrtRType	*VMin, 
			       IrtRType	*VMax);
int IRndrVisMapPrepareUVValuesOfGeoObj(IPObjectStruct *PObj, 
				       int MapWidth, 
				       int MapHeight,
				       IPObjectStruct *PObj2);
void IRndrVisMapSetScanOnUV(IRndrPtrType Rend, int IsSacnOnUV);
void IRndrIritRndrVertexTransform(IRndrPtrType Rend,
				  IPVertexStruct *Vertex,
				  IrtRType *Result);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* IRIT_RNDR_LIB_H */
