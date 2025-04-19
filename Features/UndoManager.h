#ifndef UNDO_MANAGER_H
#define UNDO_MANAGER_H

#include "../Libs/define.h"
#include "UserManager.h"
#include "../Libs/Stack.h"

class UndoManager {
    private:
        Stack<undo_t> actionStack;
        UserManager &userManager;

    public:
        UndoManager(UserManager &userManager);
        void pushAction(undo_t* action);
        undo_t popAction();
        undo_t peekAction();
        void undoAction();
};

#endif
