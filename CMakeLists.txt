PROJECT(Helloworld)
#Required in DGtal

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

### We need Boost Program Options library
SET(Boost_USE_STATIC_LIBS   ON)
SET(Boost_USE_MULTITHREADED ON)
SET(Boost_USE_STATIC_RUNTIME OFF)
SET(Boost_FOUND FALSE)
FIND_PACKAGE(Boost 1.46.0 REQUIRED COMPONENTS program_options system filesystem)
if ( Boost_FOUND )
  message(STATUS "Boost and boost_program_options found.")
  include_directories( ${Boost_INCLUDE_DIRS} )
 endif( Boost_FOUND )


FIND_PACKAGE(DGtal REQUIRED)
INCLUDE_DIRECTORIES(${DGTAL_INCLUDE_DIRS})
LINK_DIRECTORIES(${DGTAL_LIBRARY_DIRS})

IF ((WITH_QGLVIEWER) OR (WITH_VISU3D_QGLVIEWER))
  SET(SRCs
    image2mesh  ${SRCs})
  MESSAGE(STATUS "Viewer3D enabled")
ELSE()
  MESSAGE(STATUS "WITH_QGLVIEWER missing when building DGtal. I cannot construct image2mesh")
ENDIF()

add_library(Border src/border)
target_link_libraries(Border ${DGTAL_LIBRARIES})

add_library(PerimeterVSArea src/perimeterVSarea)
target_link_libraries(PerimeterVSArea Border ${DGTAL_LIBRARIES})

add_library(Convexity src/convexity)
target_link_libraries(Convexity Border ${DGTAL_LIBRARIES})

add_library(ImageCharacterization src/imageCharacterization)
target_link_libraries(ImageCharacterization Convexity PerimeterVSArea ${DGTAL_LIBRARIES})

add_library(ImageClass src/imageClass)
target_link_libraries(ImageClass ImageCharacterization)

add_executable(main src/main)
target_link_libraries(main ImageClass ImageCharacterization ${DGTAL_LIBRARIES}  ${Boost_LIBRAIRIES} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -O3")
