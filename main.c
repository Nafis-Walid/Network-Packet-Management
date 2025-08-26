#include <stdio.h>
#include "src/app.h"

/*gcc main.c src/*.c -o app && valgrind --leak-check=full --show-leak-kinds=all ./app*/
int main()
{

    PRESS_ANY_KEY_TO_CONTINUE;
    run_packet_management_project();

}