#include <string.h>
#include <stdio.h>

const int MAXLEN;

/****************************************************
*   Hashing function                                *
*   Returns hash value of a string                  *    
*   Parameters: 16 bit, Rotate right                *
*                                                   *
****************************************************/
unsigned short int hash(char transaction[]);



/****************************************************
*   Rotate Right Function                           *
*   Returns data bit rotated specified # times      *    
*   Parameters: 16 bit, Rotate right                *
*                                                   *
****************************************************/
unsigned short int ror(int numRotates, unsigned short int data);