## ---------------------------------------------------------
##  G4Basic | src/CMakeLists.txt
##
##  CMake build script.
##   * Author: Justo Martin-Albo
##   * Creation date: 14 Aug 2019
## ---------------------------------------------------------

SET(SRC   DetectorConstruction.cpp
          EventAction.cpp
          PrimaryGeneration.cpp
          RunAction.cpp
          SteppingAction.cpp
	  SDPlanes.cpp)

add_library(${CMAKE_PROJECT_NAME} OBJECT ${SRC})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${ROOT_INCLUDE_DIRS})
