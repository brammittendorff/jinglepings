/*
 * Copyright (C) 2019 Studenten Net Twente
 *
 * Licensed under the EUPL. See the file LICENSE.md in the top level
 * directory for more details.
 */

#ifndef JINGLE_BUFFERS_JINGLEBUFFER_H
#define JINGLE_BUFFERS_JINGLEBUFFER_H

#include <opencv2/opencv.hpp>

//! \brief Image buffer where pings are stored.
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
