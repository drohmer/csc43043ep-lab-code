#include "renderer.hpp"

using namespace cgp;



renderer_structure::renderer_structure()
    :image_data(), depth_buffer()
{}

renderer_structure::renderer_structure(int Nx, int Ny)
    :image_data(Nx, Ny), depth_buffer(Nx, Ny)
{}

void renderer_structure::clear()
{
    image_data.fill(background_color);
    depth_buffer.fill(1.0f); // Far plane (depth in NDC is in [-1, 1], 1 is far)
}

void renderer_structure::set_pixel(int x, int y, cgp::vec3 const& color)
{
    if (x >= 0 && x < image_data.dimension.x && y >= 0 && y < image_data.dimension.y)
        image_data(x, y) = color;
}

void renderer_structure::set_pixel(float x, float y, cgp::vec3 const& color)
{
    set_pixel(int(x), int(y), color);
}


// Helper function: compute barycentric coordinates and check if point is inside triangle
// Returns true if the point (x, y) is inside the triangle defined by p1, p2, p3
// Outputs barycentric coordinates (lambda1, lambda2, lambda3) where point = lambda1*p1 + lambda2*p2 + lambda3*p3
bool is_point_in_triangle(float x, float y,
                          vec3 const& p1, vec3 const& p2, vec3 const& p3,
                          float& lambda1, float& lambda2, float& lambda3)
{
    // Edge vectors from p1
    vec2 e1 = vec2(p2.x - p1.x, p2.y - p1.y);
    vec2 e2 = vec2(p3.x - p1.x, p3.y - p1.y);
    vec2 ep = vec2(x - p1.x, y - p1.y);

    // Compute denominator of barycentric formula
    float denom = e1.x * e2.y - e2.x * e1.y;
    if (std::abs(denom) < 1e-6f)
        return false; // Degenerate triangle

    float inv_denom = 1.0f / denom;

    // Compute barycentric coordinates
    lambda2 = (ep.x * e2.y - e2.x * ep.y) * inv_denom;
    lambda3 = (e1.x * ep.y - ep.x * e1.y) * inv_denom;
    lambda1 = 1.0f - lambda2 - lambda3;

    // Point is inside if all barycentric coordinates are non-negative
    return (lambda1 >= 0) && (lambda2 >= 0) && (lambda3 >= 0);
}



// Helper function: Project a 3D point to screen space using the projection matrix
// Returns the projected point in screen coordinates (x, y in pixels, z for depth)
vec3 renderer_structure::project(vec3 const& p) const
{
    // Apply projection matrix (homogeneous coordinates)
    vec4 p_clip = perspective_matrix * view_matrix * vec4(p, 1.0f);

    // Perspective division
    vec3 p_ndc = vec3(p_clip.x, p_clip.y, p_clip.z) / p_clip.w;

    // Convert from NDC [-1, 1] to screen coordinates
    float screen_x = (p_ndc.x + 1.0f) * 0.5f * image_data.dimension.x;
    float screen_y = (p_ndc.y + 1.0f) * 0.5f * image_data.dimension.y;

    return vec3(screen_x, screen_y, p_ndc.z);
}

// Helper function: Compute the bounding box of a triangle in screen space, clipped to image dimensions
void renderer_structure::compute_bounding_box(vec3 const& q1, vec3 const& q2, vec3 const& q3,
                                               int& x_min, int& x_max, int& y_min, int& y_max) const
{
    x_min = std::max(0, int(std::min({q1.x, q2.x, q3.x})));
    x_max = std::min(int(image_data.dimension.x) - 1, int(std::max({q1.x, q2.x, q3.x})));
    y_min = std::max(0, int(std::min({q1.y, q2.y, q3.y})));
    y_max = std::min(int(image_data.dimension.y) - 1, int(std::max({q1.y, q2.y, q3.y})));
}
/*
// TO DO: implement the triangle rasterization with projection
void renderer_structure::draw_triangle(vec3 const& p1, vec3 const& p2, vec3 const& p3, vec3 const& n1, vec3 const& n2, vec3 const& n3, cgp::vec3 const& color)
{
    // 1. Project vertices to screen space

    // 2. Compute bounding box of the triangle

    // 3. Rasterize: iterate over each pixel in the bounding box
    //    - Check if the pixel is inside the triangle using barycentric coordinates
    //    - If inside, set the pixel color

}*/
void renderer_structure::draw_triangle(vec3 const& p1, vec3 const& p2, vec3 const& p3, vec3 const& n1, vec3 const& n2, vec3 const& n3, cgp::vec3 const& color)
{
}
