#pragma once

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include <string>
#include <vector>

#include <GL\glew.h>
#include <SDL_opengl.h>

#include "vertex.h"
#include "Mesh.h"

bool loadMeshFromFile(const std::string& filename, std::vector<Mesh*>& meshes);