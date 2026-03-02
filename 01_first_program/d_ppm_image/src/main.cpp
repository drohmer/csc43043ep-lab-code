#include <vector>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <string>
#include <stdexcept>


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

    void create_image(int Nx_arg, int Ny_arg) {
        // Set dimensions and allocate storage for 3 components per pixel.
        // After this call, `data.size() == 3 * Nx * Ny`.
        Nx = Nx_arg;
        Ny = Ny_arg;
        data.clear();
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
     - Writes ASCII PPM (P3) header and pixel triples. Pixels are emitted row-by-row with y iterated from `Ny-1` down to `0` (top row first).
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

    // TO DO: Implement this function
    // ...
    // To create a file in writing mode: std::ofstream out(filename);
    // To write in the file: out << "some text";
    // ...
}   
int main() {

    image_structure img;
    img.create_image(32, 32);
    img.set_pixel(10, 10, 1.0f, 0.0f, 0.0f); // Set pixel at (10,10) to red
    export_ppm(img, "output.ppm");
return 0;
}