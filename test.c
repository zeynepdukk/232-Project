#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

struct ArrayTable {
	char name[10]; // array name
	char size[5]; // array size
	char type[10]; // int, float, etc.
};
struct ArrayTable AT[20];

char* read(char *file, char *name);
char* declare(char *array, char *n, char *type);
char* print(char *name);
char* copy(char *destination, char *source);
char* arithmetic(char *array, char *array2, char *array3, char *op);
char* initialize(char* array, char *number);
bool StartsWith(const char *a, const char *b);

int main(void) {
    int i=0;
    int numProgs=0;
    char* programs[160];
    char line[255];
	char * token;
	int count = 0;

    FILE *file;
    file = fopen("test.txt", "r");

    while(fgets(line, sizeof line, file)!=NULL) {
        programs[i]=strdup(line);
        i++;
        numProgs++;
    }

    //check to be sure going into array correctly
    for (int j=0 ; j<numProgs+1; j++) {
        if(programs[j]!=NULL && StartsWith(programs[j], "$")){
			if(programs[j]!=NULL && StartsWith(programs[j], "$dec")){
				token = strtok(programs[j], "$(,)=+-/* ;");
				token = strtok(NULL, "$(,)=+-/* ;"); // array_name
				strcpy(AT[count].name, token);
				token = strtok(NULL, "$(,)=+-/* ;"); //array_size
				strcpy(AT[count].size, token);
				token = strtok(NULL, "$(,)=+-/* ;"); //array_type
				strcpy(AT[count].type, token);
				printf("%s",declare(AT[count].name, AT[count].size, AT[count].type));
				count++;
			}
			else if(programs[j]!=NULL && StartsWith(programs[j], "$read")){
				token = strtok(programs[j], "$(,)=+-/* ;"); //read
				token = strtok(NULL, "$(,)=+-/* ;"); //file_name
				char * tmp = token; //get file_name to tmp
				token = strtok(NULL, "$(,)=+-/* ;"); // array_name
				printf("%s",read(tmp, token)); // tmp = file_name, token = array_name
			}
			else if(programs[j]!=NULL && StartsWith(programs[j], "$print")){
				token = strtok(programs[j], "$(,)=+-/* ;"); //print
				token = strtok(NULL, "$(,)=+-/* ;"); // array_name
				printf("%s",print(token));
			}
			else{
				token = strtok(programs[j], "$(,)= ;"); // array_name
				char *a_name = token;
				token = strtok(NULL, "$(,)= ;");
				if(isdigit(token[0])){// check token starts with digit or not.
					char *a_name2 = token;
					token = strtok(NULL, "$(,)= ;");
					if(StartsWith(token, "+") || StartsWith(token, "-") || StartsWith(token, "*") || StartsWith(token, "\\")){
						//$x = 5 + $y
						char* operation = token;
						token = strtok(NULL, "$(,)= ;");
						printf("%s", arithmetic(a_name, a_name2, token, operation));
						
					}
					else{
						//$x = 5;
						printf("%s", initialize(a_name, a_name2));
					}
				} 
				else{
					char *a_name2 = token;
					token = strtok(NULL, "$(,)= ;");
					if(StartsWith(token, "+") || StartsWith(token, "-") || StartsWith(token, "*") || StartsWith(token, "\\")){
						char* operation = token;
						token = strtok(NULL, "$(,)= ;");
						printf("%s", arithmetic(a_name, a_name2, token, operation));
						
						/*if(isdigit(token[0])){
							char *number = token; //$A = $B + 5
							printf("%s", arithmetic(a_name, a_name2, number, operation));
							
						}
						else{
							char * a_name3 = token; //$A = $A + $B
							printf("%s", arithmetic(a_name, a_name2, a_name3, operation));
						}*/
					}
					else if(StartsWith(token, "\n")){
						printf("%s",copy(a_name, a_name2));//$x = $y
					}
				}
			}
        }
		else if(programs[j]!=NULL)
			printf("%s", programs[j]); //printf yerine texte yazdırma yapılacak
    }

    fclose(file);
    return 0;
}

char* read(char *file, char *name)  {
	int i =0;
	for(i = 0; i<20; i++){
		if (!(strcmp(name, AT[i].name))){
			break;
		}
	}
	
	char * string;
	string = malloc(sizeof(char)*100);
	strcat(string, "	FILE *ff=fopen(\"");
	strcat(string, file);
	strcat(string, "\", \"r\");\n");
	strcat(string,"	for (int i=0; i<");
	strcat(string, AT[i].size);
	strcat(string,"; i++)\n");
	strcat(string,"		fscanf(ff, \"%d\", &");
	strcat(string, name);
	strcat(string, "[i]);\n	close(ff);\n");
	return string;
}

char* declare(char *array, char *n, char *type)  {
	
	char * string;
	string = malloc(sizeof(char)*100);
	strcat(string, "	");
	strcat(string, type);
	strcat(string, " ");
	strcat(string, array);
	strcat(string, "[");
	strcat(string, n);
	strcat(string, "];\n");
	return string;
}

char* print(char *name)  {
	int i =0;
	for(i = 0; i<20; i++){
		if (!(strcmp(name, AT[i].name))){
			break;
		}
	}
	
	char * string;
	string = malloc(sizeof(char)*100);
	strcat(string, "	for (int i=0; i<");
	strcat(string, AT[i].size);
	strcat(string, "; i++)\n");
	strcat(string, "	printf(\"%d \", &");
	strcat(string, AT[i].name);
	strcat(string, "[i])\n");
	return string;
}

char* copy(char *destination, char *source)  {
	
	int i =0;
	for(i = 0; i<20; i++){
		if (!(strcmp(destination, AT[i].name))){
			break;
		}
	}
	
	char * string;
	string = malloc(sizeof(char)*100);
	strcat(string, "	for(i=0; i<");
	strcat(string, AT[i].size);
	strcat(string, "; i++)\n		");
	strcat(string, destination);
	strcat(string, "[i] = ");
	strcat(string, source);
	strcat(string, "[i];\n");
	return string;
}

char* arithmetic(char *array, char *array2, char *array3, char *op)  {
	
	int i =0;
	for(i = 0; i<20; i++){
		if (!(strcmp(array, AT[i].name))){
			break;
		}
	}
		
	
	char * string;
	string = malloc(sizeof(char)*100);
	strcat(string, "	for(i=0; i<");
	strcat(string, AT[i].size);
	strcat(string, "; i++)\n		");
	strcat(string, array);
	strcat(string, "[i] =" );
	strcat(string, array2);
	if(!(isdigit(array2[0])))
		strcat(string, "[i]");
	strcat(string, " + ");
	strcat(string, array3);
	if(isdigit(array3[0]))
		strcat(string, ";\n");
	else
		strcat(string, "[i];\n");
	return string;
}

char* initialize(char *array,char *number){

	int i =0;
	for(i = 0; i<20; i++){
		if (!(strcmp(array, AT[i].name))){
			break;
		}
	}

	char * string;
	string = malloc(sizeof(char)*100);
	strcat(string, "	for(i=0; i<");
	strcat(string, AT[i].size);
	strcat(string, "; i++)\n		");
	strcat(string, array);
	strcat(string, "[i] = ");
	strcat(string, number);
	strcat(string, ";\n");
	return string;


}

bool StartsWith(const char *a, const char *b)
{
   if(strncmp(a, b, strlen(b)) == 0) return 1;
   return 0;
}