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

#include "Demeter/DetailTexture.h"

using namespace Demeter;

DetailTexture::DetailTexture(Texture * pTexture)
{
	m_pMask = NULL;
	m_pTexture = NULL;
	if (pTexture)
	{
		int textureMaskWidth = Settings::GetInstance()->GetTextureMaskWidth();
		int textureMaskHeight = Settings::GetInstance()->GetTextureMaskHeight();
		Uint8 *pBuffer = new Uint8[textureMaskWidth * textureMaskHeight];
		memset(pBuffer, 0, textureMaskWidth * textureMaskHeight);
		Texture *pMask = new Texture(pBuffer, textureMaskWidth, textureMaskHeight, textureMaskWidth, 0, true, false, 1, Texture::m_AlphaTextureFormat);
		delete[]pBuffer;
		//DetailTexture *pDetail = new DetailTexture;
		SetMask(pMask);
		SetTexture(pTexture);
	}
}

DetailTexture::~DetailTexture()
{
	if (m_pMask)
		delete m_pMask;
}

void DetailTexture::FlipHorizontal()
{
	m_pMask->FlipHorizontal();
	m_pTexture->FlipHorizontal();
}

void DetailTexture::FlipVertical()
{
	m_pMask->FlipVertical();
	m_pTexture->FlipVertical();
}

void DetailTexture::SetMask(Texture * pTexture)
{
	m_pMask = pTexture;
	m_pMask->SetBufferPersistent(true);
}

Texture *DetailTexture::GetMask()
{
	return m_pMask;
}

void DetailTexture::SetTexture(Texture * pTexture)
{
	m_pTexture = pTexture;
}

Texture *DetailTexture::GetTexture()
{
	return m_pTexture;
}

GLuint DetailTexture::BindMask()
{
	return m_pMask->UploadTexture();
}

GLuint DetailTexture::BindTexture()
{
	return m_pTexture->UploadTexture();
}

void DetailTexture::Unbind()
{
	m_pMask->UnloadTexture();
	m_pTexture->UnloadTexture();
}

void DetailTexture::Write(FILE * file, Terrain * pTerrain)
{
	m_pMask->Write(file, pTerrain);
	m_pTexture->Write(file, pTerrain);
}

void DetailTexture::Read(FILE * file, Terrain * pTerrain)
{
	int sharedIndex;
	fread(&sharedIndex, sizeof(int), 1, file);
	if (sharedIndex < 0)
	{
		m_pMask = new Texture;
		m_pMask->Read(file, pTerrain);
	}
	else
		m_pMask = pTerrain->GetTextureSet()->GetTexture(sharedIndex);
	fread(&sharedIndex, sizeof(int), 1, file);
	if (sharedIndex < 0)
	{
		m_pTexture = new Texture;
		m_pTexture->Read(file, pTerrain);
	}
	else
		m_pTexture = pTerrain->GetTextureSet()->GetTexture(sharedIndex);
}
