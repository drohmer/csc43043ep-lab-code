#pragma once

#include <string>
#include <vector>


/*
 image_structure

 - Fields:
     - `Nx`, `Ny`: image width and height (pixels).
     - `data`: contiguous float array storing pixel components. Components are expected in [0.0, 1.0].

 - Storage layout / indexing:
     Each pixel stores three float components R, G, B in that order. The element index
     for pixel `(x,y)` is computed as `offset = 3 * (y * Nx + x)`, where `0 <= x < Nx`
     and `0 <= y < Ny`.
*/
struct image_structure {
    int Nx; // image width/x in pixels
    int Ny; // image height/y in pixels

    // Pixel data: row-major (x and then y), 3 floats per pixel (R, G, B), values in [0,1]
    std::vector<float> data;

    // Allocate storage for an image of size Nx_arg x Ny_arg, initialized to white
    void create_image(int Nx_arg, int Ny_arg);

    // Set the RGB color of a pixel at position (x, y)
    void set_pixel(int x, int y, float r, float g, float b);
};


// Read a single data file into a buffer.
// Returns a pointer to dynamically allocated memory (must be freed with delete[]).
// The total size is stored in out_size.
void* read_data_file_as_buffer(const std::string &filepath, size_t &out_size);

// Export an image to a PPM file (P3 ASCII format)
void export_ppm(const image_structure& img, const std::string& filename);