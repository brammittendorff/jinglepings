#include "JingleController.h"

//! Constructor for Jingleping Controller.
//! Initialises a blacklist from a blacklist file if it exists.
//! \param blacklistFile file to store/retrive blacklist from.
JingleController::JingleController(std::filesystem::path &blacklistFile) : blacklistFile(blacklistFile) {
    // Get blacklist from disk if possible
    if (std::filesystem::exists(blacklistFile)) {
        std::ifstream bin(blacklistFile);
        while (bin) {
            uint64_t prefix;
            bin >> std::hex >> prefix;
            blacklist.emplace(prefix);
        }
        bin.close();
    }
}

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
    // Check if sourceAddr is known
    sourceFramesLock.lock_shared();
    if (sourceFrames.find(sourceAddr) == sourceFrames.end()) {
        sourceFramesLock.unlock_shared();
        sourceFramesLock.lock();

        // Re-check in case another thread added it
        sourceFrames.try_emplace(sourceAddr, JingleBuffer());

        // The ayes have it, unlock!
        sourceFramesLock.unlock();
        sourceFramesLock.lock_shared();
    }

    // Set the pixel
    sourceFrames[sourceAddr].setPixel(y, x, a, r, g, b);
    sourceFramesLock.unlock_shared();

    // Put the pixel on the main board if the source has not been blacklisted
    if (blacklist.find(sourceAddr) == blacklist.end()) {
        mainBuffer.setPixel(y, x, a, r, g, b);
    }
}

//! Draw a pixel on the Jingle board
//! puts a pixel on the main board with the specified values, and on a source
//! specific board to keep track of submitted images from each IP address.
//! \param sourceAddr upper 64 bits of the IPV6 source address
//! \param y
//! \param x
//! \param value
void JingleController::drawPixel(uint64_t sourceAddr, int y, int x, uint32_t value) {
    // Check if sourceAddr is known
    sourceFramesLock.lock_shared();
    if (sourceFrames.find(sourceAddr) == sourceFrames.end()) {
        sourceFramesLock.unlock_shared();
        sourceFramesLock.lock();

        // Re-check in case another thread added it
        sourceFrames.try_emplace(sourceAddr, JingleBuffer());

        // The ayes have it, unlock!
        sourceFramesLock.unlock();
        sourceFramesLock.lock_shared();
    }

    // Find the private buffer of this source to update the image
    sourceFrames[sourceAddr].setPixel(y, x, value);
    sourceFramesLock.unlock_shared();

    // Put the pixel on the main board if the source has not been blacklisted
    blacklistLock.lock_shared();
    if (blacklist.find(sourceAddr) == blacklist.end()) {
        mainBuffer.setPixel(y, x, value);
    }
    blacklistLock.unlock_shared();
}

//! Add a source address identifier to the blacklist.
//! \param sourceAddr upper 64 bits of the IPV6 source address
void JingleController::addToBlacklist(uint64_t sourceAddr) {
    blacklistLock.lock();
    blacklist.emplace(sourceAddr);
    blacklistLock.unlock();
    this->saveBlackList();
}

//! Remove a source address identifier to the blacklist.
//! \param sourceAddr upper 64 bits of the IPV6 source address
void JingleController::removeFromBlacklist(uint64_t sourceAddr) {
    blacklistLock.lock();
    blacklist.erase(sourceAddr);
    blacklistLock.unlock();
    this->saveBlackList();
}

//! Save the blacklist to file.
void JingleController::saveBlackList() {
    std::ofstream bout(blacklistFile);
    blacklistLock.lock_shared();
    for (const auto prefix : blacklist) {
        bout << std::hex << std::setfill('0') << std::setw(16) << prefix << std::endl;
    }
    blacklistLock.unlock_shared();
    bout.close();
}

//! Get the blacklist.
//! \return the blacklist
std::unordered_set<uint64_t> JingleController::getBlacklist() {
    return blacklist;
}

//! Get the main buffer.
//! \return the main buffer
cv::Mat JingleController::getMainBuffer() {
    return mainBuffer.getBuffer();
}

//! Convert a 64-bit subnet ID to a hex string.
//! \return hexadecimal representation of the subnet ID.
static inline std::string idToHex(uint64_t id) {
    char label[17];
    std::snprintf(label, sizeof label, "%016lx", id);
    return std::string(label);
}

//! Get a single matrix containing the main buffer,
//! and all source frames with an annotation.
//! \return matrix with all buffer contents.
cv::Mat JingleController::getBuffers() {
    int width = mainBuffer.getBuffer().cols;
    int height = mainBuffer.getBuffer().rows;
    cv::Mat matDst = cv::Mat::zeros(cv::Size(width, height * (sourceFrames.size() + 1)), CV_8UC4);

    // Draw first buffer
    cv::Mat matRoi = matDst(cv::Rect(0, 0, width, height));
    mainBuffer.getBuffer().copyTo(matRoi);

    // Draw other buffers
    auto h = 0;

    for (const auto &buf: sourceFrames) {
        auto c = 0;
        auto mat = buf.second.getBuffer();

        cv::MatConstIterator_<int32_t> it = mat.begin<int32_t>(), it_end = mat.end<int32_t>();
        for(; it != it_end; ++it) {
            if (*it != 0) {
                c++;
            }
            if (c >= 16) {
                break;
            }
        }

        if (c < 16) {
            continue;
        }

        matRoi = matDst(cv::Rect(0, h += height, width, height));
        buf.second.getBuffer().copyTo(matRoi);

        cv::rectangle(matRoi, cv::Rect(0, 0, 160, 16), cv::Scalar(0, 0, 0, 255), CV_FILLED);
        cv::putText(matRoi, idToHex(buf.first), cv::Point(0, 14), cv::FONT_HERSHEY_PLAIN, 1,
                    cv::Scalar(255, 255, 255, 255), 1);
    }

    return matDst.rowRange(0, h+height);
}
