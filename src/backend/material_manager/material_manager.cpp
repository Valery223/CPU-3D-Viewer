#include "material_manager.h"

namespace s21 {
MaterialManager::MaterialManager() {
  // Индекс 0 — дефолтный материал (нейтральный серый, без текстуры).
  Material def;
  def.ambient = Eigen::Vector3f(0.2f, 0.2f, 0.2f);
  def.diffuse = Eigen::Vector3f(0.7f, 0.7f, 0.7f);
  def.specular = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
  def.shininess = 1.0f;
  def.texture.width_ = 0;
  def.texture.height_ = 0;
  std::string name = "__default__";
  addMaterial(name, def);
}

const Material &MaterialManager::getMaterial(uint32_t idMaterial) const {
  if (idMaterial >= materials_.size()) return materials_[0];  // дефолтный
  return materials_[idMaterial];
}

uint32_t MaterialManager::addMaterial(std::string &name, Material &material) {
  materials_.push_back(material);
  uint32_t n = materials_.size() - 1;

  mapMaterial_.insert_or_assign(name, n);

  return n;
}

uint32_t MaterialManager::getMaterialId(std::string &name) const {
  auto it = mapMaterial_.find(name);
  return (it != mapMaterial_.end()) ? it->second : 0;  // 0 — дефолтный материал
}
}  // namespace s21