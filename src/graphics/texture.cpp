#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <glad/glad.h>

namespace {

unsigned id;

}

void load() {

    stbi_set_flip_vertically_on_load(true);

    int width, height, channels;

    auto data = stbi_load("res/atlas.png", &width, &height, &channels, 0);
    if (!data)
        ; // TODO

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id); // never needs to be unbound

    // It's a minecraft game, we don't want the textures to appear blurry
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

}

void unload() {

    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &id);

}

std::vector<float> get_vertices(Texture tex) {

    int index = static_cast<int>(tex);

    float x = index % 16, y = index / 16;
    x /= 16.0f, y /= 16.0f;

    return std::vector<float> {
        x,           y          ,
        x,           y + 0.0625f,
        x + 0.0625f, y + 0.0625f,
        x + 0.0625f, y          ,
    };

}

