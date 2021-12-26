#include "neural_net.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#ifndef DEBG
#define DEBG 0
#endif
#define PRINT_DEBUG() if ( DEBG) printf("DEBUG_%d\n",__LINE__);

typedef struct 
{
	double health;
	double knife;
	double gun;
	double enemy;
	double out[OUTPUT_NEURONS];
} ELEMENT;

#define MAX_SAMPLES 18

int  sample=0;
/*H K G E A R W H*/ 
ELEMENT samples[MAX_SAMPLES] = 
{
     { 2.0, 0.0, 0.0, 0.0, {0.0, 0.0, 1.0, 0.0} },
     { 2.0, 0.0, 0.0, 1.0, {0.0, 0.0, 1.0, 0.0} },
     { 2.0, 0.0, 1.0, 1.0, {1.0, 0.0, 0.0, 0.0} },
     { 2.0, 0.0, 1.0, 2.0, {1.0, 0.0, 0.0, 0.0} },
     { 2.0, 1.0, 0.0, 2.0, {0.0, 0.0, 0.0, 1.0} },
     { 2.0, 1.0, 0.0, 1.0, {1.0, 0.0, 0.0, 0.0} },
     { 1.0, 0.0, 0.0, 0.0, {0.0, 0.0, 1.0, 0.0} },
     { 1.0, 0.0, 0.0, 1.0, {0.0, 0.0, 0.0, 1.0} },
     { 1.0, 0.0, 1.0, 1.0, {1.0, 0.0, 0.0, 0.0} },
     { 1.0, 0.0, 1.0, 2.0, {0.0, 0.0, 0.0, 1.0} },
     { 1.0, 1.0, 0.0, 2.0, {0.0, 0.0, 0.0, 1.0} },
     { 1.0, 1.0, 0.0, 1.0, {0.0, 0.0, 0.0, 1.0} },
     { 0.0, 0.0, 0.0, 0.0, {0.0, 0.0, 1.0, 0.0} },
     { 0.0, 0.0, 0.0, 1.0, {0.0, 0.0, 0.0, 1.0} },
     { 0.0, 0.0, 1.0, 1.0, {0.0, 0.0, 0.0, 1.0} },
     { 0.0, 0.0, 1.0, 2.0, {0.0, 1.0, 0.0, 0.0} },
     { 0.0, 1.0, 0.0, 2.0, {0.0, 1.0, 0.0, 0.0} },
     { 0.0, 1.0, 0.0, 1.0, {0.0, 0.0, 0.0, 1.0} }
};


char *strings[4]={"Attack", "Run", "Wander", "Hide"};

size_t feel_weigths(double const * const p_vec_weights, const size_t count);

int action( double *vector )
{
	int index, sel;
	double max;
	sel = 0;
	max = vector[sel];
	for (index = 1 ; index < OUTPUT_NEURONS ; index++) 
	{
		if (vector[index] > max) 
		{
			max = vector[index]; 
			sel = index;
		}
	}
	return( sel );
}

void indecies_generation(int *r1, int *r2, int *r3, int pop_size)
{
	int a = rand() % pop_size;

	(*r1)=(*r2)=(*r3)=a;

	while ( (*r1 == *r2) || (*r1 == *r3) || (*r2 == *r3))
	{
		(*r1) = rand() % pop_size;
		(*r2) = rand() % pop_size;
		(*r3) = rand() % pop_size;
	}
}

// функция растригина
// Заменить
double benchmark_function (double *x, int N)
{
	double err = 0.0;
	size_t i;
	feel_weigths(x,N);
	
	for (int k =0; k < MAX_SAMPLES; ++k)
	{
        inputs[0] = samples[k].health;
	    inputs[1] = samples[k].knife;
	    inputs[2] = samples[k].gun;
	    inputs[3] = samples[k].enemy;
	    target[0] = samples[k].out[0];
	    target[1] = samples[k].out[1];
	    target[2] = samples[k].out[2];
	    target[3] = samples[k].out[3];
		feedForward();
		for (i = 0; i < OUTPUT_NEURONS; i++) 
		{
			err += sqr((samples[sample].out[i] - actual[i]));
		}
	}
	err = 0.5 * err;
	return err;
}

void generatin_population_de (double **population, int pop_size)
{
	for (int i=0;i < pop_size;i++)
	{
		for (int j=0; j < TOTAL_WEIGHTS;j++)
		{
			population[i][j] = (RAND_WEIGHT) * 40;
		}
	}
}
//!
void generatin_population (double **population, int pop_size)
{
	generatin_population_de(population,pop_size);
	return;
	for (int i=0;i < pop_size;i++)
	{
		for (int j=0; j < TOTAL_WEIGHTS;j++)
		{
			population[i][j] = RAND_WEIGHT;
		}
	}
}
//!
void copy_array(double **x, double **y, int pop_size)
{
	size_t i;
    for (i=0; i < pop_size;i++)
    {
        memcpy(y[i],x[i], sizeof(x[0]) * TOTAL_WEIGHTS);
    }
}

int main()
{
	/*! NN */
	double err;
	int i,j, iterations=0;
	int sum = 0;
	//struct timeval start;
	//struct timeval stop;


	int FEV = 10000000 * 5;             // Кол-во вычислений
	/*! DE */
	const int pop_size = 50;						// Размер популяции
	double best_fitness = 1000;//1e300;					// лучшая пригодность
	double **population = (double**) malloc(sizeof(double*) * pop_size);	// lines
	double **population_new = (double**) malloc(sizeof(double*) * pop_size);
	double * solution = (double*) malloc(sizeof(double) * TOTAL_WEIGHTS);
	double * fitness = (double*) malloc(sizeof(double) * pop_size);
	double * fitness_new = (double*) malloc(sizeof(double) * pop_size);

	double *u = (double*)malloc(sizeof(double)*TOTAL_WEIGHTS);
	double *best_solution = (double*)malloc(sizeof(double)*TOTAL_WEIGHTS);

	for (i = 0; i < pop_size; ++i)
	{
		population[i] = (double*)malloc(sizeof(double)*TOTAL_WEIGHTS);
		population_new[i] = (double*)malloc(sizeof(double)*TOTAL_WEIGHTS);
	}

	generatin_population (population, pop_size);
	copy_array(population, population_new, pop_size);

	//FILE *out = fopen("stats.txt", "w");
	/* Инициализировать генератор случайных чисел */
	srand( time(NULL) );
	
	
	for (i = 0; i < pop_size; ++i)
	{
		for (j = 0; j < TOTAL_WEIGHTS; ++j)
		{
			u[j] = population[i][j];
		}
		fitness[i] = fitness_new[i] = benchmark_function(u, TOTAL_WEIGHTS);
	}
	while (FEV>0)
	{
		for (i=0; i < pop_size; i++)
		{
			int r1, r2, r3;
			indecies_generation(&r1,&r2,&r3, pop_size);
			// Управляющие параметры [0: 1]
            double CR 	= (RAND_WEIGHT + 0.5) * (0.9 - 0.5) + 0.5;//*(0.9-0.5)-0.1;
            double F 	= (RAND_WEIGHT + 0.5);//*(0.9-0.5)-0.1;
            if ( (CR > 1 || CR < 0) || (F > 1 || F < 0))
            {
            	printf("ERROR");
        		exit(1);
        	}
            int jrand = (RAND_WEIGHT + 0.5)*(TOTAL_WEIGHTS-1);

            for ( j=0; j < TOTAL_WEIGHTS; j++)
            {
                if (CR< (RAND_WEIGHT + 0.5) || j == jrand)
                {
                    u[j] = population[i][j]+F*(population[i][j]-population[r1][j])+F*(population[r2][j]-population[r3][j]);
                }
                else
                {
                    u[j] = population[i][j];
                }
                /*if (u[j] > 20) 
                {
                	u[j] = (20 + population[i][j]) / 2;
                }
                else if (u[j] < -20)
                {
                	u[j] = (-20 + population[i][j]) / 2; //-0.5;
                }*/
            }

            double test = benchmark_function(u, TOTAL_WEIGHTS);
            FEV--;

            if (test<=fitness[i])
            {
                fitness_new[i] = test;
                for (j=0; j < TOTAL_WEIGHTS; j++)
                {
                    population_new[i][j]=u[j];
                }

                if (test<=best_fitness)
                {
            		PRINT_DEBUG();
            		//printf("new fitness : %f\n", test);
                    best_fitness = test;
                	for (j=0; j < TOTAL_WEIGHTS; j++)
                	{
                    	best_solution[j] = u[j];
                	}
            		PRINT_DEBUG();
                }
            }
        }

        for (int i=0;i<pop_size; i++)
        {
            for (int j=0;j<TOTAL_WEIGHTS;j++)
            {
                population[i][j] = population_new[i][j];
            }
            fitness[i] = fitness_new[i];
        }

    }
    printf("best solution \n");
    printf("best fitness %lf\n",best_fitness);
    for (i = 0; i < TOTAL_WEIGHTS; ++i)
    	printf("[%d] : %lf\n",i,best_solution[i]);
    printf("\n");

	//gettimeofday(&stop, NULL);
    feel_weigths(best_solution,TOTAL_WEIGHTS);
  	/* Проверить сеть */
	for (i = 0 ; i < MAX_SAMPLES ; i++) 
	{
		inputs[0] = samples[i].health;
		inputs[1] = samples[i].knife;
		inputs[2] = samples[i].gun;
		inputs[3] = samples[i].enemy;
		target[0] = samples[i].out[0];
		target[1] = samples[i].out[1];
		target[2] = samples[i].out[2];
		target[3] = samples[i].out[3];
		feedForward();
		if (action(actual) != action(target)) 
		{
			printf("%2.1g:%2.1g:%2.1g:%2.1g %s (%s)\n",
			inputs[0], inputs[1], inputs[2], inputs[3],
			strings[action(actual)], strings[action(target)]);
		} 
		else 
		{
			sum++;
		} 
	}

	printf("Network is %g%% correct\n", ((float)sum / (float)MAX_SAMPLES) * 100.0);
	/* Выполнение тестов */
	/* Здоровье Нож Пистолет Враг*/
	inputs[0] = 2.0; inputs[1] = 1.0; inputs[2] = 1.0; inputs[3] = 1.0; feedForward();
	printf("2111 Action %s\n", strings[action(actual)]);
	inputs[0] = 1.0; inputs[1] = 1.0; inputs[2] = 1.0; inputs[3] = 2.0; feedForward();
	printf("1112 Action %s\n", strings[action(actual)]);
	inputs[0] = 0.0; inputs[1] = 0.0; inputs[2] = 0.0; inputs[3] = 0.0; feedForward();
	printf("0000 Action %s\n", strings[action(actual)]);
	inputs[0] = 0.0; inputs[1] = 1.0; inputs[2] = 1.0; inputs[3] = 1.0; feedForward();
	printf("0111 Action %s\n", strings[action(actual)]);
	inputs[0] = 2.0; inputs[1] = 0.0; inputs[2] = 1.0; inputs[3] = 3.0; feedForward();
	printf("2013 Action %s\n", strings[action(actual)]);

	inputs[0] = 2.0; inputs[1] = 1.0; inputs[2] = 0.0; inputs[3] = 3.0; feedForward();
	printf("2103 Action %s\n", strings[action(actual)]);
	inputs[0] = 0.0; inputs[1] = 1.0; inputs[2] = 0.0; inputs[3] = 3.0; feedForward();
	printf("0103 Action %s\n", strings[action(actual)]);
	return 0;
}