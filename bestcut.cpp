#include <gd.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <string.h>

#include "genome.h"

#define CHANNEL_LENGTH 50 
#define FRAMES_TO_BE_CUT  26 
#define GENE_POOL_SIZE    30

#define MUTATION_RATE 10 
//                       0    1   2  3   4  5  6  7   8   9  10  11  12  13  14  15  16  17  18  19  20  21 22 23 24  25
int frame_to_be_cut[] = {23, 24, 25, 3, 27, 1, 1, 1, 20, 20, 24, 43, 37, 28, 30, 33, 33, 33, 50, 10, 23, 26, 2, 3, 7, 24};


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

        //my_gene.randomize_genome();
        my_gene.crossover();
        my_gene.dump_genome();

        genome_operator g_op;
        genome *gene_pool[GENE_POOL_SIZE];

        /* Feed in the params, including channel length */

        g_op.set_channel_size(CHANNEL_LENGTH);
        g_op.set_req_frame_sizes(frame_to_be_cut, FRAMES_TO_BE_CUT);

        /* Create the gene  pool */
        g_op.create_genome_pool(gene_pool, GENE_POOL_SIZE, FRAMES_TO_BE_CUT);

        int iteration = 0;

        while (iteration++ < 1000) {

            g_op.sort_gene_pool(gene_pool, GENE_POOL_SIZE);

            g_op.crossover_genepool(gene_pool, GENE_POOL_SIZE, MUTATION_RATE);

            g_op.calc_fitness_of_pool(gene_pool, GENE_POOL_SIZE);

            printf("iteration = %d\n", iteration);
            //gene_pool[0]->dump_genome();

        }

        gene_pool[0]->dump_genome(frame_to_be_cut, FRAMES_TO_BE_CUT, CHANNEL_LENGTH);
}



