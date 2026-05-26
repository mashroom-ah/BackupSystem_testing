# ========== СТАДИЯ 1: СБОРКА ==========
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
RUN find . -name "CMakeCache.txt" -delete
RUN cd src/ui && ln -s jobdialog.ui JobDialog.ui

WORKDIR /opt/backup/build
RUN cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu/cmake/Qt6 -G "Unix Makefiles"
RUN cmake --build . -- -j$(nproc)
RUN ctest --output-on-failure -C Release

# ========== СТАДИЯ 2: ФИНАЛЬНЫЙ ОБРАЗ ==========
FROM ubuntu:22.04

# Базовые runtime-зависимости
RUN apt-get update && apt-get install -y \
    libz1 \
    libfontconfig1 \
    libfreetype6 \
    libgl1 \
    libxcb-util1 \
    libxcb-cursor0 \
    libxcb-keysyms1 \
    libxcb-xinerama0 \
    libxcb-xkb1 \
    libxcb-icccm4 \
    libxcb-image0 \
    libxcb-randr0 \
    libxcb-render-util0 \
    libxcb-shape0 \
    libxcb-sync1 \
    libxcb-xfixes0 \
    libxcb-xinput0 \
    libxkbcommon-x11-0 \
    libx11-xcb1 \
    libxcb-glx0 \
    && rm -rf /var/lib/apt/lists/*

# Копируем исполняемые файлы
COPY --from=builder /opt/backup/build/BackupSystem /usr/local/bin/
COPY --from=builder /opt/backup/build/tests/unit_tests /usr/local/bin/
COPY --from=builder /opt/backup/build/tests/test_scenarios /usr/local/bin/

# Копируем все Qt-библиотеки и плагины из стадии сборки
COPY --from=builder /usr/lib/x86_64-linux-gnu/libQt6*.so* /usr/lib/x86_64-linux-gnu/
COPY --from=builder /usr/lib/x86_64-linux-gnu/qt6/plugins/ /usr/lib/x86_64-linux-gnu/qt6/plugins/

# Обновляем кэш библиотек
RUN ldconfig

WORKDIR /data

ENV QT_QPA_PLATFORM_PLUGIN_PATH=/usr/lib/x86_64-linux-gnu/qt6/plugins/platforms
ENV QT_QPA_PLATFORM=xcb

CMD ["/usr/local/bin/unit_tests"]