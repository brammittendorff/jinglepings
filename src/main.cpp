#include <iostream>
#include <array>
#include <thread>
#include <opencv2/opencv.hpp>

#include "draw/JingleBuffer.h"
#include "controller/JingleController.h"
#include "socket/Socket.h"
#include "tui/Tui.h"

// Gstreamer pipeline for encoding
const std::string gstreamer_pipe = "appsrc is-live=true ! videoconvert ! " \
                                   "x264enc tune=zerolatency key-int-max=2500 speed-preset=veryfast quantizer=0 qp-min=0 qp-max=0 byte-stream=true threads=4 ! " \
                                   "flvmux streamable=true ! rtmpsink location=" + std::string(JINGLE_RTMP_URL);

void overViewCreator(JingleController &controller) {
    using namespace std::chrono_literals;

    for (;;) {
        std::this_thread::sleep_for(30s);
        try {
            auto success = cv::imwrite("/tmp/overview.png", controller.getBuffers());
            if (!success) {
                std::cerr << "Error updating overview" << std::endl;
            }
        } catch (cv::Exception &e) {
            std::cerr << "Exception creating overview: " << e.what() << std::endl;
        }
    }
}

int mainCreator(JingleController &controller) {
    using namespace std::chrono_literals;

    std::cerr << "Opening GStreamer pipe with definition: " << gstreamer_pipe << std::endl;

    cv::VideoWriter writer;
    auto res = writer.open(gstreamer_pipe, cv::CAP_GSTREAMER, 0, (double) 25,
                           cv::Size(JINGLE_FRAME_WIDTH, JINGLE_FRAME_HEIGHT), true);
    if (!res) {
        std::cerr << "Error creating GStreamer pipe" << std::endl;
        exit(1);
    }

    for (;;) {
        auto deadline = std::chrono::steady_clock::now() + 40ms;

        cv::Mat frame;
        cv::cvtColor(controller.getMainBuffer(), frame, cv::COLOR_RGBA2RGB);
        writer << frame;

        std::this_thread::sleep_until(deadline);
    }
}

int main(int argc, const char *argv[]) {
    std::filesystem::path path = JINGLE_BLACKLIST_FILE;
    JingleController jingleController(path);
    Tui tui(jingleController);
    Socket s;

    // Initialize socket
    try {
        s = Socket(JINGLE_INTERFACE);
    } catch (SocketException &e) {
        std::cerr << "Socket exception: " << e.what() << std::endl;
        return 1;
    }

    // Create ping reception threads
    std::vector<std::thread> threads;
    for (int i = 0; i < JINGLE_PING_THREADS; i++) {
        threads.emplace_back(std::thread(&Socket::run, s, std::ref(jingleController)));
    }

    // Create overview creator thread
    threads.emplace_back(std::thread(overViewCreator, std::ref(jingleController)));

    // Start creating the main image
    threads.emplace_back(std::thread(mainCreator, std::ref(jingleController)));

    // Start TUI
    tui.run();

    // Wait for threads to exit
    for (auto &t : threads) {
        t.join();
    }

    return 0;
}