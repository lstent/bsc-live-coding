#pragma once

#include <vector>
#include <string>

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <btBulletDynamicsCommon.h>

#include "Model.h"
#include "Texture.h"
#include "Shader.h"

class gameObject
{
public:
	gameObject() : collision(this)
	{
		m_DiffuseMap = 0;
		m_SpecularPower = 25.0f;
		m_ShaderProgramID = 0;
	}
	~gameObject();

	void loadMeshesFromFile(const std::string& filename);
	void loadDiffuseTextureFromFile(const std::string& filename);
	void loadShaderProgram(const std::string& vertexShaderFileName, const std::string& fragmentShaderFileName);

	void destroy();
	void preRender();
	void render();

	class collision
	{
	public:
		collision(gameObject*gameObjectsRef) : gameObjectsRef(gameObjectsRef) {}
		collision();
		void inertia(float x,float y,float z);
		void mass(float mass);
		void collisionBox(float x, float y, float z);
		void startCollision(float originX, float originY, float originZ);
		btRigidBody* getRigidBody();
		void collisionUpdate();
		void collisionDelete();

	private:
		gameObject* gameObjectsRef;
		btCollisionShape* nutCollisionShape;
		btVector3 nutInertia;
		btScalar nutMass;
		btTransform nutTransform;
		btDefaultMotionState* nutMotionState;
		btRigidBody* nutRigidBody;
		btVector3 nutOrigin;
		btQuaternion nutRotation;
		
	}collision;

	class transform
	{
	public:
		transform();
		transform(gameObject*gameObjectsRef) : gameObjectsRef(gameObjectsRef) {}
		~transform();
		void update();

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
	private:
		gameObject* gameObjectsRef;

		//Transform
		glm::vec3 m_Position;
		glm::vec3 m_Scale;
		glm::vec3 m_Rotation;
		glm::mat4 m_ModelMatrix;
	}transform;

	class material
	{
	public:
			material();
			material(gameObject*gameObjectsRef) : gameObjectsRef(gameObjectsRef) {}
			~material();

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


	private:
		gameObject* gameObjectsRef;

		//material
		glm::vec4 m_AmbientMaterialColour;
		glm::vec4 m_DiffuseMaterialColour;
		glm::vec4 m_SpecularMaterialColour;

	}material;

	//Gets the diffuseMap
	const GLuint getDiffuseMap()
	{
		return m_DiffuseMap;
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

	//Textures
	GLuint m_DiffuseMap;

	float m_SpecularPower;

	GLuint m_ShaderProgramID;
};
