#include "raylib.h"

#define MAX_BULLETS 10

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

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Raylib Game - Cyberpunk Edition");

    // ১. অ디오 ইঞ্জিন চালু করা
    InitAudioDevice();

    // 💡 ব্যাকগ্রাউন্ড মিউজিক এবং সাউন্ড ইফেক্টস লোড করা
    Music bgm = LoadMusicStream("Cyberpunk Moonlight Sonata.mp3");
    bgm.looping = true;
    PlayMusicStream(bgm);

    Sound shootSound = LoadSound("shoot.wav");
    Sound explodeSound = LoadSound("explosion.wav");

    GameState currentState = STATE_MENU;
    Difficulty currentDiff = DIFF_EASY;

    Vector2 playerPosition = { (float)screenWidth/2, (float)screenHeight/2 + 100 };
    float playerSpeed = 5.0f;
    int playerScore = 0;
    int playerLives = 3;

    Rectangle enemy = { 375, 100, 50, 50 };
    float baseEnemySpeed = 3.0f;
    float currentEnemySpeed = 3.0f;

    Bullet bullets[MAX_BULLETS] = { 0 };

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // 💡 প্রতি ফ্রেমে ব্যাকগ্রাউন্ড মিউজিক বাফার আপডেট করা (স্মুথ প্লেব্যাকের জন্য)
        UpdateMusicStream(bgm);

        // --- আপডেট জোন (Update Logic) ---
        switch (currentState) {
            case STATE_MENU:
                if (IsKeyPressed(KEY_G)) currentState = STATE_DIFFICULTY_SELECT;
                if (IsKeyPressed(KEY_H)) currentState = STATE_HOW_TO_PLAY;
                if (IsKeyPressed(KEY_A)) currentState = STATE_ABOUT_US;
                break;

            case STATE_DIFFICULTY_SELECT:
                if (IsKeyPressed(KEY_ONE)) {
                    currentDiff = DIFF_EASY;
                    baseEnemySpeed = 3.0f;
                    playerLives = 5;
                    currentState = STATE_GAMEPLAY;
                }
                if (IsKeyPressed(KEY_TWO)) {
                    currentDiff = DIFF_MEDIUM;
                    baseEnemySpeed = 5.5f;
                    playerLives = 3;
                    currentState = STATE_GAMEPLAY;
                }
                if (IsKeyPressed(KEY_THREE)) {
                    currentDiff = DIFF_HARD;
                    baseEnemySpeed = 9.0f;
                    playerLives = 1;
                    currentState = STATE_GAMEPLAY;
                }
                currentEnemySpeed = baseEnemySpeed;
                
                playerPosition = (Vector2){ (float)screenWidth/2, (float)screenHeight/2 + 100 };
                playerScore = 0;
                enemy.x = 375;
                enemy.y = 100;
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

                enemy.x += currentEnemySpeed;
                if (enemy.x <= 0 || enemy.x + enemy.width >= screenWidth) {
                    currentEnemySpeed *= -1.0f; 
                }

                if (IsKeyPressed(KEY_SPACE)) {
                    for (int i = 0; i < MAX_BULLETS; i++) {
                        if (!bullets[i].active) {
                            bullets[i].position = (Vector2){ playerPosition.x + 25, playerPosition.y };
                            bullets[i].speed = (Vector2){ 0, -9.5f };
                            bullets[i].active = true;
                            
                            // 💡 লেজার শুট সাউন্ড প্লে করা
                            PlaySound(shootSound);
                            break;
                        }
                    }
                }

                for (int i = 0; i < MAX_BULLETS; i++) {
                    if (bullets[i].active) {
                        bullets[i].position.y += bullets[i].speed.y;
                        
                        if (CheckCollisionRecs((Rectangle){ bullets[i].position.x - 5, bullets[i].position.y - 5, 10, 10 }, enemy)) {
                            bullets[i].active = false;
                            playerScore += (currentDiff == DIFF_HARD) ? 30 : (currentDiff == DIFF_MEDIUM ? 20 : 10);
                            enemy.x = GetRandomValue(50, screenWidth - 100);
                            enemy.y = GetRandomValue(50, 200); 
                            
                            // 💡 শত্রু ধ্বংস হওয়ার ব্লাস্ট সাউন্ড প্লে করা
                            PlaySound(explodeSound);
                        }
                        if (bullets[i].position.y < 0) bullets[i].active = false;
                    }
                }
                
                if (CheckCollisionRecs((Rectangle){ playerPosition.x, playerPosition.y, 50, 50 }, enemy)) {
                    playerLives--;
                    enemy.x = GetRandomValue(50, screenWidth - 100);
                    enemy.y = 100;
                    
                    // 💡 প্লেয়ার আঘাত পেলে সতর্কতামূলক সাউন্ড ইফেক্ট হিসেবে এক্সপ্লোশন ট্রিগার
                    PlaySound(explodeSound);
                    
                    if (playerLives <= 0) currentState = STATE_GAME_OVER;
                }
                break;
        }

        // --- রেন্ডারিং জোন (Drawing) ---
        BeginDrawing();
            ClearBackground(BLACK);

            switch (currentState) {
                case STATE_MENU:
                    DrawText("POKEMON SHOOTER: AUDIO MASTER", 100, 150, 32, GOLD);
                    DrawText("Press [G] to Select Difficulty", 260, 280, 22, GREEN);
                    DrawText("Press [H] to How to Play", 260, 330, 22, LIGHTGRAY);
                    DrawText("Press [A] to About Us (Developer Profile)", 260, 380, 22, LIGHTGRAY);
                    DrawText("Built with Raylib Engine & C", 290, 530, 16, DARKGRAY);
                    break;

                case STATE_DIFFICULTY_SELECT:
                    DrawText("SELECT DIFFICULTY LEVEL", 220, 150, 30, GOLD);
                    DrawText("Press [1] for EASY   (5 Lives, Slow Enemy)", 200, 260, 20, GREEN);
                    DrawText("Press [2] for MEDIUM (3 Lives, Normal Speed)", 200, 320, 20, ORANGE);
                    DrawText("Press [3] for HARD   (1 Life, Insane Speed)", 200, 380, 20, RED);
                    break;

                case STATE_HOW_TO_PLAY:
                    DrawText("HOW TO PLAY", 320, 100, 30, GOLD);
                    DrawText("- Use ARROW KEYS to move your blue ship.", 150, 220, 20, WHITE);
                    DrawText("- Press SPACEBAR to fire plasma laser bullets.", 150, 270, 20, WHITE);
                    DrawText("- Harder difficulty yields up to 3x higher score!", 150, 320, 20, GOLD);
                    DrawText("Press [B] to Go Back to Main Menu", 230, 480, 20, GREEN);
                    break;

                case STATE_ABOUT_US:
                    DrawText("ABOUT THE DEVELOPER", 240, 80, 30, GOLD);
                    DrawText("Lead Engineer: Tahmid Moontaka", 120, 180, 22, GREEN);
                    DrawText("Role: 2nd-Year Computer Science & Systems Trainee", 120, 220, 18, LIGHTGRAY);
                    DrawText("Core Tech: Python, C, C++, C#, Git & GitHub Ecosystem", 120, 260, 18, LIGHTGRAY);
                    DrawText("Specialization: Competitive Programming & AI/ML Systems", 120, 300, 18, LIGHTGRAY);
                    DrawText("Press [B] to Go Back to Main Menu", 230, 490, 20, GREEN);
                    break;

                case STATE_GAME_OVER:
                    DrawText("GAME OVER", 310, 180, 40, RED);
                    DrawText(TextFormat("YOUR FINAL SCORE: %04d", playerScore), 260, 260, 24, WHITE);
                    DrawText("Press [R] to Return to Main Menu", 230, 380, 20, GREEN);
                    break;

                case STATE_GAMEPLAY:
                    DrawText(TextFormat("SCORE: %04d", playerScore), 10, 10, 20, GREEN);
                    DrawText(TextFormat("LIVES: %d", playerLives), screenWidth - 120, 10, 20, RED);
                    DrawText((currentDiff == DIFF_HARD) ? "HARD MODE" : (currentDiff == DIFF_MEDIUM ? "MEDIUM MODE" : "EASY MODE"), 350, 10, 18, ORANGE);
                    
                    DrawTriangle((Vector2){ playerPosition.x + 25, playerPosition.y },
                                 (Vector2){ playerPosition.x, playerPosition.y + 50 },
                                 (Vector2){ playerPosition.x + 50, playerPosition.y + 50 }, BLUE);

                    DrawRectangleRec(enemy, RED);
                    
                    for (int i = 0; i < MAX_BULLETS; i++) {
                        if (bullets[i].active) {
                            DrawLineV(bullets[i].position, (Vector2){ bullets[i].position.x, bullets[i].position.y - 15 }, RED);
                        }
                    }
                    break;
            }
        EndDrawing();
    }

    // 💡 মেমোরি আনলোড ও ক্লিনআপ
    UnloadSound(shootSound);
    UnloadSound(explodeSound);
    UnloadMusicStream(bgm);
    
    CloseAudioDevice(); 
    CloseWindow();
    return 0;
}
