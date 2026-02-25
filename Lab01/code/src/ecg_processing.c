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

#define MAT_READER_IMPLEMETNATION

#include "mat_reader.h"

#include "ecg_processing.h"

ECG_Context *ecg_create(const ECG_Params *params) {
    if (!params) return NULL;

    ECG_Context *ctx = malloc(sizeof(ECG_Context));
    if (!ctx) return NULL;

    
}