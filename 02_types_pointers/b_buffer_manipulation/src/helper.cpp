#include "helper.hpp"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>


void image_structure::create_image(int Nx_arg, int Ny_arg) {
    // Set dimensions and allocate storage for 3 components per pixel.
    // After this call, `data.size() == 3 * Nx * Ny`.
    Nx = Nx_arg;
    Ny = Ny_arg;
    data.resize(3 * Nx * Ny, 1.0f); // Initialize to white (1.0f)
}

void image_structure::set_pixel(int x, int y, float r, float g, float b) {
    // Bounds-check: silently ignore out-of-range writes.
    if (x < 0 || x >= Nx || y < 0 || y >= Ny)
        return; // Out of bounds

    // Compute offset into `data` for pixel (x,y): 3 components per pixel
    // offset = 3 * (row_index * width + column_index)
    int offset = 3 * (y * Nx + x);
    data[offset + 0] = r; // Red
    data[offset + 1] = g; // Green
    data[offset + 2] = b; // Blue
}



/*
 * =============================================================================
 * FUNCTION: read_data_file_as_buffer
 * =============================================================================
 *
 * Opens and reads a single file into a contiguous buffer.
 */
void* read_data_file_as_buffer(const std::string &filepath, size_t &out_size)
{
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << filepath << std::endl;
        out_size = 0;
        return nullptr;
    }

    // Get file size
    size_t total_size = static_cast<size_t>(file.tellg());
    file.seekg(0, std::ios::beg);

    if (total_size == 0) {
        std::cerr << "File is empty: " << filepath << std::endl;
        out_size = 0;
        return nullptr;
    }

    // Allocate buffer using new[] - returns uint8_t*, stored as void*
    uint8_t* buffer = new uint8_t[total_size];
    out_size = total_size;

    // Read into temporary char buffer, then copy to our uint8_t buffer
    char* temp_buffer = new char[total_size];
    file.read(temp_buffer, total_size);
    file.close();
    memcpy(buffer, temp_buffer, total_size);
    delete[] temp_buffer;

    std::cout << "Loaded " << total_size << " bytes from " << filepath << std::endl;

    // Return as void* - generic pointer to raw memory
    return static_cast<void*>(buffer);
}


/*
 * =============================================================================
 * FUNCTION: export_ppm
 * =============================================================================
 *
 * Exports an image_structure to a PPM file (P3 ASCII format).
 */
void export_ppm(const image_structure& img, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open file for writing: " << filename << std::endl;
        return;
    }

    // Write PPM header (P3 = ASCII RGB)
    file << "P3\n" << img.Nx << " " << img.Ny << "\n255\n";

    // Write pixel data as ASCII values, converting float [0,1] to int [0,255]
    int values_per_line = 0;
    for (size_t i = 0; i < img.data.size(); ++i) {
        float val = img.data[i];
        val = std::max(0.0f, std::min(1.0f, val));  // Clamp to [0,1]
        int int_val = static_cast<int>(val * 255.0f + 0.5f);

        file << int_val;
        values_per_line++;

        // Add newline every 15 values for readability, otherwise space
        if (values_per_line >= 15) {
            file << "\n";
            values_per_line = 0;
        } else {
            file << " ";
        }
    }

    file.close();
}
