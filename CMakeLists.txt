cmake_minimum_required(VERSION 3.10.0)
project(argparser VERSION 0.1.0 LANGUAGES C CXX)

include(GNUInstallDirs)

add_library(argparser SHARED argparser.cpp)

target_compile_features(argparser PUBLIC cxx_std_17)

set_target_properties(argparser PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR})

target_include_directories(argparser PRIVATE ${CMAKE_SOURCE_DIR}/include)

install(TARGETS argparser
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

add_executable(app main.cpp argparser.cpp)

target_include_directories(app PRIVATE ${CMAKE_SOURCE_DIR}/include)
