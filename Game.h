#ifndef PONG_SDL_GAME_H
#define PONG_SDL_GAME_H

#include <SDL.h>
#include "Vector2.h"

namespace pong_sdl {

  class Game {
  public:
    Game(int width, int height, Vector2 ball_vel);

    bool Initialize();

    void Shutdown();

    void RunLoop();

  private:
    void ProcessInput();

    void GenerateOutput();

    void UpdateGame();

    bool running_{true};
    int width_;
    int height_;
    SDL_Window *window_{nullptr};
    SDL_Renderer *renderer_{nullptr};
    Uint32 ticks_count_{0};
    Vector2 ball_pos_;
    Vector2 ball_vel_;
    Vector2 paddle_pos_;
    int paddle_dir_{0};
  };

}


#endif //PONG_SDL_GAME_H
