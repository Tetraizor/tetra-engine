project(TetraEngine)

add_library(${PROJECT_NAME} SHARED
    src/core.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Use the platform-aware DIST_DIR from the root CMakeLists.txt
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "TetraEngine"
    RUNTIME_OUTPUT_DIRECTORY ${DIST_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${DIST_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY ${DIST_DIR}/lib
)