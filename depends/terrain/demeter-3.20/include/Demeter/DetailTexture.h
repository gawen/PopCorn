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

#ifndef _DETAIL_TEXTURE_DEMETER_H_
#define _DETAIL_TEXTURE_DEMETER_H_

#include "Demeter/Globals.h"
#include "Demeter/Texture.h"
#include "Demeter/Terrain.h"

/// \brief The Demeter Terrain Engine.
namespace Demeter
{
	//class Texture;
	//class Terrain;

	/// \brief A DetailTexture is a "layer" that can be painted onto to put texture "splats" on a terrain surface.

	/// You should rarely need to use this class directly. Demeter uses it to manage alpha blending of detail textures
	/// onto the terrain surface. There is one instance of this class for each texture "cell" in a Terrain. The number
	/// texture cells in a Terrain is set in one of two ways: (the size of the overall terrain texture / 256) ^ 2, OR by
	/// what you specify in a call to Terrain::AllocateTextureCells().
	class TERRAIN_API DetailTexture
	{
	      public:
		/// \brief Constructs a new DetailTexture that allows splatting of the specified Texture as a detail texture.
		DetailTexture(Texture * pTexture = NULL);
		~DetailTexture();
		/// \brief Sets the alpha mask to be used for this painting layer. This should be an alpha-only Texture.
		void SetMask(Texture *);
		/// \brief Sets the Texture that this painting layer uses as a detail texture.
		void SetTexture(Texture *);
		/// \brief Binds the mask layer to an OpenGL texture object.
		GLuint BindMask();
		/// \brief Binds the detail texture to an OpenGL texture object.
		GLuint BindTexture();
		/// \brief Unloads both the mask and detail OpenGL texture objects.
		void Unbind();
		/// \brief Gets the alpha mask layer.
		Texture *GetMask();
		/// \brief Gets the detail texture.
		Texture *GetTexture();
		/// \brief Writes both layers of the DetailTexture to a binary file that can be recreated by calling Read() on that file.
		void Write(FILE * file, Terrain * pTerrain);
		/// \brief Recreates a DetailTexture object from the specified binary file.
		void Read(FILE * file, Terrain * pTerrain);
		/// \brief Works exactly like Texture::FlipVertical().
		void FlipVertical();
		/// \brief Works exactly like Texture::FlipHorizontal().
		void FlipHorizontal();
	      private:
		  Texture * m_pMask;
		Texture *m_pTexture;
	};
}

#endif
