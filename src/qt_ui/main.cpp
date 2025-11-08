#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../engines/GameEngine.h"
#include <iostream>

enum MenuState {
    MENU,
    RUNNING,
    EXIT
};

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Error inicializando SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    if (TTF_Init() == -1) {
        std::cerr << "Error inicializando TTF: " << TTF_GetError() << std::endl;
        return -1;
    }

    SDL_Window* menuWindow = SDL_CreateWindow("Seleccionar Dificultad",
                                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                              640, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer* menuRenderer = SDL_CreateRenderer(menuWindow, -1, SDL_RENDERER_ACCELERATED);

    TTF_Font* font = TTF_OpenFont("/System/Library/Fonts/Supplemental/Arial.ttf", 32);
    if (!font) {
        std::cerr << "Error cargando fuente: " << TTF_GetError() << std::endl;
        return -1;
    }

    const char* options[3] = {"FACIL", "NORMAL", "DIFICIL"};
    SDL_Color white = {255, 255, 255, 255};
    SDL_Rect optionRects[3];
    MenuState menuState = MENU;
    int selectedDifficulty = -1;

    // Precalcular textos
    for (int i = 0; i < 3; i++) {
        SDL_Surface* surf = TTF_RenderText_Solid(font, options[i], white);
        SDL_Texture* tex = SDL_CreateTextureFromSurface(menuRenderer, surf);
        optionRects[i] = {220, 150 + i * 80, surf->w, surf->h};
        SDL_FreeSurface(surf);

        SDL_RenderCopy(menuRenderer, tex, NULL, &optionRects[i]);
        SDL_DestroyTexture(tex);
    }

    SDL_RenderPresent(menuRenderer);

    // Bucle del menú
    SDL_Event e;
    while (menuState == MENU) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                menuState = EXIT;
            } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                int mx = e.button.x;
                int my = e.button.y;

                for (int i = 0; i < 3; i++) {
                    if (mx >= optionRects[i].x && mx <= optionRects[i].x + optionRects[i].w &&
                        my >= optionRects[i].y && my <= optionRects[i].y + optionRects[i].h) {
                        selectedDifficulty = i;
                        menuState = RUNNING;
                        break;
                    }
                }
            }
        }
    }

    // Cerrar menú si se eligió dificultad
    SDL_DestroyRenderer(menuRenderer);
    SDL_DestroyWindow(menuWindow);
    TTF_CloseFont(font);
    TTF_Quit();

    if (menuState == EXIT) {
        SDL_Quit();
        return 0;
    }

    // Lanzar juego con la dificultad elegida
    GameEngine game(selectedDifficulty);
    if (game.init()) {
        game.run();
    }
    game.shutdown();

    return 0;
}
