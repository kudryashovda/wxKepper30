cmake_minimum_required(VERSION 3.0.0)
project(Keep VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(converter)

aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST} resources.rc)

find_package(wxWidgets)
# sudo apt install libwxbase*-dev libwxgtk*-dev

set(wxWidgets_CONFIGURATION mswu)
if (WIN32)
    set(CMAKE_CXX_FLAGS "-mwindows")
endif (WIN32)

include(${wxWidgets_USE_FILE})
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} -lstdc++fs)

# uncomment when windows static build
# target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} -lstdc++fs -static-libstdc++ -static-libgcc -static)
