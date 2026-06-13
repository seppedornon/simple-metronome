#ifndef METRONOME_H
#define METRONOME_H

#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "player.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>

#define WIDTH 400
#define HEIGHT 750
#define TEXT_SIZE 50

typedef struct Metronome {
  SDL_Window *window;
  SDL_Renderer *renderer;
  TTF_Font *font;
  Player *player;
} Metronome;

void clearWindow(SDL_Renderer *renderer);
void InitSetup(Metronome *metronome, MIX_Mixer *mixer);
void end(Metronome *metronome);

bool isMouseInRect(float mx, float my, SDL_FRect *rect);

#endif
