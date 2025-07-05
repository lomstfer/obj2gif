#pragma once
#include "tgaimage.hpp"
#include <algorithm>
#include "util.hpp"
#include "constants.hpp"

void draw_line(int x0, int y0, int x1, int y1, TGAColor color, TGAImage &img)
{
    if (abs(x0 - x1) >= abs(y0 - y1))
    {

        if (x0 > x1)
        {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        int xd = x1 - x0;
        int yd = y1 - y0;
        float xk = (float)(yd) / (float)(xd);
        img.set(x0, y0, color);
        for (int i = 1; i <= xd; i++)
        {
            img.set(x0 + i, y0 + roundf(xk * i), color);
        }
    }
    else
    {
        if (y0 > y1)
        {
            std::swap(y0, y1);
            std::swap(x0, x1);
        }
        int yd = y1 - y0;
        int xd = x1 - x0;
        float yk = (float)(xd) / (float)(yd);
        img.set(x0, y0, color);
        for (int i = 1; i <= yd; i++)
        {
            img.set(x0 + roundf(yk * i), y0 + i, color);
        }
    }
}

void draw_model_wireframe(Model model, float zoom, TGAColor color, TGAImage &img)
{
    for (int i = 0; i < model.nfaces(); i++)
    {
        std::vector<int> face = model.face(i);
        for (int j = 0; j < face.size(); j++)
        {
            Vec3f v0 = model.vert(face[j]);
            Vec3f v1 = model.vert(face[(j + 1) % face.size()]);
            int x0 = (v0.x + zoom / 2.) * img.width() / zoom;
            int y0 = (v0.y + zoom / 2.) * img.height() / zoom;
            int x1 = (v1.x + zoom / 2.) * img.width() / zoom;
            int y1 = (v1.y + zoom / 2.) * img.height() / zoom;
            draw_line(x0, y0, x1, y1, color, img);
        }
    }
}

void draw_triangle_outline(Vec2i t0, Vec2i t1, Vec2i t2, TGAColor color, TGAImage &img)
{
    draw_line(t0.x, t0.y, t1.x, t1.y, color, img);
    draw_line(t1.x, t1.y, t2.x, t2.y, color, img);
    draw_line(t2.x, t2.y, t0.x, t0.y, color, img);
}

double signed_triangle_area(Vec2i a, Vec2i b, Vec2i c)
{
    return .5 * ((b.y - a.y) * (b.x + a.x) + (c.y - b.y) * (c.x + b.x) + (a.y - c.y) * (a.x + c.x));
}

void draw_triangle(Vec3i a, Vec3i b, Vec3i c, TGAColor color, TGAImage &img, std::vector<float>& z_buffer)
{
    // bounding box
    int minx = std::max(std::min(a.x, std::min(b.x, c.x)), 0);
    int miny = std::max(std::min(a.y, std::min(b.y, c.y)), 0);
    int maxx = std::min(std::max(a.x, std::max(b.x, c.x)), WIDTH-1);
    int maxy = std::min(std::max(a.y, std::max(b.y, c.y)), HEIGHT-1);

    double total_area = signed_triangle_area(a.xy(), b.xy(), c.xy());

    if (total_area <= 0)
    {
        return;
    }

    for (int y = miny; y <= maxy; y++)
    {
        for (int x = minx; x <= maxx; x++)
        {
            Vec2i p = Vec2i(x, y);
            double alpha = signed_triangle_area(p, b.xy(), c.xy()) / total_area;
            double beta = signed_triangle_area(p, c.xy(), a.xy()) / total_area;
            double gamma = signed_triangle_area(p, a.xy(), b.xy()) / total_area;
            bool inside = alpha >= 0 && alpha <= 1 && beta >= 0 && beta <= 1 && gamma >= 0 && gamma <= 1;
            if (!inside)
            {
                continue;
            }

            unsigned char z_for_pixel = static_cast<unsigned char>(alpha * a.z + beta * b.z + gamma * c.z);

            if (z_for_pixel > z_buffer[y * WIDTH + x])
            {
                z_buffer[y * WIDTH + x] = z_for_pixel;
                img.set(x, y, color);
            }
        }
    }
}

void draw_model(Model model, float zoom, TGAColor color, TGAImage &img, std::vector<float>& z_buffer)
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

        float angle = M_PI / 6;
        Mat3<float> rot_y_mat = Mat3<float>(
            cos(angle), 0, sin(angle),
            0, 1, 0,
            -sin(angle), 0, cos(angle));
        v0_world = rot_y_mat * v0_world;
        v1_world = rot_y_mat * v1_world;
        v2_world = rot_y_mat * v2_world;

        float cam_pos = 3;
        v0_world = v0_world * (1 / (1 - v0_world.z / cam_pos));
        v1_world = v1_world * (1 / (1 - v1_world.z / cam_pos));
        v2_world = v2_world * (1 / (1 - v2_world.z / cam_pos));


        Mat4<float> viewport_mat = Mat4<float>(
            WIDTH/2.f, 0,          0, 0 + WIDTH/2.f,
            0,         HEIGHT/2.f, 0, 0 + HEIGHT/2.f,
            0,         0,          1, 0,
            0,         0,          0, 1);

        Vec3f v0_screen = (viewport_mat * Vec4<float>(v0_world.x, v0_world.y, v0_world.z, 1)).xyz();
        Vec3f v1_screen = (viewport_mat * Vec4<float>(v1_world.x, v1_world.y, v1_world.z, 1)).xyz();
        Vec3f v2_screen = (viewport_mat * Vec4<float>(v2_world.x, v2_world.y, v2_world.z, 1)).xyz();

        // Vec3f v0_screen = Vec3f((v0_world.x + zoom / 2) * img.width() / zoom, (v0_world.y + zoom / 2) * img.height() / zoom, v0_world.z);
        // Vec3f v1_screen = Vec3f((v1_world.x + zoom / 2) * img.width() / zoom, (v1_world.y + zoom / 2) * img.height() / zoom, v1_world.z);
        // Vec3f v2_screen = Vec3f((v2_world.x + zoom / 2) * img.width() / zoom, (v2_world.y + zoom / 2) * img.height() / zoom, v2_world.z);

        color = TGAColor{static_cast<unsigned char>(rand() % 255), static_cast<unsigned char>(rand() % 255), static_cast<unsigned char>(rand() % 255), 255};
        draw_triangle(
            Vec3i(roundftoi(v0_screen.x), roundftoi(v0_screen.y), roundftoi(v0_screen.z)),
            Vec3i(roundftoi(v1_screen.x), roundftoi(v1_screen.y), roundftoi(v1_screen.z)),
            Vec3i(roundftoi(v2_screen.x), roundftoi(v2_screen.y), roundftoi(v2_screen.z)),
            color, img, z_buffer);
    }
}