#pragma once
#include "Event.h"
#include "Camera.h"
#include "CamEffectType.h"

class ShakeEvent : public Event
{
private:
	int _time, _timeOnEvent = 0;
	int _intensity;
	Camera* _cam;
	CamEffect _camEffect;

public:
	ShakeEvent(Camera* cam, int time, int intesity) : _cam(cam), _time(time), _intensity(intesity) {}

	virtual void update(const double& time);
	virtual void play();
};