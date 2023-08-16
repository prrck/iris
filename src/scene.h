#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "material.h"
#include "primitive.h"

class Scene {
public:
    Scene(const std::string& filepath);
    ~Scene();

    void add(Renderable* Renderable);
    const std::vector<Renderable*>& get_primitives() const { return _primitives; }
    const std::vector<Renderable*>& get_emissives() const { return _emissives; }

private:
    Material _default_material;
    std::vector<Renderable*> _primitives;
    std::vector<Renderable*> _emissives;
};

#endif // SCENE_H
