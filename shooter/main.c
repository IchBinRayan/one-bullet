#include "raylib.h"

#define MAX_ENEMIES 10

// Textures
Texture2D prison;
Texture2D playertexture;
Texture2D enemyTexture;

// Vars
int playerX = 800 / 2;
int playerY = 600 / 2;
int speedX = 8;
int speedY = 8;
int playerW = 25;
int playerH = 25;
int bulletX = 300;
int bulletY = 200;
int bulletSX = 10;
int bulletSY = 10;

int score = 0;
int playerHealth = 3;

bool gameRunning = false;
bool gameOver = false;

// Enemy structure
typedef struct {
    int x;
    int y;
    int speed;
    bool active;
} Enemy;

Enemy enemies[MAX_ENEMIES];

void initGame() {
    score = 0;
    playerHealth = 3;
    gameRunning = true;
    gameOver = false;
}

void map() {
    DrawRectangle(0, 0, 50, 600, RED);
    DrawRectangle(750, 0, 50, 600, RED);
    DrawRectangle(0, 0, 800, 50, RED);
    DrawRectangle(0, 550, 800, 50, RED);
}

void drawtheplayer() {
    DrawTextureEx(playertexture, (Vector2){playerX, playerY}, 0.0f, 1.75f, WHITE);
}

void player() {
    if (IsKeyDown(KEY_D) && playerX + speedX + playerW < 752) {
        playerX += speedX;
    }
    if (IsKeyDown(KEY_A) && playerX - speedX > 45) {
        playerX -= speedX;
    }
    if (IsKeyDown(KEY_W) && playerY - speedY > 40.1) {
        playerY -= speedY;
    }
    if (IsKeyDown(KEY_S) && playerY + speedY + playerH < 550) {
        playerY += speedY;
    }
}

void bullet() {
    if (IsKeyDown(KEY_RIGHT)) {
        bulletX += bulletSX;
    }
    if (IsKeyDown(KEY_LEFT)) {
        bulletX -= bulletSX;
    }
    if (IsKeyDown(KEY_UP)) {
        bulletY -= bulletSY;
    }
    if (IsKeyDown(KEY_DOWN)) {
        bulletY += bulletSY;
    }
}

void spawnEnemy() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) {
            enemies[i].x = GetRandomValue(50, 200);
            enemies[i].y = GetRandomValue(100, 500);
            enemies[i].speed = GetRandomValue(5, 8);
            enemies[i].active = true;
            break;
        }
    }
}

void updateEnemies() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            enemies[i].x += enemies[i].speed;
            if (enemies[i].x > GetScreenWidth() + 50) {
                enemies[i].active = false;
            }
        }
    }
}

void drawEnemies() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            DrawTextureEx(enemyTexture, (Vector2){enemies[i].x, enemies[i].y}, 0.0f, 1.6f, WHITE);
        }
    }
}

void checkCollisions() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active &&
            CheckCollisionCircleRec((Vector2){bulletX, bulletY}, 12, (Rectangle){enemies[i].x, enemies[i].y, 50, 50})) {
            enemies[i].active = false;
            score++;
        }
    }

    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active &&
            CheckCollisionRecs((Rectangle){playerX, playerY, playerW, playerH}, (Rectangle){enemies[i].x, enemies[i].y, 50, 50})) {
            enemies[i].active = false;
            playerHealth--;

            if (playerHealth <= 0) {
                gameRunning = false;
                gameOver = true;
            }
        }
    }
}

void drawMainMenu() {
    DrawText("Press ENTER to Start", GetScreenWidth() / 2 - MeasureText("Press ENTER to Start", 30) / 2, GetScreenHeight() / 2 - 50, 30, WHITE);
}

void drawGameOverMenu() {
    DrawText("Game Over", GetScreenWidth() / 2 - MeasureText("Game Over", 40) / 2, GetScreenHeight() / 2 - 100, 40, RED);
    DrawText(TextFormat("Score: %d", score), GetScreenWidth() / 2 - MeasureText(TextFormat("Score: %d", score), 20) / 2, GetScreenHeight() / 2 - 40, 20, WHITE);
    DrawText("Press ENTER to Retry", GetScreenWidth() / 2 - MeasureText("Press ENTER to Retry", 20) / 2, GetScreenHeight() / 2, 20, WHITE);
}

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "one bullet");

    SetTargetFPS(60);

    prison = LoadTexture("map.png");
    playertexture = LoadTexture("player.png");
    enemyTexture = LoadTexture("enemy.png");

    while (!WindowShouldClose()) {
        if (gameRunning) {
            player();
            bullet();
            spawnEnemy();
            updateEnemies();
            checkCollisions();
        } else {
            if (IsKeyPressed(KEY_ENTER)) {
                if (gameOver) {
                    initGame();
                } else {
                    gameRunning = true;
                }
            }
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        map();
        DrawTexture(prison, 0, 0, WHITE);

        if (gameRunning) {
            drawtheplayer();
            drawEnemies();
            DrawCircle(bulletX, bulletY, 12, DARKGRAY);
            DrawText(TextFormat("Score: %d", score), 10, 30, 20, WHITE);
            DrawText(TextFormat("Health: %d", playerHealth), 10, 60, 20, WHITE);
        } else {
            if (gameOver) {
                drawGameOverMenu();
            } else {
                drawMainMenu();
            }
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
