#include "Camera.h"
#include "Matrix.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.1415926f
#endif

Camera::Camera(Terrain * pTerrain)
{
	m_pTerrain = pTerrain;
	m_pLattice = NULL;
	Init();
}

Camera::Camera(TerrainLattice * pLattice)
{
	m_pLattice = pLattice;
	m_pTerrain = NULL;
	Init();
}

void Camera::Init()
{
	m_CurrentYaw = 0.0f;
	m_CurrentPitch = 0.0f;
	m_Position.x = m_Position.y = m_Position.z = 0.0f;
	m_LookUp.x = m_LookUp.y = 0.0f;
	m_LookUp.z = 1.0f;
	m_HoverElevation = 10.0f;
	m_LookAt.x = m_LookAt.z = 0.0f;
	m_LookAt.y = 1.0f;
	m_MinimumHoverElevation = 2.0f;
	m_MaximumHoverElevation = 100.0f;
}

Camera::~Camera()
{
}

void Camera::SetPosition(float x, float y)
{
	m_Position.x = x;
	m_Position.y = y;
	Recalc();
}

void Camera::GetPosition(Demeter::Vector & pos)
{
	pos = m_Position;
}

void Camera::SetYaw(float yaw)
{
	m_CurrentYaw = yaw;
	Recalc();
}

float Camera::GetYaw()
{
	return m_CurrentYaw;
}

void Camera::SetPitch(float pitch)
{
	m_CurrentPitch = pitch;
	Recalc();
}

float Camera::GetPitch()
{
	return m_CurrentPitch;
}

void Camera::SetHoverElevation(float elev)
{
	m_HoverElevation = elev;
	if (m_HoverElevation < m_MinimumHoverElevation)
		m_HoverElevation = m_MinimumHoverElevation;
	if (m_MaximumHoverElevation < m_HoverElevation)
		m_HoverElevation = m_MaximumHoverElevation;
	Recalc();
}

float Camera::GetHoverElevation()
{
	return m_HoverElevation;
}

void Camera::AddYaw(float yaw)
{
	m_CurrentYaw += yaw;
	Recalc();
}

void Camera::AddPitch(float pitch)
{
	m_CurrentPitch += pitch;
	Recalc();
}

void Camera::Translate(float x, float y)
{
	m_Position.x += x;
	m_Position.y += y;
	Recalc();
}

void Camera::TranslateForward(float distance)
{
	Demeter::Vector facing = m_LookAt;
	facing.z = 0.0f;
	facing.Normalize();
	m_Position.x += facing.x * distance;
	m_Position.y += facing.y * distance;
	Recalc();
}

void Camera::TranslateBackward(float distance)
{
	Demeter::Vector facing = m_LookAt;
	facing.z = 0.0f;
	facing.Normalize();
	m_Position.x -= facing.x * distance;
	m_Position.y -= facing.y * distance;
	Recalc();
}

void Camera::TranslateLeft(float distance)
{
	Demeter::Vector facing = m_LookAt;
	facing.z = 0.0f;
	facing.Normalize();
	Matrix rotMatrix;
	rotMatrix.SetRotateZ((2.0f * M_PI) / 4.0f);
	facing = rotMatrix * facing;
	m_Position.x += facing.x * distance;
	m_Position.y += facing.y * distance;
	Recalc();
}

void Camera::TranslateRight(float distance)
{
	Demeter::Vector facing = m_LookAt;
	facing.z = 0.0f;
	facing.Normalize();
	Matrix rotMatrix;
	rotMatrix.SetRotateZ((-2.0f * M_PI) / 4.0f);
	facing = rotMatrix * facing;
	m_Position.x += facing.x * distance;
	m_Position.y += facing.y * distance;
	Recalc();
}

void Camera::Recalc()
{
	Matrix cameraMatrix;

	m_LookAt.x = 0.0f;
	m_LookAt.y = 1.0f;
	m_LookAt.z = 0.0f;

	cameraMatrix.SetRotateX(-m_CurrentPitch);
	m_LookAt = cameraMatrix * m_LookAt;
	cameraMatrix.SetRotateZ(-m_CurrentYaw);
	m_LookAt = cameraMatrix * m_LookAt;

	if (m_pTerrain)
		m_Position.z = m_pTerrain->GetElevation(m_Position.x, m_Position.y) + m_HoverElevation;
	else
		m_Position.z = m_pLattice->GetElevation(m_Position.x, m_Position.y) + m_HoverElevation;
	m_CurrentLookAt.x = m_Position.x + m_LookAt.x;
	m_CurrentLookAt.y = m_Position.y + m_LookAt.y;
	m_CurrentLookAt.z = m_Position.z + m_LookAt.z;
}

void Camera::Update()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(m_Position.x, m_Position.y, m_Position.z, m_CurrentLookAt.x, m_CurrentLookAt.y, m_CurrentLookAt.z, m_LookUp.x, m_LookUp.y, m_LookUp.z);
}
