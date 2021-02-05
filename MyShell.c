
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


#define MAXSTR 255
#define ARGCNT 5

char ** args;
char * emptyString;
// Feb 1, 2021



/**
  @brief Fork a child to execute the command using execvp. The parent should wait for the child to terminate
  @param args Null terminated list of arguments (including program).
  @return returns 1, to continue execution and 0 to terminate the MyShell prompt.
 */
int execute(char **args)
{

	// debugging 
	// printf("%s\n", args[0]);

	if (strcmp(args[0], "exit") == 0) {
		printf("%s\n", "Exiting");
		// terminate the loop
		return 0;
	} else if (strcmp(args[0], "\n") == 0) {
		printf("%s\n", "Empty");
		// get back to infinitive loop
		return 1;
	} else if (strcmp(args[0], " ") == 0) {
		printf("%s\n", "Space input");
		return 1;
	}
	// 5. When an empty command is entered, the next "MyShell>" prompt should be shown again.
	// 6. When command "exit" is entered, the next "MyShell>" prompt should be shown.
	
	int rc = fork();
	// 1. char** args is obtained from the parse function
	if (rc < 0) {
		// 3. When fork fails an error message should be printed. After that the next "Myshell>" should be shown again
		fprintf(stderr, "Fork failed\n");
	} else if (rc == 0) {
		// 2. for a child and execute the command typed in by the user using Execvp
		int status = execvp(args[0], args);
		// printf("STATUS CODE = %d\n", status);
		// 4. When execvp is not successful, an error message should be printed, and the child should terminate. After that the next "Myshell>" should be shown again
		printf("Execvp error!\n");
		exit(1);
	} else {
		// 7. The parent should wait for the child to terminate before it accepts another command. Use "wait" or "waitpid" for this.
		wait(NULL);
		return 1;
	}

	return 0;
}


/**
  @brief gets the input from the prompt and splits it into tokens. Prepares the arguments for execvp
  @return returns char** args to be used by execvp
 */
// no parameters
char** parse(void)
{

	args = (char**) malloc(sizeof(char*) * ARGCNT);

	// 1. splits the line into tokens
	// parsing it with delimiter SPACE
	char input[MAXSTR];

	fgets(input, MAXSTR, stdin);
	// for (int i = 0; i <= strlen(input); i++)
	// 	printf("%d: %c\n", input[i], input[i]);

	// space string
	if (input[0] == ' ') {
		args[0] = " ";
		return args;
	}

 	// empty string
	if (input[0] == '\n') {
		args[0] = "\n";
		return args;
	}
	// terminates the string with '\0'
	input[strlen(input)-1] = '\0';

	// 2. Those tokens are used to create a char** pointer which will serve as an argument for execvp
	// storing it into the 2d string array

	char * token = strtok(input, " ");
	int i = 0;
	while (token != NULL) {
		// printf("%s: %lu\n", token, strlen(token));
		emptyString = malloc(sizeof(char) *  (strlen(token+1)));
		strcpy(emptyString, token);
		args[i] = emptyString;
		i++;
		token = strtok(NULL, " ");
	}

// debugging
	// for (int j = 0; j< ARGCNT; j++) {
	// 	for (int i = 0; i <= strlen(args[j]); i++)
	// 		printf("(%d) %d: %c\n", j, *(args[j]+i), *(args[j]+i));

	// }
	// printf("%s\n", args[0]);
	// printf("%s\n", args[1]);
	// printf("%s\n", args[2]);

	// args[0] = "ls";
	// printf("%s\n", args[0]);

// EC
	// if strlen(input) >= MAXSTR, reallocate the array in a bigger size
	return args;
}


/**
   @brief Main function should run infinitely until terminated manually using CTRL+C or typing in the exit command
   It should call the parse() and execute() functions
   @param argc Argument count.
   @param argv Argument vector.
   @return status code
 */
int main(int argc, char **argv)
{
	// printf("MyShell>");
	// parse();

	// indefinitive loop
	int status = 0;
	do {
		printf("MyShell>");
		status = execute(parse());
		// printf("%d\n", status);

	} while (status == 1 );
	
	

	return EXIT_SUCCESS;
}

