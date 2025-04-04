#include <SDL2/SDL.h>
#include <iostream>

const int WIDTH = 1280;
const int HEIGHT = 720;

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

    bool running = true;
    SDL_Event event;

    int lineY = HEIGHT / 2;
	int lastMouseX = 0;
	int lastMouseY = 0;
	int deltaMouseX = 0;
	int deltaMouseY = 0;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            #pragma region InputSetup

			deltaMouseX = 0;
			deltaMouseY = 0;
            if (event.type == SDL_MOUSEMOTION) {
				deltaMouseX = event.motion.x - lastMouseX;
				deltaMouseY = event.motion.y - lastMouseY;
                std::cout << "x: " << deltaMouseX
                    << ", y: " << deltaMouseY << '\n';

				lastMouseX = event.motion.x;
				lastMouseY = event.motion.y;
            }


            if (event.type == SDL_MOUSEWHEEL) {
				if (event.wheel.y > 0) {
					lineY -= 10; 
				}
				else if (event.wheel.y < 0) {
					lineY += 10; 
				}
            }


            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_UP:
                    lineY -= 10; 
                    break;
                case SDLK_DOWN:
                    lineY += 10; 
                    break;
                
                }
            }
            #pragma endregion
        }

        std::cout << "x: " << deltaMouseX
            << ", y: " << deltaMouseY << '\n';

        // Kopiowanie danych do tekstury i renderowanie

        SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Ustawienie koloru tła

        SDL_RenderDrawLine(renderer, 0, lineY, WIDTH, lineY); // Przykładowa linia
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderPresent(renderer);

        SDL_Delay(32); // Ograniczenie do ~60 FPS
    }


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
