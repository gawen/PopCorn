#include "Demeter/Brush.h"
#include <math.h>

using namespace Demeter;

Brush::Brush(int width)
{
	m_Width = width;
	m_Intensity = 1.0f;
	m_MaxIntensity = 1.0f;
	m_bErase = false;
	m_pBuffer = 0;
	BuildBuffer();
}

Brush::~Brush()
{
	delete[]m_pBuffer;
}

void Brush::Paint(Terrain * pTerrain, int detailTextureIndex, float x, float y)
{
	int textureCellX, textureCellY;
	float texU, texV;
	pTerrain->GetTextureCoordinates(x, y, textureCellX, textureCellY, texU, texV);
	if (0 <= textureCellX)
	{
		TextureCell *pTargetCell = pTerrain->GetTextureCell(textureCellX, textureCellY);
		int imageWidth = pTargetCell->GetDetailMaskImageWidth(0);
		int imageHeight = pTargetCell->GetDetailMaskImageHeight(0);
		Paint(pTerrain, detailTextureIndex, textureCellX, textureCellY, (int)(texU * imageWidth), (int)(texV * imageHeight));
	}
}

void Brush::Paint(Terrain * pTerrain, int detailTextureIndex, int cellX, int cellY, int x, int y)
{
	int targetX, targetY;
	int endX, endY;
	int halfWidth;
	if (m_Width > 1)
		halfWidth = m_Width / 2;
	else
		halfWidth = 0;

	targetX = x - halfWidth;
	targetY = y - halfWidth;
	endX = targetX + m_Width;
	endY = targetY + m_Width;
	int myX, myY;
	int maxValue = (int)(m_MaxIntensity * 255.0f);
	TextureCell *pTargetCell = pTerrain->GetTextureCell(cellX, cellY);
	int targetWidth = pTargetCell->GetDetailMaskImageWidth(0);
	int targetHeight = pTargetCell->GetDetailMaskImageHeight(0);
	if (2 * targetWidth - 1 <= m_Width)
		m_Width = 2 * targetWidth - 1;
	if (2 * targetHeight - 1 <= m_Width)
		m_Width = 2 * targetHeight - 1;

	bool refreshLeft = false;
	bool refreshTop = false;
	bool refreshRight = false;
	bool refreshBottom = false;

	for (myX = 0, targetX = x - halfWidth; targetX < endX; targetX++, myX++)
	{
		for (myY = 0, targetY = y - halfWidth; targetY < endY; targetY++, myY++)
		{
			int adjustedTargetX = targetX;
			int adjustedTargetY = targetY;
			int targetCellX = cellX;
			int targetCellY = cellY;
			if (targetX < 0)
			{
				if (cellX % 2 != 0)
				{
					targetCellX++;
					refreshRight = true;
				}
				else
				{
					targetCellX--;
					refreshLeft = true;
				}
				adjustedTargetX = abs(targetX) - 1;
			}
			if (targetY < 0)
			{
				if (cellY % 2 != 0)
				{
					targetCellY++;
					refreshTop = true;
				}
				else
				{
					targetCellY--;
					refreshBottom = true;
				}
				adjustedTargetY = abs(targetY) - 1;
			}
			if (targetWidth <= targetX)
			{
				if (cellX % 2 != 0)
				{
					targetCellX--;
					refreshLeft = true;
				}
				else
				{
					targetCellX++;
					refreshRight = true;
				}
				adjustedTargetX = targetWidth - (targetX - targetWidth) - 1;
			}
			if (targetHeight <= targetY)
			{
				if (cellY % 2 != 0)
				{
					targetCellY--;
					refreshBottom = true;
				}
				else
				{
					targetCellY++;
					refreshTop = true;
				}
				adjustedTargetY = targetHeight - (targetY - targetHeight) - 1;
			}
			if (0 <= targetCellX && 0 <= targetCellY && targetCellX < pTerrain->GetNumberOfTextureTilesWidth() && targetCellY < pTerrain->GetNumberOfTextureTilesHeight())
			{
				unsigned char *pTargetBuffer = pTerrain->GetMaskBits(targetCellX, targetCellY, detailTextureIndex, targetWidth, targetHeight);
				int targetIndex = adjustedTargetY * targetWidth + adjustedTargetX;
				int myIndex = myY * m_Width + myX;
				int offset = (int)((float)m_pBuffer[myIndex] * (float)m_Intensity);
				if (m_bErase)
					offset *= -1;
				int origValue = pTargetBuffer[targetIndex];
				int newValue = origValue;
				if (m_bErase)
				{
					if (maxValue < origValue)
					{
						newValue = origValue + offset;
						if (newValue < maxValue)
							newValue = maxValue;
					}
				}
				else
				{
					if (origValue < maxValue)
					{
						newValue = origValue + offset;
						if (maxValue < newValue)
							newValue = maxValue;

					}
				}
				if (255 < newValue)
					newValue = 255;
				if (newValue < 0)
					newValue = 0;
				pTargetBuffer[targetIndex] = (unsigned char)newValue;
			}
		}
	}
	pTerrain->ReloadMask(cellX, cellY, detailTextureIndex);
	if (refreshLeft && refreshBottom)
		pTerrain->ReloadMask(cellX - 1, cellY - 1, detailTextureIndex);
	if (refreshBottom)
		pTerrain->ReloadMask(cellX, cellY - 1, detailTextureIndex);
	if (refreshRight && refreshBottom)
		pTerrain->ReloadMask(cellX + 1, cellY - 1, detailTextureIndex);
	if (refreshRight)
		pTerrain->ReloadMask(cellX + 1, cellY, detailTextureIndex);
	if (refreshRight && refreshTop)
		pTerrain->ReloadMask(cellX + 1, cellY + 1, detailTextureIndex);
	if (refreshTop)
		pTerrain->ReloadMask(cellX, cellY + 1, detailTextureIndex);
	if (refreshLeft && refreshTop)
		pTerrain->ReloadMask(cellX - 1, cellY + 1, detailTextureIndex);
	if (refreshLeft)
		pTerrain->ReloadMask(cellX - 1, cellY, detailTextureIndex);
}

void Brush::SetWidth(int width)
{
	if (m_Width != width)
	{
		m_Width = width;
		BuildBuffer();
	}
}

void Brush::SetErase(bool bErase)
{
	m_bErase = bErase;
}

void Brush::SetIntensity(float intensity)
{
	m_Intensity = intensity;
}

void Brush::SetMaxIntensity(float maxIntensity)
{
	m_MaxIntensity = maxIntensity;
}

void Brush::BuildBuffer()
{
	if (m_pBuffer)
		delete[]m_pBuffer;
	float centerX = (float)m_Width / 2.0f;
	float centerY = (float)m_Width / 2.0f;
	m_pBuffer = new unsigned char[m_Width * m_Width];
	for (int x = 0; x < m_Width; x++)
	{
		for (int y = 0; y < m_Width; y++)
		{
			float deltaX = centerX - (float)x;
			float deltaY = centerY - (float)y;
			float dist = (float)sqrt(deltaX * deltaX + deltaY * deltaY);
			float alpha = 1.0f - (dist / ((float)m_Width / 2.0f));
			if (1.0f < alpha)
				alpha = 1.0f;
			if (alpha < 0.0f)
				alpha = 0.0f;
			m_pBuffer[y * m_Width + x] = (unsigned char)(alpha * 255.0f);
		}
	}
}
