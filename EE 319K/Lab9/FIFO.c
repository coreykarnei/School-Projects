 /*FIFO:
*Index based
*putIndex, getIndex
*Invariants?
*Count-based
*Both put and get modify count ==> Race conditions
*Pointer based
*Reallocating memory when FIFO fills up?
*/
#include <stdint.h>
// 1. store the data so the oldest is on top and one size counter defines the FIFO status; 
// 2. implement it so 5, 6, or 8 bytes are put and get at a time; 
// 3. reverse the direction of the pointers so both PutPt and GetPt decrement rather than increment; 

 /* Very simple FIFO
 * These are FIFO FIFOs which discard the new data when full.
 *
 * FIFO is empty when in == out.
 * If in != out, then 
 *  - items are placed into in before incrementing in
 *  - items are removed from out before incrementing out
 * FIFO is full when in == (out-1 + FIFO_SIZE) % FIFO_SIZE;
 *
 * The FIFO will hold FIFO_ELEMENTS number of items before the
 * calls to FIFOPut fail.
 */
 
/*Index Implementation*/

/* FIFO structure */
#define FIFO_ELEMENTS 16
#define FIFO_SIZE (FIFO_ELEMENTS + 1)
uint8_t FIFO[FIFO_SIZE];
int FIFOIn, FIFOOut;

void FIFO_Init(void){
    FIFOIn = FIFOOut = 0;
}

int FIFO_Put(uint8_t data){
    if(FIFOIn == (( FIFOOut - 1 + FIFO_SIZE) % FIFO_SIZE)){
				return -1; /* FIFO Full*/
    }
    FIFO[FIFOIn] = data;
    FIFOIn = (FIFOIn + 1) % FIFO_SIZE;
    return 0; // No errors
}

int FIFO_Get(unsigned char* data){
    if(FIFOIn == FIFOOut){
				return -1; /* FIFO Empty*/
    }
    *data = FIFO[FIFOOut];
    FIFOOut = (FIFOOut + 1) % FIFO_SIZE;
    return 0; // No errors
}
