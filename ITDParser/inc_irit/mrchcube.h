/*****************************************************************************
* MarchCube.h -	An implementation of the marching cube algorithm.	     *
*									     *
*									     *
*		      7		   K	       6			     *
*		       ***********************				     *
*		     * +		   * *				     *
*		 L *   +		 *   *		    Vertices 0 - 7   *
*		 *     +     I	       * J   *		    Edges    A - L   *
*	     4 *********************** 5     *				     *
*	       *       +	     *	     *				     *
*	       *       +	     *	     * G			     *
*	       *       + H	     *	     *				     *
*	       *       +	     *	     *				     *
*	       *       +	     * F     *				     *
*	     E *       +       C     *	     *				     *
*	       *       ++++++++++++++*+++++++* 2			     *
*	       *   D + 3	     *	   *				     *
*	       *   +		     *	 * B				     *
*	       * +		     * *				     *
*	       ***********************					     *
*	      0		  A	      1					     *
*									     *
******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute	of Technology		     *
******************************************************************************
* Written by Gershon Elber, Dec. 92.					     *
*****************************************************************************/

#ifndef IRIT_MARCH_CUBE_H
#define IRIT_MARCH_CUBE_H

#include "inc_irit/irit_sm.h"
#include "inc_irit/triv_lib.h"
#include "inc_irit/user_lib.h"

typedef struct _MCInterStruct { /* Used internally to hold intersection pts. */
    CagdPType _Nrml;
    CagdPType _Pos;
    int _HighV;
} _MCInterStruct;

typedef struct MCCubeCornerScalarStruct {
    CagdPType Vrtx0Lctn;			  /* Lowest corner position. */
    CagdPType CubeDim; 				    /* Width, Depth, Height. */
    CagdRType Corners[8];			/* Scalar values of corners. */
    CagdRType GradientX[8];		    /* Optional gradient at corners. */
    CagdRType GradientY[8];
    CagdRType GradientZ[8];
    CagdBType HasGradient;		       /* True if Gradient? are set. */

    int XIdx, YIdx, ZIdx;    /* Indices in the 3D grid of cubes we march on. */
    int XSize, YSize, ZSize;       /* The XYZ sizes of the 3D grid of cubes. */
    int SkipInputData;                    /* Are we skipping elements here!? */
    int ExtractBndry;   /* TRUE to also extract polygons representing bndry. */

    /* Used internally. */
    CagdBType _Intersect;
    CagdPType _VrtxPos[8];
    struct _MCInterStruct _Inter[12];
} MCCubeCornerScalarStruct;

typedef struct MCPolygonStruct {
    struct MCPolygonStruct *Pnext;		        /* To next in chain. */
    int NumOfVertices;
    int BndryPoly;
    CagdPType V[13];
    CagdPType N[13];
} MCPolygonStruct;

typedef struct IritTrivMCImprovePointOnIsoSrfInfoStruct
		                 *IritTrivMCImprovePointOnIsoSrfInfoStructPtr;

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

MCPolygonStruct *IritTrivMCThresholdCube(MCCubeCornerScalarStruct *CCS,
				         CagdRType Threshold);

IPObjectStruct *IritTrivMCExtractIsoSurface(const char *FileName,
					    int DataType,
					    IrtPtType CubeDim,
					    int Width,
					    int Height,
					    int Depth,
					    int SkipFactor,
					    CagdRType IsoVal,
					    CagdRType ExtractBndry);
IPObjectStruct *IritTrivMCExtractIsoSurface2(const TrivTVStruct *TV,
					     int Axis,
					     CagdBType TrivarNormals,
					     IrtPtType CubeDim,
					     int SkipFactor,
					     CagdRType SamplingFactor,
					     CagdRType IsoVal,
					     CagdRType ExtractBndry,
					     int AddColors);
IPObjectStruct *IritTrivMCExtractIsoSurface3(IPObjectStruct *ImageList,
					     IrtPtType CubeDim,
					     int SkipFactor,
					     CagdRType IsoVal,
					     CagdRType ExtractBndry);
IPObjectStruct *IritTrivMCExtractIsoSurface4(const void **ImageVector,
					     const int *Size,
					     TrivImagePixelType PixelType,
					     IrtPtType CubeDim,
					     int SkipFactor,
					     CagdRType IsoVal,
					     CagdRType ExtractBndry);
IPObjectStruct *IritTrivMCExtractIsoSurface5(
				     const UserMicroRegImplctParamStruct *MRI,
				     IrtPtType CubeDim,
				     int SkipFactor,
				     CagdRType SamplingFactor,
				     CagdRType IsoVal,
				     CagdRType ExtractBndry);
TrivTVStruct *IritTrivLoadVolumeIntoTV(const char *FileName,
				       int DataType,
				       IrtVecType VolSize,
				       IrtVecType Orders);

IritTrivMCImprovePointOnIsoSrfInfoStructPtr
                  IritTrivMCImprovePointOnIsoSrfPrelude(
					           const TrivTVStruct *TV);
void IritTrivMCImprovePointOnIsoSrfPostlude(
	     IritTrivMCImprovePointOnIsoSrfInfoStructPtr MCImprovePointOnI);
int IritTrivMCImprovePointOnIsoSrf(
             IritTrivMCImprovePointOnIsoSrfInfoStructPtr MCImprovePointOnI,
	     IrtPtType Pt,
	     const IrtPtType CubeDim,
	     CagdRType IsoVal,
	     CagdRType Tolerance,
	     CagdRType AllowedError);
CagdCrvStruct *IritTrivCoverIsoSurfaceUsingStrokes(TrivTVStruct *TV,
						   int NumStrokes,
						   int StrokeType,
						   CagdPType MinMaxPwrLen,
						   CagdRType StepSize,
						   CagdRType IsoVal,
						   CagdVType ViewDir);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* IRIT_MARCH_CUBE_H */
