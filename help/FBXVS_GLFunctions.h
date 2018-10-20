#pragma once

#ifndef _GL_FUNCTIONS_H
#define _GL_FUNCTIONS_H

#include <fbxsdk.h>

#include "glew.h"
#define GLUT_DISABLE_ATEXIT_HACK
#include "GL/gl.h"
#include "GL/glu.h"

#include "glut.h"

#include <math.h>

enum ShadingMode
{
	SHADING_MODE_WIREFRAME,
	SHADING_MODE_SHADED,
};

void GlSetCameraPerspective(double pFieldOfViewY,
	double pAspect,
	double pNearPlane,
	double pFarPlane,
	FbxVector4& pEye,
	FbxVector4& pCenter,
	FbxVector4& pUp,
	double  pFilmOffsetX,
	double  pFilmOffsetY);
void GlSetCameraOrthogonal(double pLeftPlane,
	double pRightPlane,
	double pBottomPlane,
	double pTopPlane,
	double pNearPlane,
	double pFarPlane,
	FbxVector4& pEye,
	FbxVector4& pCenter,
	FbxVector4& pUp);

void GlDrawMarker(FbxAMatrix& pGlobalPosition);
void GlDrawLimbNode(FbxAMatrix& pGlobalBasePosition,
	FbxAMatrix& pGlobalEndPosition);
void GlDrawCamera(FbxAMatrix& pGlobalPosition,
	double pRoll);
void GlDrawCrossHair(FbxAMatrix& pGlobalPosition);

#endif // #ifndef _GL_FUNCTIONS_H

