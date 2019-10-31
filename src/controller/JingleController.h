#ifndef JINGLE_BUFFERS_JINGLECONTROLLER_H
#define JINGLE_BUFFERS_JINGLECONTROLLER_H

#include <filesystem>
#include <unordered_map>
#include <unordered_set>
#include <shared_mutex>
#include "../draw/JingleBuffer.h"

class JingleController {
public:
    JingleController(std::filesystem::path &blacklistFile);

    void drawPixel(uint64_t sourceAddr, int y, int x, uint8_t a, uint8_t r, uint8_t g, uint8_t b);

    void drawPixel(uint64_t sourceAddr, int y, int x, uint32_t value);

    void addToBlacklist(uint64_t srcAddr);

    void removeFromBlacklist(uint64_t srcAddr);

    std::unordered_set<uint64_t> getBlacklist();

    cv::Mat getMainBuffer();

    cv::Mat getBuffers();

private:
    std::unordered_map<uint64_t, JingleBuffer> sourceFrames = {};
    std::unordered_set<uint64_t> blacklist = {0xfe80000000000000};
    std::filesystem::path blacklistFile;
    JingleBuffer mainBuffer;
    std::shared_mutex sourceFramesLock;

    void saveBlackList(uint64_t sourceAddr);
};

#endif //JINGLE_BUFFERS_JINGLECONTROLLER_H
