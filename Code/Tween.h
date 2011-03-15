#pragma once

//todo: replace list with something more portable
#include <list>

namespace Monocle
{
	enum EaseType
	{
		EASE_LINEAR=0,
		EASE_INOUTSIN,
		EASE_INSIN,
		EASE_OUTSIN,
		EASE_INQUAD,
		EASE_OUTQUAD,
		EASE_INOUTQUAD,
		EASE_MAX
	};

	class Tweener;

	class Tween
	{
	public:
		Tween();
		static void To(float *value, float end, float time, EaseType easeType); 
		static void FromTo(float *value, float start, float end, float time, EaseType easeType); 
		static void Update();
		static void Clear();
		static float Ease(float p, EaseType easeType);
		static void Remove(Tweener *tweener);

	private:
		static std::list<Tweener*> tweeners;
		static std::list<Tweener*> tweenersToRemove;
	};

	class Tweener
	{
	private:
		friend class Tween;
		Tweener(float *value, float end, float time, EaseType easeType);
		
		float *value;
		float timer, time, end, start;
		EaseType easeType;

		void Update();
	};
}