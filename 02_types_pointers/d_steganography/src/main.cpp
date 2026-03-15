#include <iostream>

#include "image.hpp"
#include "bitmap_font.hpp"


// ============================================================
//  Part 1: Bit plane extraction
// ============================================================

// Extract a single bit plane from a channel and produce a black/white image.
// Bit = 1 → white pixel, bit = 0 → black pixel.
void extract_bitplane(image_structure const& img,
                      int channel, int bit,
                      image_structure& output)
{
    // TODO: Extract a bit plane from the image
    //
    // 1. Create the output image with same dimensions as img
    //        output.create_image(img.Nx, img.Ny);
    // 2. For each pixel (x,y):
    //    a. Read the channel value as uint8:
    //           uint8_t val = img.get_channel_uint8(x, y, channel);
    //    b. Extract bit number `bit`:
    //           bool is_set = (val >> bit) & 1;
    //    c. If the bit is set, write white (1,1,1); otherwise write black (0,0,0)
}


// ============================================================
//  Part 2: Watermark embedding
// ============================================================

// Embed the watermark image into a specific bit plane of a specific channel.
// watermark pixel (x,y) is considered "lit" if its red channel > 0.5.
// For each lit pixel, the corresponding bit of the host image is set to 1;
// for each unlit pixel, the bit is set to 0.
void embed_watermark(image_structure& host,
                     image_structure const& watermark,
                     int channel, int bit)
{
    // TODO: Embed the watermark into the host image
    //
    // For each pixel (x,y) that is within both images:
    //   1. Read the host pixel's channel value as uint8:
    //          uint8_t val = host.get_channel_uint8(x, y, channel);
    //   2. Check if the watermark pixel is "lit" (red channel > 0.5)
    //   3. If lit:  set bit number `bit` to 1  →  val = val | (1 << bit)
    //      If not:  set bit number `bit` to 0  →  val = val & ~(1 << bit)
    //   4. Write back:
    //          host.set_channel_uint8(x, y, channel, val);
}


// ============================================================
//  Part 3: Color steganography — hide a color image using N bits
// ============================================================

// Extract a color image hidden in the `num_bits` least significant bits
// of each channel. The extracted bits are scaled back to the full [0,255] range.
void extract_color_image(image_structure const& img,
                         int num_bits,
                         image_structure& output)
{
    // TODO: Extract a hidden color image from the num_bits LSBs
    //
    // 1. Create the output image with same dimensions
    // 2. For each pixel (x,y) and each channel c in {0, 1, 2}:
    //    a. Read channel c as uint8: uint8_t val = ...
    //    b. Extract the num_bits LSBs:
    //           uint8_t bits = val & ((1 << num_bits) - 1);
    //    c. Scale to full range:
    //           uint8_t out_val = bits * 255 / ((1 << num_bits) - 1);
    //    d. Write to output channel c
}


// Embed a color image into the host using the `num_bits` least significant
// bits of each channel.
// For each pixel and each channel c (0,1,2):
//   - Take the `num_bits` most significant bits of the secret's channel c
//   - Place them in the `num_bits` least significant bits of the host's channel c
void embed_color_image(image_structure& host,
                       image_structure const& secret,
                       int num_bits)
{
    // TODO: Embed a color image into the host using num_bits LSBs per channel
    //
    // For each pixel (x,y) and each channel c in {0, 1, 2}:
    //   1. Read the host channel c as uint8:  uint8_t h = ...
    //   2. Read the secret channel c as uint8: uint8_t s = ...
    //   3. Extract the num_bits MSBs of the secret:
    //          uint8_t secret_bits = s >> (8 - num_bits);
    //   4. Clear the num_bits LSBs of the host:
    //          uint8_t mask = 0xFF << num_bits;  // e.g. 0xFC for num_bits=2
    //          h = h & mask;
    //   5. Insert the secret bits:
    //          h = h | secret_bits;
    //   6. Write back
}


// ============================================================
//  Main
// ============================================================

int main() {

    // -------------------------------------------------------
    //  Part 1: Decode a mystery image
    // -------------------------------------------------------
    std::cout << "=== Part 1: Decode the mystery ===" << std::endl;

    image_structure mystery = import_ppm("assets/image-mystery-01.ppm");

    // Extract bit planes — find which one contains the hidden message
    image_structure extracted;

    extract_bitplane(mystery, /*channel=*/2, /*bit=*/1, extracted);
    export_ppm(extracted, "01-blue-bit1.ppm");

    extract_bitplane(mystery, /*channel=*/0, /*bit=*/0, extracted);
    export_ppm(extracted, "02-red-bit0.ppm");

    extract_bitplane(mystery, /*channel=*/2, /*bit=*/7, extracted);
    export_ppm(extracted, "03-blue-bit7.ppm");


    // -------------------------------------------------------
    //  Part 2: Embed your own watermark
    // -------------------------------------------------------
    std::cout << "=== Part 2: Embed your own watermark ===" << std::endl;

    image_structure host = import_ppm("assets/image-1.ppm");

    image_structure watermark;
    watermark.create_image(host.Nx, host.Ny);
    draw_text(watermark, "MY SECRET", 10, 60, 1.0f, 1.0f, 1.0f);
    export_ppm(watermark, "04-watermark.ppm");

    embed_watermark(host, watermark, /*channel=*/2, /*bit=*/1);
    export_ppm(host, "05-watermarked.ppm");

    extract_bitplane(host, /*channel=*/2, /*bit=*/1, extracted);
    export_ppm(extracted, "06-extracted-verify.ppm");


    // -------------------------------------------------------
    //  Part 3: Color steganography (3 bits per channel)
    // -------------------------------------------------------
    std::cout << "=== Part 3: Color steganography ===" << std::endl;

    // First: extract the hidden image from the pre-encoded mystery
    image_structure mystery2 = import_ppm("assets/image-mystery-02.ppm");
    extract_color_image(mystery2, /*num_bits=*/3, extracted);
    export_ppm(extracted, "07-extracted-from-mystery.ppm");

    // Then: embed image-to-encode inside image-1 yourself
    image_structure host2 = import_ppm("assets/image-1.ppm");
    image_structure secret = import_ppm("assets/image-to-encode.ppm");

    embed_color_image(host2, secret, /*num_bits=*/3);
    export_ppm(host2, "08-watermarked-color.ppm");

    extract_color_image(host2, /*num_bits=*/3, extracted);
    export_ppm(extracted, "09-extracted-color.ppm");


    std::cout << std::endl;
    std::cout << "Done. Check the generated .ppm images." << std::endl;

    return 0;
}
