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

namespace PopCorn
{
    ObjLinesFile::~ObjLinesFile()
    {
        this->Clear();
    }
    
    void ObjLinesFile::Clear(void)
    {
        for(LISTE* i=this->Lst.m_pDebut; i != NULL; i=i->pSuivant)
            delete (char*)i->val;
    }
            
    bool ObjLinesFile::Load(const char* FileName)
    {
        FILE* File;
        char* Var;
        char c;
        unsigned int i;
        bool eof = false;
        
        this->Lines = 0;
        
#       ifdef __DEBUG__
        fprintf(PopCorn::Debug.Fp, "TextFile loading (%s) ... ", FileName);
#       endif
        
        if((File = fopen(FileName, "r")) == NULL)
        {
#           ifdef __DEBUG__
            fprintf(PopCorn::Debug.Fp, "not done, error about the file\n");
#           endif
	        return false;
        }
        
        while(!feof(File) && !eof)
        {
            Var = new char[128];
            for(i=0; i<128; i++)
            {
                c = fgetc(File);
                if(c == EOF)
                {
                    eof = true;
                    break;
                }
                
                if(c == '\n')
                    break;
                else if(c != '\t')
                    Var[i] = c;
            }
            
            Var[i] = '\0';
            
            ++this->Lines;
            
            this->Lst.AddElem((void*)Var);
        }
label:
        
#       ifdef __DEBUG__
        fprintf(PopCorn::Debug.Fp, "done\n");
#       endif

        fclose(File);
        
        return true;
    }
    
    char* ObjLinesFile::operator[](unsigned short Index)
    {
        unsigned short j=0;
        
        for(LISTE* i=this->Lst.m_pDebut; i != NULL; i=i->pSuivant)
            if(j++ == Index)
                return (char*)i->val;
        
        return NULL;
    }
};

