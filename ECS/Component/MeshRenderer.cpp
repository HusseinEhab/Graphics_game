#include "MeshRenderer.h"

void MeshRendererComponent::init(Material* Materialptr,our::Mesh *y){
        Materialpt=Materialptr;meshPtr=y;
};

void MeshRendererComponent::draw(glm::mat4 transformationMatrix,glm::mat4 cameraMatrix){
    Materialpt->Initialize();
    Materialpt->TransformShader(transformationMatrix,cameraMatrix);
    meshPtr->draw();
        //NOTE: Remember to reset the color mask such that ImGUI can draw
        glColorMask(true, true, true, true);
};
