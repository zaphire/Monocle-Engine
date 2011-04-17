#pragma once

#include <string>
#include <map>

namespace Monocle
{
	class Audio
	{
	public:
		Audio();
		void Init();

		void PlaySFX(const std::string &sfx);
		
		std::map<std::string, void*> sfxMap;
	};
}