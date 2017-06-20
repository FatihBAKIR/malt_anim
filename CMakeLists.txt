cmake_minimum_required(VERSION 3.2)
project(malt)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../malt_cmake/")
set(CMAKE_CXX_STANDARD 14)

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS} -undefined dynamic_lookup")
endif()

include(Malt)

set(HEADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/malt_anim")
set(HEADER_FILES
        ${HEADER_PATH}/module.hpp
        ${HEADER_PATH}/sqt.hpp
        ${HEADER_PATH}/joint.hpp
        ${HEADER_PATH}/skeleton.hpp
        ${HEADER_PATH}/skeleton_loader.hpp
        ${HEADER_PATH}/skinned_mesh.hpp
        ${HEADER_PATH}/skinned_mesh_loader.hpp
        ${HEADER_PATH}/components/skinned_mesh_renderer.hpp
        )

set(SOURCE_FILES src/module.cpp src/skeleton_loader.cpp src/skinned_mesh.cpp src/skinned_mesh_renderer.cpp)
add_library(malt_anim SHARED ${SOURCE_FILES})

malt_def_module(malt_anim)

malt_add_module(malt_anim malt_core)
malt_add_module(malt_anim malt_asset)
malt_add_module(malt_anim malt_render)

find_package(assimp REQUIRED)
find_package(rtk REQUIRED)

target_link_libraries(malt_anim PUBLIC rtk)
target_link_libraries(malt_anim PUBLIC assimp)

malt_install(malt_anim ${HEADER_PATH})

add_executable(anim_tests tests/test.cpp)
target_link_libraries(anim_tests PUBLIC malt_anim)