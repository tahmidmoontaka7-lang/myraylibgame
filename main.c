#include "raylib.h"

// বুলেটের ম্যাক্সিমাম সংখ্যা ডিফাইন করা
#define MAX_BULLETS 10

typedef struct Bullet {
    Vector2 position;
    Vector2 speed;
    bool active;
} Bullet;

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Raylib Game - Day 2: Firing");

    // প্লেয়ার সেটিংস
    Vector2 playerPosition = { (float)screenWidth/2, (float)screenHeight/2 };
    float playerSpeed = 5.0f;

    // বুলেটের অ্যারে ইনিশিয়াল করা
    Bullet bullets[MAX_BULLETS] = { 0 };

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // ১. প্লেয়ার মুভমেন্ট লজিক
        if (IsKeyDown(KEY_RIGHT)) playerPosition.x += playerSpeed;
        if (IsKeyDown(KEY_LEFT))  playerPosition.x -= playerSpeed;
        if (IsKeyDown(KEY_DOWN))  playerPosition.y += playerSpeed;
        if (IsKeyDown(KEY_UP))    playerPosition.y -= playerSpeed;

        // ২. স্পেসবার চাপলে নতুন বুলেট স্লট খুঁজে ফায়ার করার লজিক
        if (IsKeyPressed(KEY_SPACE)) {
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (!bullets[i].active) {
                    bullets[i].position = (Vector2){ playerPosition.x + 25, playerPosition.y }; // প্লেয়ারের মাঝখান থেকে বের হবে
                    bullets[i].speed = (Vector2){ 0, -8.0f }; // ওপরের দিকে যাবে
                    bullets[i].active = true;
                    break; 
                }
            }
        }

        // ৩. বুলেটের পজিশন আপডেট এবং স্ক্রিনের বাইরে গেলে নিষ্ক্রিয় করা
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].active) {
                bullets[i].position.y += bullets[i].speed.y;
                if (bullets[i].position.y < 0) {
                    bullets[i].active = false; 
                }
            }
        }

        // ৪. রেন্ডারিং জোন (Drawing)
        BeginDrawing();
            ClearBackground(DARKGRAY);

            DrawText("Press SPACE to Fire Bullets, Arrow Keys to Move!", 10, 10, 20, LIGHTGRAY);
            
            // প্লেয়ার ড্র করা (নীল বক্স)
            DrawRectangleV(playerPosition, (Vector2){ 50, 50 }, BLUE);

            // অ্যাক্টিভ বুলেটগুলো ড্র করা (ছোট লাল বৃত্ত)
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
