## ---------------------------------------------------------
##  G4Basic | app/CMakeLists.txt
##
##  CMake build script.
##   * Author: Justo Martin-Albo, Taylor Contreras
##   * Creation date: 14 Aug 2019
## ---------------------------------------------------------

add_executable(G4Basic G4Basic.cpp $<TARGET_OBJECTS:${CMAKE_PROJECT_NAME}>)
target_include_directories(G4Basic PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(G4Basic ${Geant4_LIBRARIES})
target_link_libraries(G4Basic ${ROOT_LIBRARIES})
target_include_directories(G4Basic PUBLIC ${ROOT_INCLUDE_DIRS})
install(TARGETS G4Basic RUNTIME DESTINATION bin)
