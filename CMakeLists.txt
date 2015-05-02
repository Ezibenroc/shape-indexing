PROJECT(Helloworld)
#Required in DGtal

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

### We need Boost Program Options library
SET(Boost_USE_STATIC_LIBS   ON)
SET(Boost_USE_MULTITHREADED ON)
SET(Boost_USE_STATIC_RUNTIME OFF)
SET(Boost_FOUND FALSE)
FIND_PACKAGE(Boost 1.46.0 REQUIRED COMPONENTS program_options)
if ( Boost_FOUND )
  message(STATUS "Boost and boost_program_options found.")
  include_directories( ${Boost_INCLUDE_DIRS} )
 endif( Boost_FOUND )
 find_package(Threads)


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

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -O3")

# Learn and classify

add_library(Filter src/filter)
target_link_libraries(Filter ${DGTAL_LIBRARIES})

add_library(Moment src/moment)
target_link_libraries(Moment ${DGTAL_LIBRARIES})

add_library(Border src/border)
target_link_libraries(Border ${DGTAL_LIBRARIES})

add_library(PerimeterVSArea src/perimeterVSarea)
target_link_libraries(PerimeterVSArea ${DGTAL_LIBRARIES})

add_library(Convexity src/convexity)
target_link_libraries(Convexity ${DGTAL_LIBRARIES})

add_library(ImageCharacterization src/imageCharacterization)
target_link_libraries(ImageCharacterization Filter Moment Border Convexity PerimeterVSArea ${DGTAL_LIBRARIES})

add_library(ImageClass src/imageClass)
target_link_libraries(ImageClass ImageCharacterization)

add_library(Classifier src/classifier)
target_link_libraries(Classifier ImageClass)

add_executable(learn src/learn)
target_link_libraries(learn ImageClass ImageCharacterization ${DGTAL_LIBRARIES}  ${Boost_LIBRAIRIES} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

add_executable(classify src/classify)
target_link_libraries(classify Classifier ImageClass ImageCharacterization ${DGTAL_LIBRARIES}  ${Boost_LIBRAIRIES} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

add_executable(distance src/distance)
target_link_libraries(distance ImageCharacterization ${DGTAL_LIBRARIES}  ${Boost_LIBRAIRIES} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

# Evaluation

SET(SRCs
    imgRotate
    imgScale
    imgAddNoise)

FOREACH(FILE ${SRCs})
  add_executable(${FILE} evaluation/${FILE})
  target_link_libraries (${FILE} ${DGTAL_LIBRARIES}  ${Boost_LIBRAIRIES}  ${Boost_PROGRAM_OPTIONS_LIBRARY})
ENDFOREACH(FILE)

add_executable(getRank evaluation/getRank)

add_custom_target(evaluation ALL
COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/evaluation/scriptClassification.sh ${CMAKE_CURRENT_BINARY_DIR}
COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/evaluation/scriptRobustness-Hurt-me-plenty.sh ${CMAKE_CURRENT_BINARY_DIR}
COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/evaluation/scriptRobustness-I-m-too-young-to-die.sh ${CMAKE_CURRENT_BINARY_DIR}
COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/evaluation/scriptRobustness-Nightmare.sh ${CMAKE_CURRENT_BINARY_DIR}
COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/classes.csv ${CMAKE_CURRENT_BINARY_DIR})

# Unit tests

find_package(CppUnit)
if (CPPUNIT_FOUND)
    add_executable(testrunner tests/main.cpp tests/border_tests.cpp tests/ratio_tests.cpp tests/integration_tests.cpp tests/common.cpp)
    target_link_libraries(testrunner Border Convexity PerimeterVSArea ImageCharacterization ${DGTAL_LIBRARIES})
    target_link_libraries(testrunner cppunit)
else (CPPUNIT_FOUND)
    message("+---------------------------------------------+")
    message("| CppUnit not found, tests won’t be compiled. |")
    message("+---------------------------------------------+")
endif (CPPUNIT_FOUND)
