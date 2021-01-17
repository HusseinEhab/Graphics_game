#include <iostream>
#include "RenderState.h"
void RenderState:: setDepthTesting(bool x,GLenum depth_function){
    if(x){
        glEnable(GL_DEPTH_TEST);
        this->depth=true;
    }
    else
        { glDisable(GL_DEPTH_TEST);}
    this->depth_function=depth_function;
    glDepthFunc(depth_function);
    glClearDepth(clear_depth);

    glDepthMask(depth_mask);
    glColorMask(color_mask.r, color_mask.g, color_mask.b, color_mask.a);
}

void RenderState:: setCulling(bool x,GLenum culled_face,GLenum front_face_winding){

    if(x)
    {
        glEnable(GL_CULL_FACE);
        this->culling=true;
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }

    this->culled_face=culled_face;
    this->front_face_winding=front_face_winding;

    glCullFace(culled_face);
    glFrontFace(front_face_winding);
}
void RenderState:: setBlending(bool x){
    if(x) {
        blend = true;
        glEnable(GL_BLEND);
    }
    else {
        blend = false;
        glDisable(GL_BLEND);
    }

      
    glBlendEquation(blend_equation);
    glBlendFunc(blend_source_function, blend_destination_function);
    glBlendFunc (GL_ONE, GL_ONE);
    glBlendColor(blend_constant_color.r, blend_constant_color.g, blend_constant_color.b, blend_constant_color.a);

    if(enable_alpha_to_coverage) glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
    else glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);


}

bool RenderState::getDepthTesting(){

    return this->depth;

}
bool RenderState::getCulling(){

    return this->culling;

}
