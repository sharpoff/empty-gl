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

    Input::getSingleton()->setCallbacks(window);

    // load renderer and models textures and so forth
    renderer.init(window);

    loadResources();

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

    // meshes with textures and lighting
    if (auto shader = ResourceManager::getShader("mesh"); shader != nullptr) {
        shader->use();
        shader->setMatrix("view", view);
        shader->setMatrix("projection", projection);
        shader->setVector("light.pos", light.pos);
        shader->setVector("light.color", light.color);

        renderer.renderModel(shader, ResourceManager::getModel("sponza"));

        auto lightModel = ResourceManager::getModel("cube");
        lightModel->transform.setTranslation(light.pos);
        lightModel->transform.setScale(vec3(0.2));
        lightModel->modelMatrix = lightModel->transform.getModelMatrix();

        renderer.renderModel(shader, lightModel);
    }

    // TODO: not working
    // renderer.renderText(ResourceManager::getShader("text"), ResourceManager::getFont("font"), "Test", vec2(10, 10), 1.0, vec3(1, 0, 0));

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
    if (input->isKeyPressed(GLFW_KEY_R)) {
        loadShaders(true);
    }

    camera.update(dt);
    input->update();
}

void Engine::loadResources()
{
    // shaders
    loadShaders(false);

    // fonts
    ResourceManager::loadFont("font", "Tiny5-Regular.ttf", 48);

    // models
    ResourceManager::loadModel("sponza", "sponza/Sponza.gltf", renderer);
    ResourceManager::loadModel("cube", "primitives/cube.glb", renderer);

    light = {vec3(0.0, 4.0, 0.0), vec3(1.0)};
}

void Engine::loadShaders(bool reload)
{
    ResourceManager::loadShader("mesh", "mesh.vert", "mesh.frag", reload);
    ResourceManager::loadShader("color", "color.vert", "color.frag", reload);
    ResourceManager::loadShader("text", "text.vert", "text.frag", reload);
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
