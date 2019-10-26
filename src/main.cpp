#include <iostream>
#include <opencv2/opencv.hpp>

#include "draw/JingleBuffer.h"
#include "controller/JingleController.h"

int main(int argc, const char *argv[]) {

    JingleController jingleController;
    for (int i = 0; i < 60; ++i) {
        for (int j = 0; j < 60; ++j) {
            jingleController.drawPixel(1u, 10 + i, 30 + j, 0xff, 0, 0, 0xFF);
            jingleController.drawPixel(2u, 30 + i, 50 + j, 0xff, 0xFF, 0, 0xFF);
            jingleController.drawPixel(3u, 70 + i, 90 + j, 0xff, 0, 0xFF, 0xFF);
        }
    }

    for (int i = 0; i < 60; ++i) {
        for (int j = 0; j < 200; ++j) {
            jingleController.drawPixel(1u, 100 + i, 70 + j, 0xff, 0, 0, 0xFF);
            jingleController.drawPixel(2u, 200 + i, 200 + j, 0xff, 0xFF, 0, 0xFF);
            jingleController.drawPixel(3u, 280 + i, 400 + j, 0xff, 0, 0xFF, 0xFF);
        }
    }

    cv::namedWindow("JinglePings", cv::WINDOW_AUTOSIZE);
    cv::imshow("JinglePings", jingleController.get4Buffers());

    cv::waitKey(0);
    return 0;
}