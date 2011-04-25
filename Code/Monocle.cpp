#include "Monocle.h"

#if defined(MONOCLE_WINDOWS)
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#include <ShellAPI.h>
#endif


namespace Monocle
{
    // Frame time in seconds
	double deltaTime		= 0.0;
    
    // Time since start in seconds
	double timeSinceStart	= 0.0;

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


	/*
	void OpenProgramWithFile(const std::string &program, const std::string &filename)
	{

		CreateProcess(program.c_str(),
			filename.c_str(),
  0,
  0,
  __in         BOOL bInheritHandles,
  __in         DWORD dwCreationFlags,
  __in_opt     LPVOID lpEnvironment,
  __in_opt     LPCTSTR lpCurrentDirectory,
  __in         LPSTARTUPINFO lpStartupInfo,
  __out        LPPROCESS_INFORMATION lpProcessInformation
);
	}
	*/
}