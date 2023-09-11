#pragma once
#include "Vec3.h"

class Particle
{
public:
	bool _active;
	bool _collision;
	Vec3<double> _pos;
public:
	Particle();
	Particle(bool active, Vec3<double> pos);
	~Particle();
};

