#include <raylib.h>
#include "../raygui.h"

#define RAYGUI_IMPLEMENTATION

int main()
{
        const int screen_width  = 800;
        const int screen_heigth = 450;

        InitWindow(screen_width, screen_heigth, "patcher");

        SetTargetFPS(60);           

        int i = 0;
        while (!WindowShouldClose())
        {
                BeginDrawing();

                ClearBackground(RAYWHITE);

                if (GuiButton((Rectangle){ 25, 255, 125, 30 }, GuiIconText(RAYGUI_ICON_FILE_SAVE, "Save File"))) {
                        i = 0;       
                }
                
                DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

                EndDrawing();
        }


        CloseWindow();
        return 0;
}
