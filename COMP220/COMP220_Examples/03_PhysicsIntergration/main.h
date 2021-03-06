#pragma once

#include <string>
#include <vector>
#include <fstream>

#include <SDL.h>
#include <gl\glew.h>
#include <SDL_opengl.h>

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <btBulletDynamicsCommon.h>

#include "vertex.h"
#include "Shader.h"
#include "Texture.h"
#include "Model.h"

#include "gameObject.h"

bool collisionOn = false;

using namespace glm;
