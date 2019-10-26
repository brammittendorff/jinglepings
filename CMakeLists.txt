cmake_minimum_required(VERSION 3.0)
project(jinglepings-server)
find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)

set(SOURCE src/main.cpp
        src/draw/JingleBuffer.cpp
        src/draw/JingleBuffer.h
        src/controller/JingleController.cpp
        src/controller/JingleController.h
        src/socket/Socket.cpp
        src/socket/Socket.h)

add_compile_definitions(JINGLE_FRAME_WIDTH=512 JINGLE_FRAME_HEIGHT=384)

add_executable(${PROJECT_NAME} ${SOURCE})
target_link_libraries(jinglepings-server ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})
