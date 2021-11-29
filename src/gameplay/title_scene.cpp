#include <cmath>

#include <title.h>
#include <camera.h>
#include <mathutils.h>
#include <player.h>
#include <physics.h>
#include <ecs.h>
#include <main.h>
#include <files.h>
#include <input.h>
#include <collision.h>
#include <behaviors.h>
#include <control.h>
#include <text.h>
#include <gameplay.h>

extern "C" {
#include <debug.h>
}

TitleScene::TitleScene() : title_timer_(0)
{
}

extern u32 fillColor;

bool TitleScene::load()
{
    debug_printf("Title load\n");
    fillColor = GPACK_RGBA5551(0, 0, 0, 1) << 16 | GPACK_RGBA5551(0, 0, 0, 1);

    return true;
}

void TitleScene::update()
{
    if (title_timer_ > 120 && g_PlayerInput.buttonsHeld & START_BUTTON)
    {
        start_scene_load(std::make_unique<GameplayScene>());
    }
    title_timer_++;
}

const char loading_text[][50] = {
    R"()",
};

const char title_text[][52] = {
    R"(-------------------------------------------------)",
    R"(|        XXXX   XXX  X     X XXXXX XXXX         |)",
    R"(|        X   X X   X X     X X     X   X        |)",
    R"(|        XXXX  X   X  X   X  XXXXX XXXX         |)",
    R"(|        X     X   X  X X X  X     X  X         |)",
    R"(|        X      XXX    X X   XXXXX X   X        |)",
    R"(|)",
    R"(| XXXX XXXXX XXXX  X   X  XXX   XXX  X     XXXXX|)",
    R"(|X       X   X   X X   X X     X     X     X    |)",
    R"(| XXX    X   XXXX  X   X X  XX X  XX X     XXXXX|)",
    R"(|    X   X   X  X  X   X X   X X   X X     X    |)",
    R"(|XXXX    X   X   X  XXX   XXX   XXX  XXXXX XXXXX|)",
    R"(-------------------------------------------------)",
};

void TitleScene::draw()
{
    set_text_color(0, 128, 0, 255);

    int y = 10;

    for (size_t text_line = 0; text_line < sizeof(title_text) / sizeof(title_text[0]); text_line++)
    {
        print_text(13, y, title_text[text_line]);
        y += 10;
    }

    print_text(13 + 48 * 6, 10 + 6 * 10, "|");

    set_text_color(128, 128, 0, 255);

    print_text_centered(screen_width / 2, y, "A GAME BY");
    y += 10;
    print_text_centered(screen_width / 2, y, "Wiseguy");
    y += 10;
    print_text_centered(screen_width / 2, y, "SausageSage");
    y += 10;
    print_text_centered(screen_width / 2, y, "InTheBeef");

    set_text_color(255, 255, 255, 255);

    if (title_timer_ > 120)
    {
        if (((title_timer_ / 60) & 0x1) == 0)
        {
            print_text_centered(screen_width / 2, screen_height - 40, "PRESS START");
        }
    }

    draw_all_text();
}

void TitleScene::after_gfx()
{

}

void TitleScene::unloading_update()
{

}