cmake_minimum_required(VERSION 3.1)
project(qsv)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
list( APPEND CMAKE_CXX_FLAGS "-std=c++1y ${CMAKE_CXX_FLAGS} -O2 -pipe -mtune=native -fprofile-arcs")

enable_testing()
set(App_SRCS  mainwindow.cpp main.cpp layoutmodel.cpp)
set(Graphics_SRC graphicsscene.cpp graphicsview.cpp graphicspolygonitem.cpp  graphicsworker.cpp)


# find_package(Qt4 REQUIRED)
# add_executable(qsv_4 ${QSV_SRCS})
# set_target_properties(qsv_4 PROPERTIES
# CMAKE_CURRENT_BINARY_DIR  ${CMAKE_BINARY_DIR}/qsv4/
# AUTOGEN_TARGETS_FOLDER ${CMAKE_BINARY_DIR}/qsv4/
# )
# target_link_libraries(qsv_4 Qt4::QtGui Qt4::QtOpenGL Qt4::QtCore )


find_package(Qt5Gui)
find_package(Qt5Widgets) ## COMPONENTS PRIVATE_HEADERS)
find_package(Qt5OpenGL)
find_package(Qt5Test)
add_library(graphics ${Graphics_SRC})
add_executable(qsv_5 ${App_SRCS} resource.qrc)

# set_target_properties(qsv_5 PROPERTIES
# CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build/qsv5/
# CMAKE_CURRENT_BINARY_DIR  ${CMAKE_BINARY_DIR}/qsv5/
# AUTOGEN_TARGETS_FOLDER ${CMAKE_BINARY_DIR}/qsv5/
# )
target_link_libraries(graphics Qt5::Widgets Qt5::Gui Qt5::OpenGL)
target_link_libraries(qsv_5 graphics Qt5::Widgets Qt5::Gui Qt5::OpenGL)


# install(TARGETS qsv RUNTIME DESTINATION bin)

