#include "waterquest.hpp"

SplashObjsClass* SplashObjs;

float ShadingStep;
float AlphaTmp;

void ShadingDown(void* Data, bool OnlyMeshes, double Advance)
{
    AlphaTmp += ShadingStep*Advance;
    if(AlphaTmp >= 255.0f)
    {
        AlphaTmp = 255.0f;
        SplashObjs->Func.RemoveTag(CRN_TAG_VISIBLE);
        QuitSplashes();
    }
    SplashObjs->ScSplash.ColFrontGround.Alpha = int(AlphaTmp);
}

void WaitingFinished(void)
{
    SplashObjs->Timer.Disable();
    SplashObjs->Func.Function = ShadingDown;
    SplashObjs->Func.AddTag(CRN_TAG_VISIBLE);
}

void ShadingUp(void* Data, bool OnlyMeshes, double Advance)
{
    AlphaTmp -= ShadingStep*Advance;
    if(0.0f >= AlphaTmp)
    {
        AlphaTmp = 0.0f;
        SplashObjs->Func.RemoveTag(CRN_TAG_VISIBLE);
        SplashObjs->Timer.Enable();
    }
    
    SplashObjs->ScSplash.ColFrontGround.Alpha = int(AlphaTmp);
}

void SplashInit(void)
{
    PopCorn::Video.ShowMouse(false);                        // Désactive la souris
    SplashObjs->ScSplash.ColBackGround.Set(0, 0, 0);

    SplashObjs->ScSplash.ColFrontGround = SplashObjs->ScSplash.ColBackGround;
    SplashObjs->ScSplash.ColFrontGround.Alpha = 255;
    AlphaTmp=float(SplashObjs->ScSplash.ColFrontGround.Alpha);
    
    // Le splash WQ
    SplashObjs->WQSpl.Load("data/splash/wq.png");
    SplashObjs->Image.Material.Tex = &SplashObjs->WQSpl;
    SplashObjs->Image.Scale = SplashObjs->WQSpl.Size;
    SplashObjs->Image.Position.Set((PopCorn::Video.Width/2)-(SplashObjs->WQSpl.Size.X/2),
                                   (PopCorn::Video.Height/2)-(SplashObjs->WQSpl.Size.Y/2)
    );
    SplashObjs->Image.RemoveTag(CRN_TAG_VISIBLE);
    SplashObjs->Image.BePersistant(CRN_FRONTOBJS);
    
    // Effet de shading du WQ, réglé à 2 sec
    ShadingStep = 255.0f/4.0f; SplashObjs->Func.Function = ShadingUp;
    SplashObjs->Func.RemoveTag(CRN_TAG_VISIBLE);
    SplashObjs->Func.BePersistant(CRN_FRONTOBJS);
    
    // Temps d'attente
    SplashObjs->Timer.Delay = 5000;
    SplashObjs->Timer.TimerFunc = WaitingFinished;
    
    // Son
    SplashObjs->Scape.LoadWAV("data/splash/scape.mp3", false);
    SplashObjs->Scape.Play();
    
    // Démarrage des processus
    SplashObjs->Image.AddTag(CRN_TAG_VISIBLE);
    SplashObjs->Func.AddTag(CRN_TAG_VISIBLE);
}

void SplashKeyboard(bool KeyDown, SDLKey Key)
{
    if(KeyDown && Key == SDLK_ESCAPE) exit(0);
}


void CallSplashes(void)
{
    SplashObjs = new SplashObjsClass;
    
    SplashObjs->ScSplash.FrontGround = true;
    SplashObjs->ScSplash.Enable3D = false;
    SplashObjs->ScSplash.Initialization = SplashInit;
    SplashObjs->ScSplash.Keyboard = SplashKeyboard;
    
    PopCorn::Scene = &SplashObjs->ScSplash;
    
    InitializeVideo();
    
    PopCorn::Go();
}

void QuitSplashes(void)
{
    delete SplashObjs;
    CallMenu();           // Passe au menu
}
