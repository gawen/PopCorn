/*
    Copyright (C) 2003  Arana Sébastien, Arab Gawen
    
    This file is part of PopCorn Library.

    Foobar is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef _POPCORN_
#define _POPCORN_

//mode debug activé
#define __DEBUG__



//============ INCLUDE SYSTEM
//graphique
#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif
#include "../include/GL/GL.H"
#include "../include/GL/GLU.h"
#include "../include/sdl/SDL.h"
#include "../include/devil/il.h"
#include "../include/devil/ilu.h"
#include "../include/devil/ilut.h"
//sonore
#include "../include/fmod/fmod.h"
//divers
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <io.h>


//============ VERSION POP CORN
// Version
#define CRN_VERSION_MINOR    1
#define CRN_VERSION_MAJOR    0
#define CRN_VERSION_RELEASE  0

//============ DEFINITION DE MACROS ET CONSTANTES
#define CRN_PI    3.14159 26535 89793 23846 26433

namespace PopCorn
{
    /*
	=============================================
	Color
	=============================================
	Classe contenant une couleur
    */
    class Color
    {
        public :
            unsigned char Red, Green, Blue, Alpha;
            Color(void);
            Color(unsigned char Red, unsigned char Green, unsigned char Blue);
            Color(unsigned char Red, unsigned char Green, unsigned char Blue, unsigned char Alpha);
            
            void SetRGB (unsigned char Red, unsigned char Green, unsigned char Blue);
            void SetRGBA(unsigned char Red, unsigned char Green, unsigned char Blue, unsigned char Alpha);
            
            void ActiveRGB(void);
            void ActiveRGBA(void);
    };
    
    
    /*
	=============================================
	Color
	=============================================
	Classe contenant une couleur
    */
    class Coordinate
    {
        public :
            float X, Y, Z;
            Coordinate(void);
            Coordinate(float X, float Y, float Z);
            
            void Set(float X, float Y, float Z);
            
            void Position(void);
            void Scale(void);
            void Rotate(void);
            
            bool isNull();
    };
    typedef class Coordinate Position;
    typedef class Coordinate Scale;
    typedef class Coordinate Rotation;


    /*
	=============================================
	Object
	=============================================
	Classe mère de chaque objet de PopCorn ...
    */
    class Objects;
    class Object
    {
        private :
            virtual void _draw(void)=0;
        public :
            Object();
            void Draw(void);
            bool PushTo(Objects& Obj);
            bool PopTo(Objects& Obj);
            bool BePersistant(void);
            bool BeNotPersistant(void);
            bool Visible;
            Coordinate Position;
            Coordinate Scale;
            Coordinate Rotation;
    };
    
    
    /*
	=============================================
	Objects
	=============================================
	Classe permettant de lister des objets PopCorn
    */
    class Objects : public Object
    {
        private :
            Object* List[128];
        public :
            Objects(void);
            bool PushObject(Object& Obj);
            bool PopObject(Object& Obj);
        protected :
            void _draw(void);
    };
    
    
    /*
	=============================================
	ObjDrawFunc
	=============================================
	Permet d'appeller une fonction lorsque l'objet
    doit être dessiner. Remplace la fonction classique
    Draw d'OpenGl.
    */
    class ObjDrawFunc : public Object
    {
        public :
            ObjDrawFunc(void);
            void (*DrawFunction)(void);
        protected :
            void _draw(void);
    };
    
 
    /*
	=============================================
	VideoConfig
	=============================================
	Classe permettant de configurer le context video
	(longueur, largeur, et si on passe en mode plein ecran)	*/
	extern class _VideoConfig
	{
		private :
			unsigned int flags;
		public :
			unsigned short Width;
			unsigned short Height;
			bool FullScreen;
		    _VideoConfig(void);
			void SetVideoResolution(unsigned short width, unsigned short height, bool fullscreen);
			bool Init(void);
			void Flush(void);
			
			void ShowMouse(bool Activate);
			
			~_VideoConfig();
	} Video;
	
	
    /*
	=============================================
	AudioConfig
	=============================================
	Classe permettant de configurer le mode audio
	(qualité du son, etc ...)	*/
	extern class _AudioConfig
	{
        private :
            int mixrate;
            int channels;
        public :
            _AudioConfig(void);
            void SetAudioResolution(int mixrate, int channels);
            bool Init(void);
            ~_AudioConfig();
    } Audio;
    
    
	/*
	=============================================
	InputsConfig
	=============================================
	La classe qui regroupe les évènements d'Inputs.
    Cette classe existe pour une meilleure portabilité
    entre les différents gestionnaires de fenètres.
	*/
    extern class _EventsConfig
    {
        public :
	        void (*Keyboard)(bool KeyDown, SDLKey Key);
            void (*MouseMotion)(unsigned short x, unsigned short y, bool LeftButton, bool MiddleButton, bool RightButton);
            void (*MouseClick)(unsigned short x, unsigned short y, bool LeftButton, bool MiddleButton, bool RightButton);
            void (*Initialization)(void);
            void (*Exit)(void);
        
            _EventsConfig(void);
            bool Init(void);
	        void Flush(void);
    } Events;
    

	/*
	=============================================
	Timer
	=============================================
	la classe timer permet de générer un timer et
	de le manipuler. Un timer de base est créé avec
	l'application. La methode Reset permet de le remettre
	à zero, et get permet de recuperer le nombre de
	milisecondes ecoulées depuis son lancement.*/
	class Timer
	{
		protected :
			long MiliStart;
		public :
			Timer(void);
			void Start(void);
			void Reset(void);
			long Get(void);
	};


	/*
	=============================================
	TimerEvent
	=============================================
	Classe Timer qui appelle toutes les x msec une
    fonction prédéfinite */
    class TimerEvent : public Object
    {
        private :
            Timer timer;
        public :
            unsigned int Delay;
            void (*TimerFunc)(void);
            TimerEvent(void);
        protected :
            void _draw(void);
    };
    
    
	/*
	=============================================
	TimerFPS
	=============================================
	la classe ImerFPS (dérivée Timer) permet de 
	connaitre le nombre de fois qu'une fonction est
	appelée par seconde. Pour cela il suffit de lancer
	le TimerFPS avant de lancer une boucle, puis
	de placer dans la fonction choisie la methode refresh.
	la methode Get permettra de connaitre les FPS en cours.*/
	extern class _TimerFPS : Timer
	{
		private :
			unsigned int compteur;
		public :
			unsigned int FPS;
			unsigned int FPSmin;
			unsigned int FPSmax;
			void Start(void);
			void Refresh(void);
	} FPS;


	#ifdef __DEBUG__
	/*
	=============================================
	Debug
	=============================================
	la classe Debug doit permettre au developpeur
	d'acceder en ecriture à un fichier servant de log.
	Le nom du fichier sera passé lors de l'instance de
	la classe. (au constructeur). Le destructeur se chargera
	de fermer le fichier.*/
	extern class _Debug
	{
		private :
			const char *debugFile;
		public :
			FILE *Fp;
			~_Debug(void);
			_Debug(const char *fichier);
	} Debug;
	#endif


	/*
	=============================================
	JukeBox
	=============================================
	Cette classe est censée gérer toutes les
	musiques chargées dans le moteur. les musiques
	sont representés par des numero, de 1 à 255.
	*/
    typedef unsigned char Music;
	extern class _JukeBox
	{
		private :
			FMUSIC_MODULE *MusicTab[255];
			bool Looping;
			bool Paused;
		public:
			_JukeBox(void);
			~_JukeBox(void);
			void StopAll(void); //stopallsong
			Music AddMusique(const char *Fichier);
			bool RemoveMusique(unsigned char index);
			bool Play(unsigned char index); //playsong
			bool ToggleLoop(unsigned char index); //setlooping
			bool Pitch(unsigned char index,float percent); //setmasterspeed
			bool Volume(unsigned char index,float percent); //setmastervolume
			bool Pan(unsigned char index,float percent); //setpanseparation
			bool TogglePause(unsigned char index); //setpaused
			bool Stop(unsigned char index); //stopsong
	} JukeBox;
	
	
	/*
	=============================================
	Scene
	=============================================
	Classe contenant les informations concernant
    la scène qui sera affiché, dont par exemple la
    couleur de background, les lumières, les objets
    à afficher, etc ...
	*/
	extern class _Scene
	{
	    public :
	        Color BackGround;    // Background noir
	        Objects ListObjects;
	} Scene;
	
	
	/*
	=============================================
	void Go(void);
	=============================================
	Cette fonction démarre la boucle principal :
    à partir d'elle, c'est le moteur qui prend la
    main.
	*/
    bool Go(void);
    
	/*
	=============================================
	void Stop(void);
	=============================================
	Cette fonction ferme PopCorn
	*/
	void Stop(void);
};

#if defined(WIN32) && defined(__DEBUG__)
#define DebugMsgBox(str) MessageBox(HWND_DESKTOP, str, "Debug", MB_OK);
#endif

#endif
