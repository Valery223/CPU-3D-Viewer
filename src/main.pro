AINCLUDEPATH += /opt/Qt6/include
QT += core gui widgets

CONFIG += c++17

SOURCES += main.cpp

#frontend
SOURCES += \
        front/MainWidget.cpp \
        front/buttons/ControlGroupWidget.cpp \
        front/buttons/file_load_button.cpp \
        front/ViewerWidget.cpp \
        front/settings/PointSettingWidget.cpp \
        front/settings/LineSettingWidget.cpp \
        front/settings/FaceSettingWidget.cpp \

#backend
SOURCES += \
        backend/loaders/objectLoader/ObjectLoader.cpp \
        backend/loaders/materialLoader/MaterialLoader.cpp \
        backend/loaders/textureLoader/TextureLoader.cpp \
        backend/material_manager/material_manager.cpp \
        backend/object/object.cpp \
        backend/mesh/mesh.cpp \
        backend/transform/transform.cpp \
        backend/scene/scene.cpp \
        backend/render/renderRasterize.cpp \

#other
SOURCES += \
        controller/controller.cpp \

#frontend
HEADERS += \
        front/MainWidget.hpp \
        front/buttons/ControlGroupWidget.hpp \
        front/buttons/file_load_button.hpp \
        front/ViewerWidget.hpp \
        front/settings/PointSettingWidget.hpp \
        front/settings/LineSettingWidget.hpp \
        front/settings/FaceSettingWidget.hpp \

#backend
HEADERS += \
        backend/render/renderSettings.hpp \
        backend/camera/camera.h

#other
HEADERS += \
        controller/IController.hpp \
        controller/MockController.h \
        stb_image.h \


QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS += -fopenmp


# Установка флагов для релизной сборки
QMAKE_CXXFLAGS_RELEASE += -O2  # Оптимизация для релизной сборки
QMAKE_LFLAGS_RELEASE += -Wl,--strip-all  # Убрать отладочную информацию из релизной сборки

# Установка флагов для отладочной сборки
QMAKE_CXXFLAGS_DEBUG += -g  # Включить отладочную информацию
QMAKE_LFLAGS_DEBUG +=  # Можно добавить флаги для отладочной сборки, если необходимо


# Переменные для тестов
TEST_TARGET = test_binary
TEST_SOURCES = tests/*.cpp backend/transform/transform.cpp
TEST_LIBS = -lgtest -lgtest_main -pthread

# Настройки сборки тестов
test.commands = $(CXX) $(CXXFLAGS) -I . $$TEST_SOURCES $$TEST_LIBS -o $$TEST_TARGET && ./$$TEST_TARGET
test.target = test
QMAKE_EXTRA_TARGETS += test
QMAKE_CLEAN += $$TEST_TARGET

# Стадия dist (создание архива проекта)
DIST_NAME = my_project
DIST_FILES = $$FILES # Автоматически берет все файлы из проекта
dist.commands = tar -czf $$DIST_NAME.tar.gz $$DIST_FILES
dist.target = dist

# Стадия dvi (создание документации)
dvi.commands = doxygen Doxyfile
dvi.target = dvi

# Стадия install (установка в /usr/local/bin)
INSTALL_PATH = bin
install.commands = install -m 755 $$TARGET $$INSTALL_PATH/
install.target = install

# Стадия uninstall (удаление установленного файла)
uninstall.commands = rm -f $$INSTALL_PATH/$$TARGET
uninstall.target = uninstall

# Добавляем кастомные стадии
QMAKE_EXTRA_TARGETS += test dist dvi install uninstall
QMAKE_CLEAN += $$TEST_TARGET $$DIST_NAME.tar.gz