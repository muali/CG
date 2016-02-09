#pragma once

#include <vector>
#include <string>
#include <assimp\Importer.hpp>

#include "mesh.h"

struct scene
{
private:
    std::vector<mesh> meshes;
};


scene load_scene_from_obj(std::string const& path)
{
    Assimp::Importer importer;
    aiScene const& scene = *importer.ReadFile(path, 0);
}