#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
int strength_of_victory(pair inputPair);
int checkIfCycle(int startPoint,int endPoint);
int findWinner(int currWinner);
void printLockedGraph(void);


int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO - maybe duplicate check if that's important
//    string preVotedCandidates[MAX];
    for(int i =0; i<candidate_count;i++){
         if(strcmp(candidates[i],name)==0){
             ranks[rank]=i;
             return true;
         }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    int highIndex=0;
    int lowIndex=0;
    for(int i = 0; i < candidate_count; i++){
        for(int j = i+1; j < candidate_count; j++){
            highIndex=ranks[i];
            lowIndex=ranks[j];
            preferences[highIndex][lowIndex]++;
//            printf("%i %i %i\n",highIndex,lowIndex,preferences[highIndex][lowIndex]);
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    pair_count=0;
    for(int i = 0; i < candidate_count;i++){
        for(int j = 1; j< candidate_count-i;j++){
            if (preferences[i][i+j] > preferences[i+j][i]){
                pairs[pair_count].winner=i;
                pairs[pair_count].loser=i+j;
            } else if(preferences[i][i+j] < preferences[i+j][i]){
                pairs[pair_count].loser=i;
                pairs[pair_count].winner=i+j;
            } else {
                pair_count=pair_count-1;
            }
//            printf("%i  %i\n",pairs[pair_count].loser,pairs[pair_count].winner);
//            printf("%i  %i\n",i,i+j);
            pair_count++;
        }
    }
//    printf("%i\n",pair_count);
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int firstPair;
    int secondPair;
    pair lowerPair;
    for(int i =0; i<pair_count; i++){
        for(int j = 1; j < pair_count-i; j++){
            firstPair = strength_of_victory(pairs[i]);
            secondPair = strength_of_victory(pairs[i+j]);
//            printf("%i   %i\n",pairs[j].winner,pairs[j].loser);
            if( secondPair > firstPair ){
                lowerPair = pairs[i];
                pairs[i]=pairs[i+j];
                pairs[i+j]=lowerPair;
            }
        }
    }
    for(int i = 0; i < pair_count; i++){
//        printf("%i   %i     %i\n",pairs[i].winner,pairs[i].loser,strength_of_victory(pairs[i]));
    }
    return;
}

int strength_of_victory(pair inputPair)
{
    return preferences[inputPair.winner][inputPair.loser] - preferences[inputPair.loser][inputPair.winner];
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    int winnerPair=0;
    int loserPair=0;
    for(int i = 0; i < pair_count;i++){
        if (checkIfCycle(pairs[i].winner,pairs[i].loser)==0)
        {
            winnerPair=pairs[i].winner;
            loserPair=pairs[i].loser;
            locked[winnerPair][loserPair]=true;
//            printf("Locked in %i over %i\n",winnerPair,loserPair);
        } else {
//            printf("Did not lock %i over %i\n",pairs[i].winner,pairs[i].loser);
        }
    }
    return;
}

int checkIfCycle(int startPoint,int endPoint){
    int notFoundBool=0;
    for(int i = 0; i < candidate_count; i++){
        if(locked[i][startPoint]){  //Check to see if any locked pair has startPoint as loser
            if(endPoint == i){
                return 1; //Loop found
            } else {
                notFoundBool=notFoundBool+checkIfCycle(i, endPoint); //Recursively run through all other. Might run poorly. Who knows
            }
        }
    }
    return notFoundBool;
}

// Print the winner of the election
void print_winner(void)
{
//    printLockedGraph();
    // int didWin=0;
    // for(int i=0; i<pair_count;i++){
    //     if(locked[pairs[0].winner][i]){
    //         didWin=1;
    //     }
    // }
 //   if (didWin==1){
    int winner;
    for(int i =0; i < candidate_count;i++){
        int doesilose=0;
        for(int j =0; j<candidate_count;j++){
            if(locked[j][i]){
                doesilose=1;
            }
        }
        if(doesilose==0){
            winner=i;
            printf("%s\n",candidates[winner]);

        }
    }
   // }
}

void printLockedGraph(void){

    printf("   ");
    for(int h = 0; h<candidate_count; h++){
        printf("%s  ",candidates[h]);
    }
    printf("\n");
    for (int i =0; i < candidate_count; i++){
    printf("%s  ",candidates[i]);
        for(int j=0; j<candidate_count;j++){
            if(i==j){
                printf("x  ");
            } else {
                printf("%i  ",locked[i][j]);
            }
        }
    printf("\n");
    }
}

int findWinner(int currWinner){
    for(int i =0; i < pair_count; i++){
        if (locked[i][currWinner]){
            return findWinner(i);
        }
    }
    return 0;
}
