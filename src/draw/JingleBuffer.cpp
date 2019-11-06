/*
 * Copyright (C) 2019 Studenten Net Twente
 *
 * Licensed under the EUPL. See the file LICENSE.md in the top level
 * directory for more details.
 */

#include "JingleBuffer.h"

//! Constructor for fast JinglePing buffer
//! Required fixed size due to implicit creation in unordered_map
JingleBuffer::JingleBuffer() : buffer(cv::Mat::zeros(cv::Size(JINGLE_FRAME_WIDTH, JINGLE_FRAME_HEIGHT), CV_8UC4)) {
}

//! Uses high performance method to set a pixel to a value (uint8_t per channel, 0-0xff)
//! \param y Y position
//! \param x X position
//! \param a Alpha
//! \param r Red
//! \param g Green
//! \param b Blue value
void JingleBuffer::setPixel(int y, int x, uint8_t a, uint8_t r, uint8_t g, uint8_t b) {
    // Convert incoming ARGB values to locally stored ARGB uint32_t
    uint32_t val = a << 24u | r << 16u | g << 8u | b;
    if (y < buffer.rows && x < buffer.cols) {
        buffer.at<int32_t>(y, x) = reinterpret_cast<int32_t&>(val);
    }
}

//! Uses high performance method to set a pixel to a value (uint8_t per channel, 0-0xff)
//! \param y Y position
//! \param x X position
//! \param value ARGB value for a pixel
void JingleBuffer::setPixel(int y, int x, uint32_t value) {
    if (y < buffer.rows && x < buffer.cols) {
        buffer.at<int32_t>(y, x) = reinterpret_cast<int32_t&>(value);
    }
}

//! Uses a vector of (r,g,b,a) to set a pixel in the buffer
//! \param y
//! \param x
//! \param value
void JingleBuffer::setPixel(int y, int x, const cv::Vec4b &value) {
    if (y < buffer.rows && x < buffer.cols) {
        buffer.at<cv::Vec4b>(y, x) = value;
    }
}

//! Returns reference to internal OpenCV buffer
//! \return internal OpenCV buffer
cv::Mat JingleBuffer::getBuffer() const {
    return buffer;
}
