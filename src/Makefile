# 3DViewer (Qt6, софтверный рендер на CPU). Сборка без qmake; пути к Qt берём
# из pkg-config. NDEBUG обязателен: иначе eigen_assert душит матричные операции.
# march=native делает бинарь непереносимым, но это локальная сборка.

CXX      ?= g++
CXXSTD   := -std=c++17
OPT      := -O2 -DNDEBUG -march=native
WARN     := -Wall -Wextra
OMP      := -fopenmp

# --- Флаги Qt6: pkg-config Qt6Widgets тянет за собой Gui и Core ----------------
QT_CFLAGS := $(shell pkg-config --cflags Qt6Widgets)
QT_LIBS   := $(shell pkg-config --libs   Qt6Widgets)

# --- moc (Meta-Object Compiler): путь зависит от дистрибутива.
#     Спрашиваем у qmake, с фолбэком на стандартное место Arch. ----------------
MOC := $(shell qmake6 -query QT_HOST_LIBEXECS 2>/dev/null)/moc
ifeq ($(wildcard $(MOC)),)
  MOC := /usr/lib/qt6/moc
endif

# -MMD -MP — автогенерация зависимостей по заголовкам (.d рядом с .o):
# правка любого #include'нутого .h пересобирает всех его потребителей.
CXXFLAGS := $(CXXSTD) $(OPT) $(WARN) $(OMP) -fPIC -MMD -MP $(QT_CFLAGS) -I.
LDFLAGS  := $(OMP) $(QT_LIBS)

BUILD  := build
TARGET := main

# --- Исходники (список 1-в-1 из main.pro) -----------------------------------
SOURCES := \
  main.cpp \
  front/MainWidget.cpp \
  front/buttons/ControlGroupWidget.cpp \
  front/buttons/file_load_button.cpp \
  front/ViewerWidget.cpp \
  front/settings/PointSettingWidget.cpp \
  front/settings/LineSettingWidget.cpp \
  front/settings/FaceSettingWidget.cpp \
  backend/loaders/objectLoader/ObjectLoader.cpp \
  backend/loaders/materialLoader/MaterialLoader.cpp \
  backend/loaders/textureLoader/TextureLoader.cpp \
  backend/material_manager/material_manager.cpp \
  backend/object/object.cpp \
  backend/mesh/mesh.cpp \
  backend/transform/transform.cpp \
  backend/scene/scene.cpp \
  backend/render/renderRasterize.cpp \
  controller/controller.cpp

# --- Заголовки с Q_OBJECT → им нужен moc ------------------------------------
MOC_HEADERS := \
  front/MainWidget.hpp \
  front/buttons/ControlGroupWidget.hpp \
  front/buttons/file_load_button.hpp \
  front/ViewerWidget.hpp \
  front/settings/PointSettingWidget.hpp \
  front/settings/LineSettingWidget.hpp \
  front/settings/FaceSettingWidget.hpp

# moc генерирует build/moc_<имя>.cpp (базовые имена здесь уникальны)
MOC_SRCS := $(patsubst %,$(BUILD)/moc_%.cpp,$(notdir $(basename $(MOC_HEADERS))))
MOC_OBJS := $(MOC_SRCS:.cpp=.o)

# обычные объектники складываем в build/, сохраняя дерево каталогов
OBJS := $(SOURCES:%.cpp=$(BUILD)/%.o)

# чтобы правило moc находило заголовок по одному лишь имени
vpath %.hpp $(sort $(dir $(MOC_HEADERS)))

# ============================================================================
.PHONY: all run test clean dist dvi install uninstall
all: $(TARGET)

$(TARGET): $(OBJS) $(MOC_OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

# компиляция обычных исходников
$(BUILD)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# генерация moc-исходника из заголовка с Q_OBJECT
$(BUILD)/moc_%.cpp: %.hpp
	@mkdir -p $(BUILD)
	$(MOC) $< -o $@

# компиляция moc-исходников
$(BUILD)/moc_%.o: $(BUILD)/moc_%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

# --- Юнит-тесты (gtest), как в main.pro: только transform + Eigen, без Qt ----
test:
	@mkdir -p $(BUILD)
	$(CXX) $(CXXSTD) -I. tests/main_test.cpp backend/transform/transform.cpp \
	    -lgtest -lgtest_main -pthread -o $(BUILD)/test_binary
	./$(BUILD)/test_binary

dvi:
	doxygen Doxyfile

dist: all
	tar -czf 3dviewer.tar.gz $(SOURCES) $(MOC_HEADERS) Makefile main.pro README.md

install: all
	install -d $(DESTDIR)/usr/local/bin
	install -m 755 $(TARGET) $(DESTDIR)/usr/local/bin/

uninstall:
	rm -f $(DESTDIR)/usr/local/bin/$(TARGET)

clean:
	rm -rf $(BUILD) $(TARGET) 3dviewer.tar.gz

# Подключаем сгенерированные файлы зависимостей (если уже есть).
-include $(OBJS:.o=.d) $(MOC_OBJS:.o=.d)
