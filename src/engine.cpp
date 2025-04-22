#include <engine.h>

void Engine::init(std::string title, float width, float height)
{
    // glfw initialization and configure
    if (glfwInit() != GLFW_TRUE) {
        Logger::print(LOG_ERROR, "Failed to initialize GLFW");
        std::exit(-1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    // glfw window creation
    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (window == NULL)
    {
        Logger::print(LOG_ERROR, "Failed to create GLFW window");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    this->width = width;
    this->height = height;

    // glew initialization
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        Logger::print(LOG_ERROR, "Failed to initialize GLEW");
        exit(EXIT_FAILURE);
    }

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetWindowUserPointer(window, static_cast<void *>(this));

    std::vector<int> keys = {
        GLFW_KEY_ESCAPE,
        GLFW_KEY_E,
        GLFW_KEY_A,
        GLFW_KEY_D,
        GLFW_KEY_S,
        GLFW_KEY_W,
        GLFW_KEY_LEFT_SHIFT,
        GLFW_KEY_SPACE,
    };

    Input::getSingleton()->registerKeys(keys);
    Input::getSingleton()->setCallbacks(window);

    // load renderer and models textures and so forth
    renderer.init(window);

    loadAssets();

    renderer.setupBuffers();

    isInitialized = true;
}


void Engine::run()
{
    if (!isInitialized) {
        Logger::print(LOG_ERROR, "Engine is not initialized!");
        return;
    }

    while (!glfwWindowShouldClose(window)) {
        update();
        render();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    destroy();
}

void Engine::destroy()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    ResourceManager::clear();
    glfwTerminate();
}

void Engine::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), width / float(height), 0.1f, 100.0f);

    if (auto meshShader = ResourceManager::getShader("mesh"); meshShader != nullptr) {
        meshShader->use();
        meshShader->setMatrix("view", view);
        meshShader->setMatrix("projection", projection);
        meshShader->setInt("useTex", true);
    } else {
        Logger::print(LOG_WARNING, "Failed to get main shader.");
    }

    renderer.renderModel(ResourceManager::getShader("mesh"), ResourceManager::getModel("sponza"));
    // renderer.renderText(ResourceManager::getShader("text"), ResourceManager::getFont("font"), "Test", vec2(10, 10), 20, vec3(1, 0, 0));
    renderer.renderImGui();
}

void Engine::update()
{
    // get deltaTime
    float currentFrame = glfwGetTime();
    static double lastFrame;
    double deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    camera.update(deltaTime);
    renderer.update();

    updateInput(deltaTime);
}

void Engine::updateInput(double dt)
{
    Input *input = Input::getSingleton();

    // input update
    if (input->isKeyPressed(GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, true);
    }

    camera.update(dt);

    // update previous input state
    input->update();
}

void Engine::loadAssets()
{
    // shaders
    ResourceManager::loadShader("mesh", "mesh.vert", "mesh.frag");
    ResourceManager::loadShader("text", "text.vert", "text.frag");

    // fonts
    ResourceManager::loadFont("font", "Tiny5-Regular.ttf", 48);

    // models
    ResourceManager::loadModel("sponza", "sponza/Sponza.gltf", renderer);
}

void Engine::framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0.0f, 0.0f, width, height);
}

void Engine::mouseCallback(GLFWwindow *window, double xPos, double yPos)
{
    Engine *engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));

    engine->camera.updateRotation(xPos, yPos);
}
