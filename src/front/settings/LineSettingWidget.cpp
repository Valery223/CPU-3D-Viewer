#include "LineSettingWidget.hpp"

#include <QPushButton>
#include <QVBoxLayout>

namespace s21 {
LineSettingsWidget::LineSettingsWidget(QWidget* parent)
    : QWidget(parent), color(Qt::black) {
  QVBoxLayout* layout = new QVBoxLayout(this);

  enableCheckbox = new QCheckBox("Enable Lines", this);
  enableCheckbox->setChecked(true);
  layout->addWidget(enableCheckbox);

  QPushButton* colorButton = new QPushButton("Choose Color", this);
  layout->addWidget(colorButton);

  dashedCheckbox = new QCheckBox("Dashed ", this);
  dashedCheckbox->setChecked(true);
  layout->addWidget(dashedCheckbox);

  connect(enableCheckbox, &QCheckBox::toggled, this,
          &LineSettingsWidget::updateLineSettings);
  connect(dashedCheckbox, &QCheckBox::toggled, this,
          &LineSettingsWidget::updateLineSettings);
  connect(colorButton, &QPushButton::clicked, this, [this]() {
    QColor newColor = QColorDialog::getColor(color, this);
    if (newColor.isValid()) {
      color = newColor;
      emit lineSettingsChanged(enableCheckbox->isChecked(), color,
                               dashedCheckbox->isChecked());
    }
  });
}

void LineSettingsWidget::updateLineSettings() {
  emit lineSettingsChanged(enableCheckbox->isChecked(), color,
                           dashedCheckbox->isChecked());
}
}  // namespace s21