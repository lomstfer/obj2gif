#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.hpp"
#include "util.hpp"

Model::Model(std::string filename) : _verts(), _faces()
{
    std::ifstream in(filename);
    if (!in)
    {
        Log("Cannot open file: " + filename);
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
            _verts.push_back(v);

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
            // triangulation
            if (face_indices.size() == 4) {
                int a = face_indices[0];
                int b = face_indices[1];
                int c = face_indices[2];
                int d = face_indices[3];
                face_indices.pop_back();
                _faces.push_back({c, d, a});
            }
            _faces.push_back(face_indices);

        }
    }



    std::cerr << "Model loaded: " << _verts.size() << " vertices, " << _faces.size() << " faces." << std::endl;
}

Model::~Model()
{
}

int Model::nverts()
{
    return (int)_verts.size();
}

int Model::nfaces()
{
    return (int)_faces.size();
}

std::vector<int> Model::face(int idx)
{
    return _faces[idx];
}

Vec3f Model::vert(int i)
{
    return _verts[i];
}