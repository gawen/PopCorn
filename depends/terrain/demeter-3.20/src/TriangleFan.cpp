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

#include "Demeter/TriangleFan.h"

using namespace Demeter;

TriangleFan::TriangleFan()
{
}

TriangleFan::~TriangleFan()
{
}

void TriangleFan::Setup(Terrain * pTerrain)
{
	minX = pTerrain->m_pVertices[m_pVertices[0]].x;
	minY = pTerrain->m_pVertices[m_pVertices[0]].y;
	for (int i = 1; i < m_NumberOfVertices; i++)
	{
		if (pTerrain->m_pVertices[m_pVertices[i]].x < minX)
			minX = pTerrain->m_pVertices[m_pVertices[i]].x;
		if (pTerrain->m_pVertices[m_pVertices[i]].y < minY)
			minY = pTerrain->m_pVertices[m_pVertices[i]].y;
	}
	int tileY = (int)((minY - pTerrain->m_OffsetY) / pTerrain->GetTextureTileHeight());
	int tileX = (int)((minX - pTerrain->m_OffsetX) / pTerrain->GetTextureTileWidth());
	textureId = tileY * pTerrain->GetNumberOfTextureTilesWidth() + tileX;
}

void TriangleFan::Render(Terrain * pTerrain)
{
	glDrawElements(GL_TRIANGLE_FAN, m_NumberOfVertices, GL_UNSIGNED_INT, m_pVertices);
}
