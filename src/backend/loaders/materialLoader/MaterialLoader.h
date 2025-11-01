#ifndef MATERIALLOADER_H
#define MATERIALLOADER_H

#include <string>

#include "backend/material_manager/material_manager.h"

namespace s21 {
/**
 * @class MtlFileLoader
 * @brief Класс для загрузки материалов из файлов .mtl.
 */
class MtlFileLoader {
 public:
  /**
   * @brief Загружает файл .mtl и добавляет материалы в MaterialManager.
   * @param filepath Путь к файлу .mtl.
   * @param materialManager Менеджер материалов, в который загружаются данные.
   */
  static void loadMtl(const std::string& filepath,
                      MaterialManager& materialManager);

 private:
  /**
   * @brief Закрытый конструктор, чтобы запретить создание экземпляров класса.
   */
  MtlFileLoader(){};
};
}  // namespace s21
#endif
