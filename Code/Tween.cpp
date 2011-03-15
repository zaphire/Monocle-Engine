#include "Tween.h"
#include "Monocle.h"
#include <math.h>
#include "Math.h"

namespace Monocle
{
	std::list<Tween*> Tween::tweens;
	std::list<Tween*> Tween::tweensToRemove;

	Tween::Tween()
	{
	}

	Tween::Tween(float *value, float end, float time, EaseType easeType)
		: time(time), timer(0.0f), easeType(easeType), end(end), value(value), start(*value)
	{
	}

	void Tween::To(float *value, float to, float time, EaseType easeType)
	{
		Tween *t = new Tween(value, to, time, easeType);
		tweens.push_back(t);
	}

	void Tween::FromTo(float *value, float from, float to, float time, EaseType easeType)
	{
		*value = from;
		Tween *t = new Tween(value, to, time, easeType);
		tweens.push_back(t);
	}

	void Tween::Update()
	{
		for (std::list<Tween*>::iterator i = tweens.begin(); i != tweens.end(); ++i)
		{
			(*i)->UpdateOne();
		}
		
		//clear garbage
		for (std::list<Tween*>::iterator i = tweensToRemove.begin(); i != tweensToRemove.end(); ++i)
		{
			tweens.remove(*i);
			delete *i;
		}
		tweensToRemove.clear();
	}

	void Tween::UpdateOne()
	{
		timer += Monocle::deltaTime;
		if (timer > time) timer = time;
		float p = (timer/time);
		p = Ease(p, easeType);
		(*value) = ((end - start) * p) + start;

		if (timer >= time)
		{
			tweensToRemove.push_back(this);
		}
	}

	//clear all tweens
	void Tween::Clear()
	{
		for (std::list<Tween*>::iterator i = tweens.begin(); i != tweens.end(); ++i)
		{
			delete *i;
		}

		tweens.clear();
		tweensToRemove.clear();
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