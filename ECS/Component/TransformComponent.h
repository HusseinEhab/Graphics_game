#include "Component.h"
#include "../Entity/Entity.h"
#include <application.hpp>
#include <shader.hpp>
#include<string>
#include <glm/gtx/euler_angles.hpp>

class TransformComponent : public Component{

public:

    our::Application* app;
    glm::vec3 position, rotation, scale;
    //glm::vec4 tint;
    //std::optional<std::string> mesh;
    //std::string texture;
    //std::unordered_map<std::string, std::shared_ptr<Transform>> children;
    Entity* entity;
    TransformComponent(Entity* e);
    [[nodiscard]] glm::mat4 to_mat4() const;
    void init( our::Application* application,glm::vec3 pos,glm::vec3 rot,glm::vec3 sc);
    void rotate(double delta_time);
    void update(double delta_time);
    void fall(double delta_time,int speed);
    void Move(double delta_time);
};