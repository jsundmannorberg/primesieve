#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

typedef uint32_t bitset_block;

#define BLOCK_SIZE 32

struct bitset {
    bitset_block *set;
    unsigned long size;
};


struct bitset *new_bitset(unsigned long size) {
    struct bitset *target;
    int numblocks = 1 + (int)(size / BLOCK_SIZE);
    target = (struct bitset*)malloc(sizeof(struct bitset));
    target->set = (bitset_block*)calloc(numblocks, sizeof(bitset_block));
    target->size = size;
    return target;
}


int bitset_set(struct bitset *set, unsigned long index) {
    unsigned long block = index / BLOCK_SIZE;
    bitset_block bit = 1 << (index % BLOCK_SIZE);
    set->set[block] |= bit;
    return 0;
}


int bitset_is_set(struct bitset *set, unsigned long index) {
    unsigned long block = index / BLOCK_SIZE;
    bitset_block bit = 1 << (index % BLOCK_SIZE);
    return set->set[block] & bit;
}


struct bitset *get_all_primes(unsigned long max) {
    unsigned long counter = 2;
    unsigned long i;
    struct bitset *set = new_bitset(max);
    // Initialize
    bitset_set(set, 0);
    bitset_set(set, 1);
    // First mark all even numbers. This means that we only need to consider
    // odd numbers and doubles the speed of the calculation.
    for (i = 4; i < max; i += 2) {
        bitset_set(set, i);
    }
    ++counter;
    // Now the odd numbers
    while (counter*counter < max) {
        for (i = counter*counter; i < max; i += counter) {
            bitset_set(set, i);
        }
        counter += 2;
        while (bitset_is_set(set, counter)) {
            counter += 2;
        }
    }
    return set;
}


int main(int argc, char **argv) {
    struct bitset *primes; 
    int max, range;
    unsigned long i;
    if (argc > 1) {
        max = atoi(argv[1]);
    }
    if (argc == 3) {
        range = atoi(argv[2]);
    }
    else {
        range = max;
    }
    primes = get_all_primes(max);
    for (i = 1; i < range; i++) {
        if (!bitset_is_set(primes, i)) {
            printf("%lu ", i);
        }
    }
    printf("\n");
    return 0;
}

