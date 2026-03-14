#pragma once

#include "image.hpp"

#include <string>
#include <cstdint>

// ============================================================
//  Bitmap font 5x7 — uppercase letters, digits, space, punctuation
// ============================================================

// Each character is a 5-wide x 7-tall bitmap stored as 7 bytes (1 byte per row, MSB = left).
// Only the 5 most significant bits of each byte are used.
struct glyph {
    uint8_t rows[7];
};

// Returns the glyph for a printable ASCII character.
// Unknown characters map to a filled block.
glyph get_glyph(char c);

// Draw a string at position (start_x, start_y) in the image.
// Each lit pixel of the glyph is written with color (r,g,b).
// Spacing: 1 pixel between characters.
void draw_text(image_structure& img, std::string const& text,
               int start_x, int start_y,
               float r, float g, float b);
