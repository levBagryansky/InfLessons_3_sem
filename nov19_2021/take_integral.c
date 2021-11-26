#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

double sum = 0;
int counter = 0;
int n_repeats = 0;
double width = 0;

void add_value(double x, double width){
    double value = x / sin(x);
    sum += value * width;
}

void part_of_integral(/*double start_point*/ void* args){
    double start_point = *((double *) args);
    for (int i = 0; i < n_repeats; ++i) {
        add_value(start_point + i * width, width);
        counter++;
    }
}

int main(int argc, char ** argv) {
    if(argc != 3){
        printf("Wrong format\n");
        exit(1);
    }
    int n_threads = atoi(argv[1]);
    int k = atoi(argv[2]); // count of points
    double width = 0.25 * M_PI / k;

    //double** args = (double **) calloc(n_threads, sizeof (double*));

    for (int i = 0; i < n_threads - 1; ++i) {
        double start_point = i * (k / n_threads) * width;
        pthread_t pt;
        pthread_create(&pt, NULL, &part_of_integral, &start_point);
        pthread_detach(pt);
        //args[i] = &start_point;
    }
    double start_point = (n_threads - 1) * (k / n_threads + k % n_threads) * width;
    pthread_t pt;
    pthread_create(&pt, NULL, part_of_integral, &start_point);
    pthread_detach(pt);

    while (counter < k){;}
    printf("%lf", sum);
    return 0;
}
