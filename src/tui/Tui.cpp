#include <thread>
#include "Tui.h"

Tui::Tui(JingleController &controller) : controller(controller) {}

void Tui::run() {
    using namespace std::chrono_literals;

    while (true) {
        std::cerr << std::endl << "> ";

        std::string cmd;
        std::cin >> cmd;

        if (std::cin.fail()) {
            std::cerr << "Exiting TUI. Use Ctrl+C to exit the program." << std::endl;
            return;
        }

        try {
            this->runCommand(cmd);
        } catch (TuiException e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }

        if (std::cin.fail()) {
            std::cin.clear();
        }
    }
}

void Tui::runCommand(std::string &cmd) {
    if (cmd == "help") {
        std::cerr << "Commands are: " << std::endl
                  << "  add <prefix>    add a prefix to the blacklist" << std::endl
                  << "  del <prefix>    remove a prefix from the blacklist" << std::endl
                  << "  ls              show all prefixes on the blacklist" << std::endl;
    } else if (cmd == "add") {
        this->cmdAdd();
    } else if (cmd == "del") {
        this->cmdDel();
    } else if (cmd == "ls") {
        this->cmdList();
    } else {
        throw TuiException("Unknown command '" + cmd + "'");
    }
}

static uint64_t getPrefix() {
    uint64_t prefix;
    std::cin >> std::hex >> prefix;
    return prefix;
}

void Tui::cmdAdd() {
    auto prefix = getPrefix();
    controller.addToBlacklist(prefix);
    std::cerr << "Added to blacklist:" << std::hex << std::setfill('0') << std::setw(16) << prefix << std::endl;
}

void Tui::cmdDel() {
    auto prefix = getPrefix();
    controller.removeFromBlacklist(prefix);
    std::cerr << "Removed from blacklist:" << std::hex << std::setfill('0') << std::setw(16) << prefix << std::endl;
}

void Tui::cmdList() {
    for (const auto prefix: controller.getBlacklist()) {
        std::cerr << std::hex << std::setfill('0') << std::setw(16) << prefix << std::endl;
    }
}