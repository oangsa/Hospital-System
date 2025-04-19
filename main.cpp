#include "Libs/Logger.h"
#include "Features/Program.h"

int main() {
    Logger logger;
    Program program(20);
    
    program.Init();

    return 0;
}
