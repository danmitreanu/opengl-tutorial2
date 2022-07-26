cmake_minimum_required(VERSION 3.0.0)
project(TutorialApp)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW glfw3)
find_package(OpenGL)

if(${GLFW_FOUND})
add_executable(
	TutorialApp
	src/main.cpp
)



target_include_directories(TutorialApp PUBLIC ${GLFW_INCLUDE_DIRS})
target_link_directories(TutorialApp PUBLIC ${GLFW_LIBRARY_DIRS})
target_link_libraries(TutorialApp PUBLIC ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES} )
target_link_libraries(TutorialApp PUBLIC "-framework OpenGL")

else()
message(FATAL_ERROR "Unable to find glfw3, tests are disabled!")
endif()
