#include "pch.h"

// #include <display/Window.h>
#include <Application.h>
#include <Scene.h>

// Logging
#include <windows.h>
#include <iostream>

Window* Window::create(WindowParams params) {
	std::shared_ptr<Window> window = std::make_shared<Window>(params);
	Application::windows[window->id] = window;

	window->loadScene(params.init_scene);
	window->switchScene(params.init_scene);

	return window.get();
}

Window::Window(WindowParams params) {
	_name = params.name;
	scene = nullptr;
	shouldClose = false;

	if (!params.x) _x = SDL_WINDOWPOS_CENTERED;
	else _x = params.x.value();

	if (!params.y) _y = SDL_WINDOWPOS_CENTERED;
	else _y = params.y.value();

	if (!params.w) _w = 720;
	else _w = params.w.value();

	if (!params.h) _h = 480;
	else _h = params.h.value();

	fpsSize frames = 60;
	if (params.fps) frames = params.fps.value();
	fps = Fps(frames);

	handle = SDL_CreateWindow(_name.c_str(), _x, _y, _w, _h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!handle) {
		printf("Failed to create window\n");
	}
	context = SDL_GL_CreateContext(handle);
	id = SDL_GetWindowID(handle);
	SDL_GetWindowPosition(handle, &_x, &_y);

	thread = std::thread(Application::start_winloop, this);
	
}

Window::~Window() {
	SDL_DestroyWindow(handle);
	SDL_GL_DeleteContext(context); //todo: THE RENDERER SHOULD DO THIS, NOT THE WINDOW!
	OutputDebugStringA("Deleted window\n");
	// delete handle;
	// delete name;
}

bool Window::switchScene(Scene* scene)
{
	if (this->scene) this->scene->finish();
	if (scene->loadedWin != this) return false;

	scene->enter();
	this->scene = scene;
	return true;
}

bool Window::loadScene(Scene* scene) {
	if (scene->loadedWin) return false;

	scene->loadedWin = this;
	scene->load(this);
	std::shared_ptr<Scene> ss = std::shared_ptr<Scene>(scene);
	loadedScenes[scene->id] = ss;
	return true;
}

bool Window::unloadScene(Scene* scene) {
	if (scene->loadedWin != this) return false;

	scene->unload(this);
	scene->loadedWin = nullptr;
	loadedScenes.erase(scene->id);
	return true;
}

void Window::centerPosition(bool x, bool y) {
	int tx = _x; int ty = _y;
	if (x) tx = SDL_WINDOWPOS_CENTERED;
	if (y) ty = SDL_WINDOWPOS_CENTERED;

	SDL_SetWindowPosition(handle, tx, ty);
	SDL_GetWindowPosition(handle, &_x, &_y);
}

Window::operator std::string() {
	std::string winString = "Window " +  std::to_string(id)  + "[\"" + _name + "\"] : " + std::to_string(w()) + "x " + std::to_string(h()) + " at position(" + std::to_string(x()) + " | " + std::to_string(y()) + ") running on " + std::to_string(fps.getMax()) + " max fps";
	return winString;
}