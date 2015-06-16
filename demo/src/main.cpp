#include <chrono>
#include <iostream>
#include <functional>
#include <thread>

#include <common/Common.h>
#include <platform/Platform.h>
#include <graphics/Graphics.h>
#include <scene/Scene.h>

#include <thread>
#include <chrono>

using namespace wntr;

int main() {
	srand((unsigned)clock());

	auto application = Application::Create();

	// while (true) {

		// create the window
		unsigned window_width = 1280;
		unsigned window_height = 960;
		auto window = Window::Create("Abe", (1980 - window_width) / 2, 10, window_width, window_height, false);

		// create context and asset loader
		auto context = RenderContext::Create(window, 1280, 960, false, false, API::OPENGL);
		auto assets = AssetLoader::Create("assets/", context);

		// create ui renderer
		auto ui_renderer = UIRenderer::Create(window->getWidth(), window->getHeight(), context);

		// create deferred renderer
		auto camera = Entity::Create(); {
			camera->attach(View::Create());
			camera->attach(Perspective::Create(window->getAspectRatio()));
		}
		auto def_renderer = DeferredRenderer::Create(camera, context);

		// create handlers
		auto keyboard_handler = KeyboardHandler::Create(window);
		auto mouse_handler = MouseHandler::Create(window);
		mouse_handler->onEvent = [mouse_handler, camera](const MouseEvent& evt) {
			static float prev_x = (float)evt.getX();
			static float prev_y = (float)evt.getY();
			// reset the previous storage of x,y positions
			if (evt.getEventCode() == MOUSE_EVENT_CODE::LEFT_DOWN) {
				prev_x = (float)evt.getX();
				prev_y = (float)evt.getY();
			}
			// we only do this if the mouse is down
			if (mouse_handler->isMouseLeftDown()) {
				if (evt.getEventCode() == MOUSE_EVENT_CODE::MOVE) {
					float diff_x = (evt.getX() - prev_x)*(3.1415f / 180.0f);
					float diff_y = (evt.getY() - prev_y)*(3.1415f / 180.0f);
					// rotate the camera
					camera->get<View>()->rotateInParentSpace(glm::angleAxis(-diff_x, glm::vec3(0.0f, 1.0f, 0.0f)));
					camera->get<View>()->rotate(glm::angleAxis(-diff_y, glm::vec3(1.0f, 0.0f, 0.0f)));
					// set the previous x,y
					prev_x = (float)evt.getX();
					prev_y = (float)evt.getY();
				}
			}
		};

		// create materials
		auto material_rock = assets->loadMaterial("images/rock.png");

		// create meshes
		auto mesh_cube = Mesh::CreateCube(1.0f, context.get());
		mesh_cube->setMaterial(material_rock);

		// create floor
		auto floor = Entity::Create(); {
			floor->attach(mesh_cube);
			floor->attach(Transform3D::Create());
			floor->get<Transform3D>()->setScale(glm::vec3(100.0f, 1.0f, 100.0f));
			floor->get<Transform3D>()->setPosition(glm::vec3(0.0f, -0.5f, 0.0f));
		}
		def_renderer->addEntity(floor);

		// create roof
		auto roof = Entity::Create(); {
			roof->attach(mesh_cube);
			roof->attach(Transform3D::Create());
			roof->get<Transform3D>()->setScale(glm::vec3(100.0f, 1.0f, 100.0f));
			roof->get<Transform3D>()->setPosition(glm::vec3(0.0f, 100.5f, 0.0f));
		}
		def_renderer->addEntity(roof);

		// create walls
		auto wall1 = Entity::Create(); {
			wall1->attach(mesh_cube);
			wall1->attach(Transform3D::Create());
			wall1->get<Transform3D>()->setScale(glm::vec3(100.0f, 100.0f, 1.0f));
			wall1->get<Transform3D>()->setPosition(glm::vec3(0.0f, 50.0f, 50.5f));
		}
		auto wall2 = Entity::Create(); {
			wall2->attach(mesh_cube);
			wall2->attach(Transform3D::Create());
			wall2->get<Transform3D>()->setScale(glm::vec3(100.0f, 100.0f, 1.0f));
			wall2->get<Transform3D>()->setPosition(glm::vec3(0.0f, 50.0f, -50.5f));
		}
		auto wall3 = Entity::Create(); {
			wall3->attach(mesh_cube);
			wall3->attach(Transform3D::Create());
			wall3->get<Transform3D>()->setScale(glm::vec3(1.0f, 100.0f, 100.0f));
			wall3->get<Transform3D>()->setPosition(glm::vec3(50.5f, 50.0f, 0.0f));
		}
		auto wall4 = Entity::Create(); {
			wall4->attach(mesh_cube);
			wall4->attach(Transform3D::Create());
			wall4->get<Transform3D>()->setScale(glm::vec3(1.0f, 100.0f, 100.0f));
			wall4->get<Transform3D>()->setPosition(glm::vec3(-50.5f, 50.0f, 0.0f));
		}
		def_renderer->addEntity(wall1);
		def_renderer->addEntity(wall2);
		def_renderer->addEntity(wall3);
		def_renderer->addEntity(wall4);

		// create an animation manager
		auto cube_animator = Animator::Create();

		// create cubes
		for (unsigned i = 0; i < 128; i++) {
			auto cube = Entity::Create(); {
				float scale = ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f;
				float x = ((float)rand() / (float)RAND_MAX) * 90.0f - 45.0f;
				float y = ((float)rand() / (float)RAND_MAX) * 90.0f + 10.0f;
				float z = ((float)rand() / (float)RAND_MAX) * 90.0f - 45.0f;
				cube->attach(mesh_cube);
				cube->attach(Transform3D::Create(glm::vec3(x, y, z)));
				cube->attach(TransformAnimation3D::Create([scale] (const std::shared_ptr<Transform3D>& transform, double elapse) -> void {
					auto diff = transform->getPosition() - glm::vec3(0.0f, 50.0f, 0.0f);
					auto quat = glm::angleAxis((float)elapse * scale, glm::vec3(0.0f, 1.0f, 0.0f));
					diff = glm::conjugate(quat) * diff * quat;
					transform->setPosition(glm::vec3(0.0f, 50.0f, 0.0f) + diff);
				}, cube->get<Transform3D>(), 1.0, true));
			}
			def_renderer->addEntity(cube);
			cube_animator->addEntity(cube);
		}

		// create lights
		auto shadow_map = ShadowMap::Create(1024, 1024, context.get());
		for (unsigned i = 0; i < 4; i++) {
			auto light = Entity::Create(); {
				float x = ((float)rand() / (float)RAND_MAX) * 50.0f - 25.0f;
				float y = ((float)rand() / (float)RAND_MAX) * 50.0f + 25.0f;
				float z = ((float)rand() / (float)RAND_MAX) * 50.0f - 25.0f;
				float r = ((float)rand() / (float)RAND_MAX) * 512.0f;
				float g = ((float)rand() / (float)RAND_MAX) * 512.0f;
				float b = ((float)rand() / (float)RAND_MAX) * 512.0f;
				light->attach(Transform3D::Create(glm::vec3(x, y, z)));
				light->attach(Color::Create(r, g, b));
				light->attach(shadow_map);
			}
			def_renderer->addLight(light);
		}

		// show the window
		window->show();
		while (window->isOpen()) {
			// calculate the elapse time for the frame
			static double elapse = 0.0;
			static auto start = std::chrono::high_resolution_clock::now();
			static auto end = std::chrono::high_resolution_clock::now();
			end = std::chrono::high_resolution_clock::now();
			elapse = ((double)std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) / 1000.0;
			start = std::chrono::high_resolution_clock::now();

			// animate
			cube_animator->animate(elapse);

			// move the camera based on which keys are currently pressed down
			glm::vec3 move(0.0f, 0.0f, 0.0f);
			if (keyboard_handler->isKeyDown(KEY_CODE::KEY_W))
				move += camera->get<View>()->getForward();
			if (keyboard_handler->isKeyDown(KEY_CODE::KEY_A))
				move += camera->get<View>()->getLeft();
			if (keyboard_handler->isKeyDown(KEY_CODE::KEY_S))
				move += camera->get<View>()->getBack();
			if (keyboard_handler->isKeyDown(KEY_CODE::KEY_D))
				move += camera->get<View>()->getRight();
			if (keyboard_handler->isKeyDown(KEY_CODE::SHIFT))
				camera->get<View>()->translate(move*10.0f*(float)elapse);
			else
				camera->get<View>()->translate(move*1.0f*(float)elapse);

			context->clearColor(0.0f, 0.0f, 0.0f, 1.0f);
			def_renderer->draw();
			ui_renderer->draw();
			context->swapBuffers();

			application->update();
		}

	// }

	application->kill();
}