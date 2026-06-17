# Backup System

## Краткая суть проекта

Приложение для резервного копирования файлов с поддержкой полного и инкрементального бэкапа, сжатия (GZIP), планировщика задач и политик хранения (ограничение по размеру и времени). Вся документация по проекту располагается в файле Техническое задание.docx.

## Используемые сторонние библиотеки

- **Qt6** (Widgets, Concurrent) — GUI и многопоточность
- **ZLIB** — сжатие данных

## Инструкция по сборке

### Требования

- CMake ≥ 3.16
- C++17 компилятор
- Qt6
- ZLIB

### Сборка

```bash
cmake -S . -B build
cmake --build build
```

### Пример запуска

**Linux / macOS:**
```bash
cd build
./BackupSystem
```

**Windows:**
```bash
cd build
./BackupSystem.exe
```
### Сборка через Docker
```bash
docker build -t backup-system .
```
#### Запуск тестов
Все тесты
```bash
docker run --rm backup-system all
```

Модульные тесты
```bash
docker run --rm backup-system all
```

Тестовые сценарии
```bash
docker run --rm backup-system scenarios
```

