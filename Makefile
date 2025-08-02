# Makefile para compilar y ejecutar tests
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
COVERAGE_FLAGS = --coverage -fprofile-arcs -ftest-coverage
LIBS = -lcjson -lpthread -lm

# Directorios
SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build
COVERAGE_DIR = coverage

# Archivos fuente principales (sin main.c para evitar conflictos)
SOURCES = $(filter-out $(SRC_DIR)/main.c, $(wildcard $(SRC_DIR)/*.c))
TEST_SOURCES = $(wildcard $(TEST_DIR)/*.c)

# Crear directorio de build si no existe
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(COVERAGE_DIR):
	mkdir -p $(COVERAGE_DIR)

# Compilar tests con coverage
test: $(BUILD_DIR) $(COVERAGE_DIR)
	@echo "Compilando tests con coverage..."
	$(CC) $(CFLAGS) $(COVERAGE_FLAGS) -I$(SRC_DIR) -Iinclude -o $(BUILD_DIR)/test_commands $(TEST_DIR)/test_commands.c $(LIBS)
	$(CC) $(CFLAGS) $(COVERAGE_FLAGS) -I$(SRC_DIR) -Iinclude -o $(BUILD_DIR)/test_shell $(TEST_DIR)/test_shell.c $(LIBS)

# Ejecutar tests
run-tests: test
	@echo "Ejecutando tests..."
	./$(BUILD_DIR)/test_commands
	./$(BUILD_DIR)/test_shell

# Generar reporte de coverage
coverage: run-tests
	@echo "Generando reporte de coverage..."
	gcov -r $(SRC_DIR)/*.c
	lcov --capture --directory . --output-file $(COVERAGE_DIR)/coverage.info
	lcov --remove $(COVERAGE_DIR)/coverage.info '/usr/*' --output-file $(COVERAGE_DIR)/coverage.info.cleaned
	genhtml $(COVERAGE_DIR)/coverage.info.cleaned --output-directory $(COVERAGE_DIR)/html
	@echo "Reporte de coverage generado en $(COVERAGE_DIR)/html/index.html"

# Limpiar archivos generados
clean:
	rm -rf $(BUILD_DIR) $(COVERAGE_DIR)
	rm -f *.gcov *.gcda *.gcno

# Compilar el proyecto principal
build: $(BUILD_DIR)
	$(CC) $(CFLAGS) -Iinclude $(SOURCES) $(SRC_DIR)/main.c -o $(BUILD_DIR)/shell $(LIBS)

.PHONY: test run-tests coverage clean build