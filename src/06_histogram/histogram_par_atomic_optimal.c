#include "histogram.h"
#include "utils.h"

#include <assert.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

struct args_context {
    size_t num_bins;
    uint64_t* bins_out;
    size_t num_indices;
    const int64_t* indices;
};

struct thrd_context {
    struct args_context args;
    uint64_t num_cores;
    uint64_t id;
    atomic_uint_fast64_t* bin_cnts;
};

static int init_zero_helper(void* ctx_untyped) {
    struct thrd_context* ctx = (struct thrd_context*)ctx_untyped;

    const uint64_t buffer_size = ctx->args.num_bins;

    const uint64_t id = ctx->id;
    const uint64_t num_cores = ctx->num_cores;
    const uint64_t chunk_size = (buffer_size + num_cores - 1) / num_cores;
    const uint64_t begin = id * chunk_size;
    const uint64_t end = MIN(buffer_size, begin + chunk_size);

    for (uint64_t i = begin; i < end; i++) {
        atomic_store_explicit(&ctx->bin_cnts[i], 0, memory_order_relaxed);
    }

    return 0;
}

static int histogram_helper(void* ctx_untyped) {
    struct thrd_context* ctx = (struct thrd_context*)ctx_untyped;

    const uint64_t buffer_size = ctx->args.num_indices;

    const uint64_t id = ctx->id;
    const uint64_t num_cores = ctx->num_cores;
    const uint64_t chunk_size = (buffer_size + num_cores - 1) / num_cores;
    const uint64_t begin = id * chunk_size;
    const uint64_t end = MIN(buffer_size, begin + chunk_size);

    for (uint64_t i = begin; i < end; i++) {
        const int64_t bin_index = ctx->args.indices[i];

        if (0 <= bin_index && bin_index < (int64_t)MIN(ctx->args.num_bins, INT64_MAX)) {
            atomic_fetch_add_explicit(&ctx->bin_cnts[bin_index], 1, memory_order_relaxed);
        }
    }

    return 0;
}

static int output_bins_helper(void* ctx_untyped) {
    struct thrd_context* ctx = (struct thrd_context*)ctx_untyped;

    const uint64_t buffer_size = ctx->args.num_bins;

    const uint64_t id = ctx->id;
    const uint64_t num_cores = ctx->num_cores;
    const uint64_t chunk_size = (buffer_size + num_cores - 1) / num_cores;
    const uint64_t begin = id * chunk_size;
    const uint64_t end = MIN(buffer_size, begin + chunk_size);

    for (uint64_t i = begin; i < end; i++) {
        ctx->args.bins_out[i] = atomic_load_explicit(&ctx->bin_cnts[i], memory_order_relaxed);
    }

    return 0;
}

void histogram_par_atomic_optimal(const size_t num_bins, uint64_t* bins, const size_t num_indices, const int64_t* indices) {
    const unsigned int num_cores = get_num_cores();

    thrd_t* threads = NULL;
    struct thrd_context* id_ctxs = NULL;
    atomic_uint_fast64_t* bin_cnts = NULL;

    bool success = true;

    threads = (thrd_t*)calloc(num_cores, sizeof(thrd_t));
    if (!threads) {
        fprintf(stderr, "could not allocate memory!\n");
        success = false;
        goto cleanup;
    }
    id_ctxs = (struct thrd_context*)calloc(num_cores, sizeof(struct thrd_context));
    if (!id_ctxs) {
        fprintf(stderr, "could not allocate memory!\n");
        success = false;
        goto cleanup;
    }
    bin_cnts = (atomic_uint_fast64_t*)calloc(num_bins, sizeof(atomic_uint_fast64_t));
    if (!bin_cnts) {
        fprintf(stderr, "could not allocate memory!\n");
        success = false;
        goto cleanup;
    }

    for (size_t i = 0; i < num_cores; i++) {
        struct args_context args_ctx = {
            .num_bins = num_bins,
            .bins_out = bins,
            .num_indices = num_indices,
            .indices = indices,
        };
        id_ctxs[i] = (struct thrd_context){
            .args = args_ctx,
            .num_cores = num_cores,
            .id = i,
            .bin_cnts = bin_cnts,
        };
    }

    {
        for (size_t i = 0; i < num_cores; i++) {
            success &= thrd_create(&threads[i], &init_zero_helper, &id_ctxs[i]) == thrd_success;
        }
        for (size_t i = 0; i < num_cores; i++) {
            success &= thrd_join(threads[i], NULL) == thrd_success;
        }
        if (!success) {
            fprintf(stderr, "error while creating or joining threads\n");
            goto cleanup;
        }
    };

    {
        for (size_t i = 0; i < num_cores; i++) {
            success &= thrd_create(&threads[i], &histogram_helper, &id_ctxs[i]) == thrd_success;
        }
        for (size_t i = 0; i < num_cores; i++) {
            success &= thrd_join(threads[i], NULL) == thrd_success;
        }
        if (!success) {
            fprintf(stderr, "error while creating or joining threads\n");
            goto cleanup;
        }
    };

    {
        for (size_t i = 0; i < num_cores; i++) {
            success &= thrd_create(&threads[i], &output_bins_helper, &id_ctxs[i]) == thrd_success;
        }
        for (size_t i = 0; i < num_cores; i++) {
            success &= thrd_join(threads[i], NULL) == thrd_success;
        }
        if (!success) {
            fprintf(stderr, "error while creating or joining threads\n");
            goto cleanup;
        }
    };

cleanup:
    if (bin_cnts)
        free(bin_cnts);
    if (id_ctxs)
        free(id_ctxs);
    if (threads)
        free(threads);
    if (!success)
        abort();
}
