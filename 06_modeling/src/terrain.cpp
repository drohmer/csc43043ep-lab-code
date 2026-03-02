
#include "terrain.hpp"

using namespace cgp;


float terrain_structure::to_world(float u) const{
    return (u - 0.5f) * length;
}

float terrain_structure::to_normalized(float x) const
{
    return x / length + 0.5f;
}

float terrain_structure::evaluate_height(float x, float y) const
{
    vec2 p_0 = { 0, 0 };
    float h_0 = 2.0f;
    float sigma_0 = 3.0f;

    float d = norm(vec2(x, y) - p_0) / sigma_0;

    float z = h_0 * std::exp(-d * d);
return z;
}

float terrain_structure::evaluate_height_normalized(float u, float v) const {
    return evaluate_height(to_world(u), to_world(v));
}

mesh terrain_structure::create_mesh() const
{
    mesh terrain;
    terrain.position.resize(N*N);

    // Fill terrain geometry
    for(int ku=0; ku<N; ++ku)
    {
        for(int kv=0; kv<N; ++kv)
        {
            // Compute local parametric coordinates (u,v) in [0,1]
            float u = ku/(N-1.0f);
            float v = kv/(N-1.0f);

            // Compute the real coordinates (x,y) of the terrain
            float x = to_world(u);
            float y = to_world(v);

            // Compute the surface height function at the given sampled coordinate
            float z = evaluate_height(x, y);

            // Store vertex coordinates
            terrain.position[kv+N*ku] = {x, y, z};
        }
    }

    // Generate triangle organization
    for(int ku=0; ku<N-1; ++ku)
    {
        for(int kv=0; kv<N-1; ++kv)
        {
            unsigned int idx = kv + N*ku;

            uint3 triangle_1 = {idx, idx+1+N, idx+1};
            uint3 triangle_2 = {idx, idx+N, idx+1+N};

            terrain.connectivity.push_back(triangle_1);
            terrain.connectivity.push_back(triangle_2);
        }
    }

    terrain.fill_empty_field();

    return terrain;
}
