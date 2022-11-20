all: minishell myecho

minishell: minishell.o get_input.o
	gcc -o minishell minishell.o get_input.o

get_input.o: get_input.c
	gcc -g -Wall -o get_input.o -c get_input.c

minishell.o: minishell.c
	gcc -g -Wall -o minishell.o -c minishell.c

myecho: myecho.c
	gcc -g -Wall -o myecho myecho.c

clean:
	rm -f *~ *.o minishell myecho
