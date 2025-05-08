#include <engine.h>
#include "render/utils.h"

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
    if (window == NULL) {
        Logger::print(LOG_ERROR, "Failed to create GLFW window");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    this->width = width;
    this->height = height;

    // glew initialization
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        Logger::print(LOG_ERROR, "Failed to initialize GLEW");
        exit(EXIT_FAILURE);
    }

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetWindowUserPointer(window, static_cast<void *>(this));

    Input::getSingleton()->setCallbacks(window);

    renderer.init(window);
    loadResources();
    renderer.setupBuffers();

    camera.setPosition(vec3(0.0, 3.0, 0.0));
    light.pos = vec3(-4.0, 3.0, 0.0);

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

    // TODO: create array of lights
    // TODO: make textures bindless
    // meshes with textures and lighting
    beginDebugGroup("Meshes", 0);
    if (auto shader = ResourceManager::getShader("mesh"); shader != nullptr) {
        shader->use();
        shader->setMatrix("view", view);
        shader->setMatrix("projection", projection);
        shader->setVector("cameraPos", camera.getPosition());
        shader->setVector("light.pos", light.pos);
        shader->setVector("light.color", light.color);

        renderer.renderModel(shader, ResourceManager::getModel("sponza"));
    }
    endDebugGroup();

    if (auto shader = ResourceManager::getShader("billboard"); shader != nullptr) {
        // light
        renderer.renderBillboard(shader, view, projection, light.pos, nullptr, light.color, light.size);

        // cacodemon
        auto cacodemonTexture = ResourceManager::getTexture("cacodemon");
        renderer.renderBillboard(shader, view, projection, cacodemon.pos, cacodemonTexture, cacodemon.color, cacodemon.size);
    }

    mat4 orthoProjection = glm::ortho(0.0f, float(width), 0.0f, float(height));
    ResourceManager::getShader("text")->use();
    ResourceManager::getShader("text")->setMatrix("projection", orthoProjection);
    renderer.renderText(ResourceManager::getShader("text"), ResourceManager::getFont("font"), "Text!", vec2(100, 100), 1.0, vec3(1.0f));

    renderImGui();
}

void Engine::renderImGui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow();

    {
        ImGui::Begin("Stats");

        ImGui::Checkbox("Multisampling", &multisampling);
        ImGui::Checkbox("Wireframe", &wireframe);

        ImGui::Text("%.1f FPS (%.1f ms)", fpsCount, msCount);
        ImGui::End();
    }

    {
        ImGui::Begin("Light");
        ImGui::DragFloat3("Position", &light.pos[0], 1.0, -100.0f, 100.0f);
        ImGui::ColorEdit3("Color", &light.color[0]);
        ImGui::DragFloat2("Size", &light.size[0], 0.05, 0.0, 2.0);
        ImGui::End();
    }

    {
        ImGui::Begin("Cacodemon");
        ImGui::DragFloat3("Position", &cacodemon.pos[0], 1.0, -100.0f, 100.0f);
        ImGui::ColorEdit3("Color", &cacodemon.color[0]);
        ImGui::DragFloat2("Size", &cacodemon.size[0], 0.05, 0.0, 2.0);
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Engine::update()
{
    // get deltaTime
    float currentFrame = glfwGetTime();
    static double lastFrame;
    double deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // set state based on settings
    if (multisampling) {
        glEnable(GL_MULTISAMPLE);  
    } else {
        glDisable(GL_MULTISAMPLE);  
    }
    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // set fps
    float currentTime = glfwGetTime();
    static float lastTime = glfwGetTime();
    float elapsedTime = currentTime - lastTime;
    if (elapsedTime > 0.25) {
        lastTime = currentTime;
        fpsCount = numFrames / elapsedTime;
        msCount = elapsedTime / numFrames * 1000;
        numFrames = 0;
    }
    numFrames++;

    updateInput(deltaTime);
}

void Engine::updateInput(double dt)
{
    if (ImGui::GetIO().WantCaptureKeyboard) return;

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
    ResourceManager::loadFont("font", "Roboto-Regular.ttf", 48);

    // models
    ResourceManager::loadModel("sponza", "sponza/Sponza.gltf", renderer);

    // textures
    ResourceManager::loadTexture("cacodemon", "cacodemon.png");

    light = {vec3(0.0, 4.0, 0.0), vec3(1.0)};
}

void Engine::loadShaders(bool reload)
{
    ResourceManager::loadShader("mesh", "mesh.vert", "mesh.frag", reload);
    ResourceManager::loadShader("text", "text.vert", "text.frag", reload);
    ResourceManager::loadShader("solid", "solid.vert", "solid.frag", reload);
    ResourceManager::loadShader("billboard", "billboard.vert", "billboard.frag", reload);
}

void Engine::framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0.0f, 0.0f, width, height);
}

void Engine::mouseCallback(GLFWwindow *window, double xPos, double yPos)
{
    if (!ImGui::GetIO().WantCaptureMouse) {
        Engine *engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
        engine->camera.updateRotation(xPos, yPos);
    }
}
