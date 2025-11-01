#include "TextureLoader.h"

#include "backend/material_manager/material_manager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <iostream>

#include "stb_image.h"

namespace s21 {
Texture TextureLoader::loadTexture(const std::string &filePath) {
  Texture texture;
  texture.colors_ =
      loadImageToEigenArray(filePath, texture.width_, texture.height_);
  return texture;
}

std::vector<Color> TextureLoader::loadImageToEigenArray(
    const std::string &filename, int &width, int &height) {
  int channels = 3;
  unsigned char *imageData =
      stbi_load(filename.c_str(), &width, &height, &channels, 3);

  if (!imageData) {
    std::cerr << "Error: Failed to load image: " << filename << std::endl;
    exit(1);
    return {};
  }

  std::vector<Eigen::Vector3f> pixelColors;
  pixelColors.reserve(width * height);

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      int index = (y * width + x) * 3;
      float r = imageData[index];
      float g = imageData[index + 1];
      float b = imageData[index + 2];

      pixelColors.emplace_back(r, g, b);
    }
  }

  stbi_image_free(imageData);

  return pixelColors;
}
}  // namespace s21