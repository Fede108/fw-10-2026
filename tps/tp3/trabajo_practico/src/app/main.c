#include "demo.h"
#include "print.h"
#include "strutils.h"

#define PROGRAM_NAME "building_process_demo"
#define PROGRAM_VERSION "1"


int main(int argc, char *argv[])
{
    if (argc == 2 && str_equal(argv[1], "--version")){
        print_str(PROGRAM_NAME);
        print_str(PROGRAM_VERSION);
        print_str("\n");
    } else {
        demo_math();
        demo_str();
        demo_embedded();
    }
    return 0;
}
