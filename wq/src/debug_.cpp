#include "waterquest.hpp"

PopCorn::ObjScene Scene;

PopCorn::ObjParticles Particules;

PopCorn::Obj2dConsole Console;
PopCorn::ObjFunction  Idle;

PopCorn::MatTexImage Tex;
#define size    20
PopCorn::ObjPlane Plane[size][size];

PopCorn::ObjSphere Sphere;

//PopCorn::ObjSound Sound;

bool done=true;
void FIdle(void* Data, bool OnlyMeshes, double Advance)
{
    static double i=0.0f, j=0.0f;
    int x, y;
    
    j+=Advance*25.0f;
    Scene.Camera.Position.Set(PopCorn::Cos(1.5f*j)*size+size/2.0f, PopCorn::Sin(1.8f*j)*size+size/2.0f, PopCorn::Cos(j)*10.0f+15.0f);
    Scene.Light[0].Position = Scene.Camera.Position;
    
    Console.Clear();
    Console.Print("FPS : %u\nAdvance : %f\ni = %f\n", PopCorn::Video.FPS.FPS, Advance, i);
    
    i+=Advance;
    if(i>=0.1f)
    {
        Particules.Col.Set(rand()%4*32+128, rand()%4*32+128, rand()%4*32+128);
        for(y=0; y<size; y++)
            for(x=0; x<size; x++)
            {
				if(rand()%20 == 0)
                {
                     ((PopCorn::MatTexImage*)Plane[x][y].Material.Tex)->Col.Set(255, 198, 128, 254);
                     Plane[x][y].AddTag(CRN_TAG_HALATION);
                     if(done) Plane[x][y].AddTag(CRN_TAG_VISIBLE);
                     if(rand()%20 == 0) Particules.Position.Set(x, y, 0.0f);
                } else {
                    ((PopCorn::MatTexImage*)Plane[x][y].Material.Tex)->Col.Set(255, 255, 255, 254);
                    Plane[x][y].RemoveTag(CRN_TAG_HALATION);
                }
            }
        i=0.0f;
    }
    
    //Sphere.Position = Scene.Light[0].Position;
}

void _Init(void)
{    
    Scene.Camera.Position.Set(size/2.0f, size/2.0f, 10.0f);
    Scene.Camera.LookAt.Set  (size/2.0f, size/2.0f, 0.0f);
    Scene.ColBackGround.Set(0, 0, 0);
    
    Console.Font = new PopCorn::MatFont;
    Console.Font->Load("../media/font/verdana10g.PCF");
    Console.BePersistant(CRN_FRONTOBJS);
    
    Idle.Function = FIdle;
    Idle.BePersistant(CRN_ALWOBJS);
    
    {
        unsigned int i, j;
        Tex.Load("../media/img/Circle.bmp");
        for(i=0; i<size; i++)
            for(j=0; j<size; j++)
            {
                Plane[i][j].Material.Tex = new PopCorn::MatTexImage;
                *(PopCorn::MatTexImage*)Plane[i][j].Material.Tex = Tex;
                Plane[i][j].Position.Set(float(i), float(j), 0.0f);
                Plane[i][j].Material.BlendFunc(GL_SRC_ALPHA, GL_ONE);
                Plane[i][j].BePersistant(CRN_3DOBJS);
                Plane[i][j].RemoveTag(CRN_TAG_VISIBLE);
            }
    }
    
    Scene.Lighting = true;
    Scene.Light[0].Position.Set(size/2.0f, size/2.0f, 5.0f);
	Scene.Light[0].Enabled = true;
    Sphere.BePersistant(CRN_3DOBJS);
    
    Particules.BePersistant(CRN_3DOBJS);
//    Particules.Material.Tex = new PopCorn::MatTexImage;
//    ((PopCorn::MatTexImage*)Particules.Material.Tex)->Load("../media/img/particle.bmp");
	Particules.Material.Tex = &Tex;
    Particules.Col.Set(255, 198, 128);
    Particules.Explosion = 1.0f;
    Particules.Load(512);
    Particules.ForceVector.Set(0.0f, 0.0f, 5.0f);
    Particules.Gravity.Set(0.0f, 0.0f, -2.0f);
    Particules.Life = -300.0f;
    Particules.AddTag(CRN_TAG_HALATION);
//    Particules.Build();

//	Sound.LoadWAV("../media/sound/police.wav", true);
//	Sound.Play();
//	Sound.pPosition = &Particules.Position;
}

void Keyboard(bool KeyDown, SDLKey Key)
{
    if(KeyDown && Key == SDLK_ESCAPE) exit(0);
    if(KeyDown && Key == SDLK_l) Scene.Lighting = !Scene.Lighting;
    if(KeyDown && Key == SDLK_q) PopCorn::Audio.DopplerFactor += 0.1f;
    if(KeyDown && Key == SDLK_a) PopCorn::Audio.DopplerFactor -= 0.1f;
}

void Debug__(void)
{
    Scene.Keyboard = Keyboard;
    Scene.Initialization = _Init;
    Scene.FrontGround = true;
    
    PopCorn::Scene = &Scene;
    
    InitializeVideo();
    PopCorn::Go();
}
