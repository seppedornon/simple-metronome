#include "button.h"
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "player.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void setButtonTexture(SDL_Renderer *renderer, TTF_Font *font, Button *btn,
                      char *title) {



  if(btn->texture){
      SDL_DestroyTexture(btn->texture);
      btn->texture = NULL;
      if(strlen(title) == 0){
          return;
      }
  }

  SDL_Surface *surf = TTF_RenderText_Blended(font, title, 0, TEXT_COLOR);
  if (surf == NULL) {
      SDL_Log("Error creating the surface for buffer '%s': %s\n",title, SDL_GetError());
  }
  btn->texture = SDL_CreateTextureFromSurface(renderer, surf);
  if (btn->texture == NULL) {
    SDL_Log("Error creating the texture for buffer '%s': %s\n",title, SDL_GetError());

  }
  SDL_DestroySurface(surf);
}

bool isMouseInRect(float mx, float my, SDL_FRect *rect) {
  return (mx >= rect->x && mx <= (rect->x + rect->w) && my >= rect->y &&
          my <= (rect->y + rect->h));
}
void handlePlayButtonPress(SDL_Event *e, Button *btn, Metronome *metronome) {
  if (e->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
    float mx, my;
    SDL_GetMouseState(&mx, &my);

    if (isMouseInRect(mx, my, &(btn->rect))) {
      btn->pressed = !btn->pressed;
      btn->pressed ? play(metronome->player) : pause(metronome->player);
    }
  }
}
void handleBpmInputButtonPress(SDL_Event *e, Button *btn, Metronome *metronome, char* buffer, bool* isEditing) {

    size_t bpmlen = strlen(buffer);
    if(e->type == SDL_EVENT_MOUSE_BUTTON_DOWN){
        float mx, my;
        SDL_GetMouseState(&mx, &my);

        if(isMouseInRect(mx, my, &(btn->rect))){
            *isEditing = true;
            SDL_StartTextInput(metronome->window);

            bpmlen = strlen(buffer);
                                setButtonTexture(metronome->renderer, metronome->font, btn, buffer);

        } else if(*isEditing){
            *isEditing = false;
            SDL_StopTextInput(metronome->window);
            int newBpm = atoi(buffer);
            if(newBpm > 0 && newBpm != metronome->player->bpm) setBpm(metronome->player,newBpm);

            snprintf(buffer, sizeof(buffer), "%d", metronome->player->bpm);
            bpmlen = strlen(buffer);
            setButtonTexture(metronome->renderer, metronome->font, btn, buffer);
        }
    }

    if(*isEditing && e->type == SDL_EVENT_TEXT_INPUT){
        const char* txt = e->text.text;
        for(size_t i = 0; txt[i] != '\0' && bpmlen + 1 < sizeof(buffer); i++){
            if(txt[i] >='0' && txt[i] <= '9'){
                buffer[bpmlen++] = txt[i];
            }
        }
        buffer[bpmlen] = '\0';
        setButtonTexture(metronome->renderer, metronome->font, btn, buffer);
    }
    if (*isEditing && e->type == SDL_EVENT_KEY_DOWN) {
        if (e->key.key == SDLK_BACKSPACE) {
            if (bpmlen > 0) {
                buffer[--bpmlen] = '\0';
                SDL_Log("buffer: %s\n", buffer);
                setButtonTexture(metronome->renderer, metronome->font, btn, buffer);
            }
        } else if (e->key.key == SDLK_RETURN) {
            int newBpm = atoi(buffer);
            if (newBpm > 0 && newBpm != metronome->player->bpm) setBpm(metronome->player, newBpm);

            *isEditing = false;
            SDL_StopTextInput(metronome->window);
            snprintf(buffer, sizeof(buffer), "%d", metronome->player->bpm);
            bpmlen = strlen(buffer);
            setButtonTexture(metronome->renderer, metronome->font, btn, buffer);
        } else if (e->key.key == SDLK_ESCAPE) {
            // cancel editing: restore displayed BPM
            *isEditing = false;
            SDL_StopTextInput(metronome->window);
            snprintf(buffer, sizeof(buffer), "%d", metronome->player->bpm);
            bpmlen = strlen(buffer);
            setButtonTexture(metronome->renderer, metronome->font, btn, buffer);
        }
    }

}

bool isEditingBpm(SDL_Event *e, Button *btn) {

  if (e->key.key == SDLK_ESCAPE || e->key.key == SDLK_RETURN) {
    return false;
  }

  if (e->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
    float mx, my;
    SDL_GetMouseState(&mx, &my);

    return isMouseInRect(mx, my, &(btn->rect));
  }

  return false;
}
void setupInputBtn(SDL_Renderer *renderer, Button *btn) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 10, 255);
}

void setupPlayBtn(SDL_Renderer *renderer, Button *btn) {
  if (btn->pressed) {
    SDL_SetRenderDrawColor(renderer, 255, 12, 10, 255);
    // btn->pressed = false;
  } else if (btn->hovered) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    btn->hovered = false;
  } else {
    SDL_SetRenderDrawColor(renderer, 54, 54, 54, 53);
  }
}

void drawButton(SDL_Renderer *renderer, Button *btn, bool isEditing) {

  if (btn->hasInput) {
    setupInputBtn(renderer, btn);
  } else {
    setupPlayBtn(renderer, btn);
  }

  SDL_RenderFillRect(renderer, &(btn->rect));

  // border
  SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
  SDL_RenderRect(renderer, &(btn->rect));

  if (btn->texture != NULL) {
    SDL_FRect dst;

    SDL_GetTextureSize(btn->texture, &dst.w, &dst.h);
    dst.x = (btn->rect.x + (btn->rect.w / 2)) - (dst.w / 2);
    dst.y = (btn->rect.y + (btn->rect.h / 2)) - (dst.h / 2);
    SDL_RenderTexture(renderer, btn->texture, NULL, &dst);

    if(isEditing){
        SDL_FRect caret = {(dst.x + dst.w), (dst.y), 2.0F, dst.h};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &caret);
    }
  }
}
