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

#include "Demeter/Texture.h"

using namespace Demeter;
using namespace std;

int Texture::m_DefaultTextureFormat = GL_RGB8;
int Texture::m_CompressedTextureFormat = COMPRESSED_RGB_S3TC_DXT1_EXT;
int Texture::m_RgbaTextureFormat = GL_RGBA;
int Texture::m_AlphaTextureFormat = GL_ALPHA4;

extern vector < GLuint > AllocatedTextures;

GLuint CreateTexture(const Uint8 * pTexels, int width, int height, int rowLength, int border, int internalFormat, bool bClamp, int bytesPerPixel)
{
	GLuint texId;
	glGenTextures(1, &texId);
	if (!texId || !pTexels)
		return 0;
	glBindTexture(GL_TEXTURE_2D, texId);
	if (bClamp)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_EXT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_EXT);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	if (bytesPerPixel == 1)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	glPixelStorei(GL_UNPACK_ROW_LENGTH, rowLength);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	if (bytesPerPixel == 4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, border, GL_RGBA, GL_UNSIGNED_BYTE, pTexels);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pTexels);
	}
	else if (bytesPerPixel == 3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, border, GL_RGB, GL_UNSIGNED_BYTE, pTexels);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, pTexels);
	}
	else if (bytesPerPixel == 1)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, border, GL_ALPHA, GL_UNSIGNED_BYTE, pTexels);
		//gluBuild2DMipmaps(GL_TEXTURE_2D,GL_ALPHA,width,height,GL_ALPHA,GL_UNSIGNED_BYTE,pTexels);
	}
	else
	{
		throw new DemeterException("Unsupported texture format (bits per pixel must be 8,24, or 32)");
	}
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	return texId;
}

Texture::Texture(const Uint8 * pBuffer, int width, int height, int stride, int borderSize, bool bClamp, bool useCompression, bool bAlpha)
{
	Init(pBuffer, width, height, stride, borderSize, bClamp, useCompression, bAlpha);
}

Texture::Texture(const Uint8 * pBuffer, int width, int height, int stride, int borderSize, bool bClamp, bool useCompression, int bytesPerPixel, int textureFormat)
{
	m_BytesPerPixel = bytesPerPixel;
	m_pBuffer = new Uint8[height * width * bytesPerPixel];
	int k = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width * bytesPerPixel; j++, k++)
		{
			m_pBuffer[k] = pBuffer[i * stride * bytesPerPixel + j];
		}
	}
	m_Width = width;
	m_Height = height;
	m_UseCompression = useCompression;
	m_BorderSize = borderSize;
	m_TextureID = 0;
	m_bClamp = bClamp;
	m_RowLength = stride;
	m_SharedIndex = -1;
	m_szFilename = NULL;
	m_TextureFormat = textureFormat;
	m_BufferPersistent = Settings::GetInstance()->IsEditor();
}

Texture::Texture(FILE * file, int width, int height, int bitsPerPixel, int borderSize, bool bClamp, bool bUseCompression, int textureFormat, int sharedID)
{
	ReadRaw(file, width, height, bitsPerPixel, borderSize, bClamp, bUseCompression, textureFormat, sharedID);
	m_BufferPersistent = Settings::GetInstance()->IsEditor();
}

Texture::Texture(const char *szFilename, int width, int height, int bitsPerPixel, int borderSize, bool bClamp, bool bUseCompression, int textureFormat, int sharedID)
{
	char szFullFilename[1024];
	Settings::GetInstance()->PrependMediaPath(szFilename, szFullFilename, 1024);
	FILE *file = fopen(szFullFilename, "rb");
	if (file == NULL)
	{
		string msg;
		msg += "Failed to open texture file ";
		msg += szFullFilename;
		throw new DemeterException(msg);
	}
	ReadRaw(file, width, height, bitsPerPixel, borderSize, bClamp, bUseCompression, textureFormat, sharedID);
	m_BufferPersistent = Settings::GetInstance()->IsEditor();
	fclose(file);
}

Texture::Texture()
{
	m_pBuffer = NULL;
	m_Width = m_Height = 0;
	m_UseCompression = m_bClamp = false;
	m_RowLength = 0;
	m_TextureID = 0;
	m_BorderSize = 0;
	m_SharedIndex = -1;
	m_szFilename = NULL;
	m_BufferPersistent = Settings::GetInstance()->IsEditor();
}

Texture::~Texture()
{
	UnloadTexture();
	if (m_pBuffer != NULL)
	{
		delete[]m_pBuffer;
		m_pBuffer = NULL;
	}
	if (m_szFilename != NULL)
		delete[]m_szFilename;
}

void Texture::Init(const Uint8 * pBuffer, int width, int height, int rowLength, int borderSize, bool bClamp, bool useCompression, bool bAlpha)
{
	int bytesPerPixel = bAlpha ? 4 : 3;
	m_pBuffer = new Uint8[height * width * bytesPerPixel];
	int k = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width * bytesPerPixel; j++, k++)
		{
			m_pBuffer[k] = pBuffer[i * rowLength * bytesPerPixel + j];
		}
	}
	m_BytesPerPixel = bytesPerPixel;
	m_Width = width;
	m_Height = height;
	m_UseCompression = useCompression;
	m_BorderSize = borderSize;
	m_TextureID = 0;
	m_bClamp = bClamp;
	m_RowLength = rowLength;
	m_SharedIndex = -1;
	m_szFilename = NULL;
	if (bAlpha)
		m_TextureFormat = m_RgbaTextureFormat;
	else
		m_TextureFormat = m_DefaultTextureFormat;
	m_BufferPersistent = Settings::GetInstance()->IsEditor();
}

void Texture::SetBufferPersistent(bool persist)
{
	m_BufferPersistent = persist;
}

int Texture::GetFormat()
{
	return m_TextureFormat;
}

GLuint Texture::UploadTexture()
{
	if (m_TextureID == 0)
	{
		m_TextureID = CreateTexture(m_pBuffer, m_Width, m_Height, m_Width, m_BorderSize, m_TextureFormat, m_bClamp, m_BytesPerPixel);
		if (m_TextureID < 1)
			cout << "TERRAIN: Warning - failed to create texture" << endl;

		if (Settings::GetInstance()->IsDiagnostic())
		{
			bool found = false;
			for (unsigned int i = 0; i < AllocatedTextures.size() && !found; i++)
				found = (AllocatedTextures[i] == m_TextureID);
			if (!found)
				AllocatedTextures.push_back(m_TextureID);
		}

		if (!m_BufferPersistent && !Settings::GetInstance()->UseDynamicTextures())
		{
			delete[]m_pBuffer;
			m_pBuffer = NULL;
		}
	}
	return m_TextureID;
}

void Texture::UnloadTexture()
{
	if (m_TextureID != 0)
	{
		glDeleteTextures(1, &m_TextureID);
		if (Settings::GetInstance()->IsDiagnostic())
		{
			vector < GLuint >::iterator iter = AllocatedTextures.begin();
			while (*iter != m_TextureID && iter != AllocatedTextures.end())
				iter++;
			if (*iter == m_TextureID)
				AllocatedTextures.erase(iter);
		}
		m_TextureID = 0;
	}
}

bool Texture::IsBound()
{
	return (m_TextureID != 0);
}

void Texture::FlipHorizontal()
{
	Uint8 *pBufferNew = new Uint8[m_Width * m_Height * m_BytesPerPixel];
	for (int y = 0; y < m_Height; y++)
	{
		for (int x = 0; x < m_Width; x++)
		{
			int sourceIndex = y * m_Width * m_BytesPerPixel + x * m_BytesPerPixel;
			int destIndex = y * m_Width * m_BytesPerPixel + ((m_Width - x) - 1) * m_BytesPerPixel;
			for (int i = 0; i < m_BytesPerPixel; i++)
				pBufferNew[destIndex + i] = m_pBuffer[sourceIndex + i];
		}
	}
	delete[]m_pBuffer;
	m_pBuffer = pBufferNew;
}

void Texture::FlipVertical()
{
	Uint8 *pBufferNew = new Uint8[m_Width * m_Height * m_BytesPerPixel];
	for (int y = 0; y < m_Height; y++)
	{
		for (int x = 0; x < m_Width; x++)
		{
			int sourceIndex = y * m_Width * m_BytesPerPixel + x * m_BytesPerPixel;
			int destIndex = ((m_Height - y) - 1) * m_Width * m_BytesPerPixel + x * m_BytesPerPixel;
			for (int i = 0; i < m_BytesPerPixel; i++)
				pBufferNew[destIndex + i] = m_pBuffer[sourceIndex + i];
		}
	}
	delete[]m_pBuffer;
	m_pBuffer = pBufferNew;
}

int Texture::GetBitsPerPixel()
{
	return m_BytesPerPixel * 8;
}

void Texture::SetSharedIndex(int index)
{
	m_SharedIndex = index;
}

int Texture::GetSharedIndex()
{
	return m_SharedIndex;
}

Uint8 *Texture::GetBuffer()
{
	return m_pBuffer;
}

int Texture::GetWidth()
{
	return m_Width;
}

int Texture::GetHeight()
{
	return m_Height;
}

void Texture::SetFilename(const char *szFilename)
{
	if (m_szFilename)
		delete[]m_szFilename;
	m_szFilename = new char[strlen(szFilename) + 1];
	sprintf(m_szFilename, szFilename);
}

bool Texture::IsClamped()
{
	return m_bClamp;
}

bool Texture::UseCompression()
{
	return m_UseCompression;
}

int Texture::GetBorderSize()
{
	return m_BorderSize;
}

void Texture::WriteRaw(FILE * file)
{
	if ((int)fwrite(m_pBuffer, 1, m_Width * m_Height * m_BytesPerPixel, file) != m_Width * m_Height * m_BytesPerPixel)
		throw new DemeterException("Failed to write texture file: disk error");
}

void Texture::ReadRaw(FILE * file, int width, int height, int bitsPerPixel, int borderSize, bool bClamp, bool bUseCompression, int textureFormat, int sharedID)
{
	m_BytesPerPixel = bitsPerPixel / 8;
	m_Width = width;
	m_Height = height;
	if (m_BytesPerPixel != 1 && m_BytesPerPixel != 3 && m_BytesPerPixel != 4)
		throw new DemeterException("Attempt to read unsupported texture format: bpp must be 8,16,or 32");
	m_pBuffer = new Uint8[m_Width * m_Height * m_BytesPerPixel];
	if ((int)fread(m_pBuffer, 1, m_Width * m_Height * m_BytesPerPixel, file) != m_Width * m_Height * m_BytesPerPixel)
		throw new DemeterException("Failed to read texture file: disk error");
	m_UseCompression = bUseCompression;
	m_BorderSize = borderSize;
	m_TextureID = 0;
	m_bClamp = bClamp;
	m_RowLength = m_Width;
	m_SharedIndex = sharedID;
	m_szFilename = NULL;
	m_TextureFormat = textureFormat;
}

void Texture::Write(FILE * file, Terrain * pTerrain)
{
	fwrite(&m_SharedIndex, sizeof(int), 1, file);
	if (m_SharedIndex < 0)
	{
		int len;
		if (m_szFilename == NULL)
			len = 0;
		else
			len = strlen(m_szFilename);

		fwrite(&len, sizeof(int), 1, file);

		if (m_szFilename == NULL)
		{
			fwrite(&m_BytesPerPixel, sizeof(int), 1, file);
			fwrite(&m_Width, sizeof(int), 1, file);
			fwrite(&m_Height, sizeof(int), 1, file);
		}
		else
		{
			fwrite(m_szFilename, sizeof(char), len, file);
		}

		fwrite(&m_BorderSize, sizeof(int), 1, file);
		Uint8 useCompression = (Uint8) m_UseCompression;
		fwrite(&useCompression, sizeof(Uint8), 1, file);
		Uint8 bClamp = (Uint8) m_bClamp;
		fwrite(&bClamp, sizeof(Uint8), 1, file);
		if (m_szFilename == NULL)
			fwrite(m_pBuffer, m_Height * m_Width * m_BytesPerPixel, 1, file);
	}
}

void Texture::Read(FILE * file, Terrain * pTerrain)
{
	int len;
	fread(&len, sizeof(int), 1, file);
	if (len == 0)
	{
		fread(&m_BytesPerPixel, sizeof(int), 1, file);
		fread(&m_Width, sizeof(int), 1, file);
		fread(&m_Height, sizeof(int), 1, file);
		m_RowLength = m_Width;
	}
	else
	{
		m_szFilename = new char[len + 1];
		fread(m_szFilename, sizeof(char), len, file);
		m_szFilename[len] = '\0';
	}

	fread(&m_BorderSize, sizeof(int), 1, file);
	Uint8 useCompression;
	fread(&useCompression, sizeof(Uint8), 1, file);
	m_UseCompression = (bool) useCompression;
	Uint8 bClamp;
	fread(&bClamp, sizeof(Uint8), 1, file);
	m_bClamp = (bool) bClamp;
	if (m_szFilename == NULL)
	{
		m_pBuffer = new Uint8[m_Height * m_Width * m_BytesPerPixel];
		fread(m_pBuffer, m_Height * m_Width * m_BytesPerPixel, 1, file);
	}
	else
	{
		//bool bAlpha = (m_BytesPerPixel == 4);
		//LoadImage(m_szFilename,m_Width,m_Height,&m_pBuffer,bAlpha);
	}
}

const char *Texture::GetFilename()
{
	return m_szFilename;
}

void Texture::SetDefaultTextureFormat(int format)
{
	m_DefaultTextureFormat = format;
}

void Texture::SetAlphaTextureFormat(int format)
{
	m_AlphaTextureFormat = format;
}

void Texture::SetRgbaTextureFormat(int format)
{
	m_RgbaTextureFormat = format;
}

void Texture::SetCompressedTextureFormat(int format)
{
	m_CompressedTextureFormat = format;
}
