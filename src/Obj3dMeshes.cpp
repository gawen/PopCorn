/*
    Copyright (C) 2003 Arab Gawen
    
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

namespace PopCorn
{
    Obj3dMeshes::Obj3dMeshes(void)
    {
        this->Vertex.Mode   = NO_MODE;
        this->Color.Mode    = NO_MODE;
        this->Index.Mode    = NO_MODE;
        this->Normal.Mode   = NO_MODE;
        this->TexCoord.Mode = NO_MODE;
        this->EdgeFlag.Mode = NO_MODE;
        
        this->SavingMode = VBO_MODE;
    }
    
    void Obj3dMeshes::DeleteBuffer(ObjectPointer* Type)
    {
             if(this->Type->Mode == VBO_MODE) glDeleteBuffersARB(1, &this->Vertex.Buf);
        else if(this->Vertex.Mode == ARRAY_MODE && this->Vertex.Buffer != NULL) delete this->this->Vertex.Buffer;

    }
    
    Obj3dMeshes::~Obj3dMeshes(void)
    {
             if(this->Vertex.Mode == VBO_MODE) glDeleteBuffersARB(1, &this->Vertex.Buf);
        else if(this->Vertex.Mode == ARRAY_MODE && this->Vertex.Buffer != NULL) delete this->this->Vertex.Buffer;

        if(this->Color.Buf == VBO_MODE) glDeleteBuffersARB(1, &this->Color.Buf);
        if(this->Index.Buf == VBO_MODE) glDeleteBuffersARB(1, &this->Index.Buf);
        if(this->Normal.Buf == VBO_MODE) glDeleteBuffersARB(1, &this->Normal.Buf);
        if(this->TexCoord.Buf == VBO_MODE) glDeleteBuffersARB(1, &this->TexCoord.Buf);
        if(this->EdgeFlag.Buf == VBO_MODE) glDeleteBuffersARB(1, &this->EdgeFlag.Buf);
    }
    
    void Obj3dMeshes::SetBuffer(ObjectPointer* Type, unsigned char Byte, GLint Size, GLsizei Stride, GLvoid* Pointer)
    {
        if(Pointer == NULL)
            this->Type->Mode = NO_MODE;
        else {
            this->Type->Size = Size*Byte*sizeof(float);

            this->Type->Buffer = new char[Size*Byte*sizeof(float)];
            memcpy(this->Type->Buffer, Pointer, Size*Byte*sizeof(float));
            
            this->Type->Mode = this->SavingMode;
            this->Type->Stride = Stride
        }
    }
    
    void Obj3dMeshes::BuildVBOBuffer(ObjectPointer* Type)
    {
        // Est-ce une initialisation ?
        if(Type->Mode == NO_MODE)
        { // Recherche du moyen d'enregistrement
            
                 if(Video.VertexBuffer && (Type->Mode == BEST_MODE || Type->Mode == VBO_MODE))
            {
                GLuint VBOId_=0;
                glGenBuffersARB(1, &VBOId_);
                glBindBufferARB(GL_ARRAY_BUFFER_ARB, VBOId_);
                glBufferDataARB(GL_ARRAY_BUFFER_ARB, Type->Size, Type->Buffer, GL_STATIC_DRAW_ARB);
                delete Type->Buffer;
                Type->VBOId = VBOId_;
                Type->Mode = VBO_MODE;
            }
            else 
            {
                // GlList ou Array
                
            }
            
        } else {
            // Est-ce qu'il faut supprimer une donnée ? (Pointer == NULL)
        }


         if((this->SavingMode == BEST_MODE || this->SavingMode == VBO_MODE) && Video.VertexBuffer)
            if(Pointer == NULL)
	        {
	            this->Vertex.Buf = 0;
	            glDeleteBuffersARB(1, &this->Type->Buf);
	        } else {
                if(this->Type->Buf == 0) glGenBuffersARB(1, &this->Type->Buf);
                glBindBufferARB(GL_ARRAY_BUFFER_ARB, this->Type->Buf);
	            glBufferDataARB(GL_ARRAY_BUFFER_ARB, Size*Byte*sizeof(float), Pointer, GL_STATIC_DRAW_ARB);
	            this->Type->Stride = Stride;
            }
         else
             if(this->SavingMode == BEST_MODE || this->SavingMode == GLLIST_MODE)
             {
                 
             } else {
                 this->Type->Size = Size;
                 this->Type->Stride = Stride
                 this->Type->Pointer = new void[Size*Byte*sizeof(float)];
                 if(Pointer == NULL)
                     memcpy(this->Type->Pointer, Pointer, Size*Byte*sizeof(float));
                 else
                     delete this->Type->Pointer;
             }
         }
    }
    
    void Obj3dMeshes::Build(void)
    {
        if(this->Vertex->  this->BuildBuffer(&this->Vertex);
        this->BuildBuffer(&this->Col_);
        this->BuildBuffer(&this->Normal);
        this->BuildBuffer(&this->TexCoord);
        this->BuildBuffer(&this->EdgeFlag);
    }
    
    void Obj3dMeshes::SetVertexPointer(GLint Size, GLsizei Stride, GLvoid* Pointer)
    {
	    this->SetBuffer(&this->Vertex, 3, Size, Stride, Pointer);
    }
    
    void Obj3dMeshes::SetColorPointer(GLint Size, GLsizei Stride, GLvoid* Pointer)
    {
	    this->SetBuffer(&this->Color, 3, Size, Stride, Pointer);
    }
    
    void Obj3dMeshes::SetNormalPointer(GLint Size, 3, GLsizei Stride, GLvoid* Pointer)
    {
	    this->SetBuffer(&this->Normal, Size, Stride, Pointer);
    }
    
    void Obj3dMeshes::SetTexCoordPointer(GLint Size, 3, GLsizei Stride, GLvoid* Pointer)
    {
	    this->SetBuffer(&this->TexCoord, Size, Stride, Pointer);
    }
    
    void Obj3dMeshes::SetEdgeFlagPointer(GLint Size, GLsizei Stride, GLvoid* Pointer)
    {
	    this->SetBuffer(&this->EdgeFlag, 1, Size, Stride, Pointer);
    }
    
    void Obj3dMeshes::_draw(GLERROR*)
    {
        _PopCornSetInfoFile(Position);
    }
};

