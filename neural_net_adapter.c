#include "neural_net.h"


static double get_total_weights_count()
{
    return (TOTAL_WEIGHTS);
}

void randomize_population(double * const p_vec_weights, const size_t count)
{
    size_t i;
    for (i = 0; i < count; ++i)
    {
        p_vec_weights[i] = RAND_WEIGHT;
    }
}

size_t feel_weigths(double const * const p_vec_weights, const size_t count)
{
    int hid, inp, out;
    int cur_iter = 0;

    if ((p_vec_weights == NULL) || (count == 0))
        return 0;

    for (inp = 0; inp < INPUT_NEURONS + 1; ++inp)
    {
        for (hid = 0; hid < HIDDEN_NEURONS; ++hid)
        {
            wih[inp][hid] = p_vec_weights[cur_iter++];
            if (cur_iter == count)
                return cur_iter;
        }
    }

    for (hid = 0; hid < HIDDEN_NEURONS + 1; ++ hid)
    {
        for (out = 0; out < OUTPUT_NEURONS; ++out)
        {
            who[hid][out] = RAND_WEIGHT;

            if (cur_iter == count)
                return cur_iter;
        }
    }
    return cur_iter;
}