// Constants
const int FEE_MULT;             // Fee Multiplier, used for extracting fee from transaction string
const int FEE_START;            // Start position in string for fee
const int FEE_LEN;              // Length of fee string
const char OUTDEF[5];           // Output file extension, append to input filename
const int BLK_SIZE;             // Size of a mined block

// Transaction Struct
typedef struct TransactionNode {
    char transactionString[100];
    int priority;
    int blockId;
    unsigned short int hash;
    struct TransactionNode *next;
} TNODE;

// Global HEAD variable for linked list
TNODE *head;

unsigned int getPriority(char transaction[]);
TNODE *createTNode( char transaction[] );
void insertTNode(TNODE *newNode);
void destroyTNodeList();
void createBlocks(TNODE * head);
int getInputData(char fileName[]);
int writeOutputData(char fileName[]);