#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "hash.h"


const int FEE_MULT  = 10000;        // Fee Multiplier, used for extracting fee from transaction string
const int FEE_START = 18;           // Start position in string for fee
const int FEE_LEN = 5;              // Length of fee string
const char OUTDEF[5] = ".out";      // Output file extension, append to input filename
const int BLK_SIZE = 1024;          // Size of a mined block


// Transaction Struct
typedef struct TransactionNode {
    char transactionString[100];
    int priority;
    int blockId;
    unsigned short int hash;
    struct TransactionNode *next;
} TNODE;


TNODE * head = NULL;                 // Global Head for linked list

/****************************************************
*                                                   *
*   get Priority                                    *
*   Calculates and assigns a priority value to a    *
*   crypto coin transaction                         *
*   Parameters: transaction string                  *
*                                                   *
****************************************************/
unsigned int getPriority(char transaction[]){
    unsigned int feeData = 0;
    int index = FEE_START;             // starting position
    int multiplier = FEE_MULT;

    // Extract Fee
    while (index <= (FEE_START + FEE_LEN)) {

        feeData+= (unsigned int)(transaction[index] - '0') * multiplier;
        index ++;
        multiplier/= 10;

    }

    return (feeData * 10000) / strlen(transaction);
}

/****************************************************
*                                                   *
*   Create Transaction Node                         *
*   Creates a new node and populates it with data   *
*   from the transaction string                     *
*   Parameters: transaction String                  *
*                                                   *
****************************************************/
TNODE *createTNode( char transaction[] ) {

    // allocate memory to the node
    TNODE *ptr = (TNODE *)malloc(sizeof(TNODE));
    if (ptr == NULL) {

        printf("Failed to create new node\n");
        exit(1);

    } else {

        ptr->priority = getPriority(transaction);
        strcpy(ptr->transactionString, transaction);
        ptr->blockId = 0;
        ptr->next = NULL;

    }
    return ptr;
}


/****************************************************
*                                                   *
*   Insert Node                                     *
*   Inserts newly created node into correct position*
*   in the linked list                              *
*   Parameters: node pointer                        *
*                                                   *
****************************************************/
void insertTNode(TNODE *newNode) {

    TNODE *ptr = head;
    TNODE *prevNode = NULL;

    // new node inserted into empty list
    if (ptr == NULL) {

        head = newNode;
        return;

    }

    // Find the last node with higher value than new node
    while ( (ptr != NULL) && (ptr->priority >= newNode->priority) ) {

        prevNode = ptr;
        ptr = ptr->next;

    }

    // Determine where to insert new node
    if ( prevNode == NULL ) {

        // new node has smallest value, insert at head
        newNode->next = head;
        head = newNode;

    } else if ( ptr == NULL ) {

        // new node has biggest value, insert at tail
        prevNode->next = newNode;

    } else {

        prevNode->next = newNode;
        newNode->next = ptr;

    }
}



/****************************************************
*                                                   *
*   destroy TNode list                              *
*   Destroys and frees memory used by linked list   *
*   Parameters: none                                *
*                                                   *
****************************************************/
void destroyTNodeList() {

    TNODE *ptr = head;
    TNODE *nextNode = NULL;

    while (ptr != NULL) {

        nextNode = ptr->next;
        free(ptr);
        ptr = nextNode;

    }

    head = NULL;
}

/****************************************************
*                                                   *
*   Create Blocks                                   *
*   Determines which members of linked list will    *
*   fit into a cryptocoin block                     *
*   Parameters: head of linked list                 *
*                                                   *
****************************************************/
void createBlocks(TNODE * head) {

    TNODE * current = head;
    int blkCount = 1;
    int blockID = 1;
    int nodesProc = 1;

    char hashString[MAXLEN];

    // TODO: Currently only configured to create first block for this project

    // This loop makes multiple passes of the Linked List until no nodes are processed in the sorted list
    while (nodesProc != 0) {

        // Reset Nodes Processed counter
        nodesProc = 0;

        // Iterate through the sorted list
        while (current != NULL) {

            // Determine if current node will fit in this block
            if ((BLK_SIZE - blkCount) >= strlen(current->transactionString)) {

                // Increment nodes processed counter
                nodesProc++;

                // Current transaction node will fit assign it block id
                current->blockId = blockID;

                // Prepare string for hashing
                strcpy(hashString, current->transactionString);

                // Determine if transaction string padding required
                if ((strlen(hashString) % 2) != 0) {

                    // Add padding to transaction string
                    strcat(hashString, " \0");

                }

                // Get Hash and assign to current node
                current->hash = hash(hashString);

                // Calculate number of bytes used in block so far
                blkCount += strlen(current->transactionString);
            }

            // Point to next node
            current = current->next;

        }
    }
}


/****************************************************
*                                                   *
*   Get Input Data                                  *
*   Reads lines from input file of transactions and *
*   and creates a node for each in a linked list    *
*   Parameters: input File name                     *
*                                                   *
****************************************************/
int getInputData(char fileName[]){

    FILE *inputFile = NULL;                 // Input file stream
    char transaction[MAXLEN];               // Incoming transaction string
    TNODE *inPtr = NULL;                    // Node for input data

    // Input File Handling
    inputFile = fopen(fileName, "r");

    if (inputFile == NULL) {

        // Error
        printf("Unable to open transactions input file at %s\n", fileName);
        exit(1);

    } else {

        // Read each transaction string
        while (fgets(transaction, MAXLEN, inputFile)!=NULL) {

            // Transaction String Pre-formatting
            transaction[strcspn(transaction, "\n")] = 0;    // remove newline (presence of \n is compiler dependent)

            // Create and insert Node for current transaction
            inPtr = createTNode(transaction);
            insertTNode(inPtr);
        }
    }

    // Close input and output streams before terminating
    fclose(inputFile);

    return 0;
}



/****************************************************
*                                                   *
*   Write Output Data                               *
*   Writes all members of a block into a text file  *
*   Parameters: output file name                    *
*                                                   *
****************************************************/
int writeOutputData(char fileName[]){

    FILE *outputFile = NULL;                 // Input file stream
    TNODE * current = head;                  //
    unsigned int hashTotal = 0;

    strcat(fileName, OUTDEF);

    // Output File Handling
    outputFile = fopen(fileName, "w");

    // Write block to output file
    if (outputFile == NULL) {

        // Error
        printf("Unable to open transactions output file at %s\n", fileName);
        return 1;

    } else {

        // Iterate through the linked list
        while (current != NULL) {

            // Determine if current node is in the block
            if (current->blockId != 0 ) {

                // Print transaction and hash
                fprintf(outputFile, "%s %d\n", current->transactionString, current->hash);

                // Add current hash to hash total
                hashTotal+= current->hash;

            }

            // Move to next node
            current = current->next;
        }

        // Print Total Hash to file
        fprintf(outputFile, "%d", hashTotal);

    }

    // User message about filename
    printf("\nThank you, Data has been output to filename: %s\n\n", fileName);

    // Close input and output streams before terminating
    fclose(outputFile);

    return 0;
}