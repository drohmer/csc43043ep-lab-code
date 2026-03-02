#pragma once

#include "cgp/cgp.hpp"

using cgp::vec3;
using cgp::mat4;

struct renderer_structure {

    renderer_structure();
    renderer_structure(int Nx, int Ny);

    void set_pixel(float x, float y, cgp::vec3 const& color);
    void set_pixel(int x, int y, cgp::vec3 const& color);

    // Project a 3D point to screen space
    vec3 project(vec3 const& p) const;

    // Compute the bounding box of a triangle, clipped to image dimensions
    void compute_bounding_box(vec3 const& q1, vec3 const& q2, vec3 const& q3,
                              int& x_min, int& x_max, int& y_min, int& y_max) const;
    void draw_triangle(vec3 const& p1, vec3 const& p2, vec3 const& p3, vec3 const& n1, vec3 const& n2, vec3 const& n3, cgp::vec3 const& color);
    void clear();

    cgp::grid_2D<cgp::vec3> image_data;
    vec3 light_position = vec3{ 0.0f, 5.0f, 5.0f };
    mat4 perspective_matrix;
    mat4 view_matrix;
    vec3 background_color = vec3{ 1.0f, 1.0f, 1.0f };
    cgp::grid_2D<float> depth_buffer;
};


