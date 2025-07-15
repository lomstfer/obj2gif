#pragma once
#include <algorithm>
#include "util.hpp"
#include "constants.hpp"
#include "geometry.hpp"
#include <cmath>
#include <string>

float signed_triangle_area(Vec2i a, Vec2i b, Vec2i c)
{
    return .5f * ((b.y - a.y) * (b.x + a.x) + (c.y - b.y) * (c.x + b.x) + (a.y - c.y) * (a.x + c.x));
}

void draw_triangle(Vec3i a, Vec3i b, Vec3i c, Color color, std::vector<uint8_t> &image, std::vector<float> &z_buffer)
{
    // bounding box
    int minx = std::max(std::min(a.x, std::min(b.x, c.x)), 0);
    int miny = std::max(std::min(a.y, std::min(b.y, c.y)), 0);
    int maxx = std::min(std::max(a.x, std::max(b.x, c.x)), WIDTH - 1);
    int maxy = std::min(std::max(a.y, std::max(b.y, c.y)), HEIGHT - 1);

    float total_area = signed_triangle_area(a.xy(), b.xy(), c.xy());

    if (total_area <= 0)
    {
        return;
    }

    for (int y = miny; y <= maxy; y++)
    {
        for (int x = minx; x <= maxx; x++)
        {
            Vec2i p = Vec2i(x, y);
            float alpha = signed_triangle_area(p, b.xy(), c.xy()) / total_area;
            float beta = signed_triangle_area(p, c.xy(), a.xy()) / total_area;
            float gamma = signed_triangle_area(p, a.xy(), b.xy()) / total_area;
            bool inside = alpha >= 0 && alpha <= 1 && beta >= 0 && beta <= 1 && gamma >= 0 && gamma <= 1;
            if (!inside)
            {
                continue;
            }

            float z_for_pixel = alpha * a.z + beta * b.z + gamma * c.z;

            if (z_for_pixel > z_buffer[y * WIDTH + x])
            {
                z_buffer[y * WIDTH + x] = z_for_pixel;
                image[(y * WIDTH + x) * 4] = color.r;
                image[(y * WIDTH + x) * 4 + 1] = color.g;
                image[(y * WIDTH + x) * 4 + 2] = color.b;
                image[(y * WIDTH + x) * 4 + 3] = color.a;
            }
        }
    }
}

void draw_model(Model model, float angle, Color color, std::vector<uint8_t> &image, std::vector<float> &z_buffer)
{
    for (int i = 0; i < model.nfaces(); i++)
    {
        std::vector<int> face = model.face(i);
        if (face.size() != 3)
        {
            continue;
        }
        Vec3f v0_world = model.vert(face[0]);
        Vec3f v1_world = model.vert(face[1]);
        Vec3f v2_world = model.vert(face[2]);
        Mat3<float> rot_y_mat = Mat3<float>(
            cos(angle), 0, sin(angle),
            0, 1, 0,
            -sin(angle), 0, cos(angle));
        v0_world = rot_y_mat * v0_world;
        v1_world = rot_y_mat * v1_world;
        v2_world = rot_y_mat * v2_world;

        // perspective
        float model_max_radius = sqrt(model.max_x * model.max_x + model.max_z * model.max_z);
        float cam_pos = model_max_radius * 3;
        v0_world = v0_world * (1 / (1 - v0_world.z / cam_pos));
        v1_world = v1_world * (1 / (1 - v1_world.z / cam_pos));
        v2_world = v2_world * (1 / (1 - v2_world.z / cam_pos));

        Vec3f light_dir = Vec3f(0.5f, 0.5f, 1).normalize();
        Vec3f face_v_a = v1_world - v0_world;
        Vec3f face_v_b = v2_world - v0_world;
        Vec3f face_normal = face_v_a.cross(face_v_b).normalize();
        float light_value = light_dir.dot(face_normal);
        light_value = light_value < 0 ? 0 : light_value > 1 ? 1
                                                            : light_value;

        float z_scale = 1000;
        Vec3f v0_screen = Vec3f(
            util::remap(v0_world.x, model.min_x, model.max_x, WIDTH / 4, WIDTH - WIDTH / 4),
            util::remap(v0_world.y, model.min_y, model.max_y, HEIGHT / 4, HEIGHT - HEIGHT / 4),
            (v0_world.z + model_max_radius) * z_scale);
        Vec3f v1_screen = Vec3f(
            util::remap(v1_world.x, model.min_x, model.max_x, WIDTH / 4, WIDTH - WIDTH / 4),
            util::remap(v1_world.y, model.min_y, model.max_y, HEIGHT / 4, HEIGHT - HEIGHT / 4),
            (v1_world.z + model_max_radius) * z_scale);
        Vec3f v2_screen = Vec3f(
            util::remap(v2_world.x, model.min_x, model.max_x, WIDTH / 4, WIDTH - WIDTH / 4),
            util::remap(v2_world.y, model.min_y, model.max_y, HEIGHT / 4, HEIGHT - HEIGHT / 4),
            (v2_world.z + model_max_radius) * z_scale);

        float r = (float)color.r * light_value;
        float g = (float)color.g * light_value;
        float b = (float)color.b * light_value;
        Color color_result = Color{(uint8_t)util::roundftoi(r), (uint8_t)util::roundftoi(g), (uint8_t)util::roundftoi(b), color.a};
        draw_triangle(
            Vec3i(util::roundftoi(v0_screen.x), util::roundftoi(v0_screen.y), util::roundftoi(v0_screen.z)),
            Vec3i(util::roundftoi(v1_screen.x), util::roundftoi(v1_screen.y), util::roundftoi(v1_screen.z)),
            Vec3i(util::roundftoi(v2_screen.x), util::roundftoi(v2_screen.y), util::roundftoi(v2_screen.z)),
            color_result, image, z_buffer);
    }
}