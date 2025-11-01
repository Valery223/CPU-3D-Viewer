#include "MaterialLoader.h"

#include <filesystem>
#include <iostream>

#include "backend/loaders/textureLoader/TextureLoader.h"

namespace s21 {
void MtlFileLoader::loadMtl(const std::string& filepath,
                            MaterialManager& materialManager) {
  std::ifstream file(filepath);
  if (!file.is_open()) {
    std::cerr << "Could not open material file " << filepath << "\n";
    return;
  }

  Material material;
  std::string line;
  std::string currentMaterialName;

  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string prefix;
    iss >> prefix;

    if (prefix == "newmtl") {
      if (!currentMaterialName.empty()) {
        materialManager.addMaterial(currentMaterialName, material);
      }
      iss >> currentMaterialName;
      material = Material();
    } else if (prefix == "Ka") {
      iss >> material.ambient[0] >> material.ambient[1] >> material.ambient[2];
    } else if (prefix == "Kd") {
      iss >> material.diffuse[0] >> material.diffuse[1] >> material.diffuse[2];
    } else if (prefix == "Ks") {
      iss >> material.specular[0] >> material.specular[1] >>
          material.specular[2];
    } else if (prefix == "Ns") {
      iss >> material.shininess;
    } else if (prefix == "map_Kd") {
      std::string textureKdName;
      iss >> textureKdName;

      std::filesystem::path mtlPath(filepath);
      std::filesystem::path dirPath = mtlPath.parent_path();

      std::filesystem::path textureKdPath = dirPath / textureKdName;

      try {
        Texture texture = TextureLoader::loadTexture(textureKdPath);
        material.texture = texture;
      } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
      }
    }
  }

  if (!currentMaterialName.empty()) {
    materialManager.addMaterial(currentMaterialName, material);
  }

  file.close();
}
}  // namespace s21