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

#ifndef _SETTINGS_DEMETER_H_
#define _SETTINGS_DEMETER_H_

#include "Demeter/Globals.h"

/// \brief The Demeter Terrain Engine.
namespace Demeter
{

	/// \brief The Settings class is simply a manager of global variables. It provides a single place to set and retrieve all of the global settings that affect Demeter as a whole.
	class TERRAIN_API Settings
	{
	      public:
		~Settings();
		/// \brief Returns the global instance of this class. The Settings class acts a singleton, so there is only one instance of this class per application.
		static Settings *GetInstance();
		/// \brief Sets the filesystem path that Demeter will look in when reading textures, elevation files, etc.
		void SetMediaPath(const char *szPath);
		/// \brief Gets the filesystem path that Demeter will look in when reading textures, elevation files, etc.
		void GetMediaPath(char **szPath);
		/// \brief Prepends the current media path to the specified filename. It is the caller's responsibility to allocate szFullFilename with sufficient size to handle the entire filename.
		void PrependMediaPath(const char *szFilename, char *szFullFilename);
		/// \brief Prepends the current media path to the specified filename. The bufferLength parameter specifies the length of the destination buffer; an exception will be thrown if the buffer is not big enough to receive the prepended filename.
		void PrependMediaPath(const char *szFilename, char *szFullFilename, int bufferLength);
		/// \brief Indicates whether or not a call to SetMediaPath() has been made.
		bool IsMediaPathSet();
		/// \brief Specifies whether or not Demeter should output diagnostic information at runtime.
		void SetVerbose(bool bVerbose);
		/// \brief Indicates whether or not Demeter is outputting diagnostic information at runtime.
		bool IsVerbose();
		/// \brief Tells Demeter what the current width of the rendering surface is.
		void SetScreenWidth(int width);
		/// \brief Retrieves the current width of the rendering surface assumed by Demeter.
		int GetScreenWidth();
		/// \brief Tells Demeter what the current height of the rendering surface is.
		void SetScreenHeight(int height);
		/// \brief Retrieves the current height of the rendering surface assumed by Demeter.
		int GetScreenHeight();
		/// \brief Sets a named global property to the specified value.
		bool SetProperty(const char *szProperty, const char *szValue);
		/// \brief Retrieves a named global property.
		bool GetProperty(const char *szProperty, char *szValue);
		/// \brief Indicates whether or not the application that is using Demeter is a terrain compiler-like tool or a real application
		bool IsCompilerOnly();
		/// \brief Determins whether or not the application that is using Demeter is a terrain compiler-like tool or a real application
		void SetCompilerOnly(bool bIsCompilerOnly);
		/// \brief Specifies whether or not textures should be unloaded when they are not visible. This allows larger amounts of texture data, but at the expense of performance.
		void SetUseDynamicTextures(bool useDynamic);
		/// \brief Forces Texture objects to use persisten buffers by default. See Texture::SetBufferPersistent() for details.
		bool UseDynamicTextures();
		/// \brief Specifies whether or not textures should be compressed.
		void SetTextureCompression(bool bCompress);
		/// \brief Indicates whether or not texture compression is used by Texture objects by default.
		bool IsTextureCompression();
		/// \brief Puts Demeter in a non-graphics mode. 

		/// This prevents textures from being loaded, geometry from being built, etc. This allows terrains to be created on a server or other "headless" environment where only the elevation data itself is required.
		void SetHeadless(bool isHeadless);
		/// \brief Indicates whether or not headless mode is on, as set by calling SetHeadless().
		bool IsHeadless();
		/// \brief Puts Demeter in editor mode. 

		/// This makes memory usage much higher but makes dynamically editing the terrain easier 
		/// for applications such as terrain editors.
		void SetEditor(bool isEditor);
		/// \brief Indicates whether or not SetEditor(true) has been called.
		bool IsEditor();
		/// \brief Set the tessellation heuristic method to use when tessellating.

		/// This setting determines which algorithm Demeter uses to simplify terrain blocks. Some
		/// settings are faster but less general and vice versa. The default setting is TM_NEW.
		void SetTessellateMethod(int method);
		/// \brief Gets the current tessellation heuristic method.
		int GetTessellateMethod();
		/// \brief ZWeight to make screen Z coordinate more significant in level of detail decisions.
		void SetTessellateZWeight(float method);
		/// \brief Gets the ZWeight.
		float GetTessellateZWeight();
		/// \brief Specifies how many times detail textures are repeated within their parent textures. Higher values will make the detail textures smaller.
		void SetDetailTextureRepeats(float repeats);
		/// \brief Gets the repeats setting as set in a call to SetDetailTextureRepeats().
		float GetDetailTextureRepeats();
		/// \brief Specifies the maximum distance between the surface of the terrain and a picked point that can be considered picking the terrain (as opposed to an application-specific object.)
		void SetPickThreshold(float threshold);
		/// Gets the pick threshold as set in a call to SetPickThreshold().
		float GetPickThreshold();
		/// \brief Specifies whether or not all of a terrain's textures should be preloaded when the terrain is loaded. 

		/// This makes terrains load more slowly but they run more smoothly since textures aren't loaded as the user's camera moves.
		void SetPreloadTextures(bool bPreload);
		/// \brief Gets the preload setting as set in a call to SetPreLoadTextures().
		bool GetPreloadTextures();
		/// \brief Specifies whether or not vertex normals should be calculated and used.

		/// Turning this option on will make Demeter use more memory and will slow rendering slightly on most hardware.
		/// You will definitely need to turn this setting on if you want to use lighting on your Terrain objects instead
		/// of "baking" lighting into the terrain textures.
		void SetUseNormals(bool bUseNormals);
		/// \brief Indicates whether or not normals are on as set by calling SetUseNormals().
		bool UseNormals();
		/// \brief If set to true, instructs Demeter to use texture border extensions to eliminate texture seams when bilinear filtering is enabled
		void SetUseBorders(bool bUseBorders);
		/// \brief Indicates whether or not texture borders are on as set by calling SetUseBorders().
		bool UseBorders();
		/// \brief If set to true, instructs Demeter to use compiled vertex array extensions - this can cause significant performance problems on older, flakey 3D hardware
		void SetUseVertexLocking(bool bUseVertexLocking);
		/// \brief Indicates whether or not compiled vertex array support is on as set by calling SetUseVertexLocking().
		bool UseVertexLocking();
		/// \brief Puts Demeter in a diagnostic mode that checks for such things as memory leaks and unbound textures.                  
		void SetTextureMaskWidth(int width);
		int GetTextureMaskWidth();
		void SetTextureMaskHeight(int height);
		int GetTextureMaskHeight();
		void SetMaskReloadQueueActive(bool isQueueActive);
		bool IsMaskReloadQueueActive();
		/// This option makes Demeter run slowly, so only use it for debuggin purposes.
		void SetDiagnostic(bool bDiagnostic);
		/// \brief Indicates whether or not diagnostic mode as been activated as set by calling SetDiagnostic().
		bool IsDiagnostic();
		/// \brief Enumerates the simplification heuristic methods for tessellation.
		enum TESSELLATEMETHOD
		{
			/// Good general algorithm (handles camera roll and pitch) - uses skewed bounding boxes that closely fit Terrain blocks; measures the 3D length of the Z axis for error metric
			TM_NEW = 0,
			/// Fast but doesn't always approximate well - goes purely by 2D screen-size of terrainblock 
			TM_SCREEN_RECT = 1,
			/// Fast heuristic but overall worse performance; doesn't handle camera rolling at all
			TM_OLD_UPRIGHTONLY = 2,
			/// Not as general as TM_NEW; handles camera rolling by also looking at 2D screen size of Z axis of bounding box
			TM_2D_ROLLONLY = 3,
			/// Goes by "screen-space" 3D length of Z axis of bounding box
			TM_OLD_NEW = 4
		};
	      private:
		  Settings();
		static Settings *m_pInstance;
		char *m_szMediaPath;
		bool m_bVerbose;
		bool m_bIsCompilerOnly;
		bool m_bCompressTextures;
		int m_ScreenWidth, m_ScreenHeight;
		bool m_IsHeadless;
		bool m_UseDynamicTextures;
		bool m_IsEditor;
		int m_TessellateMethod;
		float m_TessellateZWeight;
		float m_DetailTextureRepeats;
		float m_PickThreshold;
		bool m_bPreloadTextures;
		bool m_bUseNormals;
		bool m_bDiagnostic;
		bool m_bUseBorders;
		bool m_bUseVertexLocking;
		int m_TextureMaskWidth, m_TextureMaskHeight;
		bool m_bIsMaskReloadQueueActive;
	};
}

#endif
