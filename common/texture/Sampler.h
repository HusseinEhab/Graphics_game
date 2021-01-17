//
// Created by ahmad on 31/12/2020.
//



#ifndef PH1_SAMPLER_H
#define PH1_SAMPLER_H

#include <application.hpp>
#include <shader.hpp>
#include <imgui-utils/utils.hpp>
#include <GL/gl.h>
#include <texture/texture-utils.h>

#include <unordered_map>

class Sampler {
    //For Creating a Sampler Environment
    GLenum magnification_filter = GL_LINEAR, minification_filter = GL_LINEAR_MIPMAP_LINEAR;
    GLenum wrap_s = GL_REPEAT, wrap_t = GL_REPEAT;
    glm::vec4 border_color = {1,1,1,1};
    GLfloat max_anisotropy = 1.0f;

    GLenum polygon_mode = GL_FILL;

    GLuint sampler;
    GLsizei numOfSamplers;
public:
    Sampler();
    void InitializeSampler();
    void DestroySampler();
    void AttachSample(int unit);
    void GenSampler();
    GLuint getSampler();
    Sampler(Sampler &sampler);
    Sampler & operator=(Sampler &sampler);
    Sampler &operator=(Sampler const &) = delete;
    ~Sampler();
// setter here GLint mag,GLint min,GLint s,GLint t,GLint param,glm::vec4  borderCol, GLfloat anisotropy ,GLenum polymode


};


#endif //PH1_SAMPLER_H
