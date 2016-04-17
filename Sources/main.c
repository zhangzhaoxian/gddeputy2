#include "includes.h"



int main(void) {
  initALL();
  initPIT();

  BlueTx();
  


  /* Loop forever */
  for (;;) {
	  KeyJudge();
   
  }
}



