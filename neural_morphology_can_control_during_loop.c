#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NUM_NEURONS 10
#define NUM_SYNAPSES 5

typedef struct {
    double membrane_potential;
    double threshold;
    double* weights;
    int num_inputs;
} Neuron;

void initialize_neuron(Neuron* neuron, int num_inputs) {
    neuron->membrane_potential = 0.1; // Initial membrane potential
    neuron->threshold = 0.6; // Higher threshold
    neuron->num_inputs = num_inputs;
    neuron->weights = (double*)malloc(num_inputs * sizeof(double));
    for (int i = 0; i < num_inputs; i++) {
        neuron->weights[i] = 0.3 + (double)rand() / RAND_MAX * 0.7; // Random weights between 0.3 and 1.0
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
    synapse->weight = 0.3 + (double)rand() / RAND_MAX * 0.7; // Random weights between 0.3 and 1.0
}

int activate(Neuron* neuron) {
    if (neuron->membrane_potential >= neuron->threshold) {
        neuron->membrane_potential = 0.0; // Reset potential
        return 1; // Spike
    }
    return 0; // No spike
}

void update_weights_stdp(Synapse* synapse, double learning_rate, int pre_spike, int post_spike, double time_diff) {
    if (pre_spike && post_spike) {
        if (time_diff > 0) {
            synapse->weight += learning_rate * exp(-time_diff / 20.0); // Hebbian learning with STDP
        } else {
            synapse->weight -= learning_rate * exp(time_diff / 20.0); // Anti-Hebbian learning with STDP
        }
    }
}

void user_input(Neuron* neurons) {
    int neuron_id, choice;
    double value;

    printf("Do you want to modify a neuron? (1 for yes, 0 for no): ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Enter neuron ID (0-%d): ", NUM_NEURONS - 1);
        scanf("%d", &neuron_id);

        if (neuron_id >= 0 && neuron_id < NUM_NEURONS) {
            printf("Choose what to modify:\n");
            printf("1. Membrane Potential\n");
            printf("2. Threshold\n");
            printf("3. Add External Input\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    printf("Enter new membrane potential: ");
                    scanf("%lf", &value);
                    neurons[neuron_id].membrane_potential = value;
                    break;
                case 2:
                    printf("Enter new threshold: ");
                    scanf("%lf", &value);
                    neurons[neuron_id].threshold = value;
                    break;
                case 3:
                    printf("Enter external input to add: ");
                    scanf("%lf", &value);
                    neurons[neuron_id].membrane_potential += value;
                    break;
                default:
                    printf("Invalid choice.\n");
                    break;
            }
        } else {
            printf("Invalid neuron ID.\n");
        }
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
    double learning_rate = 0.1; // Increase learning rate
    for (int t = 0; t < 100; t++) {
        int spikes[NUM_NEURONS] = {0}; // Array to store spike information
        double last_spike_times[NUM_NEURONS] = {-1000}; // Initialize with a large negative value

        // User input to modify neurons
        user_input(neurons);

        for (int i = 0; i < NUM_NEURONS; i++) {
            double input = 0.0;
            for (int j = 0; j < NUM_SYNAPSES; j++) {
                int pre_index = (i + NUM_NEURONS - 1) % NUM_NEURONS;
                input += synapses[pre_index * NUM_SYNAPSES + j].weight * synapses[pre_index * NUM_SYNAPSES + j].pre_neuron->membrane_potential;
            }
            // Add random external input and noise
            input += 0.1 * ((double)rand() / RAND_MAX) + 0.01 * ((double)rand() / RAND_MAX - 0.5);

            neurons[i].membrane_potential += input;

            // Debugging: Print membrane potential
            printf("Neuron %d membrane potential at time %d: %f\n", i, t, neurons[i].membrane_potential);

            int spike = activate(&neurons[i]);
            if (spike) {
                printf("Neuron %d spiked at time %d\n", i, t);
                spikes[i] = 1; // Mark neuron as spiked
                last_spike_times[i] = t;
            }
        }

        // Update synapses after all neurons have been processed
        for (int i = 0; i < NUM_NEURONS; i++) {
            for (int j = 0; j < NUM_SYNAPSES; j++) {
                int pre_index = (i + NUM_NEURONS - 1) % NUM_NEURONS;
                int post_index = (i + 1) % NUM_NEURONS;
                double time_diff = last_spike_times[post_index] - last_spike_times[pre_index];
                update_weights_stdp(&synapses[pre_index * NUM_SYNAPSES + j], learning_rate, spikes[pre_index], spikes[post_index], time_diff);
            }
        }
    }

    // Free memory
    for (int i = 0; i < NUM_NEURONS; i++) {
        free_neuron(&neurons[i]);
    }

    return 0;
}