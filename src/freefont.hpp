#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

class FontLibrary {
public:
    FontLibrary() {
        int error = FT_Init_FreeType(&library);
        assert(!error && "Failed to init FreeType");
    }
    ~FontLibrary() {
        FT_Done_FreeType(library);
    }

    const FT_Library& operator()() const { return library; }
    FT_Library& operator()() { return library; }
    operator FT_Library() const { return library; }

private:
    FT_Library library;
};

class FontFace {
public:
    FontFace() = delete;
    FontFace(const FontLibrary& library, const std::string& path) : name{std::filesystem::path{path}.filename().root_name().string()} {
        assert(std::filesystem::exists(path) && "Could not load file");
        if (FT_New_Face(library, path.c_str(), 0, &face)) {
            std::cerr << "ERROR: Failed to load font: " << path << std::endl;
        }
    }
    ~FontFace() {
        FT_Done_Face(face);
    }

    const FT_Face& operator()() const { return face; }
    FT_Face& operator()() { return face; }
    operator FT_Face() const { return face; }

    const std::string& getName() const { return name; }

private:
    FT_Face face;
    std::string name;
};