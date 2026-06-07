#ifndef METRONOME_H
#define METRONOME_H

#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include <stdio.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>
#include "player.h"

#define WIDTH 400
#define HEIGHT 750
#define TEXT_SIZE 20

typedef struct Metronome{
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    Player *player;
} Metronome;

void clearWindow(SDL_Renderer *renderer);
void InitSetup(Metronome* metronome, MIX_Mixer *mixer);
void end(Metronome* metronome);



#endif
