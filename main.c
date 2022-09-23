#include <stdio.h>
#include "traitement.h"

int main(int argc, char ** argv)
{
    if(argc==2)
    {
        int end= download(argv[1]);
        print_pic(argv[1]);
        return end;
    }
    else
        return -2;
}
