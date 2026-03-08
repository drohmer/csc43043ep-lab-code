#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <cstdint>


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
    int Nx = 0; // image width/x in pixels
    int Ny = 0; // image height/y in pixels

    // Pixel data: row-major (x and then y), 3 floats per pixel (R, G, B), values in [0,1]
    std::vector<float> data;

    void create_image(int Nx_arg, int Ny_arg) {
        // Set dimensions and allocate storage for 3 components per pixel.
        // After this call, `data.size() == 3 * Nx * Ny`.
        Nx = Nx_arg;
        Ny = Ny_arg;
        data.resize(3 * Nx * Ny, 1.0f); // Initialize to white (1.0f)
    }

    void set_pixel(int x, int y, float r, float g, float b) {
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
};



struct image_structure_rgb565 {
    int Nx = 0;
    int Ny = 0;
    std::vector<uint16_t> data; // 2 bytes per pixel (R,G,B in 5-6-5 bits)
};

void float_rgb_to_rgb565(float r, float g, float b, uint16_t &rgb565) {
    // Clamp and convert to 5-6-5 bits
    uint16_t r_5 = static_cast<uint16_t>(std::max(0.0f, std::min(1.0f, r)) * 31.0f + 0.5f);
    uint16_t g_6 = static_cast<uint16_t>(std::max(0.0f, std::min(1.0f, g)) * 63.0f + 0.5f);
    uint16_t b_5 = static_cast<uint16_t>(std::max(0.0f, std::min(1.0f, b)) * 31.0f + 0.5f);

    // Pack into RGB565 format
    rgb565 = (r_5 << 11) | (g_6 << 5) | b_5;
}
void rgb565_to_float_rgb(uint16_t rgb565, float &r, float &g, float &b) {
    // Unpack RGB565
    uint16_t r_5 = (rgb565 >> 11) & 0x1F; // & 0b0000000000011111;
    uint16_t g_6 = (rgb565 >> 5) & 0x3F;  // & 0b0000000000111111;
    uint16_t b_5 = rgb565 & 0x1F;         // & 0b0000000000011111;

    // Convert to float [0,1]
    r = static_cast<float>(r_5) / 31.0f;
    g = static_cast<float>(g_6) / 63.0f;
    b = static_cast<float>(b_5) / 31.0f;
}

void convert_float_rgb_to_rgb565(const image_structure &img_float, image_structure_rgb565 &img_rgb565) {
    // TODO: Convert float RGB image to RGB565
    // - Set dimensions (Nx, Ny) and resize img_rgb565.data to Nx*Ny
    // - For each pixel (x,y), read R,G,B floats from img_float.data
    // - Use float_rgb_to_rgb565() helper to convert and store in img_rgb565.data
}
void convert_rgb565_to_float_rgb(const image_structure_rgb565 &img_rgb565, image_structure &img_float) {
    // TODO: Convert RGB565 image back to float RGB
    // - Set dimensions (Nx, Ny) and resize img_float.data to 3*Nx*Ny
    // - For each pixel (x,y), read the uint16_t from img_rgb565.data
    // - Use rgb565_to_float_rgb() helper to convert and store in img_float.data
}




int to_int255(float x) {
    // Clamp to [0,1] then scale to [0,255]
    return static_cast<int>(std::max(0.0f, std::min(1.0f, x)) * 255.0f + 0.5f);
}


/*
 Function export_ppm

 Purpose:
     Produce a PPM image file from an in-memory `image_structure`.

 Signature:
     void export_ppm(const image_structure &img, const std::string &filename);

 Parameters:
     - img: `image_structure` with fields `Nx`, `Ny`, and `data` (row-major, 3 floats per pixel: R,G,B in [0,1]).
     - filename: output file path. Existing files will be overwritten.

 Behavior:
     - Opens `filename`.
     - Writes ASCII PPM (P3) header and pixel triples. Pixels are emitted row-by-row with
         y iterated from `Ny-1` down to `0` (top row first).
     - Each float component is clamped to [0,1], scaled to [0,255], and rounded to nearest int.

 Format (typical P3 PPM example):
     P3
     <width> <height>
     <maxval>
     r g b r g b r g b ...   (pixel triples in ASCII, whitespace separated; rows may be line-broken)

 Example (2x2 image):
     P3
     2 2
     255
     255 0 0  0 255 0
     0 0 255  255 255 0

*/
void export_ppm(const image_structure &img, const std::string &filename) {

    std::ofstream out(filename);

    if (!out) {
        throw std::runtime_error("export_ppm: failed to open output file: " + filename);
    }

    // PPM header (P3 = ASCII RGB)
    out << "P3\n" << img.Nx << " " << img.Ny << "\n255\n";

    // Write pixels row by row
    // Top-to-bottom (y=0 at top). If you want bottom-to-top, flip the loop.
    for (int y = img.Ny-1; y >= 0; --y) {
        for (int x = 0; x < img.Nx; ++x) {
            const size_t offset = 3 * (y * img.Nx + x);
            int r = to_int255(img.data[offset + 0]);
            int g = to_int255(img.data[offset + 1]);
            int b = to_int255(img.data[offset + 2]);
            out << r << " " << g << " " << b << " ";
        }
        out << "\n";
    }

}   


// Generate a horizontal gradient from black (x=0) to white (x=Nx-1)
void gradient(image_structure &img) {
    if (img.Nx <= 1) {
        // Degenerate: fill with black
        for (int y = 0; y < img.Ny; ++y)
            for (int x = 0; x < img.Nx; ++x)
                img.set_pixel(x, y, 0.0f, 0.0f, 0.0f);
        return;
    }

    for (int y = 0; y < img.Ny; ++y) {
        for (int x = 0; x < img.Nx; ++x) {
            float v = float(x) / float(img.Nx - 1);
            img.set_pixel(x, y, v, v, v);
        }
    }
}

// Create a checkerboard pattern. `s` is the cell size in pixels.
void checkerboard(image_structure &img) {
    const int s = 8; // cell size (adjustable)
    for (int y = 0; y < img.Ny; ++y) {
        for (int x = 0; x < img.Nx; ++x) {
            int cell = (x / s + y / s) & 1; // 0 or 1
            if (cell == 0)
                img.set_pixel(x, y, 0.0f, 0.0f, 0.0f); // black
            else
                img.set_pixel(x, y, 1.0f, 1.0f, 1.0f); // white
        }
    }
}

void random_noise(image_structure &img) {
    for (int y = 0; y < img.Ny; ++y) {
        for (int x = 0; x < img.Nx; ++x) {
            float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            img.set_pixel(x, y, r, g, b);
        }
    }
}

// Draw a colored disk centered in the image. 
void colored_disk(image_structure &img, int R, float r, float g, float b) {
    const int cx = img.Nx / 2;
    const int cy = img.Ny / 2;
    const int R2 = R * R;

    for (int y = 0; y < img.Ny; ++y) {
        for (int x = 0; x < img.Nx; ++x) {
            int dx = x - cx;
            int dy = y - cy;
            int d2 = dx * dx + dy * dy;
            if (d2 <= R2)
                img.set_pixel(x, y, r, g, b);
        }
    }
}


struct rle_element {
    uint16_t value;
    uint16_t count;
};
struct image_structure_rle_encoding_rgb565 {
    int Nx = 0;
    int Ny = 0;
    std::vector<rle_element> data; // RLE encoded data
};

void rle_encode_rgb565(const image_structure_rgb565 &img_rgb565, image_structure_rle_encoding_rgb565 &rle_data) {
    // TODO: Implement RLE encoding
    // - Initialize rle_data dimensions and clear its data
    // - Maintain current_value and current_count
    // - Traverse pixels: if same as current, extend run; otherwise store run and start new one
    // - Don't forget to store the last run
}

void rle_encode_with_threshold_rgb565(const image_structure_rgb565 &img_rgb565, image_structure_rle_encoding_rgb565 &rle_data, float threshold) {
    // TODO: Implement RLE encoding with threshold
    // - Same as rle_encode_rgb565, but extend run when colors are "close enough"
    // - Convert current and next pixel to float RGB, compare per-channel differences against threshold
}
void rle_decode_rgb565(const image_structure_rle_encoding_rgb565 &rle_data, image_structure_rgb565 &img_rgb565) {
    // TODO: Implement RLE decoding
    // - Set dimensions and clear img_rgb565.data
    // - For each rle_element, push back its value 'count' times
}




int main() {

    image_structure img;

    // Generate a gradient image
    img.create_image(128, 128);
    //random_noise(img);
    gradient(img);
    //colored_disk(img, 40, 1.0f, 0.0f, 0.0f);



    image_structure_rgb565 img_rgb565;
    convert_float_rgb_to_rgb565(img, img_rgb565);

    image_structure_rle_encoding_rgb565 rle_data;
    rle_encode_rgb565(img_rgb565, rle_data);

    image_structure_rgb565 img_rgb565_out; 
    image_structure img_out;
    rle_decode_rgb565(rle_data, img_rgb565_out);
    convert_rgb565_to_float_rgb(img_rgb565_out, img_out);

    export_ppm(img, "1-original.ppm");
    export_ppm(img_out, "2-reconstructed.ppm");

    // Compute sizes in bytes
    int size_original = img.data.size() * sizeof(float);
    int size_rgb565 = img_rgb565.data.size() * sizeof(uint16_t);
    int size_rle = rle_data.data.size() * sizeof(rle_element);

    std::cout << "Original image size: " << size_original / 1024.0 << " kB" << std::endl;
    std::cout << "RGB565 image size: " << size_rgb565 / 1024.0 << " kB" << std::endl;
    std::cout << "RLE encoded size: " << size_rle / 1024.0 << " kB" << std::endl;

    // Compression ratios
    std::cout << "\nCompression ratios:" << std::endl;
    std::cout << "  RGB565 vs Original: " << static_cast<double>(size_original) / size_rgb565 << ":1" << std::endl;
    std::cout << "  RLE vs RGB565: " << static_cast<double>(size_rgb565) / size_rle << ":1" << std::endl;
    std::cout << "  RLE vs Original: " << static_cast<double>(size_original) / size_rle << ":1" << std::endl;

    // RLE with threshold
    image_structure_rle_encoding_rgb565 rle_data_thresh;
    rle_encode_with_threshold_rgb565(img_rgb565, rle_data_thresh, 5.0f/255.0f);

    image_structure_rgb565 img_rgb565_out_thresh;
    image_structure img_out_thresh;
    rle_decode_rgb565(rle_data_thresh, img_rgb565_out_thresh);
    convert_rgb565_to_float_rgb(img_rgb565_out_thresh, img_out_thresh);

    export_ppm(img_out_thresh, "3-reconstructed-threshold.ppm");

    int size_rle_thresh = rle_data_thresh.data.size() * sizeof(rle_element);
    std::cout << "\nRLE with threshold size: " << size_rle_thresh / 1024.0 << " kB" << std::endl;
    std::cout << "  RLE-thresh vs RGB565: " << static_cast<double>(size_rgb565) / size_rle_thresh << ":1" << std::endl;
    std::cout << "  RLE-thresh vs Original: " << static_cast<double>(size_original) / size_rle_thresh << ":1" << std::endl;

    return 0;
}