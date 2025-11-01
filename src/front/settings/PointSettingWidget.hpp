#pragma once

#include <QCheckBox>
#include <QColorDialog>
#include <QSpinBox>
#include <QWidget>

namespace s21 {
/**
 * @class PointSettingsWidget
 * @brief Виджет для настройки параметров точек.
 */
class PointSettingsWidget : public QWidget {
  Q_OBJECT
 public:
  /**
   * @brief Конструктор PointSettingsWidget.
   * @param parent Родительский виджет (по умолчанию nullptr).
   */
  explicit PointSettingsWidget(QWidget* parent = nullptr);

 signals:
  /**
   * @brief Сигнал, испускаемый при изменении настроек точек.
   * @param enabled Включено ли отображение точек.
   * @param color Цвет точек.
   * @param size Размер точек.
   * @param circulDot Включено ли круговое отображение точек.
   */
  void pointSettingsChanged(bool enabled, const QColor& color, int size,
                            bool circulDot);

 private:
  QCheckBox* enableCheckbox;  ///< Флажок включения отображения точек.
  QSpinBox* sizeSpinBox;  ///< Поле ввода размера точек.
  QColor pointColor;      ///< Цвет точек.
  QCheckBox* circulDot;  ///< Флажок включения кругового отображения точек.
};
}  // namespace s21