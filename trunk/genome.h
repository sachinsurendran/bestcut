#define SUCCESS 1
#define FAIL    0

#define MAX_CHAR 17


#define RANGE 1000
#define MUTATION_PROPABLITY 0.0901 /* 50 percent */
#define MUTATION_FACTOR (RANGE * MUTATION_PROPABLITY) 

#define WORST_FITNESS 0xffff

typedef struct pixel {
    int val;
    char c;
}pixel_t;

pixel_t data[]={{0, ' '},\
    {1, '`'},\
    {2, '.'},\
    {3, ','},\
    {4, '-'},\
    {5, '~'},\
    {6, ':'},\
    {7, '"'},\
    {8, '!'},\
    {9, 'i'},\
    {10,'1'},\
    {11,'L'},\
    {12,'S'},\
    {13,'H'},\
    {14,'B'},\
    {15,'Q'},\
    {16,'#'},\
    {MAX_CHAR,'X'}};




class genome{
    private:
        int fitness;
        int gene_len;
        unsigned int *genome_ptr;

    public:
        friend class genome_operator;

        genome(int len);
        ~genome();
        int get_fitness();
        void set_fitness(int );
        void randomize_genome();
        void mutate();
        void mutate(float percent);
        void dump_genome();
        void load_param_to_gene(int *params);

};

void genome::dump_genome()
{
    printf("Gene :");
    for (int indx = 0; indx < gene_len; indx++) {
        printf(" %d:", *(genome_ptr+indx));
    }
    printf("\n");
}

genome::genome(int len)
{
    gene_len = len;
    genome_ptr = (unsigned int *) malloc(len * sizeof(int));
    
    for (int indx=0; indx < gene_len; indx++)
    {
        *(genome_ptr+indx) = indx;
    }
}

genome::~genome()
{
    free(genome_ptr);
    printf("Destroying Genome\n");
}

void genome::load_param_to_gene(int *params)
{
    memcpy(genome_ptr, params, gene_len);
}


void genome::randomize_genome()
{
    int tmp, rand_indx;
    for (int indx = 0; indx <  gene_len; indx++)
    {
        tmp = genome_ptr[indx];
        rand_indx = (rand() % gene_len);

        *(genome_ptr + indx) = *(genome_ptr + rand_indx);
        *(genome_ptr+rand_indx) = tmp;
    }

}

void genome::mutate()
{
    int tmp,rand_indx;

    for (int indx=0; indx < gene_len; indx++)
    {
        if ((rand() % RANGE) <  MUTATION_FACTOR)
           {
               /* Mutate the genome */
               tmp = genome_ptr[indx];
               rand_indx = (rand() % gene_len);
               /* Swap current value with a randomly generated position */
               *(genome_ptr + indx) = *(genome_ptr + rand_indx);
               *(genome_ptr+rand_indx) = tmp;
           }
    }
}

void genome::mutate(float percent)
{
    int tmp, rand_indx;

    for (int indx=0; indx < gene_len; indx++)
    {
        if ((rand() % RANGE) <  (percent * RANGE/100))
           {
               /* Mutate the genome */
               tmp = genome_ptr[indx];
               rand_indx = (rand() % gene_len);
               /* Swap current value with a randomly generated position */
               *(genome_ptr + indx) = *(genome_ptr + rand_indx);
               *(genome_ptr+rand_indx) = tmp;
           }
    }
}

int genome::get_fitness()
{
    return fitness;
}

void genome::set_fitness(int new_fitness)
{
    fitness = new_fitness;
}

#ifdef DONT_COMPILE

/*
 *
 * GENOME OPERATOR:
 *
 */

class genome_operator{

    public:

        void crossover(genome *parent_gen1,
                genome *parent_gen2,
                genome *child_gen1,
                genome *child_gen2);
        int calculate_fitness(genome *genome, genome *target_genome);
        void pixelize(genome *some_genome);
        void sort_gene_pool(genome *head[], int gene_pool_size);
        void calc_fitness_of_pool(genome *head[], int gene_pool_size, genome *target_genome);
        void crossover_genepool(genome *head[], int gene_pool_size);
        void crossover_genepool(genome *head[], int gene_pool_size, float mutation_percent);
        void create_genome_pool(genome *head[], int gene_pool_size, int gene_X_size, int gene_Y_size);
};

void genome_operator::create_genome_pool(genome *head[], int gene_pool_size, int gene_X_size, int gene_Y_size)
{
    for (int count=0; count < gene_pool_size; count++)
    {
        head[count] = new genome(gene_X_size, gene_Y_size);
        head[count]->randomize_genome();
    }
}

void genome_operator::calc_fitness_of_pool(genome *head[], int gene_pool_size, genome *target_genome)
{
    int indx;

    for (indx = 0; indx < gene_pool_size; indx++)
    {
        calculate_fitness(head[indx], target_genome);
        //printf(" Fitness [%d] = %d\n", indx, head[indx]->get_fitness());
    }
    //printf("---------------------------------\n");
}

void genome_operator::sort_gene_pool(genome *head[], int gene_pool_size)
{
    genome *temp_genome_ptr;
    int indx, loop_within;

    for (indx = 0; indx < gene_pool_size/2; indx++)
    {
        for (loop_within = 0; loop_within < (gene_pool_size-1); loop_within++)
        {
            if (head[loop_within]->get_fitness() > head[loop_within+1]->get_fitness())
            {
                temp_genome_ptr = head[loop_within];
                head[loop_within] = head[loop_within+1];
                head[loop_within+1] = temp_genome_ptr;
            }
        }
    }
//#define DEBUG
#ifdef DEBUG
    /* Print fitness of best parent */
    printf("---------------------------------\n");
    printf(" Best Fitness [%d]  = %d\n", 0, head[0]->get_fitness());
    /* Print fitness of worst genome */
    printf(" Worst Fitness [%d] = %d\n", 0, head[5]->get_fitness());
    printf("---------------------------------\n");
#endif

}


void genome_operator::pixelize(genome *some_genome)
{
    unsigned char indx = 0;
    unsigned char i,j;
    int x = some_genome->x;
    int y = some_genome->y;

    printf("Pixelize\n");

    if (some_genome->genome_ptr) 
    {
        for (i=0; i<x; i++) {
            for (j=0; j<y; j++){
                //printf(" some_genome->genome_ptr[%d][%d] = %d\n", i, j, (char) (* (some_genome->genome_ptr + (x*i) + j)));
                indx = 0;
                while(data[indx++].val != (char) (* (some_genome->genome_ptr + (x*i) + j)) && (indx <= MAX_CHAR) )
                {
                    ;
                }
                //printf("%02d ", indx-1);
                if (indx == MAX_CHAR)
                    printf("%c", data[MAX_CHAR -1].c);
                else
                    printf("%c", data[indx-1].c);

            }
            printf("\n");
        }
    }
}



int genome_operator::calculate_fitness(genome *some_genome, genome *target_genome)
{
    int max_x = some_genome->x;
    int max_y = some_genome->y;
    int fitness = 0;
    int x, y;


    for (x = 0; x < max_x; x++)
    {
        for (y = 0; y < max_y; y++)
        {
            int diff = *(some_genome->genome_ptr + (x * max_x) + y) - *(target_genome->genome_ptr + (x * max_x) + y);

            if (diff > 0)
                fitness += diff;
            else
                fitness -= diff;
        }
    }
    some_genome->set_fitness(fitness);
    //printf("fitness = %d\n", fitness);
}

void genome_operator::crossover_genepool(genome *head[], int gene_pool_size)
{
    for(int indx = 2; indx < gene_pool_size; indx +=  2)
    {
        /* Crossover parent1 & 2 and produce all the children */
        crossover(head[0], head[1],
                head[indx], head[indx+1]);
    }
}

void genome_operator::crossover_genepool(genome *head[], int gene_pool_size, float mutation_percent)
{
    for(int indx = 2; indx < gene_pool_size; indx +=  2)
    {
        /* Crossover parent1 & 2 and produce all the children */
        crossover(head[0], head[1],
                head[indx], head[indx+1]);
        head[indx]->mutate(mutation_percent);
        head[indx+1]->mutate(mutation_percent);
    }
}


void genome_operator::crossover(genome *parent_gen1,
                        genome *parent_gen2,
                        genome *child_gen1,
                        genome *child_gen2)
{
    int x,y;
    /* Get the crossover limits */
    int crossover_X = (rand() % parent_gen1->x);
    int crossover_Y = (rand() % parent_gen1->y);
    int max_x = parent_gen1->x;
    int max_y = parent_gen1->y;

    for (x = 0; x < crossover_X; x++)
    {
        for (y = 0; y < crossover_Y; y++)
        {
            *(child_gen1->genome_ptr + (x * max_x) + y) 
                = *(parent_gen1->genome_ptr + (x*max_x) + y);
            *(child_gen2->genome_ptr + (x * max_x) + y)
                = *(parent_gen2->genome_ptr + (x*max_x) + y);
        }
    }
    for (x = crossover_X; x < max_x; x++)
    {
        for (y = crossover_Y; y < max_y; y++)
        {
            *(child_gen1->genome_ptr + (x * max_x) + y) 
                = *(parent_gen1->genome_ptr + (x*max_x) + y);
            *(child_gen2->genome_ptr + (x * max_x) + y)
                = *(parent_gen2->genome_ptr + (x*max_x) + y);
        }
    }

    for (x = 0; x < crossover_X; x++)
    {
        for (y = crossover_Y; y < max_y; y++)
        {
            *(child_gen1->genome_ptr + (x * max_x) + y)
                = *(parent_gen2->genome_ptr + (x*max_x) + y);
            *(child_gen2->genome_ptr + (x * max_x) + y)
                = *(parent_gen1->genome_ptr + (x*max_x) + y);
        }
    }

    for (x = crossover_X; x < max_x; x++)
    {
        for (y = 0; y < crossover_Y; y++)
        {
            *(child_gen1->genome_ptr + (x * max_x) + y)
                = *(parent_gen2->genome_ptr + (x*max_x) + y);
            *(child_gen2->genome_ptr + (x * max_x) + y)
                = *(parent_gen1->genome_ptr + (x*max_x) + y);
        }
    }
}

#endif /* DONT_COMPILE */ 
