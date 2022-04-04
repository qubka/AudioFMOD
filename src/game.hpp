#pragma once

// Setup includes
#include "window.hpp"
#include "input.hpp"

// Game includes
#include "camera.hpp"
#include "shader.hpp"
#include "audio.hpp"
#include "mesh.hpp"
#include "lights.hpp"
#include "textmesh.hpp"
#include "skybox.hpp"
#include "frustum.hpp"
#include "catmullrom.hpp"

#include <entt/entity/registry.hpp>

int main(int argc, char** argv);

// Classes used in game.  For a new class, declare it here and provide a pointer to an object of this class below.  Then, in Game.cpp, 
// include the header.  In the Game constructor, set the pointer to nullptr and in Game::Initialise, create a new object.  Don't forget to
// delete the object in the destructor.   

class Game {
private:
	Game();
	~Game();

	void init();
    void run();
    void update();
    void render();

    Window window;
    uint64_t frameNumber{ 0 };
    uint32_t frameCount{ 0 };
    uint32_t framesPerSecond{ 0 };
    float elapsedTime{ 0.0 };
    float dt{ 0.0 };

    entt::registry registry;
    entt::entity cube;

    Audio audio;
    Camera camera;
    Frustum frustum;

	DirectionalLight directionalLight;
    std::unique_ptr<Skybox> skybox;
    std::unique_ptr<TextMesh> textMesh;
	std::unique_ptr<Font> font;

    std::unique_ptr<Shader> mainShader;
    std::unique_ptr<Shader> skyboxShader;
    std::unique_ptr<Shader> textShader;

	void displayFrameRate();

    friend int ::main(int argc, char** argv);

public:
	static Game& getInstance();
};
