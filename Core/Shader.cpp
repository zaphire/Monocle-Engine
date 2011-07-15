//
//  Shader.cpp
//  Rokkit
//
//  Created by Josh Whelchel on 5/10/11.
//  Copyright 2011 jwmusic.org. All rights reserved.
//

#include "Shader.h"
#include <stdlib.h>

using namespace Monocle;

namespace Teelo {
    
    char *file2string(std::string sPath)
    {
        const char *path = sPath.c_str();
        FILE *fd;
        long len,
        r;
        char *str;
        
        if (!(fd = fopen(path, "r")))
        {
            fprintf(stderr, "Can't open file '%s' for reading\n", path);
            return NULL;
        }
        
        fseek(fd, 0, SEEK_END);
        len = ftell(fd);
        
        printf("File '%s' is %ld long\n", path, len);
        
        fseek(fd, 0, SEEK_SET);
        
        if (!(str = (char*)malloc(len * sizeof(char))))
        {
            fprintf(stderr, "Can't malloc space for '%s'\n", path);
            return NULL;
        }
        
        r = fread(str, sizeof(char), len, fd);
        
        str[r - 1] = '\0'; /* Shader sources have to term with null */
        
        fclose(fd);
        
        return str;
    }
    
    void printLog(GLuint obj)
    {
        int infologLength = 0;
        char infoLog[1024];
        
        if (glIsShader(obj))
            glGetShaderInfoLog(obj, 1024, &infologLength, infoLog);
        else
            glGetProgramInfoLog(obj, 1024, &infologLength, infoLog);
        
        if (infologLength > 0)
            printf("%s\n", infoLog);
    }
 
    Shader::Shader( std::string vertexPath, std::string fragmentPath )
    {
        this->glFShader = glCreateShader(GL_FRAGMENT_SHADER);
        this->glVShader = glCreateShader(GL_VERTEX_SHADER);
        this->glProgram = glCreateProgram();
        
        if (vertexPath != "")
        {
            const char *vShader = file2string(Assets::GetContentPath() + vertexPath);
            
            if (vShader){
                glShaderSource(this->glVShader, 1, &vShader, NULL);
                glCompileShader(this->glVShader);
                printLog(glVShader);
                
                free((void*)vShader);
                
                glAttachShader(this->glProgram, this->glVShader);
            }
        }
        
        if (fragmentPath != "")
        {
            const char *fShader = file2string(Assets::GetContentPath() + fragmentPath);
            
            if (fShader){
                glShaderSource(this->glFShader, 1, &fShader, NULL);
                glCompileShader(this->glFShader);
                printLog(glFShader);
                
                free((void*)fShader);
                
                glAttachShader(this->glProgram, this->glFShader);
            }
        }
        
        glLinkProgram(this->glProgram);
        printLog(glProgram);
    }
    
    Shader::~Shader()
    {
        glDeleteShader(this->glVShader);
        glDeleteShader(this->glFShader);
        glDeleteProgram(this->glProgram);
    }
    
    void Shader::Use()
    {
        glUseProgram(this->glProgram);
        
        GLint sampler1U = glGetUniformLocation(glProgram, "sampler1");
        glUniform1i(sampler1U, 1);
        GLint sampler2U = glGetUniformLocation(glProgram, "sampler2");
        glUniform1i(sampler2U, 2);
        
        GLint dT = glGetUniformLocation(this->glProgram, "deltaTime");
        glUniform1f(dT,Monocle::deltaTime);
        
        GLint tT = glGetUniformLocation(this->glProgram, "totalTime");
        glUniform1f(tT,Monocle::timeSinceStart);
    }
    
    void Shader::SetUniform( std::string name, float value )
    {
        GLint var = glGetUniformLocation(this->glProgram, name.c_str());
        glUniform1f(var,value);
    }
    
    void Shader::SetUniform3Vec( std::string name, Vector3 vec )
    {
        GLint var = glGetUniformLocation(this->glProgram, name.c_str());
        glUniform3f(var,vec.x,vec.y,vec.z);
    }
    
    void Shader::SetUniform2Vec( std::string name, Vector2 vec )
    {
        GLint var = glGetUniformLocation(this->glProgram, name.c_str());
        glUniform2f(var,vec.x,vec.y);
    }
    
    void Shader::SetUniformColor( std::string name, Color vec, bool ignoreAlpha )
    {
        GLint var = glGetUniformLocation(this->glProgram, name.c_str());
        
        if (ignoreAlpha)
            glUniform4f(var,vec.r,vec.g,vec.b,1.0);
        else
            glUniform4f(var,vec.r,vec.g,vec.b,vec.a);
    }
    
    void Shader::SetUniformInt( std::string name, int value )
    {
        GLint var = glGetUniformLocation(this->glProgram, name.c_str());
        glUniform1i(var,value);        
    }
    
    void Shader::None()
    {
        glUseProgram(0);
    }
}