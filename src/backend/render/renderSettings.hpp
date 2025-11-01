#pragma once

#include <QImage>
#include <fstream>
#include <iostream>

#include "backend/types.h"

namespace s21 {
/**
 * @struct RenderSettings
 * @brief Структура, содержащая параметры рендеринга.
 */
struct RenderSettings {
  bool renderDot = true;  ///< Флаг рендеринга вершин.
  bool cirul = true;  ///< Опция, связанная с рендерингом (непонятное название).
  Color vertexColor = Color(0, 0, 211);  ///< Цвет вершин.
  int vertexSize = 5;                    ///< Размер вершины.

  bool renderLine = true;  ///< Флаг рендеринга линий.
  Color lineColor = Color(0, 211, 0);  ///< Цвет линий.
  bool lineDash = true;  ///< Флаг для пунктирных линий.

  Qt::GlobalColor fon_color = Qt::white;  ///< Цвет фона.

  bool renderFace = true;  ///< Флаг рендеринга граней.
  bool texture = true;     ///< Флаг рендеринга текстур.

  /**
   * @brief Сохраняет настройки рендеринга в файл.
   * @param filename Имя файла для сохранения.
   * @return true, если успешно сохранено, иначе false.
   */
  bool saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
      std::cerr << "Failed to open file for writing: " << filename << std::endl;
      return false;
    }

    file << renderDot << " " << vertexColor.x() << " " << vertexColor.y() << " "
         << vertexColor.z() << " " << vertexSize << " " << renderLine << " "
         << lineColor.x() << " " << lineColor.y() << " " << lineColor.z() << " "
         << renderFace << " " << texture << "\n";

    file.close();
    return true;
  }

  /**
   * @brief Загружает настройки рендеринга из файла.
   * @param filename Имя файла для загрузки.
   * @return true, если успешно загружено, иначе false.
   */
  bool loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
      std::cerr << "Failed to open file for reading: " << filename << std::endl;
      return false;
    }

    file >> renderDot >> vertexColor.x() >> vertexColor.y() >>
        vertexColor.z() >> vertexSize >> renderLine >> lineColor.x() >>
        lineColor.y() >> lineColor.z() >> renderFace >> texture;

    file.close();
    return true;
  }
};
}  // namespace s21