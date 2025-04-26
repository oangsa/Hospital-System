#ifndef UNDO_MANAGER_H
#define UNDO_MANAGER_H

#include "../Libs/Define.h"
#include "../Libs/Stack.h"
#include "UserManager.h"

class UndoManager {
private:
  Stack<undo_t> actionStack;
  UserManager &userManager;

public:
  UndoManager(UserManager &userManager);
  void pushAction(undo_t *action);
  undo_t popAction();
  undo_t peekAction();
  void undoAction();
  void clear();
};

#endif
