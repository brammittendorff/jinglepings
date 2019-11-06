#ifndef JINGLEPINGS_SERVER_TUI_H
#define JINGLEPINGS_SERVER_TUI_H

#include "../controller/JingleController.h"

//! \brief TUI errors.
class TuiException : public std::runtime_error {
public:
    TuiException(std::string const &msg) :
            std::runtime_error(msg) {}
};

//! \brief TUI that allows manual controller control.
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
