#ifndef JINGLE_BUFFERS_JINGLEBUFFER_H
#define JINGLE_BUFFERS_JINGLEBUFFER_H

#include <opencv2/opencv.hpp>

class JingleBuffer {

public:
    JingleBuffer();
    virtual ~JingleBuffer() = default;

    void setPixel(int y, int x, uint8_t a, uint8_t r, uint8_t g, uint8_t b);
    void setPixel(int y, int x, uint32_t value);
    void setPixel(int y, int x, const cv::Vec4b &value);

    cv::Mat getBuffer() const;

private:
    cv::Mat buffer;
};


#endif //JINGLE_BUFFERS_JINGLEBUFFER_H
