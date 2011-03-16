#include "Tween.h"
#include "Monocle.h"
#include <math.h>
#include "Math.h"

namespace Monocle
{
	std::list<Tweener*> Tween::tweeners;
	std::list<Tweener*> Tween::tweenersToRemove;

	Tween::Tween()
	{
	}

	template <typename T, typename N>
	void Tween::To(T *value, const T &end, float time, EaseType easeType)
	{
		Tweener *t = (Tweener*)new N(value, end, time, easeType);
		tweeners.push_back(t);
	}

	template <typename T, typename N>
	void Tween::FromTo(T *value, const T &start, const T &end, float time, EaseType easeType)
	{
		*value = start;
		Tweener *t = (Tweener*)new N(value, end, time, easeType);
		tweeners.push_back(t);
	}
	
	//float
	void Tween::To(float *value, const float &end, float time, EaseType easeType)
	{
		To<float, FloatTweener>(value, end, time, easeType);
	}

	void Tween::FromTo(float *value, const float &start, const float &end, float time, EaseType easeType)
	{
		FromTo<float, FloatTweener>(value, start, end, time, easeType);
	}

	//Vector2
	void Tween::To(Vector2 *value, const Vector2 &end, float time, EaseType easeType)
	{
		To<Vector2, Vector2Tweener>(value, end, time, easeType);
	}

	void Tween::FromTo(Vector2 *value, const Vector2 &start, const Vector2 &end, float time, EaseType easeType)
	{
		FromTo<Vector2, Vector2Tweener>(value, start, end, time, easeType);
	}

	//Color
	void Tween::To(Color *value, const Color &end, float time, EaseType easeType)
	{
		To<Color, ColorTweener>(value, end, time, easeType);
	}

	void Tween::FromTo(Color *value, const Color &start, const Color &end, float time, EaseType easeType)
	{
		FromTo<Color, ColorTweener>(value, start, end, time, easeType);
	}

	void Tween::Update()
	{
		for (std::list<Tweener*>::iterator i = tweeners.begin(); i != tweeners.end(); ++i)
		{
			(*i)->Update();
		}
		
		//clear garbage
		for (std::list<Tweener*>::iterator i = tweenersToRemove.begin(); i != tweenersToRemove.end(); ++i)
		{
			tweeners.remove(*i);
			delete *i;
		}
		tweenersToRemove.clear();
	}

	void Tween::Remove(Tweener *tweener)
	{
		tweenersToRemove.push_back(tweener);
	}

	//clear all tweens
	void Tween::Clear()
	{
		for (std::list<Tweener*>::iterator i = tweeners.begin(); i != tweeners.end(); ++i)
		{
			delete *i;
		}

		tweeners.clear();
		tweenersToRemove.clear();
	}

	float Tween::Ease(float t, EaseType easeType)
	{
		float b = 0.0f;
		float c = 1.0f;
		float d = 1.0f;
		float s = 0.0f;
		float td = t/d;

		switch (easeType)
		{
			case EASE_INQUAD:
				return c*td*td + b;
			case EASE_OUTQUAD:
				return (-c) *(td)*(t-2) + b;
			case EASE_INOUTQUAD:
				if((td/2) < 1) return (c/2)*(td*td) + b;
				return (-c/2) * ((--td)*(td-2) - 1) + b;
			case EASE_INSIN:
				return -c * cos(t/d * (Math::PI/2)) + c + b;
			case EASE_OUTSIN:
				return c * sin(t/d * (Math::PI/2)) + b;
			case EASE_INOUTSIN:
				return -c/2 * (cos(Math::PI*t/d) - 1) + b;
		}

		return t;
	}


	Tweener::Tweener(float time, EaseType easeType)
		: time(time), timer(0.0f), easeType(easeType)
	{
	}

	void Tweener::Update()
	{
		timer += Monocle::deltaTime;
		if (timer > time) timer = time;
		float p = (timer/time);
		p = Tween::Ease(p, easeType);
		SetValue(p);

		if (timer >= time)
		{
			Tween::Remove((Tweener*)this);
		}
	}

	FloatTweener::FloatTweener(float *value, float end, float time, EaseType easeType)
		: Tweener(time, easeType), value(value), start(*value), end(end)
	{}

	void FloatTweener::SetValue(float p)
	{
		(*value) = ((end - start) * p) + start;
	}

	Vector2Tweener::Vector2Tweener(Vector2 *value, Vector2 end, float time, EaseType easeType)
		: Tweener(time, easeType), value(value), start(*value), end(end)
	{}

	void Vector2Tweener::SetValue(float p)
	{
		(*value) = ((end - start) * p) + start;
	}

	ColorTweener::ColorTweener(Color *value, Color end, float time, EaseType easeType)
		: Tweener(time, easeType), value(value), start(*value), end(end)
	{}

	void ColorTweener::SetValue(float p)
	{
		(*value) = ((end - start) * p) + start;
	}
}