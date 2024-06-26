#include <iostream>
//#include <Myztic.h>
#include <Windows.h>
#include <stdio.h>
#include <io.h>
#include <Application.h>
#include <graphics/Texture2D.hpp>
#include <SDL.h>
#include <Scene.h>

#include <thread>
#include <semaphore>
/*
std::binary_semaphore signal1{ 0 };
std::binary_semaphore signal2{ 0 };
std::binary_semaphore signal3{ 0 };
void t2() {
	signal2.acquire();
	std::cout << "Started thread 2 (2 seconds)\n";
	std::this_thread::sleep_for(std::chrono::seconds(2));
	std::cout << "Finished thread 2\n";
}

void t1() {
	signal1.acquire();
	std::cout << "Started thread 1 (1 second)\n";
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "Finished thread 1\n";
}

void tMainTest() {
	std::thread tr1(t1);
	std::thread tr2(t2);

	std::this_thread::sleep_for(std::chrono::seconds(1));
	signal1.release();
	signal2.release();
	std::cout << "Released 2 threads\n";

	tr1.join();
	tr2.join();
	std::cout << "Finished Main Thread\n";
}
*/
class SceneB : Scene {
	virtual void load(Window* callerWindow) {
		std::cout << "Loaded to Window: " << (std::string)*callerWindow << "\n";
	}

	virtual void unload(Window* callerWindow) {
		std::cout << "Unloaded from Window: " << (std::string)*callerWindow << "\n";
	}

	virtual void enter() {
		std::cout << "SceneB entered\n";
		// Application::log_windows_cmd();
	}
	virtual void finish() {
		std::cout << "SceneB finished\n";
	}
};

class TestScene : Scene {
	void logLoaded() {
		int size = 0;
		Scene** loadedScenes = this->loadedWin->getLoadedScenes(&size);
		std::cout << "Window::getLoadedScenes =>\n";

		int i = 0;
		while (i < size) {
			std::cout << "Scene Number "<< i <<":" << loadedScenes[i++]->id << "\n";
		}
		std::cout << "Finished\n";
	}

	virtual void load(Window* callerWindow) {
		std::cout << "Loaded to Window: " << (std::string)*callerWindow << "\n";
	}
	virtual void enter() {
		Window* myzWin = Application::windows[1].get();
		std::cout << myzWin->name() << "\n";
		myzWin->setName("Myztic Main Window");

		logLoaded();
		Application::log_windows_cmd();

		// Segunda Windowa (excellente espanol)
		WindowParams paramsB = { "Myztic Engine Test 2", new Scene(),SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 680, 480 };
		Window* windowB = Window::create(paramsB);
		Application::log_windows_cmd();
		windowB->setX(windowB->x() + 250);

		SceneB* nScene = new SceneB();

		std::cout << Application::windows[2].get()->loadScene((Scene*)nScene) << "\n";
		std::cout << this->loadedWin->loadScene((Scene*)nScene) << "\n";
		logLoaded();

		std::cout << Application::windows[2].get()->unloadScene((Scene*)nScene) << "\n";
		std::cout << this->loadedWin->loadScene((Scene*)nScene) << "\n";
		logLoaded();
	}
};

int WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd)
{
#ifdef _DEBUG
	if (!AllocConsole())
		printf("COULD NOT ALLOCATE A CONSOLE\n");
	HRESULT hr = GetLastError();
	if (FAILED(hr))
		return -1;

	AttachConsole(GetCurrentProcessId());
	hr = GetLastError();
	if (FAILED(hr))
		return -1;

	//redirect console prints
	FILE* outputHandle;
	freopen_s(&outputHandle, "conout$", "w", stdout);
	FILE* errHandle;
	freopen_s(&errHandle, "conerr$", "w", stderr);
#endif

	SDL_version ver;
	SDL_GetVersion(&ver);

	printf("Version: %u.%u Patch: %u\n", ver.major, ver.minor, ver.patch);

	TestScene* scene = new TestScene();
	Application::initMyztic((Scene*) scene);
	
	// tMainTest();

	SDL_Event e;
	bool needToQuit = false;
	while (!needToQuit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) needToQuit = true;
		}

		SDL_Delay(1);
	}

#ifdef _DEBUG
	if (errHandle)
		fclose(errHandle);

	if (outputHandle)
		fclose(outputHandle);
	FreeConsole();
#endif
    return 0;
}