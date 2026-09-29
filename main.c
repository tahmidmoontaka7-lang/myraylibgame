#include "raylib.h"

#define MAX_BULLETS 10

typedef enum GameState {
    STATE_MENU,
    STATE_GAMEPLAY,
    STATE_HOW_TO_PLAY,
    STATE_ABOUT_US,
    STATE_GAME_OVER
} GameState;

typedef struct Bullet {
    Vector2 position;
    Vector2 speed;
    bool active;
} Bullet;

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Raylib Game - Pokemon Texture Architecture");

    GameState currentState = STATE_MENU;

    // গেমের কোর ভ্যারিয়েবলসমূহ
    Vector2 playerPosition = { (float)screenWidth/2, (float)screenHeight/2 + 100 };
    float playerSpeed = 5.0f;
    int playerScore = 0;
    int playerLives = 3;

    Rectangle enemy = { 375, 100, 50, 50 };
    float enemySpeed = 3.5f;

    Bullet bullets[MAX_BULLETS] = { 0 };

    // 💡 প্রো-ইঞ্জিনিয়ারিং টিপ: ইমেজ লোডিং আর্কিটেকচার
    // তুই যখন তোর প্রজেক্ট ফোল্ডারে "player.png" এবং "enemy.png" রাখবি, 
    // তখন এই নিচের কমেন্ট করা কোড দুটি আনকমেন্ট করে দিলেই ইমেজ স্ক্রিনে চলে আসবে।
    // Texture2D playerTexture = LoadTexture("player.png");
    // Texture2D enemyTexture = LoadTexture("enemy.png");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // --- আপডেট জোন (Update Logic) ---
        switch (currentState) {
            case STATE_MENU:
                if (IsKeyPressed(KEY_G)) {
                    playerPosition = (Vector2){ (float)screenWidth/2, (float)screenHeight/2 + 100 };
                    playerScore = 0;
                    playerLives = 3;
                    enemy.x = 375;
                    enemy.y = 100;
                    for (int i = 0; i < MAX_BULLETS; i++) bullets[i].active = false;
                    currentState = STATE_GAMEPLAY;
                }
                if (IsKeyPressed(KEY_H)) currentState = STATE_HOW_TO_PLAY;
                if (IsKeyPressed(KEY_A)) currentState = STATE_ABOUT_US;
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
                // প্লেয়ার মুভমেন্ট
                if (IsKeyDown(KEY_RIGHT)) playerPosition.x += playerSpeed;
                if (IsKeyDown(KEY_LEFT))  playerPosition.x -= playerSpeed;
                if (IsKeyDown(KEY_DOWN))  playerPosition.y += playerSpeed;
                if (IsKeyDown(KEY_UP))    playerPosition.y -= playerSpeed;

                // শত্রু এআই মুভমেন্ট (স্পিড একটু বাড়ানো হয়েছে চ্যালেঞ্জের জন্য)
                enemy.x += enemySpeed;
                if (enemy.x <= 0 || enemy.x + enemy.width >= screenWidth) {
                    enemySpeed *= -1.05f; 
                }

                // শুটিং লজিক
                if (IsKeyPressed(KEY_SPACE)) {
                    for (int i = 0; i < MAX_BULLETS; i++) {
                        if (!bullets[i].active) {
                            bullets[i].position = (Vector2){ playerPosition.x + 25, playerPosition.y };
                            bullets[i].speed = (Vector2){ 0, -9.0f }; // বুলেটের গতি আরও ফাস্ট
                            bullets[i].active = true;
                            break;
                        }
                    }
                }

                // বুলেট বনাম শত্রু কলিশন
                for (int i = 0; i < MAX_BULLETS; i++) {
                    if (bullets[i].active) {
                        bullets[i].position.y += bullets[i].speed.y;
                        
                        if (CheckCollisionRecs((Rectangle){ bullets[i].position.x - 5, bullets[i].position.y - 5, 10, 10 }, enemy)) {
                            bullets[i].active = false;
                            playerScore += 10;
                            enemy.x = GetRandomValue(50, screenWidth - 100);
                            enemy.y = GetRandomValue(50, 200); 
                        }
                        if (bullets[i].position.y < 0) bullets[i].active = false;
                    }
                }
                
                // প্লেয়ার বনাম শত্রু ফিজিক্স
                if (CheckCollisionRecs((Rectangle){ playerPosition.x, playerPosition.y, 50, 50 }, enemy)) {
                    playerLives--;
                    enemy.x = GetRandomValue(50, screenWidth - 100);
                    enemy.y = 100;
                    if (playerLives <= 0) currentState = STATE_GAME_OVER;
                }
                break;
        }

        // --- রেন্ডারিং জোন (Drawing) ---
        BeginDrawing();
            ClearBackground(BLACK);

            switch (currentState) {
                case STATE_MENU:
                    DrawText("POKEMON SHOOTER: ADVANCED VISUALS", 100, 150, 32, GOLD);
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
                    DrawText("- Physical contact with the enemy costs 1 LIFE!", 150, 370, 20, RED);
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
                    
                    // 💡 ইমেজের বদলে কাস্টম স্পেসশিপ শেপ রেন্ডারিং (যদি ইমেজ পাথ ফাঁকা থাকে)
                    // DrawTexture(playerTexture, playerPosition.x, playerPosition.y, WHITE); <- ইমেজ থাকলে এটা হবে
                    DrawTriangle((Vector2){ playerPosition.x + 25, playerPosition.y },
                                 (Vector2){ playerPosition.x, playerPosition.y + 50 },
                                 (Vector2){ playerPosition.x + 50, playerPosition.y + 50 }, BLUE); // চারকোনার বদলে ত্রিভুজ স্পেসশিপ!

                    // শত্রু ড্র করা (লাল মেটেরিয়াল বক্স)
                    DrawRectangleRec(enemy, RED);
                    
                    // লেজার বুলেট রেন্ডারিং (বৃত্তের বদলে কাস্টম ক্যাপসুল লেজার লাইন)
                    for (int i = 0; i < MAX_BULLETS; i++) {
                        if (bullets[i].active) {
                            DrawLineV(bullets[i].position, (Vector2){ bullets[i].position.x, bullets[i].position.y - 15 }, RED);
                        }
                    }
                    break;
            }
        EndDrawing();
    }

    // মেমোরি আনলোড করা (ইন্ডাস্ট্রি স্ট্যান্ডার্ড ক্লীনআপ)
    // UnloadTexture(playerTexture);
    // UnloadTexture(enemyTexture);
    
    CloseWindow();
    return 0;
}


