# include "gameObject.h"

gameObject::gameObject()
{
	m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_Scale = glm::vec3(0.6, 0.6, 0.6);
	m_Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	m_ModelMatrix = glm::mat4(1.0f);

	m_DiffuseMap = 0;

	m_AmbientMaterialColour = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	m_DiffuseMaterialColour = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
	m_SpecularMaterialColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_SpecularPower = 25.0f;

	m_ShaderProgramID = 0;
}

gameObject::~gameObject()
{
}

void gameObject::loadMeshesFromFile(const std::string & fileName)
{
	loadMeshFromFile(fileName, m_Meshes);
}

void gameObject::loadDiffuseTextureFromFile(const std::string & filename)
{
	m_DiffuseMap = loadTextureFromFile(filename);
}

void gameObject::update()
{
	glm::mat4 translationMatrix = glm::translate(m_Position);
	glm::mat4 scaleMatrix = glm::scale(m_Scale);
	glm::mat4 rotationXMatrix = glm::rotate(m_Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rotationYMatrix = glm::rotate(m_Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotationZMatrix = glm::rotate(m_Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 rotationMatrix = rotationZMatrix*rotationYMatrix*rotationXMatrix;

	m_ModelMatrix = translationMatrix*rotationMatrix*scaleMatrix;
}

void gameObject::loadShaderProgram(const std::string & vertexShaderFileName, const std::string & fragmentShaderFileName)
{
	m_ShaderProgramID = LoadShaders(vertexShaderFileName.c_str(), fragmentShaderFileName.c_str());
}

void gameObject::destroy()
{
	glDeleteTextures(1, &m_DiffuseMap);
	glDeleteProgram(m_ShaderProgramID);
	auto iter = m_Meshes.begin();
	while (iter != m_Meshes.end())
	{
		if ((*iter))
		{
			delete (*iter);
			iter = m_Meshes.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

void gameObject::preRender()
{
	glUseProgram(m_ShaderProgramID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_DiffuseMap);

	GLint modelMatrixLocation = glGetUniformLocation(m_ShaderProgramID, "modelMatrix");
	GLint textureLocation = glGetUniformLocation(m_ShaderProgramID, "baseTexture");

	GLint ambientMaterialColourLocation = glGetUniformLocation(m_ShaderProgramID, "ambientMaterialColour");
	GLint diffuseMaterialColourLocation = glGetUniformLocation(m_ShaderProgramID, "diffuseMaterialColour");
	GLint specularMaterialColourLocation = glGetUniformLocation(m_ShaderProgramID, "specularMaterialColour");
	GLint specularPowerLocation = glGetUniformLocation(m_ShaderProgramID, "specularPower");

	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(m_ModelMatrix));

	glUniform1i(textureLocation, 0);

	glUniform4fv(ambientMaterialColourLocation, 1, value_ptr(m_AmbientMaterialColour));
	glUniform4fv(diffuseMaterialColourLocation, 1, value_ptr(m_DiffuseMaterialColour));
	glUniform4fv(specularMaterialColourLocation, 1, value_ptr(m_SpecularMaterialColour));
	glUniform1f(specularPowerLocation, m_SpecularPower);
}

void gameObject::render()
{
	for (Mesh *pMesh : m_Meshes)
	{
		pMesh->render();
	}
}
