#include "button.h"
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "player.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void setButtonTexture(SDL_Renderer *renderer, TTF_Font *font, Button *btn,
                      char *title) {

  if (btn->texture) {
    SDL_DestroyTexture(btn->texture);
    btn->texture = NULL;
    if (strlen(title) == 0) {
      return;
    }
  }

  SDL_Surface *surf = TTF_RenderText_Blended(font, title, 0, TEXT_COLOR);
  if (surf == NULL) {
    SDL_Log("Error creating the surface for buffer '%s': %s\n", title,
            SDL_GetError());
  }
  btn->texture = SDL_CreateTextureFromSurface(renderer, surf);
  if (btn->texture == NULL) {
    SDL_Log("Error creating the texture for buffer '%s': %s\n", title,
            SDL_GetError());
  }
  SDL_DestroySurface(surf);
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

void drawButton(SDL_Renderer *renderer, Button *btn, bool isEditing) {

  if (btn->pressed) {
    SDL_SetRenderDrawColor(renderer, 255, 12, 10, 255);
    // btn->pressed = false;
  } else if (btn->hovered) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    btn->hovered = false;
  } else {
    SDL_SetRenderDrawColor(renderer, 54, 54, 54, 53);
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

    if (isEditing) {
      SDL_FRect caret = {(dst.x + dst.w), (dst.y), 2.0F, dst.h};
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      SDL_RenderFillRect(renderer, &caret);
    }
  }
}
