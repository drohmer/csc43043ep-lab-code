#include "image_animation.hpp"

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
    set_pixel(int(x), int(y), color);
}

void image_animation_structure::set_rectangle(float x_center, float y_center, float width, float height, cgp::vec3 const& color)
{
    int x_start = int(x_center - width / 2);
    int x_end = int(x_center + width / 2);
    int y_start = int(y_center - height / 2);
    int y_end = int(y_center + height / 2);

    for (int x = x_start; x <= x_end; ++x) {
        for (int y = y_start; y <= y_end; ++y) {
            set_pixel(float(x), float(y), color);
        }
    }
}
void image_animation_structure::set_triangle(float x1, float y1, float x2, float y2, float x3, float y3, cgp::vec3 const& color)
{
    // Find bounding box
    int min_x = int(std::min({x1, x2, x3}));
    int max_x = int(std::max({x1, x2, x3}));
    int min_y = int(std::min({y1, y2, y3}));
    int max_y = int(std::max({y1, y2, y3}));

    // Compute denominator once (constant for the triangle)
    float denominator = (y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3);
    if (std::abs(denominator) < 1e-6f) return; // Degenerate triangle

    // Iterate over bounding box
    for (int y = min_y; y <= max_y; ++y) {
        for (int x = min_x; x <= max_x; ++x) {
            // Compute barycentric coordinates
            float lambda1 = ((y2 - y3) * (x - x3) + (x3 - x2) * (y - y3)) / denominator;
            float lambda2 = ((y3 - y1) * (x - x3) + (x1 - x3) * (y - y3)) / denominator;
            float lambda3 = 1.0f - lambda1 - lambda2;

            // Check if point is inside triangle
            if (lambda1 >= 0 && lambda2 >= 0 && lambda3 >= 0) {
                set_pixel(x, y, color);
            }
        }
    }
}
void image_animation_structure::set_disc(float x_center, float y_center, float radius, cgp::vec3 const& color)
{
    int x_start = int(x_center - radius - 1);
    int x_end = int(x_center + radius + 1);
    int y_start = int(y_center - radius - 1);
    int y_end = int(y_center + radius + 1);

    for (int x = x_start; x <= x_end; ++x) {
        for (int y = y_start; y <= y_end; ++y) {
            if (x < 0 || x >= data.dimension.x || y < 0 || y >= data.dimension.y)
                continue;

            float dx = x - x_center;
            float dy = y - y_center;
            float distance = std::sqrt(dx * dx + dy * dy);

            // Anti-aliasing: smooth transition over 1 pixel at the edge
            float alpha = cgp::clamp(radius - distance + 0.5f, 0.0f, 1.0f);

            if (alpha > 0.0f) {
                vec3 background = data(x, y);
                vec3 blended = alpha * color + (1.0f - alpha) * background;
                data(x, y) = blended;
            }
        }
    }
}
