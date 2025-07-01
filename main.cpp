#include "tgaimage.h"
const TGAColor white{255, 255, 255, 255};
const TGAColor red{0, 0, 255, 255};

void draw_line(int x0, int y0, int x1, int y1, TGAImage &img)
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
        img.set(x0, y0, red);
        for (int i = 1; i <= xd; i++)
        {
            img.set(x0 + i, y0 + int(xk * i + 0.5), red);
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
        img.set(x0, y0, red);
        for (int i = 1; i <= yd; i++)
        {
            img.set(x0 + int(yk * i + 0.5), y0 + i, red);
        }
    }
}

int main()
{
    TGAImage image(100, 100, TGAImage::RGB);

    image.set(52, 41, white);
    draw_line(20, 20, 0, 50, image);

    image.write_tga_file("output.tga");
}
