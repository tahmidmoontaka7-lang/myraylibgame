#include "raylib.h"

#define MAX_BULLETS 10

// গেমের বিভিন্ন স্ক্রিন বা স্টেট ডিফাইন করা
typedef enum GameState {
    STATE_MENU,
    STATE_GAMEPLAY,
    STATE_HOW_TO_PLAY,
    STATE_ABOUT_US
} GameState;

typedef struct Bullet {
    Vector2 position;
    Vector2 speed;
    bool active;
} Bullet;

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Raylib Game - Day 4: Professional Menu");

    GameState currentState = STATE_MENU;

    // প্লেয়ার ও শত্রু সেটিংস
    Vector2 playerPosition = { (float)screenWidth/2, (float)screenHeight/2 + 100 };
    float playerSpeed = 5.0f;
    int playerScore = 0;
    Rectangle enemy = { 375, 100, 50, 50 };
    float enemySpeed = 3.0f;

    Bullet bullets[MAX_BULLETS] = { 0 };

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // --- আপডেট জোন (Update Logic) ---
        
        switch (currentState) {
            case STATE_MENU:
                // মেনুর কি-ইনপুট চেকিং
                if (IsKeyPressed(KEY_G)) currentState = STATE_GAMEPLAY;
                if (IsKeyPressed(KEY_H)) currentState = STATE_HOW_TO_PLAY;
                if (IsKeyPressed(KEY_A)) currentState = STATE_ABOUT_US;
                break;

            case STATE_HOW_TO_PLAY:
                if (IsKeyPressed(KEY_B)) currentState = STATE_MENU; // B চাপলে ব্যাক টু মেনু
                break;

            case STATE_ABOUT_US:
                if (IsKeyPressed(KEY_B)) currentState = STATE_MENU;
                break;

            case STATE_GAMEPLAY:
                // প্লেয়ার মুভমেন্ট
                if (IsKeyDown(KEY_RIGHT)) playerPosition.x += playerSpeed;
                if (IsKeyDown(KEY_LEFT))  playerPosition.x -= playerSpeed;
                if (IsKeyDown(KEY_DOWN))  playerPosition.y += playerSpeed;
                if (IsKeyDown(KEY_UP))    playerPosition.y -= playerSpeed;

                // শত্রু মুভমেন্ট
                enemy.x += enemySpeed;
                if (enemy.x <= 0 || enemy.x + enemy.width >= screenWidth) enemySpeed *= -1.0f;

                // শুটিং লজিক
                if (IsKeyPressed(KEY_SPACE)) {
                    for (int i = 0; i < MAX_BULLETS; i++) {
                        if (!bullets[i].active) {
                            bullets[i].position = (Vector2){ playerPosition.x + 25, playerPosition.y };
                            bullets[i].speed = (Vector2){ 0, -8.0f };
                            bullets[i].active = true;
                            break;
                        }
                    }
                }

                // বুলেট আপডেট ও কলিশন
                for (int i = 0; i < MAX_BULLETS; i++) {
                    if (bullets[i].active) {
                        bullets[i].position.y += bullets[i].speed.y;
                        if (CheckCollisionRecs((Rectangle){ bullets[i].position.x - 5, bullets[i].position.y - 5, 10, 10 }, enemy)) {
                            bullets[i].active = false;
                            playerScore += 10;
                            enemy.x = GetRandomValue(50, screenWidth - 100);
                        }
                        if (bullets[i].position.y < 0) bullets[i].active = false;
                    }
                }
                
                // প্লেয়ার বনাম শত্রু কলিশন
                if (CheckCollisionRecs((Rectangle){ playerPosition.x, playerPosition.y, 50, 50 }, enemy)) {
                    playerScore = 0;
                }
                break;
        }

        // --- রেন্ডারিং জোন (Drawing) ---
        BeginDrawing();
            ClearBackground(BLACK);

            switch (currentState) {
                case STATE_MENU:
                    DrawText("POKEMON SHOOTER EXTRAORDINAIRE", 120, 150, 32, GOLD);
                    DrawText("Press [G] to Start Game", 280, 280, 22, GREEN);
                    DrawText("Press [H] to How to Play", 280, 330, 22, LIGHTGRAY);
                    DrawText("Press [A] to About Us (Developer Profile)", 280, 380, 22, LIGHTGRAY);
                    DrawText("Built with Raylib Engine & C", 290, 530, 16, DARKGRAY);
                    break;

                case STATE_HOW_TO_PLAY:
                    DrawText("HOW TO PLAY", 320, 100, 30, GOLD);
                    DrawText("- Use ARROW KEYS to move your blue ship.", 150, 220, 20, WHITE);
                    DrawText("- Press SPACEBAR to fire red plasma bullets.", 150, 270, 20, WHITE);
                    DrawText("- Hit the sliding RED ENEMY box to score points.", 150, 320, 20, WHITE);
                    DrawText("- Avoid physical contact with the enemy, or your score resets!", 150, 370, 20, RED);
                    
                    DrawText("Press [B] to Go Back to Main Menu", 230, 480, 20, GREEN);
                    break;

                case STATE_ABOUT_US:
                    DrawText("ABOUT THE DEVELOPER", 240, 80, 30, GOLD);
                    
                    // প্রফেশনাল ডেভেলপার প্রোফাইল শো-কেস (English Comments Inside Blocks)
                    DrawText("Lead Engineer: Tahmid Moontaka", 120, 180, 22, GREEN);
                    DrawText("Role: 2nd-Year Computer Science & Systems Trainee", 120, 220, 18, LIGHTGRAY);
                    DrawText("Core Tech: Python, C, C++, C#, Git & GitHub Ecosystem", 120, 260, 18, LIGHTGRAY);
                    DrawText("Specialization: Competitive Programming & AI/ML Systems", 120, 300, 18, LIGHTGRAY);
                    
                    DrawText("Vision: Designing scalable city-wide utility frameworks and", 120, 360, 18, WHITE);
                    DrawText("high-performance production architectures.", 120, 390, 18, WHITE);
                    
                    DrawText("Press [B] to Go Back to Main Menu", 230, 490, 20, GREEN);
                    break;

                case STATE_GAMEPLAY:
                    DrawText(TextFormat("SCORE: %04d", playerScore), 10, 10, 20, GREEN);
                    DrawRectangle(playerPosition.x, playerPosition.y, 50, 50, BLUE);
                    DrawRectangleRec(enemy, RED);
                    for (int i = 0; i < MAX_BULLETS; i++) {
                        if (bullets[i].active) DrawCircleV(bullets[i].position, 5, RED);
                    }
                    break;
            }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

