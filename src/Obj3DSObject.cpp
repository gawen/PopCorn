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


#define _POPCORN_3DS_REQUIRED_
#include "popcorn.hpp"

namespace PopCorn
{
    Obj3DSObject::Obj3DSObject(void)
    {
        this->File = NULL;
    }
    
    Obj3DSObject::~Obj3DSObject(void)
    {
        this->Clear();
    }
    
    void Obj3DSObject::Clear(void)
    {
        if(this->File)
            for(Lib3dsNode* i=this->File->nodes; i!=NULL; i=i->next)
                this->DeleteNodes(i);
        this->File = NULL;
    }

    bool Obj3DSObject::Load(const char* FileN)
    {
        Lib3dsNode* i;
        
#       ifdef __DEBUG__
        fprintf(Debug.Fp, "Loading of the 3ds Object %s ", FileN);
#       endif
        
        if(!(this->File = lib3ds_file_load(FileN)))
        {
#           ifdef __DEBUG__
            fprintf(Debug.Fp, "[ERROR] : loading 3ds file failed\n", File);
#           endif
            return false;
        }
        
        for(i=this->File->nodes; i!=NULL; i=i->next)
            this->LoadNodes(i);
    }
    
    void Obj3DSObject::LoadNodes(Lib3dsNode* Node)
    {
        // Parcoure les enfants
        for (Lib3dsNode* i=Node->childs; i!=NULL; i=i->next)
            this->LoadNodes(i);
        
        if(Node->user.d == 0)
        { // Pas de liste OpenGL chargé
            Lib3dsMesh *mesh=lib3ds_file_mesh_by_name(this->File, Node->name);
            if(!mesh) return;
            
            Node->user.d=glGenLists(1);
            glNewList(Node->user.d, GL_COMPILE);

            {
                unsigned p;
                Lib3dsVector *normalL=(Lib3dsVector*)malloc(3*sizeof(Lib3dsVector)*mesh->faces);

                {
                    Lib3dsMatrix Matrix;
                    lib3ds_matrix_copy(Matrix, mesh->matrix);
                    lib3ds_matrix_inv(Matrix);
                    glMultMatrixf(&Matrix[0][0]);
                }
           
                for (p=0; p<mesh->faces; ++p)
                {
                    Lib3dsFace *f=&mesh->faceL[p];
                    Lib3dsMaterial *mat=0;
                    if (f->material[0])
                    {
                        mat=lib3ds_file_material_by_name(this->File, f->material);
                    }

                    if (mat) {
                        static GLfloat a[4]={0,0,0,1};
                        float s;

                        printf("Name : %s\n", mat->texture1_map.name);
                      
                        glMaterialfv(GL_FRONT, GL_AMBIENT, a);
                        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat->diffuse);
                        glMaterialfv(GL_FRONT, GL_SPECULAR, mat->specular);
                        s = pow(2, 10.0*mat->shininess);
                        if (s>128.0) {
                            s=128.0;
                        }
                        glMaterialf(GL_FRONT, GL_SHININESS, s);
                    } else {
                        Lib3dsRgba a={0.2, 0.2, 0.2, 1.0};
                        Lib3dsRgba d={0.8, 0.8, 0.8, 1.0};
                        Lib3dsRgba s={0.0, 0.0, 0.0, 1.0};
                        glMaterialfv(GL_FRONT, GL_AMBIENT, a);
                        glMaterialfv(GL_FRONT, GL_DIFFUSE, d);
                        glMaterialfv(GL_FRONT, GL_SPECULAR, s);
                    }

                    {
                        int i;
                        glEnable(GL_NORMALIZE);
                        glBegin(GL_TRIANGLES);
                            glNormal3fv(f->normal);
                            for (i=0; i<3; ++i) {
                                if(mesh->texels)
                                    glTexCoord2f(mesh->texelL[f->points[i]][0], 1.0f-mesh->texelL[f->points[i]][1]);

//                                glNormal3fv(normalL[3*p+i]);
                                glVertex3fv(mesh->pointL[f->points[i]].pos);
                            }
                        glEnd();
                        glDisable(GL_NORMALIZE);
                    }
                }

                free(normalL);
            }

            glEndList();
        }
    }
    
    void Obj3DSObject::DeleteNodes(Lib3dsNode* Node)
    {
        for(Lib3dsNode* i=this->File->nodes; i!=NULL; i=i->next)
            this->DeleteNodes(i);
        
        glDeleteLists(Node->user.d, 1);
    }
    
    void Obj3DSObject::operator=(Obj3DSObject& Obj)
    {
        this->File = Obj.File;
    }
    
    void Obj3DSObject::ShowNodes(Lib3dsNode* Node)
    {
        for(Lib3dsNode* i=Node->childs; i!=NULL; i=i->next)
            this->ShowNodes(i);
        
        if(Node->user.d != 0)
        {
            Lib3dsObjectData *d;
            
            glPushMatrix();
            d=&Node->data.object;
            glMultMatrixf(&Node->matrix[0][0]);
            glTranslatef(-d->pivot[0], -d->pivot[1], -d->pivot[2]);
            glCallList(Node->user.d);
            glPopMatrix();
        }
    }
    
    void Obj3DSObject::_draw(GLERROR* Position, double Advance)
    {        
        for(Lib3dsNode* i=this->File->nodes; i!=NULL; i=i->next)
            this->ShowNodes(i);
        
        _PopCornSetInfoFile(Position);
    }

};

