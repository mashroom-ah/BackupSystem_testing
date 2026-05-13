# Тесты
add_executable(unit_tests
    test_BackupJob.cpp
    test_Schedule.cpp
    test_RetentionPolicy.cpp
    test_RestorePoint.cpp
    test_JobConverter.cpp
    test_FileService.cpp
    test_HashService.cpp
    test_DiffService.cpp
    test_CompressionService.cpp
    test_RetentionService.cpp
    test_BackupService.cpp
    test_RestoreService.cpp
    test_JobService.cpp
    test_JobRepository.cpp
    test_ApplicationFacade.cpp
    test_StorageService.cpp
)

target_link_libraries(unit_tests
    backup_lib
    Catch2::Catch2WithMain
)

# Добавляем пути к заголовкам проекта
target_include_directories(unit_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

# Отдельная программа для сценариев
add_executable(test_scenarios test_scenarios.cpp)
target_link_libraries(test_scenarios
    backup_lib
    Catch2::Catch2WithMain
)

target_include_directories(test_scenarios PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

include(CTest)
include(Catch)
catch_discover_tests(unit_tests)
catch_discover_tests(test_scenarios)