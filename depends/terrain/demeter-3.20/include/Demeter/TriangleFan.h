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

#ifndef _TRIANGLE_FAN_DEMETER_H_
#define _TRIANGLE_FAN_DEMETER_H_

#include "Demeter/Terrain.h"
#include "Demeter/TerrainBlock.h"

/// \brief The Demeter Terrain Engine.
namespace Demeter
{
	class TERRAIN_API TriangleFan
	{
	      public:
		TriangleFan();
		~TriangleFan();
		void Render(Terrain * pTerrain);
		void Setup(Terrain * pTerrain);
	      private:
		int m_pVertices[MAX_VERTICES_PER_FAN];	// Indices into the terrain vertices
		// TBD: It is a pretty awful waste of memory to preallocate MAX_VERTICES_PER_FAN vertices for every triangle fan,
		// when in most cases only a few vertices are needed. However, dynamically allocating these vertices
		// during every tessellation is not an option either because it causes huge performance problems and
		// badly fragments memory. Any good ideas for this?
		unsigned char m_NumberOfVertices;
		float minX, minY;
		unsigned int textureId;

		friend class TerrainBlock;
		friend class Terrain;
	};
}

#endif
