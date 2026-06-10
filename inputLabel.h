#ifndef INPUTLABEL_H
#define INPUTLABEL_H

#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "SDL3/SDL_stdinc.h"
#include "metronome.h"

#define TEXT_COLOR_INPUT (SDL_Color){230, 230, 230, 255}


typedef struct InputLabel{
    SDL_FRect rect;
    SDL_Texture *texture;
    bool isEditing;
    bool hovered;

    Uint64 caretBlinkStart;
} InputLabel;

void setInputTexture(InputLabel *label, SDL_Renderer *renderer, TTF_Font *font, char *title);
void handleInputButtonPress(InputLabel *label, SDL_Event *e, Metronome* metronome, char* buffer);
void drawInputLabel(InputLabel *label, SDL_Renderer *renderer);
bool isEditingBpm(InputLabel *label, SDL_Event *e);



#endif
