#pragma once

#include "cgp/cgp.hpp"


struct terrain_structure {
	int N = 100;          // number of samples per direction
	float length = 20.0f; // terrain size in world coordinates [-length/2, length/2]

	// Convert normalized coordinate u in [0,1] to world coordinate in [-length/2, length/2]
	float to_world(float u) const;
	// Convert world coordinate to normalized coordinate in [0,1]
	float to_normalized(float x) const;

	// Evaluate height at world coordinates (x,y)
	float evaluate_height(float x, float y) const;
	// Evaluate height at normalized coordinates (u,v) in [0,1]
	float evaluate_height_normalized(float u, float v) const;

	// Create the terrain mesh using current parameters
	cgp::mesh create_mesh() const;
};
