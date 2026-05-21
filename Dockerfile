# ========== СТАДИЯ 1: СБОРКА И ТЕСТИРОВАНИЕ ==========
FROM ubuntu:22.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Etc/UTC

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libz-dev \
    qt6-base-dev \
    qt6-tools-dev \
    qt6-l10n-tools \
    libgl1-mesa-dev \
    libxcb-util1 \
    libxcb-util-dev \
    libxcb-cursor0 \
    libxcb-cursor-dev \
    libxcb-keysyms1 \
    libxcb-keysyms1-dev \
    libxcb-xinerama0 \
    libxcb-xinerama0-dev \
    libxcb-xkb1 \
    libxcb-xkb-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /opt/backup
COPY . .

# Удаляем возможные старые CMakeCache.txt
RUN find . -name "CMakeCache.txt" -delete

# Исправляем регистр .ui файла для совместимости с #include "ui_JobDialog.h"
RUN cd src/ui && ln -s jobdialog.ui JobDialog.ui

WORKDIR /opt/backup/build

# Конфигурация CMake
RUN cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu/cmake/Qt6 -G "Unix Makefiles"

# Сборка
RUN cmake --build . -- -j$(nproc)

# Запуск тестов
RUN ctest --output-on-failure -C Release

# ========== СТАДИЯ 2: ФИНАЛЬНЫЙ ОБРАЗ ==========
FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    libz1 \
    libqt6core6 \
    libqt6widgets6 \
    libqt6gui6 \
    libqt6concurrent6 \
    libgl1 \
    libxcb-util1 \
    libxcb-cursor0 \
    libxcb-keysyms1 \
    libxcb-xinerama0 \
    libxcb-xkb1 \
    && rm -rf /var/lib/apt/lists/*

COPY --from=builder /opt/backup/build/BackupSystem /usr/local/bin/
COPY --from=builder /opt/backup/build/tests/unit_tests /usr/local/bin/
COPY --from=builder /opt/backup/build/tests/test_scenarios /usr/local/bin/

WORKDIR /data

CMD ["/usr/local/bin/unit_tests"]