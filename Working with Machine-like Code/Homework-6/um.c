#include <stdio.h>
#include <stdlib.h>
#include "execution.h"

int main(int argc, char* argv[])
{
        if (argc == 0) {
                return 1;
        }
        run_machine(argv[1]);
        
        exit(EXIT_SUCCESS);
}
