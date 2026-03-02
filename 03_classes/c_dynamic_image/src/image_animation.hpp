#pragma once

#include "cgp/cgp.hpp"

// Structure of image, compatible with a dynamic display
// Note. The display is handled automatically in the files scene.cpp (and don't have to be modified)
struct image_animation_structure {

    // The data of the image stored in a 2D grid of vec3 (RGB color)
    cgp::grid_2D<cgp::vec3> data;

    image_animation_structure();

    // Set a pixel (in int or float coordinates) to a given color
    void set_pixel(float x, float y, cgp::vec3 const& color);
    void set_pixel(int x, int y, cgp::vec3 const& color);

    // Fill a rectangle with a given color (defined by two opposite corner points)
    void set_rectangle(float x1, float y1, float x2, float y2, cgp::vec3 const& color);

    // Fill a triangle with a given color
    void set_disc(float x_center, float y_center, float radius, cgp::vec3 const& color);

    // Fill a triangle with a given color
    void set_triangle(float x1, float y1, float x2, float y2, float x3, float y3, cgp::vec3 const& color);

};


// Initialization function of the image_animation structure
//  This function is called only once at the beginning of the program
void image_animation_initialization(image_animation_structure& img);

// Display frame function of the image_animation structure
//  This function is called at each frame to update the content of the image.
void image_animation_display_frame(image_animation_structure& img, float time);

