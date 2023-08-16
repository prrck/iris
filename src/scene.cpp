#include "scene.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glog/logging.h>

#include "mesh.h"
#include "vec3.h"

Scene::Scene(const std::string& filepath)
    : _default_material(Vec3(0.777), Vec3(0.0))
{
    // For now assume that all elements are meshes.

    LOG(INFO) << "Loading scene \"" << filepath << "\"..." << std::endl;

    Assimp::Importer importer;

    const aiScene* const ai_scene = importer.ReadFile(
        filepath, aiProcess_Triangulate);

    if (ai_scene == nullptr) {
        throw std::runtime_error("Cannot import file " + filepath);
    }

    const int nb_meshes = ai_scene->mNumMeshes;
    _primitives.resize(nb_meshes);

    for (int i = 0; i < nb_meshes; ++i) {
        const aiMesh& ai_mesh = *ai_scene->mMeshes[i];
        // TODO: Get material info.
        // const aiMaterial* const ai_material
        //     = ai_scene->mMaterials[ai_mesh->mMaterialIndex];

        _primitives[i] = new Mesh(ai_mesh, &_default_material);
    }
}

Scene::~Scene()
{
    for (auto primitive : _primitives) {
        delete primitive;
    }
}

void Scene::add(Renderable* primitive)
{
    _primitives.push_back(primitive);
    if (primitive->material->emittance) {
        _emissives.push_back(primitive);
    }
}