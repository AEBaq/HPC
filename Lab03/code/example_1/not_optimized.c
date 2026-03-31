/*****************************************************************************************
 * HEIG-VD
 * Haute Ecole d'Ingenerie et de Gestion du Canton de Vaud
 * School of Business and Engineering in Canton de Vaud
 *****************************************************************************************
 * REDS Institute
 * Reconfigurable Embedded Digital Systems
 *****************************************************************************************
 *
 * File                 : not_optimized.c
 * Author               : AEBaq
 * Date                 : 31 mars 2026
 *
 * Context              : HPC - Lab03
 *
 *****************************************************************************************
 * Brief: This is the code not opitmized for the first example
 *
 *****************************************************************************************
 * Modifications :
 * Ver    Date        Student      Comments
 * 1.0    31.03.2026  AEBaq        First version of not opitmized code
 *
*****************************************************************************************/

#include <stdio.h>

void constant_expression_inside_loop() {
    int *a = {0, 1, 2, 3};
    int * b = {4, 5, 6, 7};
    int n = 4, i;
    for (i = 0; i < n; i++) {
        a[i] = b[i] * 2 * 3;
    }
}

int main() {
    constant_expression_inside_loop();
    return 0;
}
