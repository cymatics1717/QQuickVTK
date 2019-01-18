cmake_minimum_required(VERSION 3.1)

project(QQuickVTK LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Qml Core Quick REQUIRED)

find_package(VTK 8.0 REQUIRED COMPONENTS
  vtkInteractionImage
  vtkRenderingExternal
  vtkGUISupportQt
  vtkIOGeometry
  vtkRenderingVolumeOpenGL2
)

include(${VTK_USE_FILE})

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/*.cpp)
include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(${PROJECT_NAME} ${SRC_FILES} "qml.qrc")
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Quick ${VTK_LIBRARIES})
