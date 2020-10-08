// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <stdint.h>
#include <ctype.h>
#include "dictionary.h"
#include <string.h>
#include <math.h>


// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 1000000;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    int checkHash = hash(word);
    node *checkNode = NULL;//malloc(sizeof(node));
    char *tempWord =calloc(1,sizeof(char)*strlen(word)+1);
    bool wordFound = false;

    // clean-up input word
//printf("Cleaning up input word\n Hash: %i\n",checkHash);
    for(int i = 0; i <= strlen(word);i++){
        tempWord[i]=tolower(word[i]);
        // printf("tempword: %c  word: %c hash: %i\n",tempWord[i],word[i],checkHash);
    }
    if(table[checkHash]!=NULL){
        checkNode = table[checkHash];
    }      //  printf("Hash: %i\n",checkHash);

    while(checkNode!=NULL){
        // printf("tableWord: %s || tempWord: %s || compareResult: %i\n",checkNode->word,tempWord,strcmp(checkNode->word,tempWord));

        if(strcmp(checkNode->word,tempWord)==0){
            //free(checkNode);
            wordFound = true;
        }
        checkNode = checkNode->next;
    }
    //free(checkNode);
    free(tempWord);
    return wordFound;
}

// Hashes word to a number
unsigned int hash(const char *word){

   //printf("%i\n",h);
    int h =0;
   for(int i = 0; (i < 3) && (i <= strlen(word)); i++){
       char letter = tolower(word[i]);
       if(isalpha(letter)==0){
            letter = 125;
       }
       h = h + (letter-97) * pow(10,i);
    //   printf("Letter: %c, currentletter: %c, Hash: %i \n",word[i],letter,h);
   }
    return h;
}

void addNode(int hashNumber, char *inputWord){

    //go to table index hashnumber
    //Check Pointer,
    //Point new node to old top level pointer
    //Point top level pointer to new node
//    printf("test1\n");
    node *nextNode = calloc(1,sizeof(node));
    //nextNode = NULL;
//    printf("test2\n");
    if(table[hashNumber]==NULL){
        strcpy(nextNode->word,inputWord);
        table[hashNumber] = nextNode;
//        printf("test2.5\n");
    } else {
        nextNode->next = table[hashNumber]->next;
        strcpy(nextNode->word,table[hashNumber]->word);
        table[hashNumber]->next = nextNode;
        strcpy(table[hashNumber]->word,inputWord);
//        printf("test3\n");
    }
    //free(nextNode);

//    printf("test4\n");

//    node *testNode = table[hashNumber];

//    while (testNode->next!=NULL){
//    printf("table word: %s ",testNode->word);
//    testNode = testNode->next;
//    }
//    printf("table word: %s ",testNode->word);
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // TODO
    //Open dictionary
    //Iterate through each character for a word
    //if space a word
        //Run hash code
    FILE *openedDictionary = fopen(dictionary,"r");

    char currLetter = 0;
    int wordLength = 0;
    char *currentWord;
    int currentHash = 0;
    currentWord = malloc(LENGTH*sizeof(char)+1);

    while(currLetter!= EOF){
        currLetter = fgetc(openedDictionary);

        if(isspace(currLetter)){
            currentWord[wordLength]='\0';
            // printf("Word: %s\n",currentWord);
            // printf(" Word Length: %i\n",wordLength);
            wordLength=0;
            currentHash = hash(currentWord);
            //printf("%i\n",currentHash);
            addNode(currentHash,currentWord);
            //*table[];
        } else {
            currentWord[wordLength]=currLetter;
            wordLength++;
        }
    }
    free(currentWord);
    fclose(openedDictionary);

    return true;
}



// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    int wordCount=0;
    // printf("test1\n");
    node *currNode =NULL;// malloc(sizeof(node));
    // printf("test2\n");
    for(int i = 0; i < N ; i++){
        if(table[i]!=NULL){
            currNode = table[i];
            while(currNode!=NULL){
                wordCount++;
                currNode = currNode->next;
            }
        }
    }

    //    node *testNode = table[hashNumber];

//    while (testNode->next!=NULL){
//    printf("table word: %s ",testNode->word);
//    testNode = testNode->next;
//    }
//    printf("table word: %s ",testNode->word);

    return wordCount;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    node *iterNode =NULL;
    node *cursorNode = NULL;
    for(int i = 0; i < N; i++){
        iterNode = table[i];
        if(iterNode!=NULL){
            cursorNode = iterNode;
            while(cursorNode != NULL){
                cursorNode = iterNode->next;
                free(iterNode);
                iterNode = cursorNode;
            }
        }
    }
    // TODO
    free(iterNode);
    return true;
}

// void cleanList(node inputNode){
//     node cursorNode = inputNode;
//     if(inputNode.next == NULL){
//         free(inputNode);
//     }
// }
