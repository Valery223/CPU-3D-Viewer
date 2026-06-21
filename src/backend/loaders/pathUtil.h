#ifndef LOADERS_PATH_UTIL_H
#define LOADERS_PATH_UTIL_H

#include <filesystem>
#include <string>

namespace s21 {

// Ищет файл, на который ссылается .obj/.mtl. Экспортёры часто пишут чужие
// абсолютные пути (бывает и в Windows-виде C:\...\tex.tga), поэтому если по
// записанному пути ничего нет, отбрасываем каталог и берём файл рядом с
// исходником. baseDir — папка ссылающегося файла. Если не нашли, возвращаем
// baseDir/rawRef, чтобы вызывающий мог осмысленно ругнуться.
inline std::string resolveAssetPath(const std::filesystem::path& baseDir,
                                    const std::string& rawRef) {
  namespace fs = std::filesystem;
  std::error_code ec;

  if (fs::exists(rawRef, ec)) return rawRef;

  fs::path joined = baseDir / rawRef;
  if (fs::exists(joined, ec)) return joined.string();

  size_t sep = rawRef.find_last_of("/\\");  // и unix, и windows разделитель
  std::string name = (sep == std::string::npos) ? rawRef : rawRef.substr(sep + 1);
  fs::path beside = baseDir / name;
  if (fs::exists(beside, ec)) return beside.string();

  return joined.string();
}

}  // namespace s21

#endif  // LOADERS_PATH_UTIL_H
