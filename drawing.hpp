#include "tgaimage.hpp"
#include <algorithm>
#include "util.hpp"

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

void draw_triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAColor color, TGAImage& img) {
    draw_triangle_outline(t0, t1, t2, color, img);
    
    // bounding box
    int minx = std::min(t0.x, std::min(t1.x, t2.x));
    int miny = std::min(t0.y, std::min(t1.y, t2.y));
    int maxx = std::max(t0.x, std::max(t1.x, t2.x));
    int maxy = std::max(t0.y, std::max(t1.y, t2.y));


    Vec2i t01 = Vec2i(t1.x - t0.x, t1.y - t0.y);
    Vec2i t02 = Vec2i(t2.x - t0.x, t2.y - t0.y);
    float denom = t01.x * t02.y - t01.y * t02.x;
    for (int y = miny; y <= maxy; y++) {
        for (int x = minx; x <= maxx; x++) {
            Vec2i p = Vec2i(x - t0.x, y - t0.y);
            float a = (t02.y * p.x - t02.x * p.y) / denom;
            float b = (p.y * t01.x - p.x * t01.y) / denom;
            float c = 1 - a - b;
            bool inside = a >= 0 && a <= 1 && b >= 0 && b <= 1 && c >= 0 && c <= 1;
            if (inside) {
                img.set(x, y, color);
            }
        }
    }
}

void draw_model(Model model, float zoom, TGAColor color, TGAImage &img, Vec3f light_dir) {
    for (int i = 0; i < model.nfaces(); i++)
    {
        std::vector<int> face = model.face(i);
        if (face.size() != 3) {
            continue;
        }
        Vec3f v0_world = model.vert(face[0]);
        Vec3f v1_world = model.vert(face[1]);
        Vec3f v2_world = model.vert(face[2]);

        Vec2i v0_screen = Vec2i((v0_world.x + zoom/2) * img.width() / zoom, (v0_world.y + zoom/2) * img.height() / zoom);
        Vec2i v1_screen = Vec2i((v1_world.x + zoom/2) * img.width() / zoom, (v1_world.y + zoom/2) * img.height() / zoom);
        Vec2i v2_screen = Vec2i((v2_world.x + zoom/2) * img.width() / zoom, (v2_world.y + zoom/2) * img.height() / zoom);

        Vec3f v0 = v1_world - v0_world;
        Vec3f v1 = v2_world - v0_world;
        Vec3f normal = (v1^v0).normalize();
        float light_intensity = normal * light_dir;
        if (light_intensity <= 0) {
            continue;
        }
        unsigned char color_value = roundf(light_intensity * 255);
        color = TGAColor{color_value, color_value, color_value, color_value};
        draw_triangle(v0_screen, v1_screen, v2_screen, color, img);
    }
}