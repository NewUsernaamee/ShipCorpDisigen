cmake_minimum_required(VERSION 3.14)

project(TestAppProj LANGUAGES CXX)

find_package( Qt6Core )
find_package( Qt6Widgets )
find_package( Qt6Gui )
find_package(Qt6 REQUIRED COMPONENTS OpenGL)

set(BACK_ ${CMAKE_CURRENT_SOURCE_DIR}/../BackEnd_App)
include_directories(${BACK_})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

#use for generate ui_mainwnd
#set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTORCC ON)

set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(c3dvisionDeb ${C3D_Vision_DEBUG_DIR}/c3dvision.lib)
set(c3dvisionRel ${C3D_Vision_RELEASE_DIR}/c3dvision.lib)
SET(C3D_Vision_INCLUDE_DIR ${C3D_Vision_DIR})
SET(C3D_Vision_DIR ${CMAKE_SOURCE_DIR}/../Vision)
set(QT_VisionDeb ${QT_Vision_DEBUG_DIR}/qtvision.lib)

add_executable(TestAppProj WIN32
  main.cpp
  MainWnd.h
  MainWnd.cpp
  DrawingWdgt.h
  DrawingWdgt.cpp
  ui_MainWnd.h
  DialogWdgt.cpp
  DialogWdgt.h
  SplinesList.cpp
  SplinesList.h  
  PointsWdgt.h
  PointsWdgt.cpp
  SplinesList.h
  Vision3DView.cpp
  Vision3DView.h
  SectionCordsWdgt.h
  SectionCordsWdgt.cpp
  customTreeItem.cpp
  customTreeItem.h
  CordLineEdit.h
  CordLineEdit.cpp
  OZDialog.h
  OZDialog.cpp
  controller.cpp
  controller.h
  CuttingPlaneSettingsWdgt.h
  CuttingPlaneSettingsWdgt.cpp
  CuttingPlanesWdgt.h
  CuttingPlanesWdgt.cpp
  CuttingTools.h
  CuttingTools.cpp
  MainWnd.ui
  MainWnd.qrc
)


target_link_libraries(TestAppProj PRIVATE
    Qt::Widgets Qt::Gui Qt::Core  Qt6::OpenGL ShipSection ShipModel
    ${45_OpenGLWindow_OUTPUT}
    ${C3D_VIS_LIB}
    ${c3dvisionDeb} 
    ${c3dvisionRel}
    ${OPENGL_LIBRAREIS}
    ${QT_VisionDeb}
)
