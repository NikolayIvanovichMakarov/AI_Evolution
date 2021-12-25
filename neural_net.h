#ifndef NEURAL_NET
#define NEURAL_NET
#include <stdlib.h>
#include <math.h>

//! configure neural net
#define INPUT_NEURONS       4
#define HIDDEN_NEURONS      3
#define OUTPUT_NEURONS      4

#define LEARN_RATE 0.2
#define getSRand() ((float)rand() / (float)RAND_MAX)
#define RAND_WEIGHT (((float)rand() / (float)RAND_MAX) - 0.5)
#define getRand(x) (int)((x) * getSRand())
#define sqr(x)  ((x) * (x))

#define TOTAL_WEIGHTS (INPUT_NEURONS + 1) * HIDDEN_NEURONS + (HIDDEN_NEURONS + 1) * OUTPUT_NEURONS 

/*! weights */
extern double wih[INPUT_NEURONS + 1][HIDDEN_NEURONS];
extern double who[HIDDEN_NEURONS + 1][OUTPUT_NEURONS];

/*! activators */
extern double inputs[INPUT_NEURONS];
extern double hidden[HIDDEN_NEURONS];
extern double target[OUTPUT_NEURONS];
extern double actual[OUTPUT_NEURONS];

/*! errors */
extern double erro[OUTPUT_NEURONS];
extern double errh[HIDDEN_NEURONS];

//! рандомные числа
void assignRandomWeights(void);
double sigmoid( double val);
double sigmoidDerivative(double val);
void feedForward();
void backPropagate( void );

#endif