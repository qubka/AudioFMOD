#include "game.hpp"
#include "font.hpp"
#include "components.hpp"
#include "texture.hpp"
#include "geometry.hpp"
#include "extentions.hpp"
#include "model.hpp"

// Constructor
Game::Game() : window{ "OpenGL Template", { 1280, 720 }} {
    Input::Setup(window);
}

// Destructor
Game::~Game() {
}

// Initialisation:  This method only runs once at startup
void Game::init() {
    // Set the clear colour and depth
    glCall(glClearColor, 1.0f, 1.0f, 1.0f, 1.0f);
    glCall(glClearStencil, 0);
    glCall(glClearDepth, 1.0f);
    glCall(glEnable, GL_CULL_FACE);
    glCall(glEnable, GL_LIGHTING);
    glCall(glEnable, GL_TEXTURE_2D);
    glCall(glEnable, GL_DEPTH_TEST);
    glCall(glEnable, GL_COLOR_MATERIAL);
    glCall(glDepthFunc, GL_LEQUAL);
    glCall(glShadeModel, GL_SMOOTH);
    glCall(glPixelStorei, GL_UNPACK_ALIGNMENT, 4);
    glCall(glPointSize, 7.0f);

    // Initialise audio and play background music
    //audio.loadEventSound("resources/audio/Horse.wav");
    audio.loadMusicStream("resources/audio/fsm-team-escp-paradox.wav");
    audio.playMusicStream();

    //audio.loadSound("resources/audio/fsm-team-escp-paradox.wav", true, true);
    //audio.playSound3D("resources/audio/fsm-team-escp-paradox.wav", glm::vec3{0, 0, 0}, 1.0f);

    //audio.playMusicStream();

    mainShader = std::make_unique<Shader>();
    mainShader->link("resources/shaders/mainShader.vert", "resources/shaders/mainShader.frag");

    // Initialise lights
    directionalLight.color = glm::vec3{ 1.0f, 1.0f, 1.0f };
    directionalLight.ambientIntensity = 0.9f;
    directionalLight.diffuseIntensity = 0.5f;
    directionalLight.direction = glm::normalize(glm::vec3{ 0.0f, -1.0f, 0.0f });

    mainShader->use();
    mainShader->setUniform("fog_on", true);
    mainShader->setUniform("fog_colour", glm::vec3{ 0.5 });
    mainShader->setUniform("fog_factor_type", 0);
    mainShader->setUniform("fog_start", 20.f);
    mainShader->setUniform("fog_end", 1000.f);

    mainShader->setUniform("lighting_on", true);
    mainShader->setUniform("transparency", 1.0f);
    mainShader->setUniform("gMatSpecularIntensity", 1.f);
    mainShader->setUniform("gSpecularPower", 10.f);

    directionalLight.submit(mainShader);


    // Generate path for pipe

    std::vector<glm::vec3> points {
        { 20, 0, 0 },
        { 12, 5, 12 },
        { 0, 10, 20 },
        { -12, 20, 12 },
        { -20, 12, 0 },
        { -12, 10, -12 },
        { 0, 5, -20 },
        { 12, 0, -12 }
    };

    catmullRom.uniformlySampleControlPoints(std::move(points), 500);

    // Create entities

    glm::vec3& initial = catmullRom.getCentrelinePoints()[0];
    glm::vec3& direction = catmullRom.getCentrelineNormals()[0];

    spaceship = registry.create();
    registry.emplace<TransformComponent>(spaceship, initial, glm::quat{1, 0, 0, 0}, glm::vec3{1.0f});
    registry.emplace<ModelComponent>(spaceship, Model::Load("resources/models/Ship/SpaceShip_final.fbx"));
    registry.emplace<ShipComponent>(spaceship);
    auto& spotLight = registry.emplace<SpotLight>(spaceship);
    spotLight.position = initial + direction * 5.0f;
    spotLight.color = glm::vec3{ 1.0f, 0.0f, 0.0f };
    spotLight.ambientIntensity = 90.0f;
    spotLight.diffuseIntensity = 90.0f;
    spotLight.direction = direction;
    spotLight.cutoff = 0.9f;
    auto& pointLight = registry.emplace<PointLight>(spaceship);
    pointLight.position = initial - direction * 10.0f;
    pointLight.color = glm::vec3{ 0.17f, 1.0f, 0.025f };
    pointLight.ambientIntensity = 0.25f;
    pointLight.diffuseIntensity = 0.6f;

    auto entity = registry.create();
    registry.emplace<TransformComponent>(entity, glm::vec3{0}, glm::quat{glm::vec3{glm::radians(-90.0), 0.0, 0.0}}, glm::vec3{1000.0f, 1000.0f, 1.0f});
    registry.emplace<MeshComponent>(entity, geometry::quad({ 1.0f, 1.0f }, std::make_shared<Texture>("resources/textures/terrain.jpg", false, false, glm::vec3{100})));

    //////////////////////////////////////////////////////////////

    // Create cubemap skybox
    std::array<std::string, 6> faces {
        "resources/skyboxes/GalaxyTex_PositiveX.png",
        "resources/skyboxes/GalaxyTex_NegativeX.png",
        "resources/skyboxes/GalaxyTex_NegativeY.png",
        "resources/skyboxes/GalaxyTex_PositiveY.png",
        "resources/skyboxes/GalaxyTex_PositiveZ.png",
        "resources/skyboxes/GalaxyTex_NegativeZ.png",
    };

    skybox = std::make_unique<Skybox>(faces);

    skyboxShader = std::make_unique<Shader>();
    skyboxShader->link("resources/shaders/skyboxShader.vert", "resources/shaders/skyboxShader.frag");

    //////////////////////////////////////////////////////////////

    // Create texture atlasses for several font sizes
    FontLibrary library;
    FontFace roboto_face{library, "resources/fonts/Roboto-Black.ttf"};

    textShader = std::make_unique<Shader>();
    textShader->link("resources/shaders/textShader.vert", "resources/shaders/textShader.frag");

    textMesh = std::make_unique<TextMesh>();
    font = std::make_unique<Font>(roboto_face, 24);
}

// Render method runs repeatedly in a loop
void Game::render() {
    // Clear the buffers and enable depth testing (z-buffering)
    glCall(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCall(glDisable, GL_BLEND);
    glCall(glEnable, GL_DEPTH_TEST);

    // Get camera view and proj matricies
    auto viewMatrix = camera.getViewMatrix();
    auto projMatrix = camera.getPerspectiveProjectionMatrix();
    auto viewProjMatrix = projMatrix * viewMatrix;

    // Use the main shader program
    mainShader->use();
    mainShader->setUniform("u_view_projection", viewProjMatrix);
    mainShader->setUniform("gEyeWorldPos", camera.getPosition());
    //mainShader->setUniform("fog_on", false);
    //directionalLight.ambientIntensity = darkMode ? 0.15f : 1.0f;
    //directionalLight.diffuseIntensity = darkMode ? 0.1f : 1.0f;
    directionalLight.submit(mainShader);

    // Render scene
    frustum.update(viewProjMatrix);

    auto group = registry.group<TransformComponent>(entt::get<ModelComponent>);
    for (auto entity : group) {
        auto [transform, model] = group.get<TransformComponent, ModelComponent>(entity);

        if (frustum.checkSphere(transform.translation, model.radius * glm::max(transform.scale.x, transform.scale.y, transform.scale.z))) {
            glm::mat4 transformMatrix{ transform };
            glm::mat3 normalMatrix{ glm::transpose(glm::inverse(glm::mat3{ transformMatrix })) };

            mainShader->setUniform("u_transform", transformMatrix);
            mainShader->setUniform("u_normal", normalMatrix);
            model()->render(mainShader);
        }
    }

    auto meshes = registry.view<const TransformComponent, const MeshComponent>();
    for (auto [entity, transform, mesh] : meshes.each()) {
        if (frustum.checkSphere(transform.translation, mesh.radius * glm::max(transform.scale.x, transform.scale.y, transform.scale.z))) {
            glm::mat4 transformMatrix{ transform };
            glm::mat3 normalMatrix{ glm::transpose(glm::inverse(glm::mat3{ transformMatrix })) };

            mainShader->setUniform("u_transform", transformMatrix);
            mainShader->setUniform("u_normal", normalMatrix);
            mesh()->render(mainShader);
        }
    }

    mainShader->setUniform("lighting_on", false);

    uint32_t i = 0;
    auto spotLights = registry.view<SpotLight>();
    mainShader->setUniform("gNumSpotLights", static_cast<int>(spotLights.size()));

    for (auto [entity, light] : spotLights.each()) {
        light.submit(mainShader, i);
        i++;
    }

    i = 0;
    auto pointLights = registry.view<PointLight>();
    mainShader->setUniform("gNumPointLights", static_cast<int>(pointLights.size()));

    for (auto [entity, light] : pointLights.each()) {
        light.submit(mainShader, i);
        i++;
    }

    mainShader->setUniform("lighting_on", true);

    //////////////////////////////////////////////////////////////

    skyboxShader->use();
    skyboxShader->setUniform("u_view_projection", projMatrix * glm::mat4{glm::mat3{viewMatrix}}); // remove translation from the view matrix
    skyboxShader->setUniform("skybox", 0);

    skybox->render();

    //////////////////////////////////////////////////////////////

    // Disable depth and enable blend for text rendering
    glCall(glDisable, GL_DEPTH_TEST);
    glCall(glEnable, GL_BLEND);
    glCall(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    textShader->use();
    textShader->setUniform("u_projection", camera.getOrthographicProjectionMatrix());
    textShader->setUniform("color", glm::vec4{1});
    textShader->setUniform("atlas", 0);

    font->bind();

    textMesh->render(font, "Press '1' to play left speaker only", 20, 20, 1);
    textMesh->render(font, "Press '2' to play right speaker only", 20, 40, 1);
    textMesh->render(font, "Press '3' to play from both speakers", 20, 60, 1);
    textMesh->render(font, "Press '[' to pan sound left", 20, 80, 1);
    textMesh->render(font, "Press ']' to pan sound right", 20, 100, 1);
    textMesh->render(font, "Press '+' to increase volume", 20, 120, 1);
    textMesh->render(font, "Press '-' to decrease volume", 20, 140, 1);
    textMesh->render(font, "Press '6' pitch octave down", 20, 160, 1);
    textMesh->render(font, "Press '7' pitch semitone down", 20, 180, 1);
    textMesh->render(font, "Press '8' pitch semitone up", 20, 200, 1);
    textMesh->render(font, "Press '9' pitch octave up", 20, 220, 1);
    textMesh->render(font, "Press 'n' pitch scale down", 20, 240, 1);
    textMesh->render(font, "Press 'm' pitch scale up", 20, 260, 1);
    textMesh->render(font, "Press 'q' to pause audio", 20, 280, 1);

    textMesh->render(font, "Press 'r' to switch Lowpass filter", 20, 300, 1);
    textMesh->render(font, "Press 't' to switch Highpass filter", 20, 320, 1);
    textMesh->render(font, "Press 'y' to switch Echo filter", 20, 340, 1);
    textMesh->render(font, "Press 'u' to switch Flange filter", 20, 360, 1);
    textMesh->render(font, "Press 'i' to switch Distortion filter", 20, 380, 1);
    textMesh->render(font, "Press 'o' to switch Chorus filter", 20, 400, 1);
    textMesh->render(font, "Press 'p' to switch Parameq filter", 20, 420, 1);
    textMesh->render(font, "Press 'c' to switch Custom filter", 20, 440, 1);

    textMesh->render(font, "Press 'F1' to enable wiremode renderer", 20, 460, 1);
    textMesh->render(font, "Press 'TAB' to lock mouse and use camera", 20, 480, 1);
    textMesh->render(font, "Press 'ESC' to exit", 20, 500, 1);

    textMesh->render(font, "Time: " + std::to_string(glfwGetTime()), window.getWidth() / 2 + 150.0f, 20, 1);

	// Draw the 2D graphics after the 3D graphics
	displayFrameRate();
}

// Update method runs repeatedly with the Render method
void Game::update() {
    // Set the orthographic and perspective projection matrices based on the image size
    camera.setOrthographicProjectionMatrix(window.getWidth(), window.getHeight());
    camera.setPerspectiveProjectionMatrix(45.0f, window.getAspect(), 0.01f, 5000.0f);

    if (Input::GetKeyDown(GLFW_KEY_ESCAPE))
        window.shouldClose(true);

    if (Input::GetKeyDown(GLFW_KEY_TAB))
        window.toggleCursor();

    if (Input::GetKeyDown(GLFW_KEY_F1))
        window.toggleWireframe();

    moveShip();

    camera.update(dt);

    audio.update(camera.getPosition(), camera.getPosition(), camera.getForwardVector(), camera.getUpVector());
}

void Game::displayFrameRate() {
    // Increase the elapsed time and frame counter
    frameNumber++;
    elapsedTime += dt;
    frameCount++;

    // Now we want to subtract the current time by the last time that was stored
    // to see if the time elapsed has been over a second, which means we found our FPS.
    if (elapsedTime > 1.0f) {
        elapsedTime = 0;
        framesPerSecond = frameCount;

        // Reset the frames per second
        frameCount = 0;
    }

    if (framesPerSecond > 0) {
        textMesh->render(font, "FPS: " + std::to_string(framesPerSecond), 20, window.getHeight() - 30, 1.0f);
    }
}

void Game::moveShip() {
    auto& transform = registry.get<TransformComponent>(spaceship);
    auto& ship = registry.get<ShipComponent>(spaceship);
    auto& spotLight= registry.get<SpotLight>(spaceship);
    auto& pointLight = registry.get<PointLight>(spaceship);

    auto& points = catmullRom.getControlPoints();
    auto& normals = catmullRom.getCentrelineNormals();

    auto& current = transform.translation;
    auto& target = points[ship.path];
    auto& direction = normals[ship.path];

    if (glm::distance(current, target) < 0.1f) {
        ship.path += 1;
        if (ship.path >= points.size()) {
            ship.path = 0;
        }
    }
    glm::vec3 lastPos{ transform.translation };

    transform.translation = glm::smoothDamp(transform.translation, target, ship.velocity, 0.01f, ship.maxSpeed, dt);
    transform.rotation = glm::quatLookAt(direction, vec3::up);

    spotLight.position = transform.translation + direction * 5.0f;
    spotLight.direction = direction;
    pointLight.position = transform.translation - direction * 10.0f;
}

// The game loop runs repeatedly until game over
void Game::run() {
    float currentTime = static_cast<float>(glfwGetTime());
    float previousTime = currentTime;

    while (!window.shouldClose()) {
        currentTime = static_cast<float>(glfwGetTime());
        dt = currentTime - previousTime;
        previousTime = currentTime;

        update();
        render();

        Input::Update();

        window.swapBuffers();
        window.pollEvents();
    }
}

Game& Game::getInstance() {
    static Game instance;
    return instance;
}

int main(int args, char** argv) {
    Game& game = Game::getInstance();
    try {
        game.init();
        game.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}