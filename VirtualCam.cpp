#include <SDL2/SDL.h>
#include <iostream>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <iostream>
#include <vector>
#include "Camera.h"
#include "Edge.h"
#include "Shape.h"

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

Eigen::Vector2f project(const Eigen::Vector4f& point, const Eigen::Matrix4f& projectionMatrix) {
    Eigen::Vector4f projected = projectionMatrix * point;
    return Eigen::Vector2f(projected.x() / projected.w(), projected.y() / projected.w());
}

Eigen::Vector2f myProject(const Eigen::Vector4f& point, const Eigen::Matrix4f& projectionMatrix) {
    Eigen::Vector4f projected = projectionMatrix * point;
    return Eigen::Vector2f(projected.x() / projected.w(), projected.y() / projected.w());
}

void drawShape(SDL_Renderer* renderer, const Shape& shape, const Eigen::Matrix4f& projectionMatrix) {
    for (const Edge& edge : shape.getEdges()) {
        Eigen::Vector2f start = project(edge.getStart(), projectionMatrix);
        Eigen::Vector2f end = project(edge.getEnd(), projectionMatrix);

        SDL_RenderDrawLine(renderer,
            static_cast<int>((start.x() + 1) * WIDTH / 2),
            static_cast<int>((1 - start.y()) * HEIGHT / 2),
            static_cast<int>((end.x() + 1) * WIDTH / 2),
            static_cast<int>((1 - end.y()) * HEIGHT / 2));
    }
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

    float d = 1.0f;
    float a = (float)WIDTH / (float)HEIGHT;
	std::cout << "a: " << a << ", d: " << d << std::endl;
    Eigen::Matrix4f projectionMatrix;
    projectionMatrix <<
        1/a, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 1/d, 0;


    SDL_Event event;
    bool running = true;
	int lastMouseX = 0;
	int lastMouseY = 0;
	int deltaMouseX = 0;
	int deltaMouseY = 0;
    bool cursorLock = true;
	float d_step = 0.2f;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            #pragma region InputSetup
            
            if (event.type == SDL_MOUSEMOTION) {
				deltaMouseX = event.motion.x - lastMouseX;
				deltaMouseY = event.motion.y - lastMouseY;
				lastMouseX = event.motion.x;
				lastMouseY = event.motion.y;
            }


            if (event.type == SDL_MOUSEWHEEL) {
				if (event.wheel.y > 0) {
				}
				else if (event.wheel.y < 0) {
				}
            }


            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_UP:
					//Do when up arrow is pressed 
                    break;
                case SDLK_DOWN:
					//Do when down arrow is pressed
                    break;
				case SDLK_LEFT:
					//Do when left arrow is pressed
					break;
				case SDLK_RIGHT:
					//Do when right arrow is pressed
					break;
				case SDLK_w:
					//Do when W is pressed
					break;
				case SDLK_s:
					//Do when S is pressed
					break;
				case SDLK_a:
					//Do when A is pressed
				break;
				case SDLK_d:
					//Do when D is pressed
					break;

                
                }
            }
            #pragma endregion
        }

        // Kopiowanie danych do tekstury i renderowanie

        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        for (const Shape& shape : Shapes) {
            drawShape(renderer, shape, projectionMatrix);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderPresent(renderer);

        SDL_Delay(32); // Ograniczenie do ~60 FPS
    }


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
