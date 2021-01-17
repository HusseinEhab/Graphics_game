#include <iostream>
#include <glad/gl.h>
#include <application.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "Component.h"
 class RenderState  : public Component {
    public:

    bool depth=false;
    bool blend=false;
    bool culling=false;
    GLenum depth_function;
    float clear_depth = 1.0f;
    bool depth_mask = true;
    glm::bvec4 color_mask = {true, true, true, true};

    GLenum culled_face ;
    GLenum front_face_winding;


    GLenum blend_equation = GL_FUNC_ADD;
    GLenum blend_source_function = GL_SRC_ALPHA, blend_destination_function = GL_ONE_MINUS_SRC_ALPHA;
    glm::vec4 blend_constant_color = {1.0f,1.0f,1.0f,1.0f};

   bool enable_alpha_test = true;
   float alpha_test_threshold = 0.5;

   bool enable_alpha_to_coverage = false;

    void setDepthTesting(bool x,GLenum depth_function);
    void setCulling(bool x,GLenum culled_face,GLenum front_face_winding);
    void setBlending(bool x);
    bool getDepthTesting();
    bool getCulling();
 };