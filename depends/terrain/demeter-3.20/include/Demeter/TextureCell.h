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

#ifndef _TEXTURE_CELL_DEMETER_H_
#define _TEXTURE_CELL_DEMETER_H_

#include "Demeter/Globals.h"
#include "Demeter/DetailTexture.h"
#include "Demeter/Texture.h"
#include "Demeter/Terrain.h"

/// \brief The Demeter Terrain Engine.
namespace Demeter
{
	/// \brief Represents a square region on a Terrain surface that is textured.

	/// A Terrain's surface is divided into a grid of square regions. Each of these regions, a TextureCell,
	/// contains a texture which is part of the overall terrain texture (usually an RGB texture), and any number
	/// of detail textures (possibly zero.) Each DetailTexture is blending according to its alpha mask with the
	/// overall texture underneath it (and with any other DetailTextures underneath it.) The number
	/// texture cells in a Terrain is set in one of two ways: (the size of the overall terrain texture / 256) ^ 2, OR by
	/// what you specify in a call to Terrain::AllocateTextureCells(). The latter option is only useful if you are
	/// managing the overall terrain texture for yourself rather than letting Demeter do it automatically.
	/// There is virtually no reason to create instances of this class for yourself unless you are overriding the way
	/// Terrain manages its cells. Generally, you will use the instances of this class that are provided for you by the
	/// managing Terrain object.
	class TERRAIN_API TextureCell
	{
	      public:
		/// \brief Constructs an empty TextureCell.
		TextureCell();
		/// \brief Constructs a TextureCell that fits in the specified slot on the managing Terrain.
		/// \param index Specifies where in the Terrain's grid of TextureCells this cell should be placed.
		TextureCell(int index);
		 ~TextureCell();
		/// \brief Sets the overall Texture for this cell.
		void SetTexture(Texture * pTexture);
		/// \brief Adds a new detail texture layer to this cell.

		/// Any number of layers can be added. Layers will be rendered in the same order they were added.
		void AddDetail(DetailTexture * pDetailTexture);
		/// \brief Binds the overall texture to an OpenGL texture object.
		GLuint BindTexture();
		/// \brief Gets the number of DetailTexture layers that have been added.
		int GetNumberOfDetails();
		/// \brief Binds the mask of the specified DetailTexture layer.
		GLuint BindMask(int index);
		/// \brief Binds the detail texture of the specified DetailTexture layer.
		GLuint BindDetail(int index);
		/// \brief Unbinds the overall texture and all DetailTexture masks and detail textures.
		void UnbindAll();
		/// \brief Gets the specified DetailTexture layer.
		DetailTexture *GetDetail(int index);
		/// \brief Gets the specified DetailTexture layer.
		DetailTexture *GetDetail(Texture * pTexture);
		/// \brief Gets the overall texture.
		Texture *GetTexture();
		/// \brief Writes the entire cell (including all of its layers) to a binary file which can later be recreated by calling Read().
		void Write(FILE * file, Terrain * pTerrain);
		/// \brief Reads the entire cell (including all of its layers) from a binary file generated from a prior call to Write().
		void Read(FILE * file, Terrain * pTerrain);
		/// \brief Performs Texture::FlipHorizontal on the overall texture and all DetailTexture layers.
		void FlipHorizontal();
		/// \brief Performs Texture::FlipVertical on the overall texture and all DetailTexture layers.
		void FlipVertical();
		/// \brief Gets the standard filename that should be used to write this TextureCell to disk in a format matching that used by the Demeter Texture Editor.
		void GetDetailTextureImageFilename(int detailIndex, char *szFilename, int bufferSize);
		/// \brief Gets the standard filename that should be used to write this TextureCell to disk in a format matching that used by the Demeter Texture Editor.
		void GetDetailMaskFilename(int detailIndex, char *szFilename, int bufferSize);
		/// \brief Gets the width of the specified detail texture in pixels.
		int GetDetailTextureImageWidth(int detailIndex);
		/// \brief Gets the height of the specified detail texture in pixels.
		int GetDetailTextureImageHeight(int detailIndex);
		/// \brief Gets the color depth of the specified detail texture.
		int GetDetailTextureImageBitsPerPixel(int detailIndex);
		/// \brief Gets the width of the specified detail texture mask in pixels.
		int GetDetailMaskImageWidth(int detailIndex);
		/// \brief Gets the height of the specified detail texture mask in pixels.
		int GetDetailMaskImageHeight(int detailIndex);
		/// \brief Gets the color depth of the specified detail texture mask.
		int GetDetailMaskImageBitsPerPixel(int detailIndex);
		/// \brief Gets the standard filename that should be used to write this TextureCell to disk in a format matching that used by the Demeter Texture Editor.
		void GetBaseTextureImageFilename(char *szFilename, int bufferSize);
		/// \brief Gets the width of the overall texture in pixels.
		int GetBaseTextureImageWidth();
		/// \brief Gets the height of the overall texture in pixels.
		int GetBaseTextureImageHeight();
		/// \brief Gets the color depth of the overall texture.
		int GetBaseTextureImageBitsPerPixel();
	      private:
		  Texture * m_pTexture;
		  std::vector < DetailTexture * >m_DetailTextures;
		int m_Index;
	};
}

#endif
