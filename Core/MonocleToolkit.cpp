#include "Monocle.h"

#if defined(MONOCLE_WINDOWS)
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#include <ShellAPI.h>
#elif  defined(MONOCLE_LINUX)
	#include <sys/types.h>
	#include <dirent.h>
#endif

namespace Monocle
{
	double deltaTime					= 0.0;
	double timeSinceStart			= 0.0;
	double timeSinceSceneStart		= 0.0;

	const float pi			= 3.14159265358979f;
	const float rad2Deg		= 57.29578f;
	const float deg2Rad		= 0.01745329f;
    
    long entitiesDrawn              = 0;

	void OpenURL(const std::string &url)
	{
#if defined(MONOCLE_WINDOWS)
		//ShellExecute(NULL, "open", "c:\, NULL, NULL, SW_SHOWNORMAL);
		Debug::Log("OpenURL: " + url);

		ShellExecute(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
#elif defined(MONOCLE_MAC)
		/*
		   CFStringRef str = CFStringCreateWithCString (0, url.c_str(), 0);
		   CFURLRef ref = CFURLCreateWithString(kCFAllocatorDefault, str, NULL);
		   LSOpenCFURLRef(ref, 0);
		   CFRelease(ref);		if (type.find('.') == std::string::npos)
		{
			type = "." + type;
		}
		   CFRelease(str);
		 */
#elif defined(MONOCLE_LINUX)
		/*
		   std::string cmd("PATH=$PATH:. xdg-open '");
		   cmd += url;
		   cmd += "'";
		   system(cmd.c_str());
		 */
#endif
	}

	// AH: wanted to call this GetCurrentDirectory, but the compiler wouldn't let me Q_Q
	std::string GetWorkingDirectory()
	{
#if defined(MONOCLE_WINDOWS)
		char buffer[256];
		::GetCurrentDirectory(256, (char*)&buffer);
		return std::string(buffer) + + "\\";
#elif defined(MONOCLE_LINUX)
        char buffer[FILENAME_MAX];
        getcwd(buffer, sizeof(buffer));
		return std::string(buffer) + "\\";
#else
		return "";
#endif
	}


	void ForEachFile(std::string path, std::string type, void callback(const std::string &filename, intptr_t param), intptr_t param)
	{
		//std::string path = inPath;
		//std::string type = "." + inType;

		if (path.empty()) return;

		//stringToLowerUserData(path);
		//stringToLower(type);
		Debug::Log("forEachFile - path: " + path + " type: " + type);

		int end = path.size()-1;
		if (path[end] != '/')
			path += "/";

		if (type.find('.') == std::string::npos)
		{
			type = "." + type;
		}

		std::string fullPath = Assets::GetContentPath() + path;

#if defined(MONOCLE_LINUX)
		DIR *dir=0;

		dir = opendir(fullPath.c_str());
		if (dir)
		{
			dirent *file=0;
			while ( (file=readdir(dir)) != NULL )
			{
				if (file->d_name && strlen(file->d_name) > 4)
				{
					Debug::Log(file->d_name);
					char *extension=strrchr(file->d_name,'.');
					if (extension)
					{
						Debug::Log(extension);
						if (extension!=NULL)
						{
							if (strcasecmp(extension,type.c_str())==0)
							{
								printf("calling: %s", (path + std::string(file->d_name)).c_str());
								callback(path + std::string(file->d_name), param);
							}
						}
					}
				}
			}
			closedir(dir);
		}
		else
		{
			Debug::Log("FAILED TO OPEN DIR");
		}

#elif defined(MONOCLE_WINDOWS)
		BOOL            fFinished;
		HANDLE          hList;
		WIN32_FIND_DATA FileData;

		std::string searchPath = fullPath + "\\*";

		// Get the first file
		hList = FindFirstFile(searchPath.c_str(), &FileData);
		if (hList == INVALID_HANDLE_VALUE)
		{
			Debug::Log("No files found.");
		}
		else
		{
			// Traverse through the directory structure
			fFinished = FALSE;
			while (!fFinished)
			{
				std::string filename = FileData.cFileName;
				if (filename.size()>4)
				{
					std::string filetype = filename.substr(filename.size()-4, filename.size());
					if (filetype==type)
					{
						Debug::Log("calling callback for file: " + path+filename);
						callback(path+filename, param);
					}
				}

				if (!FindNextFile(hList, &FileData))
				{
					fFinished = TRUE;
				}
			}
		}

		FindClose(hList);
#endif
	}
    
    void PremultiplyAlpha( unsigned char *data, int w, int h )
    {
        int pixels = w*h;
        unsigned char *ptr = data;
        
        for (int i=0;i<pixels;i++)
        {
            int a = ptr[3];
            
            ptr[0] *= (a/255.0);
            ptr[1] *= (a/255.0);
            ptr[2] *= (a/255.0);
            
            ptr += 4;
        }
    }
}
