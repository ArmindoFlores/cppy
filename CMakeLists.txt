cmake_minimum_required(VERSION 3.16.3)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

find_package(Python REQUIRED COMPONENTS Interpreter)

project(CPPY VERSION 0.0.1 DESCRIPTION "A Python to C++ transpiler")

set(sources
    src/python/pybaseobject.cpp
    src/python/pybool.cpp
    src/python/pybuiltins.cpp
    src/python/pydict.cpp
    src/python/pyfunction.cpp
    src/python/pygarbagecollector.cpp
    src/python/pyglobalinstances.cpp
    src/python/pyhelpers.cpp
    src/python/pyint.cpp
    src/python/pylist.cpp
    src/python/pynone.cpp
    src/python/pynotimplemented.cpp
    src/python/pyobject.cpp
    src/python/pyrecursionctx.cpp
    src/python/pyscope.cpp
    src/python/pystring.cpp
    src/python/pytraceback.cpp
    src/python/pytuple.cpp
    src/python/pytype.cpp
    src/python/pytypes.cpp
)

add_library(cppy SHARED
    ${sources}
)
add_library(staticcppy STATIC
    ${sources}
)
target_include_directories(cppy PRIVATE src/python)
set_target_properties(cppy PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(cppy PROPERTIES PUBLIC_HEADER src/python/cppy.h)
target_include_directories(staticcppy PRIVATE src/python)
set_target_properties(staticcppy PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(staticcppy PROPERTIES PUBLIC_HEADER src/python/cppy.h)
set_target_properties(staticcppy PROPERTIES OUTPUT_NAME cppy)

add_custom_target(package ALL)
add_custom_command(
    TARGET package
    POST_BUILD
    COMMAND cd ../src/transpiler && ${Python_EXECUTABLE} setup.py build
)

add_executable(main
    tests/main.cpp
)
target_include_directories(main PUBLIC src/python)
target_link_libraries(main cppy)
install(TARGETS cppy staticcppy)
install(CODE "execute_process(COMMAND ${Python_EXECUTABLE} setup.py install WORKING_DIRECTORY ../src/transpiler)")