#pragma once

#include <QKeyEvent>
#include <QWidget>

#include "controller/IController.hpp"

namespace s21 {
/**
 * @class ViewerWidget
 * @brief Виджет для отображения 3D-сцены с возможностью взаимодействия.
 */
class ViewerWidget : public QWidget {
  Q_OBJECT

 public:
  /**
   * @brief Конструктор ViewerWidget.
   * @param controller Указатель на контроллер.
   * @param width Ширина виджета (по умолчанию 800).
   * @param height Высота виджета (по умолчанию 600).
   * @param parent Родительский виджет (по умолчанию nullptr).
   */
  ViewerWidget(IController* controller, int width = 800, int height = 600,
               QWidget* parent = nullptr);

 protected:
  /**
   * @brief Обрабатывает событие перерисовки виджета.
   * @param event Указатель на событие рисования.
   */
  void paintEvent(QPaintEvent*) override;

  /**
   * @brief Обрабатывает событие изменения размера виджета.
   * @param event Указатель на событие изменения размера.
   */
  void resizeEvent(QResizeEvent* event) override;

  /**
   * @brief Обрабатывает нажатия клавиш.
   * @param event Указатель на событие клавиатуры.
   */
  void keyPressEvent(QKeyEvent* event) override;

 private slots:
  /**
   * @brief Обновляет сцену.
   */
  void updateScene();

 public:
 signals:
  /**
   * @brief Сигнал, испускаемый при обновлении модели.
   */
  void modelUpdated();

 private:
  IController* m_controller;  ///< Указатель на контроллер.
  QImage m_image;  ///< Изображение, отображаемое в виджете.
  QTimer* timer;  ///< Таймер для периодического обновления сцены.
};
}  // namespace s21