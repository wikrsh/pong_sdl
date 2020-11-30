#include "Game.h"

int main() {
  pong_sdl::Game game{1024, 768, {-200.0f, 235.0f}};

  if (game.Initialize()) {
    game.RunLoop();
  }

  game.Shutdown();

  return 0;
}
