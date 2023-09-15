#include "sre/SDLRenderer.hpp"
#include "sre/SpriteAtlas.hpp"

glm::vec2 window_size = glm::vec2(800, 600);
sre::SDLRenderer renderer;
sre::Camera camera;
std::shared_ptr<sre::SpriteAtlas> atlas;
sre::Sprite sprite;

void Render();
void Update(float deltaTime);
void ProcessEvents(SDL_Event& event);

int main() {
	renderer.frameRender = Render;
	renderer.frameUpdate = Update;
	renderer.keyEvent = ProcessEvents;

	renderer.setWindowSize(window_size);
	renderer.init();
	camera.setWindowCoordinates();

	atlas = sre::SpriteAtlas::create("data/snake.json",
		"data/snake.png");
	sprite = atlas->get("berry.png");
	sprite.setPosition(window_size / 2.0f);

	renderer.startEventLoop();
}
void ProcessEvents(SDL_Event& event) { }

void Update(float deltaTime) { }

void Render() {
	sre::RenderPass renderPass = sre::RenderPass::create()
		.withCamera(camera)
		.withClearColor(true, { .3f, .3f, 1, 1 })
		.build();

	sre::SpriteBatch::SpriteBatchBuilder spriteBatchBuilder = sre::SpriteBatch::create();

	// send spriteBatchBuilder to your game elements, so that they can add their sprites for rendering
	spriteBatchBuilder.addSprite(sprite);

	auto spriteBatch = spriteBatchBuilder.build();
	renderPass.draw(spriteBatch);
}