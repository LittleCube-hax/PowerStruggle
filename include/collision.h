#ifndef __COLLISION_H__
#define __COLLISION_H__

#include <grid.h>
#include <types.h>

#define IS_NOT_LEAF_NODE(bvhNode) ((bvhNode).triCount != 0)
#define IS_LEAF_NODE(bvhNode) ((bvhNode).triCount == 0)

struct AABB {
    Vec3 min;
    Vec3 max;
};

uint32_t testVerticalRayVsAABB(Vec3 rayStart, float lengthInv, AABB *box, float tmin, float tmax);
float verticalRayVsAABB(Vec3 rayStart, float lengthInv, AABB *box, float tmin, float tmax);
uint32_t testRayVsAABB(Vec3 rayStart, Vec3 rayDirInv, AABB *box, float tmin, float tmax);
float rayVsAABB(Vec3 rayStart, Vec3 rayDirInv, AABB *box, float tmin, float tmax);

float raycastVertical(Vec3 rayOrigin, float rayLength, float tmin, float tmax, SurfaceType *floorSurface);
float raycast(Vec3 rayOrigin, Vec3 rayDir, float tmin, float tmax, SurfaceType *floorSurface);

void resolve_circle_collision(Vec3 pos, Vec3 vel, Vec3 hit_pos, float hit_dist, float radius);
int circle_aabb_intersect(float x, float z, float min_x, float max_x, float min_z, float max_z, float rad_sq, float* dist_out, Vec3 hit_out);
void handleWalls(Grid* grid, ColliderParams *collider, Vec3 pos, Vec3 vel);
SurfaceType handleFloorOnGround(Grid* grid, ColliderParams *collider, Vec3 pos, Vec3 vel, float stepUp, float stepDown);
SurfaceType handleFloorInAir(Grid* grid, ColliderParams *collider, Vec3 pos, Vec3 vel);

struct HitboxHit;
struct ColliderHit;

struct ColliderParams {
    float radius; // Radius of the collision cylinder
    float height; // Height of the collision cylinder
    float friction_damping; // The fraction of velocity maintained while on the ground each physics frame (e.g. if it's 0 then the object will instantly stop)
    float floor_height; // The height of the floor that the collider is on
    ColliderHit* hits; // The list of hitboxes this collider is intersecting with
    uint16_t mask; // The mask applied to hitboxes to restrict intersections
    uint8_t hit_wall; // Whether or not this collider hit a wall this frame
    SurfaceType floor_surface_type; // The surface type of the floor
    int16_t floor_tile_x, floor_tile_z;
};

struct Hitbox
{
    uint16_t size_z; // z length if rectangle hitbox, 0 if cylinder hitbox
    uint16_t radius; // x length if rectangle hitbox, radius if cylinder hitbox
    uint16_t size_y;
    uint16_t mask;
    HitboxHit* hits;
};

struct HitboxHit
{
    HitboxHit* next;
    Entity* hit;
};

struct ColliderHit
{
    ColliderHit* next;
    Entity* entity;
    Hitbox* hitbox;
    Vec3* pos;
    Vec3s* rot;
};

constexpr uint16_t player_hitbox_mask    = 0x0001;
constexpr uint16_t enemy_hitbox_mask     = 0x0002;
constexpr uint16_t interact_hitbox_mask  = 0x0004;
constexpr uint16_t load_hitbox_mask      = 0x0008;
constexpr uint16_t collision_hitbox_mask = 0x0010;

void find_collisions(Grid& grid);

#endif
