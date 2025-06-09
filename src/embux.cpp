#include "neoembux.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if(argc <= 1) {
        printf("Usage:\n");
        return EXIT_FAILURE;
    }
    neoEmbUx::ioSetup();
    if(!strcmp(argv[1], "readall")) {
        printf("+----+---------------+------+-------+------+---------------+----+\n");
        printf("|Val.|               | GPIO |  Pin  | GPIO |               |Val.|\n");
        printf("+----+---------------+------+---+---+------+---------------+----+\n");
        for(int i = 0; i < EMBUX_GPIO_NUM; i+=2) {
            int acc_pin_l = i + 1;
            int acc_pin_r = i + 2;

            printf("| %-2i |               |   %s  |%2i | %-2i|  %s   |               | %2i |\n", 
                neoEmbUx::checkPin(acc_pin_l)?neoEmbUx::readPin(acc_pin_l):-1, neoEmbUx::checkPin(acc_pin_l)?"√":" ", acc_pin_l, 
                acc_pin_r, neoEmbUx::checkPin(acc_pin_r)?"√":" ", neoEmbUx::checkPin(acc_pin_r)?neoEmbUx::readPin(acc_pin_r):-1);
        }
        printf("+----+---------------+-----+---+---+-------+---------------+----+\n");
        neoEmbUx::ioRelease();
        return EXIT_SUCCESS;
    } else {
        printf("Usage:\n readall\n");
        return EXIT_FAILURE;
    }
}