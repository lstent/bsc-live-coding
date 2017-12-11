#pragma once

#include <vector>

#include "Mesh.h"

class gameObject
{
public:
	gameObject();
	~gameObject();
private:
	std::vector<Mesh*> m_Meshes;
};
