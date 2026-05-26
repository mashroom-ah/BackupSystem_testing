# ========== ЭТАП 1: СБОРКА ==========
FROM ubuntu:22.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    git \
    ca-certificates \
    wget \
    qt6-base-dev \
    qt6-tools-dev \
    libz-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libxkbcommon-dev \
    libxcb-randr0-dev \
    libxcb-shape0-dev \
    libxcb-sync-dev \
    libxcb-xfixes0-dev \
    libxcb-xkb-dev \
    libxcb-icccm4-dev \
    libxcb-keysyms1-dev \
    libxcb-xinerama0-dev \
    && rm -rf /var/lib/apt/lists/*

RUN update-ca-certificates && \
    git config --global http.sslVerify false

ENV GIT_SSL_NO_VERIFY=1

WORKDIR /src
COPY . .

# Исправляем include-директивы в cpp-файлах, чтобы они соответствовали именам .ui файлов
RUN sed -i 's/#include "ui_JobDialog.h"/#include "ui_jobdialog.h"/' src/ui/jobdialog.cpp && \
    sed -i 's/#include "ui_RestoreDialog.h"/#include "ui_restoredialog.h"/' src/ui/restoredialog.cpp

# Модифицируем CMakeLists.txt тестов для установки исполняемых файлов
RUN sed -i '/add_executable(unit_tests/,/^)/ s/)/)\ninstall(TARGETS unit_tests RUNTIME DESTINATION bin)/' tests/CMakeLists.txt && \
    sed -i '/add_executable(test_scenarios/,/^)/ s/)/)\ninstall(TARGETS test_scenarios RUNTIME DESTINATION bin)/' tests/CMakeLists.txt

# Конфигурация и сборка
RUN cmake -B /build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/app/install \
    && cmake --build /build --parallel $(nproc) \
    && cmake --build /build --target install

# Запуск тестов (если какой-то тест упадёт, сборка прервётся)
RUN cd /build && ctest --output-on-failure

# Копируем бинарные файлы в отдельную папку для финального образа
RUN mkdir -p /out/bin && \
    cp /build/BackupSystem /out/bin/ && \
    cp /build/tests/unit_tests /out/bin/ && \
    cp /build/tests/test_scenarios /out/bin/

# ========== ЭТАП 2: ВРЕМЯ ВЫПОЛНЕНИЯ ==========
FROM ubuntu:22.04

RUN apt-get update && apt-get install -y --no-install-recommends \
    libqt6core6 \
    libqt6gui6 \
    libqt6widgets6 \
    libqt6concurrent6 \
    qt6-qpa-plugins \
    libgl1 \
    libglu1-mesa \
    libxcb-icccm4 \
    libxcb-keysyms1 \
    libxcb-xinerama0 \
    libxkbcommon-x11-0 \
    libz1 \
    libstdc++6 \
    xauth \
    x11-apps \
    && rm -rf /var/lib/apt/lists/*

COPY --from=builder /out/bin /app/bin
COPY entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

WORKDIR /app
ENTRYPOINT ["/entrypoint.sh"]