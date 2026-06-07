#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_mouse.h"
#include "SDL3/SDL_render.h"
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <stdlib.h>
#include <string.h>
#include "metronome.h"
#include "button.h"
#include "player.h"

#define QUITKEY SDLK_ESCAPE
#define BPM 300


void loop(Metronome* metronome);

MIX_Mixer *mixer;
Button playBtn = {
    .hovered = false,
    .pressed = false,
    .texture = NULL,
    .rect = {35.00F, 644.00F, 329.00F, 43.00F}
};

Button bpmBtn = {
    .hovered = false,
    .pressed = false,
    .texture = NULL,
    .rect = {135.00F, 91.00F, 130.00F, 86.00F},
    .hasInput = true
};

Player player = {
    .bpm = BPM,
    .isPlaying = false,
    .track = NULL
};

int main(int argc, char *args[])
{
    Metronome metronome = {0};
    metronome.player = &player;
    InitSetup(&metronome, mixer);

    loop(&metronome);
    end(&metronome);
    return 0;
}


void loop(Metronome* metronome)
{
    setButtonTexture( metronome->renderer, metronome->font, &playBtn, "PLAY");

    char bpmBuff[16];
    snprintf(bpmBuff, sizeof(bpmBuff),"%d", metronome->player->bpm);
    int bpmlen = strlen(bpmBuff);
    setButtonTexture( metronome->renderer, metronome->font, &bpmBtn, bpmBuff);

    if (playBtn.texture == NULL)
    {
        SDL_Log("Fout bij texture\n");
    }
    bool quit = false;
    SDL_Event e;
    SDL_zero(e);

    bool editingBpm = false;

    while (!quit)
    {

        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
            {
                quit = true;
            }


            handlePlayButtonPress(&e, &playBtn, metronome);
            handleBpmInputButtonPress(&e, &bpmBtn, metronome, bpmBuff, &editingBpm);

        }

        float mx, my;
        SDL_GetMouseState(&mx, &my);

        if (isMouseInRect(mx, my, &(playBtn.rect)))
        {
            playBtn.hovered = true;
            // MIX_PlayTrack(metronome->track, 0);
        }

        drawButton(metronome->renderer, &playBtn, false);
        drawButton(metronome->renderer, &bpmBtn, editingBpm);
        SDL_RenderPresent(metronome->renderer);
        clearWindow(metronome->renderer);
    }
}
