/*****************************************************************************
*   "Irit" - the 3d polygonal solid modeller.				     *
******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute	of Technology		     *
******************************************************************************
* Extra	functions from external, public	domain,	sources.		     *
*****************************************************************************/

#ifndef	IRIT_EXTRA_FN_H
#define IRIT_EXTRA_FN_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/* SVD decomposition. */
int IritExtraSvdMatrix4x4(IrtHmgnMatType M,
			  IrtRType U[3][3],
			  IrtVecType S,
			  IrtRType V[3][3]);
void IritExtraSvdMatrixNxN(IrtRType *M,
			   IrtRType *U,
			   IrtRType *S,
			   IrtRType *V, int n);
void IritExtraSvdDecomp(IrtRType **a, int m, int n, IrtRType *w, IrtRType **v);


typedef struct XtraSvdLSGenInfoStruct *XtraSvdLSGenInfoStructPtr;

XtraSvdLSGenInfoStructPtr IritExtraSvdLeastSqrInit(IrtRType *A,
						   int NData,
						   int Nx,
						   IrtRType *RecipCondNum);
IrtRType IritExtraSvdLeastSqr(XtraSvdLSGenInfoStructPtr SLSGI,
			      IrtRType *x,
			      IrtRType *b);
void IritExtraSvdLeastSqrFree(XtraSvdLSGenInfoStructPtr SLSGI);

/* Matrix diagonalization. */
void IritJacobiMatrixDiag4x4(IrtRType M[4][4],
			     IrtRType U[4][4],
			     IrtRType D[4][4],
			     IrtRType V[4][4]);
void IritJacobiMatrixDiagNxN(IrtRType *M[],
			     IrtRType *U[],
			     IrtRType *D[],
			     IrtRType *V[],
			     int n);

/* Bezier interpolation. */
void IritCagdBzrCrvInterp(IrtRType *Result, IrtRType *Input, int Size);

/* Irit internal random number generator. */
void IritExtraMtSrandom(long Seed);
long MtIritMiscRandom(void);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* IRIT_EXTRA_FN_H */
