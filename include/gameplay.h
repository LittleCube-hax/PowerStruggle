#ifndef __GAMEPLAY_H__
#define __GAMEPLAY_H__

#include <memory>

#include <scene.h>
#include <grid.h>
#include <player.h>

constexpr int level_unload_time = 30;
constexpr int level_transition_time = 90;

int get_current_level();
void collect_key();
int num_keys();
void use_key();
Grid* get_grid();

class GameplayScene : public Scene {
public:
    GameplayScene(int level_index);
    ~GameplayScene();
    GameplayScene(const GameplayScene&) = delete;
    GameplayScene(GameplayScene&&) = delete;
    GameplayScene& operator=(const GameplayScene&) = delete;
    GameplayScene& operator=(GameplayScene&&) = delete;
    // Called every frame after the scene is constructed, stops being called once it returns true
    bool load() override final;
    // Called every frame while the scene is active at a fixed 60Hz rate for logic handling
    void update() override final;
    // Called every frame while the scene is active every frame for drawing the scene contents
    void draw(bool unloading) override final;
    // Called every frame while the scene is active after graphics processing is complete
    void after_gfx() override final;
    // Called every frame while the scene is being unloaded
    void unloading_update() override final;
    // Gets the current level's index
    int get_level_index() override final { return level_index_; }
    void collect_key() override final { keys_++; }
    int num_keys() override final { return keys_; }
    void use_key() override final { keys_--; }
    Grid* get_grid() override final { return &grid_; }
private:
    Grid grid_;
    int level_index_;
    int unload_timer_;
    int keys_;
    int timer_;
};

class LevelTransitionScene : public Scene {
public:
    LevelTransitionScene(int level_index);
    // Called every frame after the scene is constructed, stops being called once it returns true
    bool load() override final;
    // Called every frame while the scene is active at a fixed 60Hz rate for logic handling
    void update() override final;
    // Called every frame while the scene is active every frame for drawing the scene contents
    void draw(bool unloading) override final;
    // Called every frame while the scene is active after graphics processing is complete
    void after_gfx() override final;
    // Called every frame while the scene is being unloaded
    void unloading_update() override final;
private:
    int level_index_;
    int load_timer_;
};

#endif
