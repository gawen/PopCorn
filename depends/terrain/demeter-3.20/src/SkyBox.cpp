#include "Demeter/SkyBox.h"

using namespace Demeter;
using namespace std;

extern PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB_ptr;

SkyBox::SkyBox(float diameter, Demeter::Texture * pTextureLeft, Demeter::Texture * pTextureFront, Demeter::Texture * pTextureRight, Demeter::Texture * pTextureBack, Demeter::Texture * pTextureTop, Demeter::Texture * pTextureBottom)
{
	m_pTextures = new Texture *[6];
	m_pTextures[BOX_LEFT] = pTextureLeft;
	m_pTextures[BOX_FRONT] = pTextureFront;
	m_pTextures[BOX_RIGHT] = pTextureRight;
	m_pTextures[BOX_BACK] = pTextureBack;
	m_pTextures[BOX_BOTTOM] = pTextureBottom;
	m_pTextures[BOX_TOP] = pTextureTop;
	m_BottomLeft.x = -diameter;
	m_BottomLeft.y = -diameter;
	m_BottomLeft.z = -diameter;
	m_TopRight.x = diameter;
	m_TopRight.y = diameter;
	m_TopRight.z = diameter;
}

void SkyBox::SetCameraPosition(float x, float y, float z)
{
	m_CameraPos.x = x;
	m_CameraPos.y = y;
	m_CameraPos.z = z;
}

void SkyBox::Render()
{
	glPushMatrix();
	glTranslatef(m_CameraPos.x, m_CameraPos.y, m_CameraPos.z);
	glColor3f(1.0f, 1.0f, 1.0f);
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glClientActiveTextureARB_ptr(GL_TEXTURE0_ARB);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_ALWAYS);
	// Left face
	glBindTexture(GL_TEXTURE_2D, m_pTextures[BOX_LEFT]->UploadTexture());
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(m_BottomLeft.x, m_BottomLeft.y, m_BottomLeft.z);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(m_BottomLeft.x, m_BottomLeft.y, m_TopRight.z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(m_BottomLeft.x, m_TopRight.y, m_TopRight.z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(m_BottomLeft.x, m_TopRight.y, m_BottomLeft.z);
	glEnd();
	// Front face
	glBindTexture(GL_TEXTURE_2D, m_pTextures[BOX_FRONT]->UploadTexture());
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(m_BottomLeft.x, m_TopRight.y, m_BottomLeft.z);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(m_BottomLeft.x, m_TopRight.y, m_TopRight.z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(m_TopRight.x, m_TopRight.y, m_TopRight.z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(m_TopRight.x, m_TopRight.y, m_BottomLeft.z);
	glEnd();
	// Right face
	glBindTexture(GL_TEXTURE_2D, m_pTextures[BOX_RIGHT]->UploadTexture());
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(m_TopRight.x, m_TopRight.y, m_BottomLeft.z);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(m_TopRight.x, m_TopRight.y, m_TopRight.z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(m_TopRight.x, m_BottomLeft.y, m_TopRight.z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(m_TopRight.x, m_BottomLeft.y, m_BottomLeft.z);
	glEnd();
	// Back face
	glBindTexture(GL_TEXTURE_2D, m_pTextures[BOX_BACK]->UploadTexture());
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(m_TopRight.x, m_BottomLeft.y, m_BottomLeft.z);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(m_TopRight.x, m_BottomLeft.y, m_TopRight.z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(m_BottomLeft.x, m_BottomLeft.y, m_TopRight.z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(m_BottomLeft.x, m_BottomLeft.y, m_BottomLeft.z);
	glEnd();
	// Top face
	glBindTexture(GL_TEXTURE_2D, m_pTextures[BOX_TOP]->UploadTexture());
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(m_BottomLeft.x, m_TopRight.y, m_TopRight.z);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(m_BottomLeft.x, m_BottomLeft.y, m_TopRight.z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(m_TopRight.x, m_BottomLeft.y, m_TopRight.z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(m_TopRight.x, m_TopRight.y, m_TopRight.z);
	glEnd();
	// Bottom face
	glBindTexture(GL_TEXTURE_2D, m_pTextures[BOX_BOTTOM]->UploadTexture());
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(m_BottomLeft.x, m_BottomLeft.y, m_BottomLeft.z);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(m_BottomLeft.x, m_TopRight.y, m_BottomLeft.z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(m_TopRight.x, m_TopRight.y, m_BottomLeft.z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(m_TopRight.x, m_BottomLeft.y, m_BottomLeft.z);
	glEnd();
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glPopMatrix();
}

SkyBox::~SkyBox()
{
	delete[]m_pTextures;
}
