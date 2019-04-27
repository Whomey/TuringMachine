#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Michael Murphy mmurph27@oswego.edu

struct Node {
    char symbol;
    struct Node *next;
    struct Node *prev;
};

//allows me to have multiple types in a 2D array
struct Instructions {
    char write;
    char moveDir;
    int newState;
};

void prepend(struct Node **head, char val){
	struct Node *temp = malloc(sizeof(struct Node));
	temp->symbol = val;
	
	(*head)->prev = temp;
	temp->next = *head;
	*head = temp;
}

void append(struct Node **head, char val){
	struct Node *temp = malloc(sizeof(struct Node));
	struct Node *tail = *head;
	temp->symbol = val;
	
	if(*head == NULL){
		temp->prev = NULL;
		temp->next = NULL;
		*head = temp;
		return;
	}
	
	while(tail->next != NULL){
		tail = tail->next;
	}
	tail->next = temp;
	temp->next = NULL;
	temp->prev = tail;
}

//just prints our linked list or "tape"
void printTape(struct Node **head){
	struct Node *temp = *head;
    while(temp != NULL){
        printf("%c", temp->symbol);
        temp = temp->next;
    }
}

int main() {
    FILE *fp;
    char BUFFER[100];
    char text[200] = "";
    char input[200];
    char readS, writeS, dir;
    int state, nState, states, startState, endState;
    char fileName[100];
	

    printf("\nInput file: ");
    scanf("%s", &fileName);
	
	strcat(fileName, ".txt");

    //assuming file will be in same directory
    fp = fopen(fileName, "r");

    printf("Writing tape... \n");

    fscanf(fp, "%s", input);
    fscanf(fp, "%d", &states);
    fscanf(fp, "%d", &startState);
    fscanf(fp, "%d", &endState);
    printf("Initial tape contents: %s\n", input);

    //creating and allocating out 2D array of States/Instructions based on states size
    struct Instructions **instructArray = (struct Instructions **)malloc(sizeof(struct Instructions) * states);
    for(int i = 0; i < states; i++){
        instructArray[i] = malloc(sizeof(char) * 255);
    }

    //parse and assign instructions
    while(fgets(BUFFER, sizeof(BUFFER), fp) != NULL) {

        fscanf(fp, "%d %c %c %c %d", &state, &readS, &writeS, &dir, &nState);

        instructArray[state][readS].write = writeS;
        instructArray[state][readS].moveDir = dir;
        instructArray[state][readS].newState = nState;
	
		/*printf("%d", state);
		printf("%c", readS);
		printf("%c", instructArray[state][readS].write);
		printf("%c", instructArray[state][readS].moveDir);
		printf("%d\n", instructArray[state][readS].newState);*/
    }
	
	

    //creates our linked list from input
    struct Node *head = NULL;
    for(int i = 0; i < strlen(input); i++){
        append(&head, input[i]);
    }

    //working through our linked list with the actual instructions
    struct Node *curr = head;
	int currState = startState;
	char currSymbol;
	char direction;
	
    while(currState != endState) {
		currSymbol = curr->symbol;
		
		/*printf("State: %d\n", currState);
		printf("Read: %c\n", currSymbol);
		printf("Write: %c\n", instructArray[currState][currSymbol].write);*/
		
		curr->symbol = instructArray[currState][currSymbol].write;
		direction = instructArray[currState][currSymbol].moveDir;
		
		if(direction == 'L'){
			//printf("GOING LEFT\n");
			
			if(curr->prev == NULL) {
				//printf("Prepend\n");
				prepend(&head, 'B');
            }
			
			curr = curr->prev;
			currState = instructArray[currState][currSymbol].newState;
		}
		else if (direction == 'R'){
			//printf("GOING RIGHT\n");
			
			if(curr->next == NULL) {
				//printf("Append\n");
				append(&head, 'B');
			}
				
			curr = curr->next;
			currState = instructArray[currState][currSymbol].newState;
		}	
    }

    printf("Final Tape Contents: ");
    printTape(&head);

    return 0;
}