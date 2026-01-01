#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <stdio.h>
void stampa(void) { printf("ciao come va"); }

int main(int argv, char *argc[]) {
  char *file = argc[1];
  if (argv != 2) {
    printf("Usage: ./image_viewer <image_name> (for example: ./image_viewer "
           "image_1.ppm)\n");
    return 1;
  }
  char path[256];
  sprintf(path, "/home/andrea/projects/image_viewer/%s", file);
  FILE *p = fopen(path, "rb");
  if (p == NULL) {
    printf("Invalid file name or type\n");
    return 1;
  }
  printf("Rendering: %s\n", argc[1]);
  char format[3];
  int width, height, maxval;
  char line[256];

  // Read format
  fscanf(p, "%2s", format);

  // Skip any leftover newline after format
  fgets(line, sizeof(line), p);
  stampa();

  // Read comment line
  char *commento =
      fgets(line, sizeof(line), p); // now this is the actual comment

  // Read width and height
  fscanf(p, "%d %d", &width, &height);

  // Read maxval
  fscanf(p, "%d", &maxval);

  printf("Width: %d, Height: %d\n", width, height);

  SDL_Window *image =
      SDL_CreateWindow("image", width, height, SDL_WINDOW_ALWAYS_ON_TOP);
  SDL_Renderer *renderer = SDL_CreateRenderer(image, "");
  int r, g, b, x, y;
  r = g = b = x = y = 0;
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderClear(renderer);
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      int b = getc(p);
      int r = getc(p);
      int g = getc(p);

      SDL_SetRenderDrawColor(renderer, r, g, b, 255);
      SDL_FRect pixel = {x, y, 1, 1};
      SDL_RenderFillRect(renderer, &pixel);
    }
  }
  SDL_RenderPresent(renderer);
  int running = 1;
  SDL_Event event;

  while (running) {
    // Process events
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        running = 0; // window close button pressed
      }
    }
    SDL_Delay(16); // ~60 FPS
  }
}
