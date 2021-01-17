
#include "Entity.h"
#include "../Component/Component.h"
void Entity::addComponent(Component* component,std:: string name) {

    if(this->components.find(name) != this->components.end()) {
        return; }
    this->components[name] = component;

}

Component* Entity::getComponent(std:: string name) {

    if (components.find(name) != components.end())
    {
        return this->components.at(name);
    }
    return nullptr;

}

void Entity::removeComponent(std::string name) {

    if (components.find(name) != components.end()) {
        this->components.erase(name);
    }
}
Entity:: Entity( Entity * p)
{

    parent =p;
    id =  entityCount ;
    entityCount ++;

}
int Entity:: entityCount = 0;
void Entity:: update()
{

}
int Entity::getId() {
    return this->id;
}
void Entity::setId(int id) {
    this->id =id;
}
Entity::~Entity() {
    auto iterator = this->components.begin();
    for(; iterator != this->components.end(); ++iterator) {
        delete iterator->second;
    }
}
