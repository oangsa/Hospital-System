#ifndef PANEL_H
#define PANEL_H

#include "../Features/UserManager.h"
#include "../Features/UndoManager.h"
#include "../Libs/Map.h"
#include "Define.h"

class Panel {
    private:
        User* LoggedUser = NULL;
        UserManager& userManager;
        UndoManager undoManager;
        Map<User>& userMap;

    public:
        Panel(UserManager& userManager, Map<User>& userMap);
        void clearScreen();
        void showUserInfo();
        void delay(u_int8 secs);
        void loginPanel(u_int8 attempt, u_int8 isFileExist);
        void adminMenu();
        void patientMenu();
        void doctorMenu();
        void nurseMenu();

        // Admin Section
        void addUserPanel();
        void removeUserPanel();
};

#endif
