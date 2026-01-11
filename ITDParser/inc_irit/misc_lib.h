/******************************************************************************
* Misc_lib.h - header file for the misc. library.			      *
* This header is also the interface header to the world.		      *
*******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute	of Technology		      *
*******************************************************************************
* Written by Gershon Elber, Oct. 94.					      *
******************************************************************************/

#ifndef IRIT_MISC_LIB_H
#define IRIT_MISC_LIB_H

#include <stdio.h>
#ifdef __WINCE__
#   include <memory.h>
#endif
#ifdef __WINNT__
#    include <malloc.h> 
#endif

#include "inc_irit/irit_sm.h"

typedef enum {
    MISC_ERR_MALLOC_FAILED,
    MISC_ERR_CONFIG_FILE_NO_FOUND,
    MISC_ERR_CONFIG_ERROR,
    MISC_ERR_UNKNOWN_CONFIG,

    MISC_ERR_UNDEFINE_ERR
} IritMiscFatalErrorType;

typedef enum {
    IC_NONE_TYPE = 0,
    IC_BOOLEAN_TYPE,
    IC_INTEGER_TYPE,
    IC_REAL_TYPE,
    IC_STRING_TYPE,
    IC_MULTI_STR_TYPE
} IrtCfgDataType;

typedef enum {
    IRIT_IMAGE_UNKNOWN_TYPE,
    IRIT_IMAGE_RLE_TYPE,
    IRIT_IMAGE_PPM3_TYPE,
    IRIT_IMAGE_PPM6_TYPE,
    IRIT_IMAGE_PNG_TYPE
} IrtImgImageType;

typedef enum {
    MISC_ISC_BNW,
    MISC_ISC_GRAY
} MiscISCColorTypeEnum;

typedef enum {
    IRIT_IMAGE_DITHER_REG_FL_TS = 0,
    IRIT_IMAGE_DITHER_STUCKI,
    IRIT_IMAGE_DITHER_FL_ST,
    IRIT_IMAGE_DITHER_JAR_JUD,
    IRIT_IMAGE_DITHER_BURKES,
    IRIT_IMAGE_DITHER_SIERRA1,
    IRIT_IMAGE_DITHER_SIERRA2,
    IRIT_IMAGE_DITHER_SIERRA3,
    IRIT_IMAGE_DITHER_LAST
} IrtImgImageDitherType;

typedef int (*MiscISCPrintFuncType)(const char *Format, ...);

/*****************************************************************************
* Simple expression trees parser - have	variables 'A' to 'Z'.		     *
*****************************************************************************/

enum {
    IRIT_E2T_ABS = 10,          /* Functions. */
    IRIT_E2T_ARCCOS,
    IRIT_E2T_ARCSIN,
    IRIT_E2T_ARCTAN,
    IRIT_E2T_COS,
    IRIT_E2T_EXP,
    IRIT_E2T_LN,
    IRIT_E2T_LOG,
    IRIT_E2T_SIN,
    IRIT_E2T_SQR,
    IRIT_E2T_SQRT,
    IRIT_E2T_TAN,

    IRIT_E2T_PLUS = 30,         /* Operators. */
    IRIT_E2T_MINUS,
    IRIT_E2T_MULT,
    IRIT_E2T_DIV,
    IRIT_E2T_POWER,
    IRIT_E2T_UNARMINUS,

    IRIT_E2T_OPENPARA = 40,     /* Paranthesis. */
    IRIT_E2T_CLOSPARA,

    IRIT_E2T_NUMBER = 50,       /* Numbers (or parameter t). */
    IRIT_E2T_PARAMETER,

    IRIT_E2T_TOKENERROR = -1,
    IRIT_E2T_TOKENSTART = 100,
    IRIT_E2T_TOKENEND,

    IRIT_E2T_MAX_PARAM_IDX = 'Z' - 'A'
};

enum {
    IRIT_E2T_PARAM_ALL = -1,       /* Match all IRIT_E2T_PARAMs is searches. */
    IRIT_E2T_PARAM_A = 0,
    IRIT_E2T_PARAM_B,
    IRIT_E2T_PARAM_C,
    IRIT_E2T_PARAM_D,
    IRIT_E2T_PARAM_E,
    IRIT_E2T_PARAM_F,
    IRIT_E2T_PARAM_G,
    IRIT_E2T_PARAM_H,
    IRIT_E2T_PARAM_I,
    IRIT_E2T_PARAM_J,
    IRIT_E2T_PARAM_K,
    IRIT_E2T_PARAM_L,
    IRIT_E2T_PARAM_M,
    IRIT_E2T_PARAM_N,
    IRIT_E2T_PARAM_O,
    IRIT_E2T_PARAM_P,
    IRIT_E2T_PARAM_Q,
    IRIT_E2T_PARAM_R,
    IRIT_E2T_PARAM_S,
    IRIT_E2T_PARAM_T,
    IRIT_E2T_PARAM_U,
    IRIT_E2T_PARAM_V,
    IRIT_E2T_PARAM_W,
    IRIT_E2T_PARAM_X,
    IRIT_E2T_PARAM_Y,
    IRIT_E2T_PARAM_Z,
    IRIT_E2T_PARAM_NUM_PARAM
};

/* Error numbers during expression trees parsing/derivative process. */

enum {
    IRIT_E2T_UNDEF_TOKEN_ERROR = 1,
    IRIT_E2T_PARAMATCH_ERROR,
    IRIT_E2T_EOL_ERROR,
    IRIT_E2T_REAL_MESS_ERROR,
    IRIT_E2T_SYNTAX_ERROR,
    IRIT_E2T_STACK_OV_ERROR,
    IRIT_E2T_ONE_OPERAND_ERROR,
    IRIT_E2T_TWO_OPERAND_ERROR,
    IRIT_E2T_PARAM_EXPECT_ERROR,
    IRIT_E2T_DERIV_NONE_CONST_EXP_ERROR,
    IRIT_E2T_DERIV_NO_ABS_DERIV_ERROR
};

#define IRIT_MISC_IF_MSECS_FROM_LAST_CALL(MiliSecs) \
    IRIT_STATIC_DATA IrtRType \
	PrevTime = -MiliSecs - 1.0; \
 \
    if (IritMiscSecsFromLastCall(MiliSecs, &PrevTime))

typedef struct IritE2TExprNodeStruct {
     struct IritE2TExprNodeStruct *Right, *Left;
     int NodeKind;
     IrtRType RData;
     char *SData;
} IritE2TExprNodeStruct;

typedef struct MiscExprTreeGenInfoStruct *MiscExprTreeGenInfoStructPtr;
typedef struct MiscWriteGenInfoStruct *MiscWriteGenInfoStructPtr;

typedef IrtRType (*IritE2TExprNodeParamFuncType)(
					    MiscExprTreeGenInfoStructPtr GI,
					    const char *ParamName);

typedef struct IritConfigStruct {
    const char *VarName;
    const char *SomeInfo;
    VoidPtr VarData;
    IrtCfgDataType VarType;
} IritConfigStruct;

typedef struct IrtImgPixelStruct {
    IrtBType r, g, b;
} IrtImgPixelStruct;

typedef struct IrtImgRGBAPxlStruct {
    IrtBType r, g, b, a;
} IrtImgRGBAPxlStruct;

typedef float IrtImgRealClrType;
typedef struct IrtImgRealPxlStruct {
    IrtImgRealClrType r, g, b, a;
} IrtImgRealPxlStruct;

typedef struct IritImgPrcssImgStruct {
    unsigned int Height, Width;
    float *Data; /* colors */
} IritImgPrcssImgStruct;

typedef void (*IritImgPrcssFunctionOnPixel)(IritImgPrcssImgStruct *Image,
			                    unsigned int i,
			                    unsigned int j,
			                    void *Data);

typedef struct IritHashElementStruct {
    struct IritHashElementStruct *Pnext;
    VoidPtr Data;
    IrtRType Key;
} IritHashElementStruct;

typedef struct IritHashTableStruct {
    IrtRType MinKeyVal, MaxKeyVal, DKey, KeyEps;
    IritHashElementStruct **Vec;
    int VecSize;
    VoidPtr AuxData;
} IritHashTableStruct;

typedef struct IritBiPrWeightedMatchStruct {
    int m1, m2, m3;
} IritBiPrWeightedMatchStruct;

typedef struct IritIntrvlArithStruct {
    IrtRType Max;
    IrtRType Min;
} IritIntrvlArithStruct;

typedef struct MiscISCCalculatorStruct* MiscISCCalculatorPtrType;
typedef struct MiscListStruct *MiscListPtrType;
typedef struct MiscListIteratorStruct *MiscListIteratorPtrType;

typedef unsigned char MiscISCPixelType;
typedef unsigned long MiscISCImageSizeType;

typedef void (*IritFatalMsgFuncType)(const char *Msg);
typedef void (*IritWarningMsgFuncType)(const char *Msg);
typedef void (*IritInfoMsgFuncType)(const char *Msg);
typedef void (*IritInfoWMsgFuncType)(const wchar_t *Msg);
typedef void (*IritMiscSetErrorFuncType)(IritMiscFatalErrorType ErrorFunc,
				         const char *ErrorDescription);

typedef int (*IritMiscPQCompFuncType)(VoidPtr p1, VoidPtr p2);/* Comparison func.*/
typedef int (*IritHashCmpFuncType)(VoidPtr Data1, VoidPtr Data2, VoidPtr AuxData);
typedef void IritLevenEvalFuncType(IrtRType *CurPoint,
				   IrtRType ModelParams[],
				   IrtRType *YPointer,
				   IrtRType YdParams[]);
typedef void IritLevenNumerProtectionFuncType(IrtRType InternalModelParams[]);
typedef int IritLevenIsModelValidFuncType(IrtRType InternalModelParams[]);

/* Return hash value (KeySizeBits length) of the element Elem. */
typedef unsigned long (*MiscHashFuncType)(void * Elem, 
					  unsigned long ElementParam, 
					  unsigned int KeySizeBits);
/* Create copy of the Elem. Used only for adding element to the table. */
typedef void *(*MiscHashCopyFuncType)(void *Elem, unsigned long ElementParam);
/* Free Elem. */
typedef void (*MiscHashFreeFuncType)(void *Elem);
/* Return 0 if Elem1 equals Elem2. 1 otherwise. */
typedef int (*MiscHashCompFuncType)(void *Elem1, 
                                    void *Elem2, 
                                    unsigned long ElementParam);

typedef struct MiscHashTableStruct *MiscHashPtrType;

struct IritPriorQueStruct;

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/* Basic dynamic memory allocation routines: */
#ifdef DEBUG_IRIT_MALLOC
VoidPtr IritMalloc(unsigned int Size,
		   const char *ObjType,
		   const char *FileName,
		   int LineNum);
#define IritMalloc(x)	IritMalloc((x), #x, __FILE__, __LINE__)
void IritFree(VoidPtr p);
#define IritFree(x)	{ IritFree((void *) x); x = NULL; }
void IritMiscDynMemoryDbgTestAll(void);
void IritMiscDynMemoryDbgInitTest(void);
void IritMiscDynMemoryDbgInitTest2(int DebugMalloc, int DebugSearchAllocID);
void IritMiscDynMemoryDbgCheckMark(IrtRType *Start,
			       IrtRType *KeepStackStep,
			       IrtRType *TrackAllocID);
void IritMiscDynMemoryDbgMallocSearchID(int ID);
void *IritDynMemoryDbgNoReport(void *p);
/* A debug macro to state this allocation is global and made once. */
#define IRIT_MALLOC_DBG_NO_REPORT(x)  IritDynMemoryDbgNoReport(x)
#else
VoidPtr IritMallocCheckNULL(unsigned Size);
#define IritMalloc(Size)	IritMallocCheckNULL(Size)
#define IritFree(Ptr)		free(Ptr)
#define IRIT_MALLOC_DBG_NO_REPORT(x)   (x)
#endif /* DEBUG_IRIT_MALLOC */
VoidPtr IritMiscRealloc(VoidPtr p, unsigned int OldSize, unsigned int NewSize);
void IritMiscFree2UnixFree(void *p);

#ifdef __WINNT__
#   ifdef DEBUG
#define IRIT_MAX_ALLOCA_SIZE	10000
#define IritAlloca(Size) _alloca((Size) > IRIT_MAX_ALLOCA_SIZE ? \
					     assert(0), (Size) : (Size))
#   else
#define IritAlloca(Size) _alloca(Size) 
#   endif /* DEBUG */
#else
#define IritAlloca(Size) alloca(Size)
#endif /* __WINNT__ */

/* Prototype of the configuration routines: */
const char *IritConfig(const char *PrgmName,
		       const IritConfigStruct *SetUp,
		       int NumVar,
		       char *FoundName);
void IritConfigPrint(const IritConfigStruct *SetUp, int NumVar);
int IritConfigSave(const char *FileName,
		   const IritConfigStruct *SetUp,
		   int NumVar);
void IritConfigInitState(void);

/* Get command line arguments. */
#ifdef USE_VARARGS
int IritMiscGetArgs(int va_alist, ...);
#else
int IritMiscGetArgs(int argc, ...);
#endif /* USE_VARARGS */
char *IritMiscStringErrMsg(int Error, char *OutStr);
void IritMiscPrintErrMsg(int Error);
char *IritMiscStringHowTo(const char *CtrlStr, char *OutStr);
void IritMiscPrintHowTo(const char *CtrlStr);

/* Homogeneous 4x4 matrix routines. */
void IritMiscMatGenUnitMat(IrtHmgnMatType Mat);
int IritMiscMatIsUnitMatrix(IrtHmgnMatType Mat, IrtRType Eps);
int MatIsWeightAffected(IrtHmgnMatType Mat, IrtRType Eps);
void IritMiscMatGenMatTrans(IrtRType Tx,
			    IrtRType Ty,
			    IrtRType Tz,
			    IrtHmgnMatType Mat);
void IritMiscMatGenMatUnifScale(IrtRType Scale, IrtHmgnMatType Mat);
void IritMiscMatGenMatUnifScaleCenter(IrtRType Scl,
				      const IrtPtType Center,
				      IrtHmgnMatType Mat);
void IritMiscMatGenMatScale(IrtRType Sx,
			    IrtRType Sy,
			    IrtRType Sz,
			    IrtHmgnMatType Mat);
void IritMiscMatGenMatRotX1(IrtRType Teta,
			    IrtHmgnMatType Mat);
void IritMiscMatGenMatRotX(IrtRType CosTeta,
			   IrtRType SinTeta,
			   IrtHmgnMatType Mat);
void IritMiscMatGenMatRotY1(IrtRType Teta, IrtHmgnMatType Mat);
void IritMiscMatGenMatRotY(IrtRType CosTeta,
			   IrtRType SinTeta, IrtHmgnMatType Mat);
void IritMiscMatGenMatRotZ1(IrtRType Teta, IrtHmgnMatType Mat);
void IritMiscMatGenMatRotZ1Center(IrtRType Teta,
				  const IrtPtType Center,
				  IrtHmgnMatType Mat);
void IritMiscMatGenMatRotZ(IrtRType CosTeta,
			   IrtRType SinTeta,
			   IrtHmgnMatType Mat);

void IritMiscMatMultTwo4by4(IrtHmgnMatType MatRes,
			    IrtHmgnMatType Mat1,
			    IrtHmgnMatType Mat2);
void IritMiscMatAddTwo4by4(IrtHmgnMatType MatRes,
			   IrtHmgnMatType Mat1,
			   IrtHmgnMatType Mat2);
void IritMiscMatSubTwo4by4(IrtHmgnMatType MatRes,
			   IrtHmgnMatType Mat1,
			   IrtHmgnMatType Mat2);
void IritMiscMatScale4by4(IrtHmgnMatType MatRes,
			  IrtHmgnMatType Mat,
			  const IrtRType *Scale);
int IritMiscMatSameTwo4by4(IrtHmgnMatType Mat1,
			   IrtHmgnMatType Mat2,
			   IrtRType Eps);

void IritMiscMatMultVecby4by4(IrtVecType VecRes,
			      const IrtVecType Vec,
			      IrtHmgnMatType Mat);
void IritMiscMatMultPtby4by4(IrtPtType PtRes,
			     const IrtPtType Pt,
			     IrtHmgnMatType Mat);
void IritMiscMatMultWVecby4by4(IrtRType VRes[4],
			       const IrtRType Vec[4],
			       IrtHmgnMatType Mat);

IrtRType IritMiscMatDeterminantMatrix(IrtHmgnMatType Mat);
int IritMiscMatInverseMatrix(IrtHmgnMatType M, IrtHmgnMatType InvM);
void IritMiscMatTranspMatrix(IrtHmgnMatType M, IrtHmgnMatType TranspM);
IrtRType IritMiscMatScaleFactorMatrix(IrtHmgnMatType M);
IrtRType *IritMiscMatScaleFactorMatrix2(IrtHmgnMatType M, IrtVecType ScaleVec);
void IritMiscMatRotateFactorMatrix(IrtHmgnMatType M, IrtHmgnMatType RotMat);
void IritMiscMatRotSclFactorMatrix(IrtHmgnMatType M, IrtHmgnMatType RotSclMat);
void IritMiscMatTranslateFactorMatrix(IrtHmgnMatType M, IrtVecType Trans);

/* General matrix routines. */
void IritMiscMatGnrlCopy(IrtGnrlMatType Dst, IrtGnrlMatType Src, int n);
void IritMiscMatGnrlUnitMat(IrtGnrlMatType Mat, int n);
int IritMiscMatGnrlIsUnitMatrix(IrtGnrlMatType Mat, IrtRType Eps, int n);
void IritMiscMatGnrlMultTwoMat(IrtGnrlMatType MatRes,
			       IrtGnrlMatType Mat1,
			       IrtGnrlMatType Mat2,
			       int n);
void IritMiscMatGnrlAddTwoMat(IrtGnrlMatType MatRes,
			      IrtGnrlMatType Mat1,
			      IrtGnrlMatType Mat2,
			      int n);
void IritMiscMatGnrlSubTwoMat(IrtGnrlMatType MatRes,
			      IrtGnrlMatType Mat1,
			      IrtGnrlMatType Mat2,
			      int n);
void IritMiscMatGnrlScaleMat(IrtGnrlMatType MatRes,
			     IrtGnrlMatType Mat,
			     IrtRType *Scale,
			     int n);
void IritMiscMatGnrlMultVecbyMat(IrtVecGnrlType VecRes,
				 IrtGnrlMatType Mat,
				 IrtVecGnrlType Vec,
				 int n);
void IritMiscMatGnrlMultVecbyMat2(IrtVecGnrlType VecRes,
				  IrtVecGnrlType Vec,
				  IrtGnrlMatType Mat,
				  int n);
int IritMiscMatGnrlSolveLinSys(IrtGnrlMatType M,
			       IrtVecGnrlType x,
			       const IrtVecGnrlType B,
			       int n);
int IritMiscMatGnrlInverseMatrix(IrtGnrlMatType M,
				 IrtGnrlMatType InvM,
				 int n);
void IritMiscMatGnrlTranspMatrix(IrtGnrlMatType M,
				 IrtGnrlMatType TranspM,
				 int n);
IrtRType IritMiscMatGnrlDetMatrix(IrtGnrlMatType M, int n);
int IritMiscMatGnrlOrthogonalSubspace(IrtGnrlMatType M, int n);
void IritMiscMatGnrlPrintMatrix(IrtGnrlMatType M, int n, FILE *F);
IrtRType IritMiscMat2x2Determinant(IrtRType a11,
				   IrtRType a12, 
				   IrtRType a21,
				   IrtRType a22);
IrtRType IritMiscMat3x3Determinant(IrtRType a11,
				   IrtRType a12,
				   IrtRType a13,
				   IrtRType a21,
				   IrtRType a22,
				   IrtRType a23,
				   IrtRType a31,
			           IrtRType a32,
				   IrtRType a33);

/* QR matrix factorization. */
int IritMiscQRFactorization(IrtRType *A,
			    int n,
			    int m,
			    IrtRType *Q,
			    IrtRType *R);
int IritMiscSolveUpperDiagMatrix(const IrtRType *A,
				 int n,
				 const IrtRType *b,
				 IrtRType *x);
int IritMiscSolveLowerDiagMatrix(const IrtRType *A,
				 int n,
				 const IrtRType *b,
				 IrtRType *x);
int IritMiscQRUnderdetermined(IrtRType *A,
			      IrtRType *x,
			      const IrtRType *b,
			      int m,
			      int n,
			      void **QRCache);
int IritMiscQRUnderdetermined2(IrtRType *A,
			       IrtRType *x,
			       const IrtRType *b,
			       int m,
			       int n);

/* Gauss Jordan matrix solver and Levenberg Marquardt local minimum finder. */
int IritMiscGaussJordan(IrtRType *A,
			IrtRType *B,
			unsigned int N,
			unsigned int M);
IrtRType IritMiscLevenMarMin(IrtRType **x,
			     IrtRType y[],
			     IrtRType Sigma[],
			     unsigned int NumberOfDataElements,
			     IrtRType ModelParams[],
			     IritLevenEvalFuncType *ShapeFunc,
			     IritLevenNumerProtectionFuncType *ProtectionFunc,
			     IritLevenIsModelValidFuncType *ModelValidatorFunc,
			     unsigned int NumberOfModelParams,
			     IrtRType Tolerance);
IrtRType IritMiscLevenMarMinSig1(IrtRType **XVals,
			     IrtRType YVals[],
			     unsigned int NumberOfDataElements,
			     IrtRType ModelParams[],
			     IritLevenEvalFuncType *ShapeFunc,
			     IritLevenNumerProtectionFuncType *ProtectionFunc,
			     IritLevenIsModelValidFuncType *ModelValidatorFunc,
			     unsigned int NumberOfMedelParams,
			     IrtRType Tolerance);
unsigned int IritMiscLevenMarSetMaxIterations(unsigned int NewVal);

/* An implementation of a priority queue. */
void IritMiscPQInit(struct IritPriorQueStruct **PQ,
		    IritMiscPQCompFuncType CompFunc);
int IritMiscPQEmpty(struct IritPriorQueStruct *PQ);
IritMiscPQCompFuncType IritMiscPQCompFunc(struct IritPriorQueStruct *PQ, 
					  IritMiscPQCompFuncType NewCompFunc);
VoidPtr IritMiscPQFirst(struct IritPriorQueStruct *PQ, int Delete);
VoidPtr IritMiscPQInsert(struct IritPriorQueStruct *PQ, VoidPtr NewItem);
VoidPtr IritMiscPQDelete(struct IritPriorQueStruct *PQ, VoidPtr OldItem);
VoidPtr IritMiscPQFindByFunc(struct IritPriorQueStruct *PQ,
			     IritMiscPQCompFuncType CompFunc,
			     VoidPtr CompItem);
VoidPtr IritMiscPQFind(struct IritPriorQueStruct *PQ, VoidPtr OldItem);
VoidPtr IritMiscPQNext(struct IritPriorQueStruct *PQ,
		       VoidPtr CmpItem,
		       VoidPtr BiggerThan);
int IritMiscPQSize(struct IritPriorQueStruct *PQ);
void IritMiscPQPrint(struct IritPriorQueStruct *PQ,
		     void (*PrintFunc)(VoidPtr));
void IritMiscPQFree(struct IritPriorQueStruct *PQ, int FreeItems);
void IritMiscPQFreeFunc(struct IritPriorQueStruct *PQ,
			void (*FreeFunc)(VoidPtr));

/* An implementation of a hashing table. */
IritHashTableStruct *IritMiscHashTableCreate(IrtRType MinKeyVal,
					     IrtRType MaxKeyVal,
					     IrtRType KeyEps,
					     int VecSize);
int IritMiscHashTableInsert(IritHashTableStruct *IHT,
			    VoidPtr Data,
			    IritHashCmpFuncType HashCmpFunc,
			    IrtRType Key,
			    int RplcSame);
VoidPtr IritMiscHashTableFind(IritHashTableStruct *IHT,
			      VoidPtr Data,
			      IritHashCmpFuncType HashCmpFunc,
			      IrtRType Key);
int IritMiscHashTableRemove(IritHashTableStruct *IHT,
			    VoidPtr Data,
			    IritHashCmpFuncType HashCmpFunc,
			    IrtRType Key);
void IritMiscHashTableFree(IritHashTableStruct *IHT);

/* Another implementation of a hashing table. */

/* Call back functions of the hash data structure. */
/* SizeInByte is a parameter given by the  user and propagated to each       */
/* function. The function may use it for its action (its name origin from    */
/* one use as a length of an array element).                                 */

MiscHashPtrType IritMiscHashNewHash(unsigned long HashSize,
				    MiscHashFuncType HashFunc,
				    MiscHashCopyFuncType CopyFunc,
				    MiscHashFreeFuncType FreeFunc,
				    MiscHashCompFuncType CompFunc);
int IritMiscHashAddElement(MiscHashPtrType Hash, 
			   void *Elem, 
			   unsigned long SizeInByte);
int IritMiscHashFindElement(MiscHashPtrType Hash, 
			    void *Elem, 
			    unsigned long SizeInByte);
long *IritMiscHashGetElementAuxData(MiscHashPtrType Hash,
				    void *Elem,
				    unsigned long SizeInByte);
void IritMiscHashFreeHash(MiscHashPtrType Hash);

/* An implementation of a list data structure. */

/* Call back functions of the list data structure to copy, free and compare */
/* elements (zero if equal).  Last param can be used to pass array size.    */
typedef void *(*MiscListCopyFuncType)(void *elmt, unsigned long size);
typedef void (*MiscListFreeFuncType)(void *elmt);
typedef int (*MiscListCompFuncType)(void *e1mt1, void *elmt2, unsigned long l);

MiscListPtrType IritMiscListNewEmptyList(MiscListCopyFuncType CopyFunc,
					 MiscListFreeFuncType FreeFunc,
					 MiscListCompFuncType CompFunc);

int IritMiscListAddElement(MiscListPtrType List, 
			   void *Elem, 
			   unsigned long SizeInByte);
int IritMiscListFindElementInList(MiscListPtrType List, 
				  void *Elem, 
				  unsigned long SizeInByte);
int IritMiscListCompLists(MiscListPtrType L1, MiscListPtrType L2);
void IritMiscListFreeList(MiscListPtrType List);
MiscListIteratorPtrType IritMiscListGetListIterator(MiscListPtrType List);
void IritMiscListFreeListIterator(MiscListIteratorPtrType It);
void *IritMiscListIteratorFirst(MiscListIteratorPtrType It);
void *IritMiscListIteratorNext(MiscListIteratorPtrType It);
int IritMiscListIteratorAtEnd(MiscListIteratorPtrType It);
void *IritMiscListIteratorValue(MiscListIteratorPtrType It);

/* Interval arithmetic. */
void IritMiscIntrvlArithAdd(const IritIntrvlArithStruct *A,
			    const IritIntrvlArithStruct *B,
			    IritIntrvlArithStruct *Result);
void IritMiscIntrvlArithSub(const IritIntrvlArithStruct *A,
			    const IritIntrvlArithStruct *B,
			    IritIntrvlArithStruct *Result);
void IritMiscIntrvlArithMult(const IritIntrvlArithStruct *A,
			     const IritIntrvlArithStruct *B,
			     IritIntrvlArithStruct *Result);
void IritMiscIntrvlArithDiv(const IritIntrvlArithStruct *A,
			    const IritIntrvlArithStruct *B,
			    IritIntrvlArithStruct *Result);
void IritMiscIntrvlArithSqrt(const IritIntrvlArithStruct *A,
			     IritIntrvlArithStruct *Result);
void IritMiscIntrvlArithMultScalar(const IritIntrvlArithStruct *A,
				   IrtRType Val, 
				   IritIntrvlArithStruct *Result);
void IritMiscIntrvlArithAbs(const IritIntrvlArithStruct *A,
			    IritIntrvlArithStruct *Result);
void IritMiscIntrvlArithUnion(const IritIntrvlArithStruct *A,
			      const IritIntrvlArithStruct *B,
			      IritIntrvlArithStruct *Result);
void IritMiscIntrvlArithVAdd(const IritIntrvlArithStruct *A,
			     const IritIntrvlArithStruct *B,
			     IritIntrvlArithStruct *Result);
void IritMiscIntrvlArithVDot(const IritIntrvlArithStruct *A,
			     const IritIntrvlArithStruct *B,
			     IritIntrvlArithStruct *Result);
void IritMiscIntrvlArithVCross(const IritIntrvlArithStruct *A,
			       const IritIntrvlArithStruct *B,
			       IritIntrvlArithStruct *Result);
void IritMiscIntrvlArithVFromCone(const IrtRType *Dir,
				  IrtRType Angle,
				  IritIntrvlArithStruct *Result);
void IritMiscIntrvlArithVToCone(const IritIntrvlArithStruct *IntervalV,
				IrtRType *Dir,
				IrtRType *Angle);
void IritMiscIntrvlArithVToSphere(const IritIntrvlArithStruct *IntervalV,
				  IrtRType *Center,
				  IrtRType *Radius);
void IritMiscIntrvlArithVMultScalar(const IritIntrvlArithStruct *A,
				    IrtRType Val,
				    IritIntrvlArithStruct *Result);

/* Read/Write of images. */
int IritMiscImgReadImageXAlign(int Alignment);
IrtImgPixelStruct *IritMiscImgReadImage(const char *ImageFileName,
					int *MaxX,
					int *MaxY,
					int *Alpha);
IrtImgPixelStruct *IritMiscImgReadImage2(const char *ImageFileName,
					 int *MaxX,
					 int *MaxY,
					 int *Alpha);
IrtImgPixelStruct *IritMiscImgReadImage3(const char *ImageFileName,
					 int *MaxX,
					 int *MaxY,
					 int *Alpha);
int IritMiscImgGetImageSize(const char *ImageFileName,
			    int *Width,
			    int *Height);
int IrtImgReadUpdateCache(const char *ImageFileName,
			  int MaxX,
			  int MaxY,
			  int Alpha,
			  IrtBType *Image);
void IritMiscImgReadClrCache(void);
void IritMiscImgReadClrOneImage(const char *ImageName);
int IritMiscImgWriteImg(const char *FName,
			const IrtImgRGBAPxlStruct *Pixels,
			int XSize,
			int YSize,
			int HasAlpha);
int IritMiscImgWriteImg2(const char *FName,
			 const IrtImgPixelStruct *Pixels,
			 int XSize,
			 int YSize);
IrtImgImageType IritMiscImgWriteGetType(const char *ImageType);
MiscWriteGenInfoStructPtr IritMiscImgWriteOpenFile(const char **argv,
						   const char *FName,
						   IrtImgImageType ImageType,
						   int Alpha,
						   int XSize,
						   int YSize);
void IritMiscImgWritePutLine(MiscWriteGenInfoStructPtr GI,
			     IrtBType *Alpha,
			     IrtImgPixelStruct *Pixels);
void IritMiscImgWriteCloseFile(MiscWriteGenInfoStructPtr GI);
IrtImgRGBAPxlStruct *IritMiscImgCnvrtRGB2RGBA(const IrtImgPixelStruct *RGBImg,
					      int XSize,
					      int YSize);
IrtImgPixelStruct *IritMiscImgCnvrtRGBA2RGB(const IrtImgRGBAPxlStruct *RGBAImg,
					    int XSize,
					    int YSize);
IrtImgPixelStruct *IritMiscImgFlipXYImage(const IrtImgPixelStruct *Img,
					  int MaxX,
					  int MaxY,
					  int Alpha);
IrtImgPixelStruct *IritMiscImgNegateImage(const IrtImgPixelStruct *InImage,
					  int MaxX,
					  int MaxY);
IrtImgPixelStruct *IritMiscImgFlipHorizontallyImage(
						 const IrtImgPixelStruct *Img,
						 int MaxX,
						 int MaxY,
					       int Alpha);
IrtImgPixelStruct *IritMiscImgFlipVerticallyImage(const IrtImgPixelStruct *Img,
						  int MaxX,
						  int MaxY,
						  int Alpha);
IrtImgPixelStruct *IritMiscImgGetColorsInImage(const IrtImgPixelStruct *Img,
					       int MaxX,
					       int MaxY,
					       int Alpha,
					       int *NumClrs);
int IritMiscImgParsePTextureString(const char *PTexture,
				   char *FName,
				   IrtRType *Scale,
				   IrtRType *Trans,
				   int *Flip,
				   int *NewImage);
int IritMiscImgParsePTextureString2(const char *PTexture,
				    char *FName,
				    IrtRType *Scale,
				    IrtRType *Trans,
				    int *Flip,
				    int *NewImage,
				    int *FlipHorizontally,
				    int *FlipVertically);
IrtBType *IritMiscImgDitherImageBW(IrtImgRGBAPxlStruct *Image,
				   int XSize,
				   int YSize,
				   int DitherSize,
				   IrtBType ErrorDiffusion);
IrtBType *IritMiscImgDitherImageBW1(IrtImgPixelStruct *Image,
				    int XSize,
				    int YSize,
				    int DitherSize,
				    IrtBType ErrorDiffusion);
IrtImgRGBAPxlStruct *IritMiscImgDitherImageBW2(IrtImgRGBAPxlStruct *Image,
					       int XSize,
					       int YSize,
					       int DitherSize,
					       IrtBType ErrorDiffusion);
IrtImgRGBAPxlStruct *IritMiscImgDitherImageClr(
				    IrtImgRGBAPxlStruct *Image,
				    int XSize,
				    int YSize,
				    int DitherSize,
				    IrtBType ErrorDiffusion,
				    int NumColors,
				     const IrtImgRGBAPxlStruct *DitherColors);
int IritMiscImgDitherImage2(const char *InputImage,
			    const char *OututImage,
			    int DitherSize,
			    IrtBType ErrorDiffusion,
			    int NumColors,
			    const IrtImgRGBAPxlStruct *DitherColors);

IrtImgRGBAPxlStruct *IritMiscDtrStucki(
				      IrtImgRGBAPxlStruct *Image,
				      int XSize,
				      int YSize,
				      int NumColors,
				      const IrtImgRGBAPxlStruct *DitherColors);
IrtImgRGBAPxlStruct *IritMiscDtrFloydSteinberg(
				      IrtImgRGBAPxlStruct *Image,
				      int XSize,
				      int YSize,
				      int NumColors,
				      const IrtImgRGBAPxlStruct *DitherColors);
IrtImgRGBAPxlStruct *IritMiscDtrJarvisJudiceNinke(
				      IrtImgRGBAPxlStruct *Image,
				      int XSize,
				      int YSize,
				      int NumColors,
				      const IrtImgRGBAPxlStruct *DitherColors);
IrtImgRGBAPxlStruct *IritMiscDtrBurkes(
				      IrtImgRGBAPxlStruct *Image,
				      int XSize,
				      int YSize,
				      int NumColors,
				      const IrtImgRGBAPxlStruct *DitherColors);
IrtImgRGBAPxlStruct *IritMiscDtrSierraFirst(
				      IrtImgRGBAPxlStruct *Image,
				      int XSize,
				      int YSize,
				      int NumColors,
				      const IrtImgRGBAPxlStruct *DitherColors);
IrtImgRGBAPxlStruct *IritMiscDtrSierraSecond(
				      IrtImgRGBAPxlStruct *Image,
				      int XSize,
				      int YSize,
				      int NumColors,
				      const IrtImgRGBAPxlStruct *DitherColors);
IrtImgRGBAPxlStruct *IritMiscDtrSierraThird(
				      IrtImgRGBAPxlStruct *Image,
				      int XSize,
				      int YSize,
				      int NumColors,
				      const IrtImgRGBAPxlStruct *DitherColors);
IrtImgRGBAPxlStruct *IritMiscImgDitherImageClr3(
                                      IrtImgRGBAPxlStruct *Image,
				      int XSize,
				      int YSize,
				      int NumColors,
				      const IrtImgRGBAPxlStruct *DitherColors,
				      IrtImgImageDitherType DitherMethod);
int IritMiscImgDitherImage3(const char *InputImage,
			    const char *OutputImage,
			    int NumColors,
			    const IrtImgRGBAPxlStruct *DitherColors,
			    IrtImgImageDitherType DitherMethod);

/* Simple processing of images. */

#define IRIT_IMG_COPY_PIXEL_VALUE(Pixel1, Pixel2) \
    IRIT_GEN_COPY(Pixel1, Pixel2, sizeof(float) * 4)
#define IRIT_IMG_PRCSS_GET_PIXEL_VAL(Image, i, j, Color) \
    assert(((int) (i)) <= (int) Image -> Height && \
           ((int) (j)) <= (int) Image -> Width); \
    IRIT_IMG_COPY_PIXEL_VALUE(Color, \
		          &(Image -> Data[((i) * Image -> Width + (j)) * 4]))
#define IRIT_IMG_PRCSS_GET_PIXEL_VAL2(Image, Idx, Color) \
    IRIT_IMG_COPY_PIXEL_VALUE(Color, &(Image -> Data[Idx]))
#define IRIT_IMG_PRCSS_GET_PIXEL_VAL3(Image, Idx) (&Image -> Data[Idx])
#define IRIT_IMG_PRCSS_SET_PIXEL_VAL(Image, i, j, Color) \
    assert(((int) (i)) <= (int) Image -> Height && \
           ((int) (j)) <= (int) Image -> Width); \
    IRIT_IMG_COPY_PIXEL_VALUE( \
		   &(Image -> Data[((i) * Image -> Width + (j)) * 4]), Color)
#define IRIT_IMG_PRCSS_SET_PIXEL_VAL2(Image, Idx, Color) \
    IRIT_IMG_COPY_PIXEL_VALUE(&(Image -> Data[Idx]), Color)


IritImgPrcssImgStruct *IritMiscImgPrcssCreateWhiteImg(unsigned int Height,
						      unsigned int Width);
IritImgPrcssImgStruct *IritMiscImgPrcssCreateBlkImg(unsigned int Height,
						    unsigned int Width);
IritImgPrcssImgStruct *IritMiscImgPrcssCreateImg(unsigned int Height,
						 unsigned int Width,
					     float *Data);
IritImgPrcssImgStruct *IritMiscImgPrcssCopyImg(
					  const IritImgPrcssImgStruct *Image);
int IritMiscImgPrcssGetPixelVal(const IritImgPrcssImgStruct *Image,
				unsigned int i,
				unsigned int j,
				float Color[4]);
int IritMiscImgPrcssSetPixelVal(IritImgPrcssImgStruct *Image,
				unsigned int i,
				unsigned int j,
				const float color[4]); 
IritImgPrcssImgStruct *IritMiscImgPrcssToGrayScale(const IritImgPrcssImgStruct
								       *Image);
void IritMiscImgPrcssDeleteImg(IritImgPrcssImgStruct *Image);
IritImgPrcssImgStruct *IritMiscImgPrcssConv3(
					  const IritImgPrcssImgStruct *Image,
					  float Filter[9],
					  int Normalize);
IritImgPrcssImgStruct *IritMiscImgPrcssSharpen(
					   const IritImgPrcssImgStruct *Image);
IritImgPrcssImgStruct *IritMiscImgPrcssBiSobel(
					   const IritImgPrcssImgStruct *Image);
IritImgPrcssImgStruct *IritMiscImgPrcssGaussianBlur(
					   const IritImgPrcssImgStruct *Image);
IritImgPrcssImgStruct *IritMiscImgPrcssGaussianBlurMult(
						 IritImgPrcssImgStruct *Image,
						 int Amount);
IritImgPrcssImgStruct *IritMiscImgPrcssInvert(
					   const IritImgPrcssImgStruct *Image);
IritImgPrcssImgStruct *IritMiscImgPrcssDetectEdges(
					   const IritImgPrcssImgStruct *Image,
					   int BlurAmount);
IritImgPrcssImgStruct *IritMiscImgPrcssReadImg(const char *Path);
void IritMiscImgPrcssWriteImg(IritImgPrcssImgStruct *Image,
			      const char *SavePath);

void IritImgPrcssAppFunOnLine(IritImgPrcssImgStruct *Image,
			      int X0,
			      int Y0,
			      int X1,
			      int Y1,
			      IritImgPrcssFunctionOnPixel F,
			      void *Data);

/* Read/Write of movies. */
IrtImgPixelStruct **IritMiscMovieReadMovie(const char *MovieFileName,
					   int *MaxX,
					   int *MaxY,
					   int *Alpha);
IrtImgPixelStruct **IritMiscMovieReadMovie2(const char *MovieFileName,
					    int *MaxX,
					    int *MaxY,
					    int *Alpha);
IrtImgPixelStruct **IritMiscMovieReadMovie3(const char *MovieFileName,
					    int *MaxX,
					    int *MaxY,
					    int *Alpha);
int IritMiscMovieGetMovieProps(const char *MovieFileName,
			       int *Width,
			       int *Height);
void IrtMovieReadClrCache(void);
int IritMiscMovieParsePMovieString(const char *PMovie,
				   char *FName,
				   IrtRType *Scale,
				   IrtRType *Trans,
				   int *NewImage,
				   int *Flip,
				   int *Restart,
				   IrtRType *TimeSetup,
				   int *FlipHorizontally,
				   int *FlipVertically);

/* Searching routines. */
VoidPtr IritMiscSearch2DInit(IrtRType XMin,
			     IrtRType XMax,
			     IrtRType YMin,
			     IrtRType YMax,
			     IrtRType Tol,
			     int DataSize);
void IritMiscSearch2DFree(VoidPtr S2D);
void IritMiscSearch2DInsertElem(VoidPtr S2D,
				IrtRType XKey,
				IrtRType YKey,
				VoidPtr Data);
int IritMiscSearch2DFindElem(VoidPtr S2D,
			     IrtRType XKey,
			     IrtRType YKey,
			     VoidPtr Data);

/* Set cover related routines. */
VoidPtr IritMiscRLNew(void);
void IritMiscRLAdd(VoidPtr RLC, IrtRType l, IrtRType r, int attr);
int *IritMiscRLFindCyclicCover(VoidPtr RLC, IrtRType Tol);
void IritMiscRLDelete(VoidPtr RLC);
int IritMiscRLSetGaurdiansNumber(int g);

/* Image Set Cover related routines. */
char *MiscISCGetTimeStamp(long Time, 
                          char *TimeString, 
                          int TimeZone, 
                          int UseDays);
MiscISCCalculatorPtrType IritMiscISCNewCalculator(
					      int MaxPictures,
					      MiscISCImageSizeType PixelNumber,
					      MiscISCColorTypeEnum Colors,
                                              MiscISCPrintFuncType Print);
void IritMiscISCFreeCalculator(MiscISCCalculatorPtrType Calc);
int IritMiscISCAddPicture(MiscISCCalculatorPtrType Calc,
			  MiscISCPixelType *Picture);
int MiscISCSetImageToCover(MiscISCCalculatorPtrType Calc,
			   MiscISCPixelType *RequiredCover);
int IritMiscISCCalculateExact(MiscISCCalculatorPtrType Calc,
			      int SizeLimit,
			      int **SolutionByIndex,
			      int *SolutionSize,
			      IrtRType *CoverPart);
int MiscISCCalculateExhaustive(MiscISCCalculatorPtrType Calc,
                               IrtRType CoverLimit,
                               int SizeLimit,
                               int **SolutionByIndex,
                               int *SolutionSize,
                               IrtRType *CoverPart);
int MiscISCCalculateGreedy(MiscISCCalculatorPtrType Calc,
                           int **SolutionByIndex,
                           int *SolutionSize,
                           IrtRType *CoverPart);

/* Bipartite graphs weighted matching. and (1-to-1) matchings between sets  */
/* of the different sizes.						    */
int MiscBiPrWeightedMatchBipartite(const IrtRType **Weight,
				   IritBiPrWeightedMatchStruct *Match,
				   int n);
int *MiscBiPrComputeMinCostRecMatching(const IrtRType *CostMat,
				       int NumRows,
				       int NumCols);

/* Simple infix expression trees parser. */
MiscExprTreeGenInfoStructPtr IritMiscE2TExpr2TreeInit();
void IritMiscE2TExpr2TreeFree(MiscExprTreeGenInfoStructPtr GI);
IritE2TExprNodeStruct *IritMiscE2TExpr2Tree(MiscExprTreeGenInfoStructPtr GI,
					    const char s[]);
IritE2TExprNodeParamFuncType IritMiscE2Expr2TreeSetFetchParamValueFunc(
			     MiscExprTreeGenInfoStructPtr GI,
			     IritE2TExprNodeParamFuncType FetchParamValueFunc);
IrtRType IritMiscE2Expr2TreeDefaultFetchParamValue(
					      MiscExprTreeGenInfoStructPtr GI,
					      const char *SData);
int IritMiscE2TCmpTree(const IritE2TExprNodeStruct *Root1,
		       const IritE2TExprNodeStruct *Root2);
IritE2TExprNodeStruct *IritMiscE2TCopyTree(const IritE2TExprNodeStruct *Root);
IritE2TExprNodeStruct *IritMiscE2TDerivTree(
					MiscExprTreeGenInfoStructPtr GI,
					const IritE2TExprNodeStruct *Root,
					int Param);
void IritMiscE2TPrintTree(const IritE2TExprNodeStruct *Root, char *Str);
IrtRType IritMiscE2TEvalTree(MiscExprTreeGenInfoStructPtr GI,
			     const IritE2TExprNodeStruct *Root);
void IritMiscE2TFreeTree(IritE2TExprNodeStruct *Root);
int IritMiscE2TParamInTree(const IritE2TExprNodeStruct *Root,
			   const char *ParamName);
void IritMiscE2TSetParamValue(MiscExprTreeGenInfoStructPtr GI,
			      IrtRType Value,
			      int Index);
int IritMiscE2TParseError(MiscExprTreeGenInfoStructPtr GI);
int IritMiscE2TDerivError(MiscExprTreeGenInfoStructPtr GI);

/* XGeneral routine - compatibility between Unix and Win95/WinNT/OS2/etc. */
char *IritMiscStrdup(const char *s);
char *IritMiscStrUpper(char *s);
char *IritMiscStrLower(char *s);
void IritMiscSleep(int MiliSeconds);
void IritMiscRandomInit(long Seed);
IrtRType IritMiscRandom(IrtRType Min, IrtRType Max);
void IritPseudoRandomInit(unsigned int Seed);
IrtRType IritPseudoRandom();
IrtRType IritMiscCPUTime(int Reset);
int IritMiscSecsFromLastCall(int Secs, IrtRType *Prev);
char *IritMiscRealTimeDate(char *StrTime);
IrtRType IritMiscApproxStrStrMatch(const char *Str1,
				   const char *Str2,
				   int IgnoreCase);

#ifndef AMIGA
void movmem(VoidPtr Src, VoidPtr Dest, int Len);
#endif /* AMIGA */
const char *searchpath(const char *Name, char *FullPath);
const wchar_t *searchpathW(const wchar_t *Name, wchar_t *FullPath);

#ifdef STRICMP
int strnicmp(const char *s1, const char *s2, int n);
int stricmp(const char *s1, const char *s2);
#else
#if !(defined(__WINNT__) || defined(__WINCE__) || defined(OS2GCC) || defined(AMIGA) || defined(__CYGWIN__))
#   define strnicmp(s1, s2, n) strncasecmp((s1), (s2), (n))
#   define stricmp(s1, s2)     strcasecmp((s1), (s2))
#   define _wcsicmp(s1, s2)     wcscasecmp((s1), (s2))
#endif /* !(__WINNT__ || __WINCE__|| OS2GCC || AMIGA) */

#ifdef __WINNT__
#   if _MSC_VER >= 1400  /* Visual 8, 2005 */
#       define strnicmp(s1, s2, n) _strnicmp((s1), (s2), (n))
#       define stricmp(s1, s2)     _stricmp((s1), (s2))
#   endif /* _MSC_VER >= 1400 */
#endif /*  __WINNT__ */

#ifdef __WINCE__
#   ifdef strnicmp
#       undef strnicmp
#       undef stricmp
#   endif /* strnicmp */
#   define strnicmp(s1, s2, n) _strnicmp((s1), (s2), (n))
#   define stricmp(s1, s2)     _stricmp((s1), (s2))
#endif /* __WINCE__ */
#endif /* STRICMP */

#ifdef __WINNT__
#   if _MSC_VER >= 1400  /* Visual 8, 2005 */
#   ifdef getcwd
#       undef getcwd
#   endif /* getcwd */
#   define getcwd(buf, maxlen)	_getcwd(buf, maxlen)
#   define chdir(dir)		_chdir(dir)
#   define putenv(str)		_putenv(str)
#   endif /* _MSC_VER >= 1400 */
#endif /*  __WINNT__ */

/* Used by parallel computation (under windows). */
void IritEmulatePthreadMutexLock(IRIT_MUTEX *Mx);
void IritMiscEmulatePthreadMutexUnLock(IRIT_MUTEX Mx);

const char *IritMiscStrIStr(const char *s, const char *Pattern);
char *IritMiscSubstStr(const char *S,
		       const char *Src,
		       const char *Dst,
		       int CaseInsensitive);


#ifdef STRSTR
char *strstr(const char *s, const char *Pattern);
#endif /* STRSTR */

#ifdef GETCWD
char *getcwd(char *s, int Len);
#endif /* GETCWD */


#if !defined(__WINNT__) && !defined(__OS2GCC__)
/* Emulating windows functions under Unix... */
FILE *_wfopen(const wchar_t *Filename, const wchar_t *Mode);
FILE *_wpopen(const wchar_t *Command, const wchar_t *Mode);
wchar_t *_wgetenv(const wchar_t *WName);
#endif /* __WINNT__ || __OS2GCC__ */


/* Error handling. */

IritMiscSetErrorFuncType IritMiscSetFatalErrorFunc(
					   IritMiscSetErrorFuncType ErrorFunc);
void IritMiscFatalError(IritMiscFatalErrorType ErrID, char *ErrDesc);
const char *IritMiscDescribeError(IritMiscFatalErrorType ErrorNum);

int IritMiscLineHasCntrlChar(const char *Line);

IritFatalMsgFuncType IritSetFatalErrorFunc(IritFatalMsgFuncType FatalMsgFunc);

void IritFatalError(const char *Msg);
IritWarningMsgFuncType IritSetWarningMsgFunc(IritWarningMsgFuncType WrnMsgFunc);
void IritWarningMsg(const char *Msg);
IritInfoMsgFuncType IritSetInfoMsgFunc(IritInfoMsgFuncType InfoMsgFunc);
IritInfoWMsgFuncType IritSetInfoWMsgFunc(IritInfoWMsgFuncType InfoWMsgFunc);
void IritInformationMsg(const char *Msg);
void IritInformationWMsg(const wchar_t *Msg);

/* Wide char processing. */
const char *IritMiscTextLocaleInit(void);
char *IritMiscWChar2Ascii(const wchar_t *Str);
wchar_t *IritMiscAscii2WChar(const char *Str);

#ifdef USE_VARARGS
void IritFatalErrorPrintf(const char *va_alist, ...);
void IritWarningMsgPrintf(const char *va_alist, ...);
void IritInformationMsgPrintf(const char *va_alist, ...);
#else
void IritFatalErrorPrintf(const char *Format, ...);
void IritWarningMsgPrintf(const char *Format, ...);
void IritInformationMsgPrintf(const char *Format, ...);
#endif /* USE_VARARGS */

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* IRIT_MISC_LIB_H */
