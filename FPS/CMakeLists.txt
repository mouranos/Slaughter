project(FPS)
cmake_minimum_required (VERSION 3.0)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
find_package(OpenGL REQUIRED)
find_package(Bullet REQUIRED)
find_package(GLFW REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)
find_package(OpenAL REQUIRED)
find_package(ALURE REQUIRED)
find_package(PhysFS REQUIRED)
find_package(GLUT REQUIRED)
find_package(Freetype REQUIRED)

if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
	message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
file(GLOB SRC "main/[^.]*.*" "common/*" "main/*/*")
add_executable(${PROJECT_NAME} ${SRC})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
include_directories(
    ${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${OPENAL_INCLUDE_DIR}
    ${ALURE_INCLUDE_DIR}
    ${PHYSFS_INCLUDE_DIR}
    ${BULLET_INCLUDE_DIRS}
    ${GLUT_INCLUDE_DIR}
    ${FREETYPE_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

target_link_libraries(${PROJECT_NAME}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${GLFW_LIBRARY}
    ${OPENAL_LIBRARY}
    ${ALURE_LIBRARY}
    ${PHYSFS_LIBRARY}
    ${BULLET_LIBRARIES}
    ${GLUT_LIBRARIES}
    ${FREETYPE_LIBRARIES}
)
