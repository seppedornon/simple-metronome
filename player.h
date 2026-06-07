#ifndef PLAYER_H
#define PLAYER_H

#include "SDL3/SDL_timer.h"
#include <stdio.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>



typedef struct Player{
    MIX_Track *track;
    SDL_TimerID timer;
    int bpm;
    bool isPlaying;
} Player;

void setPlayerSound(MIX_Track *track, char* filePath, MIX_Mixer* mixer);
Uint32 playerCallback(void* userdate, SDL_TimerID timerID, Uint32 interval);
void play(Player* player);
void pause(Player* player);
void setBpm(Player* player, int bpm);

#endif
