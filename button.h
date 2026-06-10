#ifndef BUTTON_H
#define BUTTON_H
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "metronome.h"

#define TEXT_COLOR (SDL_Color){255, 255, 255, 255}


typedef struct Button
{
    SDL_FRect rect;
    SDL_Texture *texture;
    bool pressed;
    bool hovered;
    bool hasInput;

} Button;

void setButtonTexture(SDL_Renderer *renderer, TTF_Font *font, Button *btn, char *title);
void handlePlayButtonPress(SDL_Event *e, Button *btn, Metronome* metronome);
void drawButton(SDL_Renderer *renderer, Button *btn, bool isEditing);

#endif
