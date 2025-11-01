#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "backend/camera/camera.h"
#include "backend/material_manager/material_manager.h"
#include "backend/object/object.h"

namespace s21 {
/**
 * @class Scene
 * @brief Класс, представляющий 3D-сцену с объектами, камерами и освещением.
 */
class Scene {
 public:
  /**
   * @brief Конструктор сцены.
   */
  Scene();

 public:
  /**
   * @brief Выполняет рендеринг сцены.
   */
  void render();

 public:
  /**
   * @brief Загружает объект из файла.
   * @param filepath Путь к файлу объекта.
   */
  void loadObject(std::string filepath);

  /**
   * @brief Обновляет материал объекта.
   */
  void UpdateObjectMaterial();

  /**
   * @brief Обновляет текстуру объекта.
   */
  void updateObjectTexture();

  /**
   * @brief Добавляет объект в сцену.
   * @param obj Объект для добавления.
   */
  void addObject(Object &obj);

  /**
   * @brief Возвращает ссылку на список объектов сцены.
   * @return Вектор объектов сцены.
   */
  std::vector<Object> &getObjects();

  /**
   * @brief Возвращает источник света по индексу.
   * @param index Индекс источника света.
   * @return Константная ссылка на источник света.
   */
  const Light &getLight(uint32_t index) const;

  /**
   * @brief Возвращает текущую активную камеру.
   * @return Указатель на текущую камеру.
   */
  Camera *getCurrentCamera();

  /**
   * @brief Возвращает материал по его идентификатору.
   * @param idMaterial Идентификатор материала.
   * @return Константная ссылка на материал.
   */
  const Material &getMaterial(uint32_t idMaterial) const;

 private:
  std::vector<Object> objects;  ///< Список объектов сцены.
  MaterialManager materialManager;  ///< Менеджер материалов сцены.
  std::vector<Camera> cameras;  ///< Список камер сцены.
  Camera *currentCamera;  ///< Указатель на текущую активную камеру.
  std::vector<Light> lights;  ///< Список источников света.
};
}  // namespace s21
#endif  // SCENE_H