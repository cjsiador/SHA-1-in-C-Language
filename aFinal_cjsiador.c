/*
 * Author: Christian Siador
 * Since 4/25/20
 * This program is a security program that will Hash a given txt file. This program is also SHA-1 (Secure Hash Standard).
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_SIZE 1048576
#define W_SIZE 80
#define DEBUG 1

unsigned int readFile(unsigned char buffer[]);
unsigned int calculateBlocks(unsigned int sizeOfFileInBytes);
void convertCharArrayToIntArray(unsigned char buffer[], unsigned int message[], unsigned int sizeOfFileInBytes);
void addBitCountToLastBlock(unsigned int message[], unsigned int sizeOfFileInBytes, unsigned int blockCount);
void computeMessageDigest(unsigned int message[], unsigned int blockCount);
unsigned int f(unsigned int t, unsigned int B, unsigned int C, unsigned int D);
unsigned int K(unsigned int t);

int main(){

    unsigned char characterTemp;
    unsigned char buffer[MAX_SIZE];
    unsigned int message[MAX_SIZE];
    unsigned int sizeOfFileInBytes;
    unsigned int blockCount;

    sizeOfFileInBytes = readFile(buffer);
    
    blockCount = calculateBlocks(sizeOfFileInBytes);
    
    convertCharArrayToIntArray(buffer, message, sizeOfFileInBytes);
    
    addBitCountToLastBlock(message, sizeOfFileInBytes, blockCount);
    
    computeMessageDigest(message, blockCount);

    return 0;
}

//Function Definition: readFile will read a txt file and converts into buffer[]. 
//the number of characters in the file
//Example:  ./program < abc.txt
//Note:     stdin is abc.txt "fgets(buffer, MAX_SIZE, stdin);".
unsigned int readFile(unsigned char buffer[]){
    
    int a = 0;
    int i = 0;
    unsigned int count = 0;
    int letter = 0;
    char letterTemp = ' ';
    
    letterTemp = getchar();

    while(EOF != letterTemp){ 
        //Error Checking if Exceed MAX_SIZE.
        if(count <= MAX_SIZE){
            buffer[count] = letterTemp;
            count++;
            letterTemp = getchar();
        }else{
            printf("Your input file is too big for the program!\n");
            exit(1);
        }
    }
    //To exclude '/0'
    getchar();
 
    //Apppend the 1 (one) bit at the end of the message.
    buffer[count] = 0x80;
    
    //Debug Code.
    if(DEBUG == 1){
        printf("\nDEBUG MODE FOR readFile \n");
        printf("count = %d\n", count); 
        for(i = 0; i <= count; i++){
            printf("| %d 0x%x |", i, buffer[i]);
        }
    printf("\nTesting end of the array 0x%x\n", buffer[count]);
    printf("Count test: %d\n\n", count);
    }

    return count;   
}

//Function Definition : calculateblocks() will calculate how many block (512) of a file and returns blockCount.
unsigned int calculateBlocks(unsigned int sizeOfFileInBytes){
    
    unsigned int blockCount; 

    blockCount = (((8 * sizeOfFileInBytes) + 1) / 512) + 1;

    if((((8 * sizeOfFileInBytes) + 1) % 512) > (512 - 64)){
        blockCount = blockCount + 1;
    }

    //Debug Code.
    if(DEBUG == 1){
        printf("sizeOfFileInBytes = %d ", sizeOfFileInBytes);
        puts("DEBUG CODE for calculateBlocks");
        printf("Block Count in function = %d\n", blockCount);
    }

    return blockCount;
}

//Function Definition : convertCharArrrayToIntArray() will convert chars into unsigned integers.
void convertCharArrayToIntArray(unsigned char buffer[], unsigned int message[], unsigned int sizeOfFileInBytes){
    
    //Initialize intPacking.
    unsigned int intPack1 = 0;
    unsigned int intPack2 = 0;
    unsigned int intPack3 = 0;
    unsigned int intPack4 = 0;
    
    unsigned int result = 0;

    unsigned int i = 0;
    unsigned int indexNumber = 0;

    unsigned int mask = 0x80000000; 
    //Converts 4 unsigned int into one or more word (8 int).
    for(i = 0; sizeOfFileInBytes >= i; i++){
        intPack1 = buffer[i];
        intPack2 = buffer[++i];
        intPack3 = buffer[++i];
        intPack4 = buffer[++i];
        
        result = ((intPack1 << 24) | (intPack2 << 16) | (intPack3 << 8) | (intPack4));
        
        message[indexNumber] = result;
        
        indexNumber++; 
        if(DEBUG == 1){        
            printf("sizeOfFileInBytes = %d | ", sizeOfFileInBytes);
            printf("message[%d] = %X", i, message[indexNumber]);
    
        } 
    }

    /*
    unsigned int mask = 0x80000000;
    unsigned int j = 0;
    while(sizeOfFileInBytes >= i){
        mask = mask << 8 | buffer[i];
        mask = mask << 8 | buffer[++i];
        mask = mask << 8 | buffer[++i];
        mask = mask << 8 | buffer[++i];
        message[j] = mask;
        i++;
        j++;
            
    }
    */

    //Debug Code
    if(DEBUG == 1){
        puts("DEBUG MODE FOR convertCharArrayToIntArray()");
        printf("i = %d ", i);
        printf("Testing intPack %d : %x %x %x %x \n", (i+1), intPack1, intPack2, intPack3, intPack4);
        printf("result : 0x%x\n\n", message[i]);
        printf("Last 0x%x\n", buffer[sizeOfFileInBytes]);
    } 
}

//Function Definition: addBitCountToLastBlock
void addBitCountToLastBlock(unsigned int message[], unsigned int sizeOfFileInBytes, unsigned int blockCount){
    
    int i = 0;
    int indexOfEndOfLastBlock = 0;
    int indexOfLastWord = 0;

    //8 bits in a byte.
    sizeOfFileInBytes = sizeOfFileInBytes * 8;

    //16 integer array elements in each block.
    indexOfEndOfLastBlock = (blockCount * 16) - 1; 
    
    message[indexOfEndOfLastBlock] = sizeOfFileInBytes;

    //Debug Code
    if(DEBUG == 1){
        puts("DEBUG CODE FOR addBitCountToLastBlock");
        printf("sizeOfFileInBytes = %d\n", sizeOfFileInBytes);
        printf("indexOfEndOfLastBlock = %d \n\n", indexOfEndOfLastBlock);
    } 
}

// ORIGINAL 
//Function Definition: This function will compute the message and digest it.
void computeMessageDigest(unsigned int message[], unsigned int blockCount){

    //Initializing Variable.
    unsigned int H[5]; 
    H[0] = 0x67452301;
    H[1] = 0xEFCDAB89;
    H[2] = 0x98BADCFE;
    H[3] = 0x10325476;
    H[4] = 0xC3D2E1F0;

    unsigned int A = H[0];
    unsigned int B = H[1];
    unsigned int C = H[2];
    unsigned int D = H[3];
    unsigned int E = H[4];

    unsigned int W[W_SIZE];
    unsigned int wTemp;

    unsigned int count = 0;
    unsigned int i = 0;
    int t = 0;

    int bCount = 0;
    int j = 0;
    int b = 0;
    
    if(DEBUG == 1){
        printf("BLOCKCOUNT : %d\n", blockCount);
    }
    
    for(bCount = 0; bCount < blockCount; bCount++){   
        //a
        t = (16*bCount);
        for(j = 0; t < 16*bCount+16; t++){
            
            //Caluclation for block placement.  
            W[j] = message[t];
            j++;
            //printf("W[%d] = %x\n", j, W[j]);
        }
 
        //b
        for(i = 16; i <= 79; i++){
        
            W[i] = (((W[i-3] ^ W[i-8] ^ W[i-14] ^ W[i-16]) << 1) | ((W[i-3] ^ W[i-8] ^ W[i-14] ^ W[i-16]) >> 32 - 1)); //WORKING FOR ABC.TXT
            
            //DEBUG to check W's input.
            if(DEBUG == 1){
                printf("W[%d] = %x\n", i, W[i]);
            }   
        }
        
        //c
        A = H[0];
        B = H[1];
        C = H[2];
        D = H[3];
        E = H[4];

        //d
        for(t = 0; t <= 79; t++){
            wTemp = (((A << 5) | (A >> 32 - 5)) + f(t, B, C, D) + E + W[t] + K(t));
            E = D;
            D = C;
            C = (B << 30) | (B >> 32 - 30);
            B = A; 
            A = wTemp;
            
            //Debug Mode that will list t's result..
            if(DEBUG == 1){
                printf("t = %d: %X %X %X %X %X\n", t, A, B, C, D, E);
            }
        }
        
        //e
        H[0] = H[0] + A;
        H[1] = H[1] + B;
        H[2] = H[2] + C;
        H[3] = H[3] + D;
        H[4] = H[4] + E;
        
        if(DEBUG == 1){
            //Message Padding Result.
            printf("\nMessage Padding Result: %X %X %X %X %X \n", H[0], H[1], H[2], H[3], H[4]);
        }
    }
    printf("\nMessage Digest: %X %X %X %X %X\n", H[0], H[1], H[2], H[3], H[4]);
}

//Function Definition: f funciton will convert certain t into fT using B, C, D.
unsigned int f(unsigned int t, unsigned int B, unsigned int C, unsigned int D){
    
    unsigned int fT;

    if(0 <= t && t <= 19){
        fT = ((B & C) | ((~B) & D));
        return fT;
    }
    if(20 <= t && t <= 39){
        fT = (B ^ C ^ D);
        return fT;
    }
    if(40 <= t && t <= 59){
        fT = ((B & C) | (B & D) | (C & D)); 
        return fT;
    }
    
    if(60 <= t && t <= 79){
        fT = (B ^ C ^ D); 
        return fT;
    }
    
return fT;
}

//Function Defintion: K function will insert t's designated k's unsigned int value.
unsigned int K(unsigned int t){
    
    unsigned int k;

    if(0 <= t && t <= 19){
        k = 0x5A827999;   
    }
    if(20 <= t && t <= 39){
        k = 0x6ED9EBA1;
    }
    if(40 <= t && t <= 59){
        k = 0x8F1BBCDC;
    }
    if(60 <= t && t <= 79){
        k = 0xCA62C1D6;
    }
    return k;
}
