// Demeter Terrain Visualization Library by Clay Fowler
// Copyright (C) 2002 Clay Fowler

// $ID$

/*
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this library; if not, write to the
Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA  02111-1307, USA.
*/

#include "Demeter/Settings.h"
#include "Demeter/DemeterException.h"

using namespace Demeter;

Settings *pSettingsInstance = NULL;	// Singleton instance of the global settings

Settings::Settings()
{
	m_szMediaPath = NULL;
	m_bVerbose = false;
	m_bIsCompilerOnly = false;
	m_IsHeadless = false;
	m_UseDynamicTextures = false;
	m_IsEditor = false;
	m_TessellateMethod = Settings::TM_NEW;
	m_TessellateZWeight = 200000;
	m_DetailTextureRepeats = 20.0f;
	m_PickThreshold = -1.0f;
	m_bUseNormals = false;
	m_bDiagnostic = false;
	m_bPreloadTextures = false;
	m_bCompressTextures = false;
	m_bUseBorders = false;
	m_bUseVertexLocking = false;
	m_TextureMaskWidth = m_TextureMaskHeight = 256;
	m_bIsMaskReloadQueueActive = false;
}

Settings::~Settings()
{
	if (m_szMediaPath != NULL)
		delete[]m_szMediaPath;
}

Settings *Settings::GetInstance()
{
	if (pSettingsInstance == NULL)
		pSettingsInstance = new Settings();
	return pSettingsInstance;
}

void Settings::SetUseNormals(bool bUseNormals)
{
	m_bUseNormals = bUseNormals;
}

bool Settings::UseNormals()
{
	return m_bUseNormals;
}

void Settings::SetUseVertexLocking(bool bUseVertexLocking)
{
	m_bUseVertexLocking = bUseVertexLocking;
}

bool Settings::UseVertexLocking()
{
	return m_bUseVertexLocking;
}

void Settings::SetUseBorders(bool bUseBorders)
{
	m_bUseBorders = bUseBorders;
}

bool Settings::UseBorders()
{
	return m_bUseBorders;
}

void Settings::SetDiagnostic(bool bDiagnostic)
{
	m_bDiagnostic = bDiagnostic;
}

bool Settings::IsDiagnostic()
{
	return m_bDiagnostic;
}

bool Settings::SetProperty(const char *szProperty, const char *szValue)
{
	bool bSuccess = false;

	if (strcmp(szProperty, "verbose") == 0)
	{
		SetVerbose(strcmp(szValue, "true") == 0);
		bSuccess = true;
	}

	return bSuccess;
}

bool Settings::GetProperty(const char *szProperty, char *szValue)
{
	bool bSuccess = false;

	if (strcmp(szProperty, "verbose") == 0)
	{
		sprintf(szValue, m_bVerbose ? "true" : "false");
		bSuccess = true;
	}
	else if (strcmp(szProperty, "glinfo") == 0)
	{
		sprintf(szValue, "OpenGL Vendor: %s; OpenGL Extensions Supported: %s", glGetString(GL_VENDOR), glGetString(GL_EXTENSIONS));
		bSuccess = true;
	}

	return bSuccess;
}

void Settings::SetPreloadTextures(bool bPreload)
{
	m_bPreloadTextures = bPreload;
}

bool Settings::GetPreloadTextures()
{
	return m_bPreloadTextures;
}

void Settings::SetPickThreshold(float threshold)
{
	m_PickThreshold = threshold;
}

float Settings::GetPickThreshold()
{
	return m_PickThreshold;
}

void Settings::SetDetailTextureRepeats(float repeats)
{
	m_DetailTextureRepeats = repeats;
}

float Settings::GetDetailTextureRepeats()
{
	return m_DetailTextureRepeats;
}

void Settings::SetUseDynamicTextures(bool use)
{
	m_UseDynamicTextures = use;
}

bool Settings::UseDynamicTextures()
{
	return m_UseDynamicTextures;
}

void Settings::SetMediaPath(const char *szPath)
{
#ifdef _WIN32
	char separator = '\\';
#else
	char separator = '/';
#endif
	m_bCompressTextures = false;
	if (m_szMediaPath != NULL)
		delete[]m_szMediaPath;
	if (szPath[strlen(szPath) - 1] == separator)
	{
		m_szMediaPath = new char[strlen(szPath) + 1];
		sprintf(m_szMediaPath, szPath);
	}
	else
	{
		m_szMediaPath = new char[strlen(szPath) + 2];
		sprintf(m_szMediaPath, "%s%c", szPath, separator);
	}
}

void Settings::GetMediaPath(char **szPath)
{
	char *szOutput = new char[strlen(m_szMediaPath) + 1];
	sprintf(szOutput, m_szMediaPath);
	*szPath = szOutput;
}

bool Settings::IsMediaPathSet()
{
	if (m_szMediaPath == NULL)
		return false;
	else
		return true;
}

void Settings::PrependMediaPath(const char *szFilename, char *szFullFilename)
{
	sprintf(szFullFilename, "%s%s", m_szMediaPath, szFilename);
}

void Settings::PrependMediaPath(const char *szFilename, char *szFullFilename, int bufferLength)
{
	int length = strlen(m_szMediaPath) + strlen(szFilename) + 1;

	if (bufferLength < length)
		throw new DemeterException("String buffer is too small to contain requested data");

	sprintf(szFullFilename, "%s%s", m_szMediaPath, szFilename);
}

void Settings::SetHeadless(bool isHeadless)
{
	m_IsHeadless = isHeadless;
}

bool Settings::IsHeadless()
{
	return m_IsHeadless;
}

void Settings::SetVerbose(bool bVerbose)
{
	m_bVerbose = bVerbose;
}

bool Settings::IsVerbose()
{
	return m_bVerbose;
}

void Settings::SetEditor(bool isEditor)
{
	m_IsEditor = isEditor;
}

bool Settings::IsEditor()
{
	return m_IsEditor;
}

int Settings::GetScreenWidth()
{
	return m_ScreenWidth;
}

void Settings::SetScreenWidth(int width)
{
	m_ScreenWidth = width;
}

int Settings::GetScreenHeight()
{
	return m_ScreenHeight;
}

void Settings::SetScreenHeight(int height)
{
	m_ScreenHeight = height;
}

bool Settings::IsCompilerOnly()
{
	return m_bIsCompilerOnly;
}

void Settings::SetCompilerOnly(bool bIsCompilerOnly)
{
	m_bIsCompilerOnly = bIsCompilerOnly;
}

void Settings::SetTextureCompression(bool bCompress)
{
	m_bCompressTextures = bCompress;
}

bool Settings::IsTextureCompression()
{
	return m_bCompressTextures;
}

void Settings::SetTessellateMethod(int method)
{
	m_TessellateMethod = method;
}

int Settings::GetTessellateMethod()
{
	return m_TessellateMethod;
}

void Settings::SetTessellateZWeight(float method)
{
	m_TessellateZWeight = method;
}

float Settings::GetTessellateZWeight()
{
	return m_TessellateZWeight;
}

void Settings::SetTextureMaskWidth(int width)
{
	m_TextureMaskWidth = width;
}

int Settings::GetTextureMaskWidth()
{
	return m_TextureMaskWidth;
}

void Settings::SetTextureMaskHeight(int width)
{
	m_TextureMaskHeight = width;
}

int Settings::GetTextureMaskHeight()
{
	return m_TextureMaskHeight;
}

void Settings::SetMaskReloadQueueActive(bool isQueueActive)
{
	m_bIsMaskReloadQueueActive = isQueueActive;
}

bool Settings::IsMaskReloadQueueActive()
{
	return m_bIsMaskReloadQueueActive;
}
