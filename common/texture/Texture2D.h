//
// Created by ahmad on 31/12/2020.
//

#ifndef PH1_TEXTURE2D_H
#define PH1_TEXTURE2D_H
#include <application.hpp>
#include <shader.hpp>
#include <imgui-utils/utils.hpp>

#include <texture/texture-utils.h>

#include <unordered_map>

class Texture2D {





public:
    GLuint texture;
    Texture2D();
    Texture2D(const char * file);
    void onDestroy();
    void ActivateTexture(const char * filename, bool generate_mipmap);
    void LinkTexture(int unit);
    Texture2D (const Texture2D&) = delete;
    Texture2D& operator= (const Texture2D&) = delete;
};


#endif //PH1_TEXTURE2D_H
