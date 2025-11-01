#pragma once

#include <QImage>
#include <string>

#include "backend/types.h"

namespace s21 {
/**
 * @class IController
 * @brief Интерфейс контроллера для управления сценой и рендерингом.
 */
class IController {
 public:
  /**
   * @brief Загружает объект в сцену.
   * @param filePath Путь к файлу объекта.
   */
  virtual void loadObject(const std::string& filePath) = 0;

  /**
   * @brief Получает изображение сцены.
   * @return Изображение сцены в формате QImage.
   */
  virtual QImage getImage() = 0;

  /**
   * @brief Обновляет модель сцены.
   */
  virtual void updateModel() = 0;

  /**
   * @brief Изменяет размер буферов рендеринга.
   * @param new_width Новая ширина.
   * @param new_height Новая высота.
   */
  virtual void resizeBuffers(int new_width, int new_height) = 0;

  /**
   * @brief Изменяет настройки рендеринга вершин.
   */
  virtual void changeRenderDotSetting(bool enable, Color color, int size,
                                      bool circulDot) = 0;

  /**
   * @brief Изменяет настройки рендеринга линий.
   */
  virtual void changeRenderLineSetting(bool enable, Color color,
                                       bool dashed) = 0;

  /**
   * @brief Изменяет настройки рендеринга граней.
   */
  virtual void changeRenderFaceSetting(bool enable, bool texture) = 0;

  /**
   * @brief Перемещает сцену в пространстве.
   */
  virtual void move(float x, float y, float z) = 0;
  virtual void moveUp(float angle = 1) = 0;
  virtual void moveDown(float angle = 1) = 0;
  virtual void moveLeft(float angle = 1) = 0;
  virtual void moveRight(float angle = 1) = 0;
  virtual void moveForward(float angle = 1) = 0;
  virtual void moveBackward(float angle = 1) = 0;

  /**
   * @brief Вращает сцену вокруг осей.
   */
  virtual void axiosRotate(float angleX, float angleY, float angleZ) = 0;
  virtual void axiosRotateYLeft(float angle = 175) = 0;
  virtual void axiosRotateXLeft(float angle = 175) = 0;
  virtual void axiosRotateZLeft(float angle = 175) = 0;

  virtual void axiosRotateYRight(float angle = 175) = 0;
  virtual void axiosRotateXRight(float angle = 175) = 0;
  virtual void axiosRotateZRight(float angle = 175) = 0;

  /**
   * @brief Изменяет масштаб сцены.
   */
  virtual void axiosScale(float x, float y, float z) = 0;
  virtual void axiosScaleLess(float scaleValue = 1.01) = 0;
  virtual void axiosScaleMore(float scaleValue = 0.99) = 0;

  /**
   * @brief Виртуальный деструктор.
   */
  virtual ~IController() = default;
};
}  // namespace s21
