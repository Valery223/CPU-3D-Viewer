#include "file_load_button.hpp"

namespace s21 {
FileLoadButton::FileLoadButton(QWidget *parent)
    : QPushButton(tr("Load Object"), parent) {
  connect(this, &QPushButton::clicked, this, &FileLoadButton::onButtonClicked);
}

void FileLoadButton::onButtonClicked() {
  QString initialDirectory = "../src/objects_files";
  QString filePath = QFileDialog::getOpenFileName(
      this, tr("Open Object File"), initialDirectory,
      tr("Object Files (*.obj *.stl);;All Files (*)"));

  if (!filePath.isEmpty()) {
    emit fileSelected(filePath);
  }
}
}  // namespace s21