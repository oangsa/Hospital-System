/**************************************************
 *                                                *
 *     UndoManager Class Implementation File      *
 *     Latest Update: Unknown                     *
 *                                                *
 **************************************************/


#include "UndoManager.h"
#include "UserManager.h"
#include <iostream>

/*
    Constructor
    Params: UserManager& userManager
*/
UndoManager::UndoManager(UserManager& userManager): userManager(userManager) {};

/*
    Push the action to the stack
    Params: undo_t* action
*/
void UndoManager::pushAction(undo_t* action) {
    this->actionStack.push(*action);
}

/*
    Pop the action from the stack
    Return: if the stack is empty, return an empty action,
    else return the action
*/
undo_t UndoManager::popAction() {
    user_t user;
    undo_t u = {
        .actionType = ACTION_TYPE::EMPTY_ACTION,
        .user = user
    };

    if (this->actionStack.isEmpty()) {
        return u;
    }

    return *this->actionStack.pop();
}

/*
    Peek the action from the stack
    Return: if the stack is empty, return an empty action,
    else return the action
*/
undo_t UndoManager::peekAction() {
    user_t user;
    undo_t u = {
        .actionType = ACTION_TYPE::EMPTY_ACTION,
        .user = user
    };

    if (this->actionStack.isEmpty()) {
        return u;
    }

    return *this->actionStack.peek();
}

/*
    Undo the last action
    This function will revert the last action in the stack
*/
void UndoManager::undoAction() {
    undo_t action = this->popAction();

    if (action.actionType == ACTION_TYPE::EMPTY_ACTION) {
        std::cout << "There's nothing no revert.\n";
        return;
    }

    User user = action.user;
    switch (action.actionType) {
        case ACTION_TYPE::USER_DELETE:
            this->userManager.addUser(user);
            this->userManager.saveToFile("Database/Users/users.csv");
            break;

        case ACTION_TYPE::USER_ADD:
            this->userManager.removeUser(user.getID());
            this->userManager.saveToFile("Database/Users/users.csv");
            break;

        case ACTION_TYPE::USER_UPDATE:
        case ACTION_TYPE::HISTORY_DELETE:
        case ACTION_TYPE::EMPTY_ACTION:
            std::cout << "This cannot revert.\n";
            break;
    }
}

/*
    Clear the stack
*/
void UndoManager::clear() {
    this->actionStack.clearStack();
}
