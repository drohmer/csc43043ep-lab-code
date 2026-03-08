#include "image_animation.hpp"

#include <algorithm>
#include <cmath>

using namespace cgp;


image_animation_structure::image_animation_structure()
    :data()
{}

void image_animation_structure::set_pixel(int x, int y, cgp::vec3 const& color)
{
    if (x >= 0 && x < data.dimension.x && y >= 0 && y < data.dimension.y)
        data(x, y) = color;
}

void image_animation_structure::set_pixel(float x, float y, cgp::vec3 const& color)
{
    set_pixel(int(x+0.5f), int(y+0.5f), color);
}
void image_animation_structure::set_rectangle(float x1, float y1, float x2, float y2, cgp::vec3 const& color)
{
    // TODO: Fill all pixels inside the rectangle defined by corners (x1,y1) and (x2,y2)
    //  Use set_pixel to set each pixel color
}
void image_animation_structure::set_triangle(float x1, float y1, float x2, float y2, float x3, float y3, cgp::vec3 const& color)
{
    // TODO: Fill all pixels inside the triangle defined by vertices (x1,y1), (x2,y2), (x3,y3)
}
void image_animation_structure::set_disc(float x_center, float y_center, float radius, cgp::vec3 const& color)
{
    // TODO: Fill all pixels inside the disc of center (x_center,y_center) and given radius
}

void image_animation_initialization(image_animation_structure& img)
{
    img.data.resize(32, 32);
    img.data.fill({1.0f, 1.0f, 1.0f});
}



void image_animation_display_frame(image_animation_structure& img, float time)
{
    img.data.fill({1.0f, 1.0f, 1.0f});

    // Example: A moving pixel
    float x = 10*(1+cos(time));
    float y = 10*(1+sin(time));
    float r = 0.5f+0.5f*cos(time/3);
    float g = 0.5f-0.5f*cos(time/3);
    img.set_pixel(x,y,{r,g,0});
}
