#ifndef _HOVERCAMERA_H_
#define _HOVERCAMERA_H_

#include "Demeter/Terrain.h"
#include <iostream>
#include <osg/Geode>
#include <osg/Material>
#include <osg/Vec3>
#include <osg/Transform>
#include <osg/Texture>
#include <osg/TexEnv>
#include <osg/BlendFunc>
#include <osg/AlphaFunc>
#include <osg/Billboard>
#include <osgDB/ReadFile>
#include <osgUtil/SceneView>
#include <math.h>
#include "Demeter/DemeterDrawable.h"

using namespace Demeter;
using namespace osg;
using namespace osgUtil;

// A camera that follows the surface of the terrain as it is moved around
class HoverCamera
{
      public:
	HoverCamera(Terrain * pTerrain, SceneView * pView, float maxViewDistance, float aspectRatio);
	 ~HoverCamera();
	void SetPosition(float x, float y);
	void GetPosition(Vec3 & pos);
	void SetYaw(float yaw);
	float GetYaw();
	void SetHoverElevation(float elev);
	float GetHoverElevation();
	void AddYaw(float yawDelta);
	void Translate(float x, float y);
	void TranslateForward(float distance);
	void TranslateBackward(float distance);
	void TranslateLeft(float distance);
	void TranslateRight(float distance);
	void Recalc();
      private:
	  Vec3 m_Position;
	Vec3 m_LookUp;
	Vec3 m_LookAt;
	Vec3 m_FieldOfView;
	  ref_ptr < Terrain > m_RefTerrain;
	  ref_ptr < SceneView > m_RefSceneView;
	float m_CurrentYaw;
	float m_HoverElevation;
	float m_MinimumHoverElevation;
};

#endif
