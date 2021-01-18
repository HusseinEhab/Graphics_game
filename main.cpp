#include <application.hpp>
#include <iostream>
#include <fstream>
#include <cassert>
#include "input/keyboard.hpp"
#include "input/mouse.hpp"
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <shader.hpp>
#include <memory>
#include <utility>
#include <texture/texture-utils.h>
#include <glm/gtx/euler_angles.hpp>
#include <json/json.hpp>
#include <mesh/common-vertex-types.hpp>
#include <mesh/common-vertex-attributes.hpp>
#include "../ECS/Entity/Entity.h"
//#include "../ECS/Component/TransformComponent.h"
#include "../ECS/Component/CameraControllerComponent.h"
#include "../ECS/Component/MeshRenderer.h"
#include "glm/gtx/string_cast.hpp"
#include <iostream>
#include <vector>
#include <texture/Texture2D.h>
#include <texture/Sampler.h>
#include <ctime>
#include "Component/LightComponent.h"
#pragma region helper_functions

struct SkyLight {
    bool enabled;
    glm::vec3 top_color, middle_color, bottom_color;
};
std::string read_file(const char *filename)
{
    std::ifstream fin(filename);
    if (fin.fail())
    {
        std::cerr << "Unable to open shader file: " << filename << std::endl;
        std::exit(-1);
    }
    return std::string(std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>());
}
void checkShaderCompilationErrors(GLuint shader)
{
    //Check and log for any error in the compilation process.
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status); // Takes a shader and returns the status of this shader program.
    if (!status)
    { // If there is a status (status != 0):
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length); // Get the error length (char array length).
        char *logStr = new char[length];
        glGetShaderInfoLog(shader, length, nullptr, logStr); // Get the error char array.
        std::cerr << "ERROR:" << logStr << std::endl;        // print the char array of the log error.
        delete[] logStr;
        std::exit(-1);
    }
}
void attachShader(GLuint program, const char *filename, GLenum shader_type)
{

    // 1. Reads the shader from file, compiles it,
    std::string source_code = read_file(filename);
    const char *source_code_as_c_str = source_code.c_str();

    // 2. Pass the program as a string to the GPU and then compile it.
    GLuint shader = glCreateShader(shader_type);
    // Function parameter:
    // shader (GLuint): shader object name.
    // count (GLsizei): number of strings passed in the third parameter. We only have one string here.
    // string (const GLchar**): an array of source code strings.
    // lengths (const GLint*): an array of string lengths for each string in the third parameter. if null is passed,
    //                          then the function will deduce the lengths automatically by searching for '\0'.
    glShaderSource(shader, 1, &source_code_as_c_str, nullptr);
    glCompileShader(shader);


    // 3. Check for any Compilation errors.
    checkShaderCompilationErrors(shader);

    // 4. Attach this shader to the program if no errors found in shader.
    glAttachShader(program, shader);

    // 5. Delete the shader as it is already attached to the program.
    glDeleteShader(shader);
}
// Reads a file using a path and returns the file text in a string.
void checkProgramLinkingErrors(GLuint program)
{
    //Check and log for any error in the linking process.
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status); // Takes a shader program (vertex & fragment) and returns the status of this shader program.
    if (!status)                                      // If there is a status (status != 0):
    {
        GLint length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length); // Get the error length (char array length).
        char *logStr = new char[length];
        glGetProgramInfoLog(program, length, nullptr, logStr); // Get the error char array.
        std::cerr << "LINKING ERROR: " << logStr << std::endl; // print the char array of the log error.
        delete[] logStr;
        std::exit(-1);
    }
}

#pragma endregion

GLuint program = 0, vertex_array = 0;
int keyShaderOption = 2;
glm::vec2 mouse = glm::vec2(0, 0);
GameState *switchState(int state);


our::Application *app = new our::Application();
int currentLevel = 1;
class LevelOne : public GameState
{
public:
    ~LevelOne()
    {
        for(int i = 0; i< Entities.size(); i++)
            delete Entities[i];
        for(int i = 0; i< LightEntities.size(); i++)
            delete LightEntities[i];

        shader.destroy();
        shader2.destroy();
        sky_program.destroy();
        for ( int i = 0; i<meshes.size(); i++)
            meshes[i]->destroy();
        delete MaterialObj;
        delete MaterialObj2;
        delete MaterialObj3;
        delete MaterialObj4;
        delete MaterialObj5;
        delete MaterialObj6;
        delete MaterialObj7;
        delete MaterialObj8;
        delete TextureObject;
    }
private:

    float delay;
    float Monsterspeed;
    bool win = false;
    bool lose = false;
    float xThresh = 1.3, yThresh = 2.5, zThresh= 2;
    int nMonsters = 7;
    int nKilled=  0;
    int nGenerated = 0;
    bool continue_game = false;
    float counter = 5;
    our::ShaderProgram shader, shader2, sky_program;
    our::ShaderProgram *ptrShader,*ptrShader2;
    our::Mesh cube_model;
    our::Mesh sun_model;
    our::Mesh plane_model;
    our::Mesh wall_model;
    our::Mesh Girl_model;
    our::Mesh Monster_model;
    our::Mesh Monster_model2;
    our::Mesh Gun_model;
    our::Mesh Bullet_model;
    our::Mesh worldbox;
    std::vector<Entity *> Entities;
    std::vector<Entity *> LightEntities;
    std::vector<Entity *> Monsters;
    std::vector<Entity *> Bullets;
    std::vector<Entity *> Axes;

//    std::vector<Component *> components;
    std::vector<our::Mesh*> meshes;
    int mml = 0;
    float zoom = 1;
    glm::mat4 cameraMatrix;
    Entity *CameraEntity = new Entity(NULL);
    CameraComponent *cameraComponent = new CameraComponent();
    TransformComponent * cameraTransform = new TransformComponent(CameraEntity);
    CameraControllerComponent *cameraController = new CameraControllerComponent(app,cameraComponent);

    //////////////////////// Texture 2d and Sampler///////////////////////
    Texture2D* TextureObject = new Texture2D();
    Sampler* SampleObject = new Sampler();


    //////////// CREATING LIGHTS AND ITS //components////////////////////////////
    SkyLight sky_light;
    float sky_box_exposure = 2.0f;
    Entity *Light1 = new Entity(NULL);
    LightComponent *light = new LightComponent(LightType::DIRECTIONAL);
    TransformComponent * trlight = new TransformComponent(Light1);

    Entity *Light2 = new Entity(NULL);
    LightComponent *light2 = new LightComponent(LightType::POINT);
    TransformComponent * trlight2 = new TransformComponent(Light2);
///////////////////////////////////////////////////////////////////////////////////////
    ////////Adding Material
    Material * MaterialObj= new Material();
    Material * MaterialObj2= new Material();
    Material * MaterialObj3= new Material();
    Material * MaterialObj4= new Material();
    Material * MaterialObj5= new Material();
    Material * MaterialObj6= new Material();
    Material * MaterialObj7= new Material();
    Material * MaterialObj8= new Material();

    GameState *handleEvents()
    {
        if(win)
        {
//            EndLevel();
            if(currentLevel == 1)
            {
                win = false;
                currentLevel = 2;
                // TODO CALL FUNCTIONS THAT WILL DELETE ALL BULLETS AND RESET ALL VARIABLES RELATED TO SPEED AND SHIT
            }
            else
            {
                win = false;
                resetLevel();
            }
            return switchState(0);
        }
        else if(lose)
        {
            lose = false;
            resetLevel();
            return switchState(0);

        }
        if (app->getKeyboard().justPressed(GLFW_KEY_ESCAPE))
        {
            onExit();
            return switchState(0);
        }
        return NULL;
    }

    void CreateMaterials()
    {
        ///////////////////// GENERATING METAL MATERIAL //////////////////////////
//        std::vector<Texture2D *> Metal;
//        TextureObject = new Texture2D();
//        TextureObject->ActivateTexture("assets/images/common/materials/metal/albedo.jpg", true);
//        Metal.push_back(TextureObject);
//
//        TextureObject = new Texture2D();
//        TextureObject->ActivateTexture("assets/images/common/materials/metal/specular.jpg", true);
//        Metal.push_back(TextureObject);
//
//        TextureObject = new Texture2D();
//        TextureObject->ActivateTexture("assets/images/common/materials/metal/roughness.jpg", true);
//        Metal.push_back(TextureObject);
//
//        TextureObject = new Texture2D();
//        glGenTextures(1, &(TextureObject->texture));
//        our::texture_utils::singleColor(TextureObject->texture, {0, 0, 0, 255});
//        Metal.push_back(TextureObject);
//
//        TextureObject = new Texture2D();
//        glGenTextures(1, &(TextureObject->texture));
//        our::texture_utils::singleColor(TextureObject->texture, {255, 255, 255, 255});
//        Metal.push_back(TextureObject);


        ///////////////////// GENERATING WOOD MATERIAL //////////////////////////
        std::vector<Texture2D *> Wood;

        TextureObject = new Texture2D();
        TextureObject->ActivateTexture("assets/images/common/materials/wood/albedo.jpg", true);
        Wood.push_back(TextureObject);

        TextureObject = new Texture2D();
        TextureObject->ActivateTexture("assets/images/common/materials/wood/specular.jpg", true);
        Wood.push_back(TextureObject);

        TextureObject = new Texture2D();
        TextureObject->ActivateTexture("assets/images/common/materials/wood/roughness.jpg", true);
        Wood.push_back(TextureObject);

        TextureObject = new Texture2D();
        glGenTextures(1, &(TextureObject->texture));
        our::texture_utils::singleColor(TextureObject->texture, {0, 0, 0, 255});
        Wood.push_back(TextureObject);

        TextureObject = new Texture2D();
        glGenTextures(1, &(TextureObject->texture));
        our::texture_utils::singleColor(TextureObject->texture, {255, 255, 255, 255});
        Wood.push_back(TextureObject);


        ///////////////////// GENERATING ASPHALT MATERIAL //////////////////////////
        std::vector<Texture2D *> asphalt;

        TextureObject = new Texture2D();
        TextureObject->ActivateTexture("assets/images/common/materials/asphalt/albedo.jpg", true);
        asphalt.push_back(TextureObject);

        TextureObject = new Texture2D();
        TextureObject->ActivateTexture("assets/images/common/materials/asphalt/specular.jpg", true);
        asphalt.push_back(TextureObject);

        TextureObject = new Texture2D();
        TextureObject->ActivateTexture("assets/images/common/materials/asphalt/roughness.jpg", true);
        asphalt.push_back(TextureObject);

        TextureObject = new Texture2D();
        TextureObject->ActivateTexture("assets/images/common/materials/asphalt/emissive.jpg", true);
        asphalt.push_back(TextureObject);

        TextureObject = new Texture2D();
        glGenTextures(1, &(TextureObject->texture));
        our::texture_utils::singleColor(TextureObject->texture, {255, 255, 255, 255});
        asphalt.push_back(TextureObject);


        ///////////////////// GENERATING Girl MATERIAL //////////////////////////
        std::vector<Texture2D *> Lava;

        TextureObject = new Texture2D();
        TextureObject->ActivateTexture("assets/images/common/materials/Lava/Lava_002_COLOR.png", true);
        Lava.push_back(TextureObject);

        TextureObject = new Texture2D();
        TextureObject->ActivateTexture("assets/images/common/materials/Lava/Lava_002_SPEC.png", true);
        Lava.push_back(TextureObject);


        TextureObject = new Texture2D();
        glGenTextures(1, &(TextureObject->texture));
        our::texture_utils::singleColor(TextureObject->texture, {255, 255, 255, 255});
        Lava.push_back(TextureObject);

        TextureObject = new Texture2D();
        glGenTextures(1, &(TextureObject->texture));
        our::texture_utils::singleColor(TextureObject->texture, {0, 0, 0, 255});
        Lava.push_back(TextureObject);

        TextureObject = new Texture2D();
        TextureObject->ActivateTexture("assets/images/common/materials/Lava/Lava_002_OCC.png", true);
        Lava.push_back(TextureObject);


        ////////////////////////////// MONSTER ///////////////////////////////////
        std::vector<Texture2D *> Monster;

        TextureObject = new Texture2D();
        TextureObject->ActivateTexture("assets/models/monster/creature1Albedo.png", true);
        Monster.push_back(TextureObject);

        TextureObject = new Texture2D();
        TextureObject->ActivateTexture("assets/models/monster/creature1Specular.png", true);
        Monster.push_back(TextureObject);

        TextureObject = new Texture2D();
        TextureObject->ActivateTexture("assets/models/monster/creature1Normal.png", true);
        Monster.push_back(TextureObject);

        TextureObject = new Texture2D();
        glGenTextures(1, &(TextureObject->texture));
        our::texture_utils::singleColor(TextureObject->texture, {0, 0, 0, 255});
        Monster.push_back(TextureObject);

        TextureObject = new Texture2D();
        TextureObject->ActivateTexture("assets/models/monster/creature1AO.png", true);
        Monster.push_back(TextureObject);


        std::vector<Texture2D *> Monster2;

        TextureObject = new Texture2D();
        TextureObject->ActivateTexture("assets/models/monster3/Albedo2.png", true);
        Monster2.push_back(TextureObject);

        TextureObject = new Texture2D();
        TextureObject->ActivateTexture("assets/models/monster3/specular.jpeg", true);
        Monster2.push_back(TextureObject);

        TextureObject = new Texture2D();
        TextureObject->ActivateTexture("assets/models/monster3/monster_default_rough.png", true);
        Monster2.push_back(TextureObject);

        TextureObject = new Texture2D();
        glGenTextures(1, &(TextureObject->texture));
        our::texture_utils::singleColor(TextureObject->texture, {0, 0, 0, 255});
        Monster2.push_back(TextureObject);

        TextureObject = new Texture2D();
        glGenTextures(1, &(TextureObject->texture));
        our::texture_utils::singleColor(TextureObject->texture, {255, 255, 255, 255});
        Monster2.push_back(TextureObject);

        std::vector<Texture2D *> Gun;

        TextureObject = new Texture2D();
        TextureObject->ActivateTexture("assets/models/Gun/handgun_C.jpg", true);
        Gun.push_back(TextureObject);

        TextureObject = new Texture2D();
        TextureObject->ActivateTexture("assets/models/Gun/handgun_S.jpg", true);
        Gun.push_back(TextureObject);

        TextureObject = new Texture2D();
        glGenTextures(1, &(TextureObject->texture));
        our::texture_utils::singleColor(TextureObject->texture, {255, 255, 255, 255});
        Gun.push_back(TextureObject);

        TextureObject = new Texture2D();
        glGenTextures(1, &(TextureObject->texture));
        our::texture_utils::singleColor(TextureObject->texture, {0, 0, 0, 255});
        Gun.push_back(TextureObject);

        TextureObject = new Texture2D();
        glGenTextures(1, &(TextureObject->texture));
        our::texture_utils::singleColor(TextureObject->texture, {255, 255, 255, 255});
        Gun.push_back(TextureObject);


        std::vector<Texture2D *> Bullet;

        TextureObject = new Texture2D();
        TextureObject->ActivateTexture("assets/models/Bullet/Bullet_BaseColor.png", true);
        Bullet.push_back(TextureObject);

        TextureObject = new Texture2D();
        glGenTextures(1, &(TextureObject->texture));
        our::texture_utils::singleColor(TextureObject->texture, {0, 0, 0, 255});
        Bullet.push_back(TextureObject);

        TextureObject = new Texture2D();
        TextureObject->ActivateTexture("assets/models/Bullet/Bullet_Roughness.png", true);
        Bullet.push_back(TextureObject);

        TextureObject = new Texture2D();
        glGenTextures(1, &(TextureObject->texture));
        our::texture_utils::singleColor(TextureObject->texture, {0, 0, 0, 255});
        Bullet.push_back(TextureObject);

        TextureObject = new Texture2D();
        TextureObject->ActivateTexture("assets/models/Bullet/Bullet_AO.png", true);
        Bullet.push_back(TextureObject);

        RenderState * rs = new RenderState();
        rs->setDepthTesting(true,GL_LEQUAL);
        rs->setCulling(false,GL_BACK,GL_CCW);
        rs->setBlending(false);
        //components.push_back(rs);

        RenderState * rs2 = new RenderState();
        rs2->setDepthTesting(true,GL_LEQUAL);
        rs2->setCulling(false,GL_BACK,GL_CCW);
        rs2->setBlending(false);
        //components.push_back(rs2);


//        MaterialObj->init(ptrShader, Metal, rs, SampleObject);
//        MaterialObj->tint = {1,1,1,1};

        MaterialObj2->init(ptrShader, Wood, rs2, SampleObject);

        MaterialObj3->init(ptrShader, asphalt, rs2, SampleObject);

        MaterialObj4->init(ptrShader, Lava, rs2, SampleObject);

        MaterialObj5->init(ptrShader, Monster, rs2, SampleObject);

        MaterialObj6->init(ptrShader, Monster2, rs2, SampleObject);

        MaterialObj7->init(ptrShader, Gun, rs2, SampleObject);

        MaterialObj8->init(ptrShader, Bullet, rs2, SampleObject);

    }

    void passLights(our::ShaderProgram * shader) {
        (*shader).set("camera_position", cameraComponent->getEyePosition());
        (*shader).set("view_projection", cameraComponent->getVPMatrix());
        (*shader).set("sky_light.top_color", sky_light.enabled ? sky_light.top_color : glm::vec3(0.0f));
        (*shader).set("sky_light.middle_color", sky_light.enabled ? sky_light.middle_color : glm::vec3(0.0f));
        (*shader).set("sky_light.bottom_color", sky_light.enabled ? sky_light.bottom_color : glm::vec3(0.0f));
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int light_index = 0;
        const int MAX_LIGHT_COUNT = 16;
        for (const auto &e : LightEntities) {
            // GET LIGHT COMPONENT FROM ENTITY CONTAINING LIGHT
            LightComponent *light = ((LightComponent *) e->getComponent("light"));
            TransformComponent *transComponent = ((TransformComponent *) e->getComponent("transform"));

            if (!light->enabled) continue;
            std::string prefix = "lights[" + std::to_string(light_index) + "].";

            (*shader).set(prefix + "type", static_cast<int>(light->Type));
            (*shader).set(prefix + "color", {1, 1, 1});


            switch (light->Type) {
                case LightType::DIRECTIONAL:
                    (*shader).set(prefix + "direction", glm::normalize(transComponent->rotation));
                    break;
                case LightType::POINT:
                    (*shader).set(prefix + "position", transComponent->position);
                    (*shader).set(prefix + "attenuation_constant", light->attenuation.constant);
                    (*shader).set(prefix + "attenuation_linear", light->attenuation.linear);
                    (*shader).set(prefix + "attenuation_quadratic", light->attenuation.quadratic);
                    break;
                case LightType::SPOT:
                    (*shader).set(prefix + "position", transComponent->position);
                    (*shader).set(prefix + "direction", glm::normalize(transComponent->rotation));
                    (*shader).set(prefix + "attenuation_constant", light->attenuation.constant);
                    (*shader).set(prefix + "attenuation_linear", light->attenuation.linear);
                    (*shader).set(prefix + "attenuation_quadratic", light->attenuation.quadratic);
                    (*shader).set(prefix + "inner_angle", light->spot_angle.inner);
                    (*shader).set(prefix + "outer_angle", light->spot_angle.outer);
                    break;
            }
            light_index++;
            if (light_index >= MAX_LIGHT_COUNT) break;
        }
        // Since the light array in the shader has a constant size, we need to tell the shader how many lights we sent.
        (*shader).set("light_count", light_index);
    }

    void EmptyBullets()
    {
        for(int j = 0; j<Bullets.size();j++)
        {
            for (auto it3 = Entities.begin(); it3 != Entities.end(); ++it3)
            {
                
                if(Bullets[j] == (*it3))
                {
                    it3 = Entities.erase(it3);
                    it3 --;
                }
                
            }
            delete Bullets[j];
            delete Axes[j];
        }

        int size = Bullets.size();
        for(int j = 0; j<size;j++)
        {
            Bullets.pop_back();
            Axes.pop_back();

        }
        

    }

    void EmptyMonsters()
    {
        for(int j = 0; j<Monsters.size();j++)
        {
            for (auto it3 = Entities.begin(); it3 != Entities.end(); ++it3)
            {

                if(Monsters[j] == (*it3))
                {
                    it3 = Entities.erase(it3);
                    it3 --;
                    break;
                }

            }
            delete Monsters[j];

        }
        int size = Monsters.size();
        for(int j = 0; j<size;j++)
        {
            Monsters.pop_back();

        }


    }

    void Changetexture()
    {
        for(int i = 0; i< Entities.size(); i++)
        {
            if(Entities[i]->getComponent("mesh"))
            {
                MeshRendererComponent *mesh =((MeshRendererComponent *)Entities[i]->getComponent("mesh"));
                if(mesh->Materialpt == MaterialObj2)
                    mesh->Materialpt = MaterialObj4;
            }

        }

    }

    void GotoLevel2()
    {
        delay = 1.5;
        nMonsters = 15;
        Monsterspeed = 0.2;
        nKilled = 0;
        nGenerated = 0;
        xThresh = 2.3;
        yThresh = 6;
        zThresh= 2;
        EmptyBullets();
        Changetexture();

        sky_light.bottom_color={0.098, 0.101, 0.33};
        sky_light.middle_color={0.098, 0.101, 0.33};
//        sky_light.top_color={0.098, 0.101, 0.33};

    }

    void resetLevel()
    {
        nKilled = 0;
        nGenerated = 0;
        EmptyBullets();
        EmptyMonsters();
    }
    
    void onEnter() override
    {
        if(currentLevel == 2)
            GotoLevel2();
        if(continue_game)
            return;
        srand((unsigned) time(0));

        continue_game = true;
        delay = 6;
//        nMonsters = 4;
        Monsterspeed = 0.08;




        counter = 0;
        SampleObject->InitializeSampler();


        int width, height;
        glfwGetFramebufferSize(app->window, &width, &height);
        our::mesh_utils::Cuboid(worldbox);
        meshes.push_back(&worldbox);

        shader2.create();
        shader2.attach("assets/shaders/light/light_transform.vert", GL_VERTEX_SHADER);
        shader2.attach("assets/shaders/light/light_array.frag", GL_FRAGMENT_SHADER);
        shader2.link();

        shader.create();
        shader.attach("assets/shaders/light/light_transform.vert", GL_VERTEX_SHADER);
        shader.attach("assets/shaders/light/light_array.frag", GL_FRAGMENT_SHADER);
        shader.link();

        sky_program.create();
        // This shader is responsible for rendering the sky box. (Not important for lighting but looks better than a blank background).
        sky_program.attach("assets/shaders/light/sky_transform.vert", GL_VERTEX_SHADER);
        sky_program.attach("assets/shaders/light/sky.frag", GL_FRAGMENT_SHADER);
        sky_program.link();




        // FOR EACH ENTITY WE WILL USE IT ON ITS SHADER
        ptrShader = &shader;
        ptrShader2 = &shader2;
        CreateMaterials();

        //Camera Operations
        CameraEntity->addComponent(cameraComponent, "camera");
        CameraEntity->addComponent(cameraController, "controller");
        cameraController->setCameraTransform(cameraTransform);
        CameraEntity->addComponent(cameraTransform, "transform");
//        cameraController->setPosition({5.39, 0.878, 6.56});
        cameraController->setPosition({0, 0.0, 0});
        cameraMatrix = cameraComponent->getCameraMatrix();
//        cameraComponent->setEyePosition({3.8742, 0.771,4.3913})
        cameraComponent->setEyePosition({1, 0,0});
        cameraComponent->setTarget({0, 0, 0});
        cameraComponent->setUp({0, 1, 0});
        cameraComponent->setupPerspective(glm::pi<float>()/2, static_cast<float>(width)/height, 0.1f, 100.0f);
        Entities.push_back(CameraEntity);


        //////////// INITIALIZING LIGHTS AND ITS //components////////////////////////////
        sky_light.enabled  = true;
        sky_light.top_color = {0.25, 0.3, 0.5};
        sky_light.middle_color = {0.35, 0.35, 0.4};
        sky_light.bottom_color = {0.25, 0.25, 0.25};

        trlight->init(app, {0, 1, 2}, {-1, -1, -1}, {1, 1, 1});
        Light1->addComponent(trlight, "transform");
        Light1->addComponent(light, "light");
        LightEntities.push_back(Light1);

        trlight2->init(app, {0, 1, 2}, {-1, -1, -1}, {1, 1, 1});
        Light2->addComponent(trlight2, "transform");
        Light2->addComponent(light2, "light");
        LightEntities.push_back(Light2);

        ////////////////Initializing Material/////////////////////////////

//        our::mesh_utils::Cuboid(cube_model, false);
//        meshes.push_back(&cube_model);
//        createObject({5, 5, 5}, {0, 0, 0}, {5, 5, 5}, cube_model, MaterialObj);


        our::mesh_utils::Plane(plane_model, {1, 1}, false, {0, 0, 0},
                               {1, 1}, {0, 0}, {100, 100});
        meshes.push_back(&plane_model);
        createObject({0,-2, -50}, {0, 0, 0}, {1000, 1000, 300}, plane_model,  MaterialObj3);

        // TODO MAKE AS WOODEN WALLS
        our::mesh_utils::Plane(wall_model, {1, 1}, false, {0, 0, 0},
                               {1, 1}, {0, 0}, {100, 100});
        meshes.push_back(&wall_model);
        createObject({-10,-2, 0}, {0, 0, glm::pi<float>()/2}, {40, 10, 200}, wall_model,  MaterialObj2);
        createObject({11,-2, 0}, {0, 0, glm::pi<float>()/2}, {40, 10, 200}, wall_model,  MaterialObj2);

//        our::mesh_utils::Sphere(sun_model, {32, 16}, false);
//        meshes.push_back(&sun_model);
//        createObject({7, 10, -7}, {0, 0, 0}, {5, 5, 5}, sun_model,  MaterialObj3);


//        our::mesh_utils::loadOBJ(Girl_model, "assets/models/kawai/Signora.obj");
//        meshes.push_back(&Girl_model);
//        createObject({2, 4, -3}, {0, 0, 0}, {0.01, 0.01, 0.01}, Girl_model, MaterialObj4);


        our::mesh_utils::loadOBJ(Monster_model, "assets/models/monster/monsterr.obj");
        meshes.push_back(&Monster_model);

        our::mesh_utils::loadOBJ(Monster_model2, "assets/models/monster3/monster.obj");
        meshes.push_back(&Monster_model2);
//        createPlayingObject({2, -2, -35}, {0, 0, 0}, {0.1, 0.1, 0.1}, Monster_model2, MaterialObj6, true);

        our::mesh_utils::loadOBJ(Gun_model, "assets/models/Gun/newgun.obj");
        meshes.push_back(&Gun_model);
        createObject({0, -1, -2}, {0, 0, 0}, {1, 1, 1}, Gun_model, MaterialObj7, CameraEntity);



        our::mesh_utils::loadOBJ(Bullet_model, "assets/models/Bullet/newbullet.obj");
        meshes.push_back(&Bullet_model);
//        createPlayingObject({0, -1, -1}, {0, 0, 0}, {0.3, 0.3, 0.3}, Bullet_model, MaterialObj8, false);
//        createPlayingObject({2, -1, -1}, {0, 0, 0}, {0.3, 0.3, 0.3}, Bullet_model, MaterialObj8, false);

//        createObject({25, 25, -3}, {0, 0, 5}, {1, 1, 1}, Bullet_model, MaterialObj8);
//        glSamplerParameteri(height_sampler->getSampler(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        glSamplerParameteri(height_sampler->getSampler(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//        // The height texture won't be repeated so no need to use repeat.
//        glSamplerParameteri(height_sampler->getSampler(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//        glSamplerParameteri(height_sampler->getSampler(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//        glEnable(GL_DEPTH_TEST);
//        glDepthFunc(GL_LEQUAL);
//
//        glEnable(GL_CULL_FACE);
//        glCullFace(GL_BACK);
//        glFrontFace(GL_CCW);
        glClearColor(0, 0, 0, 0);

    }

    void CreateMonster1(float x=0)
    {
//        our::mesh_utils::loadOBJ(Monster_model, "assets/models/monster/monster.obj");
//        meshes.push_back(&Monster_model);
        createPlayingObject({x, -2, -30}, {0, 0, 0}, {0.1, 0.1, 0.1}, Monster_model2, MaterialObj6, true);

    }

    void CreateMonster2(float x=0)
    {
//        our::mesh_utils::loadOBJ(Monster_model2, "assets/models/monster3/monster.obj");
//        meshes.push_back(&Monster_model2);
        createPlayingObject({x, -2, -30}, {0, 0, 0}, {1, 1, 1}, Monster_model, MaterialObj5, true);
    }

    void GenerateMonsters()
    {

        int result = 1 + (rand() % 3);
        switch (result) {
            case 1:
                if(currentLevel == 1)
                    CreateMonster1(-4);
                else
                    CreateMonster2(-4);
                break;
            case 2:
                if(currentLevel == 1)
                    CreateMonster1(0);
                else
                    CreateMonster2(0);
                break;
            case 3:
                if(currentLevel == 1)
                    CreateMonster1(4);
                else
                    CreateMonster2(4);
                break;
        }
    }

    void CreateBullet() {
        Entity *axis = new Entity(nullptr);
        TransformComponent *ts = new TransformComponent(axis);
        //components.push_back(ts);
        glm::vec3 pos = ((TransformComponent *) (CameraEntity)->getComponent("transform"))->position;
        glm::vec3 rot = ((TransformComponent *) (CameraEntity)->getComponent("transform"))->rotation;
        ts->init(app, pos, rot, {1, 1, 1});
        axis->addComponent(ts, "transform");
        Axes.push_back(axis);
        createPlayingObject({0, -0.5, -2}, {0, 0, 0}, {0.5, 0.5, 0.5}, Bullet_model, MaterialObj8, false, axis);
    }

    void onDraw(double deltaTime) override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        counter += deltaTime;
        if(counter >= delay && nGenerated <nMonsters)
        {
            GenerateMonsters();
            nGenerated++;
            counter =0;
        }

        for (int i = 0; i < Monsters.size(); i++) {
            glm::mat4 tr1mat = ((TransformComponent *) Monsters[i]->getComponent("transform"))->to_mat4();
            glm::vec3 MonsterPos = {tr1mat[3][0], tr1mat[3][1], tr1mat[3][2]};
            glm::vec3 pos = ((TransformComponent *) (CameraEntity)->getComponent("transform"))->position;
            if(MonsterPos[2] >= pos[2]) {
                lose = true;
//                win = true;
            }
        }
        if(app->getMouse().justPressed(GLFW_MOUSE_BUTTON_1))
        {
            CreateBullet();
        }
        for (int i = 0; i < Monsters.size(); i++)
        {
            glm::mat4 tr1mat = ((TransformComponent *) Monsters[i]->getComponent("transform"))->to_mat4();
            glm::vec3 MonsterPos = {tr1mat[3][0], tr1mat[3][1], tr1mat[3][2]};
//            glm::vec3 MonsterPos = ((TransformComponent *) (*it)->getComponent("transform"))->position;
//            for (auto it2 = Bullets.begin(); it2 != Bullets.end() ; ++it2)
//            for (auto it2 = Bullets.begin(); it2 != Bullets.end() ; ++it2)
            for (int j = 0; j < Bullets.size(); j++)
            {
                glm::mat4 tr2mat = ((TransformComponent *) (Bullets[j])->getComponent("transform"))->to_mat4();
                glm::vec3 BulletPos = {tr2mat[3][0], tr2mat[3][1], tr2mat[3][2]};;
//                glm::vec3 BulletPos = ((TransformComponent *) (*it2)->getComponent("transform"))->position;

//                float a = MonsterPos[2];
//                float b = BulletPos[2];
//                std::cout << a <<std::endl;
//                std::cout << b << std::endl;
                // TODO CHECK FOR X SCALE OF MONSTER
//                if(BulletPos[2] <= MonsterPos[2] && BulletPos[0] <= MonsterPos[0]+1.5 && BulletPos[0] >= MonsterPos[0]-1.5 && BulletPos[1] <=MonsterPos[1]+3 && BulletPos[1] >= MonsterPos[1]) //monster2

//                if(BulletPos[2] >= MonsterPos[2]-2 && BulletPos[2] <= MonsterPos[2] && BulletPos[0] <= MonsterPos[0]+2.3 && BulletPos[0] >= MonsterPos[0]-2.3 && BulletPos[1] <=MonsterPos[1]+6 && BulletPos[1] >= MonsterPos[1])//monster1
                if(BulletPos[2] >= MonsterPos[2]-zThresh && BulletPos[2] <= MonsterPos[2] && BulletPos[0] <= MonsterPos[0]+xThresh && BulletPos[0] >= MonsterPos[0]-xThresh && BulletPos[1] <=MonsterPos[1]+yThresh && BulletPos[1] >= MonsterPos[1])//monster1
                {
                    nKilled ++;
//                    auto it4 = Entities.begin(), it5 = Entities.begin();
                    for (auto it3 = Entities.begin(); it3 != Entities.end(); ++it3)
                    {



                        if(Monsters[i] == (*it3))
                        {
                            it3 = Entities.erase(it3);
                            it3 --;
                        }
                        if(Bullets[j] == (*it3))
                        {
                            it3 = Entities.erase(it3);
                            it3 --;
                        }


                    }
                    delete Bullets[j];
                    delete Axes[j];
                    delete Monsters[i];

                    Bullets.erase(Bullets.begin() + j);
                    Axes.erase(Axes.begin() + j);
                    j --;
                    Monsters.erase(Monsters.begin() + i);
                    i --;
                    break;


                }
            }


        }
//        std::cout<< Entities.size() << std::endl;
//        std::cout<< Monsters.size() << std::endl;
//        std::cout<< Bullets.size() << std::endl;

        if(!lose)
        {
            if(nKilled == nMonsters)
            {
                win = true;
            }
        }



        for (unsigned int i = 0; i < Monsters.size(); i++) {
                TransformComponent *transComponent = ((TransformComponent *) Monsters[i]->getComponent("transform"));
                transComponent->Move(Monsterspeed);
        }
        for (unsigned int i = 0; i < Bullets.size(); i++) {
            TransformComponent *transComponent = ((TransformComponent *) Bullets[i]->getComponent("transform"));
            transComponent->Move(-0.8);
        }


        glm::vec3 cameraPos = cameraController->getPosition();
        std::sort(Entities.begin()+1, Entities.end(), [&cameraPos]
                (Entity * e1, Entity * e2) -> bool {
            if(e1->getComponent("mesh") && e2->getComponent("mesh")) {
                RenderState *state1 =((MeshRendererComponent *)e1->getComponent("mesh"))->Materialpt->renderstate;
//                TransformComponent *tr1 = ((TransformComponent *) e1->getComponent("transform"));
//                TransformComponent *tr2 = ((TransformComponent *) e2->getComponent("transform"));
                glm::mat4 tr1mat = ((TransformComponent *) e1->getComponent("transform"))->to_mat4();
                glm::vec3 pos1 = {tr1mat[3][0], tr1mat[3][1], tr1mat[3][2]};
                glm::mat4 tr2mat = ((TransformComponent *) e2->getComponent("transform"))->to_mat4();
                glm::vec3 pos2 = {tr2mat[3][0], tr2mat[3][1], tr2mat[3][2]};
                RenderState *state2 = ((MeshRendererComponent *)e2->getComponent("mesh"))->Materialpt->renderstate;
                if(state1->blend && state2->blend) {
//                    float d1 = glm::length(tr1->position - cameraPos);
//                    float d2 = glm::length(tr2->position - cameraPos);
                    float d1 = glm::length(pos1 - cameraPos);
                    float d2 = glm::length(pos2 - cameraPos);
                    if(d1 <= d2) {
                        return false;
                    }
                    else
                        return true;
                } else if(state1->blend && !state2->blend) {
                    return false;
                }
                else if(!state1->blend && state2->blend)
                    return true;
                else {
//                    float d1 = glm::length(tr1->position - cameraPos);
//                    float d2 = glm::length(tr2->position - cameraPos);
                    float d1 = glm::length(pos1 - cameraPos);
                    float d2 = glm::length(pos2 - cameraPos);
                    if(d1 <= d2) {
                        return true;
                    }
                    return false;
                }
            }

        });


        cameraController->update(deltaTime);
        cameraMatrix = cameraComponent->getCameraMatrix();


        std::vector<our::ShaderProgram *> usedShaders;
        for (unsigned int i = 0; i < Entities.size(); i++)
        {

//
            //// modified here
            if (Entities[i]->getComponent("mesh"))
            {

                TransformComponent *tr1 = ((TransformComponent *) Entities[i]->getComponent("transform"));
                float d1 = glm::length(tr1->position - cameraPos);
                RenderState *state1 =((MeshRendererComponent *)Entities[i]->getComponent("mesh"))->Materialpt->renderstate;
                bool bl = state1->blend;
                our::ShaderProgram * S= ((MeshRendererComponent *)Entities[i]->getComponent("mesh"))->Materialpt->MatrixShader;
                Material * material = ((MeshRendererComponent *)Entities[i]->getComponent("mesh"))->Materialpt;
                Texture2D * albedo_map = material->albedo_map;
                Texture2D * specular_map =material->specular_map;
                Texture2D * roughness_map =material->roughness_map;
                Texture2D * emissive_map =material->emissive_map;
                Texture2D * ambient_occlusion_map =material->ambient_occlusion_map;
                Sampler * samp =material->sampler;


                glUseProgram(*S);
                if (!std::count(usedShaders.begin(), usedShaders.end(), S))
                {
                    usedShaders.push_back(S);
                    passLights(S);
                }



//                passLights(S);
                (*S).set("sampler", 0);
                (*S).set("lod", mml);
                (*S).set("zoom", zoom);
                //// BIND ALL SAMPLERS
                for(GLuint unit = 0; unit < 5; ++unit)
                    samp->AttachSample(unit);

//                S->set("camera_position", cameraComponent->getEyePosition());
//                S->set("view_projection", cameraComponent->getVPMatrix());
//                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                //// new added here
//                Text->LinkTexture();
//                samp->AttachSample();

                

                RenderState * state= ((MeshRendererComponent *)Entities[i]->getComponent("mesh"))->Materialpt->renderstate;
                if(!state->getDepthTesting())
                {
                    glDisable(GL_DEPTH_TEST);
                }
                else
                {
                    glEnable(GL_DEPTH_TEST);
                }
                if(!state->getCulling())
                {
                    glDisable(GL_CULL_FACE);
                }
                else
                {
                    glEnable(GL_CULL_FACE);
                }

                TransformComponent* transComponent = ((TransformComponent *)Entities[i]->getComponent("transform"));
                glm::mat4 transformationMatrix = transComponent->to_mat4();
//                glm::mat4 inverseview = glm::inverse(cameraComponent->getViewMatrix());
//                if(i == 1)
//                    transformationMatrix = glm::inverse(cameraComponent->getViewMatrix()) * transformationMatrix;
                glm::mat4 camTransformationMatrix =((CameraComponent *) Entities[0]->getComponent("camera"))->getCameraMatrix();


                (*S).set("object_to_world", transformationMatrix);
                (*S).set("object_to_world_inv_transpose", glm::inverse(transformationMatrix), true);
                (*S).set("material.albedo_tint", material->albedo_tint);
                (*S).set("material.specular_tint", material->specular_tint);
                (*S).set("material.roughness_range", material->roughness_range);
                (*S).set("material.emissive_tint", material->emissive_tint);

                albedo_map->LinkTexture(0);
                (*S).set("material.albedo_map", 0);

                specular_map->LinkTexture(1);
                (*S).set("material.specular_map", 1);

                ambient_occlusion_map->LinkTexture(2);
                (*S).set("material.ambient_occlusion_map", 2);

                roughness_map->LinkTexture(3);
                (*S).set("material.roughness_map", 3);

                emissive_map->LinkTexture(4);
                (*S).set("material.emissive_map", 4);


//                shader.set("light.diffuse", light.diffuse);
//                shader.set("light.specular", light.specular);
//                shader.set("light.ambient", light.ambient);
//                // TODO SET LIGHT DIRECTION AND POSITION FROM TRANSFORM COMPONENT
////                shader.set("light.direction", glm::normalize(light.direction));
//                shader.set("material.diffuse",MaterialObj->diffuse);
//                shader.set("material.specular", MaterialObj->specular);
//                shader.set("material.ambient", MaterialObj->ambient);
//                shader.set("material.shininess", MaterialObj->shininess);
//                shader.set("object_to_world", transformationMatrix);
//                shader.set("object_to_world_inv_transpose", glm::inverse(transformationMatrix), true);
                transformationMatrix=transformationMatrix* glm::inverse(camTransformationMatrix);


                ((MeshRendererComponent *)Entities[i]->getComponent("mesh"))->draw(transformationMatrix, cameraMatrix);
            }

        }

        glUseProgram(sky_program);

        // We don't need a model matrix for the box. Since it follows the camera, we will send the camera position and add it to the sky box vertices.
        sky_program.set("view_projection", cameraComponent->getVPMatrix());
        sky_program.set("camera_position", cameraComponent->getEyePosition());
        // We will then send the sky light to get the colors and the exposure to control how bright the sky will look
        sky_program.set("sky_light.top_color", sky_light.enabled ? sky_light.top_color : glm::vec3(0.0f));
        sky_program.set("sky_light.middle_color", sky_light.enabled ? sky_light.middle_color : glm::vec3(0.0f));
        sky_program.set("sky_light.bottom_color", sky_light.enabled ? sky_light.bottom_color : glm::vec3(0.0f));
        sky_program.set("exposure", sky_box_exposure);

        // Since we are inside the sky box and we are using a cube that was meant to be seen from the outside,
        // We will temporarily flip the culling to keep the back faces and remove the front faces.
        glCullFace(GL_FRONT);
        worldbox.draw();
        glCullFace(GL_BACK);

    }

    void createObject(glm::vec3 pos, glm::vec3 rot, glm::vec3 sc, our::Mesh &mesh, Material * material, Entity * parent =nullptr)
    {
        Entity* ent = new Entity(parent);
        TransformComponent * ts = new TransformComponent(ent);
        ts->init(app, pos, rot, sc);
        MeshRendererComponent* mrc = new MeshRendererComponent();
        mrc->init(material, &mesh);
        //components.push_back(mrc);
        //components.push_back(ts);
        ent->addComponent(mrc, "mesh");
        ent->addComponent(ts, "transform");
        Entities.push_back(ent);
    }

    void createPlayingObject(glm::vec3 pos, glm::vec3 rot, glm::vec3 sc, our::Mesh &mesh, Material * material, bool Monster, Entity * parent = nullptr)
    {
        Entity* ent = new Entity(parent);
        TransformComponent * ts = new TransformComponent(ent);
        ts->init(app, pos, rot, sc);
        MeshRendererComponent* mrc = new MeshRendererComponent();
        mrc->init(material, &mesh);
        //components.push_back(mrc);
        //components.push_back(ts);
        ent->addComponent(mrc, "mesh");
        ent->addComponent(ts, "transform");
        Entities.push_back(ent);
        if(Monster)
            Monsters.push_back(ent);
        else
            Bullets.push_back(ent);
    }

    void onExit() override
    {

        continue_game = true;

    }
    




};
LevelOne pState;
//LevelOne *Level2 = new LevelOne();

class MenuState : public GameState
{
    int width, height;
public:


    GameState *handleEvents()
    {

//        if (app->getKeyboard().justPressed(GLFW_KEY_ESCAPE))
//        {
//            return switchState(2);
//        }
        if (app->getMouse().justReleased(0))
        {
            glm::vec2 position = app->getMouse().getMousePosition();
            if (position[0] < (width/2))
            {
                return switchState(1);

            }
            else {
                //TODO:
                glfwDestroyWindow(app->window);

            }
        }
        return NULL;

    }
    void onEnter() override
    {

        glfwGetFramebufferSize(app->window, &width, &height);
        program = glCreateProgram(); // We ask GL to create a program for us and return a uint that we will use it by.
        // (act as a pointer to the created program).

        attachShader(program, "assets/shaders/phase1.vert", GL_VERTEX_SHADER);   // read the vertex shader and attach it to the program.
        attachShader(program, "assets/shaders/phase1.frag", GL_FRAGMENT_SHADER); // read the fragment shader and attach it to the program.

        glLinkProgram(program);             // Link the vertex and fragment shader together.
        checkProgramLinkingErrors(program); // Check if there is any link errors between the fragment shader and vertex shader.

        glGenVertexArrays(1, &vertex_array); // Ask GL to create a vertex array to easily create a triangle.

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set the clear color
    }
    void onDraw(double deltaTime) override
    {

        glClear(GL_COLOR_BUFFER_BIT);    // Clear the frame buffer (back buffer of the window)
        glUseProgram(program);           // Ask GL to use this program for the upcoming operations.
        // Every shader and rendering call after glUseProgram will now use this program object (and the shaders).
        glBindVertexArray(vertex_array); // Binding is like selecting which object to use.
        // Note that we need to bind a vertex array to draw
        // Even if that vertex array does not send any data down the pipeline
//        if (app->getMouse().justReleased(0))
//        {
//            if (app->getMouse().getMousePosition()[0] < (width/2))
//                switchState(pState);
//            else {
//                //TODO:
//                glfwDestroyWindow(app->window);
//
//            }
//        }
        //Following Code is for phase 1
        if (app->getKeyboard().justPressed(GLFW_KEY_1))
            keyShaderOption = 1;
        else if (app->getKeyboard().justPressed(GLFW_KEY_2))
            keyShaderOption = 2;
        else if (app->getKeyboard().justPressed(GLFW_KEY_2))
           keyShaderOption = 3;
        else if (app->getKeyboard().justPressed(GLFW_KEY_2))
            keyShaderOption = 4;

        GLuint keyboard_uniform_location = glGetUniformLocation(program, "keyboard");
        glUniform1i(keyboard_uniform_location, keyShaderOption);

        mouse = app->getMouse().getMousePosition();
        GLuint mouse_uniform_location = glGetUniformLocation(program, "mouse");
        glUniform2f(mouse_uniform_location, mouse.x, mouse.y);
        // Sends vertices down the pipeline for drawing
        // Parameters:
        // mode (GLenum): what primitives to draw. GL_TRIANGLES will combine each 3 vertices into a triangle.
        // first (GLint): the index of the first vertex to draw. It is useless here since we are not receiving data through the vertex array.
        // count (GLsizei): How many vertices to send to the pipeline. Since we are sending 3 vertices only, only one triangle will be drawn.
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindVertexArray(0); // Unbind the buffer.
    }
    void onExit() override
    {
        glDeleteProgram(program);               // Cleaning up the program we compiled and saved.
        glDeleteVertexArrays(1, &vertex_array); // Clean up the array we allocated Params: (n: number of vertex array objects, array containing the n addresses of the objects)
    }
};

MenuState mState;


GameState *switchState(int state)
{
    if (state == 0) {
//        mState = new MenuState();

        return &mState;
    }
//    pState = new LevelOne();

        return &pState;


    // TODO CHECK CULLING ON SWITCH
}



int main(int argc, char **argv)
{
    // ShaderIntroductionApplication().gotoState(menu);
    app->gotoState(&mState);

    // return ShaderIntroductionApplication().run();
    return app->run();
//    delete pState,mState;
}
