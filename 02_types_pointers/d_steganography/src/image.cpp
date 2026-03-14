#include "image.hpp"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>


uint8_t image_structure::get_channel_uint8(int x, int y, int c) const {
    int offset = 3 * (y * Nx + x) + c;
    return static_cast<uint8_t>(std::max(0.0f, std::min(1.0f, data[offset])) * 255.0f + 0.5f);
}

void image_structure::set_channel_uint8(int x, int y, int c, uint8_t value) {
    if (x < 0 || x >= Nx || y < 0 || y >= Ny)
        return;
    int offset = 3 * (y * Nx + x) + c;
    data[offset] = static_cast<float>(value) / 255.0f;
}


// ============================================================
//  PPM export
// ============================================================

static int to_int255(float x) {
    return static_cast<int>(std::max(0.0f, std::min(1.0f, x)) * 255.0f + 0.5f);
}

void export_ppm(image_structure const& img, std::string const& filename) {
    std::ofstream out(filename);
    if (!out)
        throw std::runtime_error("export_ppm: failed to open " + filename);

    out << "P3\n" << img.Nx << " " << img.Ny << "\n255\n";
    for (int y = img.Ny - 1; y >= 0; --y) {
        for (int x = 0; x < img.Nx; ++x) {
            int offset = 3 * (y * img.Nx + x);
            out << to_int255(img.data[offset + 0]) << " "
                << to_int255(img.data[offset + 1]) << " "
                << to_int255(img.data[offset + 2]) << " ";
        }
        out << "\n";
    }
}


// ============================================================
//  PPM import
// ============================================================

image_structure import_ppm(std::string const& filename) {
    std::ifstream in(filename);
    if (!in)
        throw std::runtime_error("import_ppm: failed to open " + filename);

    std::string magic;
    in >> magic;
    if (magic != "P3")
        throw std::runtime_error("import_ppm: expected P3 format in " + filename);

    // Skip comments
    char ch;
    while (in.peek() == '\n' || in.peek() == '\r' || in.peek() == ' ')
        in.get(ch);
    while (in.peek() == '#') {
        std::string comment;
        std::getline(in, comment);
    }

    int Nx, Ny, max_val;
    in >> Nx >> Ny >> max_val;

    image_structure img;
    img.create_image(Nx, Ny);

    for (int y = Ny - 1; y >= 0; --y) {
        for (int x = 0; x < Nx; ++x) {
            int r, g, b;
            in >> r >> g >> b;
            int offset = 3 * (y * Nx + x);
            img.data[offset + 0] = static_cast<float>(r) / max_val;
            img.data[offset + 1] = static_cast<float>(g) / max_val;
            img.data[offset + 2] = static_cast<float>(b) / max_val;
        }
    }

    return img;
}


// ============================================================
//  Image generators
// ============================================================

void colored_disk(image_structure& img, int R, float r, float g, float b) {
    int cx = img.Nx / 2;
    int cy = img.Ny / 2;
    int R2 = R * R;
    for (int y = 0; y < img.Ny; ++y)
        for (int x = 0; x < img.Nx; ++x) {
            int dx = x - cx, dy = y - cy;
            if (dx * dx + dy * dy <= R2)
                img.set_pixel(x, y, r, g, b);
        }
}

void gradient(image_structure& img) {
    for (int y = 0; y < img.Ny; ++y)
        for (int x = 0; x < img.Nx; ++x) {
            float v = static_cast<float>(x) / static_cast<float>(img.Nx - 1);
            img.set_pixel(x, y, v, 0.3f + 0.4f * v, 0.8f - 0.3f * v);
        }
}
