#include "Monocle.h"

#if defined(MONOCLE_WINDOWS)
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#include <ShellAPI.h>
#endif

namespace Monocle
{
	double deltaTime		= 0.0;
	double timeSinceStart		= 0.0;

	const float pi			= 3.14159265358979f;
	const float rad2Deg		= 57.29578f;
	const float deg2Rad		= 0.01745329f;

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
		   CFRelease(ref);
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
#else
		return "";
#endif
	}


	void ForEachFile(const std::string &path, const std::string &type, void callback(const std::string &filename, intptr_t param), intptr_t param)
	{
		if (path.empty()) return;

		//stringToLowerUserData(path);
		//stringToLower(type);
		Debug::Log("forEachFile - path: " + path + " type: " + type);

#if defined(BBGE_BUILD_UNIX)
		DIR *dir=0;
		dir = opendir(path.c_str());
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
#endif

#ifdef BBGE_BUILD_WINDOWS
		BOOL            fFinished;
		HANDLE          hList;
		TCHAR           szDir[MAX_PATH+1];
		WIN32_FIND_DATA FileData;

		int end = path.size()-1;
		if (path[end] != '/')
			path[end] += '/';

		// Get the proper directory path
		// \\ %s\\*



		if (type.find('.')==std::string::npos)
		{
			type = "." + type;
		}


		//std::string add = "%s*" + type;

		//sprintf(szDir, "%s*", path.c_str());
		sprintf(szDir, "%s\\*", path.c_str());

		stringToUpper(type);

		// Get the first file
		hList = FindFirstFile(szDir, &FileData);
		if (hList == INVALID_HANDLE_VALUE)
		{
			//printf("No files found\n\n");
			Debug::Log("No files of type " + type + " found in path " + path);
		}
		else
		{
			// Traverse through the directory structure
			fFinished = FALSE;
			while (!fFinished)
			{
				// Check the object is a directory or not
				//printf("%*s%s\n", indent, "", FileData.cFileName);
				std::string filename = FileData.cFileName;
				//Debug::Log("found: " + filename);
				if (filename.size()>4)
				{

					std::string filetype = filename.substr(filename.size()-4, filename.size());
					stringToUpper(filetype);
					//Debug::Log("comparing: " + filetype + " and: " + type);
					if (filetype==type)
					{
						callback(path+filename, param);
					}
				}


				if (!FindNextFile(hList, &FileData))
				{
					/*
					   if (GetLastError() == ERROR_NO_MORE_FILES)
					   {
					   fFinished = TRUE;
					   }
					 */
					fFinished = TRUE;
				}
			}
		}

		FindClose(hList);
#endif
	}
}
