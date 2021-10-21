#ifndef __MODEL_H__
#define __MODEL_H__

#include <platform_gfx.h>
#include <gfx.h>

#include <types.h>

///////////////////////
// Animation defines //
///////////////////////

// Animation flags
#define ANIM_LOOP (1 << 0)
#define ANIM_HAS_TRIGGERS (1 << 1)

// Joint table flags
#define CHANNEL_POS_X (1 << 0)
#define CHANNEL_POS_Y (1 << 1)
#define CHANNEL_POS_Z (1 << 2)
#define CHANNEL_ROT_X (1 << 3)
#define CHANNEL_ROT_Y (1 << 4)
#define CHANNEL_ROT_Z (1 << 5)
#define CHANNEL_SCALE_X (1 << 6)
#define CHANNEL_SCALE_Y (1 << 7)
#define CHANNEL_SCALE_Z (1 << 8)

struct Animation {
    uint16_t frameCount; // The number of frames of data this animation has
    uint8_t jointCount; // Number of joints this animation has data for
    uint8_t flags; // Flags for this animation
    JointTable *jointTables; // Segmented pointer to the array of joint animation tables
    AnimTrigger *triggers; // Segmented pointer to the array of triggers for this animation
};

struct JointTable {
    uint32_t flags; // Flags to specify which channels are encoded in this joint's animation data
    int16_t *channels; // Segmented pointer to the array of all channel data
};

struct AnimTrigger {
    uint32_t frame; // The frame at which this trigger should run
    void (*triggerCb)(Model* model, uint32_t frame); // The callback to run at the specified frame
};

#define ANIM_COUNTER_FACTOR 16.0f
#define ANIM_COUNTER_SHIFT 4
#define ANIM_COUNTER_TO_FRAME(x) ((x) >> (ANIM_COUNTER_SHIFT))

struct AnimState {
    Animation* anim;
    uint16_t counter; // Frame counter of format 12.4
    int8_t speed; // Animation playback speed of format s3.4
    int8_t triggerIndex; // Index of the previous trigger
};

Gfx *gfxCbBeforeBillboard(Joint* joint, JointMeshLayer *layer);
Gfx *gfxCbAfterBillboard(Joint* joint, JointMeshLayer *layer);

#endif
