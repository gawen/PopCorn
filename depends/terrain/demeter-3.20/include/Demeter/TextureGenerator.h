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

#ifndef _TEXTURE_GENERATOR_DEMETER_H_
#define _TEXTURE_GENERATOR_DEMETER_H_

#include "Demeter/Terrain.h"

/// \brief The Demeter Terrain Engine.
namespace Demeter
{

	class TERRAIN_API TextureGenerator
	{
	      public:
		virtual void Init(Terrain * pTerrain, int textureWidth, int textureHeight);
		virtual void Generate() = 0;
	      protected:
		void GetPosition(int texelX, int texelY, float &worldX, float &worldY);
		Terrain *m_pTerrain;
		int m_TextureWidth, m_TextureHeight;
	};
}

#endif
