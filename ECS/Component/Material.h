#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H
#include <shader.hpp>
#include <utility>
#include <imgui-utils/utils.hpp>
#include <string>
#include <texture/Texture2D.h>
#include <texture/Sampler.h>
#include "RenderState.h"

class Material{
public:
//    Texture2D * texture;
    Texture2D * albedo_map;
    Texture2D * specular_map;
    Texture2D * roughness_map;
    Texture2D * emissive_map;
    Texture2D * ambient_occlusion_map;

    Sampler * sampler;
//    Sampler * albedo_samp;
//    Sampler * specular_samp;
//    Sampler * roughness_samp;
//    Sampler * emissive_samp;
//    Sampler * ambient_occlusion_samp;

    glm::vec3 albedo_tint{}, specular_tint{}, emissive_tint{};
    glm::vec2 roughness_range{};
    
    RenderState * renderstate;
    our::ShaderProgram * MatrixShader;
//    glm::vec4 tint;
//    glm::vec3 specularTint, emissive;
//    glm::vec3 diffuse, specular, ambient;
//    float shininess;
    void init(our::ShaderProgram * ShaderPointer, std::vector<Texture2D *> textures, RenderState * renderstate, Sampler *  sampler, glm:: vec3 albedo_tint ={1.0f, 1.0f, 1.0f}, glm:: vec3 specular_tint ={1.0f, 1.0f, 1.0f}, glm:: vec2 roughness_range ={0 , 1.0f}, glm:: vec3 emissive_tint ={1.0f, 1.0f, 1.0f});
    void Initialize();
    void TransformShader(glm::mat4 transformationMatrix,glm::mat4 cameraMatrix);
};

#endif