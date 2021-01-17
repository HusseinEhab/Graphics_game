#include <shader.hpp>
#include <utility>
#include <imgui-utils/utils.hpp>
#include <mesh/mesh.hpp>
#include <mesh/mesh-utils.hpp>
#include <string>
#include "Component.h"
#include "Material.h"

class MeshRendererComponent : public Component{
   public:
        our::Mesh * meshPtr;
        Material* Materialpt;
        void init(Material* Materialptr,our::Mesh *y);
        void draw(glm::mat4 transformationMatrix,glm::mat4 cameraMatrix);

};