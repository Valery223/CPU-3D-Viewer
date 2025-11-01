#pragma once

#include <QDoubleSpinBox>
#include <QPushButton>
#include <QVector>
#include <QWidget>

namespace s21 {
/**
 * @class ControlGroupWidget
 * @brief Виджет управления группой значений с кнопкой и числовыми полями ввода.
 */
class ControlGroupWidget : public QWidget {
  Q_OBJECT
 public:
  /**
   * @brief Конструктор ControlGroupWidget.
   * @param buttonText Текст кнопки.
   * @param labelNames Список названий для числовых полей.
   * @param parent Родительский виджет (по умолчанию nullptr).
   */
  ControlGroupWidget(const QString &buttonText, const QStringList &labelNames,
                     QWidget *parent = nullptr);

  /**
   * @brief Получает значения из числовых полей ввода.
   * @return Вектор значений.
   */
  QVector<double> getValues() const;

 signals:
  /**
   * @brief Сигнал, отправляющий значения числовых полей.
   * @param values Вектор значений.
   */
  void controlValues(const QVector<double> &values);

 private:
  QPushButton *m_button;  ///< Кнопка управления.
  QVector<QDoubleSpinBox *> m_spinBoxes;  ///< Вектор числовых полей ввода.
};
}  // namespace s21