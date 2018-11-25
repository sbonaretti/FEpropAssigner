cmake_minimum_required(VERSION 2.6)
 
PROJECT(FEMPropertyAssigner)

FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})
 
FIND_PACKAGE(VTK)
INCLUDE(${VTK_USE_FILE})
 
# use what QVTK built with
SET(QT_MOC_EXECUTABLE ${VTK_QT_MOC_EXECUTABLE} CACHE FILEPATH "")
SET(QT_UIC_EXECUTABLE ${VTK_QT_UIC_EXECUTABLE} CACHE FILEPATH "")
SET(QT_QMAKE_EXECUTABLE ${VTK_QT_QMAKE_EXECUTABLE} CACHE FILEPATH "")
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

# Use the include path and library for Qt that is used by VTK.
INCLUDE_DIRECTORIES(
  ${QT_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  src
  src/test
  src/command
  src/assignment
  src/mesh
  src/gui
  src/ctimage
)

file(GLOB TestSrcs src/test/*.cxx)
file(GLOB TestHeaders src/test/*.h)
file(GLOB PropertyAssignmentSrcs
src/*.cxx
src/*.txx
src/command/*.cxx
src/test/*.cxx
src/assignment/*.cxx
src/mesh/*.cxx
src/gui/*.cxx
src/ctimage/*.cxx
)
file(GLOB PropertyAssignmentHeaders
src/*.h
src/command/*.h
src/test/*.h
src/extrusion/*.h
src/mesh/*.h
src/gui/*.h
src/ctimage/*.h
)

SET(PropertyAssignmentUI src/gui/mainwindow.ui)

# The rest should just work (sure...)
QT4_WRAP_UI(UISrcs ${PropertyAssignmentUI})
QT4_WRAP_CPP(MOCSrcs ${PropertyAssignmentHeaders} )

SOURCE_GROUP("Resources" FILES
  ${PropertyAssignmentUI}
  ${EXE_ICON}
)

SOURCE_GROUP("Generated" FILES
  ${UISrcs}
  ${MOCSrcs}
  ${RCS_SOURCES}
)

ADD_DEFINITIONS(-DQT_GUI_LIBS -DQT_CORE_LIB -DQT3_SUPPORT)
SET_SOURCE_FILES_PROPERTIES(${PropertyAssignmentSrcs} PROPERTIES
                            OBJECT_DEPENDS "${UISrcs}")


ADD_EXECUTABLE(FEMPropertyAssigner ${PropertyAssignmentSrcs}  ${UISrcs} ${MOCSrcs})
SET(MACOSX_BUNDLE_INFO_STRING "FEM Property Assigner")
SET(MACOSX_BUNDLE_BUNDLE_NAME "FEM Property Assigner")


TARGET_LINK_LIBRARIES( FEMPropertyAssigner
  QVTK
  ${QT_LIBRARIES}
  vtkRendering
  vtkGraphics
  vtkHybrid
  vtkImaging
  vtkIO
  vtkCommon
  vtkInfovis
  vtkViews
  ITKBasicFilters
  ITKCommon
  ITKIO
  ITKAlgorithms
  ITKStatistics
)

SET(CMAKE_BUILD_TYPE Release)
