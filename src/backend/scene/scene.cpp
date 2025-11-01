#include "scene.h"

#include "backend/loaders/objectLoader/ObjectLoader.h"

namespace s21 {
Scene::Scene() {
  Camera camera{};
  cameras.push_back(camera);
  currentCamera = &(cameras[0]);

  Light light{};
  lights.push_back(light);
}

void Scene::render() {}

void Scene::loadObject(std::string filepath) {
  Mesh mesh;
  ObjectLoader::loadObj(filepath, mesh, materialManager);
  Object obj{mesh};

  addObject(obj);
}

void Scene::addObject(Object& obj) { objects.push_back(obj); }

std::vector<Object>& Scene::getObjects() { return objects; }

const Light& Scene::getLight(uint32_t index) const { return lights[index]; }

Camera* Scene::getCurrentCamera() { return currentCamera; }

const Material& Scene::getMaterial(uint32_t idMaterial) const {
  return materialManager.getMaterial(idMaterial);
}
}  // namespace s21