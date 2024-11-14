cmake_minimum_required(VERSION 3.14...3.22)

# ---- Project ----

project(
  Factify
  VERSION 1.0
  LANGUAGES CXX
)

# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

# ---- Add dependencies via CPM ----

include(cmake/CPM.cmake)

CPMAddPackage("gh:TheLartians/PackageProject.cmake@1.8.0")

CPMAddPackage(
  NAME fmt
  GIT_TAG 10.2.1
  GITHUB_REPOSITORY fmtlib/fmt
  OPTIONS "FMT_INSTALL YES"
)

CPMAddPackage("gh:nlohmann/json@3.10.5")

# ---- Add JNI ----

# find_package(JNI REQUIRED)
# if (JNI_FOUND)
#     message(STATUS "JNI found: ${JNI_INCLUDE_DIRS}")
#     include_directories(${JNI_INCLUDE_DIRS})
#     link_libraries(${JNI_LIBRARIES})
# else()
#     # Manually set paths if JNI is not found automatically
#     include_directories("C:/Program Files/Java/jdk-17.0.4.1/include")
#     include_directories("C:/Program Files/Java/jdk-17.0.4.1/include/win32")
#     link_directories("C:/Program Files/Java/jdk-17.0.4.1/lib")
# endif()

# ---- Add source files ----

file(GLOB_RECURSE headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")

# ---- Create library ----

add_library(${PROJECT_NAME} ${headers} ${sources})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

# being a cross-platform target, we enforce standards conformance on MSVC
target_compile_options(${PROJECT_NAME} PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->")

# Link dependencies
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt nlohmann_json)

target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                         $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
)

# ---- Create an installable target ----

string(TOLOWER ${PROJECT_NAME}/version.h VERSION_HEADER_LOCATION)

packageProject(
  NAME ${PROJECT_NAME}
  VERSION ${PROJECT_VERSION}
  NAMESPACE ${PROJECT_NAME}
  BINARY_DIR ${PROJECT_BINARY_DIR}
  INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include
  INCLUDE_DESTINATION include/${PROJECT_NAME}-${PROJECT_VERSION}
  VERSION_HEADER "${VERSION_HEADER_LOCATION}"
  COMPATIBILITY SameMajorVersion
  DEPENDENCIES "fmt 9.1.0"
)

# ---- Install header dependencies ----

install(TARGETS nlohmann_json EXPORT nlohmann_json-config DESTINATION lib)
install(EXPORT nlohmann_json-config DESTINATION share/nlohmann_json/cmake)

# ---- Add FactifyTests ----

# enable_testing()

# include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
# doctest_discover_tests(${PROJECT_NAME})

# ---- code coverage ----

if(ENABLE_TEST_COVERAGE)
  target_compile_options(Factify PUBLIC -O0 -g -fprofile-arcs -ftest-coverage)
  target_link_options(Factify PUBLIC -fprofile-arcs -ftest-coverage)
endif()