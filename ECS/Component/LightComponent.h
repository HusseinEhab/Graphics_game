
enum class LightType {
    DIRECTIONAL,
    POINT,
    SPOT
};

class LightComponent : public Component{
public:
    LightType Type;
    bool enabled = true;
//    glm::vec3 diffuse, specular, ambient;
//    glm::vec3 position; // Used for Point and Spot Lights only
//    glm::vec3 direction; // Used for Directional and Spot Lights only
    struct {
        float constant, linear, quadratic;
    } attenuation; // Used for Point and Spot Lights only
    struct {
        float inner, outer;
    } spot_angle; // Used for Spot Lights only

    LightComponent (LightType type){
        Type = type;
//        diffuse = {1,1,1};
//        specular = {1,1,1};
//        ambient = {0.1f, 0.1f, 0.1f};
//        direction = {-1, -1, -1};
//        position = {0, 1, 2};
        attenuation = {0, 0, 0.1};
        spot_angle = {glm::pi<float>()/4, glm::pi<float>()/2};
    }

    void setAttenuation(float constant,  float linear = -1, float quadratic = -1)
    {

        this->attenuation.constant = (constant >= 0)? constant:this->attenuation.constant;
        this->attenuation.linear =  (linear >= 0)? linear:this->attenuation.linear;
        this->attenuation.quadratic =  (quadratic >= 0)? quadratic:this->attenuation.quadratic;
    }

};
