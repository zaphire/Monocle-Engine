#pragma once

//todo: replace list with something more portable
#include <list>
#include "Vector2.h"
#include "Color.h"

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
		EASE_INBOUNCE,
		EASE_OUTBOUNCE,
		EASE_INOUTBOUNCE,
		EASE_MAX
	};

	class Tweener;

	class Tween
	{
	public:
		Tween();

		// float
		static void To(float *value, const float &end, float time, EaseType easeType);
		static void FromTo(float *value, const float &start, const float &end, float time, EaseType easeType);

		// Vector2
		static void To(Vector2 *value, const Vector2 &end, float time, EaseType easeType);
		static void FromTo(Vector2 *value, const Vector2 &start, const Vector2 &end, float time, EaseType easeType);

		// Color
		static void To(Color *value, const Color &end, float time, EaseType easeType);
		static void FromTo(Color *value, const Color &start, const Color &end, float time, EaseType easeType);

		static void Update();
		static void Clear();
		static float Ease(float p, EaseType easeType);
		static void Remove(Tweener *tweener);

	private:
		template <typename T, typename N>
		static void To(T *value, const T &end, float time, EaseType easeType);
		template <typename T, typename N>
		static void FromTo(T *value, const T &start, const T &end, float time, EaseType easeType);

		static std::list<Tweener*> tweeners;
		static std::list<Tweener*> tweenersToRemove;
	};

	class Tweener
	{
	protected:
		friend class Tween;
		
		Tweener(float time, EaseType easeType);

		float timer, time;
		EaseType easeType;

		void Update();
		virtual void SetValue(float p)=0;
	};

	class FloatTweener : public Tweener
	{
	private:
		friend class Tween;

		FloatTweener(float *value, float end, float time, EaseType easeType);
		void SetValue(float p);

		float *value, start, end;
	};

	class Vector2Tweener : public Tweener
	{
	private:
		friend class Tween;

		Vector2Tweener(Vector2 *value, Vector2 end, float time, EaseType easeType);
		void SetValue(float p);

		Vector2 *value, start, end;
	};

	class ColorTweener : public Tweener
	{
	private:
		friend class Tween;

		ColorTweener(Color *value, Color end, float time, EaseType easeType);
		void SetValue(float p);

		Color *value, start, end;
	};
}