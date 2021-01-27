#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int size;
	char **items;
} tokenlist;

char *get_input(void);
tokenlist *get_tokens(char *input);

tokenlist *new_tokenlist(void);
void add_token(tokenlist *tokens, char *item);
void free_tokens(tokenlist *tokens);

int main()
{

	while (1) {
		printf("%s@%s : %s> ",getenv("USER"),getenv("HOSTNAME"),getenv("PWD"));

		/* input contains the whole command
		 * tokens contains substrings from input split by spaces
		 */


		char *input = get_input();
		printf("whole input: %s\n", input);


		tokenlist *tokens = get_tokens(input);
        printf("input address: %pn\n",input);
        printf("tokens address: %pn\n",tokens);
        printf("items address: %pn\n",tokens->items);

        for (int i = 1; i < tokens->size; i++) {
            const char c = '$';
            char * dollarPtr = strchr(tokens->items[i], c);
            if (dollarPtr != NULL) {
                char * temp = dollarPtr + 1;
                tokens->items[i] = getenv(temp);
                printf("%s\n", getenv(dollarPtr));

            }
        }

        //tilde expansion --------------------------
        const char tilde = '~';
        char full_path[100];
        char * home = getenv("HOME");
        for (int i = 1; i < tokens->size; i++) {
            char * tildePtr = strchr(tokens->items[i], tilde);
            if (tildePtr != NULL) { //if there is a tilde in a token
                char* with_tilde = strchr(tokens->items[i],tilde); //set token with tilde into variable
                char* without_tilde = with_tilde + 1;    //change variable to filename without the tilde
                char * realPath = strcat(home,without_tilde);
                tokens->items[i] = realPath;


                printf(realPath);
            }
        }



        if(strcmp(tokens->items[0],"echo") == 0)
        {
            for (int i = 1; i < tokens->size; i++) {

                printf("%s",tokens->items[i]);


            }
            printf("\n");
        }
        else if(tokens->items[0] == "ls")
        {

        }
		for (int i = 0; i < tokens->size; i++) {
			printf("token %d: (%s)\n", i, tokens->items[i]);
		}








        //free(input);
		//free_tokens(tokens);
	}

	return 0;
}

tokenlist *new_tokenlist(void)
{
	tokenlist *tokens = (tokenlist *) malloc(sizeof(tokenlist));
	tokens->size = 0;
	tokens->items = (char **) malloc(sizeof(char *));
	tokens->items[0] = NULL; /* make NULL terminated */
	return tokens;
}

void add_token(tokenlist *tokens, char *item)
{
	int i = tokens->size;

	tokens->items = (char **) realloc(tokens->items, (i + 2) * sizeof(char *));
	tokens->items[i] = (char *) malloc(strlen(item) + 1);
	tokens->items[i + 1] = NULL;
	strcpy(tokens->items[i], item);

	tokens->size += 1;
}

char *get_input(void)
{
	char *buffer = NULL;
	int bufsize = 0;

	char line[5];
	while (fgets(line, 5, stdin) != NULL) {
		int addby = 0;
		char *newln = strchr(line, '\n');
		if (newln != NULL)
			addby = newln - line;
		else
			addby = 5 - 1;

		buffer = (char *) realloc(buffer, bufsize + addby);
		memcpy(&buffer[bufsize], line, addby);
		bufsize += addby;

		if (newln != NULL)
			break;
	}

	buffer = (char *) realloc(buffer, bufsize + 1);
	buffer[bufsize] = 0;

	return buffer;
}

tokenlist *get_tokens(char *input)
{
	char *buf = (char *) malloc(strlen(input) + 1);
	strcpy(buf, input);

	tokenlist *tokens = new_tokenlist();

	char *tok = strtok(buf, " ");
	while (tok != NULL) {
		add_token(tokens, tok);
		tok = strtok(NULL, " ");
	}

	free(buf);
	return tokens;
}

void free_tokens(tokenlist *tokens)
{
	for (int i = 0; i < tokens->size; i++)
		free(tokens->items[i]);
	free(tokens->items);
	free(tokens);
}
