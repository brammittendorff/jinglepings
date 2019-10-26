#include "JingleController.h"

//! Draw a pixel on the Jingle board
//! puts a pixel on the main board with the specified values, and on a source
//! specific board to keep track of submitted images from each IP address.
//! \param sourceAddr IPV6 source address
//! \param y
//! \param x
//! \param a
//! \param r
//! \param g
//! \param b
void JingleController::drawPixel(uint64_t sourceAddr, int y, int x, uint8_t a, uint8_t r, uint8_t g, uint8_t b) {
    // Find the private buffer of this source to update the image
    JingleBuffer sourceFrame = sourceFrames[sourceAddr];
    sourceFrame.setPixel(y, x, a, r, g, b);

    // Put the pixel on the main board
    mainBuffer.setPixel(y, x, a, r, g, b);
}

//! Draw a pixel on the Jingle board
//! puts a pixel on the main board with the specified values, and on a source
//! specific board to keep track of submitted images from each IP address.
//! \param sourceAddr IPV6 source address
//! \param y
//! \param x
//! \param value
void JingleController::drawPixel(uint64_t sourceAddr, int y, int x, uint32_t value) {
    // Find the private buffer of this source to update the image
    JingleBuffer sourceFrame = sourceFrames[sourceAddr];
    sourceFrame.setPixel(y, x, value);

    // Put the pixel on the main board
    mainBuffer.setPixel(y, x, value);
}

cv::Mat JingleController::get4Buffers() {
    int width = mainBuffer.getBuffer().cols;
    int height = mainBuffer.getBuffer().rows;
    cv::Mat matDst = cv::Mat::zeros(cv::Size(width * 2, height * 2), CV_8UC4);

    // Cut first buffer
    cv::Mat matRoi = matDst(cv::Rect(0, 0, width, height));
    mainBuffer.getBuffer().copyTo(matRoi);

    // Cut second buffer
    matRoi = matDst(cv::Rect(width, 0, width, height));
    sourceFrames[1u].getBuffer().copyTo(matRoi);

    // Cut third buffer
    matRoi = matDst(cv::Rect(0, height, width, height));
    sourceFrames[2u].getBuffer().copyTo(matRoi);

    // Cut fourth buffer
    matRoi = matDst(cv::Rect(width, height, width, height));
    sourceFrames[3u].getBuffer().copyTo(matRoi);

    return matDst;
}
