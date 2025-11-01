#ifndef MAIN_WIDGET_HPP
#define MAIN_WIDGET_HPP

#include <QKeyEvent>
#include <QObject>
#include <QWidget>

#include "ViewerWidget.hpp"
#include "buttons/ControlGroupWidget.hpp"
#include "buttons/file_load_button.hpp"
#include "controller/IController.hpp"
#include "settings/FaceSettingWidget.hpp"
#include "settings/LineSettingWidget.hpp"
#include "settings/PointSettingWidget.hpp"

namespace s21 {
/**
 * @class MainWidget
 * @brief Главный виджет приложения, содержащий элементы управления и область
 * просмотра.
 */
class MainWidget : public QWidget {
  Q_OBJECT
 public:
  /**
   * @brief Конструктор MainWidget.
   * @param controller Указатель на контроллер.
   * @param parent Родительский виджет (по умолчанию nullptr).
   */
  MainWidget(IController* controller, QWidget* parent = nullptr);

  /**
   * @brief Обрабатывает нажатия клавиш.
   * @param event Указатель на событие клавиатуры.
   */
  void keyPressEvent(QKeyEvent* event) override;

 private:
  /**
   * @brief Устанавливает пользовательский интерфейс.
   */
  void setupUi();

  /**
   * @brief Устанавливает соединения сигналов и слотов.
   */
  void setupConnections();

 private:
  ViewerWidget* viewer;     ///< Виджет отображения сцены.
  FileLoadButton* loadBtn;  ///< Кнопка загрузки файла.
  ControlGroupWidget*
      moveControl;  ///< Группа элементов управления перемещением.
  ControlGroupWidget*
      rotateControl;  ///< Группа элементов управления вращением.
  ControlGroupWidget*
      scaleControl;  ///< Группа элементов управления масштабированием.
  PointSettingsWidget* pointSettingWidget;  ///< Виджет настроек точек.
  LineSettingsWidget* lineSettingWidget;  ///< Виджет настроек линий.
  FaceSettingsWidget* faceSettingWidget;  ///< Виджет настроек граней.

  IController* m_controller;  ///< Указатель на контроллер.
};
}  // namespace s21
#endif