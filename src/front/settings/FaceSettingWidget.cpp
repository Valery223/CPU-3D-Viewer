#include "FaceSettingWidget.hpp"

#include <QPushButton>
#include <QVBoxLayout>

namespace s21 {
FaceSettingsWidget::FaceSettingsWidget(QWidget* parent) {
  QVBoxLayout* layout = new QVBoxLayout(this);

  enableCheckbox = new QCheckBox("Enable Faces", this);
  enableCheckbox->setChecked(true);
  layout->addWidget(enableCheckbox);

  texturesCheckbox = new QCheckBox("Enable Texture", this);
  texturesCheckbox->setChecked(true);
  layout->addWidget(texturesCheckbox);

  connect(enableCheckbox, &QCheckBox::toggled, this,
          &FaceSettingsWidget::updateFaceSettings);
  connect(texturesCheckbox, &QCheckBox::toggled, this,
          &FaceSettingsWidget::updateFaceSettings);
}

void FaceSettingsWidget::updateFaceSettings() {
  emit faceSettingsChanged(enableCheckbox->isChecked(),
                           texturesCheckbox->isChecked());
}
}  // namespace s21