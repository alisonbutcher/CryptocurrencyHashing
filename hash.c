const int MAXLEN = 1024;                    // Max length of transaction


/****************************************************
*   Rotate Right Function                           *
*   Returns data bit rotated specified # times      *    
*   Parameters: 16 bit, Rotate right                *
*                                                   *
****************************************************/
unsigned short int ror(int numRotates, unsigned short int data){

    unsigned short int bit = 0;
    unsigned short int buffer = 0;

    // Only need to do remainder of numRotates / 16
    numRotates = numRotates % 16;

    // Perform Rotate
    for (int i = 0; i < numRotates; i++) {
        bit = ((data & 0x0001) << 15);       // Get LSB and put in MSB with Mask
        buffer = data >> 1;                  // perform right shift 1 place
        data = buffer | bit;                 // combine shifted word and bit
    }

    return data;
}

/****************************************************
*   Hashing function                                *
*   Returns hash value of a string                  *    
*   Parameters: 16 bit, Rotate right                *
*                                                   *
****************************************************/
unsigned short hash(char transaction[]) {

    unsigned short int data = 0;
    unsigned short int hash;            // hashed data
    unsigned int index;                 // transaction char array index
    unsigned int count= 0;              // Proof of work count
    long int len = strlen(transaction);

    // Proof of work loop
    do {

        // Reset hash at each count
        hash = 0;

        // Reset index at each count
        index = 0;

        // Loop over transaction string
        while (index <  len) {

            // Pack next 2 bytes of transaction into data
            data = ((short int)transaction[index]) << 8 | (short int)transaction[index+1];

            // Rotate the data based on position in the transaction string
            data = ror(index/2,data);

            // Get hash value by XOR with rotated data
            hash = hash ^ data;

            // Set index to next 2 bytes
            index+= 2;
        }

        // Rotate and XOR count with hash
        hash = hash ^ (ror(len/2,count));          // expects even (ie: padded if req) transaction string from calling function

        // Increment the Proof of Work Counter
        count++;

    } while (hash!=0);

    return count - 1;
}