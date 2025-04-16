#ifndef PANEL_H
#define PANEL_H

#include "../Features/UserManager.h"
#include "../Libs/Map.h"
#include "Define.h"

class Panel {
    private:
        User* LoggedUser = NULL;
        UserManager& userManager;
        Map<User>& userMap;
    
    public:
        Panel(UserManager& userManager, Map<User>& userMap);
        void clearScreen();
        void delay(u_int8 secs);
        void loginPanel();
        // void showLogin();
        // void showMainMenu();
        // void showUserMenu();
        // void showDoctorMenu();
        // void showNurseMenu();
        // void showAdminMenu();
};

#endif
