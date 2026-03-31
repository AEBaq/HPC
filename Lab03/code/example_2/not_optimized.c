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

void strenght_not_reduced() {
  int array[100];
  int n = 100, i;
  for (i = 0; i < n; i++) {
      array[i] = i / 8;
  }
}

void main(){
  strength_not_reduced();
}
