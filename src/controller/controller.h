#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QImage>
#include <QPainter>

#include "IController.hpp"
#include "backend/render/irender.h"
#include "backend/scene/scene.h"

namespace s21 {
/**
 * @class Controller
 * @brief Контроллер для управления сценой и рендерингом.
 */
class Controller : public IController {
 public:
  /**
   * @brief Конструктор контроллера.
   * @param scene Указатель на сцену.
   * @param render Указатель на рендерер.
   */
  Controller(Scene* scene, IRender* render) : scene(scene), render(render) {}

  /**
   * @brief Загружает объект в сцену.
   * @param filePath Путь к файлу объекта.
   */
  void loadObject(const std::string& filePath) { scene->loadObject(filePath); }

  /**
   * @brief Получает изображение сцены.
   * @return Изображение сцены в формате QImage.
   */
  QImage getImage();

  /**
   * @brief Обновляет модель сцены.
   */
  void updateModel();

  /**
   * @brief Изменяет размер буферов рендеринга и обновляет соотношение сторон
   * камеры.
   * @param new_width Новая ширина буфера.
   * @param new_height Новая высота буфера.
   */
  void resizeBuffers(int new_width, int new_height) {
    render->resizeBuffers(new_width, new_height);
    float new_aspect_ratio =
        static_cast<float>(float(new_width) / float(new_height));
    scene->getCurrentCamera()->setAspectRatio(new_aspect_ratio);
  }

  /**
   * @brief Изменяет настройки рендеринга вершин.
   */
  void changeRenderDotSetting(bool enable, Color color, int size,
                              bool circulDot);

  /**
   * @brief Изменяет настройки рендеринга линий.
   */
  void changeRenderLineSetting(bool enable, Color color, bool dashed);

  /**
   * @brief Изменяет настройки рендеринга граней.
   */
  void changeRenderFaceSetting(bool enable, bool texture);

  /**
   * @brief Перемещает сцену в пространстве.
   */
  void move(float x, float y, float z);

  /** @brief Двигает камеру вверх. */
  void moveUp(float angle = 1);
  /** @brief Двигает камеру вниз. */
  void moveDown(float angle = 1);
  /** @brief Двигает камеру влево. */
  void moveLeft(float angle = 1);
  /** @brief Двигает камеру вправо. */
  void moveRight(float angle = 1);
  /** @brief Двигает камеру вперед. */
  void moveForward(float angle = 1);
  /** @brief Двигает камеру назад. */
  void moveBackward(float angle = 1);

  /**
   * @brief Выполняет вращение сцены вокруг осей.
   */
  void axiosRotate(float angleX, float angleY, float angleZ);

  /** @brief Вращает вокруг оси Y влево. */
  void axiosRotateYLeft(float angle = 175);
  /** @brief Вращает вокруг оси X влево. */
  void axiosRotateXLeft(float angle = 175);
  /** @brief Вращает вокруг оси Z влево. */
  void axiosRotateZLeft(float angle = 175);

  /** @brief Вращает вокруг оси Y вправо. */
  void axiosRotateYRight(float angle = 175);
  /** @brief Вращает вокруг оси X вправо. */
  void axiosRotateXRight(float angle = 175);
  /** @brief Вращает вокруг оси Z вправо. */
  void axiosRotateZRight(float angle = 175);

  /**
   * @brief Изменяет масштаб сцены.
   */
  void axiosScale(float x, float y, float z);

  /** @brief Уменьшает масштаб сцены. */
  void axiosScaleLess(float scaleValue = 1.01);
  /** @brief Увеличивает масштаб сцены. */
  void axiosScaleMore(float scaleValue = 0.99);

 private:
  IRender* render;  ///< Указатель на рендерер.
  Scene* scene;     ///< Указатель на сцену.
};
}  // namespace s21
#endif  // CONTROLLER_H
