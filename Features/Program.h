#pragma once
#ifndef PROGRAM_H
#define PROGRAM_H

#include "../Features/UserManager.h"
#include "../Libs/Panel.h"

class Program {
    private:
        Map<User> userMap;
        UserManager userManager;
        Panel panel;

    public:
        Program(u_int32 size);

        u_int8 Init();
        void Test();
};

#endif
