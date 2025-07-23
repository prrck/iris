#include "scene.h"

#include <cassert>
#include <optional>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <spdlog/spdlog.h>

#include "bvh.h"
#include "intersection.h"
#include "mesh.h"
#include "vec3.h"

Scene::Scene(const std::string &filepath)
    : _default_material(Vec3(0.777), Vec3(0.0)) {
  // For now assume that all elements are meshes.
  spdlog::info("Loading scene \"{}\"...", filepath);

  Assimp::Importer importer;

  const aiScene *const ai_scene =
      importer.ReadFile(filepath, aiProcess_Triangulate);

  if (ai_scene == nullptr) {
    throw std::runtime_error("Cannot import file " + filepath);
  }

  const int nb_meshes = ai_scene->mNumMeshes;
  _primitives.resize(nb_meshes);

  for (int i = 0; i < nb_meshes; ++i) {
    const aiMesh &ai_mesh = *ai_scene->mMeshes[i];
    // TODO: Get material info.
    // const aiMaterial* const ai_material
    //     = ai_scene->mMaterials[ai_mesh->mMaterialIndex];

    _primitives[i] = new Mesh(ai_mesh, &_default_material);
  }
}

Scene::~Scene() {
  for (auto primitive : _primitives) {
    delete primitive;
  }
  if (_bvh)
    delete _bvh;
}

void Scene::add(Primitive *primitive) {
  _primitives.push_back(primitive);
  if (primitive->material->emittance) {
    _emissives.push_back(primitive);
  }
}

void Scene::build_bvh() {
  // _bvh = new BVH(_primitives.begin(), _primitives.end());
  // LOG(INFO) << "Building BVH..." << std::endl;
  spdlog::info("Building scene BVH...");
  _bvh = new BVH(_primitives);
}

// std::optional<Intersection> Scene::find_closest_inter(Ray &ray) const {
//   assert(_bvh);

//   std::optional<Intersection> closest_inter = std::nullopt;

//   for (auto p : _primitives) {
//     if (p->bounds().intersect_b(ray)) {
//       std::optional<Intersection> inter = p->intersect(ray);

//       if (inter) {
//         if (!closest_inter || inter.value().t < closest_inter.value().t) {
//           closest_inter = inter;
//           ray.t_max = inter.value().t;
//         }
//       }
//     }
//   }

//   return closest_inter;
// }

std::optional<Intersection> Scene::find_closest_inter(Ray &ray) const {
  assert(_bvh);

  return _bvh->intersect(ray);
}