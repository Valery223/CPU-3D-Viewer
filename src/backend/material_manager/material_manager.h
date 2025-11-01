#ifndef MATERIAL_MANAGER_H
#define MATERIAL_MANAGER_H

#include <fstream>
#include <unordered_map>
#include <vector>

#include "backend/types.h"

namespace s21 {
/**
 * @struct Texture
 * @brief Структура, представляющая текстуру.
 */
struct Texture {
  std::vector<Color> colors_;  ///< Массив цветов текстуры.
  int width_;                  ///< Ширина текстуры.
  int height_;                 ///< Высота текстуры.
};

/**
 * @struct Material
 * @brief Структура, представляющая материал с параметрами освещения и
 * текстурой.
 */
struct Material {
  Eigen::Vector3f ambient;  ///< Фоновая (окружающая) составляющая освещения.
  Eigen::Vector3f diffuse;  ///< Диффузная составляющая освещения.
  Eigen::Vector3f specular;  ///< Зеркальная составляющая освещения.
  float shininess;  ///< Коэффициент блеска материала.

  Texture texture;  ///< Текстура, связанная с материалом.
};

/**
 * @class MaterialManager
 * @brief Класс для управления материалами и их загрузки.
 */
class MaterialManager {
 public:
  /**
   * @brief Загружает материал из файла.
   * @param filePath Путь к файлу материала.
   * @return Идентификатор загруженного материала.
   */
  uint32_t loadtMaterial(std::string filePath);

  /**
   * @brief Получает материал по его идентификатору.
   * @param idMaterial Идентификатор материала.
   * @return Константная ссылка на материал.
   */
  const Material& getMaterial(uint32_t idMaterial) const;

  /**
   * @brief Получает идентификатор материала по его имени.
   * @param name Имя материала.
   * @return Идентификатор материала.
   */
  uint32_t getMaterialId(std::string& name) const;

  /**
   * @brief Добавляет новый материал в менеджер.
   * @param name Имя материала.
   * @param material Объект материала для добавления.
   * @return Идентификатор добавленного материала.
   */
  uint32_t addMaterial(std::string& name, Material& material);

 private:
  std::vector<Material> materials_;  ///< Вектор всех загруженных материалов.
  std::unordered_map<std::string, uint32_t>
      mapMaterial_;  ///< Хеш-таблица для быстрого поиска материала по имени.
};
}  // namespace s21
#endif  // MATERIAL_MANAGER_H
