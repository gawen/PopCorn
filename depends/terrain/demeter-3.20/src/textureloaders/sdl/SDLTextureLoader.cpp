#include "Demeter/Terrain.h"
#include <SDL_image.h>

using namespace Demeter;
using namespace std;

void LoadImage(const char *szShortFilename, int &width, int &height, Uint8 ** ppBuffer, bool bAlpha);
void ApplyTexture(const char *szFilename, bool isBaseTexture, Terrain * pTerrain);

#ifdef _WIN32
#define LOADER_API __declspec(dllexport)
#else
#define LOADER_API
#endif

#if defined (_MSC_VER) && (_MSC_VER >= 1300)
#define THROW_SPEC throw(...)
#else
#define THROW_SPEC
#endif

extern "C" LOADER_API void LoadTerrainTexture(int argc, const char **argv, Terrain * pTerrain) THROW_SPEC
{
	if (argc != 1)
		throw new DemeterException("SDL Loader: requires 1 parameter (texture filename)");
	const char *szFilename = argv[0];
	ApplyTexture(szFilename, true, pTerrain);
}

extern "C" LOADER_API void LoadCommonTerrainTexture(int argc, const char **argv, Terrain * pTerrain) THROW_SPEC
{
	if (argc != 1)
		throw new DemeterException("SDL Loader: requires 1 parameter (texture filename)");
	const char *szFilename = argv[0];
	ApplyTexture(szFilename, false, pTerrain);
}

extern "C" LOADER_API Texture * LoadTexture(int argc, const char **argv) THROW_SPEC
{
	if (argc != 4)
		throw new DemeterException("SDL Loader: requires 4 parameters (texture filename,clamp (true/false),compress (true/false),use alpha(true/false)");
	const char *szFilename = argv[0];
	bool bClamp = strcmp(argv[1], "true") == 0;
	bool bUseCompression = strcmp(argv[2], "true") == 0;
	bool bUseAlpha = strcmp(argv[3], "true") == 0;

	char szFullFilename[MAX_FILENAME_LENGTH];
	if (strstr(szFilename, "\\") || strstr(szFilename, "/"))
		sprintf(szFullFilename, szFilename);
	else
		Settings::GetInstance()->PrependMediaPath(szFilename, szFullFilename);

	// Load the texture data.
	int texWidth, texHeight;
	Uint8 *pTextureImage = NULL;
	if (szFullFilename != NULL)
	{
		LoadImage(szFullFilename, texWidth, texHeight, &pTextureImage, bUseAlpha);
		if (texWidth == 0)
		{
			string msg("SDL Loader: Failed to load texture image file '");
			msg += szFullFilename;
			msg += "'; This means that the file was not found or it is not an image type that SDL_image can read.";
			throw new DemeterException(msg);
		}
	}
	Texture *pTexture = new Texture(pTextureImage, texWidth, texHeight, texWidth, 0, bClamp, bUseCompression, bUseAlpha ? 4 : 3, bUseAlpha ? GL_RGBA : GL_RGB);
	delete[]pTextureImage;
	return pTexture;
}

void ApplyTexture(const char *szFilename, bool isBaseTexture, Terrain * pTerrain) THROW_SPEC
{
	char szFullFilename[MAX_FILENAME_LENGTH];
	if (strstr(szFilename, "\\") || strstr(szFilename, "/"))
		sprintf(szFullFilename, szFilename);
	else
		Settings::GetInstance()->PrependMediaPath(szFilename, szFullFilename);

	// Load the texture data.
	int texWidth, texHeight;
	Uint8 *pTextureImage = NULL;
	if (szFullFilename != NULL)
	{
		LoadImage(szFullFilename, texWidth, texHeight, &pTextureImage, false);
		if (texWidth == 0)
		{
			string msg("SDL Loader: Failed to load texture image file '");
			msg += szFullFilename;
			msg += "'; This means that the file was not found or it is not an image type that SDL_image can read.";
			throw new DemeterException(msg);
		}
	}
	if (isBaseTexture)
		pTerrain->SetTexture(pTextureImage, texWidth, texHeight);
	else
		pTerrain->SetCommonTexture(pTextureImage, texWidth, texHeight);
	delete[]pTextureImage;
}

void LoadImage(const char *szShortFilename, int &width, int &height, Uint8 ** ppBuffer, bool bAlpha)
{
	SDL_Init(SDL_INIT_VIDEO);
	char szFullFilename[MAX_FILENAME_LENGTH];
	if (strstr(szShortFilename, "\\") || strstr(szShortFilename, "/"))
		sprintf(szFullFilename, szShortFilename);
	else
		Settings::GetInstance()->PrependMediaPath(szShortFilename, szFullFilename);

	SDL_Surface *pImage = IMG_Load(szFullFilename);
	if (pImage != NULL)
	{
		width = pImage->w;
		height = pImage->h;
		Uint8 *pBufferTemp;
		if (bAlpha)
			pBufferTemp = new Uint8[width * height * 4];
		else
			pBufferTemp = new Uint8[width * height * 3];
		int i, j;
		Uint8 *pImagePixels = (Uint8 *) pImage->pixels;
		for (i = 0, j = 0; i < pImage->h * pImage->pitch; i += pImage->pitch)
		{
			Uint8 *pImageRow = pImagePixels + i;
			for (Uint8 * pImagePixel = pImageRow; pImagePixel < pImageRow + pImage->w * pImage->format->BytesPerPixel; pImagePixel += pImage->format->BytesPerPixel)
			{
				Uint8 red, green, blue, alpha;
				// Read the pixel into a 32-bit dword for use by SDL_GetRGBA
				Uint8 pPixel[4];
				for (int i = 0; i < pImage->format->BytesPerPixel; i++)
					pPixel[i] = pImagePixel[i];
				Uint32 *pCurrentPixel = (Uint32 *) pPixel;
				SDL_GetRGBA(*pCurrentPixel, pImage->format, &red, &green, &blue, &alpha);
				if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
				{
					Uint8 *p = (Uint8 *) pCurrentPixel;
					Uint32 currentPixelConverted = p[0] << 16 | p[1] << 8 | p[2];
					SDL_GetRGBA(currentPixelConverted, pImage->format, &red, &green, &blue, &alpha);
				}

				pBufferTemp[j++] = red;
				pBufferTemp[j++] = green;
				pBufferTemp[j++] = blue;
				if (bAlpha)
				{
					pBufferTemp[j++] = alpha;
				}
			}
		}
		*ppBuffer = pBufferTemp;
		SDL_FreeSurface(pImage);
	}
	else
	{
		width = 0;
		height = 0;
		*ppBuffer = NULL;
	}
}
