//
// Created by ahmad on 31/12/2020.
//

#include "Sampler.h"

Sampler::Sampler() {sampler=0;}
void Sampler::InitializeSampler()
{  glGenSamplers(1, &sampler);}

void Sampler::DestroySampler()
{  glDeleteSamplers(1, &sampler);}

void Sampler::AttachSample(int unit)
{
    glBindSampler(unit, sampler);



}

GLuint Sampler::getSampler() {
    return sampler;
}
void Sampler::GenSampler()
{
    glGenSamplers(1, &this->sampler);
    // TODO CHECK SAMPLER PARAMETERS
//    glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, magnification_filter);
//    glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, minification_filter);
//    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, wrap_s);
//    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, wrap_t);
//    glSamplerParameterfv(sampler, GL_TEXTURE_BORDER_COLOR,  glm::value_ptr(border_color));
//    glSamplerParameterf(sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_anisotropy);
//    glPolygonMode(GL_FRONT_AND_BACK, polygon_mode);
    glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
    GLfloat max_anisotropy_upper_bound = 1.0f;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_anisotropy_upper_bound);
    glSamplerParameterf(sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_anisotropy_upper_bound);
}



Sampler::Sampler(Sampler &sampler) {
    this->sampler = sampler.sampler;
    sampler.sampler = 0;
}

Sampler & Sampler::operator=(Sampler &argsampler) {
    if(this != &argsampler) {
        glDeleteSamplers(1, &this->sampler);
        this->sampler = argsampler.sampler;
        argsampler.sampler = 0;
    }
    return *this;
}


Sampler::~Sampler() {
    glDeleteSamplers(1, &sampler);
}