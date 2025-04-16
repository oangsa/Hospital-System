#include "Libs/Logger.h"
#include "Features/Program.h"

int main() {
    Logger logger;
    Program program(20);
    logger.log("test %d", 1);
    logger.log("test2");
    
    program.Init();

    return 0;
}
