#pragma once

#include <vector>
#include <string>

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Mesh.h"
#include "Model.h"
#include "Texture.h"
#include "Shader.h"

class gameObject
{
public:
	gameObject();
	~gameObject();

	void loadMeshesFromFile(const std::string& filename);
	void loadDiffuseTextureFromFile(const std::string& filename);
	void loadShaderProgram(const std::string& vertexShaderFileName, const std::string& fragmentShaderFileName);
	
	void update();
	void destroy();
	void preRender();
	void render();

	//Sets the position of the game object
	void setPosition(const glm::vec3& position)
	{
		m_Position = position;
	};

	//Gets the position
	const glm::vec3& getPosition()
	{
		return m_Position;
	};

	//Sets the scale of the game object
	void setScale(const glm::vec3& scale)
	{
		m_Scale = scale;
	};

	//Gets the scale
	const glm::vec3& getScale()
	{
		return m_Scale;
	};

	//Sets the rotation of the game object
	void setRotation(const glm::vec3& rotation)
	{
		m_Rotation = rotation;
	};

	//Gets the rotation
	const glm::vec3& getRotation()
	{
		return m_Rotation;
	};

	//Gets the modelMatrix
	const glm::mat4& getModelMatrix()
	{
		return m_ModelMatrix;
	};

	//Gets the diffuseMap
	const GLuint getDiffuseMap()
	{
		return m_DiffuseMap;
	};

	//Sets the ambient material colour for the game object
	void setAmbientMaterialColour(const glm::vec4& colour)
	{
		m_AmbientMaterialColour;
	};
	//gets the ambient materal colour
	const glm::vec4& getAmbientMaterialColour()
	{
		return m_AmbientMaterialColour;
	};

	//Sets the diffuse material colour for the game object
	void setDiffuseMaterialColour(const glm::vec4& colour)
	{
		m_DiffuseMaterialColour;
	};
	//gets the diffuse materal colour
	const glm::vec4& getDiffuseMaterialColour()
	{
		return m_DiffuseMaterialColour;
	};

	//Sets the specular material colour for the game object
	void setSpecularMaterialColour(const glm::vec4& colour)
	{
		m_SpecularMaterialColour;
	};
	//gets the specular materal colour
	const glm::vec4& getspecularMaterialColour()
	{
		return m_SpecularMaterialColour;
	};

	//Sets the specular power for the game object
	void setSpecularPower(const float power)
	{
		m_SpecularPower = power;
	};

	//gets the specular power
	const float getSpecularPower()
	{
		return m_SpecularPower;
	};

	//gets the shader program ID
	const GLuint getShaderProgramID()
	{
		return m_ShaderProgramID;
	};

private:
	//The visable mesh
	std::vector<Mesh*> m_Meshes;

	//Transform
	glm::vec3 m_Position;
	glm::vec3 m_Scale;
	glm::vec3 m_Rotation;
	glm::mat4 m_ModelMatrix;

	//Textures
	GLuint m_DiffuseMap;

	//material
	glm::vec4 m_AmbientMaterialColour;
	glm::vec4 m_DiffuseMaterialColour;
	glm::vec4 m_SpecularMaterialColour;
	float m_SpecularPower;

	GLuint m_ShaderProgramID;
};
