#include "Particle.h"

Particle::Particle()
{
}

Particle::Particle(bool active, Vec3<double> pos)
{
	_collision = false;
	_active = active;
	_pos = pos;
}

Particle::~Particle()
{
	_collision = false;
}
