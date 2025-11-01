#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <string>

#include "backend/types.h"

namespace s21 {
/**
 * @struct Texture
 * @brief Структура, представляющая текстуру.
 */
struct Texture;

/**
 * @class TextureLoader
 * @brief Класс для загрузки текстур из файлов изображений.
 */
class TextureLoader {
 public:
  /**
   * @brief Загружает текстуру из файла.
   * @param filePath Путь к файлу изображения.
   * @return Загруженная текстура.
   */
  static Texture loadTexture(const std::string &filePath);

 private:
  /**
   * @brief Закрытый конструктор, чтобы запретить создание экземпляров класса.
   */
  TextureLoader(){};

  /**
   * @brief Загружает изображение в массив Eigen.
   * @param filename Путь к файлу изображения.
   * @param width Ширина изображения (возвращаемое значение).
   * @param height Высота изображения (возвращаемое значение).
   * @return Вектор цветов, представляющий изображение.
   */
  static std::vector<Color> loadImageToEigenArray(const std::string &filename,
                                                  int &width, int &height);
};
}  // namespace s21
#endif
