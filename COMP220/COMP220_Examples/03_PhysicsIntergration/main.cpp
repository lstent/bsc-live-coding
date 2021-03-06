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

	//Load in the Giraffe JPG texture
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

	//Camera
	vec3 cameraPosition = vec3(0.0f, 2.0f, 10.0f);
	vec3 cameraTarget = vec3(0.0f, 0.0f, 0.0f);
	vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

	mat4 viewMatrix = lookAt(cameraPosition, cameraTarget, cameraUp);
	mat4 projectionMatrix = perspective(radians(90.0f), float(800 / 640), 0.1f, 100.0f);

	//light
	vec4 ambientLightColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec3 lightDirection = vec3(0.0f, 0.0f, 1.0f);
	vec4 diffuseLightColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec4 specularLightColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);

	std::vector<gameObject*>gameObjectList;

	//Create game object
	gameObject * pNut = new gameObject();
	pNut->transform.setPosition(vec3(0.0f, 6.0f, 0.0f));
	pNut->loadMeshesFromFile("Large_Oak_Dark_01.obj");
	pNut->loadDiffuseTextureFromFile("Giraffe.jpg");
	pNut->loadShaderProgram("textureVert.glsl", "textureFrag.glsl");
	pNut->collision.collisionBox(0.5,0.5,0.5);
	pNut->collision.mass(1.0f);
	pNut->collision.inertia(0,0,0);
	pNut->collision.startCollision(0.0f, 7.0f, 0.0f);
	gameObjectList.push_back(pNut);

	gameObject * pShroom = new gameObject();
	pShroom->transform.setPosition(vec3(5.0f, 0.0f, 0.0f));
	pShroom->transform.setScale(vec3(7.0f, 7.0f, 7.0f));
	pShroom->loadMeshesFromFile("Mushroom_Red_01.obj");
	pShroom->loadDiffuseTextureFromFile("iDunno.jpg");
	pShroom->loadShaderProgram("textureVert.glsl", "textureFrag.glsl");
	pShroom->collision.collisionBox(0.0f, 0.0f, 0.0f);
	pShroom->collision.mass(0.0f);
	pShroom->collision.inertia(0, 0, 0);
	pShroom->collision.startCollision(5.0f, 0.0f, 0.0f);
	gameObjectList.push_back(pShroom);

	gameObject * pTree = new gameObject();
	pTree->transform.setPosition(vec3(-5.0f, 0.0f, 0.0f));
	pTree->transform.setScale(vec3(7.0f, 7.0f, 7.0f));
	pTree->loadMeshesFromFile("Oak_Dark_01.obj");
	pTree->loadDiffuseTextureFromFile("wood.jpg");
	pTree->loadShaderProgram("textureVert.glsl", "textureFrag.glsl");
	pTree->collision.collisionBox(0.0f, 0.0f, 0.0f);
	pTree->collision.mass(0.0f);
	pTree->collision.inertia(0, 0, 0);
	pTree->collision.startCollision(-5.0f, 0.0f, 0.0f);
	gameObjectList.push_back(pTree);

	gameObject * pGrass = new gameObject();
	pGrass->transform.setPosition(vec3(-70.0f, 0.0f, 15.0f));
	pGrass->transform.setScale(vec3(187.0f, 1.0f, 187.0f));
	pGrass->loadMeshesFromFile("Plate_Grass_01.obj");
	pGrass->loadDiffuseTextureFromFile("feather.jpg");
	pGrass->loadShaderProgram("textureVert.glsl", "textureFrag.glsl");
	pGrass->collision.collisionBox(187.0f, 0.0f, 187.0f);
	pGrass->collision.mass(0.0f);
	pGrass->collision.inertia(0, 0, 0);
	pGrass->collision.startCollision(-70.0f, 0.0f, 15.0f);
	gameObjectList.push_back(pGrass);

	//colour buffer texture
	GLuint colourBufferID = createTexture(700, 700);

	//create depth buffer
	GLuint depthRenderBufferID;
	glGenRenderbuffers(1, &depthRenderBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 700, 700);

	//create framebuffer
	GLuint frameBufferID;
	glGenFramebuffers(1, &frameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBufferID);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colourBufferID, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "unable to create frame buffer for post processing", "Frame buffer error", NULL);
	}

	//create screen alligned quad
	GLfloat screenVerts[] =
	{
		-1, -1,
		1, -1,
		-1, 1,
		1, 1
	};

	GLuint screenQuadVBOID;
	glGenBuffers(1, &screenQuadVBOID);
	glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBOID);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), screenVerts, GL_STATIC_DRAW);

	GLuint screenVAO;
	glGenVertexArrays(1, &screenVAO);
	glBindVertexArray(screenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBOID);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	GLuint postProcessingProgramID = LoadShaders("passThroughVert.glsl", "postWaterEffect.glsl");
	//GLuint postProcessingProgramID = LoadShaders("passThroughVert.glsl", "postBlackAndWhite.glsl");
	GLint texture0Location = glGetUniformLocation(postProcessingProgramID, "texture0");
	GLint offSetLocation = glGetUniformLocation(postProcessingProgramID, "offSet");

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

	glEnable(GL_DEPTH_TEST);
	int lastTicks = SDL_GetTicks();
	int currentTicks = SDL_GetTicks();
	
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -1, 0));

	for (gameObject*pObj : gameObjectList)
	{
		dynamicsWorld->addRigidBody(pObj->collision.getRigidBody());
	}

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

				//QUIT Message, usually called when the window has been closed
			case SDL_QUIT:
				running = false;
				break;

				//KEYDOWN Message, called when a key has been pressed down
			case SDL_KEYDOWN:
				//Check the actual key code of the key that has been pressed
				switch (ev.key.keysym.sym)
				{
					//Space key
				case SDLK_SPACE:
					collisionOn = true;
					break;

					//W key
				case SDLK_w:
					cameraPosition.z -= 0.1;
					cameraTarget.z -= 0.1;
					break;

					//S key
				case SDLK_s:
					cameraPosition.z += 0.1;
					cameraTarget.z += 0.1;
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
		
		// mouse position
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

		currentTicks = SDL_GetTicks();
		float deltaTime = (float)(currentTicks - lastTicks) / 1000.0f;

		if (collisionOn == true)
		{
			dynamicsWorld->stepSimulation(1.f / 60.f, 10);
		}

		//Do rendering here
		glEnable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		for (gameObject*pObj : gameObjectList)
		{
			if (collisionOn == true)
			{
				pObj->collision.collisionUpdate();
			}
			
			pObj->transform.update();
			pObj->preRender();
			GLuint currentProgramID = pObj->getShaderProgramID();

			//Retreive the shader values
			GLint viewMatrixLocation = glGetUniformLocation(currentProgramID, "viewMatrix");
			GLint projectionMatrixLocation = glGetUniformLocation(currentProgramID, "projectionMatrix");

			//Send shader values
			glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, value_ptr(projectionMatrix));
			glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, value_ptr(viewMatrix));

			pObj->render();

		}

		//Draw stuff here
		glDisable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		//bind post processing shaders
		glUseProgram(postProcessingProgramID);

		//activate texture unit 0 for the colour buffer
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colourBufferID);
		glUniform1i(texture0Location, 0);
		glUniform1f(offSetLocation, deltaTime);

		glBindVertexArray(screenVAO);
		glDrawArrays(GL_TRIANGLE_STRIP,0,4);

		SDL_GL_SwapWindow(window);

		lastTicks = currentTicks;
	}

	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject*obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody*body = btRigidBody::upcast(obj);
		if (body&&body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	//delete dynamics world
	delete dynamicsWorld;

	//delete solver
	delete solver;

	//delete broadphase
	delete overlappingPairCache;

	//delete dispatcher
	delete dispatcher;

	delete collisionConfiguration;

	auto gameObjectIter = gameObjectList.begin();
	while (gameObjectIter != gameObjectList.end())
	{
		if ((*gameObjectIter))
		{
			(*gameObjectIter)->destroy();
			delete(*gameObjectIter);
			gameObjectIter = gameObjectList.erase(gameObjectIter);
		}
	}
	glDeleteProgram(postProcessingProgramID);
	glDeleteVertexArrays(1, &screenVAO);
	glDeleteBuffers(1, &screenQuadVBOID);
	glDeleteFramebuffers(1, &frameBufferID);
	glDeleteRenderbuffers(1, &depthRenderBufferID);
	glDeleteTextures(1, &colourBufferID);

	SDL_GL_DeleteContext(GL_Context);
	//Destroy the window and quit SDL2, NB we should do this after all cleanup in this order!!!
	//https://wiki.libsdl.org/SDL_DestroyWindow
	SDL_DestroyWindow(window);
	//Unload image
	IMG_Quit();
	//https://wiki.libsdl.org/SDL_Quit
	SDL_Quit();

	return 0;
}