#include "particles.hpp"

using namespace cgp;
particle_bubble::particle_bubble(float creation_time)
{
	t0 = creation_time;
	float const theta = rand_uniform(0.0f, 2 * Pi);
	float const radius_position = std::sqrt(rand_uniform(0.0f, 0.7f*0.7f));
	p0 = radius_position * vec3(std::cos(theta), 0.25f, std::sin(theta));
	radius = std::sqrt(rand_uniform(0, 0.08f*0.08f));
	color = { 0.5f + rand_uniform(0,0.2f),0.6f + rand_uniform(0,0.2f),1.0f - rand_uniform(0,0.2f) };
}

// Evaluate the position of the bubble at a given time
//  absolute_time is the current time since the beginning of the program
vec3 particle_bubble::evaluate_position(float absolute_time) const
{
	float t = absolute_time - t0;
	// TO DO: Modify the following line ...
	return { std::sin(3 * t), t, 0.0f };
}

// Initialize the position of the billboard at the time of its creation
particle_billboard::particle_billboard(float creation_time)
{
	t0 = creation_time;
	// To be completed ...
}

// Evaluate the position of the billboard at a given time
vec3 particle_billboard::evaluate_position(float absolute_time) const
{
	// To be modified
	return { 0,0,0 };
}


template <typename T>
static void remove_old_element(std::vector<T>& container, float current_time, float max_time)
{
	// Loop over all active particles
	for (auto it = container.begin(); it != container.end();)
	{
		// if a particle is too old, remove it
		if (current_time - it->t0 > max_time)
			it = container.erase(it);

		// Go to the next particle if we are not already on the last one
		if (it != container.end())
			++it;
	}
}


void particle_system_structure::remove_old_particles(float t)
{
	remove_old_element(bubbles, t, 3.0f);
	remove_old_element(billboards, t, 3.0f);
}