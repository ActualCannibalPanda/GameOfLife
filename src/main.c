#include <raylib.h>

#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>

size_t MAP_WIDTH = 32;
size_t MAP_HEIGHT = 32;
size_t TILE_WIDTH = 32;
size_t TILE_HEIGHT = 32;

typedef struct Game {
    Camera2D camera;
    int screenWidth;
    int screenHeight;
    int seed;
    float mapScale;
    bool *map;
} Game;

void InitGame(Game *game, int screenWidth, int screenHeight);
void Update(Game *game);
void Draw(Game *game);
void DeinitGame(Game *game);

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;

    srand((unsigned int)time(NULL));

    InitWindow(screenWidth, screenHeight, "FPS");

    Game game = {0};
    InitGame(&game, screenWidth, screenHeight);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        Update(&game);
        Draw(&game);
    }

    DeinitGame(&game);
    CloseWindow();

    return 0;
}

void InitGame(Game *game, int screenWidth, int screenHeight) {
    game->screenWidth = screenWidth;
    game->screenHeight = screenHeight;

    float ratio = (float)GetScreenHeight() / (float)GetScreenWidth();

    int mapWidth = TILE_WIDTH * MAP_WIDTH;
    int mapHeight = TILE_HEIGHT * MAP_HEIGHT;

    game->camera.target = (Vector2){mapWidth / 2.0f, mapHeight / 2.0f};
    game->camera.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    game->camera.rotation = 0.0f;
    game->camera.zoom =
        (float)GetScreenHeight() / (float)(TILE_HEIGHT * MAP_HEIGHT);

    game->map = (bool *)malloc(MAP_WIDTH * MAP_HEIGHT * sizeof(bool));
    game->mapScale = 1.0f;
    game->seed = rand();

    srand(game->seed);
    for (size_t x = 0; x < MAP_WIDTH; ++x) {
        for (size_t y = 0; y < MAP_HEIGHT; ++y) {
            if ((float)rand() / (float)RAND_MAX < 0.2) {
                game->map[x * MAP_WIDTH + y] = true;
            } else {
                game->map[x * MAP_WIDTH + y] = false;
            }
            // game->map[x * MAP_WIDTH + y] = (x + y) % 2 == 0;
        }
    }
}

void Update(Game *game) {
    if (IsKeyDown(KEY_UP)) {
        game->camera.zoom = expf(logf(game->camera.zoom) + 0.01f);
    }
    if (IsKeyDown((KEY_DOWN))) {
        game->camera.zoom = expf(logf(game->camera.zoom) - 0.01f);
    }

    if (IsKeyDown(KEY_W)) {
        game->camera.target.y += 5.0f;
    }
    if (IsKeyDown(KEY_S)) {
        game->camera.target.y -= 5.0f;
    }

    if (IsKeyDown(KEY_A)) {
        game->camera.target.x += 5.0f;
    }
    if (IsKeyDown(KEY_D)) {
        game->camera.target.x -= 5.0f;
    }
}

void Draw(Game *game) {
    BeginDrawing();

    ClearBackground(BLACK);

    BeginMode2D(game->camera);

    float ratio = (float)GetScreenHeight() / (float)GetScreenWidth();

    int width = TILE_WIDTH * game->mapScale;
    int height = TILE_HEIGHT * game->mapScale;

    // DrawRectangle(0, 0, width * MAP_WIDTH, height * MAP_HEIGHT, BLACK);
    for (size_t x = 0; x < MAP_WIDTH; ++x) {
        for (size_t y = 0; y < MAP_HEIGHT; ++y) {
            if (game->map[x * MAP_WIDTH + y]) {
                DrawRectangle(x * width, y * height, width, height, GREEN);
            }
        }
    }

    EndMode2D();

    EndDrawing();
}

void DeinitGame(Game *game) {
    if (game->map != NULL) {
        free(game->map);
        game->map = NULL;
    }
}