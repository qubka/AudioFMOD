#pragma once

class Texture;
class Mesh;

class aiScene;
class aiNode;
class aiMesh;
class aiMaterial;

#include <assimp/material.h>

class Shader;

class Model {
public:
    Model() = default;
    ~Model() = default;

    static void Create(const std::string& path, const std::unique_ptr<Mesh>& mesh, const std::string& format = "fbx");
    static std::shared_ptr<Model> Load(const std::filesystem::path& path);

    void render(const std::unique_ptr<Shader>& shader) const;

private:
    std::filesystem::path directory;
    std::vector<std::unique_ptr<Mesh>> meshes;
    std::vector<std::shared_ptr<Texture>> texturesLoaded;

    void processNode(const aiScene* scene, const aiNode* node);
    void processMesh(const aiScene* scene, const aiMesh* mesh);
    std::vector<std::shared_ptr<Texture>> loadTextures(const aiMaterial* material, aiTextureType type);

    static aiScene GenerateScene(const std::unique_ptr<Mesh>& mesh);
};
