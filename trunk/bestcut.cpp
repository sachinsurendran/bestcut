#include <gd.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <string.h>

#include "genome.h"

int main() {

	gdImagePtr im; //declaration of the image
	int black,white,x,y;
	time_t time_now;

	time_now = time(NULL);

	printf(" Time = %d\n", (int ) time_now);

	srand((int)time_now);

        /* Create genome */

        genome my_gene(10);

        my_gene.dump_genome();

        my_gene.randomize_genome();
        my_gene.dump_genome();
}



