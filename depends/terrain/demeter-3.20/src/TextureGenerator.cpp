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

#include "Demeter/TextureGenerator.h"

using namespace Demeter;

void TextureGenerator::Init(Terrain * pTerrain, int textureWidth, int textureHeight)
{
	m_pTerrain = pTerrain;
	m_TextureWidth = textureWidth;
	m_TextureHeight = textureHeight;
}

void TextureGenerator::GetPosition(int texelX, int texelY, float &worldX, float &worldY)
{
	worldX = ((float)texelX / (float)m_TextureWidth) * m_pTerrain->GetWidth();
	worldY = ((float)texelY / (float)m_TextureHeight) * m_pTerrain->GetHeight();
}
