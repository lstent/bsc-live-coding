//main.cpp - defines the entry point of the application

#include "main.h"

int main(int argc, char* args[])
{
	//Initialises the SDL Library, passing in SDL_INIT_VIDEO to only initialise the video subsystems
	//https://wiki.libsdl.org/SDL_Init
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		//Display an error message box
		//https://wiki.libsdl.org/SDL_ShowSimpleMessageBox
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SDL_GetError(), "SDL_Init failed", NULL);
		return 1;
	}

	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

	//Create a window, note we have to free the pointer returned using the DestroyWindow Function
	//https://wiki.libsdl.org/SDL_CreateWindow
	SDL_Window* window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 700, 700, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	//Checks to see if the window has been created, the pointer will have a value of some kind
	if (window == nullptr)
	{
		//Show error
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SDL_GetError(), "SDL_CreateWindow failed", NULL);
		//Close the SDL Library
		//https://wiki.libsdl.org/SDL_Quit
		SDL_Quit();
		return 1;
	}

	//lets ask for a 3.2 core profile version of OpenGL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GLContext GL_Context = SDL_GL_CreateContext(window);
	if (GL_Context == nullptr)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SDL_GetError(), "SDL GL Create Context failed", NULL);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}
	//Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, (char*)glewGetErrorString(glewError), "GLEW Init Failed", NULL);
	}

	//loadModelFromFile("cube.nff", );

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// This will identify our vertex buffer
	GLuint vertexbuffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	//Give our vertices to OpenGL
	//glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(Vertex), triangleVertices, GL_STATIC_DRAW);

	GLuint elementbuffer;
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36*sizeof(unsigned int),triangleIndices,GL_STATIC_DRAW);

	std::vector<Mesh*> meshes;
	loadMeshFromFile("Large_Oak_Dark_01.obj", meshes);

	GLuint textureID = loadTextureFromFile("Giraffe.jpg");

	vec3 trianglePosition = vec3(-1.0f, -2.0f, -10.0f);
	vec3 scalingValue = vec3(0.8, 0.8, 0.8);
	mat4 translationMatrix = translate(trianglePosition);
	mat4 scaleMatrix = scale(scalingValue);

	vec3 triangleRotation = vec3(0.0f, 0.0f, 0.0f);
	mat4 rotationXMatrix = rotate(triangleRotation.x, vec3(1.0f, 0.0f, 0.0f));
	mat4 rotationYMatrix = rotate(triangleRotation.y, vec3(0.0f, 1.0f, 0.0f));
	mat4 rotationZMatrix = rotate(triangleRotation.z, vec3(0.0f, 0.0f, 1.0f));
	mat4 rotationMatrix = rotationZMatrix*rotationYMatrix*rotationXMatrix;

	mat4 modelMatrix = translationMatrix*rotationMatrix*scaleMatrix;

	vec3 cameraPosition = vec3(0.0f, 0.0f, -5.0f);
	vec3 cameraTarget = vec3(0.0f, 0.0f, 0.0f);
	vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

	mat4 viewMatrix = lookAt(cameraPosition, cameraTarget, cameraUp);

	mat4 projectionMatrix = perspective(radians(90.0f), float(800 / 600), 0.1f, 100.0f);

	//light
	vec3 lightDirection = vec3(0.0f, 0.0f, 1.0f);
	vec4 diffuseLightColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);

	//material
	vec4 diffuseMaterialColour = vec4(0.8f, 0.8f, 0.8f, 1.0f);

	glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

	// position
	glm::vec3 position = glm::vec3(0, 0, 5);
	// horizontal angle : toward -Z
	float horizontalAngle = 3.14f;
	// vertical angle : 0, look at the horizon
	float verticalAngle = 0.0f;
	// Initial Field of View
	float initialFoV = 45.0f;

	float speed = 3.0f; // 3 units / second
	float mouseSpeed = 0.005f;

	int xpos=0, ypos=0;



	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	double deltaTime = 0;

	// Give our vertices to OpenGL.
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint programID = LoadShaders("lightingVert.glsl", "lightingFrag.glsl");
	if (programID < 0)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, (char*)glewGetErrorString(glewError), "LoadShaders Failed", NULL);
	}	

	GLuint timeLocation = glGetUniformLocation(programID, "time");
	GLint modelMatrixLocation = glGetUniformLocation(programID, "modelMatrix");
	GLint viewMatrixLocation = glGetUniformLocation(programID, "viewMatrix");
	GLint projectionMatrixLocation = glGetUniformLocation(programID, "projectionMatrix");
	GLint textureLocation = glGetUniformLocation(programID, "baseTexture");
	GLint lightDirectionLocation = glGetUniformLocation(programID, "lightDirection");
	GLint diffuseLightColourLocation = glGetUniformLocation(programID, "diffuseLightColour");
	GLint diffuseMaterialColourLocation = glGetUniformLocation(programID, "diffuseMaterialColour");

	//setup post-processing
	//GLuint colourBufferTextureID = createTexture(700, 700);

	//GLuint depthBufferID;
	//glGenRenderbuffers(1, &depthBufferID);
	//glBindRenderbuffer(GL_RENDERBUFFER, depthBufferID);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 700, 700);

	//GLuint frameBufferID;
	//glGenFramebuffers(1, &frameBufferID);
	//glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colourBufferTextureID, 0);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);

	//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	//{
		//SDL_ShowSimpleMessageBox(SDL_MESSSAGEBOX_ERROR,"unable to create Framebuffer","FramBuffer",)
	//}


	glEnable(GL_DEPTH_TEST);
	int lastTicks = SDL_GetTicks();
	int currentTicks = SDL_GetTicks();

	//Event loop, we will loop until running is set to false, usually if escape has been pressed or window is closed
	bool running = true;
	//SDL Event structure, this will be checked in the while loop
	SDL_Event ev;
	while (running)
	{
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();
		deltaTime = (double)((NOW - LAST) * 1000 / SDL_GetPerformanceFrequency());

		//Poll for the events which have happened in this frame
		//https://wiki.libsdl.org/SDL_PollEvent
		while (SDL_PollEvent(&ev))
		{
			//Switch case for every message we are intereted in
			switch (ev.type)
			{
			//case SDL_MOUSEMOTION:
				//break;

				//QUIT Message, usually called when the window has been closed
			case SDL_QUIT:
				running = false;
				break;

				//KEYDOWN Message, called when a key has been pressed down
			case SDL_KEYDOWN:
				//Check the actual key code of the key that has been pressed
				switch (ev.key.keysym.sym)
				{
					//W key
				case SDLK_w:
					cameraPosition.z += 0.1;
					cameraTarget.z += 0.1;
					break;

					//S key
				case SDLK_s:
					cameraPosition.z -= 0.1;
					cameraTarget.z -= 0.1;
					break;

					//A key
				case SDLK_a:
					cameraPosition.x -= 0.1;
					cameraTarget.x -= 0.1;
					break;

					//D key
				case SDLK_d:
					cameraPosition.x += 0.1;
					cameraTarget.x += 0.1;
					break;

					//Escape key
				case SDLK_ESCAPE:
					running = false;
					break;
				}
			}
		}
		
		//int SDL_SetRelativeMouseMode(SDL_bool enabled);
		
		SDL_GetRelativeMouseState(&xpos, &ypos);

		printf("Mouse %i %i\n", xpos, ypos);


		// Compute new orientation
		horizontalAngle += mouseSpeed * deltaTime * float(xpos);
		verticalAngle += mouseSpeed * deltaTime * float(ypos);

		// Direction : Spherical coordinates to Cartesian coordinates conversion
		glm::vec3 direction(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle) 
		);

		// Right vector
		glm::vec3 right = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
		);

		// Up vector : perpendicular to both direction and right
		glm::vec3 up = glm::cross(right, direction);

		viewMatrix = lookAt(cameraPosition,cameraPosition + direction, cameraUp);

		float currentTicks = SDL_GetTicks();
		float deltaTime = (float)(currentTicks - lastTicks) / 1000.0f;

		//Do rendering here
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

		glUseProgram(programID);

		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(modelMatrix));
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, value_ptr(viewMatrix));
		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, value_ptr(projectionMatrix));
		glUniform1f(timeLocation, (float)(currentTicks) / 1000.0f);
		glUniform1i(textureLocation, 0);

		glUniform3fv(lightDirectionLocation,1,value_ptr(lightDirection));
		glUniform4fv(diffuseLightColourLocation, 1, value_ptr(diffuseLightColour));

		glUniform4fv(diffuseMaterialColourLocation,1,value_ptr(diffuseMaterialColour));

		for (Mesh *pMesh : meshes)
		{
			pMesh->render();
		}

		SDL_GL_SwapWindow(window);

		//lastTicks = currentTicks;
	}

	//GLDelete(1, &)
	auto iter = meshes.begin();
	while (iter != meshes.end())
	{
		if ((*iter))
		{
			delete (*iter);
			iter = meshes.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	meshes.clear();
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &elementbuffer);
	glDeleteTextures(1, &textureID);
	glDeleteProgram(programID);

	SDL_GL_DeleteContext(GL_Context);
	//Destroy the window and quit SDL2, NB we should do this after all cleanup in this order!!!
	//https://wiki.libsdl.org/SDL_DestroyWindow
	SDL_DestroyWindow(window);
	//Unload image
	IMG_Quit;
	//https://wiki.libsdl.org/SDL_Quit
	SDL_Quit();

	return 0;
}