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

#ifndef _TERRAIN_LOADER_H_
#define _TERRAIN_LOADER_H_

#include "Demeter/Terrain.h"

/// \brief The Demeter Terrain Engine.
namespace Demeter
{
	/// \brief The Loader is a Singleton object that provides access to ElevationLoaders and TextureLoaders.

	/// Loader dynamically loads DLL's (or shared objects) when their services are requested. It forwards along
	/// the loading parameters passed by the application to the dynamically loaded ElevationLoader or TextureLoader
	/// library. Since ElevationLoader and TextureLoader libraries are loaded dynamically, it is your responsibility
	/// to ensure that your application provides the required .DLL or .so files along with your install.
	/// See the documentation at terrainengine.com for a discussion of this topic.
	class TERRAIN_API Loader
	{
	      public:
		/// \brief Returns the Singleton instance of the Loader class.
		static Loader *GetInstance();
		/// \brief Uses the specified ElevationLoader library to load elevation data for the specified Terrain object.

		/// \param szPluginName The name of the ElevationLoader to use. This should be the name
		/// of the ElevationLoader library WITHOUT any .DLL or .so extensions.
		/// \param szPluginParameters A comma-delimeted string of parameters to pass to the ElevationLoader itself.
		/// Every ElevationLoader defines its own unique set of parameters.
		/// \param pTerrain A pointer to the Terrain object that will be loaded with elevation data. This
		/// object should have already been allocated (for example, by calling new Terrain().)
		void LoadElevations(const char *szPluginName, const char *szPluginParameters, Terrain * pTerrain);
		/// \brief Uses the specified TextureLoader library to create a Texture object. 

		/// This object may then be used for detail texture painting, etc. by adding it to a Terrain object's TextureSet.
		/// \param szPluginName The name of the TextureLoader to use. This should be the name
		/// of the TextureLoader library WITHOUT any .DLL or .so extensions.
		/// \param szPluginParameters A comma-delimeted string of parameters to pass to the TextureLoader itself.
		/// Every TextureLoader defines its own unique set of parameters, but they generally include a filename and some bool switches which
		/// should be passed as the string literal "true" or "false."
		Texture *LoadTexture(const char *szPluginName, const char *szPluginParameters);
		/// \brief Uses the specified TextureLoader library to apply an overall texture to the specified Terrain object.

		/// The texture will automatically be chopped into TextureCell objects and a grid of textures will be arranged
		/// across the Terrain's surface.
		/// \param szPluginName The name of the TextureLoader to use. This should be the name
		/// of the TextureLoader library WITHOUT any .DLL or .so extensions.
		/// \param szPluginParameters A comma-delimeted string of parameters to pass to the TextureLoader itself.
		/// Every TextureLoader defines its own unique set of parameters, but they generally include a filename and some bool switches which
		/// should be passed as the string literal "true" or "false."
		/// \param pTerrain A pointer to the Terrain object that will be loaded with texture data. This
		/// object should have already been allocated (for example, by calling new Terrain()) and should have
		/// already had its elevation data set (either manually or via an ElevationLoader.)
		void LoadTerrainTexture(const char *szPluginName, const char *szPluginParameters, Terrain * pTerrain);
		/// \brief Uses the specified TextureLoader library to apply a repeating common texture to the specified Terrain object.

		/// See Terrain::SetCommonTexture for details on what a common texture is.
		/// \param szPluginName The name of the TextureLoader to use. This should be the name
		/// of the TextureLoader library WITHOUT any .DLL or .so extensions.
		/// \param szPluginParameters A comma-delimeted string of parameters to pass to the TextureLoader itself.
		/// Every TextureLoader defines its own unique set of parameters, but they generally include a filename and some bool switches which
		/// should be passed as the string literal "true" or "false."
		/// \param pTerrain A pointer to the Terrain object that will be loaded with texture data. This
		/// object should have already been allocated (for example, by calling new Terrain()) and should have
		/// already had its elevation data set (either manually or via an ElevationLoader.)
		void LoadCommonTerrainTexture(const char *szPluginName, const char *szPluginParameters, Terrain * pTerrain);
	      private:
		  Loader();
		 ~Loader();
		void ApplyTexture(const char *szPluginName, const char *szPluginParameters, bool isBaseTexture, Terrain * pTerrain);
	};
}

#endif
