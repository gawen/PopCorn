/*
    Copyright (C) 2003 The SilverCoders
    
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

// VideoConfig.cpp --> Vérification pour le multitexturing
// Musique.cpp     --> Problème de loopage des ziks

#ifndef _POPCORN_
#define _POPCORN_

#define __DEBUG__

#define CRN_NB_MULTITEXTURE     32

//#ifdef WIN32
//	#define WIN32_LEAN_AND_MEAN
//	#include <windows.h>
//#endif


#include "GL/GL.H"
#include "GL/GLU.h"
#include "GL/glext.h"
#include "sdl/SDL.h"
#include "sdl/SDL_Image.h"
//sonore
//#include "fmod.h"
#include <al/al.h>
#include <al/alc.h>
#include <al/alu.h>
#include <al/alut.h>
//divers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <io.h>
#if defined(_POPCORN_3DS_REQUIRED_) || defined(__MAIN__)
#   include <lib3ds/file.h>                        
#   include <lib3ds/camera.h>
#   include <lib3ds/mesh.h>
#   include <lib3ds/node.h>
#   include <lib3ds/material.h>
#   include <lib3ds/matrix.h>
#   include <lib3ds/vector.h>
#   include <lib3ds/light.h>
#endif

#define CRN_3DOBJS 0x1
#define CRN_FRONTOBJS 0x2
#define CRN_BACKOBJS 0x4
#define CRN_ALWOBJS 0x8

#ifndef PI
#   define PI    3.141592653
#endif

#define SDL_GetTicks_() (SDL_GetTicks()-PopCorn::TicksError)

namespace PopCorn
{
    extern Uint32 TicksError;
    
    // Extensions OpenGL ARB
    #define GL_ARRAY_BUFFER_ARB 0x8892
    #define GL_STATIC_DRAW_ARB 0x88E4
    typedef void (APIENTRY * PFNGLBINDBUFFERARBPROC) (GLenum target, GLuint buffer);
    typedef void (APIENTRY * PFNGLDELETEBUFFERSARBPROC) (GLsizei n, const GLuint *buffers);
    typedef void (APIENTRY * PFNGLGENBUFFERSARBPROC) (GLsizei n, GLuint *buffers);
    typedef void (APIENTRY * PFNGLBUFFERDATAARBPROC) (GLenum target, int size, const GLvoid *data, GLenum usage);

    extern PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;
    extern PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB;
    extern PFNGLGENBUFFERSARBPROC glGenBuffersARB;
    extern PFNGLBINDBUFFERARBPROC glBindBufferARB;
    extern PFNGLBUFFERDATAARBPROC glBufferDataARB;
    extern PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB;
    void InitGLExtensions(void);

    // Blending
    extern PFNGLBLENDEQUATIONEXTPROC glBlendEquationEXT;

    extern const double Pi180;
    
    // Variable de définition des noms de picking
    extern bool _Picking;

#   define CRN_TAG_VISIBLE        0x1
    typedef long ObjTag;
    
	// DynamicArray
	struct LISTE
    {
       void* val;
       LISTE* pSuivant;
    };
    class CLst
    {
    public :
    	CLst();
    	~CLst();

    	int GetNbElem();

    	int AddElem(void* val);
    	bool InsertElem(int nPos,void* val);
    	bool SetAt(int nPos, void* val);
    	bool DeleteElem(int nPos);
    	bool DeleteElem(void* val);
    	int Find(void* val);
    	void Delete();
    	
    	float* GetArray(void);
	
    	void* operator[] (int nPos);

	    LISTE* m_pDebut;
    private :
    	LISTE* m_pFin;
    	int m_nNbElem;

    	void* GetPtrAt(int nPos);
    };
    
    
    /*** Type de données ***/
    class Vector
    {
        private :
            float Array[4];
        
        public :
            float X, Y, Z;
            Vector(void);
            Vector(float X, float Y, float Z);
            
            void Set(float X, float Y, float Z);
            void Set(float X, float Y);
            
            void Position(void);
            void Scale(void);
            void Rotate(void);
            
            float* GetGLArray(void);
            
            void operator=(Vector& Coord);
            
            bool isNull();
    };
    typedef class Vector Position;
    typedef class Vector Scale;
    typedef class Vector Rotation;
    
    
    /*** Objets ***/
#   define _PopCornSetInfoFile(Position)    { Position->file=__FILE__; Position->line=__LINE__; }
    class MatTexture
    {
        public :
            virtual void Enable(void)=0;
            virtual void Disable(void)=0;
            virtual void TexCoord(float s, float t)=0;
//            virtual void TexCoordv(float* ar)=0;
            
            Vector Size;
    };
    
    class MatTexColor : public MatTexture
    {
        private :
            float Array[4];
        
        public :
            unsigned char Red, Green, Blue, Alpha;
            MatTexColor(void);
            
            void Set(unsigned char Red, unsigned char Green, unsigned char Blue);
            void Set(unsigned char Red, unsigned char Green, unsigned char Blue, unsigned char Alpha);
            
            void Enable(void);
            void Disable(void);
            void TexCoord(float s, float t);
            
            void EnableRGB(void);
            void DisableRGB(void);
            
            float* GetGLArray(void);
    };
    
    class MatImage
    {
        public :
            MatImage();
            ~MatImage();

            Vector Size;
            float RelationWidth, RelationHeight;
            void* Image;
            
            void SetBlur(bool h, bool v, bool d);
            
            void operator=(MatImage& Tex);
            bool Load(const char* Str);
            bool Load(unsigned int Width, unsigned int Height, void* Image);
            void Clear(void);
            
    };
    
    class ObjScene;
    class MatTexImage : public MatTexture
    {
        public :
            float RelationWidth, RelationHeight;
            GLuint Tex;
            
            bool Mirror;
            MatTexColor Col;

            MatTexImage(void);
            ~MatTexImage();
            
            void Enable(void);
            void Disable(void);
            void TexCoord(float s, float t);

            void operator=(MatTexImage& Tex);
            bool Build(ObjScene& Scene, ObjTag Tag);
            bool Load(const char* Str);
            bool Load(unsigned int Width, unsigned int Height, void* Image);
            bool Load(MatImage& Image);
            bool Load(unsigned int Width, unsigned int Height);
            void Clear(void);
    };
    
    class MatTexImageVid : public MatTexture
    {
        private :
            CLst Tex;
        
        public :
            unsigned int Frame;
            unsigned int Frames;
        
            void Enable(void);
            void Disable(void);
            void TexCoord(float s, float t);
            
            void Add(MatTexImage* Tex);
            void Remove(unsigned int Frame);
            
            MatTexImageVid(void);
    };
    
/*    class MatTexMulti : public MatTexture
    {
        private :
            MatTexImage Tex[32];
        
        public :
            void Enable(void);
            void Disable(void);
            void TexCoord(float s, float t);
            
            void Add(MatTexImage* Tex);
            void Remove(MatTexImage* Tex);
            
            MatTexImageVid(void);
    };*/
    
/*    class MatTexColorVid : public MatTexture
    {
        private :
            CLst Tex;
        
        public :
            float Frame;
            Color Begin;
            Color End;
        
            void Enable(void);
            void Disable(void);
            void TexCoord(float s, float t);
            
            void Add(float Frame, Color* Tex);
            void AddRGB(float Frame, unsigned char Red, unsigned char Green, unsigned char Blue);
            void Remove(float Frame);
            
            MatTexImageVid(void);
    };*/
    
    typedef struct _GLERROR
    {
        const char* file;
        unsigned int line;
    } GLERROR;
       
    class Objects;
    class ObjSound;
    
    class Material_
    {
        public :
            MatTexture* Tex;
            
            bool Blending;
            GLenum BlendingSource;
            GLenum BlendingDestination;
            void BlendFunc(GLenum sfactor, GLenum dfactor);
            
            Material_(void);
    };
    
    class Object
    {
        private :
            virtual void _draw(GLERROR*, double Advance)=0;
			virtual void _equ(Object& Obj)=0;
			virtual void _clear(void);

            double MoyLastTime(void);
            CLst LastTime;
            int LastTime_;
        
        protected :
            bool enPosition;
            bool enScale;
            bool enRotation;
            
        public :
            Object();
            void Draw(void);
            void Center2d(void);
            bool PushTo(Objects& Obj);
            bool PopTo(Objects& Obj);
            bool BePersistant(unsigned char Type);
            bool BeNotPersistant(unsigned char Type);
            bool Solid;
            
            Material_ Material;
            
            void AddTag(ObjTag);
            void RemoveTag(ObjTag);
            
            void (*MouseClick)(void);
            ObjTag Tag;
            
            void _InitTimers(void);

            Vector Position;
            Vector Scale;
            Vector Rotation;

			void operator=(Object& Obj);
			void Clear(void);
    };

   class Objects : public Object
    {
        private :
            CLst List;
        public :
            bool PushObject(Object& Obj);
            bool PopObject(Object& Obj);
        protected :
            void _draw(GLERROR*, double Advance);
			void _equ(Object& Obj);
    };
    
    class ObjFunction : public Object
    {
        public :
            ObjFunction(void);
            void (*Function)(void* Data, bool OnlyMeshes, double Advance);
            void* Data;
        protected :
            void _draw(GLERROR*, double Advance);
    };
    
    class ObjList : public Object
    {
        private :
            GLuint Id;
            void Verify(void);
        public :
            bool Loaded;
            
            ObjList(void);
            ~ObjList(void);
            void Load(Object*);
            void Build(void);
            void Clear(void);
            
            void Begin(void);
            void End(void);
            
            Object* Obj;
        protected :
            void _draw(GLERROR*, double Advance);
    };
    
    class ObjTerrain : public Object
	{
	    public :
            

	};
	
	class ObjTeapot : public Object
    {
        protected :
            void _draw(GLERROR*, double Advance);
    };
    
    class ObjSphere : public Object
    {
        protected :
            void _draw(GLERROR*, double Advance);
            GLdouble _Radius;
            GLint _Slices;
            GLint _Stacks;
        
        public :
            GLdouble Radius;
            GLint Slices;
            GLint Stacks;
            
            ObjSphere();
    };

#if defined(_POPCORN_3DS_REQUIRED_) || defined(__MAIN__)
    class Obj3DSObject : public Object
    {
        private :
            void LoadNodes(Lib3dsNode* Node);
            void DeleteNodes(Lib3dsNode* Node);
            void ShowNodes(Lib3dsNode* Node);
        
        protected :
            void _draw(GLERROR*, double Advance);
        
        public :
            Lib3dsFile *File;
            Obj3DSObject(void);
            ~Obj3DSObject(void);
            void operator=(Obj3DSObject& Obj);
            bool Load(const char* File);
            void Clear(void);
    };
#endif
/*
    enum MeshesMode { NO_MODE, VBO_MODE, GLLIST_MODE, ARRAY_MODE };
    typedef struct ObjectPointerStruct
    {
        enum MeshesMode Mode;
        union
        {
            GLuint VBOId;       // VBO Name
            GLuint GLListID;    // GLList name
            void* Buffer;       // Buffer
        };
        GLint Stride;
        GLint Size;
    } ObjectPointer;
    class Obj3dMeshes : Object
    {
        private :
            ObjectPointer Vertex;
            ObjectPointer Col_;
            ObjectPointer Normal;
            ObjectPointer TexCoord;
            ObjectPointer EdgeFlag;
            void SetBuffer(ObjectPointer* Type, unsigned char Byte, GLint Size, GLsizei Stride, GLvoid* Pointer);
            void BuildVBOBuffer(ObjectPointer* Type);
            void DeleteBuffer(ObjectPointer* Type);
            
        
        protected :
            void _draw(GLERROR*, double Advance);
        
        public :
            void SetVertexPointer(GLint Size, GLsizei Stride, GLvoid* Pointer);
            void SetColorPointer(GLint Size, GLsizei Stride, GLvoid* Pointer);
            void SetNormalPointer(GLint Size, GLsizei Stride, GLvoid* Pointer);
            void SetTexCoordPointer(GLint Size, GLsizei Stride, GLvoid* Pointer);
            void SetEdgeFlagPointer(GLint Size, GLsizei Stride, GLvoid* Pointer);
            
            void Build(void);
            
            enum MeshesMode SavingMode;
            
            Obj3dMeshes(void);
    };
*/

    struct Particle
    {
        float Life;
        float Fade;
        
        float Color[3];
        float Position[3];
        float Vector[3];
        
        void* Tag;
    };
    
    class ObjParticles;
    typedef struct ObjParticlesMethodStruct
    {
        void (*Initialization)(ObjParticles&);
        void (*InitParticle)(ObjParticles&,  Particle&);
        void (*ShowerParticle)(ObjParticles&, Particle&);
        void (*ConfigParticle)(ObjParticles&, Particle&, double Advance);
    } ObjParticlesMethod;
    
    class ObjParticles : public Object
    {
        protected :
            void _draw(GLERROR*, double Advance);
        
        private :
            Particle* Particles;
            unsigned int NbParticles;
            void InitializeParticle(struct Particle* i);
            void ShowerParticle(struct Particle* i);
            void ConfigParticle(struct Particle* i, double Advance);
            ObjParticlesMethod* Method;
        
        public :
            bool Enabled;
            float Life;
            float Explosion;
            Vector Size;
            Vector ForceVector;
            Vector Gravity;
            MatTexColor Col;
            
            ObjParticles(void);
            ~ObjParticles();
            
            bool Load(unsigned int NbParticles);
            void Clear(void);
            bool LoadPlugIn(ObjParticlesMethod* Method);
            void Build(void);
    };
    
    // Définition de Plug-In pour les particules
    void _InitParticlesPlugins(void);
    extern ObjParticlesMethod Flam;
    
    
    class ObjPlane;
    class MatFont
    {
	    private :
            bool YInversed;

	    public :
            MatTexImage* Chars[256];            // En tout, 1 024 octets par défaut
            bool Load(const char* File);
            void Clear(void);
            
            unsigned short HeightText(const char* Str, ...);
            unsigned short WidthText(const char* Str, ...);
            void PrintChar(MatTexColor* Col, float x, float y, char c);
            
            MatFont(void);
            ~MatFont();
    };

    class ObjPlane : public Object
    {
        protected :
            void _draw(GLERROR*, double Advance);
    }; 
    
    class Obj2dConsole : public Object
    {
        protected :
            void _draw(GLERROR*, double Advance);
        
        public :
            MatFont* Font;
            MatTexColor Col;
            unsigned int Lines;
            
            unsigned int Width, Height;
            char Str[5192];
            
            Obj2dConsole(void);
            
            void Print(char* Str, ...);
            void Clear(void);
    };
    
    class Obj2dLabel : public Object
    {
        private :
            char Label[512];

        protected :
            void _draw(GLERROR*, double Advance);
        
        public :
            MatFont* Font;
            MatTexColor Col;
            unsigned int Width, Height;
            void SetLabel(char *Label, ...);
            
            Obj2dLabel(void);
    };
    
 
	/*** Timer ***/
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
	
    class TimerEvent : public Object
    {
        private :
            Timer timer;
        public :
            unsigned int Delay;
            void (*TimerFunc)(void);
            void Enable(void);
            void Disable(void);
            TimerEvent(void);
        protected :
            void _draw(GLERROR*, double Advance);
    };
    
    
	class _TimerFPS : Timer
	{
		private :
			unsigned int compteur;
		public :
		    _TimerFPS(void);
			unsigned int FPS;
			unsigned int FPSmin;
			unsigned int FPSmax;
			unsigned int QueryFPS;
			void Start(void);
			void Refresh(void);
	};


    /*** Audio ***/
/*
    class ObjMusic {
        private :
            union {
                FSOUND_STREAM* Stream;
                FMUSIC_MODULE* Music;
            } Sound;
            int Channel;
            bool IsMusicFormat;
            bool Loaded;
            unsigned char Volume;
            bool Loop;
        public :
            ObjMusic(void);
            ~ObjMusic();
            bool LoadStream(const char* Music);
            bool LoadMusic(const char* Music);
            void Clear(void);
            void Play(void);
            void Stop(void);
            void Pause(void);
            void Resume(void);
            void SetLooping(bool Enabled);
            void SetVolume(unsigned char Volume);
    };
*/    
    class ObjSound {
	    public :
            unsigned int Source;
			unsigned int SampleSet;
			unsigned int* _ObjLoaded;

			Vector* pPosition;
			Vector   Position;
			Vector   Velocity;
            
            ObjSound(void);
            ~ObjSound();
            bool LoadWAV(const char* Sound, bool Looping);
            void Clear(void);
            void Play(void);
            void Stop(void);
            void SetVolume(unsigned char Volume);
            void SetLooping(bool Enabled);
			void Set3dSound(bool Enabled);
            void Refresh(void);
            ObjSound& operator=(ObjSound&);
    };

	
	/*** Scène 3d ***/
	class _Camera
	{
	    public:
	        Vector Position;
	        Vector LookAt;
	        Vector VectorUp;
	        
	        _Camera(void);
            void Define(void);
	};
	
	typedef struct LightStruct
	{
	    bool Enabled;
     
        MatTexColor Ambient;
        MatTexColor Diffuse;
        MatTexColor Specular;
        Vector Position;

        float SpotExponent;
        float SpotCutoff;
        Vector SpotDirection;
	        
        float ConstantAttenuation;
        float LinearAttenuation;
        float QuadraticAttenuation;
	} _Light;
	
    extern class ObjScene
	{
        public :
            Vector Position;
			Vector Size;

            ObjPlane FrontFilter;
	        MatTexColor ColBackGround;    // Background noir
	        MatTexColor ColFrontGround;
	        Objects List3dObjects;
	        Objects List2dObjects;
	        Objects ListBackObjects;
	        Objects ListObjects;
			CLst ListSound;
	        _Camera Camera;
	        ObjTag Tag;
	        
            bool Lighting;
            _Light Light[8];
	        
	        bool FrontGround;
			bool Enable3D;
			bool BackGround;
			
	        void (*Keyboard)(bool KeyDown, SDLKey Key);
            void (*MouseMotion)(unsigned short x, unsigned short y, bool LeftButton, bool MiddleButton, bool RightButton);
            void (*MouseClick)(bool ButtonDown, unsigned short x, unsigned short y, bool LeftButton, bool MiddleButton, bool RightButton);
            void (*Initialization)(void);
            void (*Exit)(void);
            void (*Idle)(void);
            
            void Draw(void);
            void AddTag(ObjTag);
            void RemoveTag(ObjTag);
	        
	        ObjScene(void);
	} *Scene;

	/*** Configuration ***/
    extern class _VideoConfig
	{
		private :
			unsigned int flags;
			MatFont* ConsoleFont;
            Obj2dConsole* ConsoleWriter;
            
		public :
			unsigned short Width;
			unsigned short Height;
			
			void SetCaption(char* Caption);
			
			bool FullScreen;
		    _VideoConfig(void);
			void SetVideoResolution(unsigned short width, unsigned short height, bool fullscreen);
			bool Init(void);
			void Flush(void);
//			void InstallConsole(void);
			
			void ShowMouse(bool Activate);
			_TimerFPS FPS;
			
			unsigned char MovementLifting;
			
			bool MultiTexturing;
			bool VertexBuffer;
			bool EXTBlend;
			
			~_VideoConfig();
	} Video;
	
	
/*	extern class _AudioConfig
	{
        private :
            int mixrate;
            int channels;
            bool InitSound(int mixrate, int maxsoftwarechannels);

            
        public :
            _AudioConfig(void);
            void SetAudioResolution(int mixrate, int channels);
            bool Init(void);
            void Flush(void);
            
            ~_AudioConfig();
    } Audio;
*/  
	extern class _AudioConfig
	{
        private :
            ALCcontext *Context;
            ALCdevice *Device;
            bool InitSound(int mixrate, int maxsoftwarechannels);

            
        public :
            _AudioConfig(void);
            bool Init(void);
            void Flush(void);

			ALfloat DopplerFactor;
			ALfloat DopplerVelocity;
            
            ~_AudioConfig();
    } Audio;
    
    extern class _EventsConfig
    {
        public :
            _EventsConfig(void);
            bool Init(void);
	        void Flush(void);
    } Events;
	
	
    bool Go(void);
    
	void Stop(void);
	
	#ifdef __DEBUG__
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
	
	struct VariableStruct
	{
	    char Var[128];
	    char* Value;
	};
    class ObjConfigFile
    {
        private :
            CLst Lst;
        
        public :
            ~ObjConfigFile();
            
            bool Load(const char* FileName);
            void Clear(void);
            char* Value(char* Var, ...);
            char* operator[](char* Var);
    };
    
    class ObjLinesFile
    {
        private :
            CLst Lst;
        
        public :
            ~ObjLinesFile();
            
            unsigned int Lines;
            bool Load(const char* FileName);
            void Clear(void);
            char* operator[](unsigned short Index);
    };
	
	double Sin(float Angle);
	double Cos(float Angle);
	double Tan(float Angle);
	
#ifdef __DEBUG__
#   define DebugPointer(s)  PopCorn::_dbgPointer(__FILE__, __LINE__, s)
#   define DebugPointerFile(f,s) PopCorn::_dbgPointerFile(f, __FILE__, __LINE__, s)
    void _dbgPointer(const char* file, int line, const char* comment);
    void _dbgPointerFile(unsigned int f, const char* file, int line, const char* comment);
    void _dbgMsgBox(const char* msg, ...);
#endif
	void _PopCornError(const char* Str);
};

#define _PopCornNotEnoughMemory()    _PopCornError("Not enought memory");


#endif
