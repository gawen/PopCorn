/* This is a collection of utilities for managing the sample data for Demeter's sample programs.
   As a developer/user of the Demeter library, you needn't do anything like this. Simply lookup
   Settings::SetMediaPath() in the API reference and forget about this file. */

#include <Demeter/config.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

bool TestPath(char *szPath);
void ShowDataError();

// Find the sample data path by looking for an environment variable or by falling back
// to the most likely places. If it can't be found at all, then complain so the user
// can see what's wrong.
bool GetSampleDataPath(char *szPath)
{
	char separator;
#ifdef _WIN32
	separator = '\\';
#else
	separator = '/';
#endif
	char *szEnvironmentSpecifiedPath = getenv("DEMETER_SAMPLE_DATA_PATH");
	if (szEnvironmentSpecifiedPath != NULL)
	{
		char szEnvironmentSpecifiedPathTrailing[1024];
		sprintf(szEnvironmentSpecifiedPathTrailing, szEnvironmentSpecifiedPath);
		int lastChar = strlen(szEnvironmentSpecifiedPath);
		if (szEnvironmentSpecifiedPathTrailing[lastChar] != separator)
		{
			szEnvironmentSpecifiedPathTrailing[lastChar] = separator;
			szEnvironmentSpecifiedPathTrailing[lastChar + 1] = '\0';
		}
		if (TestPath(szEnvironmentSpecifiedPathTrailing))
		{
			sprintf(szPath, szEnvironmentSpecifiedPathTrailing);
			return true;
		}
	}
	char szFullPath[1024];
	sprintf(szFullPath, "/usr/share/DemeterSampleData-3.20/");
	if (TestPath(szFullPath))
	{
		sprintf(szPath, szFullPath);
		return true;
	}
	sprintf(szFullPath, "/usr/local/share/DemeterSampleData-3.20/");
	if (TestPath(szFullPath))
	{
		sprintf(szPath, szFullPath);
		return true;
	}
	sprintf(szFullPath, "../DemeterSampleData-3.20/");
	if (TestPath(szFullPath))
	{
		sprintf(szPath, szFullPath);
		return true;
	}
	sprintf(szFullPath, "..\\DemeterSampleData-3.20\\");
	if (TestPath(szFullPath))
	{
		sprintf(szPath, szFullPath);
		return true;
	}
	ShowDataError();
	return false;
}

bool TestPath(char *szPath)
{
	cout << "DEMETER SAMPLE: Looking for sample data at " << szPath << endl;
	char szFilename[1024];
	sprintf(szFilename, "%sSkyBox0000.bmp", szPath);
	FILE *testFile = fopen(szFilename, "rb");
	bool success = (testFile != NULL);
	if (success)
		fclose(testFile);
	return success;
}

const char *szDataError = "ERROR: Sample data not found.\nThe sample data is required in order to run the sample programs that come bundled with the \
Demeter library. This data is a separate download from the main Demeter distribution. You \
must download and install this seperate sample data package before you will be able to run \
any of the sample applications. If you have already installed the sample data, but the sample \
apps still can't find it (e.g. you installed it in a non-standard location), then you can specify \
its location with an environment variable called DEMETER_SAMPLE_DATA_PATH. For example, if you \
downloaded the sample data and installed it in /usr/share/TheData then you could issue this bash shell \
command before running any of the samples:\n\n \
export DEMETER_SAMPLE_DATA_PATH=/usr/share/TheData\n\n \
or, in Windows, set this environment variable through System Properties/Advanced.\n\n \
Alternatively, you can run the sample applications from the directory in which the sample data is \
installed. For example, if the data were installed at /opt/sampledata then you could do a:\n\n \
cd /opt/sampledata \n\n \
and then run the samples from there.";

void ShowDataError()
{
#ifdef _WIN32
	MessageBox(NULL, szDataError, "Demeter Sample", MB_OK);
#endif
	cout << szDataError << endl;
}
