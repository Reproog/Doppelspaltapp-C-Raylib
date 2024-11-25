#include <stdio.h>
#include <assert.h>

#include "raylib.h"
#include "raymath.h"
#include "config.h"

static float clampf(float value, float min, float max) {
    return (value < min) ? min : (value > max) ? max : value;
}

static float ilerpf(float min, float max, float value) {
    return (value - min) / (max - min);
}

static float lerpf(float min, float max, float value) {
    return min + value * (max - min);
}

static void slider(int id, Rectangle bounds, float *value, float min, float max, const char *label, bool show, float change_value) {
    bounds.x += MARGIN;
    //bounds.y += MARGIN;
    bounds.width -= 2 * MARGIN;

    DrawRectangle(
        bounds.x, 
        bounds.y + bounds.height * 0.5f - SLIDER_THICKNESS * 0.5f, 
        bounds.width, 
        SLIDER_THICKNESS, 
        SLIDER_COLOR
    );

    assert(min <= max);
    float grip_value = ilerpf(min, max, *value) * bounds.width;

    float grip_pos_x = bounds.x + grip_value - SLIDER_GRIP_SIZE;
    DrawRectangle(
        grip_pos_x, 
        bounds.y + bounds.height * 0.5f - SLIDER_GRIP_SIZE, 
        SLIDER_GRIP_SIZE * 2.0f, 
        SLIDER_GRIP_SIZE * 2.0f, 
        SLIDER_GRIP_COLOR
    );

    DrawText(
        label, 
        bounds.x - MARGIN - MeasureText(label, FONT_SIZE), 
        bounds.y + bounds.height * 0.5f - FONT_SIZE * 0.5f, 
        FONT_SIZE, 
        DARKGRAY
    );

    if (show) {
        DrawText(
            TextFormat("%.2f", *value - change_value), 
            bounds.x + bounds.width + MARGIN, 
            bounds.y + bounds.height * 0.5f - FONT_SIZE * 0.5f, 
            FONT_SIZE, 
            DARKGRAY
        );
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        Vector2 mouse_position = GetMousePosition();
        if (active_id < 0) {
            Rectangle grip_rect = {
                grip_pos_x,
                bounds.y + bounds.height * 0.5f - SLIDER_GRIP_SIZE,
                SLIDER_GRIP_SIZE * 2.0f,
                SLIDER_GRIP_SIZE * 2.0f
            };
            if (CheckCollisionPointRec(mouse_position, grip_rect)) {
                active_id = id;
            }
        } else if (active_id == id) {
            float grip_min = bounds.x;
            float grip_max = bounds.x + bounds.width;
            float xf = clampf(mouse_position.x, grip_min, grip_max);
            xf = ilerpf(grip_min, grip_max, xf);
            xf = lerpf(min, max, xf);
            *value = xf;
        }
    } else if (active_id == id) {
        active_id = -1;
    }
}
