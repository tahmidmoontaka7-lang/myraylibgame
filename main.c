#include "raylib.h"
#include <stdio.h>
#include <stdbool.h>

#define MAX_BULLETS 10
#define BOSS_MAX_HEALTH 10

typedef enum GameState {
    STATE_MENU,
    STATE_DIFFICULTY_SELECT,
    STATE_GAMEPLAY,
    STATE_HOW_TO_PLAY,
    STATE_ABOUT_US,
    STATE_GAME_OVER
} GameState;

typedef enum Difficulty {
    DIFF_EASY,
    DIFF_MEDIUM,
    DIFF_HARD
} Difficulty;

typedef struct Bullet {
    Vector2 position;
    Vector2 speed;
    bool active;
} Bullet;

int LoadHighScore(void) {
    int highScore = 0;
    FILE *file = fopen("highscore.txt", "r");
    if (file != NULL) {
        fscanf(file, "%d", &highScore);
        fclose(file);
    }
    return highScore;
}

void SaveHighScore(int score) {
    FILE *file = fopen("highscore.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d", score);
        fclose(file);
    }
}

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Pokemon Shooter - Ultimate Boss Edition");

    InitAudioDevice();

    Music bgm = LoadMusicStream("Cyberpunk Moonlight Sonata.mp3");
    bgm.looping = true;
    PlayMusicStream(bgm);

    Sound shootSound = LoadSound("shoot.wav");
    Sound explodeSound = LoadSound("explosion.wav");

    GameState currentState = STATE_MENU;
    Difficulty currentDiff = DIFF_EASY;

    Vector2 playerPosition = { (float)screenWidth/2, (float)screenHeight/2 + 100 };
    float playerSpeed = 5.5f;
    int playerScore = 0;
    int playerLives = 3;
    int highScore = LoadHighScore();

    Rectangle enemy = { 375, 100, 60, 40 };
    float baseEnemySpeed = 3.0f;
    float currentEnemySpeed = 3.0f;

    bool isBossActive = false;
    Rectangle boss = { 300, 60, 200, 60 };
    float bossSpeed = 4.0f;
    int bossHealth = BOSS_MAX_HEALTH;
    
    Vector2 bossAttackPos = { 0, 0 };
    bool bossAttackActive = false;
    float bossAttackSpeed = 5.0f;

    Bullet bullets[MAX_BULLETS] = { 0 };

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        UpdateMusicStream(bgm);

        switch (currentState) {
            case STATE_MENU:
                if (IsKeyPressed(KEY_G)) currentState = STATE_DIFFICULTY_SELECT;
                if (IsKeyPressed(KEY_H)) currentState = STATE_HOW_TO_PLAY;
                if (IsKeyPressed(KEY_A)) currentState = STATE_ABOUT_US;
                break;

            case STATE_DIFFICULTY_SELECT:
                if (IsKeyPressed(KEY_ONE)) { currentDiff = DIFF_EASY; baseEnemySpeed = 3.5f; playerLives = 5; currentState = STATE_GAMEPLAY; }
                if (IsKeyPressed(KEY_TWO)) { currentDiff = DIFF_MEDIUM; baseEnemySpeed = 6.0f; playerLives = 3; currentState = STATE_GAMEPLAY; }
                if (IsKeyPressed(KEY_THREE)) { currentDiff = DIFF_HARD; baseEnemySpeed = 9.5f; playerLives = 1; currentState = STATE_GAMEPLAY; }
                currentEnemySpeed = baseEnemySpeed;
                
                playerPosition = (Vector2){ (float)screenWidth/2, (float)screenHeight/2 + 100 };
                playerScore = 0;
                playerLives = (currentDiff == DIFF_EASY) ? 5 : ((currentDiff == DIFF_MEDIUM) ? 3 : 1);
                enemy.x = 375; enemy.y = 100;
                isBossActive = false;
                bossHealth = BOSS_MAX_HEALTH;
                bossAttackActive = false;
                highScore = LoadHighScore();
                for (int i = 0; i < MAX_BULLETS; i++) bullets[i].active = false;
                break;

            case STATE_HOW_TO_PLAY:
                if (IsKeyPressed(KEY_B)) currentState = STATE_MENU;
                break;

            case STATE_ABOUT_US:
                if (IsKeyPressed(KEY_B)) currentState = STATE_MENU;
                break;

            case STATE_GAME_OVER:
                if (IsKeyPressed(KEY_R)) currentState = STATE_MENU;
                break;

            case STATE_GAMEPLAY:
                if (IsKeyDown(KEY_RIGHT)) playerPosition.x += playerSpeed;
                if (IsKeyDown(KEY_LEFT))  playerPosition.x -= playerSpeed;
                if (IsKeyDown(KEY_DOWN))  playerPosition.y += playerSpeed;
                if (IsKeyDown(KEY_UP))    playerPosition.y -= playerSpeed;

                if ((playerScore >= 100 && playerScore < 150) || (playerScore >= 200)) {
                    isBossActive = true;
                } else {
                    isBossActive = false;
                }

                if (!isBossActive) {
                    enemy.x += currentEnemySpeed;
                    if (enemy.x <= 0 || enemy.x + enemy.width >= screenWidth) currentEnemySpeed *= -1.0f;
                } else {
                    boss.x += bossSpeed;
                    if (boss.x <= 0 || boss.x + boss.width >= screenWidth) bossSpeed *= -1.0f;

                    if (!bossAttackActive && GetRandomValue(1, 100) < 4) {
                        bossAttackPos = (Vector2){ boss.x + boss.width/2, boss.y + boss.height };
                        bossAttackActive = true;
                    }

                    if (bossAttackActive) {
                        bossAttackPos.y += bossAttackSpeed;
                        if (CheckCollisionRecs((Rectangle){ playerPosition.x, playerPosition.y, 50, 50 }, (Rectangle){ bossAttackPos.x - 8, bossAttackPos.y, 16, 16 })) {
                            bossAttackActive = false;
                            playerLives--;
                            PlaySound(explodeSound);
                            if (playerLives <= 0) {
                                if (playerScore > highScore) SaveHighScore(playerScore);
                                currentState = STATE_GAME_OVER;
                            }
                        }
                        if (bossAttackPos.y > screenHeight) bossAttackActive = false;
                    }
                }

                if (IsKeyPressed(KEY_SPACE)) {
                    for (int i = 0; i < MAX_BULLETS; i++) {
                        if (!bullets[i].active) {
                            bullets[i].position = (Vector2){ playerPosition.x + 25, playerPosition.y };
                            bullets[i].speed = (Vector2){ 0, -10.0f };
                            bullets[i].active = true;
                            PlaySound(shootSound);
                            break;
                        }
                    }
                }

                for (int i = 0; i < MAX_BULLETS; i++) {
                    if (bullets[i].active) {
                        bullets[i].position.y += bullets[i].speed.y;
                        
                        if (!isBossActive) {
                            if (CheckCollisionRecs((Rectangle){ bullets[i].position.x - 4, bullets[i].position.y - 15, 8, 15 }, enemy)) {
                                bullets[i].active = false;
                                playerScore += (currentDiff == DIFF_HARD) ? 30 : (currentDiff == DIFF_MEDIUM ? 20 : 10);
                                enemy.x = GetRandomValue(50, screenWidth - 100);
                                PlaySound(explodeSound);
                            }
                        } else {
                            if (CheckCollisionRecs((Rectangle){ bullets[i].position.x - 4, bullets[i].position.y - 15, 8, 15 }, boss)) {
                                bullets[i].active = false;
                                bossHealth--;
                                PlaySound(explodeSound);
                                
                                if (bossHealth <= 0) {
                                    playerScore += 100;
                                    bossHealth = BOSS_MAX_HEALTH;
                                    isBossActive = false;
                                    playerScore += 10;
                                }
                            }
                        }
                        if (bullets[i].position.y < 0) bullets[i].active = false;
                    }
                }
                
                if (!isBossActive) {
                    if (CheckCollisionRecs((Rectangle){ playerPosition.x, playerPosition.y, 50, 50 }, enemy)) {
                        playerLives--;
                        enemy.x = GetRandomValue(50, screenWidth - 100);
                        PlaySound(explodeSound);
                        if (playerLives <= 0) {
                            if (playerScore > highScore) SaveHighScore(playerScore);
                            currentState = STATE_GAME_OVER;
                        }
                    }
                } else {
                    if (CheckCollisionRecs((Rectangle){ playerPosition.x, playerPosition.y, 50, 50 }, boss)) {
                        playerLives = 0;
                        PlaySound(explodeSound);
                        if (playerScore > highScore) SaveHighScore(playerScore);
                        currentState = STATE_GAME_OVER;
                    }
                }
                break;
        }

        BeginDrawing();
            ClearBackground(BLACK);

            switch (currentState) {
                case STATE_MENU:
                    DrawText("POKEMON SHOOTER: ULTIMATE BOSS", 90, 130, 32, GOLD);
                    DrawText(TextFormat("LIFETIME HIGH SCORE: %04d", highScore), 240, 210, 22, RED);
                    DrawText("Press [G] to Select Difficulty", 260, 290, 22, GREEN);
                    DrawText("Press [H] to How to Play", 260, 340, 22, LIGHTGRAY);
                    DrawText("Press [A] to About Us (Developer Profile)", 260, 390, 22, LIGHTGRAY);
                    DrawText("Dedicated to the Light Queen", 285, 460, 18, MAGENTA);
                    break;

                case STATE_DIFFICULTY_SELECT:
                    DrawText("SELECT DIFFICULTY LEVEL", 220, 150, 30, GOLD);
                    DrawText("Press [1] for EASY   (5 Lives)", 200, 260, 20, GREEN);
                    DrawText("Press [2] for MEDIUM (3 Lives)", 200, 320, 20, ORANGE);
DrawText("Press [3] for HARD   (1 Life)", 200, 380, 20, RED);
break;

                    case STATE_HOW_TO_PLAY:
DrawText("HOW TO PLAY", 320, 100, 30, GOLD);
DrawText("- Arrow Keys to move. SPACEBAR to fire purple laser.", 120, 220, 20, WHITE);
DrawText("- Score 100+ to summon the DREADED BOSS SHIP!", 120, 270, 20, GOLD);
DrawText("- Boss takes 10 HITS and fires back green plasma tracks!", 120, 320, 20, RED);
DrawText("Press [B] to Go Back to Main Menu", 230, 480, 20, GREEN);
break;
case STATE_ABOUT_US:
DrawText("ABOUT THE DEVELOPER", 240, 80, 30, GOLD);
DrawText("Lead Engineer: Tahmid Moontaka", 120, 180, 22, GREEN);
DrawText("Role: 2nd-Year Computer Science & Systems Trainee", 120, 220, 18, LIGHTGRAY);
DrawText("Core Tech: Python, C, C++, C#, Git & GitHub Ecosystem", 120, 260, 18, LIGHTGRAY);
DrawText("Press [B] to Go Back to Main Menu", 230, 490, 20, GREEN);
break;
case STATE_GAME_OVER:
DrawText("GAME OVER", 310, 180, 40, RED);
DrawText(TextFormat("YOUR FINAL SCORE: %04d", playerScore), 260, 260, 24, WHITE);
if (playerScore >= highScore) DrawText("NEW HIGH SCORE RECORDED!", 240, 310, 20, GREEN);
DrawText("Press [R] to Return to Main Menu", 230, 400, 20, GREEN);
break;
case STATE_GAMEPLAY:
DrawText(TextFormat("SCORE: %04d", playerScore), 10, 10, 20, GREEN);
DrawText(TextFormat("LIVES: %d", playerLives), screenWidth - 120, 10, 20, RED);
DrawText((currentDiff == DIFF_HARD) ? "HARD MODE" : (currentDiff == DIFF_MEDIUM ? "MEDIUM MODE" : "EASY MODE"), 350, 10, 18, ORANGE);
DrawTriangle((Vector2){ playerPosition.x + 25, playerPosition.y },
(Vector2){ playerPosition.x - 5, playerPosition.y + 45 },
(Vector2){ playerPosition.x + 55, playerPosition.y + 45 }, SKYBLUE);
DrawRectangle(playerPosition.x + 10, playerPosition.y + 45, 30, 8, BLUE);
if (!isBossActive) {
DrawRectangleRec(enemy, RED);
DrawRectangle(enemy.x + 10, enemy.y + 10, 40, 20, MAROON);
} else {
DrawRectangleRec(boss, MAROON);
DrawRectangle(boss.x + 20, boss.y + 15, 160, 30, RED);
DrawRectangle(boss.x + 80, boss.y + 45, 40, 15, GOLD);
DrawText("BOSS HEALTH", 240, 12, 16, GOLD);
DrawRectangle(350, 10, 200, 20, DARKGRAY);
DrawRectangle(350, 10, (int)(((float)bossHealth / BOSS_MAX_HEALTH) * 200), 20, GREEN);
DrawRectangleLines(350, 10, 200, 20, WHITE);
if (bossAttackActive) {
DrawCircle(bossAttackPos.x, bossAttackPos.y, 8, LIME);
}
}
for (int i = 0; i < MAX_BULLETS; i++) {
if (bullets[i].active) {
DrawLineEx(bullets[i].position, (Vector2){ bullets[i].position.x, bullets[i].position.y - 18 }, 3.0f, PURPLE);
}
}
break;
}
EndDrawing();
}
UnloadSound(shootSound);
UnloadSound(explodeSound);
UnloadMusicStream(bgm);
CloseAudioDevice();
CloseWindow();
return 0;
}
