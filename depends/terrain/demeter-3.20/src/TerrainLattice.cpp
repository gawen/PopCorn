// Demeter Terrain Visualization Library by Clay Fowler
// Copyright (C) 2002 Clay Fowler

// $ID$

/*
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this library; if not, write to the
Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA  02111-1307, USA.
*/
#include "Demeter/TerrainLattice.h"

using namespace Demeter;
using namespace std;

TerrainLattice::TerrainLattice(TerrainLatticeLoader * pLoader)
{
	m_pLoader = pLoader;
	m_WidthTerrains = pLoader->GetNumberOfTerrainsX();
	m_HeightTerrains = pLoader->GetNumberOfTerrainsY();
	m_pTerrains = new Terrain *[m_WidthTerrains * m_HeightTerrains];
	for (int i = 0; i < m_WidthTerrains * m_HeightTerrains; i++)
		m_pTerrains[i] = NULL;
	m_WidthActiveTerrains = m_HeightActiveTerrains = 1;
	m_TerrainWidth = pLoader->GetTerrainWidth();
	m_TerrainHeight = pLoader->GetTerrainHeight();
}

TerrainLattice::~TerrainLattice()
{
	delete[]m_pTerrains;
}

void TerrainLattice::AddTerrain(Terrain * pTerrain, int positionX, int positionY)
{
	m_pTerrains[positionY * m_WidthTerrains + positionX] = pTerrain;
	pTerrain->SetLatticePosition(positionX, positionY);
	pTerrain->SetOffset(positionX * m_TerrainWidth, positionY * m_TerrainHeight);
}

Terrain *TerrainLattice::GetTerrain(int positionX, int positionY)
{
	return m_pTerrains[positionY * m_WidthTerrains + positionX];
}

Terrain *TerrainLattice::GetTerrainRelative(Terrain * pTerrain, Demeter::DIRECTION direction)
{
	int offsetX, offsetY;
	switch (direction)
	{
	case Demeter::DIR_NORTH:
		offsetX = 0;
		offsetY = 1;
		break;
	case Demeter::DIR_NORTHEAST:
		offsetX = 1;
		offsetY = 1;
		break;
	case Demeter::DIR_EAST:
		offsetX = 1;
		offsetY = 0;
		break;
	case Demeter::DIR_SOUTHEAST:
		offsetX = 1;
		offsetY = -1;
		break;
	case Demeter::DIR_SOUTH:
		offsetX = 0;
		offsetY = -1;
		break;
	case Demeter::DIR_SOUTHWEST:
		offsetX = -1;
		offsetY = -1;
		break;
	case Demeter::DIR_WEST:
		offsetX = -1;
		offsetY = 0;
		break;
	case Demeter::DIR_NORTHWEST:
		offsetX = -1;
		offsetY = 1;
		break;
	case Demeter::DIR_CENTER:
	case Demeter::DIR_INVALID:
		break;
	}
	return GetTerrainRelative(pTerrain, offsetX, offsetY);
}


Terrain *TerrainLattice::GetTerrainRelative(Terrain * pTerrain, int positionX, int positionY)
{
	Terrain *pRequestedTerrain = NULL;
	if (-1 <= positionX && positionX <= 1 && -1 <= positionY && positionY <= 1)
	{
		int x, y;
		pTerrain->GetLatticePosition(x, y);
		x += positionX;
		y += positionY;

		Terrain *pTerrainCenter = m_pTerrains[m_CurrentTerrainIndex[Demeter::DIR_CENTER]];
		int centerX, centerY;
		pTerrainCenter->GetLatticePosition(centerX, centerY);
		if (abs(x - centerX) <= m_WidthActiveTerrains && abs(y - centerY) <= m_HeightActiveTerrains)
		{
			if (0 <= x && x < m_WidthTerrains && 0 <= y && y < m_HeightTerrains)
				pRequestedTerrain = GetTerrain(x, y);
		}
	}
	return pRequestedTerrain;
}

Terrain *TerrainLattice::GetTerrainAtPoint(float x, float y)
{
	int indexX = (int)x / (int)m_TerrainWidth;
	int indexY = (int)y / (int)m_TerrainHeight;
	if (0 <= indexX && indexX < m_WidthTerrains && 0 <= indexY && indexY < m_HeightTerrains)
		return GetTerrain(indexX, indexY);
	else
		return NULL;
}

void TerrainLattice::SetCameraPosition(float x, float y, float z)
{
	int indexX = (int)x / (int)m_TerrainWidth;
	int indexY = (int)y / (int)m_TerrainHeight;
	m_CurrentTerrainIndex[Demeter::DIR_CENTER] = indexY * m_WidthTerrains + indexX;
	m_CurrentTerrainIndex[Demeter::DIR_SOUTH] = 0 < indexY ? m_CurrentTerrainIndex[Demeter::DIR_CENTER] - m_WidthTerrains : -1;
	m_CurrentTerrainIndex[Demeter::DIR_SOUTHEAST] = (0 < indexY && indexX < m_WidthTerrains - 1) ? m_CurrentTerrainIndex[Demeter::DIR_CENTER] - m_WidthTerrains + 1 : -1;
	m_CurrentTerrainIndex[Demeter::DIR_SOUTHWEST] = 0 < indexX && 0 < indexY ? m_CurrentTerrainIndex[Demeter::DIR_CENTER] - m_WidthTerrains - 1 : -1;
	m_CurrentTerrainIndex[Demeter::DIR_EAST] = indexX < m_WidthTerrains - 1 ? m_CurrentTerrainIndex[Demeter::DIR_CENTER] + 1 : -1;
	m_CurrentTerrainIndex[Demeter::DIR_WEST] = 0 < indexX ? m_CurrentTerrainIndex[Demeter::DIR_CENTER] - 1 : -1;
	m_CurrentTerrainIndex[Demeter::DIR_NORTH] = indexY < m_HeightTerrains - 1 ? m_CurrentTerrainIndex[Demeter::DIR_CENTER] + m_WidthTerrains : -1;
	m_CurrentTerrainIndex[Demeter::DIR_NORTHEAST] = (indexX < m_WidthTerrains - 1 && indexY < m_HeightTerrains - 1) ? m_CurrentTerrainIndex[Demeter::DIR_CENTER] + m_WidthTerrains + 1 : -1;
	m_CurrentTerrainIndex[Demeter::DIR_NORTHWEST] = (indexY < m_HeightTerrains - 1 && 0 < indexX) ? m_CurrentTerrainIndex[Demeter::DIR_CENTER] + m_WidthTerrains - 1 : -1;

	for (int i = 0; i < m_WidthTerrains * m_HeightTerrains; i++)
	{
		bool active = false;
		for (int dir = 0; dir < 9 && !active; dir++)
			active = (i == m_CurrentTerrainIndex[dir]);
		if (!active && m_pTerrains[i] != NULL)
		{
			delete m_pTerrains[i];
			m_pTerrains[i] = NULL;
		}
		else if (active && m_pTerrains[i] == NULL)
		{
			LoadTerrain(i);
		}
	}
}

void TerrainLattice::LoadTerrain(int index)
{
	int indexX = index % m_WidthTerrains;
	int indexY = index / m_WidthTerrains;
	Terrain *pTerrain = m_pLoader->LoadTerrainAt(indexX, indexY);
	AddTerrain(pTerrain, indexX, indexY);
}

void TerrainLattice::SetDetailThreshold(float threshold)
{
	for (int i = 0; i < 9; i++)
	{
		if (0 <= m_CurrentTerrainIndex[i])
		{
			m_pTerrains[m_CurrentTerrainIndex[i]]->SetDetailThreshold(threshold);
		}
	}
}

float TerrainLattice::GetElevation(float x, float y)
{
	Terrain *pTerrain = GetTerrainAtPoint(x, y);
	if (pTerrain != NULL)
		return pTerrain->GetElevation(x, y);
	else
		return 0.0f;
}

Demeter::DIRECTION TerrainLattice::GetOppositeDirection(Demeter::DIRECTION direction)
{
	Demeter::DIRECTION oppositeDirection;
	switch (direction)
	{
	case Demeter::DIR_NORTH:
		oppositeDirection = Demeter::DIR_SOUTH;
		break;
	case Demeter::DIR_NORTHEAST:
		oppositeDirection = Demeter::DIR_SOUTHWEST;
		break;
	case Demeter::DIR_EAST:
		oppositeDirection = Demeter::DIR_WEST;
		break;
	case Demeter::DIR_SOUTHEAST:
		oppositeDirection = Demeter::DIR_NORTHWEST;
		break;
	case Demeter::DIR_SOUTH:
		oppositeDirection = Demeter::DIR_NORTH;
		break;
	case Demeter::DIR_SOUTHWEST:
		oppositeDirection = Demeter::DIR_NORTHEAST;
		break;
	case Demeter::DIR_WEST:
		oppositeDirection = Demeter::DIR_EAST;
		break;
	case Demeter::DIR_NORTHWEST:
		oppositeDirection = Demeter::DIR_SOUTHEAST;
		break;
	case Demeter::DIR_CENTER:
		oppositeDirection = Demeter::DIR_CENTER;
		break;
	case Demeter::DIR_INVALID:
		break;
	}
	return oppositeDirection;
}

void TerrainLattice::ModelViewMatrixChanged()
{
	int i;
	for (i = 0; i < 9; i++)
	{
		if (m_CurrentTerrainIndex[i] != -1)
			m_pTerrains[m_CurrentTerrainIndex[i]]->ModelViewMatrixChanged();
	}

	for (i = 0; i < 9; i++)
	{
		if (m_CurrentTerrainIndex[i] != -1)
		{
			for (int direction = 0; direction < 8; direction++)
			{
				if (direction != Demeter::DIR_CENTER)
				{
					Terrain *pTerrain = GetTerrainRelative(m_pTerrains[m_CurrentTerrainIndex[i]], (Demeter::DIRECTION) direction);
					if (pTerrain != NULL)
					{
						m_pTerrains[m_CurrentTerrainIndex[i]]->UpdateNeighbor(pTerrain, (Demeter::DIRECTION) direction);
						pTerrain->UpdateNeighbor(m_pTerrains[m_CurrentTerrainIndex[i]], GetOppositeDirection((Demeter::DIRECTION) direction));
					}
				}
			}
		}
	}

	for (i = 0; i < 9; i++)
	{
		if (m_CurrentTerrainIndex[i] != -1)
			m_pTerrains[m_CurrentTerrainIndex[i]]->m_pRootBlock->RepairCracks(m_pTerrains[m_CurrentTerrainIndex[i]], m_pTerrains[m_CurrentTerrainIndex[i]]->m_pTriangleFans, &m_pTerrains[m_CurrentTerrainIndex[i]]->m_CountFans);
	}

}

void TerrainLattice::Render()
{
	for (int i = 0; i < 9; i++)
	{
		if (m_CurrentTerrainIndex[i] != -1)
			m_pTerrains[m_CurrentTerrainIndex[i]]->Render();
	}
}

float TerrainLattice::GetWidth()
{
	return m_WidthTerrains * m_TerrainWidth;
}

float TerrainLattice::GetHeight()
{
	return m_HeightTerrains * m_TerrainHeight;
}
