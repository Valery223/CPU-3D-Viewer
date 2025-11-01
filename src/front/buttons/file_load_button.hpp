#ifndef FILE_LOAD_BUTTON_H
#define FILE_LOAD_BUTTON_H

#include <QFileDialog>
#include <QPushButton>
#include <QString>

namespace s21 {
/**
 * @class FileLoadButton
 * @brief Кнопка для выбора и загрузки файла через диалоговое окно.
 */
class FileLoadButton : public QPushButton {
  Q_OBJECT
 public:
  /**
   * @brief Конструктор FileLoadButton.
   * @param parent Родительский виджет (по умолчанию nullptr).
   */
  explicit FileLoadButton(QWidget *parent = nullptr);

 signals:
  /**
   * @brief Сигнал, испускаемый при выборе файла.
   * @param filePath Путь к выбранному файлу.
   */
  void fileSelected(const QString &filePath);

 private slots:
  /**
   * @brief Слот, вызываемый при нажатии на кнопку.
   */
  void onButtonClicked();
};
}  // namespace s21
#endif  // FILE_LOAD_BUTTON_H