#pragma once
#ifndef PROGRAM_H
#define PROGRAM_H

#include "../Features/UserManager.h"
#include "../Libs/Panel.h"

class Program {
    private:
        Panel panel;
        UserManager userManager;
        Map<User> userMap;

    public:
        Program(u_int32 size);

        u_int8 Init();
        void Test();
};

#endif
