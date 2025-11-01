#pragma once
#include <QImage>

#include "IController.hpp"

namespace s21 {
/**
 * @class MockController
 * @brief Мок-имплементация интерфейса IController для тестирования.
 */
class MockController : public IController {
 public:
  /**
   * @brief Заглушка метода загрузки объекта.
   */
  void loadObject(const std::string&) override {}

  /**
   * @brief Возвращает тестовое изображение 100x100 пикселей.
   * @return QImage тестового размера.
   */
  QImage getImage() override { return QImage(100, 100, QImage::Format_ARGB32); }

  /**
   * @brief Заглушка метода обновления модели.
   */
  void updateModel() override {}

  /**
   * @brief Заглушка метода изменения размера буфера.
   */
  void resizeBuffers(int, int) override {}

  /**
   * @brief Заглушка методов перемещения объекта.
   */
  void move(float, float, float) override {}
  void moveUp(float) override {}
  void moveDown(float) override {}
  void moveLeft(float) override {}
  void moveRight(float) override {}
  void moveForward(float) override {}
  void moveBackward(float) override {}

  /**
   * @brief Заглушка методов вращения объекта.
   */
  void axiosRotate(float, float, float) override {}
  void axiosRotateYLeft(float) override {}
  void axiosRotateXLeft(float) override {}
  void axiosRotateZLeft(float) override {}
  void axiosRotateYRight(float) override {}
  void axiosRotateXRight(float) override {}
  void axiosRotateZRight(float) override {}

  /**
   * @brief Заглушка методов масштабирования объекта.
   */
  void axiosScale(float, float, float) override {}
  void axiosScaleLess(float) override {}
  void axiosScaleMore(float) override {}

  /**
   * @brief Виртуальный деструктор.
   */
  ~MockController() override = default;
};
}  // namespace s21