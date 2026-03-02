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
