#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.hpp"

Model::Model(std::string filename) : verts_(), faces_()
{
    std::ifstream in(filename);
    if (!in)
    {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::string line;

    while (std::getline(in, line))
    {
        if (line.empty() || line[0] == '#')
        {
            continue;
        }

        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v")
        {
            Vec3f v;

            iss >> v.x >> v.y >> v.z;
            verts_.push_back(v);

            min_x = std::min(v.x, min_x);
            min_y = std::min(v.y, min_y);
            min_z = std::min(v.z, min_z);
            max_x = std::max(v.x, max_x);
            max_y = std::max(v.y, max_y);
            max_z = std::max(v.z, max_z);
        }
        else if (prefix == "f")
        {
            std::vector<int> face_indices;
            std::string face_vertex_str;

            while (iss >> face_vertex_str)
            {
                size_t slash_pos = face_vertex_str.find('/');
                int idx = std::stoi(face_vertex_str.substr(0, slash_pos));

                face_indices.push_back(idx - 1);
            }
            faces_.push_back(face_indices);
        }
    }

    std::cerr << "Model loaded: " << verts_.size() << " vertices, " << faces_.size() << " faces." << std::endl;
}

Model::~Model()
{
}

int Model::nverts()
{
    return (int)verts_.size();
}

int Model::nfaces()
{
    return (int)faces_.size();
}

std::vector<int> Model::face(int idx)
{
    return faces_[idx];
}

Vec3f Model::vert(int i)
{
    return verts_[i];
}