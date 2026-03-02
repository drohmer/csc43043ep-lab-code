#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "helper.hpp"


// Container for parsed image data: titles and corresponding images
struct structured_data {
    std::vector<std::string> titles;
    std::vector<image_structure> images;
};

/*
 * =============================================================================
 * FUNCTION: parse_data
 * =============================================================================
 *
 * Parses binary data from a void* buffer to extract image information.
 *
 *
 * Binary format:
 * - uint8_t:  Number of images N
 * For each image:
 * - uint8_t:  Size of title string (bytes, includes null terminator)
 * - char[]:   Title string (null-terminated)
 * - uint16_t: Width (little-endian)
 * - uint16_t: Height (little-endian)
 * - uint8_t:  Data format (0=uint8_t, 1=float, 2=double)
 * - pixels:   width * height * 3 components, format as specified
 */
structured_data parse_data(void* data)
{
    structured_data result;
    // TODO: Implement parsing logic here
    //
    // Reminder: static_cast<uint8_t*>(data) to cast void* to byte pointer
    // memcpy can be used to read multi-byte values (uint16_t, float, double)
    //  ex. uint16_t width; memcpy(&width, ptr, sizeof(uint16_t));
    // ...
return result;
}




/*
 * =============================================================================
 * MAIN FUNCTION
 * =============================================================================
 */
int main() {

    // -------------------------------------------------------------------------
    // Step 1: Load data file into a buffer
    // -------------------------------------------------------------------------
    std::cout << "Loading data file..." << std::endl;
    size_t buffer_size = 0;
    void* buffer = read_data_file_as_buffer("data/data.dat", buffer_size);

    if (buffer == nullptr) {
        std::cerr << "Failed to load data file" << std::endl;
        return 1;
    }

    // -------------------------------------------------------------------------
    // Step 2: Parse images from the buffer
    // -------------------------------------------------------------------------
    std::cout << "\nParsing data..." << std::endl;
    structured_data parsed = parse_data(buffer);

    // -------------------------------------------------------------------------
    // Step 3: Export each image as PPM file
    // -------------------------------------------------------------------------
    std::cout << "\nExporting images..." << std::endl;

    for (size_t i = 0; i < parsed.images.size(); ++i) {
        std::cout << "Image " << (i + 1) << ": " << parsed.titles[i] << std::endl;

        std::string filename = "output_" + std::to_string(i + 1) + ".ppm";
        export_ppm(parsed.images[i], filename);
        std::cout << "  Saved as: " << filename << std::endl;
    }

    // -------------------------------------------------------------------------
    // Step 4: Clean up - free the buffer
    // -------------------------------------------------------------------------
    // Must cast void* back to uint8_t* before delete[]
    delete[] static_cast<uint8_t*>(buffer);

    std::cout << "\nDone!" << std::endl;
    return 0;
}
