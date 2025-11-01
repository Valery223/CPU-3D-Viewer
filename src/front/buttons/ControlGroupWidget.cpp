#include "ControlGroupWidget.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

namespace s21 {
ControlGroupWidget::ControlGroupWidget(const QString& buttonText,
                                       const QStringList& labelNames,
                                       QWidget* parent)
    : QWidget(parent) {
  setFixedHeight(170);
  QVBoxLayout* mainLayout = new QVBoxLayout(this);

  for (const QString& lableName : labelNames) {
    QHBoxLayout* rowLayout = new QHBoxLayout();

    QLabel* label = new QLabel(lableName, this);
    rowLayout->addWidget(label);

    QDoubleSpinBox* spinBox = new QDoubleSpinBox(this);
    spinBox->setRange(-1000.0, 1000.0);
    spinBox->setSingleStep(0.1);
    spinBox->setDecimals(2);
    m_spinBoxes.append(spinBox);
    rowLayout->addWidget(spinBox);

    mainLayout->addLayout(rowLayout);
  }

  m_button = new QPushButton(buttonText, this);
  mainLayout->addWidget(m_button);

  connect(m_button, &QPushButton::clicked, this,
          [this]() { emit controlValues(getValues()); });
}

QVector<double> ControlGroupWidget::getValues() const {
  QVector<double> values;
  for (const QDoubleSpinBox* spinBox : m_spinBoxes) {
    values.append(spinBox->value());
  }

  return values;
}
}  // namespace s21