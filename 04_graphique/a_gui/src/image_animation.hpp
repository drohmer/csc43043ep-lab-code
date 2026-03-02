#pragma once

#include "cgp/cgp.hpp"

struct image_animation_structure {

    image_animation_structure();
    void set_pixel(float x, float y, cgp::vec3 const& color);
    void set_pixel(int x, int y, cgp::vec3 const& color);
    void set_rectangle(float x_center, float y_center, float width, float height, cgp::vec3 const& color);
    void set_triangle(float x1, float y1, float x2, float y2, float x3, float y3, cgp::vec3 const& color);
    void set_disc(float x_center, float y_center, float radius, cgp::vec3 const& color);
cgp::grid_2D<cgp::vec3> data;
};


