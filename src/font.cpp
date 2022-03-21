#include "font.hpp"
#include "opengl.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#define NUM_GLYPHS 128

Font::Font(const FontFace& face, int size) {
    FT_Set_Pixel_Sizes(face, 0, size);
    const auto& glyph = face()->glyph;
    //FT_Set_Char_Size(face, 0, size << 6, 48, 48);

    metrics = 1 + (face()->size->metrics.height >> 6);
    glm::ivec2 o{0, 0};
    int maxDim = metrics * std::ceil(std::sqrt(NUM_GLYPHS / 2));

    width = 1;
    while (width < maxDim) width <<= 1;
    height = width;

    std::vector<unsigned char> pixels(width * height);

    for (unsigned char c = 32; c < NUM_GLYPHS; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_LIGHT)) {
            std::cerr << "ERROR: Failed to load glyph '" << c << "'" << std::endl;
            continue;
        }

        FT_Bitmap* bmp = &glyph->bitmap;

        if (o.x + bmp->width >= width) {
            o.x = 0;
            o.y += metrics;
        }

        for (int row = 0; row < bmp->rows; ++row) {
            for (int col = 0; col < bmp->width; ++col) {
                int x = o.x + col;
                int y = o.y + row;
                pixels[y * width + x] = bmp->buffer[row * bmp->pitch + col];
            }
        }

        glyphs.emplace(c, Glyph {
            {glyph->advance.x >> 6, glyph->advance.y >> 6},
            {glyph->bitmap.width, glyph->bitmap.rows},
            {glyph->bitmap_left, glyph->bitmap_top},
            {o.x / static_cast<float>(width), o.y / static_cast<float>(height)}
        });

        o.x += bmp->width + 1;
    }

    glCall(glGenTextures, 1, &textureId);
    glCall(glBindTexture, GL_TEXTURE_2D, textureId);
    glCall(glPixelStorei, GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    glCall(glTexImage2D, GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, pixels.data());

    glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    std::string path {"resources/fonts/" + face.getName() + ".png"};
    stbi_write_png(path.c_str(), width, height, 1, pixels.data(), width * 1);

    std::cout << "Generated a " << width << "x " << height << " (" << width * height / 1024 << " kb) texture atlas." << std::endl;
}

Font::~Font() {
    glCall(glDeleteTextures, 1, &textureId);
}

void Font::bind() const {
    glCall(glActiveTexture, GL_TEXTURE0);
    glCall(glBindTexture, GL_TEXTURE_2D, textureId);
}

void Font::unbind() const {
    glCall(glBindTexture, GL_TEXTURE_2D, 0);
}