#include "raylib.h"

#define MAX_BULLETS 10

typedef struct Bullet {
    Vector2 position;
    Vector2 speed;
    bool active;
} Bullet;

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Raylib Game - Day 3: Collision Physics");

    // ১. প্লেয়ার সেটিংস
    Vector2 playerPosition = { (float)screenWidth/2, (float)screenHeight/2 };
    float playerSpeed = 5.0f;
    int playerScore = 0;

    // ২. শত্রু (Enemy) সেটিংস
    Rectangle enemy = { 375, 100, 50, 50 }; // স্ক্রিনের ওপরের দিকে ৫০x৫০ সাইজের বক্স
    float enemySpeed = 3.0f;

    // ৩. বুলেটের অ্যারে
    Bullet bullets[MAX_BULLETS] = { 0 };

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // --- আপডেট জোন (Update Logic) ---

        // প্লেয়ার মুভমেন্ট
        if (IsKeyDown(KEY_RIGHT)) playerPosition.x += playerSpeed;
        if (IsKeyDown(KEY_LEFT))  playerPosition.x -= playerSpeed;
        if (IsKeyDown(KEY_DOWN))  playerPosition.y += playerSpeed;
        if (IsKeyDown(KEY_UP))    playerPosition.y -= playerSpeed;

        // শত্রু স্বয়ংক্রিয়ভাবে ডানে-বামে সরবে (AI Movement)
        enemy.x += enemySpeed;
        if (enemy.x <= 0 || enemy.x + enemy.width >= screenWidth) {
            enemySpeed *= -1.0f; // স্ক্রিনের কোনায় ধাক্কা খেলে উল্টো দিকে যাবে
        }

        // স্পেসবার চাপলে বুলেট ফায়ার
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

        // বুলেটের পজিশন ও কলিশন চেক
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].active) {
                bullets[i].position.y += bullets[i].speed.y;
                
                // ফিজিক্স হ্যাক: বুলেট আর শত্রুর ধাক্কা লাগা চেক (Collision Detection)
                // বুলেটের চারকোনা বাউন্ডিং বক্স বানিয়ে শত্রুর বক্সের সাথে চেক করা হচ্ছে
                if (CheckCollisionRecs((Rectangle){ bullets[i].position.x - 5, bullets[i].position.y - 5, 10, 10 }, enemy)) {
                    bullets[i].active = false; // বুলেট গায়েব
                    playerScore += 10;         // স্কোর ১০ বাড়বে
                    enemy.x = GetRandomValue(50, screenWidth - 100); // শত্রু নতুন র্যান্ডম জায়গায় স্পন হবে
                }

                // স্ক্রিনের বাইরে গেলে ডিঅ্যাক্টিভ
                if (bullets[i].position.y < 0) {
                    bullets[i].active = false;
                }
            }
        }

        // প্লেয়ার নিজে শত্রুর সাথে ধাক্কা খেলো কি না চেক
        Rectangle playerRec = { playerPosition.x, playerPosition.y, 50, 50 };
        if (CheckCollisionRecs(playerRec, enemy)) {
            playerScore = 0; // প্লেয়ার নিজে ধাক্কা খেলে স্কোর পেনাল্টি (০ হয়ে যাবে)
        }

        // --- রেন্ডারিং জোন (Drawing) ---
        BeginDrawing();
            ClearBackground(DARKGRAY);

            // স্কোরবোর্ড ড্র করা
            DrawText(TextFormat("SCORE: %04d", playerScore), 10, 10, 20, GREEN);
            DrawText("Arrow Keys to Move | SPACE to Shoot | Hit the Red Box!", 10, 40, 20, LIGHTGRAY);
            
            // প্লেয়ার ড্র (নীল বক্স)
            DrawRectangleRec(playerRec, BLUE);

            // শত্রু ড্র (লাল বক্স)
            DrawRectangleRec(enemy, RED);

            // বুলেট ড্র (লাল বৃত্ত)
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (bullets[i].active) {
                    DrawCircleV(bullets[i].position, 5, RED);
                }
            }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
