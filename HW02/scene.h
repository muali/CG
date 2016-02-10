#pragma once

#include <functional>
#include <vector>
#include <string>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include "mesh.h"

struct scene
{
    
    void add_mesh(mesh item)
    {
        meshes.push_back(item);
    }

    void load()
    {
        for (auto& item: meshes)
            item.load();
    }

    void draw()
    {
        for (auto& item: meshes)
            item.draw();
    }

private:
    std::vector<mesh> meshes;
};


inline scene load_scene_from_obj(std::string const& path)
{
    Assimp::Importer importer;
    aiScene const& ai_scene = *importer.ReadFile(path, 0);
    scene result;

    auto process_mesh = [&](aiMesh* mesh)
    {
        std::vector<vertex> vertices;
        std::vector<GLuint> indices;

        for (size_t i = 0; i < mesh->mNumVertices; ++i)
        {
            vertex v;
            v.position.x = mesh->mVertices[i].x;
            v.position.y = mesh->mVertices[i].y;
            v.position.z = mesh->mVertices[i].z;
            v.normal.x = mesh->mNormals[i].x;
            v.normal.y = mesh->mNormals[i].y;
            v.normal.z = mesh->mNormals[i].z;
            vertices.push_back(v);
        }

        for(size_t i = 0; i < mesh->mNumFaces; ++i)
        {
            aiFace face = mesh->mFaces[i];
            for(GLuint j = 0; j < face.mNumIndices; ++j)
                indices.push_back(face.mIndices[j]);
        }  

        return ::mesh(vertices, indices);
    };

    std::function<void(aiNode*)> process_node = [&](aiNode* node)
    {
        for (size_t i = 0; i < node->mNumMeshes; ++i)
        {
            result.add_mesh(process_mesh(ai_scene.mMeshes[node->mMeshes[i]]));
        }
        for (size_t i = 0; i < node->mNumChildren; ++i)
        {
            process_node(node->mChildren[i]);
        }
    };


    process_node(ai_scene.mRootNode);
    result.load();
    return result;
}