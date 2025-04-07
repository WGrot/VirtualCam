#include <SDL2/SDL.h>
#include <iostream>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <iostream>
#include <vector>
#include "Camera.h"
#include "Edge.h"
#include "Shape.h"
#include "MyRenderer.h"

const int WIDTH = 1280;
const int HEIGHT =  720;


std::vector<Edge> createCubeEdges() {
    std::vector<Edge> edges;
    // Definiowanie wierzchołków sześcianu
    Eigen::Vector4f v0(-1, -1, 9, 1);
    Eigen::Vector4f v1(1, -1, 9, 1);
    Eigen::Vector4f v2(1, 1, 9, 1);
    Eigen::Vector4f v3(-1, 1, 9, 1);
    Eigen::Vector4f v4(-1, -1, 11, 1);
    Eigen::Vector4f v5(1, -1, 11, 1);
    Eigen::Vector4f v6(1, 1, 11, 1);
    Eigen::Vector4f v7(-1, 1, 11, 1);

    // Definiowanie krawędzi sześcianu
    edges.push_back(Edge(v0, v1));
    edges.push_back(Edge(v1, v2));
    edges.push_back(Edge(v2, v3));
    edges.push_back(Edge(v3, v0));
    edges.push_back(Edge(v4, v5));
    edges.push_back(Edge(v5, v6));
    edges.push_back(Edge(v6, v7));
    edges.push_back(Edge(v7, v4));
    edges.push_back(Edge(v0, v4));
    edges.push_back(Edge(v1, v5));
    edges.push_back(Edge(v2, v6));
    edges.push_back(Edge(v3, v7));

    return edges;
}



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

    std::vector<Shape> Shapes;
    Shapes.push_back(Shape(createCubeEdges()));

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
                    cursorLock = !cursorLock;
					break;
                
                }
            }
            #pragma endregion
        }

        // Kopiowanie danych do tekstury i renderowanie

        SDL_RenderClear(renderer);
		myRenderer.RecalculateViewMatrix();
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        for (const Shape& shape : Shapes) {
            myRenderer.drawShape(shape);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderPresent(renderer);

        SDL_Delay(16); // Ograniczenie do ~60 FPS
    }


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
