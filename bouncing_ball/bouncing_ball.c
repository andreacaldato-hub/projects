#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <stdio.h>
int v_x;
int v_y;

struct Circle {
  double x_center;
  double y_center;
  double radius;
};

void draw_cicle(SDL_Renderer *renderer, struct Circle circle);
void step(struct Circle *circle, SDL_Renderer *renderer);
int main() {
  int vy;
  int vx;
  printf("Choose the velocity (vy vx):\n");
  scanf("%i %i", &vy, &vx);
  v_x = vx;
  v_y = vy;

  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *image =
      SDL_CreateWindow("image", 1200, 750, SDL_WINDOW_ALWAYS_ON_TOP);
  SDL_Renderer *renderer = SDL_CreateRenderer(image, "");

  struct Circle circle = {300, 200, 80};
  SDL_Event e;
  int running = 1;
  int v_x = 0;
  int v_y = 2;
  while (running) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_EVENT_QUIT)
        running = 0;
    }
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw rectangle
    draw_cicle(renderer, circle);
    step(&circle, renderer);

    // Present to window
  }

  return 0;
}

void draw_cicle(SDL_Renderer *renderer, struct Circle circle) {
  double x_low = circle.x_center - circle.radius;
  double y_low = circle.y_center - circle.radius;
  double x_high = circle.x_center + circle.radius;
  double y_high = circle.y_center + circle.radius;
  SDL_FRect pixel;
  double distance;
  double radius_squared;
  for (double x = x_low; x <= x_high; x++) {
    for (double y = y_low; y <= y_high; y++) {
      distance = (circle.x_center - x) * (circle.x_center - x) +
                 (circle.y_center - y) * (circle.y_center - y);
      radius_squared = circle.radius * circle.radius;

      if (distance <= radius_squared) {
        pixel = (SDL_FRect){x, y, 1, 1};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &pixel);
      }
    }
  }
  SDL_RenderPresent(renderer);
}

void step(struct Circle *circle, SDL_Renderer *renderer) {

  double y_low = circle->y_center + circle->radius;
  double y_high = circle->y_center - circle->radius;
  double x_low = circle->x_center - circle->radius;
  double x_high = circle->x_center + circle->radius;
  if ((y_low >= 750)) {
    v_y = -v_y;
  }
  if (y_high <= 0) {
    v_y = -v_y;
  }
  if (x_high >= 1200) {
    v_x = -v_x;
  }
  if (x_low <= 0) {
    v_x = -v_x;
  }
  double t = SDL_GetTicks();
  double time = t / 1000;
  double new_y = circle->y_center + v_y;
  double new_x = circle->x_center + v_x;
  circle->y_center = new_y;
  circle->x_center = new_x;
}
