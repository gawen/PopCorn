#include "HoverCamera.h"

float maxV, asp;

HoverCamera::HoverCamera(Terrain * pTerrain, SceneView * pView, float maxViewDistance, float aspectRatio)
{
	maxV = maxViewDistance;
	asp = aspectRatio;
	m_RefTerrain = pTerrain;
	m_RefSceneView = pView;
	m_RefSceneView->setProjectionMatrixAsPerspective(60.0, aspectRatio, 1.0, maxViewDistance);
	m_CurrentYaw = 0.0f;
	m_Position.x() = m_Position.y() = m_Position.z() = 0.0f;
	m_LookUp.x() = m_LookUp.y() = 0.0f;
	m_LookUp.z() = 1.0f;
	m_HoverElevation = 10.0f;
	m_LookAt.x() = m_LookAt.z() = 0.0f;
	m_LookAt.y() = 1.0f;
	m_MinimumHoverElevation = 2.0f;
}

HoverCamera::~HoverCamera()
{
}

void HoverCamera::SetPosition(float x, float y)
{
	m_Position.x() = x;
	m_Position.y() = y;
}

void HoverCamera::GetPosition(Vec3 & pos)
{
	pos = m_Position;
}

void HoverCamera::SetYaw(float yaw)
{
	m_CurrentYaw = yaw;
}

float HoverCamera::GetYaw()
{
	return m_CurrentYaw;
}

void HoverCamera::SetHoverElevation(float elev)
{
	m_HoverElevation = elev;
	if (m_HoverElevation < m_MinimumHoverElevation)
		m_HoverElevation = m_MinimumHoverElevation;
}

float HoverCamera::GetHoverElevation()
{
	return m_HoverElevation;
}

void HoverCamera::AddYaw(float yaw)
{
	m_CurrentYaw += yaw;
}

void HoverCamera::Translate(float x, float y)
{
	m_Position.x() += x;
	m_Position.y() += y;
}

void HoverCamera::TranslateForward(float distance)
{
	Vec3 facing = m_LookAt;
	facing.z() = 0.0f;
	facing.normalize();
	m_Position.x() += facing.x() * distance;
	m_Position.y() += facing.y() * distance;
}

void HoverCamera::TranslateBackward(float distance)
{
	Vec3 facing = m_LookAt;
	facing.z() = 0.0f;
	facing.normalize();
	m_Position.x() -= facing.x() * distance;
	m_Position.y() -= facing.y() * distance;
}

void HoverCamera::TranslateLeft(float distance)
{
	Vec3 facing = m_LookAt;
	facing.z() = 0.0f;
	facing.normalize();
	Matrix rotMatrix;
	rotMatrix.makeRotate(M_PI * 0.5f, 0.0f, 0.0f, 1.0f);
	facing = facing * rotMatrix;
	m_Position.x() += facing.x() * distance;
	m_Position.y() += facing.y() * distance;
}

void HoverCamera::TranslateRight(float distance)
{
	Vec3 facing = m_LookAt;
	facing.z() = 0.0f;
	facing.normalize();
	Matrix rotMatrix;
	rotMatrix.makeRotate(-M_PI * 0.5f, 0.0f, 0.0f, 1.0f);
	facing = facing * rotMatrix;
	m_Position.x() += facing.x() * distance;
	m_Position.y() += facing.y() * distance;
}

void HoverCamera::Recalc()
{
	Matrix cameraMatrix;
	cameraMatrix.makeRotate(-m_CurrentYaw, 0.0f, 0.0f, 1.0f);

	m_LookAt.x() = 0.0f;
	m_LookAt.y() = 1.0f;
	m_LookAt.z() = 0.0f;
	m_LookAt = m_LookAt * cameraMatrix;

	m_Position.z() = m_RefTerrain->GetElevation(m_Position.x(), m_Position.y()) + m_HoverElevation;
	Vec3 currentLookAt;
	currentLookAt.x() = m_Position.x() + m_LookAt.x();
	currentLookAt.y() = m_Position.y() + m_LookAt.y();
	currentLookAt.z() = m_Position.z() + m_LookAt.z();
	m_RefSceneView->setProjectionMatrixAsPerspective(60.0, asp, 1.0, maxV);
	m_RefSceneView->setViewMatrixAsLookAt(m_Position, currentLookAt, m_LookUp);
}
