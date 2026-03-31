/*****************************************************************************************
 * HEIG-VD
 * Haute Ecole d'Ingenerie et de Gestion du Canton de Vaud
 * School of Business and Engineering in Canton de Vaud
 *****************************************************************************************
 * REDS Institute
 * Reconfigurable Embedded Digital Systems
 *****************************************************************************************
 *
 * File                 : optimized_by_student.c
 * Author               : AEBaq
 * Date                 : 31 mars 2026
 *
 * Context              : HPC - Lab03
 *
 *****************************************************************************************
 * Brief: This is the code opitmized by student for the first example
 *
 *****************************************************************************************
 * Modifications :
 * Ver    Date        Student      Comments
 * 1.0    31.03.2026  AEBaq        First version of opitmized code by student
 *
*****************************************************************************************/

void strength_reduction() {
  int array[100];
  int n = 100, i;
  for (i = 0; i < n; i++) {
      array[i] = i >> 3;
  }
}

void main() {
  strength_reduction();
}
