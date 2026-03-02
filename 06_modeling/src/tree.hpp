#pragma once

#include "cgp/cgp.hpp"


struct tree_structure {
	float trunk_height = 0.7f;
	float trunk_radius = 0.1f;

	cgp::mesh create_cylinder_mesh(float radius, float height) const;
	cgp::mesh create_cone_mesh(float radius, float height, float z_offset) const;
};
