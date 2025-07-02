#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.hpp"

Model::Model(std::string filename) : verts_(), faces_() {
    // Use the ifstream constructor for RAII-style file management
    std::ifstream in(filename);
    if (!in) {
        // Throw an exception on failure. A constructor should not fail silently.
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::string line;
    // Use `while (std::getline(...))` which is the correct way to read a file line-by-line.
    while (std::getline(in, line)) {
        // Skip empty lines or comments
        if (line.empty() || line[0] == '#') {
            continue;
        }

        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix; // Read the first token (e.g., "v", "f", "vn")

        if (prefix == "v") {
            Vec3f v;
            // Directly read into the vector's components
            iss >> v.raw[0] >> v.raw[1] >> v.raw[2];
            verts_.push_back(v);

        } else if (prefix == "f") {
            std::vector<int> face_indices;
            std::string face_vertex_str;
            
            // This loop now correctly handles different face formats
            while (iss >> face_vertex_str) {
                // The vertex index is the number before the first '/'
                size_t slash_pos = face_vertex_str.find('/');
                int idx = std::stoi(face_vertex_str.substr(0, slash_pos));
                
                // .obj indices are 1-based, so convert to 0-based
                face_indices.push_back(idx - 1);
            }
            faces_.push_back(face_indices);
        }
    }
    
    // Logging to std::cerr is fine for debugging, but consider making it optional
    // for a reusable library class.
    std::cerr << "Model loaded: " << verts_.size() << " vertices, " << faces_.size() << " faces." << std::endl;
}

Model::~Model() {
}

int Model::nverts() {
    return (int)verts_.size();
}

int Model::nfaces() {
    return (int)faces_.size();
}

std::vector<int> Model::face(int idx) {
    return faces_[idx];
}

Vec3f Model::vert(int i) {
    return verts_[i];
}