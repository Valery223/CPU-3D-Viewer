#pragma once

#include <QCheckBox>
#include <QWidget>

namespace s21 {
/**
 * @class FaceSettingsWidget
 * @brief Виджет для настройки параметров отображения граней.
 */
class FaceSettingsWidget : public QWidget {
  Q_OBJECT
 public:
  /**
   * @brief Конструктор FaceSettingsWidget.
   * @param parent Родительский виджет (по умолчанию nullptr).
   */
  explicit FaceSettingsWidget(QWidget* parent = nullptr);

 signals:
  /**
   * @brief Сигнал, испускаемый при изменении настроек отображения граней.
   * @param enabled Включено ли отображение граней.
   * @param texturesEnabled Включены ли текстуры.
   */
  void faceSettingsChanged(bool enabled, bool texturesEnabled);

 private slots:
  /**
   * @brief Обновляет настройки отображения граней при изменении флажков.
   */
  void updateFaceSettings();

 private:
  QCheckBox* enableCheckbox;  ///< Флажок включения отображения граней.
  QCheckBox* texturesCheckbox;  ///< Флажок включения текстур.
};
}  // namespace s21