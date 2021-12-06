#include <cmath>

#include <gameplay.h>
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
#include <audio.h>

extern "C" {
#include <debug.h>
}

extern GridDefinition get_grid_definition(const char *file);

GameplayScene::GameplayScene(int level_index) : grid_{}, level_index_{level_index}, unload_timer_{0}, keys_{0}
{
}

GameplayScene::~GameplayScene()
{
    deleteAllEntities();
    stopMusic();
}

// LoadHandle handle;

// Entity* rect_hitbox;

std::array levels {
    "levels/1",
    "levels/2",
    "levels/3"
};

bool GameplayScene::load()
{
    Vec3 pos;
    switch (level_index_)
    {
        case 0:
            pos[0] = 2229.0f;
            pos[1] = 10.0f;
            pos[2] = 26620.0f;
            break;
        case 1:
            pos[0] = (-1 - -4) * tile_size + tile_size / 2;
            pos[1] = 10.0f;
            pos[2] = (2 - 0) * tile_size + tile_size / 2;
            break;
        case 2:
            pos[0] = 2229.0f;
            pos[1] = 512.0f;
            pos[2] = 26620.0f;
            break;
    }

    // Create the player entity
    createPlayer(pos);

    debug_printf("Loading tiles\n");

    dynamic_array<TileType> tiles(32);

    int i = 0;

    tiles[i++] = TileType{load_model("models/Floor"),         TileCollision::floor};
    tiles[i++] = TileType{load_model("models/FloorBlue"),     TileCollision::floor};
    tiles[i++] = TileType{load_model("models/FloorBrown"),    TileCollision::floor};
    tiles[i++] = TileType{load_model("models/FloorGray"),     TileCollision::floor};
    tiles[i++] = TileType{load_model("models/FloorGreen"),    TileCollision::floor};
    tiles[i++] = TileType{load_model("models/FloorRed"),      TileCollision::floor};
    tiles[i++] = TileType{load_model("models/FloorWhite"),    TileCollision::floor};
    tiles[i++] = TileType{load_model("models/FloorYellow"),   TileCollision::floor};
    tiles[i++] = TileType{load_model("models/Slope"),         TileCollision::slope};
    tiles[i++] = TileType{load_model("models/SlopeBlue"),     TileCollision::slope};
    tiles[i++] = TileType{load_model("models/SlopeBrown"),    TileCollision::slope};
    tiles[i++] = TileType{load_model("models/SlopeGray"),     TileCollision::slope};
    tiles[i++] = TileType{load_model("models/SlopeGreen"),    TileCollision::slope};
    tiles[i++] = TileType{load_model("models/SlopeRed"),      TileCollision::slope};
    tiles[i++] = TileType{load_model("models/SlopeWhite"),    TileCollision::slope};
    tiles[i++] = TileType{load_model("models/SlopeYellow"),   TileCollision::slope};
    tiles[i++] = TileType{load_model("models/Wall"),          TileCollision::wall};
    tiles[i++] = TileType{load_model("models/WallBlue"),      TileCollision::wall};
    tiles[i++] = TileType{load_model("models/WallBrown"),     TileCollision::wall};
    tiles[i++] = TileType{load_model("models/WallGray"),      TileCollision::wall};
    tiles[i++] = TileType{load_model("models/WallGreen"),     TileCollision::wall};
    tiles[i++] = TileType{load_model("models/WallRed"),       TileCollision::wall};
    tiles[i++] = TileType{load_model("models/WallWhite"),     TileCollision::wall};
    tiles[i++] = TileType{load_model("models/WallYellow"),    TileCollision::wall};
    tiles[i++] = TileType{load_model("models/Corner"),        TileCollision::wall};
    tiles[i++] = TileType{load_model("models/CornerBlue"),    TileCollision::wall};
    tiles[i++] = TileType{load_model("models/CornerBrown"),   TileCollision::wall};
    tiles[i++] = TileType{load_model("models/CornerGray"),    TileCollision::wall};
    tiles[i++] = TileType{load_model("models/CornerGreen"),   TileCollision::wall};
    tiles[i++] = TileType{load_model("models/CornerRed"),     TileCollision::wall};
    tiles[i++] = TileType{load_model("models/CornerWhite"),   TileCollision::wall};
    tiles[i++] = TileType{load_model("models/CornerYellow"),  TileCollision::wall};

    debug_printf("Getting grid definition\n");

    GridDefinition def = get_grid_definition(levels[level_index_]);
    grid_ = Grid{def, std::move(tiles)};

    debug_printf("Finished GameplayScene::load\n");

    grid_.load_objects();

    playMusic(0);

    return true;
}

void GameplayScene::update()
{
    grid_.unload_nonvisible_chunks(g_Camera);
    grid_.load_visible_chunks(g_Camera);
    grid_.process_loading_chunks();
    // if ((g_PlayerInput.buttonsHeld & R_TRIG) || (g_PlayerInput.buttonsPressed & L_TRIG))
    {
        // Increment the physics state
        // debug_printf("before physics tick\n");
        physicsTick(grid_);
        find_collisions(grid_);
        control_update();
        // Process all entities that have a behavior
        // debug_printf("before behaviors\n");
        iterateBehaviorEntities();
        tickDestroyTimers();
    }
}

void GameplayScene::draw(bool unloading)
{
    Vec3 lightDir = { 100.0f * sinf((M_PIf / 180.0f) * 30.0f), 100.0f * cosf((M_PIf / 180.0f) * 30.0f), 0.0f};
    // Set up the camera
    // debug_printf("before camera\n");
    setupCameraMatrices(&g_Camera);
    // debug_printf("before light dir\n");
    setLightDirection(lightDir);
    set_text_color(255, 255, 255, 255);

    if (g_gameTimer > 30)
    {
        // debug_printf("before drawing\n");
        draw_enemy_heads();
        drawAllEntities();
        grid_.draw(&g_Camera);

        // drawAllEntitiesHealth();
        set_text_color(0, 128, 0, 255);
        print_text(10, screen_height - 8 - 10 - border_height, get_player_controlled_definition()->base.enemy_name);
        if (keys_ > 0)
        {
            char key_text[17];
            sprintf(key_text, "Keys: %d", keys_);
            print_text(10, screen_height - 8 - 20 - border_height, key_text);
        }
        draw_all_text();
        // drawAllHitboxes();
    }

    if (unloading)
    {
        shadeScreen((float)unload_timer_ / level_unload_time);
    }
}

void GameplayScene::after_gfx()
{
    // debug_printf("After end frame\n");
}

void GameplayScene::unloading_update()
{
    unload_timer_++;
}

extern std::unique_ptr<Scene> cur_scene;
extern std::unique_ptr<Scene> loading_scene;

int get_current_level()
{
    if (cur_scene)
    {
        return cur_scene->get_level_index();
    }
    return -1;
}

void collect_key()
{
    if (cur_scene)
    {
        cur_scene->collect_key();
    }
}

int num_keys()
{
    if (cur_scene)
    {
        return cur_scene->num_keys();
    }
    return 0;
}

void use_key()
{
    if (cur_scene)
    {
        cur_scene->use_key();
    }
}

Grid* get_grid()
{
    if (cur_scene)
    {
        return cur_scene->get_grid();
    }
    return nullptr;
}

LevelTransitionScene::LevelTransitionScene(int level_index) : level_index_{level_index}, load_timer_{0}
{
}
// Called every frame after the scene is constructed, stops being called once it returns true
bool LevelTransitionScene::load()
{
    load_timer_++;
    if (load_timer_ >= level_unload_time)
    {
        load_timer_ = 0;
        return true;
    }
    return false;
}
// Called every frame while the scene is active at a fixed 60Hz rate for logic handling
void LevelTransitionScene::update()
{
    load_timer_++;
    if (load_timer_ >= level_transition_time)
    {
        start_scene_load(std::make_unique<GameplayScene>(level_index_));
    }
}
// Called every frame while the scene is active every frame for drawing the scene contents
void LevelTransitionScene::draw(UNUSED bool unloading)
{

}
// Called every frame while the scene is active after graphics processing is complete
void LevelTransitionScene::after_gfx()
{

}
// Called every frame while the scene is being unloaded
void LevelTransitionScene::unloading_update()
{

}