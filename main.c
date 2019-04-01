#include <stdio.h>
#include <string.h>
#include "block.h"

int main() {

    char transactionInput[100];         // input file name

    printf("\nWelcome to 1806ICT Assessment PartB\n");

    // Get input filename from user
    printf("Please enter input filename: ");
    scanf(" %s", &transactionInput);

    // Get input file data
    getInputData(transactionInput);

    // Add transactions to block
    createBlocks(head);

    // Write Block to output file
    writeOutputData(transactionInput);

    // Deallocate resources in the linked list
    destroyTNodeList();

}


