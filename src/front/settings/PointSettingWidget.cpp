#include "PointSettingWidget.hpp"

#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QVBoxLayout>

namespace s21 {
PointSettingsWidget::PointSettingsWidget(QWidget* parent)
    : QWidget(parent), pointColor(Qt::white) {
  QVBoxLayout* layout = new QVBoxLayout(this);

  enableCheckbox = new QCheckBox("Enable Points", this);
  enableCheckbox->setChecked(true);
  layout->addWidget(enableCheckbox);

  QPushButton* colorButton = new QPushButton("Choose Color", this);
  layout->addWidget(colorButton);

  QLabel* sizeLabel = new QLabel("Point Size:", this);
  sizeSpinBox = new QSpinBox(this);
  sizeSpinBox->setRange(1, 10);
  sizeSpinBox->setValue(3);
  layout->addWidget(sizeLabel);
  layout->addWidget(sizeSpinBox);

  circulDot = new QCheckBox("Circul Dot", this);
  circulDot->setChecked(true);
  layout->addWidget(circulDot);

  connect(enableCheckbox, &QCheckBox::toggled, this, [this](bool checked) {
    emit pointSettingsChanged(checked, pointColor, sizeSpinBox->value(),
                              circulDot->isChecked());
  });

  connect(colorButton, &QPushButton::clicked, this, [this]() {
    QColor color =
        QColorDialog::getColor(pointColor, this, "Select Point Color");
    if (color.isValid()) {
      pointColor = color;
      emit pointSettingsChanged(enableCheckbox->isChecked(), pointColor,
                                sizeSpinBox->value(), circulDot->isChecked());
    }
  });

  connect(sizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this,
          [this](int value) {
            emit pointSettingsChanged(enableCheckbox->isChecked(), pointColor,
                                      value, circulDot->isChecked());
          });

  connect(circulDot, &QCheckBox::toggled, this, [this](bool checked) {
    emit pointSettingsChanged(enableCheckbox->isChecked(), pointColor,
                              sizeSpinBox->value(), checked);
  });
}
}  // namespace s21