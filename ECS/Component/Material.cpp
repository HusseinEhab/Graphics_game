#include "Material.h"
#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"


void Material::init(our::ShaderProgram * ShaderPointer, std::vector<Texture2D *> textures, RenderState * renderstate, Sampler *  sampler, glm::vec3 albedo_tint, glm::vec3 specular_tint, glm::vec2 roughness_range, glm::vec3 emissive_tint){
    MatrixShader = ShaderPointer;
//    diffuse = {0.96,0.9,0.64};
//    specular = {0.4,0.4,0.4};
//    ambient = {0.96, 0.9, 0.64};
//    shininess = 10.0;
//    this->texture = texture;
    this->renderstate = renderstate;
    this->sampler = sampler;

    // TODO CHANGE VALUES IN SETTER
    this->albedo_tint = albedo_tint;
    this->specular_tint = specular_tint;
    this->roughness_range = roughness_range;
    this->emissive_tint = emissive_tint;
    albedo_map = textures[0];
    specular_map = textures[1];
    roughness_map = textures[2];
    emissive_map = textures[3];
    ambient_occlusion_map = textures[4];

}

void Material::Initialize(){
//    MatrixShader->set("tint", tint);
};
// TODO CHECK IF NEEDED
void Material::TransformShader(glm::mat4 transformationMatrix,glm::mat4 cameraMatrix){
    MatrixShader->set("transform",cameraMatrix*transformationMatrix);
};