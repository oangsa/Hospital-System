#ifndef PANEL_H
#define PANEL_H

#include "../Features/UserManager.h"
#include "../Features/UndoManager.h"
#include "../Libs/Map.h"
#include "../Libs/PriorityQueue.h"
#include "../Libs/Queue.h"
#include "../Libs/Logger.h"
#include "Define.h"
#include "Validator.h"

class Panel {
    private:
        UserManager& userManager;
        Map<User>& userMap;
        UndoManager undoManager;
        User* LoggedUser = NULL;
        Validator validator;
        Logger logger;

    public:
        Panel(UserManager& userManager, Map<User>& userMap);
        void clearScreen();
        void showUserInfo();
        void delay(u_int8 secs);
        void loginPanel(u_int16 attempt);
        void mainMenu(u_int8 isFileExist);
        void switchPanel();
        void registerPanel();
        void adminMenu();
        void patientMenu();
        void doctorMenu();
        void nurseMenu();

        // Admin Section
        void addUserPanel();
        void removeUserPanel();
        void updateUserPanel();

        // Patient Section
        void showHistories();
        void showRemaining();

        // Nurse Section
        ESI_LEVEL determine();
        u_int8 askYesNo(string question);
        void nurseProcessPatientPanel();
        void askAddAllergies(Patient* patient);

        // Doctor Section
        void doctorProcessPatientPanel();
        void addRecordPanel(Patient* user, PatientHistory* history);
};

#endif
