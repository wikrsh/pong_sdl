#include <SDL.h>
#include "Game.h"

namespace pong_sdl {

  const int kThickness = 15;
  const int kPaddleHeight = 128;

  Game::Game(int width, int height, Vector2 ball_vel)
      : width_{width},
        height_{height},
        ball_pos_{static_cast<float>(width) / 2.0f, static_cast<float>(height) / 2.0f},
        ball_vel_{ball_vel},
        paddle_pos_{kThickness, static_cast<float>(height) / 2.0f} {}

  // Initialize SDL, window, and renderer.
  // Returns true if initialization is successful, otherwise returns false.
  bool Game::Initialize() {
    int sdl_result = SDL_Init(SDL_INIT_VIDEO);
    if (sdl_result != 0) {
      SDL_Log("Unable to Initialize SDL: %s", SDL_GetError());
      return false;
    }

    window_ = SDL_CreateWindow("hello_sdl", 100, 100, width_, height_, 0);
    if (!window_) {
      SDL_Log("Failed to create window: %s", SDL_GetError());
      return false;
    }

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer_) {
      SDL_Log("Failed to create renderer: %s", SDL_GetError());
      return false;
    }

    return true;
  }

  // Destroys renderer and window, and quits SDL.
  void Game::Shutdown() {
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
  }

  // Run game loops.
  void Game::RunLoop() {
    while (running_) {
      ProcessInput();
      UpdateGame();
      GenerateOutput();
    }
  }

  // Processes user inputs such as window close and key inputs,
  // and then changes game states.
  void Game::ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          running_ = false;
          break;
      }
    }

    const Uint8 *state = SDL_GetKeyboardState(nullptr);
    if (state[SDL_SCANCODE_ESCAPE]) {
      running_ = false;
    }

    paddle_dir_ = 0;
    if (state[SDL_SCANCODE_W]) {
      paddle_dir_ = -1;
    }
    if (state[SDL_SCANCODE_S]) {
      paddle_dir_ = 1;
    }
  }

  // Render a frame to buffer.
  void Game::GenerateOutput() {
    // Background
    SDL_SetRenderDrawColor(renderer_, 0, 0, 255, 255);
    SDL_RenderClear(renderer_);

    SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);

    // Render walls
    SDL_Rect walls[] = {
        {0, 0, width_, kThickness}, // top
        {width_ - kThickness, 0, kThickness, height_}, // right
        {0, height_ - kThickness, width_, kThickness}, // bottom
    };
    for (SDL_Rect &wall: walls) {
      SDL_RenderFillRect(renderer_, &wall);
    }

    // Render a ball
    SDL_Rect ball{
        static_cast<int>(ball_pos_.x - kThickness / 2.0),
        static_cast<int>(ball_pos_.y - kThickness / 2.0),
        kThickness,
        kThickness
    };
    SDL_RenderFillRect(renderer_, &ball);

    // Render a paddle
    SDL_Rect paddle{
        static_cast<int>(paddle_pos_.x - kThickness / 2.0),
        static_cast<int>(paddle_pos_.y - kPaddleHeight / 2.0),
        kThickness,
        kPaddleHeight
    };
    SDL_RenderFillRect(renderer_, &paddle);

    SDL_RenderPresent(renderer_);
  }

  // Updates objects' states
  void Game::UpdateGame() {
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticks_count_ + 16));

    Uint32 now = SDL_GetTicks();

    float deltaTime = static_cast<float>(now - ticks_count_) / 1000.0f;
    deltaTime = fmin(deltaTime, 0.05f);

    ticks_count_ = now;

    // Update the paddle position.
    if (paddle_dir_ != 0) {
      paddle_pos_.y += static_cast<float>(paddle_dir_) * 300.0f * deltaTime;
      paddle_pos_.y = fmax(paddle_pos_.y, kPaddleHeight / 2.0f + kThickness);
      paddle_pos_.y = fmin(paddle_pos_.y, static_cast<float>(height_) - kPaddleHeight / 2.0f - kThickness);
    }

    // Update the ball position.
    ball_pos_.x += ball_vel_.x * deltaTime;
    ball_pos_.y += ball_vel_.y * deltaTime;

    // Check collision between the ball and the walls.
    // If the ball collides with the wall, reverse ball's velocity.

    // Top wall
    if (ball_pos_.y <= kThickness && ball_vel_.y < 0) {
      ball_vel_.y *= -1;
    }
    // Right wall
    if (ball_pos_.x >= static_cast<float>(width_ - kThickness) && ball_vel_.x > 0) {
      ball_vel_.x *= -1;
    }
    // Bottom wall
    if (ball_pos_.y >= static_cast<float>(height_ - kThickness) && ball_vel_.y > 0) {
      ball_vel_.y *= -1;
    }

    // Check collision between the ball and the paddle.
    if (ball_vel_.x < 0
        && (ball_pos_.x <= (paddle_pos_.x + kThickness / 2.0))
        && (ball_pos_.y > (paddle_pos_.y - kPaddleHeight / 2.0))
        && (ball_pos_.y < (paddle_pos_.y + kPaddleHeight / 2.0))) {
      ball_vel_.x *= -1;
    }

    // If the ball is going outside of window, quits the game.
    if (ball_pos_.x < 0.0f) {
      running_ = false;
    }
  }

}
