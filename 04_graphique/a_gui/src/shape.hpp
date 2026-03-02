#pragma once

#include "cgp/cgp.hpp"
#include "image_animation.hpp"
#include <string>

using cgp::vec3;
using cgp::vec2;
/**
 * TO DO: Complete the shape structures below, with
 * - A generic base structure "shape_generic" containing common attributes
 * - A rectangle shape "shape_rectangle" derived from "shape_generic" (center, width, height)
 * - A circle shape "shape_circle" derived from "shape_generic" (center, radius)
 * - A triangle shape "shape_triangle" derived from "shape_generic" (vertices p1, p2, p3)
 * 
 * The generic shape should support the virtual method .draw(image_animation_structure& img)
 * 
  * 
 */
struct shape_generic {
    vec3 color = {1.0f, 1.0f, 1.0f};
    std::string name = "no-name";
    vec2 translation = {0.0f, 0.0f};
    float scaling = 1.0f;

    // Virtual function to draw the shape from a shape_generic pointer or reference
    virtual void draw(image_animation_structure& img) = 0;

    // default virtual destructor for virtual class
    virtual ~shape_generic() = default; 
};


struct shape_rectangle : shape_generic {
    // fill the content
};


struct shape_circle : shape_generic {
    // fill the content
};


struct shape_triangle : shape_generic {
    // fill the content
};
