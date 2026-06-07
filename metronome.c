#include "metronome.h"
#include "SDL3/SDL_keyboard.h"
#include "SDL3_mixer/SDL_mixer.h"
#include "player.h"

void clearWindow(SDL_Renderer *renderer){
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 100);
    SDL_RenderClear(renderer);
}
void InitSetup(Metronome* metronome, MIX_Mixer *mixer){
    if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO))
    {
        SDL_Log("Window creation failed! Cuz: %s\n", SDL_GetError());
    }


    if (!TTF_Init())
    {
        SDL_Log("TTF creation failed! Cuz: %s\n", SDL_GetError());
    }

    if(!MIX_Init()){
        SDL_Log("MIXER creation failed: %s\n", SDL_GetError());
    }

    mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (!mixer) {
        SDL_Log("Couldn't create mixer on default device: %s", SDL_GetError());
    }

    metronome->player->track = MIX_CreateTrack(mixer);
    if (!metronome->player->track) {
        SDL_Log("Couldn't create a mixer track: %s", SDL_GetError());
    }

    setPlayerSound(metronome->player->track, "/Users/seppedornon/Desktop/metronome/sounds/metronome1.mp3", mixer);
    /* start the audio playing! */

    metronome->window = SDL_CreateWindow("Metronome", WIDTH, HEIGHT, 0);

    if(!metronome->window){
        SDL_Log("Window creation failed! Cuz: %s\n", SDL_GetError());
    }
    metronome->renderer = SDL_CreateRenderer(metronome->window, NULL);
    if(!metronome->renderer){
        SDL_Log("Renderer creation failed! Cuz: %s\n", SDL_GetError());
    }
    metronome->font = TTF_OpenFont("/Users/seppedornon/Desktop/metronome/fonts/Roboto-Black.ttf", TEXT_SIZE);
    if(!metronome->font){
        SDL_Log("Error opening the font: %s\n", SDL_GetError());
    }


}
void end(Metronome* metronome){

    TTF_CloseFont(metronome->font);
    metronome->font = NULL;
    SDL_DestroyWindow(metronome->window);
    metronome->window = NULL;

    MIX_Quit();
    TTF_Quit();
    SDL_Quit();
}
