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

#ifndef _TEXTURE_SET_DEMETER_H_
#define _TEXTURE_SET_DEMETER_H_

#include "Demeter/Globals.h"
#include "Demeter/Terrain.h"
#include "Demeter/Texture.h"

#include <stdlib.h>
#include <vector>

/// \brief The Demeter Terrain Engine.
namespace Demeter
{
	//class Terrain;
	//class vector;
	//class Texture;

	/// \brief A TextureSet is a pool of Texture objects that are used by a Terrain to do detail texture rendering.

	/// You use TextureSet when your application needs to do detail painting (or "splats.")
	/// You can get a Terrain's TextureSet by calling Terrain::GetTextureSet(). Then you may add Texture objects to
	/// it by calling TextureSet::AddTexture() as many times as needed to fill it with all of the Texture objects you
	/// want to do detail painting with. Each of these Texture objects now has a unique index that you can get by
	/// calling Texture::GetSharedIndex(). Use this index to call the Terrain::Paint() method or any method that requires
	/// a shared index of a Texture.
	class TERRAIN_API TextureSet
	{
	      public:
		/// \brief Constructs a new TextureSet.

		/// Since Terrain automatically creates one of these for you, you should rarely need to
		/// create new instances of this class manually. You might do so if you are making several
		/// instances of Terrain share a single TextureSet.
		TextureSet();
		~TextureSet();
		/// \brief Adds a Texture to the TextureSet.

		/// This gives the Texture a shared index which can be used to identify the Texture as a
		/// part of the Terrain's pool of shared Textures.
		int AddTexture(Texture * pTexture);
		/// \brief Gets a Texture by its shared index.

		/// \param index The index of the Texture to fetch from the pool. This is the value returned by Texture::GetSharedIndex().
		Texture *GetTexture(int index);
		/// \brief Indicates how many Texture objects are in the TextureSet.
		int GetNumTextures();
		/// \brief Gets the standard filename that the Demeter Texture Editor will use when storing shared textures from this TextureSet.
		void GetSharedTextureFilename(int index, char *szFilename, int bufferSize);
		/// \brief Writes this TextureSet and all of its shared Textures to a binary file.                                                              
		void Write(FILE * file, Terrain * pTerrain);
		/// \brief Reads this TextureSet and all of its shared Textures from a binary file.
		void Read(FILE * file, Terrain * pTerrain);
	      private:
		  std::vector < Texture * >m_Textures;
	};
}

#endif
