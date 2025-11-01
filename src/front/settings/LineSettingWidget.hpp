#pragma once

#include <QCheckBox>
#include <QColorDialog>
#include <QWidget>

namespace s21 {
/**
 * @class LineSettingsWidget
 * @brief Виджет для настройки параметров линий.
 */
class LineSettingsWidget : public QWidget {
  Q_OBJECT
 public:
  /**
   * @brief Конструктор LineSettingsWidget.
   * @param parent Родительский виджет (по умолчанию nullptr).
   */
  explicit LineSettingsWidget(QWidget* parent = nullptr);

 signals:
  /**
   * @brief Сигнал, испускаемый при изменении настроек линий.
   * @param enabled Включено ли отображение линий.
   * @param color Цвет линий.
   * @param isDashed Включены ли пунктирные линии.
   */
  void lineSettingsChanged(bool enabled, const QColor& color, bool isDashed);

 private slots:
  /**
   * @brief Обновляет настройки линий при изменении флажков.
   */
  void updateLineSettings();

 private:
  QCheckBox* enableCheckbox;  ///< Флажок включения отображения линий.
  QColor color;  ///< Цвет линий.
  QCheckBox* dashedCheckbox;  ///< Флажок включения пунктирных линий.
};
}  // namespace s21
