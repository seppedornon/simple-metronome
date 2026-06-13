#include "inputLabel.h"
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_timer.h"
#include "player.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static const Uint64 CARET_BLINK_PERIOD_MS = 1000ULL;
static const Uint64 CARET_BLINK_ON_MS = 500ULL;

void setInputTexture(InputLabel *label, SDL_Renderer *renderer, TTF_Font *font,
                     char *title) {
  if (label->texture) {

    SDL_DestroyTexture(label->texture);
    label->texture = NULL;

    if (strlen(title) == 0) {
      return;
    }
  }

  SDL_Surface *surf = TTF_RenderText_Blended(font, title, 0, TEXT_COLOR_INPUT);

  if (surf == NULL) {
    SDL_Log("Error creating the surface for buffer '%s': %s\n", title,
            SDL_GetError());
  }

  label->texture = SDL_CreateTextureFromSurface(renderer, surf);

  if (label->texture == NULL) {
    SDL_Log("Error creating the texture for buffer '%s': %s\n", title,
            SDL_GetError());
  }

  SDL_DestroySurface(surf);
}

void handleInputButtonPress(InputLabel *label, SDL_Event *e,
                            Metronome *metronome, char *buffer) {
  size_t bpmlen = strlen(buffer);
  if (e->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
    float mx, my;
    SDL_GetMouseState(&mx, &my);

    if (isMouseInRect(mx, my, &(label->rect))) {
      label->isEditing = true;
      SDL_StartTextInput(metronome->window);

      label->caretBlinkStart = SDL_GetTicks();

      bpmlen = strlen(buffer);
      setInputTexture(label, metronome->renderer, metronome->font, buffer);

    } else if (label->isEditing) {
      label->isEditing = false;
      label->caretBlinkStart = 0;
      SDL_StopTextInput(metronome->window);
      int newBpm = atoi(buffer);
      if (newBpm > 0 && newBpm != metronome->player->bpm)
        setBpm(metronome->player, newBpm);

      snprintf(buffer, sizeof(buffer), "%d", metronome->player->bpm);
      bpmlen = strlen(buffer);
      setInputTexture(label, metronome->renderer, metronome->font, buffer);
    }
  }

  if (label->isEditing && e->type == SDL_EVENT_TEXT_INPUT) {
    const char *txt = e->text.text;
    for (size_t i = 0; txt[i] != '\0' && bpmlen + 1 < sizeof(buffer); i++) {
      if (txt[i] >= '0' && txt[i] <= '9') {
        buffer[bpmlen++] = txt[i];
      }
    }
    buffer[bpmlen] = '\0';
    setInputTexture(label, metronome->renderer, metronome->font, buffer);
    label->caretBlinkStart = SDL_GetTicks();
  }
  if (label->isEditing && e->type == SDL_EVENT_KEY_DOWN) {
    if (e->key.key == SDLK_BACKSPACE) {
      if (bpmlen > 0) {
        buffer[--bpmlen] = '\0';
        SDL_Log("buffer: %s\n", buffer);
        setInputTexture(label, metronome->renderer, metronome->font, buffer);
        label->caretBlinkStart = SDL_GetTicks();
      }
    } else if (e->key.key == SDLK_RETURN) {
      int newBpm = atoi(buffer);
      if (newBpm > 0 && newBpm != metronome->player->bpm)
        setBpm(metronome->player, newBpm);

      label->isEditing = false;
      label->caretBlinkStart = 0;
      SDL_StopTextInput(metronome->window);
      snprintf(buffer, sizeof(buffer), "%d", metronome->player->bpm);
      bpmlen = strlen(buffer);
      setInputTexture(label, metronome->renderer, metronome->font, buffer);
    } else if (e->key.key == SDLK_ESCAPE) {
      // cancel editing: restore displayed BPM
      label->isEditing = false;
      label->caretBlinkStart = 0;
      SDL_StopTextInput(metronome->window);
      snprintf(buffer, sizeof(buffer), "%d", metronome->player->bpm);
      bpmlen = strlen(buffer);
      setInputTexture(label, metronome->renderer, metronome->font, buffer);
    }
  }
}

void drawCaret(InputLabel *label, SDL_FRect *dest, SDL_Renderer *renderer) {
  Uint64 now = SDL_GetTicks();
  SDL_FRect caret;
  if (label->caretBlinkStart == 0)
    label->caretBlinkStart = now;

  if (((now - label->caretBlinkStart) % CARET_BLINK_PERIOD_MS) <
      CARET_BLINK_ON_MS) {
    SDL_FRect caret;
    if (label->texture != NULL) {
      // caret after the text
      caret.x = dest->x + dest->w + 2.0f;    // a small gap
      caret.y = dest->y + (dest->h * 0.15f); // shrink vertically a bit
      caret.w = 2.0f;
      caret.h = dest->h * 0.7f;
    } else {
      // fallback if there's no text texture (empty buffer)
      caret.x = label->rect.x + (label->rect.w / 2);
      caret.y = label->rect.y + (label->rect.h * 0.25f);
      caret.w = 2.0f;
      caret.h = label->rect.h * 0.5f;
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &caret);
  }
}

void drawInputLabel(InputLabel *label, SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 10, 255);

  SDL_RenderFillRect(renderer, &(label->rect));

  // border
  SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
  SDL_RenderRect(renderer, &(label->rect));

  SDL_FRect dst;
  if (label->texture != NULL) {

    SDL_GetTextureSize(label->texture, &dst.w, &dst.h);
    dst.x = (label->rect.x + (label->rect.w / 2)) - (dst.w / 2);
    dst.y = (label->rect.y + (label->rect.h / 2)) - (dst.h / 2);
    SDL_RenderTexture(renderer, label->texture, NULL, &dst);
  }
  if (label->isEditing) {
    drawCaret(label, &dst, renderer);
  }
}
bool isEditingBpm(InputLabel *label, SDL_Event *e) {
  if (e->key.key == SDLK_ESCAPE || e->key.key == SDLK_RETURN) {
    return false;
  }

  if (e->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
    float mx, my;
    SDL_GetMouseState(&mx, &my);

    return isMouseInRect(mx, my, &(label->rect));
  }

  return false;
}
