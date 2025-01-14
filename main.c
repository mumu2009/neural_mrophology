#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_NEURONS 10
#define NUM_SYNAPSES 5

typedef struct {
    double membrane_potential;
    double threshold;
    double* weights;
    int num_inputs;
} Neuron;

void initialize_neuron(Neuron* neuron, int num_inputs) {
    neuron->membrane_potential = 0.0;
    neuron->threshold = 1.0;
    neuron->num_inputs = num_inputs;
    neuron->weights = (double*)malloc(num_inputs * sizeof(double));
    for (int i = 0; i < num_inputs; i++) {
        neuron->weights[i] = (double)rand() / RAND_MAX; // Random weights
    }
}

void free_neuron(Neuron* neuron) {
    free(neuron->weights);
}
typedef struct {
    Neuron* pre_neuron;
    Neuron* post_neuron;
    double weight;
} Synapse;

void initialize_synapse(Synapse* synapse, Neuron* pre, Neuron* post) {
    synapse->pre_neuron = pre;
    synapse->post_neuron = post;
    synapse->weight = (double)rand() / RAND_MAX; // Random weight
}
int activate(Neuron* neuron) {
    if (neuron->membrane_potential >= neuron->threshold) {
        neuron->membrane_potential = 0.0; // Reset potential
        return 1; // Spike
    }
    return 0; // No spike
}
void update_weights(Synapse* synapse, double learning_rate, int pre_spike, int post_spike) {
    if (pre_spike && post_spike) {
        synapse->weight += learning_rate * (1 - synapse->weight); // Hebbian learning
    } else if (pre_spike && !post_spike) {
        synapse->weight -= learning_rate * synapse->weight; // Anti-Hebbian learning
    }
}
int main() {
    srand(time(NULL));

    // Initialize neurons
    Neuron neurons[NUM_NEURONS];
    for (int i = 0; i < NUM_NEURONS; i++) {
        initialize_neuron(&neurons[i], NUM_SYNAPSES);
    }

    // Initialize synapses
    Synapse synapses[NUM_NEURONS * NUM_SYNAPSES];
    int synapse_index = 0;
    for (int i = 0; i < NUM_NEURONS; i++) {
        for (int j = 0; j < NUM_SYNAPSES; j++) {
            initialize_synapse(&synapses[synapse_index], &neurons[i], &neurons[(i + 1) % NUM_NEURONS]);
            synapse_index++;
        }
    }

    // Simulation loop
    double learning_rate = 0.01;
    for (int t = 0; t < 100; t++) {
        for (int i = 0; i < NUM_NEURONS; i++) {
            double input = 0.0;
            for (int j = 0; j < NUM_SYNAPSES; j++) {
                int pre_index = (i + NUM_NEURONS - 1) % NUM_NEURONS;
                input += synapses[pre_index * NUM_SYNAPSES + j].weight * synapses[pre_index * NUM_SYNAPSES + j].pre_neuron->membrane_potential;
            }
            neurons[i].membrane_potential += input;

            int spike = activate(&neurons[i]);
            if (spike) {
                printf("Neuron %d spiked at time %d\n", i, t);
            }

            // Update synapses
            for (int j = 0; j < NUM_SYNAPSES; j++) {
                int post_index = (i + 1) % NUM_NEURONS;
                update_weights(&synapses[i * NUM_SYNAPSES + j], learning_rate, spike, activate(&neurons[post_index]));
            }
        }
    }

    // Free memory
    for (int i = 0; i < NUM_NEURONS; i++) {
        free_neuron(&neurons[i]);
    }

    return 0;
}