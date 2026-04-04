#include "raylib.h"
#include "rcamera.h"

typedef struct CameraSettings {
    Vector2 sensitivity;
    Vector3 speed;
} CameraSettings;

typedef struct Game {
    Camera camera;
    CameraSettings cameraSettings;
} Game;

void Update(Game *game);
void Draw(Game *game);

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "FPS");

    Game game = {0};
    game.camera.position = (Vector3){0.0f, 2.0f, 4.0f};
    game.camera.target = (Vector3){0.0f, 2.0f, 0.0f};
    game.camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    game.camera.fovy = 60.0f;
    game.camera.projection = CAMERA_PERSPECTIVE;
    game.cameraSettings.sensitivity = (Vector2){0.1f, 0.1f};
    game.cameraSettings.speed = (Vector3){0.2f, 0.2f, 0.2f};

    DisableCursor();

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        Update(&game);
        Draw(&game);
    }

    CloseWindow();

    return 0;
}

void Update(Game *game) {
    UpdateCameraPro(
        &game->camera,
        (Vector3){
            IsKeyDown(KEY_W) * game->cameraSettings.speed.z -
                IsKeyDown(KEY_S) *
                    game->cameraSettings.speed.z, // forward and back
            IsKeyDown(KEY_D) * game->cameraSettings.speed.x -
                IsKeyDown(KEY_A) *
                    game->cameraSettings.speed.x, // right and left
            0.0f                                  // up and down
        },
        (Vector3){GetMouseDelta().x * game->cameraSettings.sensitivity.x,
                  GetMouseDelta().y * game->cameraSettings.sensitivity.y, 0.0f},
        0.0f);
}

void Draw(Game *game) {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    BeginMode3D(game->camera);
    DrawPlane((Vector3){0.0f, 0.0f, 0.0f}, (Vector2){32.0f, 32.0f},
              LIGHTGRAY); // Draw ground
    DrawCube((Vector3){-16.0f, 2.5f, 0.0f}, 1.0f, 5.0f, 32.0f,
             BLUE); // Draw a blue wall
    DrawCube((Vector3){16.0f, 2.5f, 0.0f}, 1.0f, 5.0f, 32.0f,
             LIME); // Draw a green wall
    DrawCube((Vector3){0.0f, 2.5f, 16.0f}, 32.0f, 5.0f, 1.0f,
             GOLD); // Draw a yellow wall
    EndMode3D();

    EndDrawing();
}