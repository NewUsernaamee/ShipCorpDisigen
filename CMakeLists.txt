cmake_minimum_required(VERSION 3.14)

project(TestAppProj LANGUAGES CXX)

set(C3D_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/c3d_sdk/Include/c3d CACHE PATH "Choose C3D Include directory")
set(C3D_DEBUG_DIR ${CMAKE_SOURCE_DIR}/c3d_sdk/Debug/c3d CACHE PATH "Choose C3D Debug directory")
set(C3D_RELEASE_DIR ${CMAKE_SOURCE_DIR}/c3d_sdk/Release/c3d CACHE PATH "Choose C3D Release directory")
set(C3D_Vision_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/c3d_sdk/Include/Vision CACHE PATH "Choose vision Include directory")
set(C3D_Vision_DEBUG_DIR ${CMAKE_SOURCE_DIR}/c3d_sdk/Debug/Vision CACHE PATH "Choose C3D_Vision_DEBUG_DIR ")
set(C3D_Vision_RELEASE_DIR ${CMAKE_SOURCE_DIR}/c3d_sdk/Release/Vision CACHE PATH "Choose (C3D_Vision_RELEASE_DIR ")
set(QT_Vision_DEBUG_DIR ${CMAKE_SOURCE_DIR}/c3d_sdk/Debug/QtVision CACHE PATH "Choose (QT_Vision_DEBUG_DIR ")
set(QT_Vision_INCLUDE_DIR  ${CMAKE_SOURCE_DIR}/c3d_sdk/Include/QtVision CACHE PATH "Choose (QT_Vision_INCLUDE_DIR ")

include_directories(${C3D_INCLUDE_DIR} ${OPENGL_LIBRAREIS} ${C3D_Vision_INCLUDE_DIR} ${QT_Vision_INCLUDE_DIR} )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_subdirectory ( UI_App )
add_subdirectory ( BackEnd_App )
