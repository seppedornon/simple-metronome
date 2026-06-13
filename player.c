#include "player.h"

void setPlayerSound(MIX_Track *track, char *filePath, MIX_Mixer *mixer) {
  MIX_Audio *audio = NULL;
  audio = MIX_LoadAudio(mixer, filePath, false);

  if (!audio) {
    SDL_Log("Couldn't load %s: %s", filePath, SDL_GetError());
  }

  MIX_SetTrackAudio(track, audio);
}
Uint32 playerCallback(void *userdate, SDL_TimerID timerID, Uint32 interval) {
  Player *player = (Player *)userdate;

  MIX_PlayTrack(player->track, 0);

  return interval;
}

void play(Player *player) {
  if (!player->isPlaying) {
    int interval = (60000 / player->bpm);
    MIX_PlayTrack(player->track, 0);
    player->timer = SDL_AddTimer(interval, playerCallback, player);
    player->isPlaying = true;
  }
}

void pause(Player *player) {
  if (player->isPlaying) {
    SDL_RemoveTimer(player->timer);
    player->isPlaying = false;
  }
}

void setBpm(Player *player, int bpm) {
  player->bpm = bpm;
  if (player->isPlaying) {
    pause(player);
    play(player);
  }
}
