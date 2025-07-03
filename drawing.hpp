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


void draw_model_wireframe(Model model, float zoom, TGAColor color, TGAImage &img) {
    for (int i = 0; i < model.nfaces(); i++)
    {
        std::vector<int> face = model.face(i);
        for (int j = 0; j < face.size(); j++)
        {
            Vec3f v0 = model.vert(face[j]);
            Vec3f v1 = model.vert(face[(j + 1) % face.size()]);
            int x0 = (v0.x + zoom/2.) * img.width() / zoom;
            int y0 = (v0.y + zoom/2.) * img.height() / zoom;
            int x1 = (v1.x + zoom/2.) * img.width() / zoom;
            int y1 = (v1.y + zoom/2.) * img.height() / zoom;
            draw_line(x0, y0, x1, y1, color, img);
        }
    }
}

void draw_triangle_outline(Vec2i t0, Vec2i t1, Vec2i t2, TGAColor color, TGAImage& img) {
    draw_line(t0.x, t0.y, t1.x, t1.y, color, img);
    draw_line(t1.x, t1.y, t2.x, t2.y, color, img);
    draw_line(t2.x, t2.y, t0.x, t0.y, color, img);
}

double signed_triangle_area(Vec2i a, Vec2i b, Vec2i c) {
    return .5*((b.y-a.y)*(b.x+a.x) + (c.y-b.y)*(c.x+b.x) + (a.y-c.y)*(a.x+c.x));
}

void draw_triangle_no_z_buffer(Vec2i a, Vec2i b, Vec2i c, TGAColor color, TGAImage& img) {    
    // bounding box
    int minx = std::min(a.x, std::min(b.x, c.x));
    int miny = std::min(a.y, std::min(b.y, c.y));
    int maxx = std::max(a.x, std::max(b.x, c.x));
    int maxy = std::max(a.y, std::max(b.y, c.y));

    double total_area = signed_triangle_area(a, b, c);
    for (int y = miny; y <= maxy; y++) {
        for (int x = minx; x <= maxx; x++) {
            Vec2i p = Vec2i(x, y);
            double alpha = signed_triangle_area(p, b, c) / total_area;
            double beta = signed_triangle_area(p, c, a) / total_area;
            double gamma = signed_triangle_area(p, a, b) / total_area;
            bool inside = alpha >= 0 && alpha <= 1 && beta >= 0 && beta <= 1 && gamma >= 0 && gamma <= 1;
            if (inside) {
                img.set(x, y, color);
            }
        }
    }
}

void draw_triangle(Vec3f a, Vec3f b, Vec3f c, TGAColor color, TGAImage& img, std::vector<float>& zbuffer) {    
    Vec2i ai = Vec2i(roundftoi(a.x), roundftoi(a.y));
    Vec2i bi = Vec2i(roundftoi(b.x), roundftoi(b.y));
    Vec2i ci = Vec2i(roundftoi(c.x), roundftoi(c.y));

    // bounding box
    int minx = std::min(ai.x, std::min(bi.x, ci.x));
    int miny = std::min(ai.y, std::min(bi.y, ci.y));
    int maxx = std::max(ai.x, std::max(bi.x, ci.x));
    int maxy = std::max(ai.y, std::max(bi.y, ci.y));

    double total_area = signed_triangle_area(ai, bi, ci);
    for (int y = miny; y <= maxy; y++) {
        for (int x = minx; x <= maxx; x++) {
            Vec2i p = Vec2i(x, y);
            double alpha = signed_triangle_area(p, bi, ci) / total_area;
            double beta = signed_triangle_area(p, ci, ai) / total_area;
            double gamma = signed_triangle_area(p, ai, bi) / total_area;
            bool inside = alpha >= 0 && alpha <= 1 && beta >= 0 && beta <= 1 && gamma >= 0 && gamma <= 1;
            if (!inside) {
                continue;
            }

            float z_value = a.z * alpha + b.z * beta + c.z * gamma;
            if (z_value > zbuffer[y * WIDTH + x]) {
                zbuffer[y * WIDTH + x] = z_value;
                img.set(x, y, color);
            }
        }
    }
}

void draw_model(Model model, float zoom, TGAColor color, TGAImage &img, Vec3f light_dir, std::vector<float>& zbuffer) {
    for (int i = 0; i < model.nfaces(); i++)
    {
        std::vector<int> face = model.face(i);
        if (face.size() != 3) {
            continue;
        }
        Vec3f v0_world = model.vert(face[0]);
        Vec3f v1_world = model.vert(face[1]);
        Vec3f v2_world = model.vert(face[2]);

        Vec3f v0_screen = Vec3f((v0_world.x + zoom/2) * img.width() / zoom, (v0_world.y + zoom/2) * img.height() / zoom, v0_world.z);
        Vec3f v1_screen = Vec3f((v1_world.x + zoom/2) * img.width() / zoom, (v1_world.y + zoom/2) * img.height() / zoom, v1_world.z);
        Vec3f v2_screen = Vec3f((v2_world.x + zoom/2) * img.width() / zoom, (v2_world.y + zoom/2) * img.height() / zoom, v2_world.z);

        Vec3f v0 = v1_world - v0_world;
        Vec3f v1 = v2_world - v0_world;
        Vec3f normal = (v1^v0).normalize();
        float light_intensity = normal * light_dir;
        if (light_intensity <= 0) {
            continue;
        }
        unsigned char color_value = roundftoi(light_intensity * 255);
        color = TGAColor{color_value, color_value, color_value, 255};
        draw_triangle(v0_screen, v1_screen, v2_screen, color, img, zbuffer);
        // draw_triangle_no_z_buffer(Vec2i(roundftoi(v0_screen.x), roundftoi(v0_screen.y)), Vec2i(roundftoi(v1_screen.x), roundftoi(v1_screen.y)), Vec2i(roundftoi(v2_screen.x), roundftoi(v2_screen.y)), color, img);
    }
}