#ifndef _HOVERCAMERA_H_
#define _HOVERCAMERA_H_

#include <Demeter/Terrain.h>
#include <iostream>
#include <math.h>

using namespace Demeter;

// A camera that follows the surface of the terrain as it is moved around
class Camera
{
      public:
	Camera(Terrain *);
	Camera(TerrainLattice *);
	~Camera();
	void SetPosition(float x, float y);
	void GetPosition(Demeter::Vector & pos);
	void SetYaw(float yaw);
	float GetYaw();
	void SetPitch(float pitch);
	float GetPitch();
	void SetHoverElevation(float elev);
	float GetHoverElevation();
	void AddYaw(float yawDelta);
	void AddPitch(float pitchDelta);
	void Translate(float x, float y);
	void TranslateForward(float distance);
	void TranslateBackward(float distance);
	void TranslateLeft(float distance);
	void TranslateRight(float distance);
	void Update();
      private:
	void Init();
	void Recalc();
	  Demeter::Vector m_Position;
	  Demeter::Vector m_LookUp;
	  Demeter::Vector m_LookAt;
	  Demeter::Vector m_CurrentLookAt;
	  Demeter::Vector m_FieldOfView;
	Terrain *m_pTerrain;
	TerrainLattice *m_pLattice;
	float m_CurrentYaw;
	float m_CurrentPitch;
	float m_HoverElevation;
	float m_MinimumHoverElevation;
	float m_MaximumHoverElevation;
};

#endif
