/*****************************************************************************************
 *
 * File                 : ecg_processing.c
 * Author               : Emily Baquerizo
 * Date                 : 18.02.2026
 *
 * Context              : HPC Lab 01 - ECG Signal Processing
 *
 *****************************************************************************************
 * Brief: 
 *
 *****************************************************************************************
 * Modifications :
 * Ver    Date        Student           Comments
 * 0.0    18.02.2026  Emily Baquerizo   Initial version.
 *
*****************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ecg_processing.h"
#include "ecg_utils.h"
#include "output_structs.h"

// Helpers internes, style simple
static double ecg_max(const double *x, size_t n) {
    if (!x || n == 0) return 0.0;
    double m = x[0];
    for (size_t i = 1; i < n; ++i) {
        if (x[i] > m) m = x[i];
    }
    return m;
}

ECG_Context *ecg_create(const ECG_Params *params) {
    if (!params) return NULL;

    ECG_Context *ctx = malloc(sizeof(ECG_Context));
    if (!ctx) return NULL;

    ctx->params = *params; // Copie des paramètres
    return ctx;
}

void ecg_destroy(ECG_Context *ctx) {
    if(ctx) {
        printf("[ecg_destroy] Contexte free\n");
        free(ctx);
    } else {
        printf("[ecg_destroy] Contexte NULL, rien à free\n");
    }
}

ECG_Status ecg_analyze(
    ECG_Context *ctx,
    const double *signal,
    size_t n_samples,
    int lead_index,
    ECG_Peaks *peaks,
    ECG_Intervals *intervals
) {
    (void)lead_index;

    ECG_Status err = ECG_OK;

    // Check des paramètres
    if (!ctx) {
        printf("[ecg_analyze] Contexte NULL\n");
        return ECG_ERR_NULL;
    }
    if (!signal) {
        printf("[ecg_analyze] Signal NULL\n");
        err = ECG_ERR_NULL;
        goto exit;
    }
    if (!peaks) {
        printf("[ecg_analyze] peaks NULL\n");
        err = ECG_ERR_NULL;
        goto exit;
    }
    if (!intervals) {
        printf("[ecg_analyze] intervals NULL\n");
        err = ECG_ERR_NULL;
        goto exit;
    }
    if (n_samples == 0) {
        printf("[ecg_analyze] n_samples == 0\n");
        err = ECG_ERR_PARAM;
        goto exit;
    }
    if (ctx->params.sampling_rate_hz <= 0) {
        printf("[ecg_analyze] sampling_rate_hz invalide (%d)\n", ctx->params.sampling_rate_hz);
        err = ECG_ERR_PARAM;
        goto exit;
    }

    // Reset sorties
    memset(peaks, 0, sizeof(*peaks));
    memset(intervals, 0, sizeof(*intervals));

    // Buffers
    double *x0 = NULL;
    double *x1 = NULL;
    double *derivative = NULL;
    double *squared = NULL;
    double *integrated = NULL;

    x0 = malloc(n_samples * sizeof(double));
    x1 = malloc(n_samples * sizeof(double));
    derivative = malloc(n_samples * sizeof(double));
    squared = malloc(n_samples * sizeof(double));
    integrated = malloc(n_samples * sizeof(double));

    if (!x0 || !x1 || !derivative || !squared || !integrated) {
        printf("[ecg_analyze] Échec allocation mémoire (buffers)\n");
        err = ECG_ERR_ALLOC;
        goto cleanup;
    }

    // Copie + gain + remove DC (optionnel mais robuste)
    const double gain = (ctx->params.gain > 0.0) ? ctx->params.gain : 1.0;
    for (size_t i = 0; i < n_samples; ++i) {
        x0[i] = signal[i] * gain;
    }
    ecg_remove_dc(x0, n_samples);

    // Filtrage passe-bande (approx 5–15 Hz avec les utils)
    // Fenêtres : on reste simple (éviter win=0)
    size_t win_lp = (size_t)(ctx->params.sampling_rate_hz / 15);
    size_t win_hp = (size_t)(ctx->params.sampling_rate_hz / 5);
    if (win_lp < 1) win_lp = 1;
    if (win_hp < 1) win_hp = 1;

    // Passe-bas: x1 = MA(x0, win_lp)
    ecg_moving_average(x0, x1, n_samples, win_lp);

    // Passe-haut: x0 = x1 - MA(x1, win_hp)
    ecg_highpass_ma(x1, x0, n_samples, win_hp);

    // Calcul de la dérivée
    ecg_derivative_1(x0, derivative, n_samples);

    // Mise au carré
    ecg_square(derivative, squared, n_samples);

    // Intégration sur fenêtre glissante (MWI) - typiquement 150 ms
    size_t win_mwi = (size_t)(0.150 * (double)ctx->params.sampling_rate_hz);
    if (win_mwi < 1) win_mwi = 1;
    ecg_mwi(squared, integrated, n_samples, win_mwi);

    // Détection : seuil + max local + réfractaire
    double threshold = ctx->params.r_threshold_hint;
    if (threshold <= 0.0) {
        double m = ecg_max(integrated, n_samples);
        threshold = 0.35 * m; // heuristique simple
        printf("[ecg_analyze] threshold auto = %.6f (max=%.6f)\n", threshold, m);
    } else {
        printf("[ecg_analyze] threshold hint = %.6f\n", threshold);
    }

    int fs = ctx->params.sampling_rate_hz;
    int refractory = (int)(0.25 * (double)fs); // 250 ms
    if (refractory < 1) refractory = 1;

    int last_r = -refractory;
    size_t start_i = (size_t)(2 * (size_t)fs);
    // Ignore les premiers échantillons (filtrage instable au début)
    if (start_i < 1) start_i = 1;
    if (start_i >= n_samples) start_i = 1;

    for (size_t i = start_i; i + 1 < n_samples; ++i) {
        // maximum local
        if (!(integrated[i] > integrated[i - 1] && integrated[i] >= integrated[i + 1])) {
            continue;
        }

        // seuil
        if (integrated[i] < threshold) {
            continue;
        }

        // réfractaire
        if (((int)i - last_r) < refractory) {
            continue;
        }

        // ok -> pic R
        if (peaks->R_count < MAX_BEATS) {
            peaks->R[peaks->R_count++] = (int)i;
        }
        last_r = (int)i;

        // seuil adaptatif ultra simple (ton style : optionnel mais utile)
        // threshold = 0.875*threshold + 0.125*integrated[i];
        threshold = 0.875 * threshold + 0.125 * integrated[i];
    }

    // Intervalles RR (en secondes)
    intervals->count = 0;
    for (int k = 1; k < peaks->R_count; ++k) {
        int d = peaks->R[k] - peaks->R[k - 1];
        double rr_s = (double)d / (double)fs;

        if (intervals->count < MAX_BEATS) {
            intervals->RR[intervals->count++] = rr_s;
        }
    }

    printf("[ecg_analyze] %d pics R détectés, %d RR calculés\n",
           peaks->R_count, intervals->count);

cleanup:
    if (integrated) free(integrated);
    if (squared) free(squared);
    if (derivative) free(derivative);
    if (x1) free(x1);
    if (x0) free(x0);

exit:
    return err;
}