#include "model.hpp"
#include "vertex.hpp"
#include "texture.hpp"
#include "mesh.hpp"
#include "common.hpp"

#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/types.h>

void Model::Create(const std::string& path, const std::unique_ptr<Mesh>& mesh, const std::string& format) {
    Assimp::Exporter exporter;
    const aiScene scene = GenerateScene(mesh);

    exporter.Export(&scene, format, path);
}

std::shared_ptr<Model> Model::Load(const std::filesystem::path& path) {
    auto model = std::make_shared<Model>();

    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path.string(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR: Failed to load model at: " << path << " - " << import.GetErrorString() << std::endl;
        return model;
    }

    model->directory = path.parent_path();
    model->processNode(scene, scene->mRootNode);
    return model;
}

void Model::processNode(const aiScene* scene, const aiNode* node) {
    for (size_t i = 0; i< node->mNumMeshes; i++) {
        const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(scene, mesh);
    }

    for (size_t i = 0; i< node->mNumChildren; i++) {
        processNode(scene, node->mChildren[i]);
    }
}

void Model::processMesh(const aiScene* scene, const aiMesh* mesh) {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<std::shared_ptr<Texture>> textures;

    for (size_t i = 0; i< mesh->mNumVertices; i++) {
        vertices.emplace_back(
            glm::vec3_cast(mesh->mVertices[i]),
            glm::vec3_cast(mesh->mNormals[i]),
            mesh->HasTextureCoords(0) ? glm::vec2_cast(mesh->mTextureCoords[0][i]) : vec2::zero
        );
    }

    for (size_t i = 0; i< mesh->mNumFaces; i++) {
        const aiFace& face = mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex >= 0) {
        const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        auto diffuseMaps = loadTextures(material, aiTextureType_DIFFUSE);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        /*auto specularMaps = loadTextures(material, aiTextureType_SPECULAR);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        auto normalMaps = loadTextures(material, aiTextureType_HEIGHT);
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

        auto heightMaps = loadTextures(material, aiTextureType_AMBIENT);
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());*/

        if (textures.empty()) {
            aiColor3D color;
            material->Get(AI_MATKEY_COLOR_DIFFUSE, color);

            auto r = static_cast<uint8_t>(color.r * 255);
            auto g = static_cast<uint8_t>(color.g * 255);
            auto b = static_cast<uint8_t>(color.b * 255);
            textures.push_back(std::make_shared<Texture>(r, g, b));
        }
    }

    meshes.push_back(std::make_unique<Mesh>(std::move(vertices), std::move(indices), std::move(textures)));
}

std::vector<std::shared_ptr<Texture>> Model::loadTextures(const aiMaterial* material, aiTextureType type) {
    std::vector<std::shared_ptr<Texture>> textures;
    for (size_t i = 0; i < material->GetTextureCount(type); i++) {
        aiString str;
        if (material->GetTexture(type, i, &str) == AI_SUCCESS) {
            std::filesystem::path path = directory;
            path /= str.C_Str();

            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            bool skip = false;
            for (const auto& texture : texturesLoaded) {
                if (path == texture->getPath()) {
                    textures.push_back(texture);
                    skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }

            // if texture hasn't been loaded already, load it
            if (!skip) {
                if (!std::filesystem::exists(path)) {
                    std::cerr << "ERROR: Could load texture from path: " << path << std::endl;
                    continue;
                }

                auto texture = std::make_shared<Texture>(path.string(), true, false);
                textures.push_back(texture);
                texture->setType(type);
                texturesLoaded.push_back(texture); // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
            }
        } else {
            std::cerr << "ERROR: Could not get texture from material" << std::endl;
        }
    }

    return textures;
}

aiScene Model::GenerateScene(const std::unique_ptr<Mesh>& mesh) {
    aiScene scene;
    scene.mRootNode = new aiNode{};

    scene.mMaterials = new aiMaterial*[1];
    scene.mNumMaterials = 1;
    scene.mMaterials[0] = new aiMaterial{};

    scene.mMeshes = new aiMesh*[1];
    scene.mNumMeshes = 1;
    scene.mMeshes[0] = new aiMesh{};
    scene.mMeshes[0]->mMaterialIndex = 0;

    scene.mRootNode->mMeshes = new unsigned int[1];
    scene.mRootNode->mNumMeshes = 1;
    scene.mRootNode->mMeshes[0] = 0;

    auto pMesh = scene.mMeshes[0];

    const auto& vertices = mesh->vertices;
    size_t numVertices = vertices.size();

    pMesh->mVertices = new aiVector3D[numVertices];
    pMesh->mNumVertices = numVertices;

    pMesh->mNormals = new aiVector3D[numVertices];

    pMesh->mTextureCoords[0] = new aiVector3D[numVertices];
    pMesh->mNumUVComponents[0] = numVertices;

    for (size_t i = 0; i < numVertices; i++) {
        const auto& v = vertices[i];
        pMesh->mVertices[i] = aiVector3D{v.position.x, v.position.y, v.position.z};
        pMesh->mNormals[i] = aiVector3D{v.normal.x, v.normal.y, v.normal.z};
        pMesh->mTextureCoords[0][i] = aiVector3D{v.texture.x, v.texture.y, 0};
    }

    const auto& indices = mesh->indices;
    size_t numFaces = indices.size() / 3;

    pMesh->mFaces = new aiFace[numFaces];
    pMesh->mNumFaces = numFaces;

    for (size_t i = 0, j = 0; i < numFaces; i++, j += 3) {
        aiFace& face = pMesh->mFaces[i];
        face.mIndices = new unsigned int[3];
        face.mNumIndices = 3;
        face.mIndices[0] = indices[j+0];
        face.mIndices[1] = indices[j+1];
        face.mIndices[2] = indices[j+2];
    }

    return scene;
}

void Model::render(const std::unique_ptr<Shader>& shader) const {
    for (auto& mesh : meshes) {
        mesh->render(shader);
    }
}
