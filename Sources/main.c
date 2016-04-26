#include "includes.h"



int main(void) {
  initALL();
  initPIT();

  BlueTx();
  
  
  SET_motor(50,50);


  /* Loop forever */
  for (;;) {
	  KeyJudge();
   
  }
}



