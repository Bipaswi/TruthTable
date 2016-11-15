#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "header.h"

int counter = 0;
int ansStack[256];
int numberOfBracketsLeft = 0;
int numberOfBracketsRight = 0;
int numOperator = 0;
	
	int main(int argc, char **argv){
		printf("%d\n", argc);
		int rpn[256] = {0};
		char var[27] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 0};
		
		int n = strlen(argv[2]);
		int pos = 0;
		int rpnPos=0;
	 	
	 	countBO(argv[2], n);

	 	//check validity ill formed formula or not
	 	if((numberOfBracketsLeft == numberOfBracketsRight) && ((numberOfBracketsLeft + numberOfBracketsRight) > numOperator)){
	 	shunt(&pos, &rpnPos, argv[2], n, rpn);
	 	}
	 	else{
	 		printf("%s", "invalid input");
	 	}
	 	
	 	//prints the table here
	 	int x;
		x = atoi (argv[1]);
		printf("\n");
		if (x<=26){
		for (int a = 0; a < x; a++){
			printf("%c ", var[a]);
		}
		printFormula(argv[2]);
		printf( " : Result\n" );
		for(int i =0; i < power(2, x); i++){
			counter = i;
			char result[n];
			memset( result, ' ', n * sizeof(char) );
			bool answer = evalRpn(result, argv[2], rpnPos, rpn);
			for(int j =0; j < x; j++){
				char temp = var[j];
				int value = (counter>>(temp-'a'))%2;
				printf("%d ", value);
			}
			for(int j=0;j<n;j++)
				printf( "%c", result[j] );
				printf( " :   %d", answer );printf("\n");
		}
		printf("\n");
		}
		else{
			printf("too big");
		}
		
		// char add[n];
		// memset( add, ' ', n * sizeof(char) );
		// checkValid(x, add, argv[2], rpnPos, rpn);
	}

	//counts number of brackets and operators to check validity
	void countBO(char formula[], int fLength){
	for(int b = 0; b < fLength; b++){
	 		if (formula[b] == '('){
	 			numberOfBracketsLeft++;
	 		}
	 		if (formula[b] == ')'){
	 			numberOfBracketsRight++;
	 		}
	 		if ((formula[b] == '&') || (formula[b] == '|') || (formula[b] == '#') || (formula[b] == '>') || (formula[b] == '=')){
	 			numOperator++;
	 		}
	 	}
	 }
	// power function to create table
	int power(int nr, int power){
		int hi = nr;
		for (int i=1; i< power; i++){
			nr = hi*nr;
		}
		return nr;
	}
	
	//prints formula
	void printFormula(char formula[]){
		for (int i = 0; formula[i] != '\0'; i++){
			printf("%c", formula[i]);
		}
	}

	//shunting algorithm to parse the formula without having to worry about brackets
	#define curPos (*pos)
	#define rpnPosition (*rpnPos)

	void shunt(int* pos, int* rpnPos, char formula[], int fLength, int* rpn){
		int operStack[256];
		int oprPos = -1;

		while(curPos < fLength) {
			if (formula[curPos] == '(') {
			 curPos++;
			 shunt(pos, rpnPos, formula, fLength, rpn );
			}
			if (formula[curPos] == ')'){
				curPos++;
				break;
			}
			if(formula[curPos] >= '0' && formula[curPos] <= '9'){
				rpn[rpnPosition] = curPos;
				(rpnPosition)++;
			}
			else if(formula[curPos] >= 'a' && formula[curPos] <= 'z'){
				rpn[rpnPosition] = curPos;
				(rpnPosition)++;
			}
			else if(isOperator(formula[curPos])) {
				while(oprPos!=-1){
					int o = operStack[oprPos];
					//
					oprPos--;
					if (precedence(formula[o]) >= precedence(formula[curPos])){
					rpn[rpnPosition] = o;
					(rpnPosition)++;
					}
					else{
						oprPos++;
						operStack[oprPos] = o;
			 			break;
					}
				}
			 	oprPos++;
				operStack[oprPos] = curPos;
			}
			curPos++;
		}
		while (oprPos!=-1){
      		rpn[rpnPosition] = operStack[oprPos];
      		(rpnPosition)++;
      		oprPos--;
		 }
	}

	//defines precedence of formulas
	int precedence(char operator){
		if (operator == '-'){
			return 6;
		}
		else if(operator == '&'){
			return 5;
		}
		else if(operator == '|'){
			return 4;
		}
		else if(operator == '#'){
			return 3;
		}
		else if(operator == '>'){
			return 2;
		}
		else if(operator == '='){
			return 1;
		}
		else{
			return -1;
		}
	}

	//checks if current charcter is an operator
	bool isOperator(char operator){
		if (operator == '-'){
			return true;
		}
		else if(operator == '&'){
			return true;
		}
		else if(operator == '|'){
			return true;
		}
		else if(operator == '#'){
			return true;
		}
		else if(operator == '>'){
			return true;
		}
		else if(operator == '='){
			return true;
		}
		else{
			return false;
		}
	}

	//evaluates the rpn with all possibilites
	#define POP stack[--stackSize]
	#define PUSH(x) stack[stackSize++] = (x)

	bool evalRpn(char* result, char* formula,int rpnPos, int* rpn){
		bool stack[256];
		int stackSize = 0;
		for(int i=0;i<rpnPos;i++){
			char c = formula[rpn[i]];
			switch(c){
				case '-':{
					bool p = POP;
					PUSH (!p);
					result[rpn[i]] = (!p)+'0';
					break;
				}
				case '&':{
					bool p = POP;
					bool q = POP;
					PUSH (p && q);
					
					result[rpn[i]] = (p&&q)+'0';
					break;
				}
				case '|':{
					bool p = POP;
					bool q = POP;
					PUSH (p || q);
					
					result[rpn[i]] = (p||q)+'0';
					break;
				}
				case '#':{
					bool p = POP;
					bool q = POP;
					PUSH (p != q);
					result[rpn[i]] = (p!=q)+'0';
					break;
				}
				case '>':{
					bool p = POP;
					bool q = POP;
					PUSH (p<=q);
					result[rpn[i]] = (p <= q)+'0';
					break;
				}
				case '=':{
					bool p = POP;
					bool q = POP;
					PUSH (p == q);
					result[rpn[i]] = (p==q)+'0';
					break;
				}
				case '0':{
					PUSH (0);
					result[rpn[i]] = '0';
					break;
				}
				case '1':{
					PUSH (1);
					result[rpn[i]] = '1';
					break;
				}
				default :{
					if(c >= 'a' && c <= 'z'){
					PUSH (counter>>(c-'a'))%2;
					}
					else{
						printf("%s\n", "unrecognised character");
						exit(1);
					}
					break;
				}
			}
		}
		return POP;
	 }

	 //extension
	 //check valid

	 // void checkValid(int x, char* result, char formula[], int rpnPos, int* rpn){
	 // 	bool array[power(2, x)];
	 // 	printf("%s\n", "hi");
		// bool check = 0;
	 // 	for(int i =0; i < power(2, x); i++){
	 // 		array[i] = evalRpn(result, formula, rpnPos, rpn);
	 // 	}
	 // 	for(int j =0; j < power(2, x); j++){
	 // 		if(array[j] == 1){
	 // 			printf("%s", "valid");
	 // 		}
	 // 		else{
	 // 			printf("%s", "invalid");
	 // 		}
	 // 	}
	 // }





