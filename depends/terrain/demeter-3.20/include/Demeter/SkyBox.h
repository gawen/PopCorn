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

#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#include "Demeter/Globals.h"
#include "Demeter/Terrain.h"

namespace Demeter
{
	enum BOXSIDES
	{
		BOX_LEFT = 0,
		BOX_FRONT = 1,
		BOX_RIGHT = 2,
		BOX_BACK = 3,
		BOX_BOTTOM = 4,
		BOX_TOP = 5
	};

	class TERRAIN_API SkyBox
	{
	      public:
		SkyBox(float diameter, Demeter::Texture * pTextureLeft, Demeter::Texture * pTextureFront, Demeter::Texture * pTextureRight, Demeter::Texture * pTextureBack, Demeter::Texture * pTextureTop, Demeter::Texture * pTextureBottom);
		 ~SkyBox();
		void Render();
		void SetCameraPosition(float x, float y, float z);
	      private:
		  Demeter::Texture ** m_pTextures;
		  Demeter::Vector m_BottomLeft, m_TopRight;
		  Demeter::Vector m_CameraPos;
	};
}

#endif
