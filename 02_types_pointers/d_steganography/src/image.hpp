#pragma once

#include <vector>
#include <string>
#include <cstdint>


// ============================================================
//  Image structure
// ============================================================

struct image_structure {
    int Nx = 0;
    int Ny = 0;
    std::vector<float> data; // row-major, 3 floats per pixel (R, G, B), values in [0,1]

    void create_image(int Nx_arg, int Ny_arg) {
        Nx = Nx_arg;
        Ny = Ny_arg;
        data.resize(3 * Nx * Ny, 0.0f);
    }

    void set_pixel(int x, int y, float r, float g, float b) {
        if (x < 0 || x >= Nx || y < 0 || y >= Ny)
            return;
        int offset = 3 * (y * Nx + x);
        data[offset + 0] = r;
        data[offset + 1] = g;
        data[offset + 2] = b;
    }

    // Return the value of channel c (0=R, 1=G, 2=B) of pixel (x,y) as uint8
    uint8_t get_channel_uint8(int x, int y, int c) const;

    // Set channel c (0=R, 1=G, 2=B) of pixel (x,y) from uint8
    void set_channel_uint8(int x, int y, int c, uint8_t value);
};


// ============================================================
//  PPM export
// ============================================================

void export_ppm(image_structure const& img, std::string const& filename);
image_structure import_ppm(std::string const& filename);


// ============================================================
//  Image generators
// ============================================================

void colored_disk(image_structure& img, int R, float r, float g, float b);
void gradient(image_structure& img);
