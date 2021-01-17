#include "TransformComponent.h"

TransformComponent::TransformComponent(Entity* e)
{
    entity =e;
    position = {0, 0, 0};
    rotation = {0, 0, 0};
    scale = {1, 1, 1};
}

[[nodiscard]] glm::mat4 TransformComponent::to_mat4() const
{
    if (entity->parent != NULL)
    {
        TransformComponent * trans = ((TransformComponent *  )entity->parent->getComponent("transform"));
        glm::mat4 parent_mat = trans->to_mat4();

        glm::mat4 my_mat= glm::translate(glm::mat4(1.0f), position) *
                          glm::yawPitchRoll(rotation.y, rotation.x, rotation.z) *
                          glm::scale(glm::mat4(1.0f), scale);
//        std::cout << trans->rotation.x << "  "<< trans->rotation.y<< "  "<< trans->rotation.z << std::endl;
        return parent_mat * my_mat;
//        return parent_mat;
    }

    return glm::translate(glm::mat4(1.0f), position) *
           glm::yawPitchRoll(rotation.y, rotation.x, rotation.z) *
           glm::scale(glm::mat4(1.0f), scale) ;

}

void TransformComponent::init(our::Application *application, glm::vec3 pos, glm::vec3 rot, glm::vec3 sc)
{
    this->app = application;
    position = pos;
    rotation = rot;
    scale = sc;

}

void TransformComponent::rotate(double delta_time)
{
    glm::vec2 mouse_pos = app->getMouse().getMousePosition();
    glm::vec2 diff = app->getMouse().getMouseDelta();
    double delta_x =position[0]-mouse_pos[0];
    double delta_y = position[2]-mouse_pos[1];
    double factor_x=  (diff[0]*delta_time)/5;
    double factor_y=  (diff[1]*delta_time)/5;
    if(delta_x >0)
    {
        rotation[1] = rotation[1] - factor_x;
    }

    if(delta_x <0)
    {
        rotation[1] = rotation[1]  +factor_x;
    }
    if(delta_y >0)
    {
        rotation[2] = rotation[2]+factor_y;
    }

    if(delta_y <0)
    {
        rotation[2] = rotation[2]  -factor_y;
    }


}

void TransformComponent::update(double delta_time)
{

    double factor = delta_time;

    if (app->getKeyboard().isPressed(GLFW_KEY_UP))
    {
        position[1] = position[1] + factor;
//        rotation[0] = rotation[0] + factor;

    }
    if (app->getKeyboard().isPressed(GLFW_KEY_DOWN))
    {
        position[1] = position[1] - factor;
//        rotation[0] = rotation[0] -factor;
    }
    if (app->getKeyboard().isPressed(GLFW_KEY_RIGHT))
    {
        position[0] = position[0] + factor;
//        rotation[2] = rotation[2] + factor;

    }
    if (app->getKeyboard().isPressed(GLFW_KEY_LEFT))
    {
        position[0] = position[0] - factor;
//        rotation[2] = rotation[2] - factor;

    }
    if (app->getKeyboard().isPressed(GLFW_KEY_H))
    {
        position[2] = position[2] + factor;
//        rotation[1] = rotation[1] + factor;

    }
    if (app->getKeyboard().isPressed(GLFW_KEY_G))
    {
        position[2] = position[2] - factor;
//        rotation[1] = rotation[1] - factor;

    }

    //   if(app->getKeyboard().isPressed(GLFW_KEY_S)) position -= front * ((float)delta_time * current_sensitivity.z);
    // if(app->getKeyboard().isPressed(GLFW_KEY_Q)) position += up * ((float)delta_time * current_sensitivity.y);
    // if(app->getKeyboard().isPressed(GLFW_KEY_E)) position -= up * ((float)delta_time * current_sensitivity.y);
    // if(app->getKeyboard().isPressed(GLFW_KEY_D)) position += right * ((float)delta_time * current_sensitivity.x);
    // if(app->getKeyboard().isPressed(GLFW_KEY_A)) position -= right * ((float)delta_time * current_sensitivity.x);

}


void TransformComponent::fall(double delta_time, int speed)
{
    double factor = speed * delta_time;

    position[1] = position[1] - factor;
    rotation[0] = rotation[0] -factor;

}

void TransformComponent::Move(double delta)
{

    double factor = delta;
    position[2] = position[2] + factor;


}
