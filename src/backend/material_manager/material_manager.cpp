#include "material_manager.h"

namespace s21 {
const Material &MaterialManager::getMaterial(uint32_t idMaterial) const {
  return materials_[idMaterial];
}

uint32_t MaterialManager::addMaterial(std::string &name, Material &material) {
  materials_.push_back(material);
  uint32_t n = materials_.size() - 1;

  mapMaterial_.insert_or_assign(name, n);

  return n;
}

uint32_t MaterialManager::getMaterialId(std::string &name) const {
  return mapMaterial_.at(name);
}
}  // namespace s21