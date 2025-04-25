#include <SDL2/SDL.h>
#include <iostream>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <iostream>
#include <vector>

#include "Edge.h"
#include "Shape.h"
#include "MyRenderer.h"
#include "Scene.h"

const int WIDTH = 1280;
const int HEIGHT =  720;

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("SDL VirtualCam", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Error creating window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Error creating renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }


	Scene scene;
	scene.loadFromFile("Scene1.txt");

	MyRenderer myRenderer(renderer, 1, WIDTH, HEIGHT);

    SDL_Event event;
    bool running = true;
	int lastMouseX = 0;
	int lastMouseY = 0;
	int deltaMouseX = 0;
	int deltaMouseY = 0;
    bool cursorLock = true;


    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            #pragma region InputSetup
            


            if (event.type == SDL_MOUSEWHEEL) {
				myRenderer.stepD(event.wheel.y);
            }


            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_UP:
                    myRenderer.RotateCamera(-4, 0, 0);
                    break;
                case SDLK_DOWN:
					myRenderer.RotateCamera(4, 0, 0);
                    break;
				case SDLK_LEFT:
                    myRenderer.RotateCamera(0, -4, 0);
					break;
				case SDLK_RIGHT:
                    myRenderer.RotateCamera(0, 4, 0);
					break;
                case SDLK_q:
                    myRenderer.RotateCamera(0, 0, 4);
                    break;
                case SDLK_e:
                    myRenderer.RotateCamera(0, 0, -4);
                    break;
				case SDLK_w:
					myRenderer.moveCameraForward(1);
					break;
				case SDLK_s:
                    myRenderer.moveCameraForward(-1);
					break;
				case SDLK_a:
					myRenderer.moveCameraRight(-1);
				break;
				case SDLK_d:
                    myRenderer.moveCameraRight(1);
					break;
                case SDLK_SPACE:
                    myRenderer.moveCameraUp(1);
                    break;
                case SDLK_LCTRL:
                    myRenderer.moveCameraUp(-1);
                    break;
				case SDLK_ESCAPE:
                    std::cout << "ViewMatrix:\n" << myRenderer.getViewMatrix() << "\n";
					break;
                
                }
            }
            #pragma endregion
        }



        SDL_RenderClear(renderer);
		myRenderer.RecalculateViewMatrix();
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        for (const Shape& shape : scene.getShapes()) {
            myRenderer.drawShape(shape);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderPresent(renderer);

        SDL_Delay(16); 
    }


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
