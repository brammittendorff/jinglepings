#ifndef JINGLE_BUFFERS_JINGLECONTROLLER_H
#define JINGLE_BUFFERS_JINGLECONTROLLER_H

#include "../draw/JingleBuffer.h"

class JingleController {

public:
    void drawPixel(uint64_t sourceAddr, int y, int x, uint8_t a, uint8_t r, uint8_t g, uint8_t b);
    void drawPixel(uint64_t sourceAddr, int y, int x, uint32_t value);

    cv::Mat get4Buffers();

private:
    std::unordered_map<uint64_t, JingleBuffer> sourceFrames = {};
    JingleBuffer mainBuffer;
};


#endif //JINGLE_BUFFERS_JINGLECONTROLLER_H
