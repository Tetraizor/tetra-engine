include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.17.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/editor/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/**/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/editor/**/*.cpp
)

add_executable(run_tests ${TEST_SOURCES})

target_link_libraries(run_tests
    PRIVATE
    gtest_main
    TetraEngine
)

enable_testing()
add_test(NAME run_tests COMMAND run_tests)
