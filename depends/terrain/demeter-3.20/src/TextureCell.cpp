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

#include "Demeter/TextureCell.h"

using namespace Demeter;
using namespace std;

extern void SafelyFillString(char *szSource, char *szTarget, int targetSize);

TextureCell::TextureCell()
{
	m_pTexture = NULL;
	m_Index = -1;
}

TextureCell::TextureCell(int index)
{
	m_pTexture = NULL;
	m_Index = index;
}

TextureCell::~TextureCell()
{
	if (m_pTexture)
		delete m_pTexture;
	while (!m_DetailTextures.empty())
	{
		vector < DetailTexture * >::iterator iter = m_DetailTextures.begin();
		DetailTexture *pDetailTexture = *iter;
		m_DetailTextures.erase(iter);
		delete pDetailTexture;
	}
}

Texture *TextureCell::GetTexture()
{
	return m_pTexture;
}

int TextureCell::GetDetailTextureImageWidth(int detailIndex)
{
	return m_DetailTextures[detailIndex]->GetTexture()->GetWidth();
}

int TextureCell::GetDetailTextureImageHeight(int detailIndex)
{
	return m_DetailTextures[detailIndex]->GetTexture()->GetHeight();
}

int TextureCell::GetDetailTextureImageBitsPerPixel(int detailIndex)
{
	return m_DetailTextures[detailIndex]->GetTexture()->GetBitsPerPixel();
}

int TextureCell::GetDetailMaskImageWidth(int detailIndex)
{
	return Settings::GetInstance()->GetTextureMaskWidth();
}

int TextureCell::GetDetailMaskImageHeight(int detailIndex)
{
	return Settings::GetInstance()->GetTextureMaskHeight();
}

int TextureCell::GetDetailMaskImageBitsPerPixel(int detailIndex)
{
	return TextureMaskBitsPerPixel;
}

void TextureCell::GetBaseTextureImageFilename(char *szFilename, int bufferSize)
{
	char szTmp[256];
	sprintf(szTmp, "bt%d.raw", m_Index);
	SafelyFillString(szTmp, szFilename, bufferSize);
}

int TextureCell::GetBaseTextureImageWidth()
{
	return m_pTexture->GetWidth();
}

int TextureCell::GetBaseTextureImageHeight()
{
	return m_pTexture->GetHeight();
}

int TextureCell::GetBaseTextureImageBitsPerPixel()
{
	return m_pTexture->GetBitsPerPixel();
}

void TextureCell::GetDetailTextureImageFilename(int detailIndex, char *szFilename, int bufferSize)
{
	char szTmp[256];
	sprintf(szTmp, "dt%d-%d.raw", m_Index, detailIndex);
	SafelyFillString(szTmp, szFilename, bufferSize);
}

void TextureCell::GetDetailMaskFilename(int detailIndex, char *szFilename, int bufferSize)
{
	char szTmp[256];
	sprintf(szTmp, "dm%d-%d.raw", m_Index, detailIndex);
	SafelyFillString(szTmp, szFilename, bufferSize);
}

void TextureCell::FlipHorizontal()
{
	m_pTexture->FlipHorizontal();
	for (unsigned int i = 0; i < m_DetailTextures.size(); i++)
		m_DetailTextures[i]->FlipHorizontal();
}

void TextureCell::FlipVertical()
{
	m_pTexture->FlipVertical();
	for (unsigned int i = 0; i < m_DetailTextures.size(); i++)
		m_DetailTextures[i]->FlipVertical();
}

void TextureCell::Write(FILE * file, Terrain * pTerrain)
{
	int numDetails = m_DetailTextures.size();
	fwrite(&numDetails, sizeof(int), 1, file);
	for (int i = 0; i < numDetails; i++)
		m_DetailTextures[i]->Write(file, pTerrain);
}

void TextureCell::Read(FILE * file, Terrain * pTerrain)
{
	int numDetails;
	fread(&numDetails, sizeof(int), 1, file);
	for (int i = 0; i < numDetails; i++)
	{
		DetailTexture *pDet = new DetailTexture;
		pDet->Read(file, pTerrain);
		m_DetailTextures.push_back(pDet);
	}
}

void TextureCell::SetTexture(Texture * pTexture)
{
	m_pTexture = pTexture;
}

DetailTexture *TextureCell::GetDetail(int index)
{
	return m_DetailTextures[index];
}

DetailTexture *TextureCell::GetDetail(Texture * pTexFind)
{
	DetailTexture *pDetailFound = NULL;
	for (unsigned int i = 0; i < m_DetailTextures.size() && pDetailFound == NULL; i++)
	{
		if (m_DetailTextures[i]->GetTexture() == pTexFind)
			pDetailFound = m_DetailTextures[i];
	}
	return pDetailFound;
}

void TextureCell::AddDetail(DetailTexture * pDetail)
{
	m_DetailTextures.push_back(pDetail);
}

int TextureCell::GetNumberOfDetails()
{
	return m_DetailTextures.size();
}

GLuint TextureCell::BindTexture()
{
	return m_pTexture->UploadTexture();
}

GLuint TextureCell::BindMask(int index)
{
	return m_DetailTextures[index]->BindMask();
}

GLuint TextureCell::BindDetail(int index)
{
	return m_DetailTextures[index]->BindTexture();
}

void TextureCell::UnbindAll()
{
	m_pTexture->UnloadTexture();
	for (unsigned int i = 0; i < m_DetailTextures.size(); i++)
		m_DetailTextures[i]->Unbind();
}
