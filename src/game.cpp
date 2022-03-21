#include "game.hpp"
#include "font.hpp"
#include "components.hpp"
#include "geometry.hpp"
#include "texture.hpp"
#include "poissonsampling.hpp"
#include "random.hpp"
#include "extentions.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

// Constructor
Game::Game() : window{ "OpenGL Template", { 1280, 720 }}, camera{}, audioManager{} {
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

    mainShader = std::make_unique<Shader>();
    mainShader->link("resources/shaders/mainShader.vert", "resources/shaders/mainShader.frag");

    // Initialise lights
    directionalLight.color = glm::vec3{ 1.0f, 1.0f, 1.0f };
    directionalLight.ambientIntensity = darkMode ? 0.15f : 1.0f;
    directionalLight.diffuseIntensity = darkMode ? 0.1f : 1.0f;
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
        { 400,  -50,  0 },
        { 250,   50,  250 },
        { 0,    100,  400 },
        { -250,  50, 250 },
        { -400, 5,  0 },
        { -250,  -20,  -250 },
        { 0,    -100, -400 },
        { 250,   -150, -250 }
    };

    catmullRom.uniformlySampleControlPoints(std::move(points), 500);

    auto tube = registry.create();
    registry.emplace<TransformComponent>(tube);
    registry.emplace<MeshComponent>(tube, geometry::tube(catmullRom.getControlPoints(), 30.0f, 48, std::make_unique<Texture>(150, 0, 150)), FLT_MAX / 2.0f);

    auto t = geometry::torus(24, 72, 35.0f, 7.5f, std::make_shared<Texture>("resources/textures/magic.png", true, false));
    auto& p = catmullRom.getCentrelinePoints();
    auto& n = catmullRom.getCentrelineNormals();
    for (int i = 0; i < p.size(); i += 30) {
        auto entity = registry.create();
        registry.emplace<TransformComponent>(entity, p[i], glm::quatLookAt(n[i], vec3::up), glm::vec3{1.0f});
        registry.emplace<MeshComponent>(entity, t, 35.0f);
    }

    auto tetrahedron = registry.create();
    registry.emplace<TransformComponent>(tetrahedron);
    registry.emplace<MeshComponent>(tetrahedron, geometry::tetrahedron(glm::vec3{1.0f, 3.0f, 1.0f}, std::make_unique<Texture>(255, 0, 255)));

    // Load meshes

    std::vector<std::shared_ptr<Model>> asteroidsModels {
        Model::Load("resources/models/Asteroids/Asteroid_1.fbx"),
        Model::Load("resources/models/Asteroids/Asteroid_2.fbx"),
        Model::Load("resources/models/Asteroids/Asteroid_3.fbx"),
        Model::Load("resources/models/Asteroids/Asteroid_4.fbx"),
        Model::Load("resources/models/Asteroids/Asteroid_5.fbx"),
        Model::Load("resources/models/Asteroids/Asteroid_6.fbx"),
        Model::Load("resources/models/Asteroids/Asteroid_7.fbx"),
        Model::Load("resources/models/Asteroids/Asteroid_8.fbx"),
        Model::Load("resources/models/Asteroids/Asteroid_9.fbx"),
        Model::Load("resources/models/Asteroids/Asteroid_10.fbx")
    };

    // Create entities

    glm::vec3& initial = catmullRom.getCentrelinePoints()[0];
    glm::vec3& direction = catmullRom.getCentrelineNormals()[0];

    spaceship = registry.create();
    registry.emplace<TransformComponent>(spaceship, initial);
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

    for (const auto& v : poisson::diskSampler2D(50, {1000, 1000}, 50)) {
        auto entity = registry.create();
        registry.emplace<TransformComponent>(entity, glm::vec3{v.x - 500.0f, Random::FloatRange(-300.0f, 300.0f), v.y - 500.0f}, glm::quat{{ Random::FloatValue(), Random::FloatValue(), Random::FloatValue() }}, glm::vec3{2.5f});
        registry.emplace<ModelComponent>(entity, asteroidsModels[Random::IntRange(0, static_cast<int>(asteroidsModels.size()) - 1)], 5.0f);
    }

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

    splineShader = std::make_unique<Shader>();
    splineShader->link("resources/shaders/splineShader.vert", "resources/shaders/splineShader.frag");

    //////////////////////////////////////////////////////////////

    // Create texture atlasses for several font sizes
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "ERROR: Failed to init FreeType" << std::endl;
        return;
    }

    FontLibrary library;
    FontFace roboto_face{library, "resources/fonts/Roboto-Black.ttf"};
    FontFace icon_face{ library, "resources/fonts/Font90Icons-2ePo.ttf"};

    textShader = std::make_unique<Shader>();
    textShader->link("resources/shaders/textShader.vert", "resources/shaders/textShader.frag");

    textMesh = std::make_unique<TextMesh>();
    font = std::make_unique<Font>(roboto_face, 32);
    icons = std::make_unique<Font>(icon_face, 32);
}

// Render method runs repeatedly in a loop
void Game::render() {
    // Clear the buffers and enable depth testing (z-buffering)
    glCall(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
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
    mainShader->setUniform("fog_on", darkMode);
    directionalLight.ambientIntensity = darkMode ? 0.15f : 1.0f;
    directionalLight.diffuseIntensity = darkMode ? 0.1f : 1.0f;
    directionalLight.submit(mainShader);

    // Render scene
    frustum.update(viewProjMatrix);

    auto group = registry.group<TransformComponent>(entt::get<ModelComponent>);
    for (auto entity : group) {
        auto [transform, model] = group.get<TransformComponent, ModelComponent>(entity);

        if (frustum.checkSphere(transform.translation, model.radius)) {
            glm::mat4 transformMatrix{ transform };
            glm::mat3 normalMatrix{ glm::transpose(glm::inverse(glm::mat3{ transformMatrix })) };

            mainShader->setUniform("u_transform", transformMatrix);
            mainShader->setUniform("u_normal", normalMatrix);
            model()->render(mainShader);
        }
    }

    auto meshes = registry.view<const TransformComponent, const MeshComponent>();
    for (auto [entity, transform, mesh] : meshes.each()) {
        if (frustum.checkSphere(transform.translation, mesh.radius)) {
            glm::mat4 transformMatrix{ transform };
            glm::mat3 normalMatrix{ glm::transpose(glm::inverse(glm::mat3{ transformMatrix })) };

            mainShader->setUniform("u_transform", transformMatrix);
            mainShader->setUniform("u_normal", normalMatrix);
            mesh()->render(mainShader);
        }
    }

    mainShader->setUniform("lighting_on", false);

    uint32_t i = 0;
    auto spotLights = registry.view<const SpotLight>();
    mainShader->setUniform("gNumSpotLights", static_cast<int>(spotLights.size()));

    for (auto [entity, light] : spotLights.each()) {
        light.submit(mainShader, i);
        i++;
    }

    i = 0;
    auto pointLights = registry.view<const PointLight>();
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

    /*splineShader->use();
    splineShader->setUniform("u_view_projection", viewProjMatrix);

    tubeMesh->render();*/

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

    textMesh->render(font, "Press TAB to lock mouse and use camera", 20, 20, 1);
    textMesh->render(font, "Press ESC to exit", 20, 50, 1);
    textMesh->render(font, "Press F1 to enable wiremode renderer", 20, 80, 1);
    textMesh->render(font, "Press F2 to toggle lighting", 20, 110, 1);
    textMesh->render(font, "Press F3 to switch view mode", 20, 140, 1);
    textMesh->render(font, glm::to_string(camera.getPosition()), window.getWidth() / 2, window.getHeight() - 30, 1);
    textMesh->render(font, "Time: " + std::to_string(glfwGetTime()), window.getWidth() / 2 + 150.0f, 20, 1);

	// Draw the 2D graphics after the 3D graphics
	displayFrameRate();

    // Draw icons

    icons->bind();

    textShader->setUniform("color", glm::vec4{1, 0, 0, 1});

    textMesh->render(icons, "abcdefghijkl", 20, window.getHeight() / 2, 1);

    textShader->setUniform("color", glm::vec4{0, 1, 0, 1});

    textMesh->render(icons, "mnopqrstuvwxyz", 20, window.getHeight() / 2 - 30, 1);

    textShader->setUniform("color", glm::vec4{0, 0, 1, 1});

    textMesh->render(icons, "ABCDEFGHIJKLMN\nOPQRSTUVWXYZ", 20, window.getHeight() / 2 - 60, 1);
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

    if (Input::GetKeyDown(GLFW_KEY_F2))
        darkMode = !darkMode;

    if (Input::GetKeyDown(GLFW_KEY_F3))
        viewMode = viewMode + 1 % 4;

    moveShip();

    if (window.Locked()) {
        camera.update(dt);
    }
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

    transform.translation = glm::smoothDamp(transform.translation, target, ship.velocity, 0.01f, ship.maxSpeed, dt);
    transform.rotation = glm::quatLookAt(direction, vec3::up);

    spotLight.position = transform.translation + direction * 5.0f;
    spotLight.direction = direction;
    pointLight.position = transform.translation - direction * 10.0f;

    if (!window.Locked()) {
        switch (viewMode) {
            case 0: {
                camera.setPosition(transform.translation - direction * 30.0f);
                camera.setRotation(transform.rotation);
                break;
            }
            case 1: {
                auto sideDirection = glm::rotate(direction, glm::radians(90.0f), vec3::right);
                camera.setPosition(transform.translation - sideDirection * 30.0f);
                camera.setRotation(glm::quatLookAt(sideDirection, vec3::up));
                break;
            }
            case 2: {
                auto topDirection = glm::rotate(direction, glm::radians(90.0f), vec3::up);
                camera.setPosition(transform.translation - topDirection * 30.0f);
                camera.setRotation(glm::quatLookAt(topDirection, vec3::up));
                break;
            }
            case 3: {
                camera.setPosition(transform.translation + direction * 5.0f);
                camera.setRotation(transform.rotation);
                break;
            }
        }
    }
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