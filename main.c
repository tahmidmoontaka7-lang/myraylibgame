#include "raylib.h"

int main(void) {
    // স্ক্রিন সাইজ ও উইন্ডো ওপেন করা
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Raylib Game - Day 1");

    // প্লেয়ারের শুরুর পজিশন ও সাইজ (Rectangle struct)
    Vector2 playerPosition = { (float)screenWidth/2, (float)screenHeight/2 };
    float playerSpeed = 5.0f;

    SetTargetFPS(60); // গেম লুপ প্রতি সেকেন্ডে ৬০ বার চলবে

    // মেইন গেম লুপ
    while (!WindowShouldClose()) {
        
        // ১. ইনপুট নেওয়া ও প্লেয়ার পজিশন আপডেট (Update Logic)
        if (IsKeyDown(KEY_RIGHT)) playerPosition.x += playerSpeed;
        if (IsKeyDown(KEY_LEFT))  playerPosition.x -= playerSpeed;
        if (IsKeyDown(KEY_DOWN))  playerPosition.y += playerSpeed;
        if (IsKeyDown(KEY_UP))    playerPosition.y -= playerSpeed;

        // ২. স্ক্রিনে অবজেক্ট ড্র করা (Render Logic)
        BeginDrawing();
            ClearBackground(DARKGRAY); // ব্যাকগ্রাউন্ড কালার

            // স্ক্রিনে টেক্সট ও প্লেয়ার ড্র করা
            DrawText("Use Arrow Keys to Move the Blue Square!", 10, 10, 20, MAROON);
            DrawRectangleV(playerPosition, (Vector2){ 50, 50 }, BLUE); // ৫০x৫০ সাইজের নীল বক্স
        EndDrawing();
    }

    // উইন্ডো বন্ধ করা
    CloseWindow();
    return 0;
}
