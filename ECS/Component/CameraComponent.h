#include "Component.h"
#include "../Entity/Entity.h"
#include <application.hpp>
#include <shader.hpp>
#include <glm/gtx/euler_angles.hpp>
#include<string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//class Transform;
class CameraComponent : public Component{
    enum struct CameraType {
        Orthographic,
        Perspective
    };
    private:
        // Dirty Flags are programming pattern where we only regenerate some output if:
        // 1- The inputs were changed.
        // 2- The output is needed.
        // Where we have flags for whether the View, Projection & ViewProjection matrices needed to be regenerated.
        static const uint8_t V_DIRTY = 1, P_DIRTY = 2, VP_DIRTY = 4;
        uint8_t dirtyFlags = 0;

        // The camera position, camera forward direction and camera up direction
        glm::vec3 eye = {10, 10, 10}, direction = {0, 0, -1}, up = {0, 1, 0};
        CameraType type = CameraType::Perspective;

        // The field_of_view_y is in radians and is only used for perspective cameras
        // The orthographic_height is only used for orthographic cameras
        float field_of_view_y = glm::radians(90.0f), orthographic_height = 2.0f, aspect_ratio = 1.0f, near = 0.01f, far = 1000.0f;

        glm::mat4 V{}, P{}, VP{};

    public:

        // Setup the camera as a perspective camera
        void setupPerspective(float field_of_view_y, float aspect_ratio, float near, float far);

        // Setup the camera as an orthographic camera
        void setupOrthographic(float orthographic_height, float aspect_ratio, float near, float far);

        void setType(CameraType _type);
        void setOrthographicSize(float orthographic_height);
        void setVerticalFieldOfView(float field_of_view_y);
        void setAspectRatio(float aspect_ratio);
        void setNearPlane(float near);
        void setFarPlane(float far);
        void setEyePosition(glm::vec3 eye);
        void setDirection(glm::vec3 direction);
        void setTarget(glm::vec3 target);
        void setUp(glm::vec3 up);

        glm::mat4 getProjectionMatrix();

        glm::mat4 getViewMatrix();

        glm::mat4 getVPMatrix();

        CameraType getType();
        [[nodiscard]] float getVerticalFieldOfView() const ;
        [[nodiscard]] float getHorizontalFieldOfView() const ;
        [[nodiscard]] float getOrthographicHeight() const  ;
        [[nodiscard]] float getOrthographicWidth() const ;
        [[nodiscard]] float getAspectRatio() const ;
        [[nodiscard]] float getNearPlane() const ;
        [[nodiscard]] float getFarPlane() const ;
        [[nodiscard]] glm::vec3 getEyePosition() const ;
        [[nodiscard]] glm::vec3 getDirection() const ;
        [[nodiscard]] glm::vec3 getOriginalUp() const ;

        // Get the directions of the camera coordinates in the world space
        glm::vec3 Right();
        glm::vec3 Left();
        glm::vec3 Up();
        glm::vec3 Down();
        glm::vec3 Forward();
        glm::vec3 Backward();

        // Transform point from world space to normalized device coordinates
        glm::vec3 fromWorldToDeviceSpace(glm::vec3 world);

        // Transform point from normalized device coordinates to world space
        glm::vec3 fromDeviceToWorldSpace(glm::vec3 device);
    
    glm::mat4 getCameraMatrix();

  

};