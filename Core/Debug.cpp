#include "Debug.h"

#include <iostream>
#include <fstream>
#include "Entity.h"

namespace Monocle
{
	class Debug::DebugOutStream
	{
	public:
		DebugOutStream() : logOut(), logFilename(""), outDest(LOG_COUT) {}

		~DebugOutStream()
		{
			if(logOut.is_open())
			{
				logOut.close();
			}
		}

		void SetLogFile(const std::string &filename)
		{
			if(filename == "")
			{
				outDest &= ~LOG_FILE;
			}

			if(filename != logFilename)
			{
				if(logOut.is_open())
				{
					logOut.close();
				}

				if(filename != "")
				{
					char *cfilename = strdup(filename.c_str());
					logOut.open(cfilename);
					free(cfilename);
				}

				logFilename = filename;
			}
		}
		
		void SetDestination(int dest, const std::string &filename)
		{
			outDest = dest;

			if(dest & LOG_FILE)
			{
				SetLogFile(filename);
			}
			else
			{
				SetLogFile("");
			}
		}

		template <typename T>
		DebugOutStream &operator<<(const T &obj)
		{
			if(outDest & LOG_FILE)
			{
				logOut << obj;
			}
			if(outDest & LOG_COUT)
			{
				std::cout << obj;
			}
			if(outDest & LOG_CERR)
			{
				std::cerr << obj;
			}
			if(outDest & LOG_CLOG)
			{
				std::clog << obj;
			}

			return *this;
		}

		DebugOutStream &operator<<(std::ostream& func(std::ostream&))
		{
			if(outDest & LOG_FILE)
			{
				logOut << func;
			}
			if(outDest & LOG_COUT)
			{
				std::cout << func;
			}
			if(outDest & LOG_CERR)
			{
				std::cerr << func;
			}
			if(outDest & LOG_CLOG)
			{
				std::clog << func;
			}

			return *this;
		}
	private:
		std::ofstream logOut;
		std::string logFilename;
		int outDest;
	};

	bool Debug::render = false;
	bool Debug::showBounds = false;
	Entity *Debug::selectedEntity;
	int Debug::layerMin = -50;
	int Debug::layerMax = 50;

	Debug::DebugOutStream Debug::outStream
#ifndef MONOCLE_MAC
     = Debug::DebugOutStream()
#endif
    ;
	void Debug::Init()
	{
		render = false;
	}

	void Debug::Log(const char *outputString)
	{
		outStream << outputString << std::endl;
	}

	void Debug::Log(bool boolean)
	{
		outStream << (boolean?"true":"false") << std::endl;
	}

	void Debug::Log(int num)
	{
		outStream << num << std::endl;
	}

	void Debug::Log(long num)
	{
		outStream << num << std::endl;
	}

	void Debug::Log(float num)
	{
		outStream << num << std::endl;
	}

	void Debug::Log(double num)
	{
		outStream << num << std::endl;
	}

	void Debug::Log(const Vector2& vec)
	{
		outStream << "Vector2: (" << vec.x << ", " << vec.y << ")" << std::endl;
	}

	void Debug::Log(const Vector3& vec)
	{
		outStream << "Vector3: (" << vec.x << ", " << vec.y << ", " << vec.z << ")" << std::endl;
	}

	void Debug::Log(const std::string& string)
	{
		outStream << string << std::endl;
	}

	void Debug::SetLogDestination(OutputDestFlags dest, const std::string &filename)
	{
		outStream.SetDestination(dest, filename);
	}
}
