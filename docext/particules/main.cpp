/*
*	Exemple de moteur de particules avec gestion de la physique
*
*  dake/calodox
*  http://www.calodox.org
*
*  Initialisation openGL : Blaine Hodge
*
*/

#include "stdafx.h"
#include "MoteurParticules.h"

#define	ECRAN_X		640	
#define	ECRAN_Y		480

// [ variables globales ] ////////////////////////////////////////////////////////////////////////////

GLuint texture;	

//////////////////////////////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void InitialiseOpenGL(HWND hWnd, HDC* hDC, HGLRC* hRC);
void EnleveOpenGL(HWND hWnd, HDC hDC, HGLRC hRC);

//////////////////////////////////////////////////////////////////////////////////////////////////////

bool GenereTextureParticule()
{
	unsigned char *pixels = new unsigned char[32*32*3];

	// pour générer une particule, nous allons utiliser la distance entre le pixel et le centre
	// de la texture couplé à une fonction exponentielle. Cette fonction déterminera l'intensité
	// du pixel.

	int i = 0;

	for (int y = 0; y < 32; y++) {
		for (int x = 0; x < 32; x++) {
			float dx = (float)x - 16.0f;
			float dy = (float)y - 16.0f;

			// on trouve la distance, normalisée entre 0 et 1 et on l'inverse pour avoir 
			// le maximum d'intensité au centre
			float distance = sqrtf(dx * dx + dy * dy) / sqrtf(16*16 + 16*16);

			int intensite = expf(-distance * 8)*512.0f;
			if (intensite >= 255)
				intensite = 255;

			// pixel en RGB
			pixels[i] = intensite;
			pixels[i + 1] = intensite;
			pixels[i + 2] = intensite;

			i += 3;
		}
	}

	// crée la texture openGL
	glGenTextures(1, &texture);							
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 32, 32, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	delete[] pixels;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	WNDCLASS wc;
	HWND hWnd;
	HDC hDC;
	HGLRC hRC;
	MSG msg;
	BOOL quitter = FALSE;

	// les initialisations standard de Win32
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "dake";
	RegisterClass(&wc);

	// création de la fenêtre
	hWnd = CreateWindow("dake", "Moteur particules", 
	WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
	0, 0, ECRAN_X, ECRAN_Y,
	NULL, NULL, hInstance, NULL);

	InitialiseOpenGL(hWnd, &hDC, &hRC);

	// calcul la texture de la particule
	GenereTextureParticule();

	// crée une instance de la classe MoteurParticules et rajoute un émetteur de type explosion
	MoteurParticules* Moteur = new MoteurParticules();
	Moteur->Genere_Explosion_Particules(V3D(0,0,0), 0.1f, NB_PARTICULES);


	while (!quitter) {
		// on teste les messages de Windows
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				quitter = TRUE;
			} else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		} else {
			// on affiche les particules
			Moteur->Afficher();
			SwapBuffers(hDC);
		}
	}

	delete Moteur;

	EnleveOpenGL(hWnd, hDC, hRC);

	// on détruit la fenêtre
	DestroyWindow(hWnd);

	return msg.wParam;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

// fonction qui prend en charge les messages/events envoyés par Windows à notre programme.

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_CREATE:
		return 0;

	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	case WM_DESTROY:
		return 0;

	case WM_KEYDOWN:
		switch (wParam) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			return 0;
		}
		return 0;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

// Mise en place des paramètres openGL et de la fenêtre

void InitialiseOpenGL(HWND hWnd, HDC* hDC, HGLRC* hRC)
{
	PIXELFORMATDESCRIPTOR pfd;
	int format;

	// on prend le "device context"
	*hDC = GetDC(hWnd);

	// on lui attribue un format de pixel, ici 24 bits
	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	format = ChoosePixelFormat(*hDC, &pfd);
	SetPixelFormat(*hDC, format, &pfd);

	// on active le "render context"
	*hRC = wglCreateContext(*hDC);
	wglMakeCurrent(*hDC, *hRC);

	// on efface le fond avec du noir et le zbuffer avec la valeur 1.0f
	glClearColor(0.0f,0.0f,0.0f,0.0f);				
	glClearDepth(1.0f);			

	// paramètres pour avoir un meilleur rendu, on peut les changer pour gagner de la vitesse
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);	
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);			

	// on active la gestion des textures et celle des particules est activée par défaut
	glEnable(GL_TEXTURE_2D);					
	glBindTexture(GL_TEXTURE_2D,texture);		
	glShadeModel(GL_SMOOTH);	


	// on met en place les attributs de caméra et d'écran
	glViewport(0,0,ECRAN_X,ECRAN_Y);					

	glMatrixMode(GL_PROJECTION);					
	glLoadIdentity();								

	gluPerspective(45.0f,(GLfloat)ECRAN_X / (GLfloat)ECRAN_Y,0.01f,20000.0f);

	glMatrixMode(GL_MODELVIEW);						
	glLoadIdentity();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void EnleveOpenGL(HWND hWnd, HDC hDC, HGLRC hRC)
{
	// on détruit le "render context" et le "device context"
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(hWnd, hDC);
}
