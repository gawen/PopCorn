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

#ifndef _TERRAIN_LATTICE_DEMETER_H_
#define _TERRAIN_LATTICE_DEMETER_H_

#include "Demeter/Terrain.h"
#include "Demeter/TerrainLoadListener.h"
#include "Demeter/TextureSet.h"
#include "Demeter/TextureFactory.h"

#include <vector>

/// \brief The Demeter Terrain Engine.
namespace Demeter
{
	/// \brief You derive a concrete implementation of this interface in order to serve Terrains up to a TerrainLattice.

	/// TerrainLattice is a grid of Terrain objects that are stitched together and kept seamless, even under dynamic tessellation.
	/// TerrainLattice needs to be fed each of the Terrain objects at each x,y position in the lattice as the user's camera moves around
	/// the scene and Terrains become visible. When the camera moves in such a way that a Terrain in the lattice becomes visible, your object's
	/// LoadTerrainAt() method will be called. You must load a Terrain by whatever means you like (e.g. using ElevationLoader and TextureLoader objects,
	/// or raw arrays of data, etc.) and return that Terrain so that TerrainLattice can make it visible to the user. You may also want to load other
	/// application-specific objects at this time (such as the buildings, etc. that populate the loaded Terrain - anything that is specific to THAT Terrain.)
	/// When a TerrainObject is no longer visible, your object's UnloadTerrain() method will be called, giving you a chance to dispose of the Terrain 
	/// and any application-specific objects.
	class TERRAIN_API TerrainLatticeLoader
	{
	      public:
		/// \brief Called by the TerrainLattice when a Terrain object in the lattice has entered the visible region and, therefore, needs to be loaded into RAM.
		virtual Terrain * LoadTerrainAt(int latticeX, int latticeY) = 0;
		/// \brief Called by the TerrainLattice when a Terrain object in the lattice is no longer within the visible region and can, therefore, be disposed of, freeing RAM for other visible Terrain objects.
		virtual void UnloadTerrain(int latticeX, int latticeY, Terrain * pTerrain) = 0;
		/// \brief Returns the number of Terrain objects in the lattice along the world X-axis. This multiplied by the return value of GetNumberOfTerrainsY() yields the total number of Terrain objects in the lattice.
		virtual int GetNumberOfTerrainsX() = 0;
		/// \brief Returns the number of Terrain objects in the lattice along the world Y-axis. This multiplied by the return value of GetNumberOfTerrainsX() yields the total number of Terrain objects in the lattice.
		virtual int GetNumberOfTerrainsY() = 0;
		/// \brief Returns the width in world units of each of the individual Terrain objects in the lattice (they must all be the same width.)
		virtual float GetTerrainWidth() = 0;
		/// \brief Returns the height in world units of each of the individual Terrain objects in the lattice (they must all be the same height.)
		virtual float GetTerrainHeight() = 0;
	};

	/// \brief Manages a network of Terrain objects arranged in a grid pattern to model extremely large worlds that couldn't be handled by a 
	/// single Terrain object.

	/// TerrainLattice manages the seams between Terrain objects so that, even during dynamic tessellation, there are no visible cracks between
	/// neighboring Terrain objects in the lattice.
	/// \warning This class is temporarily disabled for the current release, but will be available again soon.
	class TERRAIN_API TerrainLattice
	{
	      public:
		TerrainLattice(TerrainLatticeLoader * pLoader);
		~TerrainLattice();
		void Load(char *szBaseName, int maxNumTriangles, int maxBlockSize, float commonRepeats);
		Terrain *GetTerrain(int positionX, int positionY);
		Terrain *GetTerrainAtPoint(float x, float y);
		void SetCameraPosition(float x, float y, float z);
		void SetDetailThreshold(float threshold);
		void ModelViewMatrixChanged();
		void Render();
		float GetElevation(float x, float y);
		float GetWidth();
		float GetHeight();
	      private:
		void Tessellate();
		void AddTerrain(Terrain * pTerrain, int positionX, int positionY);
		  Demeter::DIRECTION GetOppositeDirection(Demeter::DIRECTION direction);
		Terrain *GetTerrainRelative(Terrain * pTerrain, int positionX, int positionY);
		Terrain *GetTerrainRelative(Terrain * pTerrain, Demeter::DIRECTION direction);
		void LoadTerrain(int index);
		TerrainLatticeLoader *m_pLoader;
		int m_WidthTerrains, m_HeightTerrains;
		int m_WidthActiveTerrains, m_HeightActiveTerrains;
		float m_TerrainWidth, m_TerrainHeight;
		Terrain **m_pTerrains;
		int m_CurrentTerrainIndex[9];
	};
}

#endif
