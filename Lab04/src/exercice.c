#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include <immintrin.h>

typedef struct {
    int x;
    int y;
    int z;
    int bias;
} Sample;

typedef struct {
    int *x;
    int *y;
    int *z;
    int *bias;
    size_t count;
} SoA_Sample;

static long long now_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000LL + (long long)ts.tv_nsec / 1000000LL;
}

static inline int clamp_int(int v, int lo, int hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

void init_samples(Sample *a, SoA_Sample *b, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        int base = (int)(i * 3 + 7);
        a[i].x    =  (base % 50) - 25;
        a[i].y    = ((base * 2) % 60) - 30;
        a[i].z    = ((base * 5) % 40) - 20;
        a[i].bias =  (int)(i % 8) - 4;
    }

    // SIMD initialization
    b->x    = (int*)malloc(n * sizeof(int));
    b->y    = (int*)malloc(n * sizeof(int));
    b->z    = (int*)malloc(n * sizeof(int));
    b->bias = (int*)malloc(n * sizeof(int));
    b->count = n;

    if (!b->x || !b->y || !b->z || !b->bias) {
        perror("malloc");
        free(b->x);
        free(b->y);
        free(b->z);
        free(b->bias);
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < n; ++i) {
        int base = (int)(i * 3 + 7);
        b->x[i]    =  (base % 50) - 25;
        b->y[i]    = ((base * 2) % 60) - 30;
        b->z[i]    = ((base * 5) % 40) - 20;
        b->bias[i] =  (int)(i % 8) - 4;
    }
}

void process_samples(const Sample *a, size_t n, int *energy, int *score) {
    for (size_t i = 0; i < n; ++i) {
        int x = a[i].x;
        int y = a[i].y;
        int z = a[i].z;
        int b = a[i].bias;

        // Normes
        int e = x * x + y * y + z * z;

        // Calcul d'un score abitraire
        int s = (x + y - z) * 3 + b * 5;

        // Clamp foncièrement inutile mais intéressant pour l'exercice
        s = clamp_int(s, -1000, 1000);

        energy[i] = e;
        score[i]  = s;
    }
}

void process_samples_simd(const SoA_Sample *a, size_t n, int *energy, int *score){
    /* Process 8 samples at a time with AVX2 */
    size_t i = 0;

    for (; i + 7 < n; i += 8) {
        /* Load 8 values from each array */
        __m256i vx = _mm256_loadu_si256((const __m256i *)&a->x[i]);
        __m256i vy = _mm256_loadu_si256((const __m256i *)&a->y[i]);
        __m256i vz = _mm256_loadu_si256((const __m256i *)&a->z[i]);
        __m256i vb = _mm256_loadu_si256((const __m256i *)&a->bias[i]);

        /* Compute energy = x*x + y*y + z*z */
        __m256i vx2 = _mm256_mullo_epi32(vx, vx);
        __m256i vy2 = _mm256_mullo_epi32(vy, vy);
        __m256i vz2 = _mm256_mullo_epi32(vz, vz);

        __m256i ve = _mm256_add_epi32(vx2, vy2);
        ve = _mm256_add_epi32(ve, vz2);

        /* Store energy */
        _mm256_storeu_si256((__m256i *)&energy[i], ve);

        /* Compute score = (x + y - z)*3 + bias*5 */
        __m256i vsum = _mm256_add_epi32(vx, vy);
        vsum = _mm256_sub_epi32(vsum, vz);

        __m256i v3 = _mm256_set1_epi32(3);
        __m256i v5 = _mm256_set1_epi32(5);

        __m256i vs = _mm256_mullo_epi32(vsum, v3);
        __m256i vb5 = _mm256_mullo_epi32(vb, v5);
        vs = _mm256_add_epi32(vs, vb5);

        /* Clamp manually for each element */
        int *score_ptr = (int *)&vs;
        for (int j = 0; j < 8; ++j) {
            score_ptr[j] = clamp_int(score_ptr[j], -1000, 1000);
        }

        /* Store score */
        _mm256_storeu_si256((__m256i *)&score[i], vs);
    }

    /* Handle remaining samples */
    for (; i < n; ++i) {
        int x = a->x[i];
        int y = a->y[i];
        int z = a->z[i];
        int b = a->bias[i];

        int e = x * x + y * y + z * z;
        int s = (x + y - z) * 3 + b * 5;
        s = clamp_int(s, -1000, 1000);

        energy[i] = e;
        score[i] = s;
    }
}

void compare_arrays(const int *a, const int *b, size_t n, const char *name) {
    for (size_t i = 0; i < n; ++i) {
        if (a[i] != b[i]) {
            printf("Mismatch in %s at index %zu: %d != %d\n", name, i, a[i], b[i]);
            return;
        }
    }
    printf("Arrays %s match.\n", name);
}

int main(int argc, char** argv) {
    const size_t n = 30000000;
    Sample *samples  = (Sample*)malloc(n * sizeof(Sample));
    SoA_Sample *soa_samples = (SoA_Sample*)malloc(sizeof(SoA_Sample));
    int *energy      = (int*)malloc(n * sizeof(int));
    int *score       = (int*)malloc(n * sizeof(int));

    if (!samples || !energy || !score || !soa_samples) {
        perror("malloc");
        free(samples);
        free(energy);
        free(score);
        free(soa_samples);
        return 1;
    }

    init_samples(samples, soa_samples, n);

    long long startTime = now_ms();
    process_samples(samples, n, energy, score);
    long long endTime = now_ms();
    long long endProcessNormal = endTime - startTime;

    startTime = now_ms();
    process_samples_simd(soa_samples, n, energy, score);
    endTime = now_ms();
    long long endProcessSimd = endTime - startTime;

    printf("Processing time (normal) : %lld ms\n", endProcessNormal);
    printf("Processing time (SIMD)   : %lld ms\n", endProcessSimd);

    // compare_arrays(energy, <Your array>, n, "energy");

    free(samples);
    free(soa_samples->x);
    free(soa_samples->y);
    free(soa_samples->z);
    free(soa_samples->bias);
    free(soa_samples);
    free(energy);
    free(score);
    return 0;
}