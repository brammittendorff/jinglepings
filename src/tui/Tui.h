#ifndef JINGLEPINGS_SERVER_TUI_H
#define JINGLEPINGS_SERVER_TUI_H

#include "../controller/JingleController.h"

class TuiException : public std::runtime_error {
public:
    TuiException(std::string const &msg) :
            std::runtime_error(msg) {}
};

class Tui {
private:
    JingleController &controller;

public:
    Tui(JingleController &controller);

    void run();

private:
    void runCommand(std::string &cmd);

    void cmdAdd();

    void cmdDel();

    void cmdList();
};

#endif //JINGLEPINGS_SERVER_TUI_H
