#pragma once

struct Image {
    uint8_t* pixels;
    int width;
    int height;
    int channels;

    Image() = delete;
    explicit Image(const std::string& path, bool flip = false);
    ~Image();
};