#!/usr/bin/env python3
"""Generate mystery PPM images for the steganography exercise."""

from pathlib import Path
from PIL import Image

ASSETS = Path(__file__).parent / "assets"
PRIVATE = ASSETS / "_private"

# ── Bitmap font (5x7, same as bitmap_font.cpp) ──────────────────────

GLYPHS = {
    'A': [0x70,0x88,0x88,0xF8,0x88,0x88,0x88],
    'B': [0xF0,0x88,0x88,0xF0,0x88,0x88,0xF0],
    'C': [0x70,0x88,0x80,0x80,0x80,0x88,0x70],
    'D': [0xF0,0x88,0x88,0x88,0x88,0x88,0xF0],
    'E': [0xF8,0x80,0x80,0xF0,0x80,0x80,0xF8],
    'F': [0xF8,0x80,0x80,0xF0,0x80,0x80,0x80],
    'G': [0x70,0x88,0x80,0xB8,0x88,0x88,0x70],
    'H': [0x88,0x88,0x88,0xF8,0x88,0x88,0x88],
    'I': [0x70,0x20,0x20,0x20,0x20,0x20,0x70],
    'J': [0x38,0x10,0x10,0x10,0x10,0x90,0x60],
    'K': [0x88,0x90,0xA0,0xC0,0xA0,0x90,0x88],
    'L': [0x80,0x80,0x80,0x80,0x80,0x80,0xF8],
    'M': [0x88,0xD8,0xA8,0x88,0x88,0x88,0x88],
    'N': [0x88,0xC8,0xA8,0x98,0x88,0x88,0x88],
    'O': [0x70,0x88,0x88,0x88,0x88,0x88,0x70],
    'P': [0xF0,0x88,0x88,0xF0,0x80,0x80,0x80],
    'Q': [0x70,0x88,0x88,0x88,0xA8,0x90,0x68],
    'R': [0xF0,0x88,0x88,0xF0,0xA0,0x90,0x88],
    'S': [0x70,0x88,0x80,0x70,0x08,0x88,0x70],
    'T': [0xF8,0x20,0x20,0x20,0x20,0x20,0x20],
    'U': [0x88,0x88,0x88,0x88,0x88,0x88,0x70],
    'V': [0x88,0x88,0x88,0x88,0x88,0x50,0x20],
    'W': [0x88,0x88,0x88,0x88,0xA8,0xD8,0x88],
    'X': [0x88,0x88,0x50,0x20,0x50,0x88,0x88],
    'Y': [0x88,0x88,0x50,0x20,0x20,0x20,0x20],
    'Z': [0xF8,0x08,0x10,0x20,0x40,0x80,0xF8],
    '0': [0x70,0x88,0x98,0xA8,0xC8,0x88,0x70],
    '1': [0x20,0x60,0x20,0x20,0x20,0x20,0x70],
    '2': [0x70,0x88,0x08,0x10,0x20,0x40,0xF8],
    '3': [0xF8,0x10,0x20,0x10,0x08,0x88,0x70],
    '4': [0x10,0x30,0x50,0x90,0xF8,0x10,0x10],
    '5': [0xF8,0x80,0xF0,0x08,0x08,0x88,0x70],
    '6': [0x30,0x40,0x80,0xF0,0x88,0x88,0x70],
    '7': [0xF8,0x08,0x10,0x20,0x40,0x40,0x40],
    '8': [0x70,0x88,0x88,0x70,0x88,0x88,0x70],
    '9': [0x70,0x88,0x88,0x78,0x08,0x10,0x60],
    ' ': [0x00,0x00,0x00,0x00,0x00,0x00,0x00],
    '.': [0x00,0x00,0x00,0x00,0x00,0x60,0x60],
    '!': [0x20,0x20,0x20,0x20,0x20,0x00,0x20],
    '?': [0x70,0x88,0x08,0x10,0x20,0x00,0x20],
    '-': [0x00,0x00,0x00,0xF8,0x00,0x00,0x00],
    '_': [0x00,0x00,0x00,0x00,0x00,0x00,0xF8],
    ':': [0x00,0x60,0x60,0x00,0x60,0x60,0x00],
    '+': [0x00,0x20,0x20,0xF8,0x20,0x20,0x00],
}
DEFAULT_GLYPH = [0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8]


def get_glyph(c):
    if c.islower():
        c = c.upper()
    return GLYPHS.get(c, DEFAULT_GLYPH)


def draw_text_on_binary(bitmap, w, h, text, start_x, start_y):
    """Draw text on a binary dict bitmap (same logic as C++ draw_text).
    bitmap is a dict: (x, y) -> True for lit pixels."""
    cursor_x = start_x
    for c in text:
        gl = get_glyph(c)
        for row in range(7):
            for col in range(5):
                lit = (gl[row] >> (7 - col)) & 1
                if lit:
                    px = cursor_x + col
                    py = start_y + row
                    if 0 <= px < w and 0 <= py < h:
                        bitmap[(px, py)] = True
        cursor_x += 6


def write_ppm(path, pixels, w, h):
    """Write RGB pixel data as ASCII PPM (P3).
    pixels is a flat bytearray of R,G,B values."""
    with open(path, "w") as f:
        f.write(f"P3\n{w} {h}\n255\n")
        for i in range(0, len(pixels), 3):
            f.write(f"{pixels[i]} {pixels[i+1]} {pixels[i+2]}\n")


def load_image_as_bytes(path, target_size=None):
    """Load image and return (bytearray of RGB, width, height)."""
    img = Image.open(path).convert("RGB")
    if target_size:
        img = img.resize(target_size, Image.LANCZOS)
    w, h = img.size
    data = bytearray(img.tobytes())
    return data, w, h


def get_pixel(data, w, x, y, c):
    """Get channel c (0=R,1=G,2=B) of pixel (x,y)."""
    return data[3 * (y * w + x) + c]


def set_pixel(data, w, x, y, c, val):
    """Set channel c of pixel (x,y)."""
    data[3 * (y * w + x) + c] = val


def embed_watermark(host_data, w, h, watermark_set, channel, bit):
    """Embed binary watermark into a specific bit plane.
    watermark_set is a set of (x,y) tuples for lit pixels."""
    result = bytearray(host_data)
    for y in range(h):
        for x in range(w):
            val = get_pixel(result, w, x, y, channel)
            if (x, y) in watermark_set:
                val = val | (1 << bit)
            else:
                val = val & ~(1 << bit)
            set_pixel(result, w, x, y, channel, val)
    return result


def embed_color_image(host_data, secret_data, w, h, num_bits):
    """Embed secret image into host using num_bits LSBs per channel."""
    result = bytearray(host_data)
    mask = (0xFF << num_bits) & 0xFF
    for y in range(h):
        for x in range(w):
            for c in range(3):
                hv = get_pixel(result, w, x, y, c)
                sv = get_pixel(secret_data, w, x, y, c)
                secret_bits = sv >> (8 - num_bits)
                hv = (hv & mask) | secret_bits
                set_pixel(result, w, x, y, c, hv)
    return result


def main():
    print("Loading source images...")
    img1_data, w, h = load_image_as_bytes(PRIVATE / "image-1.jpg")
    print(f"  image-1: {w}x{h}")

    img2_data, _, _ = load_image_as_bytes(PRIVATE / "image-2.jpg", target_size=(w, h))
    print(f"  image-2: resized to {w}x{h}")

    # ── Generate image-1.ppm and image-2.ppm ──
    print("Writing image-1.ppm and image-2.ppm...")
    write_ppm(ASSETS / "image-1.ppm", img1_data, w, h)
    write_ppm(ASSETS / "image-2.ppm", img2_data, w, h)

    # ── Generate image-mystery-01.ppm ──
    # Hidden message in blue channel (2), bit 1
    print("Generating image-mystery-01.ppm (hidden text message)...")
    message = "Hidden message: I like to work on C++ in CSC_43043_EP !"
    watermark = {}
    draw_text_on_binary(watermark, w, h, message, 10, 60)
    mystery1 = embed_watermark(img1_data, w, h, watermark, channel=2, bit=1)
    write_ppm(ASSETS / "image-mystery-01.ppm", mystery1, w, h)

    # ── Generate image-mystery-02.ppm ──
    # image-2 hidden in image-1 using 3 LSBs per channel
    print("Generating image-mystery-02.ppm (hidden color image)...")
    mystery2 = embed_color_image(img1_data, img2_data, w, h, num_bits=3)
    write_ppm(ASSETS / "image-mystery-02.ppm", mystery2, w, h)

    print("Done! Generated files in", ASSETS)


if __name__ == "__main__":
    main()
