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

int dead_code_not_eliminated(int n) {
    int sum = 0, temp = 0, unused_count = 0;
    for (int i = 0; i < n; i++) {
        sum += i;
        unused_count++;
        temp = sum * 2;
    }  
    return sum;
}

void main() {
  printf("sum = %d\n", dead_code_not_eliminated(100));
}
