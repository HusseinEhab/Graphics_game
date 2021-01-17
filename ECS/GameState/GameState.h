 #ifndef GAMESTATE_H
#define GAMESTATE_H
#include <iostream>
#include <fstream>
#include <cassert>
#include "input/keyboard.hpp"
#include "input/mouse.hpp"
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
 class GameState {

      public:
        virtual void onEnter();
        virtual void onDraw(double deltaTime);
        virtual void onExit();
        virtual GameState* handleEvents();

};
#endif