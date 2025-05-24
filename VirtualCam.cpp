#include <SDL2/SDL.h>
#include <iostream>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <iostream>
#include <vector>
#include <random>
#include "Edge.h"
#include "Shape.h"
#include "MyRenderer.h"
#include "Scene.h"

const int WIDTH = 1280;
const int HEIGHT =  720;

const int RENDER_WIDTH = 320;
const int RENDER_HEIGHT = 180;

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

    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dist(0.3f, 1.0f);

    SDL_Surface* drawingSurface = SDL_CreateRGBSurfaceWithFormat(0, RENDER_WIDTH, RENDER_HEIGHT, 32, SDL_PIXELFORMAT_ARGB8888);

    SDL_Surface* surface = SDL_GetWindowSurface(window);


	Scene scene;
	scene.loadFromFile("phong.txt");

	MyRenderer myRenderer(renderer, drawingSurface, 1, RENDER_WIDTH, RENDER_HEIGHT);

    SDL_Event event;
    bool running = true;
	SDL_Texture* texture = nullptr;
	int lightColorCounter = 0;
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
                case SDLK_MINUS:
                    myRenderer.stepD(-1);
                    break;
                case SDLK_EQUALS:
                    myRenderer.stepD(1);
                    break;

                
                }
            }
            #pragma endregion
        }


        SDL_FillRect(drawingSurface, nullptr, SDL_MapRGB(drawingSurface->format, 0, 0, 0));

        myRenderer.RecalculateViewMatrix();
		
        myRenderer.orbitLightAroundY(Eigen::Vector3f(0, 3.0f, 5), 0.05f);
		lightColorCounter++;
        if (lightColorCounter > 100) {
            myRenderer.ChangeLightColor(dist(gen), dist(gen), dist(gen));
            lightColorCounter = 0;
        }

        myRenderer.drawFaces(scene);

        texture = SDL_CreateTextureFromSurface(renderer, drawingSurface);
        SDL_RenderClear(renderer);

        SDL_Rect dstRect = { 0, 0, WIDTH, HEIGHT };
        SDL_RenderCopy(renderer, texture, NULL, &dstRect);

        SDL_RenderPresent(renderer);
        SDL_RenderPresent(renderer);
        if (texture != nullptr) {
            SDL_DestroyTexture(texture);
        }
        

        SDL_Delay(16);
    }


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
