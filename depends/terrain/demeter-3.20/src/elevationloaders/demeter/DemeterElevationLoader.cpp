#include "Demeter/Terrain.h"
#if defined(_WIN32) || defined(__APPLE__)
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#else
#include <libxml2/libxml/xmlmemory.h>
#include <libxml2/libxml/parser.h>
#endif

using namespace Demeter;
using namespace std;

xmlDocPtr doc;
xmlNodePtr GetChildElement(xmlNodePtr parent, char *szChildName, bool isRequired = true);
void GetElementText(xmlNodePtr element, char *szTargetBuffer);
int GetIntValue(xmlNodePtr element);
float GetFloatValue(xmlNodePtr element);
float *ReadRawElevations(char *szFilename, int width, int height);
char szText[1024];

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

// TestBigEndian - for PPC support
#ifdef __APPLE__
#include <inttypes.h>
bool TestBigEndian(void)
{
	int16_t one = 1;
	char *cp = (char *)&one;

	if (*cp == 0)
	{
		return true;
	}

	return false;
}
#endif

extern "C" LOADER_API void LoadElevations(int argc, const char **argv, Terrain * pTerrain) THROW_SPEC
{
	if (argc != 1)
		throw new DemeterException("DEMETER LOADER: requires 1 params (filename)");

	const char *szFilename = argv[0];

	char szFullFilename[MAX_FILENAME_LENGTH];
	if (strstr(szFilename, "\\") || strstr(szFilename, "/"))
		sprintf(szFullFilename, szFilename);
	else
		Settings::GetInstance()->PrependMediaPath(szFilename, szFullFilename);
	try
	{
		doc = xmlParseFile(szFullFilename);
	}
	catch(...)
	{
		cout << "DEMETER LOADER: Failed to parse XML file" << endl;
	}
	if (doc == NULL)
	{
		string msg;
		msg += "The XML file ";
		msg += szFullFilename;
		msg += " is not valid";
		throw new DemeterException(msg);
	}

	if (Settings::GetInstance()->IsVerbose())
		cout << "DEMETER LOADER: Parsed XML successfully" << endl;

	int widthVertices, heightVertices;
	float vertexSpacing;

	xmlNodePtr terrainNode = GetChildElement(xmlDocGetRootElement(doc), "terrain");
	xmlNodePtr element = GetChildElement(terrainNode, "widthVertices");
	widthVertices = GetIntValue(element);
	element = GetChildElement(terrainNode, "heightVertices");
	heightVertices = GetIntValue(element);
	element = GetChildElement(terrainNode, "vertexSpacing");
	vertexSpacing = GetFloatValue(element);
	element = GetChildElement(terrainNode, "elevationFile");
	char szElevFilename[1024];
	GetElementText(element, szElevFilename);
	float *pElevations = ReadRawElevations(szElevFilename, widthVertices, heightVertices);
	if (pElevations == NULL)
		throw new DemeterException("Failed to read elevations file specified in XML file");
	pTerrain->SetAllElevations(pElevations, widthVertices, heightVertices, vertexSpacing, 1.0f);
	delete[]pElevations;
}

float *ReadRawElevations(char *szFilename, int width, int height)
{
	float *pBuffer = NULL;

	char szElevFullFilename[1024];
	Settings::GetInstance()->PrependMediaPath(szFilename, szElevFullFilename, 1024);
	FILE *file = fopen(szElevFullFilename, "rb");
	if (file != NULL)
	{
		pBuffer = new float[width * height];
		if ((int)fread(pBuffer, sizeof(float), width * height, file) != width * height)
		{
			delete[]pBuffer;
			pBuffer = NULL;
		}
		fclose(file);
	}

#ifdef __APPLE__
	if (TestBigEndian())
	{
		for (int counter = 0; counter < width * height; counter++)
		{

			unsigned char temp;
			unsigned char *ptr = (unsigned char *)&pBuffer[counter];
			temp = ptr[0];
			ptr[0] = ptr[3];
			ptr[3] = temp;
			temp = ptr[1];
			ptr[1] = ptr[2];
			ptr[2] = temp;

		}
	}
#endif

	return pBuffer;
}

int GetIntValue(xmlNodePtr element)
{
	GetElementText(element, szText);
	int val = atoi(szText);
	return val;
}

float GetFloatValue(xmlNodePtr element)
{
	GetElementText(element, szText);
	float val = (float)atof(szText);
	return val;
}

xmlNodePtr GetChildElement(xmlNodePtr parent, char *szChildName, bool isRequired)THROW_SPEC
{
	xmlNodePtr child = NULL;

	xmlNodePtr node = parent->xmlChildrenNode;
	while (node != NULL && child == NULL)
	{
		if (xmlStrcmp(node->name, (const xmlChar *)szChildName) == 0)
			child = node;
		node = node->next;
	}

	if (child == NULL)
	{
		string msg;
		msg += "The specified XML file does not contain the required element type <";
		msg += szChildName;
		msg += ">";
		throw new DemeterException(msg);
	}

	return child;
}

void GetElementText(xmlNodePtr element, char *szText)
{
	char *szElementText = (char *)xmlNodeListGetString(doc, element->xmlChildrenNode, 1);
	sprintf(szText, szElementText);
	//delete[] szElementText;
}
