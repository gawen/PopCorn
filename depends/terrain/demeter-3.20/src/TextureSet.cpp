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

#include "Demeter/TextureSet.h"

using namespace Demeter;
using namespace std;

extern void SafelyFillString(char *szSource, char *szTarget, int targetSize);

TextureSet::TextureSet()
{
}

TextureSet::~TextureSet()
{
	while (!m_Textures.empty())
	{
		vector < Texture * >::iterator iter = m_Textures.begin();
		Texture *pTexture = *iter;
		m_Textures.erase(iter);
		delete pTexture;
	}
}

void TextureSet::GetSharedTextureFilename(int index, char *szFilename, int bufferSize)
{
	char szTmp[256];
	sprintf(szTmp, "sdt%d.raw", index);
	SafelyFillString(szTmp, szFilename, bufferSize);
}

int TextureSet::AddTexture(Texture * pTex)
{
	m_Textures.push_back(pTex);
	pTex->SetSharedIndex(m_Textures.size() - 1);
	return m_Textures.size() - 1;
}

Texture *TextureSet::GetTexture(int index)
{
	return m_Textures[index];
}

int TextureSet::GetNumTextures()
{
	return m_Textures.size();
}

void TextureSet::Write(FILE * file, Terrain * pTerrain)
{
	int size = m_Textures.size();
	fwrite(&size, sizeof(int), 1, file);
	for (unsigned int i = 0; i < m_Textures.size(); i++)
	{
		int index = m_Textures[i]->GetSharedIndex();
		m_Textures[i]->SetSharedIndex(-1);
		m_Textures[i]->Write(file, pTerrain);
		m_Textures[i]->SetSharedIndex(index);
	}
}

void TextureSet::Read(FILE * file, Terrain * pTerrain)
{
	int size;
	fread(&size, sizeof(int), 1, file);
	for (int i = 0; i < size; i++)
	{
		int index;
		fread(&index, sizeof(int), 1, file);
		Texture *pTex = new Texture;
		pTex->Read(file, pTerrain);
		AddTexture(pTex);
	}
}
