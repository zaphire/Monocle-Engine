//
//  Shader.h
//  Rokkit
//
//  Created by Josh Whelchel on 5/10/11.
//  Copyright 2011 jwmusic.org. All rights reserved.
//  Some additions by Tom Rab
#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Color.h"
#include <map>
#include <string>

namespace Monocle
{    
	class Shader
	{
		public:
        
		Shader(const std::string &vertexPath,const std::string &fragmentPath);
		~Shader();
        
		void Use();
        
		void SetUniformFloat(const std::string &name, float value );
		void SetUniformVec2(const std::string &name, Vector2 vec );
		void SetUniformVec3(const std::string &name, Vector3 vec );
		void SetUniformVec4(const std::string &name, Color vec, bool ignoreAlpha = false );
		void SetUniformInt(const std::string &name, int value );
		void SetUniformSampler2D(const std::string &name);
        
		static void UseNone();
        
		unsigned int vertexShader;
		unsigned int fragmentShader;
		unsigned int shaderProgram;

		std::map<std::string,float> floats;
		std::map<std::string,Vector2> vec2s;
		std::map<std::string,Vector3> vec3s;
		std::map<std::string,Color> vec4s;
		std::map<std::string,int> ints;
		std::map<std::string,unsigned int> textures;
	};
}
