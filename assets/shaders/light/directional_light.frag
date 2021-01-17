#version 330 core

#include "light_common.glsl"

in vec4 vertex_color;
uniform sampler2D sampler;
uniform int lod;
uniform float zoom;


uniform vec4 tint; // A tint is something we can use to modify colors

in Varyings {
    vec4 color;
    vec2 tex_coord;
    vec3 world;
    vec3 view;
    vec3 normal;
} fsin;

struct DirectionalLight {
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;

    vec3 direction;
};

uniform Material material;
uniform DirectionalLight light;

out vec4 frag_color;

void main() {
    vec3 normal = normalize(fsin.normal);
    vec3 view = normalize(fsin.view);

    vec3 diffuse = material.diffuse * light.diffuse * calculate_lambert(normal, light.direction);
    vec3 specular = material.specular * light.specular * calculate_phong(normal, light.direction, view, material.shininess);
    vec3 ambient = material.ambient * light.ambient;




    frag_color = tint * texture(sampler, fsin.tex_coord);
    frag_color = frag_color*fsin.color * vec4(diffuse + specular + ambient, 1.0f);
}
