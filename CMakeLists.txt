cmake_minimum_required(VERSION 3.16)
project(BackupSystem VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC OFF)

find_package(Qt6 REQUIRED COMPONENTS Widgets Concurrent)
find_package(ZLIB REQUIRED)

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.5.2
)
FetchContent_MakeAvailable(Catch2)

set(LIB_SOURCES
    src/domain/BackupJob.cpp
    src/domain/Schedule.cpp
    src/domain/RetentionPolicy.cpp
    src/domain/RestorePoint.cpp
    src/application/JobService.cpp
    src/application/BackupService.cpp
    src/application/RestoreService.cpp
    src/application/StorageService.cpp
    src/application/ApplicationFacade.cpp
    src/infrastructure/FileService.cpp
    src/infrastructure/HashService.cpp
    src/infrastructure/DiffService.cpp
    src/infrastructure/CompressionService.cpp
    src/infrastructure/RetentionService.cpp
    src/infrastructure/JobRepository.cpp
    src/dto/JobConverter.cpp
    src/ui/mainwindow.cpp
    src/ui/jobdialog.cpp
    src/ui/restoredialog.cpp
)

set(HEADERS
    src/ui/mainwindow.h
    src/ui/jobdialog.h
    src/ui/restoredialog.h
    include/domain/BackupJob.h
    include/domain/Schedule.h
    include/domain/RetentionPolicy.h
    include/domain/RestorePoint.h
    include/domain/BackupType.h
    include/application/JobService.h
    include/application/BackupService.h
    include/application/RestoreService.h
    include/application/StorageService.h
    include/application/ApplicationFacade.h
    include/infrastructure/FileService.h
    include/infrastructure/HashService.h
    include/infrastructure/DiffService.h
    include/infrastructure/CompressionService.h
    include/infrastructure/RetentionService.h
    include/infrastructure/JobRepository.h
    include/dto/BackupJobDTO.h
    include/dto/JobConverter.h
)

set(UI_FILES
    src/ui/mainwindow.ui
    src/ui/jobdialog.ui
    src/ui/restoredialog.ui
)

# Статическая библиотека с основной логикой
add_library(backup_lib STATIC ${LIB_SOURCES} ${HEADERS} ${UI_FILES})
target_include_directories(backup_lib PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(backup_lib
    Qt6::Widgets
    Qt6::Concurrent
    ZLIB::ZLIB
)

# Основное приложение
add_executable(BackupSystem src/main.cpp)
target_link_libraries(BackupSystem backup_lib)
target_include_directories(BackupSystem PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Подключение тестов
add_subdirectory(tests)