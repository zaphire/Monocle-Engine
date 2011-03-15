#pragma once

namespace Native
{
	class Backend
	{
	public:
		virtual void Update() = 0;
		virtual void ShowBuffer() = 0;
		virtual long GetMilliseconds() = 0;
	};
}