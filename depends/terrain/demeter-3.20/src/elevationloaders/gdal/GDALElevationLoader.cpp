#include "Demeter/Terrain.h"
#include <gdal_priv.h>

using namespace Demeter;
using namespace std;

// If this flag is #defined, then all elevation data loaded by GDAL will be mirrored about the X-axis
// when loaded into Demeter. Turn this #define off if your elevation data seems to be "mirrored" vertically.
#define _SWAP_VERTICAL

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

bool FirstTime = true;

void DimensionPowerOf2(int origX, int origY, int &newX, int &newY);

extern "C" LOADER_API void LoadElevations(int argc, const char **argv, Terrain * pTerrain) THROW_SPEC
{
	if (argc < 3)
		throw new DemeterException("GDAL Loader: requires 3 params (filename,vertex spacing,elevation scale) and allows an optional 4th bool param ('true' means mirror elevation data about x-axis; use this if your elevation data seems to be mirrored about the x-axis)");

	const char *szElevationsFilename = argv[0];
	float vertexSpacing = (float)atof(argv[1]);
	float elevationScale = (float)atof(argv[2]);
	bool swapVertical = (argc == 4 && strcmp(argv[3], "true") == 0);

	char szFullFilename[MAX_FILENAME_LENGTH];
	if (strstr(szElevationsFilename, "\\") || strstr(szElevationsFilename, "/"))
		sprintf(szFullFilename, szElevationsFilename);
	else
		Settings::GetInstance()->PrependMediaPath(szElevationsFilename, szFullFilename);

	float *pImageData = NULL;
	int elevWidth;
	int elevHeight;
	//int i;

	if (FirstTime)
	{
		GDALAllRegister();
		FirstTime = false;
	}

	GDALDataset *poDataset = (GDALDataset *) GDALOpen(szFullFilename, GA_ReadOnly);
	if (poDataset == NULL)
		cout << "GDAL Loader: Failed to load elevation data" << endl;

	DimensionPowerOf2(poDataset->GetRasterXSize(), poDataset->GetRasterYSize(), elevWidth, elevHeight);
	cout << "GDAL Loader: Changing size to " << elevWidth << "," << elevHeight << endl;

	GDALRasterBand *poBand = poDataset->GetRasterBand(1);
	int nBlockXSize, nBlockYSize;
	poBand->GetBlockSize(&nBlockXSize, &nBlockYSize);

	pImageData = new float[elevWidth * elevHeight];

	poBand->RasterIO(GF_Read, 0, 0, poDataset->GetRasterXSize(), poDataset->GetRasterYSize(), (void *)pImageData, elevWidth, elevHeight, GDT_Float32, 0, 0);

	// Eliminate outrageous elevations (formats such as ArcGrid often contain these values instead of bathymetry)
	for (int i = 0; i < elevHeight * elevWidth; i++)
	{
		if (pImageData[i] > 10000.0f || pImageData[i] < -10000.0f)
			pImageData[i] = 0.0f;
	}

	if (swapVertical)
	{
		float *pTempData = new float[elevWidth * elevHeight];
		int i;
		for (i = 0; i < elevWidth * elevHeight; i++)
		{
			pTempData[i] = pImageData[i];
		}
		for (i = 0; i < elevHeight; i++)
		{
			for (int j = 0; j < elevWidth; j++)
			{
				pImageData[i * elevWidth + j] = pTempData[(elevHeight - i - 1) * elevWidth + j];
			}
		}
		delete[]pTempData;
	}

	delete poDataset;

	pTerrain->SetAllElevations(pImageData, elevWidth, elevHeight, vertexSpacing, elevationScale);

	delete[]pImageData;
}

void DimensionPowerOf2(int origX, int origY, int &newX, int &newY)
{
	newX = newY = 0;
	for (int numPows = 20; newX == 0 || newY == 0; numPows *= 2)
	{
		int *powers = new int[numPows];
		int i;
		for (i = 0; i < numPows; i++)
			powers[i] = (int)pow(2.0, i);
		newX = 0;
		for (i = 0; i < numPows && newX == 0; i++)
		{
			if (origX <= powers[i])
				newX = powers[i];
		}
		newY = 0;
		for (i = 0; i < numPows && newY == 0; i++)
		{
			if (origY <= powers[i])
				newY = powers[i];
		}
		delete[]powers;
	}
}
