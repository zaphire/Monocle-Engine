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

	class Tween
	{
	public:
		Tween();
		static void To(float *value, float end, float time, EaseType easeType); 
		static void FromTo(float *value, float start, float end, float time, EaseType easeType); 
		static void Update();
		static void Clear();
		static float Ease(float p, EaseType easeType);

	private:
		Tween(float *value, float end, float time, EaseType easeType);
		void UpdateOne();

		float *value;
		float timer, time, end, start;
		EaseType easeType;

		static std::list<Tween*> tweens;
		static std::list<Tween*> tweensToRemove;
	};
}