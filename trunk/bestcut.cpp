#include <gd.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <string.h>


#define CHANNEL_LENGTH 5000 
#define FRAMES_TO_BE_CUT  27 

#define GENE_POOL_SIZE  50

/* Number of parents from previous generation to take to next gen */
#define KEEPALIVE_POPULATION 5 

#define MUTATION_RATE 5 

#include "genome.h"

//                       0    1   2  3   4  5  6  7   8   9  10  11  12  13  14  15  16  17  18  19  20  21 22 23 24  25
int frame_to_be_cut[] = {2300, 2400, 2500, 300, 2700, 1000, 1200, 1500, 500, 800, 2400, 3000, 3700, 2800, 3000, 3300, 3300, 3300, 1500, 1000, 2300, 2600, 2000, 3000, 700, 2400, 2000};


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

        while (iteration++ < 30000) {

            g_op.sort_gene_pool(gene_pool, GENE_POOL_SIZE);

            g_op.crossover_genepool(gene_pool, GENE_POOL_SIZE, MUTATION_RATE);

            g_op.calc_fitness_of_pool(gene_pool, GENE_POOL_SIZE);

            //g_op.kill_duplicate_parents(gene_pool, GENE_POOL_SIZE);

            printf("iteration = %d\n", iteration);
            //gene_pool[0]->dump_genome();

        }

        //for(int i=0; i< GENE_POOL_SIZE; i++)
        gene_pool[0]->dump_genome(frame_to_be_cut, FRAMES_TO_BE_CUT, CHANNEL_LENGTH);
}



