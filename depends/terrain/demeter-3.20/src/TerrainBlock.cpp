// Demeter Terrain Visualization Library by Clay Fowler
// Copyright (C) 2002 Clay Fowler

// $ID$

/*
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this library; if not, write to the
Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA  02111-1307, USA.
*/

#include "Demeter/TerrainBlock.h"
#include "Demeter/Settings.h"
#include "Demeter/Terrain.h"

extern float numBlocks;
extern float numLevels;
extern float hashDelta;

using namespace Demeter;
using namespace std;

#if _USE_RAYTRACING_SUPPORT_ > 0
int RayPlaneIntersect(const Ray * ray, const Plane * plane, Vector * point, float *distance)
{
	float vd, vo, PnDOTRo, t;

	vd = plane->a * ray->m_Direction.x + plane->b * ray->m_Direction.y + plane->c * ray->m_Direction.z;
	if (vd == 0.0)
		// The plane is parallel to the ray. I've never seen this happen but someday it will . . .
		return -1;
	if (vd > 0.0)
	{
		// The plane is facing away from the ray so no intersection occurs.
		return -2;
	}
	PnDOTRo = plane->a * ray->m_Origin.x + plane->b * ray->m_Origin.y + plane->c * ray->m_Origin.z;
	vo = -1.0f * (PnDOTRo + plane->d);
	t = vo / vd;
	if (t < 0.0f)
		// The intersection occurs behind the ray's origin.
		return -3;
	point->x = ray->m_Origin.x + ray->m_Direction.x * t;
	point->y = ray->m_Origin.y + ray->m_Direction.y * t;
	point->z = ray->m_Origin.z + ray->m_Direction.z * t;
	if (distance != NULL)
		*distance = t;
	return 1;
}

int RayBoxIntersect(const Ray * ray, const Box * box, Vector * point, float *distance)
{
	float tnear, tfar, t1, t2;

	tnear = -INFINITY;
	tfar = INFINITY;

// Find intersection with x-aligned planes of box.
	// If the ray is parallel to the box and not within the planes of the box it misses.
	if (ray->m_Direction.x == 0.0)
		if ((ray->m_Origin.x < box->m_Min.x) && (ray->m_Origin.x > box->m_Max.x))
			return 0;
	// Calculate intersection distance with the box's planes.
	t1 = (box->m_Min.x - ray->m_Origin.x) / ray->m_Direction.x;
	t2 = (box->m_Max.x - ray->m_Origin.x) / ray->m_Direction.x;
	if (t1 > t2)
	{
		float tmp = t1;
		t1 = t2;
		t2 = tmp;
	}
	if (t1 > tnear)
		tnear = t1;
	if (t2 < tfar)
		tfar = t2;
	if (tnear > tfar)
		return 0;
	if (tfar < 0.0)
		return 0;
// Find intersection with y-aligned planes of box.
	// If the ray is parallel to the box and not within the planes of the box it misses.
	if (ray->m_Direction.y == 0.0)
		if ((ray->m_Origin.y < box->m_Min.y) && (ray->m_Origin.y > box->m_Max.y))
			return 0;
	// Calculate intersection distance with the box's planes.
	t1 = (box->m_Min.y - ray->m_Origin.y) / ray->m_Direction.y;
	t2 = (box->m_Max.y - ray->m_Origin.y) / ray->m_Direction.y;
	if (t1 > t2)
	{
		float tmp = t1;
		t1 = t2;
		t2 = tmp;
	}
	if (t1 > tnear)
		tnear = t1;
	if (t2 < tfar)
		tfar = t2;
	if (tnear > tfar)
		return 0;
	if (tfar < 0.0)
		return 0;
// Find intersection with z-aligned planes of box.
	// If the ray is parallel to the box and not within the planes of the box it misses.
	if (ray->m_Direction.z == 0.0)
		if ((ray->m_Origin.z < box->m_Min.z) && (ray->m_Origin.z > box->m_Max.z))
			return 0;
	// Calculate intersection distance with the box's planes.
	t1 = (box->m_Min.z - ray->m_Origin.z) / ray->m_Direction.z;
	t2 = (box->m_Max.z - ray->m_Origin.z) / ray->m_Direction.z;
	if (t1 > t2)
	{
		float tmp = t1;
		t1 = t2;
		t2 = tmp;
	}
	if (t1 > tnear)
		tnear = t1;
	if (t2 < tfar)
		tfar = t2;
	if (tnear > tfar)
		return 0;
	if (tfar < 0.0)
		return 0;
// If we survived all of the tests, the box is hit.
	if (point != NULL)
	{
		point->x = ray->m_Origin.x + tnear * ray->m_Direction.x;
		point->y = ray->m_Origin.y + tnear * ray->m_Direction.y;
		point->z = ray->m_Origin.z + tnear * ray->m_Direction.z;
		*distance = tnear;
	}
	return 1;
}
#endif

TerrainBlock::TerrainBlock(TerrainBlock * pParent)
{
	m_pChildren = NULL;
	m_pTriangleStrip = NULL;
#if _USE_RAYTRACING_SUPPORT_ > 0
	m_pTriangles = NULL;
#endif
}

TerrainBlock::TerrainBlock(int homeVertex, int stride, Terrain * pTerrain, TerrainBlock * pParent)
{
	m_pTriangleStrip = NULL;
	m_HomeIndex = homeVertex;
	m_Stride = stride;
	static int numBlocksBuilt = 0;

	if (Settings::GetInstance()->IsVerbose())
	{
		if (hashDelta <= numBlocksBuilt++)
		{
			cout << "#" << flush;
			numBlocksBuilt = 0;
		}
	}

// Recursively build children blocks of this block.
	if (2 < m_Stride)
	{
		m_pChildren = new TerrainBlock *[4];
		int childrenStride = m_Stride / 2;
		m_pChildren[0] = new TerrainBlock(homeVertex, childrenStride, pTerrain, this);
		m_pChildren[1] = new TerrainBlock(homeVertex + childrenStride, childrenStride, pTerrain, this);
		m_pChildren[2] = new TerrainBlock(homeVertex + childrenStride * pTerrain->GetWidthVertices() + childrenStride, childrenStride, pTerrain, this);
		m_pChildren[3] = new TerrainBlock(homeVertex + childrenStride * pTerrain->GetWidthVertices(), childrenStride, pTerrain, this);
	}
	CalculateGeometry(pTerrain);
}

TerrainBlock::~TerrainBlock()
{
	m_pTriangleStrip = NULL;
	if (m_pChildren != NULL && 2 < m_Stride)
	{
		for (int i = 0; i < 4; i++)
		{
			delete m_pChildren[i];
			m_pChildren[i] = NULL;
		}
		delete[]m_pChildren;
	}
}

bool TerrainBlock::IsActive()
{
	return m_bIsActive;
}

void TerrainBlock::Tessellate(const double *pMatModelView, const double *pMatProjection, const int *pViewport, TriangleStrip * pTriangleStrips, unsigned int *pCountStrips, Terrain * pTerrain)
{
	Box boundingBox;
	float width = m_Stride * pTerrain->GetVertexSpacing();
	boundingBox.m_Min.x = pTerrain->m_pVertices[m_HomeIndex].x;
	boundingBox.m_Min.y = pTerrain->m_pVertices[m_HomeIndex].y;
	boundingBox.m_Min.z = m_MinElevation;
	boundingBox.m_Max.x = boundingBox.m_Min.x + width;
	boundingBox.m_Max.y = boundingBox.m_Min.y + width;
	boundingBox.m_Max.z = m_MaxElevation;

	if ((*pCountStrips < pTerrain->m_MaxNumberOfPrimitives) && pTerrain->CuboidInFrustum(boundingBox))
	{
		if (m_Stride == 2)
		{
			int offset;

			pTerrain->SetVertexStatus(m_HomeIndex, 1);
			pTriangleStrips[*pCountStrips].m_pVertices[0] = m_HomeIndex;
			offset = m_HomeIndex + pTerrain->GetWidthVertices();
			pTerrain->SetVertexStatus(offset, 1);
			pTriangleStrips[*pCountStrips].m_pVertices[1] = offset;
			offset = m_HomeIndex + 1;
			pTerrain->SetVertexStatus(offset, 1);
			pTriangleStrips[*pCountStrips].m_pVertices[2] = offset;
			offset = m_HomeIndex + 1 + pTerrain->GetWidthVertices();
			pTerrain->SetVertexStatus(offset, 1);
			pTriangleStrips[*pCountStrips].m_pVertices[3] = offset;
			offset = m_HomeIndex + 2;
			pTerrain->SetVertexStatus(offset, 1);
			pTriangleStrips[*pCountStrips].m_pVertices[4] = offset;
			offset = m_HomeIndex + 2 + pTerrain->GetWidthVertices();
			pTerrain->SetVertexStatus(offset, 1);
			pTriangleStrips[*pCountStrips].m_pVertices[5] = offset;
			pTriangleStrips[*pCountStrips].m_NumberOfVertices = 6;
			pTriangleStrips[*pCountStrips].m_bEnabled = true;
			*pCountStrips = *pCountStrips + 1;

			if (*pCountStrips < pTerrain->m_MaxNumberOfPrimitives)
			{
				offset = pTerrain->GetWidthVertices() + m_HomeIndex;
				pTerrain->SetVertexStatus(offset, 1);
				pTriangleStrips[*pCountStrips].m_pVertices[0] = offset;
				offset = pTerrain->GetWidthVertices() + m_HomeIndex + pTerrain->GetWidthVertices();
				pTerrain->SetVertexStatus(offset, 1);
				pTriangleStrips[*pCountStrips].m_pVertices[1] = offset;
				offset = pTerrain->GetWidthVertices() + m_HomeIndex + 1;
				pTerrain->SetVertexStatus(offset, 1);
				pTriangleStrips[*pCountStrips].m_pVertices[2] = offset;
				offset = pTerrain->GetWidthVertices() + m_HomeIndex + 1 + pTerrain->GetWidthVertices();
				pTerrain->SetVertexStatus(offset, 1);
				pTriangleStrips[*pCountStrips].m_pVertices[3] = offset;
				offset = pTerrain->GetWidthVertices() + m_HomeIndex + 2;
				pTerrain->SetVertexStatus(offset, 1);
				pTriangleStrips[*pCountStrips].m_pVertices[4] = offset;
				offset = pTerrain->GetWidthVertices() + m_HomeIndex + 2 + pTerrain->GetWidthVertices();
				pTerrain->SetVertexStatus(offset, 1);
				pTriangleStrips[*pCountStrips].m_pVertices[5] = offset;
				pTriangleStrips[*pCountStrips].m_NumberOfVertices = 6;
				pTriangleStrips[*pCountStrips].m_bEnabled = true;
				*pCountStrips = *pCountStrips + 1;
			}

			m_bIsActive = true;
		}
		else
		{
			if (pTerrain->m_MaximumVisibleBlockSize < m_Stride)
			{
				m_pChildren[0]->Tessellate(pMatModelView, pMatProjection, pViewport, pTriangleStrips, pCountStrips, pTerrain);
				m_pChildren[1]->Tessellate(pMatModelView, pMatProjection, pViewport, pTriangleStrips, pCountStrips, pTerrain);
				m_pChildren[2]->Tessellate(pMatModelView, pMatProjection, pViewport, pTriangleStrips, pCountStrips, pTerrain);
				m_pChildren[3]->Tessellate(pMatModelView, pMatProjection, pViewport, pTriangleStrips, pCountStrips, pTerrain);
				m_bIsActive = false;
				m_bChildrenActive = true;
			}
			else
			{
				// Check screen coordinates of center of each face of bounding box
				double screenTopX, screenTopY, screenTopZ, screenBottomX, screenBottomY, screenBottomZ;
				float halfWidth = (boundingBox.m_Max.x - boundingBox.m_Min.x) / 2;
				// calculate z half way up the BoundingBox
				float CenterZ = (boundingBox.m_Min.z + boundingBox.m_Max.z) * 0.5f;
				float screenDist;
				int tm = Settings::GetInstance()->GetTessellateMethod();

				float faceX, faceY, faceZ;	//FIXME: use faceX/Y/Z throughout rest of method too.
				// bottom face
				faceX = boundingBox.m_Min.x + halfWidth;
				faceY = boundingBox.m_Min.y + halfWidth;
				if (tm == Settings::TM_NEW)
				{
					faceZ = -halfWidth * m_BPlane_A - halfWidth * m_BPlane_B - m_BPlane_Max_D;
				}
				else
				{
					faceZ = boundingBox.m_Min.z;
				}
				gluProject(faceX, faceY, faceZ, pMatModelView, pMatProjection, (GLint *) pViewport, &screenBottomX, &screenBottomY, &screenBottomZ);
				// top face
				//faceZ = boundingBox.m_Max.z;
				faceZ = -halfWidth * m_BPlane_A - halfWidth * m_BPlane_B - m_BPlane_Min_D;
				gluProject(faceX, faceY, faceZ, pMatModelView, pMatProjection, (GLint *) pViewport, &screenTopX, &screenTopY, &screenTopZ);
				if ((tm == Settings::TM_NEW) || (tm == Settings::TM_OLD_NEW))
				{
					float deltaX, deltaY, deltaZ;
					deltaX = (float)(screenTopX - screenBottomX);
					deltaY = (float)(screenTopY - screenBottomY);
					float zweight = Settings::GetInstance()->GetTessellateZWeight();
					deltaZ = (float)(screenTopZ - screenBottomZ) * zweight;
					screenDist = (float)sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
				}
				else
				{
					float deltaX, deltaY, boxHeight;
					float screenDistHorizontal, screenDistVertical, screenDistNew;
					if (tm == Settings::TM_OLD_UPRIGHTONLY)
					{
						// Disregard DistHorizontal: assume we're vertical
						screenDistHorizontal = 0;
					}
					else
					{
						screenDistHorizontal = (float)fabs(screenTopX - screenBottomX);
						if (tm == Settings::TM_2D_ROLLONLY)
						{
							deltaX = (float)(screenTopX - screenBottomX);
							deltaY = (float)(screenTopY - screenBottomY);
							boxHeight = (float)sqrt(deltaX * deltaX + deltaY * deltaY);
						}
					}
					screenDistVertical = (float)fabs(screenTopY - screenBottomY);

					// four side faces
					gluProject(boundingBox.m_Min.x + halfWidth, boundingBox.m_Min.y, CenterZ, pMatModelView, pMatProjection, (GLint *) pViewport, &screenBottomX, &screenBottomY, &screenBottomZ);
					gluProject(boundingBox.m_Min.x + halfWidth, boundingBox.m_Max.y, CenterZ, pMatModelView, pMatProjection, (GLint *) pViewport, &screenTopX, &screenTopY, &screenTopZ);
					screenDistNew = (float)fabs(screenTopX - screenBottomX);
					if (screenDistNew > screenDistHorizontal)
						screenDistHorizontal = screenDistNew;
					if (tm != Settings::TM_OLD_UPRIGHTONLY)
					{
						screenDistNew = (float)fabs(screenTopY - screenBottomY);
						if (screenDistNew > screenDistVertical)
							screenDistVertical = screenDistNew;
					}

					gluProject(boundingBox.m_Min.x, boundingBox.m_Min.y + halfWidth, CenterZ, pMatModelView, pMatProjection, (GLint *) pViewport, &screenBottomX, &screenBottomY, &screenBottomZ);
					gluProject(boundingBox.m_Max.x, boundingBox.m_Min.y + halfWidth, CenterZ, pMatModelView, pMatProjection, (GLint *) pViewport, &screenTopX, &screenTopY, &screenTopZ);
					screenDistNew = (float)fabs(screenTopX - screenBottomX);
					if (screenDistNew > screenDistHorizontal)
						screenDistHorizontal = screenDistNew;
					if (tm != Settings::TM_OLD_UPRIGHTONLY)
					{
						screenDistNew = (float)fabs(screenTopY - screenBottomY);
						if (screenDistNew > screenDistVertical)
							screenDistVertical = screenDistNew;
					}

					// Use the smaller of vertical and horizontal screen size to decide whether or not the block should be simplified.
					screenDist = screenDistHorizontal < screenDistVertical ? screenDistHorizontal : screenDistVertical;
					if (tm == Settings::TM_2D_ROLLONLY)
					{
						if (boxHeight < screenDist)
							screenDist = boxHeight;
					}
				}

				if (screenDist <= pTerrain->GetDetailThreshold())
				{
					// This block is simplified, so add its triangles to the list and stop recursing.
					CreateTriangleStrip(pTriangleStrips, pCountStrips, pTerrain);
					m_bIsActive = true;
					m_bChildrenActive = false;
				}
				else
				{
					m_pChildren[0]->Tessellate(pMatModelView, pMatProjection, pViewport, pTriangleStrips, pCountStrips, pTerrain);
					m_pChildren[1]->Tessellate(pMatModelView, pMatProjection, pViewport, pTriangleStrips, pCountStrips, pTerrain);
					m_pChildren[2]->Tessellate(pMatModelView, pMatProjection, pViewport, pTriangleStrips, pCountStrips, pTerrain);
					m_pChildren[3]->Tessellate(pMatModelView, pMatProjection, pViewport, pTriangleStrips, pCountStrips, pTerrain);
					m_bIsActive = false;
					m_bChildrenActive = true;
				}
			}
		}
	}
	else
	{
		m_bIsActive = false;
		m_bChildrenActive = false;
	}
}

void TerrainBlock::CreateTriangleStrip(TriangleStrip * pTriangleStrips, unsigned int *pCount, Terrain * pTerrain)
{
	if (*pCount < pTerrain->m_MaxNumberOfPrimitives)
	{
		pTerrain->SetVertexStatus(m_HomeIndex, 1);
		pTriangleStrips[*pCount].m_pVertices[0] = m_HomeIndex;
		int offset = m_HomeIndex + pTerrain->GetWidthVertices() * m_Stride;
		pTerrain->SetVertexStatus(offset, 1);
		pTriangleStrips[*pCount].m_pVertices[1] = offset;
		offset = m_HomeIndex + m_Stride;
		pTerrain->SetVertexStatus(offset, 1);
		pTriangleStrips[*pCount].m_pVertices[2] = offset;
		offset = m_HomeIndex + m_Stride + pTerrain->GetWidthVertices() * m_Stride;
		pTerrain->SetVertexStatus(offset, 1);
		pTriangleStrips[*pCount].m_pVertices[3] = offset;
		pTriangleStrips[*pCount].m_bEnabled = true;
		pTriangleStrips[*pCount].m_NumberOfVertices = 4;

		m_pTriangleStrip = &pTriangleStrips[*pCount];
		*pCount = *pCount + 1;
	}
}

void TerrainBlock::EnableStrip(bool bEnabled)
{
	m_pTriangleStrip->m_bEnabled = false;
}

int TerrainBlock::GetStride()
{
	return m_Stride;
}

int TerrainBlock::GetHomeIndex()
{
	return m_HomeIndex;
}

void TerrainBlock::RepairCracks(Terrain * pTerrain, TriangleFan * pTriangleFans, unsigned int *pCountFans)
{
	if (2 < m_Stride)
	{
		if (m_bIsActive)
		{
			int halfStride = m_Stride / 2;
			int bottomLeft = m_HomeIndex + m_Stride * pTerrain->GetWidthVertices();
			int bottomRight = bottomLeft + m_Stride;
			int i, previousVertex;
			int v0;
			int numVertices = 0;

			bool bNeedToFix = false;
			for (i = m_HomeIndex + m_Stride - 1; m_HomeIndex < i && !bNeedToFix; i--)
				bNeedToFix = (pTerrain->GetVertexStatus(i) == 1);
			if (!bNeedToFix)
			{
				for (i = m_HomeIndex + pTerrain->GetWidthVertices(); i < m_HomeIndex + m_Stride * pTerrain->GetWidthVertices() && !bNeedToFix; i += pTerrain->GetWidthVertices())
					bNeedToFix = (pTerrain->GetVertexStatus(i) == 1);
				if (!bNeedToFix)
				{
					for (i = bottomLeft + 1; i < bottomRight && !bNeedToFix; i++)
						bNeedToFix = (pTerrain->GetVertexStatus(i) == 1);
					if (!bNeedToFix)
					{
						for (i = bottomRight - pTerrain->GetWidthVertices(); m_HomeIndex + m_Stride < i && !bNeedToFix; i -= pTerrain->GetWidthVertices())
							bNeedToFix = (pTerrain->GetVertexStatus(i) == 1);
					}
				}
			}

			if (bNeedToFix)
			{
				EnableStrip(false);
				v0 = m_HomeIndex + halfStride + halfStride * pTerrain->GetWidthVertices();
				assert(0 <= v0);
				assert(v0 < pTerrain->GetNumberOfVertices());
				pTriangleFans[*pCountFans].m_pVertices[0] = v0;
				numVertices = 0;
				for (i = m_HomeIndex + m_Stride; m_HomeIndex <= i; i--)
				{
					assert(0 <= i);
					assert(i < pTerrain->GetNumberOfVertices());
					if (pTerrain->GetVertexStatus(i) == 1)
					{
						if (++numVertices == MAX_VERTICES_PER_FAN - 1)
						{
							// We have reached the maximum size for a fan, so start a new fan.
							pTriangleFans[*pCountFans].m_NumberOfVertices = numVertices;
							*pCountFans = *pCountFans + 1;
							pTriangleFans[*pCountFans].m_pVertices[0] = v0;
							pTriangleFans[*pCountFans].m_pVertices[1] = previousVertex;
							numVertices = 2;
						}
						pTriangleFans[*pCountFans].m_pVertices[numVertices] = i;
						previousVertex = i;
					}
				}
				for (i = m_HomeIndex + pTerrain->GetWidthVertices(); i <= m_HomeIndex + m_Stride * pTerrain->GetWidthVertices(); i += pTerrain->GetWidthVertices())
				{
					assert(0 <= i);
					assert(i < pTerrain->GetNumberOfVertices());
					if (pTerrain->GetVertexStatus(i) == 1)
					{
						if (++numVertices == MAX_VERTICES_PER_FAN - 1)
						{
							// We have reached the maximum size for a fan, so start a new fan.
							pTriangleFans[*pCountFans].m_NumberOfVertices = numVertices;
							*pCountFans = *pCountFans + 1;
							pTriangleFans[*pCountFans].m_pVertices[0] = v0;
							pTriangleFans[*pCountFans].m_pVertices[1] = previousVertex;
							numVertices = 2;
						}
						pTriangleFans[*pCountFans].m_pVertices[numVertices] = i;
						previousVertex = i;
					}
				}
				for (i = bottomLeft; i <= bottomRight; i++)
				{
					assert(0 <= i);
					assert(i < pTerrain->GetNumberOfVertices());
					if (pTerrain->GetVertexStatus(i) == 1)
					{
						if (++numVertices == MAX_VERTICES_PER_FAN - 1)
						{
							// We have reached the maximum size for a fan, so start a new fan.
							pTriangleFans[*pCountFans].m_NumberOfVertices = numVertices;
							*pCountFans = *pCountFans + 1;
							pTriangleFans[*pCountFans].m_pVertices[0] = v0;
							pTriangleFans[*pCountFans].m_pVertices[1] = previousVertex;
							numVertices = 2;
						}
						pTriangleFans[*pCountFans].m_pVertices[numVertices] = i;
						previousVertex = i;
					}
				}
				for (i = bottomRight - pTerrain->GetWidthVertices(); m_HomeIndex + m_Stride <= i; i -= pTerrain->GetWidthVertices())
				{
					assert(0 <= i);
					assert(i < pTerrain->GetNumberOfVertices());
					if (pTerrain->GetVertexStatus(i) == 1)
					{
						if (++numVertices == MAX_VERTICES_PER_FAN - 1)
						{
							// We have reached the maximum size for a fan, so start a new fan.
							pTriangleFans[*pCountFans].m_NumberOfVertices = numVertices;
							*pCountFans = *pCountFans + 1;
							pTriangleFans[*pCountFans].m_pVertices[0] = v0;
							pTriangleFans[*pCountFans].m_pVertices[1] = previousVertex;
							numVertices = 2;
						}
						pTriangleFans[*pCountFans].m_pVertices[numVertices] = i;
						previousVertex = i;
					}
				}
				pTriangleFans[*pCountFans].m_NumberOfVertices = numVertices + 1;
				*pCountFans = *pCountFans + 1;
			}
		}
		else if (m_bChildrenActive)
		{
			m_pChildren[0]->RepairCracks(pTerrain, pTriangleFans, pCountFans);
			m_pChildren[1]->RepairCracks(pTerrain, pTriangleFans, pCountFans);
			m_pChildren[2]->RepairCracks(pTerrain, pTriangleFans, pCountFans);
			m_pChildren[3]->RepairCracks(pTerrain, pTriangleFans, pCountFans);
		}
	}
}

void TerrainBlock::VertexChanged(Terrain * pTerrain)
{
	CalculateGeometry(pTerrain);
	if (2 < m_Stride)
	{
		m_pChildren[0]->VertexChanged(pTerrain);
		m_pChildren[1]->VertexChanged(pTerrain);
		m_pChildren[2]->VertexChanged(pTerrain);
		m_pChildren[3]->VertexChanged(pTerrain);
	}
}

// speed up variant of VertexChanged by detecting blocks 
// where vertex changed
void TerrainBlock::VertexChanged(Terrain * pTerrain, int index)
{
	int y1 = m_HomeIndex / pTerrain->GetWidthVertices();
	int x1 = m_HomeIndex - y1 * pTerrain->GetWidthVertices();
	int last_index = m_HomeIndex + m_Stride * (pTerrain->GetWidthVertices() + 1);
	int y2 = last_index / pTerrain->GetWidthVertices();
	int x2 = last_index - y2 * pTerrain->GetWidthVertices();
	int y = index / pTerrain->GetWidthVertices();
	int x = index - y * pTerrain->GetWidthVertices();
	if (x >= x1 && x <= x2 && y >= y1 && y <= y2)
	{
		CalculateGeometry(pTerrain);
		if (2 < m_Stride)
		{
			m_pChildren[0]->VertexChanged(pTerrain, index);
			m_pChildren[1]->VertexChanged(pTerrain, index);
			m_pChildren[2]->VertexChanged(pTerrain, index);
			m_pChildren[3]->VertexChanged(pTerrain, index);
		}
	}
}

void TerrainBlock::VertexChanged(Terrain * pTerrain, int index1, int index2)
{
	// block coords
	int last_index = m_HomeIndex + m_Stride * (pTerrain->GetWidthVertices() + 1);

	// top left
	int block_y11 = m_HomeIndex / pTerrain->GetWidthVertices();
	int block_x11 = m_HomeIndex - block_y11 * pTerrain->GetWidthVertices();

	// bottom right
	int block_y22 = last_index / pTerrain->GetWidthVertices();
	int block_x22 = last_index - block_y22 * pTerrain->GetWidthVertices();

	// top right
	int block_x12 = block_x22;
	int block_y12 = block_y11;

	// bottom left
	int block_x21 = block_x11;
	int block_y21 = block_y22;

	// changed rectangle coords
	// top left
	int changed_y11 = index1 / pTerrain->GetWidthVertices();
	int changed_x11 = index1 - changed_y11 * pTerrain->GetWidthVertices();

	// bottom right
	int changed_y22 = index2 / pTerrain->GetWidthVertices();
	int changed_x22 = index2 - changed_y22 * pTerrain->GetWidthVertices();

	// top right
	int changed_x12 = changed_x22;
	int changed_y12 = changed_y11;

	// bottom left
	int changed_x21 = changed_x11;
	int changed_y21 = changed_y22;

	// detect of intersection of two boxes ((block_x11, block_y11) - (block_x22, block_y22))
	// and ((changed_x11, changed_y11) - (changed_x22, changed_y22))
	if ((changed_x11 >= block_x11 && changed_x11 <= block_x22 && changed_y11 >= block_y11 && changed_y11 <= block_y22) || (changed_x12 >= block_x11 && changed_x12 <= block_x22 && changed_y12 >= block_y11 && changed_y12 <= block_y22) || (changed_x21 >= block_x11 && changed_x21 <= block_x22 && changed_y21 >= block_y11 && changed_y21 <= block_y22) || (changed_x22 >= block_x11 && changed_x22 <= block_x22 && changed_y22 >= block_y11 && changed_y22 <= block_y22) || (block_x11 >= changed_x11 && block_x11 <= changed_x22 && block_y11 >= changed_y11 && block_y11 <= changed_y22) || (block_x12 >= changed_x11 && block_x12 <= changed_x22 && block_y12 >= changed_y11 && block_y12 <= changed_y22) || (block_x21 >= changed_x11 && block_x21 <= changed_x22 && block_y21 >= changed_y11 && block_y21 <= changed_y22) || (block_x22 >= changed_x11 && block_x22 <= changed_x22 && block_y22 >= changed_y11 && block_y22 <= changed_y22))
	{
		CalculateGeometry(pTerrain);
		if (2 < m_Stride)
		{
			m_pChildren[0]->VertexChanged(pTerrain, index1, index2);
			m_pChildren[1]->VertexChanged(pTerrain, index1, index2);
			m_pChildren[2]->VertexChanged(pTerrain, index1, index2);
			m_pChildren[3]->VertexChanged(pTerrain, index1, index2);
		}
	}
}


void TerrainBlock::CalculateGeometry(Terrain * pTerrain)
{
	// Calculate slope of best plane approximation of TerrainBlock
	//FIXME: (later, don't optimize too early) - decrease multiplies by adding first.
	//       or include e.g. the 0.5 in the scaling factor below.
	m_BPlane_A = 0.5f * pTerrain->GetElevation(m_HomeIndex) - 0.5f * pTerrain->GetElevation(m_HomeIndex + m_Stride) - 0.5f * pTerrain->GetElevation(m_HomeIndex + m_Stride + pTerrain->GetWidthVertices() * m_Stride) + 0.5f * pTerrain->GetElevation(m_HomeIndex + pTerrain->GetWidthVertices());
	m_BPlane_B = 0.5f * pTerrain->GetElevation(m_HomeIndex) + 0.5f * pTerrain->GetElevation(m_HomeIndex + m_Stride) - 0.5f * pTerrain->GetElevation(m_HomeIndex + m_Stride + pTerrain->GetWidthVertices() * m_Stride) - 0.5f * pTerrain->GetElevation(m_HomeIndex + pTerrain->GetWidthVertices());
	float VertexSpacing = pTerrain->GetVertexSpacing();
	float scalefactor = m_Stride * VertexSpacing;
	m_BPlane_A /= scalefactor;
	m_BPlane_B /= scalefactor;

	// Find this block's bounding box.
	m_MinElevation = pTerrain->GetElevation(m_HomeIndex);
	m_MaxElevation = pTerrain->GetElevation(m_HomeIndex);
	m_BPlane_Min_D = -pTerrain->GetElevation(m_HomeIndex);	// top plane
	m_BPlane_Max_D = -pTerrain->GetElevation(m_HomeIndex);	// bottom plane
	int WidthVertices = pTerrain->GetWidthVertices();
	for (int i = 0; i <= m_Stride; i++)
	{
		for (int j = 0; j <= m_Stride; j++)
		{
			float elevation = pTerrain->GetElevation(m_HomeIndex + i * WidthVertices + j);
			float planeD = -m_BPlane_A * (j * VertexSpacing) - m_BPlane_B * (i * VertexSpacing) - elevation;
			if (planeD < m_BPlane_Min_D)
				m_BPlane_Min_D = planeD;
			if (m_BPlane_Max_D < planeD)
				m_BPlane_Max_D = planeD;
			if (elevation < m_MinElevation)
				m_MinElevation = elevation;
			if (m_MaxElevation < elevation)
				m_MaxElevation = elevation;
		}
	}

#if _USE_RAYTRACING_SUPPORT_ > 0
// Build triangles for ray intersection and collision detection.
	if (m_Stride == 2)
	{
		m_pTriangles = new Triangle[8];
		m_pTriangles[0].DefineFromPoints(pTerrain->m_pVertices[m_HomeIndex], pTerrain->m_pVertices[m_HomeIndex + pTerrain->GetWidthVertices()], pTerrain->m_pVertices[m_HomeIndex + 1]);
		m_pTriangles[1].DefineFromPoints(pTerrain->m_pVertices[m_HomeIndex + 1], pTerrain->m_pVertices[m_HomeIndex + pTerrain->GetWidthVertices()], pTerrain->m_pVertices[m_HomeIndex + pTerrain->GetWidthVertices() + 1]);
		m_pTriangles[2].DefineFromPoints(pTerrain->m_pVertices[m_HomeIndex + 1], pTerrain->m_pVertices[m_HomeIndex + 1 + pTerrain->GetWidthVertices()], pTerrain->m_pVertices[m_HomeIndex + 2]);
		m_pTriangles[3].DefineFromPoints(pTerrain->m_pVertices[m_HomeIndex + 2], pTerrain->m_pVertices[m_HomeIndex + 1 + pTerrain->GetWidthVertices()], pTerrain->m_pVertices[m_HomeIndex + 2 + pTerrain->GetWidthVertices()]);
		m_pTriangles[4].DefineFromPoints(pTerrain->m_pVertices[m_HomeIndex + pTerrain->GetWidthVertices()], pTerrain->m_pVertices[m_HomeIndex + pTerrain->GetWidthVertices() + pTerrain->GetWidthVertices()], pTerrain->m_pVertices[m_HomeIndex + pTerrain->GetWidthVertices() + 1]);
		m_pTriangles[5].DefineFromPoints(pTerrain->m_pVertices[m_HomeIndex + pTerrain->GetWidthVertices() + 1], pTerrain->m_pVertices[m_HomeIndex + pTerrain->GetWidthVertices() + pTerrain->GetWidthVertices()], pTerrain->m_pVertices[m_HomeIndex + pTerrain->GetWidthVertices() + pTerrain->GetWidthVertices() + 1]);
		m_pTriangles[6].DefineFromPoints(pTerrain->m_pVertices[m_HomeIndex + pTerrain->GetWidthVertices() + 1], pTerrain->m_pVertices[m_HomeIndex + pTerrain->GetWidthVertices() + 1 + pTerrain->GetWidthVertices()], pTerrain->m_pVertices[m_HomeIndex + pTerrain->GetWidthVertices() + 2]);
		m_pTriangles[7].DefineFromPoints(pTerrain->m_pVertices[m_HomeIndex + pTerrain->GetWidthVertices() + 2], pTerrain->m_pVertices[m_HomeIndex + pTerrain->GetWidthVertices() + 1 + pTerrain->GetWidthVertices()], pTerrain->m_pVertices[m_HomeIndex + pTerrain->GetWidthVertices() + 2 + pTerrain->GetWidthVertices()]);
	}
#endif
}


#if _USE_RAYTRACING_SUPPORT_ > 0
void TerrainBlock::IntersectRay(const Ray & ray, Vector & intersectionPoint, float &lowestDistance, const Terrain * pTerrain)
{
// First test ray against this block's bounding box.
	Box boundingBox;
	float width = m_Stride * pTerrain->GetVertexSpacing();
	boundingBox.m_Min.x = pTerrain->m_pVertices[m_HomeIndex].x;
	boundingBox.m_Min.y = pTerrain->m_pVertices[m_HomeIndex].y;
	boundingBox.m_Min.z = m_MinElevation;
	boundingBox.m_Max.x = boundingBox.m_Min.x + width;
	boundingBox.m_Max.y = boundingBox.m_Min.y + width;
	boundingBox.m_Max.z = m_MaxElevation;
	if (RayBoxIntersect(&ray, &boundingBox, NULL, NULL))
	{
		if (2 < m_Stride)
		{
			m_pChildren[0]->IntersectRay(ray, intersectionPoint, lowestDistance, pTerrain);
			m_pChildren[1]->IntersectRay(ray, intersectionPoint, lowestDistance, pTerrain);
			m_pChildren[2]->IntersectRay(ray, intersectionPoint, lowestDistance, pTerrain);
			m_pChildren[3]->IntersectRay(ray, intersectionPoint, lowestDistance, pTerrain);
		}
		else
		{
			float distance;
			Vector point;
			for (int i = 0; i < 8; i++)
			{
				if (RayPlaneIntersect(&ray, m_pTriangles[i].GetPlane(), &point, &distance) == 1)
				{
					if (i == 0 || i == 2 || i == 4 || i == 6)
					{
						if (m_pTriangles[i].GetVertex(0)->x <= point.x && m_pTriangles[i].GetVertex(0)->y <= point.y && point.x <= m_pTriangles[i].GetVertex(2)->x && point.y <= m_pTriangles[i].GetVertex(1)->y && ((fmod(point.y, pTerrain->m_VertexSpacing) + fmod(point.x, pTerrain->m_VertexSpacing)) <= pTerrain->m_VertexSpacing))
						{
							if (distance < lowestDistance)
							{
								lowestDistance = distance;
								intersectionPoint.x = point.x;
								intersectionPoint.y = point.y;
								intersectionPoint.z = point.z;
							}
						}
					}
					else
					{
						if (m_pTriangles[i].GetVertex(1)->x <= point.x && m_pTriangles[i].GetVertex(0)->y <= point.y && point.x <= m_pTriangles[i].GetVertex(2)->x && point.y <= m_pTriangles[i].GetVertex(1)->y && ((fmod(point.y, pTerrain->m_VertexSpacing) + fmod(point.x, pTerrain->m_VertexSpacing)) >= pTerrain->m_VertexSpacing))
						{
							if (distance < lowestDistance)
							{
								lowestDistance = distance;
								intersectionPoint.x = point.x;
								intersectionPoint.y = point.y;
								intersectionPoint.z = point.z;
							}
						}
					}
				}
			}
		}
	}
}
#endif
