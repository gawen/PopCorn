// Demeter Terrain Visualization Library by Clay Fowler
// Copyright (C) 2002 Clay Fowler

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

#ifdef __APPLE__
#include <mach-o/dyld.h>
#include <dlfcn.h>
#include <sys/stat.h>
#elif !defined(_WIN32)
#include <dlfcn.h>
#endif
#include "Demeter/Loader.h"

using namespace Demeter;
using namespace std;

Loader *pLoader = NULL;

typedef void (LoadElevationsFunc) (int argc, const char **argv, Terrain * pTerrain);
typedef Texture *(LoadTextureFunc) (int argc, const char **argv);
typedef void (LoadTerrainTextureFunc) (int argc, const char **argv, Terrain * pTerrain);

char **Tokenize(const char *szInput, int &numTokens);

Loader *Loader::GetInstance()
{
	if (pLoader == NULL)
		pLoader = new Loader();
	if (!Settings::GetInstance()->IsMediaPathSet())
	{
		string msg("TERRAIN LOADER: Warning! Media path has NOT been set via Settings::SetMediaPath() - it should be set prior to using the Loader");
		cerr << msg << endl;
		throw new DemeterException(msg);
	}
	return pLoader;
}

Loader::Loader()
{
}

Loader::~Loader()
{
}

void Loader::LoadElevations(const char *szPluginName, const char *szPluginParameters, Terrain * pTerrain)
{
#ifdef _WIN32
	HINSTANCE pModule = LoadLibrary(szPluginName);
	if (pModule == NULL)
	{
		cerr << "LOADER ERROR: Failed to load plugin " << szPluginName << endl;
	}
	FARPROC pLoadFunc = GetProcAddress(pModule, "LoadElevations");
	if (pLoadFunc == NULL)
	{
		cerr << "LOADER ERROR: Plugin " << szPluginName << " does not export symbol LoadElevations; is this a valid Demeter Loader?" << endl;
	}
	else
	{
		LoadElevationsFunc *pLoadElevationsFunc = (LoadElevationsFunc *) pLoadFunc;
		try
		{
			int numTokens;
			char **szParams = Tokenize(szPluginParameters, numTokens);
			const char **szParamsConst = (const char **)szParams;
			pLoadElevationsFunc(numTokens, szParamsConst, pTerrain);
		}
		catch(DemeterException * pException)
		{
			cerr << "LOADER: Demeter Exception thrown by plugin's LoadElevations():" << endl;
			cerr << pException->GetErrorMessage() << endl;
			throw pException;
		}
		catch(...)
		{
			cerr << "LOADER: Unexpected exception thrown by plugin's LoadElevations():" << endl;
		}
	}
#elif defined(__APPLE__)
	NSObjectFileImage image;
	NSModule os_handle = NULL;
	string fileName = findPlugin(szPluginName);
	if (NSCreateObjectFileImageFromFile(fileName.c_str(), &image) == NSObjectFileImageSuccess)
	{
		os_handle = NSLinkModule(image, szPluginName, TRUE);
		NSDestroyObjectFileImage(image);
	}

	if (os_handle == NULL)
	{
		cerr << "LOADER ERROR: Failed to load plugin " << szPluginName << endl;
	}

	// Get the proc address
	NSSymbol symbol;
	symbol = NSLookupSymbolInModule(os_handle, "_LoadElevations");
	LoadElevationsFunc *pLoadElevationsFunc = (LoadElevationsFunc *) NSAddressOfSymbol(symbol);

	if (pLoadElevationsFunc == NULL)
	{
		cerr << "LOADER ERROR: Plugin " << szPluginName << " does not export symbol LoadElevations; is this a valid Demeter Loader?" << endl;
	}
	else
	{
		try
		{
			int numTokens;
			char **szParams = Tokenize(szPluginParameters, numTokens);
			const char **szParamsConst = (const char **)szParams;
			pLoadElevationsFunc(numTokens, szParamsConst, pTerrain);
		}
		catch(DemeterException * pException)
		{
			cerr << "LOADER: Demeter Exception thrown by plugin's LoadElevations():" << endl;
			cerr << pException->GetErrorMessage() << endl;
			throw pException;
		}
		catch(...)
		{
			cerr << "LOADER: Unexpected exception thrown by plugin's LoadElevations():" << endl;
		}
	}
#else
	char *szLibName = new char[strlen(szPluginName) + 7];
	sprintf(szLibName, "lib%s.so", szPluginName);

	void *pModule = dlopen(szLibName, RTLD_NOW);
	if (pModule == NULL)
	{
		cerr << "LOADER ERROR: Failed to load plugin " << szLibName << endl;
		cerr << "\t(" << dlerror() << ")" << endl;
	}
	else
	{
		void *pLoadFunc = dlsym(pModule, "LoadElevations");
		if (pLoadFunc == NULL)
		{
			cerr << "LOADER ERROR: Plugin " << szLibName << " does not export symbol LoadElevations; is this a valid Demeter Loader?" << endl;
			cerr << "\t(" << dlerror() << ")" << endl;
		}
		else
		{
			LoadElevationsFunc *pLoadElevationsFunc = (LoadElevationsFunc *) pLoadFunc;
			try
			{
				int numTokens;
				char **szParams = Tokenize(szPluginParameters, numTokens);
				const char **szParamsConst = (const char **)szParams;
				pLoadElevationsFunc(numTokens, szParamsConst, pTerrain);
			}
			catch(DemeterException * pException)
			{
				cerr << "LOADER: Demeter Exception thrown by plugin's LoadElevations():" << endl;
				cerr << pException->GetErrorMessage() << endl;
				throw pException;
			}
			catch(...)
			{
				cerr << "LOADER: Unexpected exception thrown by plugin's LoadElevations():" << endl;
			}
		}
	}

	delete[]szLibName;
#endif
}

Texture *Loader::LoadTexture(const char *szPluginName, const char *szPluginParameters)
{
	Texture *pTexture = NULL;
#ifdef _WIN32
	HINSTANCE pModule = LoadLibrary(szPluginName);
	if (pModule == NULL)
	{
		cerr << "LOADER ERROR: Failed to load plugin " << szPluginName << endl;
	}
	FARPROC pLoadFunc = GetProcAddress(pModule, "LoadTexture");
	if (pLoadFunc == NULL)
	{
		cerr << "LOADER ERROR: Plugin " << szPluginName << " does not export symbol LoadTexture; is this a valid Demeter Loader?" << endl;
	}
	else
	{
		LoadTextureFunc *pLoadTextureFunc = (LoadTextureFunc *) pLoadFunc;
		try
		{
			int numTokens;
			char **szParams = Tokenize(szPluginParameters, numTokens);
			const char **szParamsConst = (const char **)szParams;
			pTexture = pLoadTextureFunc(numTokens, szParamsConst);
			for (int i = 0; i < numTokens; i++)
				delete[]szParams[i];
			delete[]szParams;
		}
		catch(DemeterException * pException)
		{
			cerr << "LOADER: Demeter Exception thrown by plugin's LoadTexture():" << endl;
			cerr << pException->GetErrorMessage() << endl;
			throw pException;
		}
		catch(...)
		{
			cerr << "LOADER: Unexpected exception thrown by plugin's LoadElevations():" << endl;
		}
	}
#elif defined(__APPLE__)
	NSObjectFileImage image;
	NSModule os_handle = NULL;
	string fileName = findPlugin(szPluginName);
	if (NSCreateObjectFileImageFromFile(fileName.c_str(), &image) == NSObjectFileImageSuccess) 
	{
		os_handle = NSLinkModule(image, szPluginName, TRUE);
		NSDestroyObjectFileImage(image);
	}

	if (os_handle == NULL)
	{
		cerr << "LOADER ERROR: Failed to load plugin " << szPluginName << endl;
	}

	// Get the proc address
	NSSymbol symbol;
	symbol = NSLookupSymbolInModule(os_handle, "_LoadTexture");
	LoadTextureFunc *pLoadTextureFunc = (LoadTextureFunc *) NSAddressOfSymbol(symbol);

	if (pLoadTextureFunc == NULL)
	{
		cerr << "LOADER ERROR: Plugin " << szPluginName << " does not export symbol LoadTexture; is this a valid Demeter Loader?" << endl;
	}
	else
	{
		try
		{
			int numTokens;
			char **szParams = Tokenize(szPluginParameters, numTokens);
			const char **szParamsConst = (const char **)szParams;
			pTexture = pLoadTextureFunc(numTokens, szParamsConst);
			for (int i = 0; i < numTokens; i++)
				delete[]szParams[i];
			delete[]szParams;
		}
		catch(DemeterException * pException)
		{
			cerr << "LOADER: Demeter Exception thrown by plugin's LoadTexture():" << endl;
			cerr << pException->GetErrorMessage() << endl;
			throw pException;
		}
		catch(...)
		{
			cerr << "LOADER: Unexpected exception thrown by plugin's LoadElevations():" << endl;
		}
	}
#else
	char *szLibName = new char[strlen(szPluginName) + 7];
	sprintf(szLibName, "lib%s.so", szPluginName);

	void *pModule = dlopen(szLibName, RTLD_NOW);
	if (pModule == NULL)
	{
		cerr << "LOADER ERROR: Failed to load plugin " << szLibName << endl;
		cerr << "\t(" << dlerror() << ")" << endl;
	}
	else
	{
		void *pLoadFunc;

		pLoadFunc = dlsym(pModule, "LoadTexture");

		if (pLoadFunc == NULL)
		{
			cerr << "LOADER ERROR: Plugin " << szLibName << " does not export symbols LoadTexture; is this a valid Demeter Loader?" << endl;
			cerr << "\t(" << dlerror() << ")" << endl;
		}
		else
		{
			LoadTextureFunc *pLoadTextureFunc = (LoadTextureFunc *) pLoadFunc;
			try
			{
				int numTokens;
				char **szParams = Tokenize(szPluginParameters, numTokens);
				const char **szParamsConst = (const char **)szParams;
				pTexture = pLoadTextureFunc(numTokens, szParamsConst);
				for (int i = 0; i < numTokens; i++)
					delete[]szParams[i];
				delete[]szParams;
			}
			catch(DemeterException * pException)
			{
				cerr << "LOADER: Demeter Exception thrown by plugin's LoadTexture():" << endl;
				cerr << pException->GetErrorMessage() << endl;
				throw pException;
			}
			catch(...)
			{
				cerr << "LOADER: Unexpected exception thrown by plugin's LoadElevations():" << endl;
			}
		}
	}

	delete[]szLibName;
#endif

	return pTexture;
}

void Loader::LoadTerrainTexture(const char *szPluginName, const char *szPluginParameters, Terrain * pTerrain)
{
	ApplyTexture(szPluginName, szPluginParameters, true, pTerrain);
}

void Loader::LoadCommonTerrainTexture(const char *szPluginName, const char *szPluginParameters, Terrain * pTerrain)
{
	ApplyTexture(szPluginName, szPluginParameters, false, pTerrain);
}

void Loader::ApplyTexture(const char *szPluginName, const char *szPluginParameters, bool isBaseTexture, Terrain * pTerrain)
{
#ifdef _WIN32
	HINSTANCE pModule = LoadLibrary(szPluginName);
	if (pModule == NULL)
	{
		cerr << "LOADER ERROR: Failed to load plugin " << szPluginName << endl;
	}
	FARPROC pLoadFunc = NULL;
	if (isBaseTexture)
		pLoadFunc = GetProcAddress(pModule, "LoadTerrainTexture");
	else
		pLoadFunc = GetProcAddress(pModule, "LoadCommonTerrainTexture");
	if (pLoadFunc == NULL)
	{
		cerr << "LOADER ERROR: Plugin " << szPluginName << " does not export symbols LoadTerrainTexture and LoadCommonTerrainTexture; is this a valid Demeter Loader?" << endl;
	}
	else
	{
		LoadTerrainTextureFunc *pLoadTextureFunc = (LoadTerrainTextureFunc *) pLoadFunc;
		try
		{
			int numTokens;
			char **szParams = Tokenize(szPluginParameters, numTokens);
			const char **szParamsConst = (const char **)szParams;
			pLoadTextureFunc(numTokens, szParamsConst, pTerrain);
			for (int i = 0; i < numTokens; i++)
				delete[]szParams[i];
			delete[]szParams;
		}
		catch(DemeterException * pException)
		{
			cerr << "LOADER: Demeter Exception thrown by plugin's LoadTerrainTexture():" << endl;
			cerr << pException->GetErrorMessage() << endl;
			throw pException;
		}
		catch(...)
		{
			cerr << "LOADER: Unexpected exception thrown by plugin's LoadElevations():" << endl;
		}
	}
#elif defined(__APPLE__)
	NSObjectFileImage image;
	NSModule os_handle = NULL;
	string fileName = findPlugin(szPluginName);
	if (NSCreateObjectFileImageFromFile(fileName.c_str(), &image) ==  NSObjectFileImageSuccess) 
	{
		os_handle = NSLinkModule(image, szPluginName, TRUE);
		NSDestroyObjectFileImage(image);
	}
	if (os_handle == NULL)
	{
		cerr << "LOADER ERROR: Failed to load plugin " << szPluginName << endl;
	}

	// Get the proc address
	NSSymbol symbol;
	if (isBaseTexture)
	{
		symbol = NSLookupSymbolInModule(os_handle, "_LoadTerrainTexture");
	}
	else
	{
		symbol = NSLookupSymbolInModule(os_handle, "_LoadCommonTerrainTexture");
	}
	LoadTerrainTextureFunc *pLoadTextureFunc = (LoadTerrainTextureFunc *) NSAddressOfSymbol(symbol);

	if (pLoadTextureFunc == NULL)
	{
		cerr << "LOADER ERROR: Plugin " << szPluginName << " does not export symbols LoadTerrainTexture and LoadCommonTerrainTexture; is this a valid Demeter Loader?" << endl;
	}
	else
	{
		try
		{
			int numTokens;
			char **szParams = Tokenize(szPluginParameters, numTokens);
			const char **szParamsConst = (const char **)szParams;
			pLoadTextureFunc(numTokens, szParamsConst, pTerrain);
			for (int i = 0; i < numTokens; i++)
				delete[]szParams[i];
			delete[]szParams;
		}
		catch(DemeterException * pException)
		{
			cerr << "LOADER: Demeter Exception thrown by plugin's LoadTerrainTexture():" << endl;
			cerr << pException->GetErrorMessage() << endl;
			throw pException;
		}
		catch(...)
		{
			cerr << "LOADER: Unexpected exception thrown by plugin's LoadElevations():" << endl;
		}
	}
#else
	char *szLibName = new char[strlen(szPluginName) + 7];
	sprintf(szLibName, "lib%s.so", szPluginName);

	void *pModule = dlopen(szLibName, RTLD_NOW);
	if (pModule == NULL)
	{
		cerr << "LOADER ERROR: Failed to load plugin " << szLibName << endl;
		cerr << "\t(" << dlerror() << ")" << endl;
	}
	else
	{
		void *pLoadFunc;

		if (isBaseTexture)
			pLoadFunc = dlsym(pModule, "LoadTerrainTexture");
		else
			pLoadFunc = dlsym(pModule, "LoadCommonTerrainTexture");

		if (pLoadFunc == NULL)
		{
			cerr << "LOADER ERROR: Plugin " << szLibName << " does not export symbols LoadTerrainTexture and LoadCommonTerrainTexture; is this a valid Demeter Loader?" << endl;
			cerr << "\t(" << dlerror() << ")" << endl;
		}
		else
		{
			LoadTerrainTextureFunc *pLoadTextureFunc = (LoadTerrainTextureFunc *) pLoadFunc;
			try
			{
				int numTokens;
				char **szParams = Tokenize(szPluginParameters, numTokens);
				const char **szParamsConst = (const char **)szParams;
				pLoadTextureFunc(numTokens, szParamsConst, pTerrain);
				for (int i = 0; i < numTokens; i++)
					delete[]szParams[i];
				delete[]szParams;
			}
			catch(DemeterException * pException)
			{
				cerr << "LOADER: Demeter Exception thrown by plugin's LoadTerrainTexture():" << endl;
				cerr << pException->GetErrorMessage() << endl;
				throw pException;
			}
			catch(...)
			{
				cerr << "LOADER: Unexpected exception thrown by plugin's LoadElevations():" << endl;
			}
		}
	}

	delete[]szLibName;
#endif
}

char **Tokenize(const char *szInput, int &numTokens)
{
	const int inputLength = strlen(szInput);

	const char delim = ',';

	numTokens = 1;
	int i;
	for (i = 0; i < inputLength; i++)
		if (szInput[i] == delim)
			numTokens++;

	char szToken[1024];
	char **szTokens = new char *[numTokens];

	int currentToken = 0;
	int length = 0;

	for (i = 0; i < inputLength - 1; i++)
	{
		if (szInput[i] == delim)
		{
			szToken[length] = '\0';
			szTokens[currentToken] = new char[length + 1];
			sprintf(szTokens[currentToken], szToken);
			currentToken++;
			length = 0;
		}
		else
		{
			szToken[length] = szInput[i];
			length++;
		}
	}
	// do the last char
	szToken[length] = szInput[inputLength - 1];
	length++;
	szToken[length] = '\0';
	szTokens[currentToken] = new char[length + 1];
	sprintf(szTokens[currentToken], szToken);
	return szTokens;
}

#ifdef __APPLE__
/* This function returns a string that contains the concatenated environment
 * variables DYLD_LIBRARY_PATH, LD_LIBRARY_PATH, and DYLD_FALLBACK_LIBRARY_PATH
 * (unless the latter is undefined, in which case a default fallback path will
 * be used).
 * The path building logic inspired by dlcompat's dlfcn.c
 */
static string buildPath(void)
{
	string path;
	const char* ldlp = getenv("LD_LIBRARY_PATH");
	const char* dyldlp = getenv("DYLD_LIBRARY_PATH");
	const char* dyldflp = getenv("DYLD_FALLBACK_LIBRARY_PATH");
	if (dyldlp)
	{
		path += dyldlp;
		path += ":";
	}
	if (ldlp)
	{
		path += ldlp;
		path += ":";
	}
	if (dyldflp)
	{
		path += dyldflp;
	}
	else
	{
		path += "/usr/local/lib:/lib:/usr/lib";
	}

	return path;
}

/* This function tries to find the absolute path to a file in the 
 * library search path. Hopefully all special cases are handled.
 */
static string findPlugin(const char *fname)
{
	static bool havePath = false;
	static vector<string> searchPath;
	static struct stat stat_arg; // just needed for calling stat
	static vector<string> prefix;
	static vector<string> suffix;
	vector<string> name;

	if (!havePath)
	{
		havePath = true;

		// first build the prefix/suffix lists
		prefix.push_back(""); // first one is unchanged
		suffix.push_back("");
		prefix.push_back("lib"); // lib$fname.so
		suffix.push_back(".so");
		prefix.push_back("lib"); // lib$fname.0.so
		suffix.push_back(".0.so");

		// now build the search path
		string all = buildPath();
		size_t left = 0;
		size_t right = 0;
		left = all.find_first_not_of(':', 0);
		right = all.find_first_of(':', left);
		// split the colon separated search path into individual dirs
		while (right != string::npos)
		{
			searchPath.push_back(all.substr(left, right - left));
			left = all.find_first_not_of(':',right + 1);
			right = all.find_first_of(':', left);
		} 
		// copy the last directory (usually not terminated with :)
		if (left != string::npos && all.size() > 0)
		{
				searchPath.push_back(all.substr(left, all.size() - left));
		}
	}

	if (!fname)
	{
		cerr << "Loader.cpp: findPlugin(): some moron called me with NULL arg."
			<< endl;
		return string("");
	}

	// if fname is absolute path, no search is done
	if (fname && fname[0] == '/')
	{
		return string(fname);
	}

	// first try to look in current directory
	for (vector<string>::iterator pi = prefix.begin(), si = suffix.begin();
		 pi != prefix.end(); pi++, si++)
	{
		string name = *pi;
		name += fname;
		name += *si;
		if (!stat(name.c_str(), &stat_arg))
		{
			return string(name);
		}
	}
	
	// now look at the search path
	for (vector<string>::iterator iter = searchPath.begin();
		 iter != searchPath.end(); iter++)
	{
		for (vector<string>::iterator pi = prefix.begin(), si = suffix.begin();
			 pi != prefix.end(); pi++, si++)
		{
			string filePath = *iter;
			filePath += "/";
			filePath += *pi;
			filePath += fname;
			filePath += *si;
//			cerr << "findPlugin(): looking at: " << filePath << endl;
			if (!stat(filePath.c_str(), &stat_arg))
			{
				return string(filePath);
			}
		}
	}

	// better than nothing...
	return string(fname);
}

#endif // APPLE

