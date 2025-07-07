#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.hpp"

class Model
{
private:
    std::vector<Vec3f> verts_;
    std::vector<std::vector<int>> faces_;

public:
    Model(std::string filename);
    ~Model();
    int nverts();
    int nfaces();
    Vec3f vert(int i);
    std::vector<int> face(int idx);
    float min_x = std::numeric_limits<float>::max();
    float min_y = std::numeric_limits<float>::max();
    float min_z = std::numeric_limits<float>::max();
    float max_x = -std::numeric_limits<float>::max();
    float max_y = -std::numeric_limits<float>::max();
    float max_z = -std::numeric_limits<float>::max();
};

#endif //__MODEL_H__