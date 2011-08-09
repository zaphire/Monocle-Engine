//
//  Shader.cpp
//
//  Created by Josh Whelchel on 5/10/11.
//  Copyright 2011 jwmusic.org. All rights reserved.
//  Some additions by Tom Rab

#ifdef MONOCLE_OPENGLES

#include "../Monocle.h"
#include "../Shader.h"
#include "../Assets.h"
#include "../Debug.h"
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include <stdlib.h>

//#define ENABLE_GLES_SHADERS

namespace Monocle
{
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
        
		str[r] = '\0'; /* Shader sources have to term with null */
        
		fclose(fd);
        
		return str;
	}
    
	void printLog(GLuint obj)
	{
#ifndef ENABLE_GLES_SHADERS
        return;
#endif
		int infologLength = 0;
		char infoLog[1024];
        
		if (glIsShader(obj))
			glGetShaderInfoLog(obj, 1024, &infologLength, infoLog);
		else
			glGetProgramInfoLog(obj, 1024, &infologLength, infoLog);
        
		if (infologLength > 0)
			Debug::Log(infoLog);
	}
 
	Shader::Shader(const std::string &vertexPath,const std::string &fragmentPath )
	{
#ifndef ENABLE_GLES_SHADERS
        Debug::Log("Shaders not enabled on this platform");
        return;
#else
		this->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
		this->shaderProgram = glCreateProgram();
        
		if (vertexPath != "")
		{
			const char *vShader = file2string(Assets::GetContentPath() + vertexPath);
            
			if (vShader){
				glShaderSource(vertexShader, 1, &vShader, NULL);
				glCompileShader(vertexShader);
				printLog(vertexShader);
                
				free((void*)vShader);
                
				glAttachShader(shaderProgram, vertexShader);
			}
		}
        
		if (fragmentPath != "")
		{
			const char *fShader = file2string(Assets::GetContentPath() + fragmentPath);
            
			if (fShader){
				glShaderSource(fragmentShader, 1, &fShader, NULL);
				glCompileShader(fragmentShader);
				printLog(fragmentShader);
                
				free((void*)fShader);
                
				glAttachShader(shaderProgram, fragmentShader);
			}
		}
        
		glLinkProgram(shaderProgram);
		printLog(shaderProgram);
#endif
	}
    
	Shader::~Shader()
	{
#ifndef ENABLE_GLES_SHADERS
        return;
#else
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteProgram(shaderProgram);
#endif
	}
    
	void Shader::Use()
	{
#ifndef ENABLE_GLES_SHADERS
        return;
#else
		glUseProgram(shaderProgram);
        
		for(std::map<std::string,float>::iterator iter = floats.begin(); iter != floats.end();iter++)
		{
			GLint var = glGetUniformLocation(shaderProgram, iter->first.c_str());
			glUniform1f(var,iter->second);
		}
		for(std::map<std::string,Vector2>::iterator iter = vec2s.begin(); iter != vec2s.end();iter++)
		{
			GLint var = glGetUniformLocation(shaderProgram, iter->first.c_str());
			glUniform2f(var,iter->second.x,iter->second.y);
		}
		for(std::map<std::string,Vector3>::iterator iter = vec3s.begin(); iter != vec3s.end();iter++)
		{
			GLint var = glGetUniformLocation(shaderProgram, iter->first.c_str());
			glUniform3f(var,iter->second.x,iter->second.y,iter->second.z);
		}
		for(std::map<std::string,Color>::iterator iter = vec4s.begin(); iter != vec4s.end();iter++)
		{
			GLint var = glGetUniformLocation(shaderProgram, iter->first.c_str());
			glUniform4f(var,iter->second.r,iter->second.g,iter->second.b,iter->second.a);
		}
		for(std::map<std::string,int>::iterator iter = ints.begin(); iter != ints.end();iter++)
		{
			GLint var = glGetUniformLocation(shaderProgram, iter->first.c_str());
			glUniform1i(var,iter->second);
		}
		for(std::map<std::string,unsigned int>::iterator iter = textures.begin(); iter != textures.end();iter++)
		{
			GLint var = glGetUniformLocation(shaderProgram, iter->first.c_str());
			glUniform1i(var,iter->second);
		}
        
        GLint dT = glGetUniformLocation(shaderProgram, "deltaTime");
        glUniform1f(dT,Monocle::deltaTime);
        
        GLint tT = glGetUniformLocation(shaderProgram, "totalTime");
        glUniform1f(tT,Monocle::timeSinceStart);
#endif
	}
    
	void Shader::SetUniformFloat(const std::string &name, float value )
	{
		floats[name] = value;
	}

	void Shader::SetUniformVec2(const std::string &name, Vector2 vec )
	{
		vec2s[name] = vec;
	}
    
	void Shader::SetUniformVec3(const std::string &name, Vector3 vec )
	{
		vec3s[name] = vec;
	}
    
	void Shader::SetUniformVec4(const std::string &name, Color vec, bool ignoreAlpha )
	{
		vec4s[name] = ignoreAlpha ? Color(vec.r,vec.g,vec.b,1.0f) : vec;
	}
    
	void Shader::SetUniformInt(const std::string &name, int value )
	{
		ints[name] = value;      
	}

	void Shader::SetUniformSampler2D(const std::string &name)
	{
		textures[name] = textures.size()-1;
	}
    
	void Shader::None()
	{
#ifdef ENABLE_GLES_SHADERS
		glUseProgram(0);
#endif
	}
}

#endif // MONOCLE_OPENGLES
