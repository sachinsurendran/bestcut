#define SUCCESS 1
#define FAIL    0

#define MAX_CHAR 17


#define RANGE 1000
#define MUTATION_PROPABLITY 0.0901 /* 50 percent */
#define MUTATION_FACTOR (RANGE * MUTATION_PROPABLITY) 

/* Number of parents from previous generation to take to next gen */
#define KEEPALIVE_POPULATION 5 

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
        void copy_genome(genome *src_genome);
        void mutate();
        void mutate(float percent);
        void dump_genome();
        void dump_genome(int *required_frames, int no_of_frames, int channel_size);
        void load_param_to_gene(int *params);
        void crossover();

};

void genome::copy_genome(genome *src_genome)
{
    bcopy(src_genome->genome_ptr, genome_ptr, (sizeof(int) * gene_len));
}


void genome::crossover()
{
    int *tmp_gene_ptr;
    int crossover_point;

    tmp_gene_ptr = (int *) malloc(sizeof(int) * gene_len);

    bcopy(genome_ptr, tmp_gene_ptr, gene_len * sizeof(int));

    crossover_point = (rand() % gene_len);
#define DEBUG
#ifdef DEBUG
    printf("Crossover point = %d\n", crossover_point);
#endif /* DEBUG */
    /* Move segment from crossover point to end of the gene, to start 
       Note: bcopy(src, dst, len ) */
    bcopy((void *) (tmp_gene_ptr + crossover_point), (void *)genome_ptr,
            (gene_len - crossover_point) * sizeof(int));
    /* Move segment start to crossover to the end */
    bcopy((void *) (tmp_gene_ptr), (void *) (genome_ptr + (gene_len - crossover_point)),
            (crossover_point * sizeof(int)));

    free (tmp_gene_ptr);
}





void genome::dump_genome()
{
    printf("Gene :");
    for (int indx = 0; indx < gene_len; indx++) {
        printf(" %d:", *(genome_ptr+indx));
    }
    printf("\n");
}

void genome::dump_genome(int *required_frames, int no_of_frames, int channel_size)
{
    int used_up_size = 0;
    int indx = 0;
    while(indx < gene_len) {

        if (used_up_size + required_frames[*(genome_ptr+indx)] <= channel_size) {

            printf(" %d:", required_frames[*(genome_ptr+indx)]);
            used_up_size += required_frames[*(genome_ptr+indx)];
            /* Next frame */
            indx++;
        } else {
            printf ( "\n Next Channel \n");
            used_up_size = 0;
        }

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


/*
 *
 * GENOME OPERATOR:
 *
 */

class genome_operator{

    int *required_frames;
    int channel_size;

    public:
        ~genome_operator();
        int calculate_fitness(genome *genome);
        void sort_gene_pool(genome *head[], int gene_pool_size);
        void calc_fitness_of_pool(genome *head[], int gene_pool_size);
        void crossover_genepool(genome *head[], int gene_pool_size);
        void crossover_genepool(genome *head[], int gene_pool_size, float mutation_percent);
        void create_genome_pool(genome *head[], int gene_pool_size, int gene_size);
        void set_channel_size(int ch_size);
        void set_req_frame_sizes(int *req_frames, int no_of_frames);
};

genome_operator::~genome_operator()
{
    free(required_frames);
}

void genome_operator::set_req_frame_sizes(int *req_frames, int no_of_frames)
{
    required_frames = (int *) malloc(no_of_frames * sizeof(int));
    memcpy(required_frames, req_frames, (no_of_frames * sizeof(int)));
}


void genome_operator::set_channel_size(int ch_size)
{
    channel_size = ch_size;
}

void genome_operator::create_genome_pool(genome *head[], int gene_pool_size, int gene_size)
{
    for (int count=0; count < gene_pool_size; count++)
    {
        head[count] = new genome(gene_size);
        head[count]->randomize_genome();
    }
}

void genome_operator::calc_fitness_of_pool(genome *head[], int gene_pool_size)
{
    int indx;

    for (indx = 0; indx < gene_pool_size; indx++)
    {
        calculate_fitness(head[indx]);
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
    //printf(" Worst Fitness [%d] = %d\n", 0, head[5]->get_fitness());
    printf("---------------------------------\n");
#endif

}


int genome_operator::calculate_fitness(genome *some_genome)
{
    int length_usedup = 0;
    int wastage = 0;
    int frame_to_be_cut_next;
    int channel_size_remaining;
    int indx = 0;

    while(indx < some_genome->gene_len)
    {
            /* See if we can cut the required frame from current frame */
            frame_to_be_cut_next = required_frames[*(some_genome->genome_ptr + indx)];
            channel_size_remaining = channel_size - length_usedup;

            if (frame_to_be_cut_next <= channel_size_remaining)
            {
#ifdef DEBUG
                //printf("%d:Frame size %d cut\n", indx, frame_to_be_cut_next);
#endif /* DEBUG */
                length_usedup += frame_to_be_cut_next;
                /* Move to next frame to be cut */
                indx++;
            } else {
                /* Add rest to wastage */
                //printf("%d: Wastage = %d\n", indx, wastage);
                wastage += channel_size_remaining;
                /* Start with new channel
                 * NOTE: dont keep track of channels, as wastage tracks he same*/
                length_usedup = 0;
            }
    }
    some_genome->set_fitness(wastage);
#ifdef DEBUG
    //printf(" Wastage = %d\n", wastage);
#endif /* DEBUG */
}



void genome_operator::crossover_genepool(genome *head[], int gene_pool_size)
{
    for(int indx = KEEPALIVE_POPULATION; indx < gene_pool_size; indx++)
    {
        head[indx]->crossover();
    }
}

void genome_operator::crossover_genepool(genome *head[], int gene_pool_size, float mutation_percent)
{
    for(int indx = KEEPALIVE_POPULATION; indx < gene_pool_size; indx++)
    {
        /* Take the sucessful parents and do crossover and mutation on them */
        head[indx]->copy_genome(head[indx % KEEPALIVE_POPULATION]);
        head[indx]->mutate(mutation_percent);
    }
}



