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
#include <stdarg.h>

namespace PopCorn
{
    ObjConfigFile::~ObjConfigFile()
    {
        this->Clear();
    }
    
    void ObjConfigFile::Clear(void)
    {
        for(LISTE* i=this->Lst.m_pDebut; i != NULL; i=i->pSuivant)
            delete (struct VariableStruct*)i->val;
    }
    
    bool ObjConfigFile::Load(const char* FileName)
    {
        FILE* File;
        struct VariableStruct* Var;
        unsigned short i, len;
        char c;
        bool eof=false;
        
#       ifdef __DEBUG__
        fprintf(PopCorn::Debug.Fp, "TextFile loading (%s) ... ", FileName);
#       endif
        
        if((File = fopen(FileName, "r")) == NULL)
        {
#       ifdef __DEBUG__
        fprintf(PopCorn::Debug.Fp, "not done, error about the file\n");
#       endif
	        return false;
        }
        
        while(!feof(File) && !eof)
        {
            Var = new struct VariableStruct;
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
                    Var->Var[i] = c;
            }
            
            Var->Var[i] = '\0';

            len=strlen(Var->Var);
            
            for(i=0; i<len; i++)
                if(Var->Var[i] == '=')
                {
                    Var->Var[i] = '\0';
                    Var->Value = &Var->Var[i+1];
                    break;
                }            
            
            this->Lst.AddElem((void*)Var);
        }
        
#       ifdef __DEBUG__
        fprintf(PopCorn::Debug.Fp, "done\n");
#       endif
        
        return true;
    }
    
    char* ObjConfigFile::Value(char* Var, ...)
    {
        va_list list;
        char Str[256];
        
        va_start(list, Var);
        vsprintf(Str, Var, list);
        va_end(list);
        
        for(LISTE* i=this->Lst.m_pDebut; i != NULL; i=i->pSuivant)
            if( strcmp(((struct VariableStruct*)i->val)->Var, Str) == 0)
                return ((struct VariableStruct*)i->val)->Value;
                
        return NULL;
    }
    
    char* ObjConfigFile::operator[](char* Var)
    {
        return this->Value(Var);
    }
};

