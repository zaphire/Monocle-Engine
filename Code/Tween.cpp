#include "Tween.h"
#include "Monocle.h"
#include <math.h>
#include "Math.h"

namespace Monocle
{
	std::list<Tweener*> Tween::tweeners;
	std::list<Tweener*> Tween::tweenersToRemove;

	Tweener::Tweener(float *value, float end, float time, EaseType easeType)
		: time(time), timer(0.0f), easeType(easeType), end(end), value(value), start(*value)
	{
	}

	void Tweener::Update()
	{
		timer += Monocle::deltaTime;
		if (timer > time) timer = time;
		float p = (timer/time);
		p = Tween::Ease(p, easeType);
		(*value) = ((end - start) * p) + start;

		if (timer >= time)
		{
			Tween::Remove(this);
		}
	}

	Tween::Tween()
	{
	}

	void Tween::To(float *value, float to, float time, EaseType easeType)
	{
		Tweener *t = new Tweener(value, to, time, easeType);
		tweeners.push_back(t);
	}

	void Tween::FromTo(float *value, float from, float to, float time, EaseType easeType)
	{
		*value = from;
		Tweener *t = new Tweener(value, to, time, easeType);
		tweeners.push_back(t);
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
}