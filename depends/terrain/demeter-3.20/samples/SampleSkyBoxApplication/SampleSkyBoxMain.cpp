// Demeter Terrain Visualization Library by Clay Fowler
// Copyright (C) 2001 Clay Fowler

/* 
This sample shows how to use the SkyBox class included with Demeter. This sample relies on
the GDAL library and SDL and SDL_image, but you don't need these libraries in order to use SkyBox.

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

#include <Demeter/Terrain.h>
#include <Demeter/Loader.h>
#include <iostream>
#include "Camera.h"
#include "Matrix.h"
#include <SDL.h>

using namespace Demeter;
using namespace std;

// App configuration
const int ScreenWidth = 800;
const int ScreenHeight = 600;
bool bFullscreen = true;
const bool bUseBorders = false;
const bool bAutomaticallyAdjustForTargetFrameRate = true;
const float TARGET_FRAME_RATE = 30.0f;
const float MAX_THRESHOLD = 50.0f;
const float MIN_THRESHOLD = 3.0f;

// Environment/control settings
const float CAMERA_SPEED = 150.0f;	// Meters per second
const float CAMERA_ROTATE_SPEED = 0.5f;	// Radians per second
const float CAMERA_BORDER_SIZE = 1500.0f;	// Meters
const float MAX_VIEW_DISTANCE = 6000.0f;	// Meters
const float FOG_RED = 0.54f;
const float FOG_GREEN = 0.55f;
const float FOG_BLUE = 0.62f;
const float FOG_ALPHA = 0.0f;

void ToggleWireframe(char *szMsg);
bool bWireframe = false;
float currentRenderSpeed = TARGET_FRAME_RATE;
Terrain *pTerrain = NULL;

extern bool GetSampleDataPath(char *szPath);

SkyBox *BuildSkyBox();

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, char **argv)
#endif
{
#ifdef _DEBUG
	bFullscreen = false;
	#ifdef _WIN32
		MessageBox(NULL,"You are running a DEBUG build so performance will be very bad.\nRun a release build for better performance.","Warning",MB_OK);
	#endif
#endif

	char szMediaPath[1024];
	if (!GetSampleDataPath(szMediaPath))
		exit(9);
	float threshold = 8.0f;	// The "detail level" of the terrain - higher values will render faster but yield less visual quality
	bool continueRunning = true;
	int cycles = 0;
	float cycleDuration = 0.03f;

	Settings::GetInstance()->SetVerbose(true);
	Settings::GetInstance()->SetScreenWidth(ScreenWidth);
	Settings::GetInstance()->SetScreenHeight(ScreenHeight);
	Settings::GetInstance()->SetMediaPath(szMediaPath);

	// Initialize the rendering surface
	if (Settings::GetInstance()->IsVerbose())
		cout << "Initializing video" << endl;
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
	{
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}
	atexit(SDL_Quit);

	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_EnableUNICODE(1);
	if (Settings::GetInstance()->IsVerbose())
		cout << "VIEWER: Setting video mode (OpenGL)" << endl;
	Uint32 screenFlags = SDL_OPENGL;
	if (bFullscreen)
		screenFlags |= SDL_FULLSCREEN;
	SDL_Surface *pScreen = SDL_SetVideoMode(Settings::GetInstance()->GetScreenWidth(), Settings::GetInstance()->GetScreenHeight(), 32, screenFlags);
	if (pScreen == NULL)
	{
		cout << "VIEWER: Unable to set screen resolution" << endl;
		exit(2);
	}
	SDL_WM_SetCaption("Demeter", "Demeter");
	if (Settings::GetInstance()->IsVerbose())
	{
		cout << "VIEWER: OpenGL vendor: " << glGetString(GL_VENDOR) << endl;
		cout << "VIEWER: OpenGL extensions supported: " << glGetString(GL_EXTENSIONS) << endl;
	}
// Create the terrain
	try
	{
		const int MAX_NUM_VISIBLE_TRIANGLES = 50000;	// Chosen based on the expected number of triangles that will be visible on-screen at any one time (the terrain mesh will typically have far more triangles than are seen at one time, especially with dynamic tessellation)
		pTerrain = new Terrain(MAX_NUM_VISIBLE_TRIANGLES,0.0f,0.0f);
#ifdef _DEBUG
		Loader::GetInstance()->LoadElevations("GDALElevationLoaderDebug", "LlanoElev.png,20.0,3.0", pTerrain);
		Loader::GetInstance()->LoadTerrainTexture("SDLTextureLoaderDebug", "LlanoTex.jpg", pTerrain);
		Loader::GetInstance()->LoadCommonTerrainTexture("SDLTextureLoaderDebug", "dirt2.jpg", pTerrain);
#else
		Loader::GetInstance()->LoadElevations("GDALElevationLoader", "LlanoElev.png,20.0,3.0", pTerrain);
		Loader::GetInstance()->LoadTerrainTexture("SDLTextureLoader", "LlanoTex.jpg", pTerrain);
		Loader::GetInstance()->LoadCommonTerrainTexture("SDLTextureLoader", "dirt2.jpg", pTerrain);
#endif
	}
	catch(DemeterException * pEx)
	{
		cout << pEx->GetErrorMessage() << endl;
		exit(0);
	}
	catch(...)
	{
		cout << "VIEWER: Unexpected exception while creating terrain" << endl;
		exit(0);
	}
	try
	{
		pTerrain->SetMaximumVisibleBlockSize(64);
		// Setup the sky box
		SkyBox *pSkyBox = BuildSkyBox();
		// Setup 3D scene
		// Vanilla OpenGL setup
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glViewport(0, 0, Settings::GetInstance()->GetScreenWidth(), Settings::GetInstance()->GetScreenHeight());
		glFrustum(-0.5f, 0.5f, -0.5f, 0.5f, 0.65f, MAX_VIEW_DISTANCE);
		glClearColor(0.5f, 0.75f, 1.0f, 0.0f);
		glDisable(GL_NORMALIZE);
		glEnable(GL_DEPTH_TEST);
		glShadeModel(GL_SMOOTH);
		// Setup camera
		Vector cameraPosition;
		cameraPosition.x = pTerrain->GetWidth() / 2.0f;
		cameraPosition.y = pTerrain->GetHeight() / 2.0f;
		cameraPosition.z = 100.0f;
		Camera camera(pTerrain);
		camera.SetPosition(cameraPosition.x, cameraPosition.y);
		camera.SetHoverElevation(100.0f);
		// Setup fog - it is highly recommended that apps ALWAYS use fog for realistic terrains...
		float fogColor[4];
		fogColor[0] = FOG_RED;
		fogColor[1] = FOG_GREEN;
		fogColor[2] = FOG_BLUE;
		fogColor[3] = FOG_ALPHA;
		glEnable(GL_FOG);
		glFogf(GL_FOG_MODE, GL_LINEAR);
		glFogfv(GL_FOG_COLOR, fogColor);
		glFogf(GL_FOG_START, 100.0f);
		glFogf(GL_FOG_END, MAX_VIEW_DISTANCE);
		glHint(GL_FOG_HINT, GL_FASTEST);

		// Main control and rendering loop
		//  SDL_WM_GrabInput(SDL_GRAB_ON); // Grab input can be useful for game-like applications
		int numPeriodsSkipped = 0;
		Uint32 startTime = SDL_GetTicks();
		Uint32 wireframeToggledTime = startTime;
		while (continueRunning)
		{
			// Setup viewing transformation
			camera.Update();
			camera.GetPosition(cameraPosition);
			pSkyBox->SetCameraPosition(cameraPosition.x, cameraPosition.y, cameraPosition.z);
			// Render the terrain and sky
			pTerrain->SetDetailThreshold(threshold);
			pTerrain->ModelViewMatrixChanged();
			glClear(GL_DEPTH_BUFFER_BIT);	// We only clear the depth buffer because the skybox will fill the color buffer
			glDisable(GL_FOG);	// Disable fog for the sky box
			pSkyBox->Render();
			glEnable(GL_FOG);	// Reenable it for the terrain
			pTerrain->Render();
			// Swap the double buffer so we can see what we just rendered
			SDL_GL_SwapBuffers();
			// Use SDL for keyboard input
			int numKeys;
			Uint8 *pKeys = SDL_GetKeyState(&numKeys);
			// Adjust control velocities based on user's current execution speed
			float rotateSpeed = cycleDuration * CAMERA_ROTATE_SPEED;
			float translateSpeed = cycleDuration * CAMERA_SPEED;
			// Handle mouse camera commands
			int mouseX, mouseY;
			SDL_GetMouseState(&mouseX, &mouseY);
			if (mouseX == 0)
				camera.AddYaw(-rotateSpeed);
			if (mouseX == Settings::GetInstance()->GetScreenWidth() - 1)
				camera.AddYaw(rotateSpeed);
			if (mouseY == 0)
			{
				if (pKeys[SDLK_s])
					camera.AddPitch(rotateSpeed);
				else
					camera.TranslateForward(translateSpeed);
			}
			if (mouseY == Settings::GetInstance()->GetScreenHeight() - 1)
			{
				if (pKeys[SDLK_s])
					camera.AddPitch(-rotateSpeed);
				else
					camera.TranslateBackward(translateSpeed);
			}
			// Handle miscellaneous input
			SDL_PumpEvents();
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
					continueRunning = false;
			}

			// Handle keyboard commands
			if (pKeys[SDLK_t])
			{
				if (1000 < SDL_GetTicks() - wireframeToggledTime)
				{
					ToggleWireframe(NULL);
					wireframeToggledTime = SDL_GetTicks();
				}
			}
			if (pKeys[SDLK_w])
				camera.TranslateForward(translateSpeed);
			if (pKeys[SDLK_x])
				camera.TranslateBackward(translateSpeed);
			if (pKeys[SDLK_a])
				camera.TranslateLeft(translateSpeed);
			if (pKeys[SDLK_d])
				camera.TranslateRight(translateSpeed);
			if (pKeys[SDLK_q])
				camera.SetHoverElevation(camera.GetHoverElevation() + translateSpeed);
			if (pKeys[SDLK_z])
				camera.SetHoverElevation(camera.GetHoverElevation() - translateSpeed);
			if (pKeys[SDLK_ESCAPE])
				continueRunning = false;

			if (!bAutomaticallyAdjustForTargetFrameRate)
			{
				if (pKeys[SDLK_KP_PLUS])
				{
					// Increase the detail threshold (make the terrain less detailed - worse looking but faster)
					threshold += 0.5f;
					cout << "VIEWER: Threshold = " << threshold << endl;
				}
				if (pKeys[SDLK_KP_MINUS])
				{
					// Decrease the detail threshold (make the terrain more detailed - better looking but slower)
					threshold -= 0.5f;
					cout << "VIEWER: Threshold = " << threshold << endl;
				}
			}

			// Clamp all control values to legal limits
			if (threshold < 0.0f)
				threshold = 0.0f;
			camera.GetPosition(cameraPosition);
			if (cameraPosition.x < CAMERA_BORDER_SIZE)
				cameraPosition.x = CAMERA_BORDER_SIZE;
			if (pTerrain->GetWidth() - CAMERA_BORDER_SIZE < cameraPosition.x)
				cameraPosition.x = pTerrain->GetWidth() - CAMERA_BORDER_SIZE;
			if (cameraPosition.y < CAMERA_BORDER_SIZE)
				cameraPosition.y = CAMERA_BORDER_SIZE;
			if (pTerrain->GetHeight() - CAMERA_BORDER_SIZE < cameraPosition.y)
				cameraPosition.y = pTerrain->GetHeight() - CAMERA_BORDER_SIZE;
			camera.SetPosition(cameraPosition.x, cameraPosition.y);

			// Measure execution speed so control rates can be properly updated
			Uint32 elapsedTime = SDL_GetTicks() - startTime;
			if (500 < elapsedTime)
			{
				currentRenderSpeed = (float)cycles / ((float)elapsedTime / 1000.0f);
				if (++numPeriodsSkipped == 20)	// Show framerate every 6 seconds
				{
					numPeriodsSkipped = 0;
					cout << "VIEWER: " << currentRenderSpeed << " frames per second" << endl;
				}
				cycles = 0;
				startTime = SDL_GetTicks();
				if (bAutomaticallyAdjustForTargetFrameRate && 2.0f < (float)fabs(TARGET_FRAME_RATE - currentRenderSpeed))
				{
					if (currentRenderSpeed < TARGET_FRAME_RATE)
						threshold += (TARGET_FRAME_RATE - currentRenderSpeed) * 0.1f;
					else
						threshold -= (currentRenderSpeed - TARGET_FRAME_RATE) * 0.1f;
					if (MAX_THRESHOLD < threshold)
						threshold = MAX_THRESHOLD;
					else if (threshold < MIN_THRESHOLD)
						threshold = MIN_THRESHOLD;
					cout << "Auto threshold = " << threshold << endl;
				}
			}
			cycles++;
		}
		SDL_WM_GrabInput(SDL_GRAB_OFF);
		SDL_Quit();
	}
	catch(DemeterException * pEx)
	{
		cout << pEx->GetErrorMessage() << endl;
		SDL_WM_GrabInput(SDL_GRAB_OFF);
		SDL_Quit();
	}
	catch(...)
	{
		SDL_WM_GrabInput(SDL_GRAB_OFF);
		SDL_Quit();
	}
	return 1;
}

void ToggleWireframe(char *szMsg)
{
	bWireframe = !bWireframe;
	if (bWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

SkyBox *BuildSkyBox()
{
#ifdef _DEBUG
	Texture *pTextureFront = Loader::GetInstance()->LoadTexture("SDLTextureLoaderDebug", "SkyBox0000.bmp,true,false,true");
	Texture *pTextureRight = Loader::GetInstance()->LoadTexture("SDLTextureLoaderDebug", "SkyBox0001.bmp,true,false,true");
	Texture *pTextureBack = Loader::GetInstance()->LoadTexture("SDLTextureLoaderDebug", "SkyBox0002.bmp,true,false,true");
	Texture *pTextureLeft = Loader::GetInstance()->LoadTexture("SDLTextureLoaderDebug", "SkyBox0003.bmp,true,false,true");
	Texture *pTextureBottom = Loader::GetInstance()->LoadTexture("SDLTextureLoaderDebug", "SkyBox0004.bmp,true,false,true");
	Texture *pTextureTop = Loader::GetInstance()->LoadTexture("SDLTextureLoaderDebug", "SkyBox0005.bmp,true,false,true");
#else
	Texture *pTextureFront = Loader::GetInstance()->LoadTexture("SDLTextureLoader", "SkyBox0000.bmp,true,false,true");
	Texture *pTextureRight = Loader::GetInstance()->LoadTexture("SDLTextureLoader", "SkyBox0001.bmp,true,false,true");
	Texture *pTextureBack = Loader::GetInstance()->LoadTexture("SDLTextureLoader", "SkyBox0002.bmp,true,false,true");
	Texture *pTextureLeft = Loader::GetInstance()->LoadTexture("SDLTextureLoader", "SkyBox0003.bmp,true,false,true");
	Texture *pTextureBottom = Loader::GetInstance()->LoadTexture("SDLTextureLoader", "SkyBox0004.bmp,true,false,true");
	Texture *pTextureTop = Loader::GetInstance()->LoadTexture("SDLTextureLoader", "SkyBox0005.bmp,true,false,true");
#endif
	SkyBox *pSkyBox = new SkyBox(MAX_VIEW_DISTANCE * 0.7f, pTextureLeft, pTextureFront, pTextureRight, pTextureBack, pTextureTop, pTextureBottom);
	return pSkyBox;
}
