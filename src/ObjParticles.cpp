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

#include "popcorn.hpp"

extern PopCorn::Objects Vaisseau;

namespace PopCorn
{
    void _InitParticlesPlugins(void)
    {
        // La flame
    }
    
    ObjParticles::ObjParticles(void)
    {
        this->Particles = NULL;
        this->LoadPlugIn(NULL);
        this->Enabled = true;
        this->Size.Set(1.0f, 1.0f);
        this->Life = -50.0f;
        this->Gravity.Set(0.0f, -1.0f, 0.0f);
        this->Explosion = 1.0f;
        this->enPosition = false;
        this->Material.BlendFunc(GL_SRC_ALPHA, GL_ONE);
    }
    
    ObjParticles::~ObjParticles()
    {
        if(this->Particles != NULL)
            delete this->Particles;
    }
    
    void ObjParticles::Clear(void)
    {
        if(this->Particles != NULL)
            delete this->Particles;
    }
    
    bool ObjParticles::Load(unsigned int NbParticles)
    {
        unsigned int i;
        
        if(this->Particles != NULL)
        {
#           ifdef __DEBUG__
            fprintf(Debug.Fp, "deleted the already loaded particles, and ");
#           endif
            delete this->Particles;
        }
        
#       ifdef __DEBUG__
        fprintf(Debug.Fp, "%u particles loading ", NbParticles);
#       endif

        this->Particles = new Particle[NbParticles];
        
        if(this->Particles == NULL)
        {
#           ifdef __DEBUG__
            fprintf(Debug.Fp, "not done, can't allocate enough memory for particles ...\n");
#           endif
            return false;
        }
        this->NbParticles = NbParticles;
        
        for(i=0; i<NbParticles; i++)
        {
            this->Particles[i].Life = 0.0f;
        }
        
#       ifdef __DEBUG__
        fprintf(Debug.Fp, "done.\n");
#       endif

        return true;
    }
    
    void ObjParticles::_draw(GLERROR* Position, double Advance)
    {
        unsigned int i;
        float matrice[16];
        struct { float x, y, z; } vHaut, vDroite, *vHaut_, *vDroite_;
        struct Particle* Particle;
        
        _PopCornSetInfoFile(Position);
        
        if(this->NbParticles == 0) return;
        
        // BillBoarding 
        glPushMatrix();
        glGetFloatv(GL_MODELVIEW_MATRIX, (float*)matrice);
       	vDroite.x = matrice[0];
        vDroite.y = matrice[4];
        vDroite.z = matrice[8];
        vHaut.x = matrice[1];
        vHaut.y = matrice[5];
        vHaut.z = matrice[9];
    
        vDroite_ = &vDroite;
        vDroite_->x *= this->Size.X / 2.0f;
        vDroite_->y *= this->Size.X / 2.0f;
        vDroite_->z *= this->Size.X / 2.0f;
        vHaut_ = &vHaut;
        vHaut_->x *= this->Size.Y / 2.0f;
        vHaut_->y *= this->Size.Y / 2.0f;
        vHaut_->z *= this->Size.Y / 2.0f;
        
        glDepthMask(GL_FALSE);
        glEnable(GL_BLEND);
        glEnable(GL_NORMALIZE);
        this->Material.Tex->Enable();
        
        for(i=0; i<this->NbParticles; i++)
        {
            Particle = &this->Particles[i];
            if(Particle->Life <= 0.0f && Enabled == true)
                if(this->Method != NULL && this->Method->InitParticle != NULL)
                    this->Method->InitParticle(*this, *Particle);
                else
                    this->InitializeParticle(Particle);
            else if(Particle->Life > 0.0f)
            {
                if(this->Method != NULL && this->Method->ShowerParticle != NULL)
                    this->Method->ShowerParticle(*this, *Particle);
                else
                    this->ShowerParticle(Particle);
                
                 glPushMatrix();
                 glTranslatef(Particle->Position[0], Particle->Position[1], Particle->Position[2]);
//                 Size.Scale();

                 glBegin(GL_TRIANGLE_STRIP);
                     this->Material.Tex->TexCoord(1.0f, 1.0f);
                     glVertex3f( + vHaut_->x + vDroite_->x,
                                 + vHaut_->y + vDroite_->y,
                                 + vHaut_->z + vDroite_->z
                                );
                     this->Material.Tex->TexCoord(0.0f, 1.0f);
                     glVertex3f( - vHaut_->x + vDroite_->x,
                                 - vHaut_->y + vDroite_->y,
                                 - vHaut_->z + vDroite_->z
                     );
                     this->Material.Tex->TexCoord(1.0f, 0.0f);
                     glVertex3f( + vHaut_->x - vDroite_->x,
                                 + vHaut_->y - vDroite_->y,
                                 + vHaut_->z - vDroite_->z
                     );
                     this->Material.Tex->TexCoord(0.0f, 0.0f);
                     glVertex3f( - vHaut_->x - vDroite_->x,
                                 - vHaut_->y - vDroite_->y,
                                 - vHaut_->z - vDroite_->z
                     );

                 glEnd();
                 glPopMatrix();
                 
                if(this->Method != NULL && this->Method->ConfigParticle != NULL)
                    this->Method->ConfigParticle(*this, *Particle, Advance);
                else
                    this->ConfigParticle(Particle, Advance);
            }
        }
        glPopMatrix();
        
        this->Material.Tex->Disable();

        glDisable(GL_NORMALIZE);
        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);
    }
    
    void ObjParticles::Build(void)
    {
        unsigned int i;
        for(i=0; i<this->NbParticles; i++)
            this->InitializeParticle(&this->Particles[i]);
    }
    
    void ObjParticles::InitializeParticle(struct Particle* i)
    {
        i->Life = 1.0f;
        i->Fade = (this->Life < 0) ? 1.0f/(-this->Life * (float(rand()%100)/100.0f) ) : 1.0f/this->Life;
                    
        i->Color[0] = this->Col.Red / 255.0f;
        i->Color[1] = this->Col.Green / 255.0f;
        i->Color[2] = this->Col.Blue / 255.0f;
                    
        i->Position[0] = this->Position.X;
        i->Position[1] = this->Position.Y;
        i->Position[2] = this->Position.Z;
                    
        i->Vector[0] = this->ForceVector.X + ((float(rand()%3600)/3600.0f) *this->Explosion)-(this->Explosion/2.0f);
        i->Vector[1] = this->ForceVector.Y + ((float(rand()%3600)/3600.0f) *this->Explosion)-(this->Explosion/2.0f);
        i->Vector[2] = this->ForceVector.Z + ((float(rand()%3600)/3600.0f) *this->Explosion)-(this->Explosion/2.0f);
    }
    
    void ObjParticles::ShowerParticle(struct Particle* i)
    {
        glColor4f(i->Color[0],
            i->Color[1],
            i->Color[2],
            i->Life
        );
    }
    
    void ObjParticles::ConfigParticle(struct Particle* i, double Advance)
    {
        i->Life -= i->Fade;
                 
        i->Position[0] += i->Vector[0]*Advance;
        i->Position[1] += i->Vector[1]*Advance;
        i->Position[2] += i->Vector[2]*Advance;
                 
        i->Vector[0] += this->Gravity.X*Advance;
        i->Vector[1] += this->Gravity.Y*Advance;
        i->Vector[2] += this->Gravity.Z*Advance;
    }
    
    bool ObjParticles::LoadPlugIn(ObjParticlesMethod* Method)
    {
        if(Method == NULL)
            this->Method = NULL;
        else {
            this->Method = Method;
            if(Method->Initialization != NULL) Method->Initialization(*this);
        }
    }
};

