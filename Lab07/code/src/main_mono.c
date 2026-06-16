#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_KMER 100

typedef struct {
    char kmer[MAX_KMER];
    int count;
} KmerEntry;

typedef struct {
    KmerEntry *entries;
    int count;
    int capacity;
} KmerTable;

void init_kmer_table(KmerTable *table) {
    table->count = 0;
    table->capacity = 100;
    table->entries = malloc(table->capacity * sizeof(KmerEntry));
    if (!table->entries) {
        perror("Error allocating memory for k-mer table");
        exit(1);
    }
}

void add_kmer(KmerTable *table, const char *kmer) {
    // Search for existing k-mer
    for (int i = 0; i < table->count; i++) {
        if (strcmp(table->entries[i].kmer, kmer) == 0) {
            table->entries[i].count++;
            return;
        }
    }

    // Expand capacity if needed
    if (table->count >= table->capacity) {
        table->capacity *= 2;
        table->entries = realloc(table->entries, table->capacity * sizeof(KmerEntry));
        if (!table->entries) {
            perror("Error reallocating memory for k-mer table");
            exit(1);
        }
    }

    // Add new k-mer
    strncpy(table->entries[table->count].kmer, kmer, MAX_KMER - 1);
    table->entries[table->count].kmer[MAX_KMER - 1] = '\0';
    table->entries[table->count].count = 1;
    table->count++;
}

void free_kmer_table(KmerTable *table) {
    free(table->entries);
}

int compare_kmers(const void *a, const void *b) {
    const KmerEntry *entry_a = (const KmerEntry *)a;
    const KmerEntry *entry_b = (const KmerEntry *)b;
    return entry_b->count - entry_a->count;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <k>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *input_file = argv[1];
    int k = atoi(argv[2]);

    if (k <= 0 || k >= MAX_KMER) {
        fprintf(stderr, "Error: k must be between 1 and %d.\n", MAX_KMER - 1);
        return EXIT_FAILURE;
    }

    // Get file size
    FILE *file = fopen(input_file, "r");
    if (!file) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fclose(file);

    // Initialize table
    KmerTable table;
    init_kmer_table(&table);

    // Start timing
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Read entire file into buffer
    file = fopen(input_file, "r");
    if (!file) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    char *file_buffer = malloc(file_size + 1);
    if (!file_buffer) {
        perror("Error allocating memory for file buffer");
        fclose(file);
        return EXIT_FAILURE;
    }

    size_t bytes_read = fread(file_buffer, 1, file_size, file);
    fclose(file);

    if (bytes_read != (size_t)file_size) {
        fprintf(stderr, "Error reading file\n");
        free(file_buffer);
        return EXIT_FAILURE;
    }

    // Extract k-mers
    char kmer[MAX_KMER];
    for (long i = 0; i <= file_size - k; i++) {
        strncpy(kmer, file_buffer + i, k);
        kmer[k] = '\0';
        add_kmer(&table, kmer);
    }

    // End timing
    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    // Sort results
    qsort(table.entries, table.count, sizeof(KmerEntry), compare_kmers);

    // Print statistics
    printf("K-mer statistics, mono :\n");
    printf("- File size: %ld bytes\n", file_size);
    printf("- K-mer size: %d\n", k);
    printf("- Unique k-mers: %d\n", table.count);
    printf("- Execution time: %.4f seconds\n\n", elapsed);
    printf("- Top k-mers:\n");
    int display_count = (table.count < 50) ? table.count : 50;
    for (int i = 0; i < display_count; i++) {
        printf("%s: %d\n", table.entries[i].kmer, table.entries[i].count);
    }

    free(file_buffer);
    free_kmer_table(&table);

    return 0;
}
