//
// Created by Hadar on 15/01/2023.
//

#ifndef EX4_COMMAND2_ALGORITHMSETTINGS_H
#define EX4_COMMAND2_ALGORITHMSETTINGS_H

#include "Command.h"

class Command2_AlgorithmSettings : Command{
public:
    Command2_AlgorithmSettings(DefaultIO dio, Data* data);
    void execute();
};


#endif //EX4_COMMAND2_ALGORITHMSETTINGS_H