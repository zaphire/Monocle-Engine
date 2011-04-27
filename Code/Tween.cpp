#include "Tween.h"
#include "MonocleToolkit.h"
#include <math.h>

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
				return -c * cos(t/d * (Monocle::pi/2)) + c + b;
			case EASE_OUTSIN:
				return c * sin(t/d * (Monocle::pi/2)) + b;
			case EASE_INOUTSIN:
				return -c/2 * (cos(Monocle::pi*t/d) - 1) + b;

			case EASE_INBOUNCE:
				return c - Ease(1-t, EASE_OUTBOUNCE) + b;
			case EASE_OUTBOUNCE:
				t /= d;
				if ((t) < (1/2.75f))
				{
					return c*(7.5625f*t*t) + b;
				}
				else if (t < (2/2.75f))
				{
					t-=(1.5f/2.75f);
					return c*(7.5625f*(t)*t + .75f) + b;
				}
				else if (t < (2.5f/2.75f))
				{
					t-=(2.25f/2.75f);
					return c*(7.5625f*(t)*t + .9375f) + b;
				}
				else
				{
					t-=(2.625f/2.75f);
					return c*(7.5625f*(t)*t + .984375f) + b;
				}
				break;
			case EASE_INOUTBOUNCE:
				if (t < d/2)
					return Ease(t*2, EASE_INBOUNCE) * .5f + b;
				else
					return Ease(t*2-d, EASE_OUTBOUNCE) * .5f + c*.5f + b;
				break;
			default:
				break;
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
