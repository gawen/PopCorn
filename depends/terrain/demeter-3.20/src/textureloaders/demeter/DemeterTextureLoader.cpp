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
Texture *ReadTextureElement(xmlNodePtr textureElement);
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

extern "C" LOADER_API void LoadTerrainTexture(int argc, const char **argv, Terrain * pTerrain) THROW_SPEC
{
	if (argc != 1)
		throw new DemeterException("DEMETER LOADER: requires 1 parameter (filename)");
	const char *szFilename = argv[0];

	char szFullFilename[1024];
	Settings::GetInstance()->PrependMediaPath(szFilename, szFullFilename, 1024);
	if (Settings::GetInstance()->IsVerbose())
		cout << "DEMETER LOADER: Parsing XML file " << szFullFilename << endl;

	doc = xmlParseFile(szFullFilename);
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

	int numTextureCellsX, numTextureCellsY;

	xmlNodePtr terrainNode = GetChildElement(xmlDocGetRootElement(doc), "terrain");
	xmlNodePtr element = GetChildElement(terrainNode, "widthTextureCells");
	numTextureCellsX = GetIntValue(element);
	element = GetChildElement(terrainNode, "heightTextureCells");
	numTextureCellsY = GetIntValue(element);
	element = GetChildElement(terrainNode, "textureSet");
	xmlNodePtr node;

	// Ensure that order is unimportant in the XML file.
	vector < Texture * >sharedTextures;
	node = element->xmlChildrenNode;
	while (node != NULL)
	{
		if (xmlStrcmp(node->name, (const xmlChar *)"texture") == 0)
		{
			Texture *pTexture = ReadTextureElement(node);
			sharedTextures.push_back(pTexture);
		}
		node = node->next;
	}
	for (unsigned int i = 0; i < sharedTextures.size(); i++)
	{
		Texture *pTexture = NULL;
		for (unsigned int j = 0; j < sharedTextures.size() && pTexture == NULL; j++)
		{
			Texture *pCandidateTex = sharedTextures[i];
			if ((unsigned int)pCandidateTex->GetSharedIndex() == i)
				pTexture = pCandidateTex;
		}
		pTerrain->GetTextureSet()->AddTexture(pTexture);
	}

	pTerrain->AllocateTextureCells(numTextureCellsX, numTextureCellsY);

	node = terrainNode->xmlChildrenNode;
	while (node != NULL)
	{
		if (xmlStrcmp(node->name, (const xmlChar *)"textureCell") == 0)
		{
			xmlNodePtr element = GetChildElement(node, "positionX");
			int posX = GetIntValue(element);
			element = GetChildElement(node, "positionY");
			int posY = GetIntValue(element);
			xmlNodePtr textureElement = GetChildElement(node, "baseTexture");
			Texture *pTexture = ReadTextureElement(textureElement);
			TextureCell *pCell = new TextureCell(posY * numTextureCellsX + posX);
			pCell->SetTexture(pTexture);

			xmlNodePtr childNode = node->xmlChildrenNode;
			while (childNode != NULL)
			{
				if (xmlStrcmp(childNode->name, (const xmlChar *)"detailTexture") == 0)
				{
					xmlNodePtr element = GetChildElement(childNode, "sharedTexture");
					int sharedIndex = GetIntValue(element);
					xmlNodePtr textureElement = GetChildElement(childNode, "mask");
					Texture *pMask = ReadTextureElement(textureElement);
					DetailTexture *pDetail = new DetailTexture(NULL);
					pDetail->SetTexture(pTerrain->GetTextureSet()->GetTexture(sharedIndex));
					pDetail->SetMask(pMask);
					pCell->AddDetail(pDetail);
				}
				childNode = childNode->next;
			}

			pTerrain->SetTextureCell(posY * numTextureCellsX + posX, pCell);
		}
		node = node->next;
	}

	node = terrainNode->xmlChildrenNode;
	while (node != NULL)
	{
		if (xmlStrcmp(node->name, (const xmlChar *)"commonTexture") == 0)
		{
			Texture *pTexture = ReadTextureElement(node);
			pTerrain->SetCommonTexture(pTexture);
		}
		node = node->next;
	}
}

Texture *ReadTextureElement(xmlNodePtr textureElement)
{
	xmlNodePtr element = GetChildElement(textureElement, "width");
	int width = GetIntValue(element);
	element = GetChildElement(textureElement, "height");
	int height = GetIntValue(element);
	element = GetChildElement(textureElement, "bpp");
	int bpp = GetIntValue(element);
	element = GetChildElement(textureElement, "isClamped");
	char szValue[256];
	GetElementText(element, szValue);
	bool isClamped = (strcmp(szValue, "true") == 0);
	element = GetChildElement(textureElement, "useCompression");
	GetElementText(element, szValue);
	bool useCompression = (strcmp(szValue, "true") == 0);
	element = GetChildElement(textureElement, "borderSize");
	int borderSize = GetIntValue(element);
	element = GetChildElement(textureElement, "sharedID");
	int sharedID = GetIntValue(element);
	element = GetChildElement(textureElement, "format");
	int textureFormat = GetIntValue(element);
	element = GetChildElement(textureElement, "imageFile");
	char szFilename[1024];
	GetElementText(element, szFilename);
	Texture *pTexture = new Texture(szFilename, width, height, bpp, borderSize, isClamped, useCompression, textureFormat, sharedID);
	return pTexture;
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

xmlNodePtr GetChildElement(xmlNodePtr parent, char *szChildName, bool isRequired)
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
}

extern "C" LOADER_API void LoadCommonTerrainTexture(int argc, const char **argv, Terrain * pTerrain) THROW_SPEC
{
	throw new DemeterException("DEMETER LOADER: Does not support LoadCommonTexture - this plugin only loads textures from Demeter Editor XML Files");
}

extern "C" LOADER_API Texture * LoadTexture(int argc, const char **argv) THROW_SPEC
{
	throw new DemeterException("DEMETER LOADER: Does not support LoadTexture - this plugin only loads terrain textures from Demeter Editor XML Files");
	return NULL;		// Appease brain-dead compilers
}
