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

#ifndef _BRUSH_H_
#define _BRUSH_H_

#ifdef _WIN32
#ifdef TERRAIN_EXPORTS
#define BRUSH_API __declspec(dllexport)
#else
#define BRUSH_API __declspec(dllimport)
#endif
#else
#define BRUSH_API
#endif

#include "Demeter/Terrain.h"

/// \brief The Demeter Terrain Engine.
namespace Demeter
{
	/// \brief A "brush" that can be used to paint detail texture "splats" on a Terrain object's surface.

	/// This one simple example of how a brush can be implemented. This brush paints in a circular pattern, with
	/// opaque texture at the center of the splat and decreasing opaqueness as you move along the radius to the outside
	/// of the circle. You may derive from this class to create brushed of other shapes and behaviors for detail painting.
	class BRUSH_API Brush
	{
	      public:
		Brush(int width);
		virtual ~Brush();
		void SetWidth(int width);
		virtual void SetErase(bool bErase);
		virtual void SetIntensity(float intensity);
		virtual void SetMaxIntensity(float maxIntensity);
		virtual void Paint(Terrain * pTerrain, int detailTextureIndex, float x, float y);
	      private:
		void Paint(Terrain * pTerrain, int detailTextureIndex, int cellX, int cellY, int x, int y);
		void BuildBuffer();
		unsigned char *m_pBuffer;
		int m_Width;
		float m_Intensity;
		float m_MaxIntensity;
		bool m_bErase;
	};
}

#endif
