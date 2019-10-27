cmake_minimum_required(VERSION 3.0)
project(jinglepings-server)
find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE src/main.cpp
        src/draw/JingleBuffer.cpp
        src/draw/JingleBuffer.h
        src/controller/JingleController.cpp
        src/controller/JingleController.h
        src/socket/Socket.cpp
        src/socket/Socket.h
        src/tui/Tui.cpp
        src/tui/Tui.h)

add_compile_definitions(JINGLE_FRAME_WIDTH=512 JINGLE_FRAME_HEIGHT=384)
add_compile_definitions(JINGLE_INTERFACE="lo" JINGLE_PING_THREADS=4 JINGLE_RTMP_URL="rtmp://130.89.149.181:1935/jinglepings")
add_compile_definitions(JINGLE_BLACKLIST_FILE="blacklist.txt")

add_executable(${PROJECT_NAME} ${SOURCE})
target_link_libraries(jinglepings-server ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT} stdc++fs)
