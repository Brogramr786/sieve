#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<time.h>

typedef struct Natural {
    int x;
} NaturalType;

void initNatural(NaturalType* self) {
    self->x = 2;
}

int nextNatural(NaturalType* self) {
    return self->x++;
}

typedef struct Filter {
    int number;
    struct Filter *next;
    struct Filter *last;
} FilterType;

FilterType* newFilter(int n) {
    FilterType* f = malloc(sizeof(FilterType));
    f->number = n;
    f->next = NULL;
    f->last = f;
    return f;
}

void releaseFilter(FilterType* filter) {
    while (filter != NULL) {
        FilterType* next = filter->next;
        free(filter);
        filter = next;
    }
}

int acceptAndAdd(FilterType* filter, int n) {
    FilterType* first = filter;
    int upto = (int)sqrt(n);
    for (;;) {
        if (n % filter->number == 0) {
            return 0;
        }
        if (filter->number > upto) {
            break;
        }
        filter = filter->next;
    }
    FilterType* f = newFilter(n);
    first->last->next = f;
    first->last = f;
    return 1;
}

typedef struct Primes {
    NaturalType* natural;
    FilterType* filter;
} PrimesType;

void initPrimes(PrimesType* self, NaturalType* natural) {
    self->natural = natural;
    self->filter = NULL;
}

void releasePrimes(PrimesType* self) {
    releaseFilter(self->filter);
}

int nextPrime(PrimesType* self) {
    for (;;) {
        int n = nextNatural(self->natural);
        if (self->filter == NULL) {
            self->filter = newFilter(n);
            return n;
        }
        if (acceptAndAdd(self->filter, n)) {
            return n;
        }
    }
}

long currentTimeMillis() {
    clock_t t1;

    t1 = clock();
    return t1 / 1000;
}

long measure(int prntCnt, int upto) {
    NaturalType n;
    initNatural(&n);

    PrimesType primes;
    initPrimes(&primes, &n);

    long start = currentTimeMillis();
    int cnt = 0;
    int res = -1;
    for (;;) {
        res = nextPrime(&primes);
        cnt++;
        if (cnt % prntCnt == 0) {
            printf("Computed %d primes in %ld ms. Last one is %d\n", cnt, (currentTimeMillis() - start), res);
            prntCnt *= 2;
        }
        if (upto && cnt >= upto) {
            break;
        }
    }

    releasePrimes(&primes);

    return currentTimeMillis() - start;
}

int main(int argc, char** argv) {
    int count = -1;
    if (argc == 2) {
        count = atoi(argv[1]);
    }
    for (;;) {
        printf("Hundred thousand prime numbers in %ld ms\n", measure(97, 100000));
        if (count != -1) {
            if (--count == 0) {
                break;
            }
        }
    }
    return (EXIT_SUCCESS);
}

