#include <iostream>
#include <string.h>
#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <fstream>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <vector>



using namespace std;

#define TOKENSIZE 100

using namespace std;
void StrTokenizer(char *line, char **argv);
void myExecvp(char **argv);
void lsCommand(char **argv);
void touchCommand(char **argv);
void rmdirCommand(char **argv);
void mkdirCommand(char **argv);

void clearCommand(char **argv);
void pwdCommand(char **argv);
void echoCommand(char **argv);
void rmCommand(char **argv);

void catCommand(char **argv);
void cdCommand(char **argv);
void lnCommand(char **argv);
void historyCommand();
void execute(int cmdNo);


string currDir = "C-Shell:";

vector<char**> history;


int main()
{

    char *path2;
    char *arr[250];
    char *Tokenized;
    char input[250];
    char *argv[TOKENSIZE];
	

    while (true)
    {
        cout << currDir;
        cin.getline(input, 250);
        StrTokenizer(input, argv);
        
        if (strcmp(input, "history") != 0 && input[0] != '!') {
		    char** argvCopy = (char**)calloc(250, sizeof(char*));
		    int i = 0;
		    while (argv[i] != NULL) {
		    	argvCopy[i] = (char*)calloc(10, sizeof(char));
		    	strcpy(argvCopy[i], argv[i]);
		    	i++;
		    }
		    argvCopy[i] = NULL;
		    history.push_back(argvCopy);
		}
		
		
        if (strcmp(input, "exit") == 0) {
            break;
        }
        else if (strcmp(input, "\n") == 0) {
            continue;
        }
        else if (strcmp(input, "cat") == 0) {
            catCommand(argv);
            continue;
        }
        else if (strcmp(input, "rm") == 0) {
            rmCommand(argv);
            continue;
        }
        else if (strcmp(input, "rmdir") == 0) {
            rmdirCommand(argv);
            continue;
        }
        else if (strcmp(input, "mkdir") == 0) {
            mkdirCommand(argv);
            continue;
        }
        else if (strcmp(input, "clear") == 0) {
            clearCommand(argv);
            continue;
        }
        else if (strcmp(input, "pwd") == 0) {
            pwdCommand(argv);
            continue;
        }
        else if (strcmp(input, "echo") == 0) {
            echoCommand(argv);
            continue;
        }
        else if (strcmp(input, "ln") == 0) {
            lnCommand(argv);
            continue;
        }
        else if (strcmp(input, "touch") == 0) {
            touchCommand(argv);
            continue;
        }
        else if (strcmp(input, "ls") == 0) {
            lsCommand(argv);
            continue;
        }
        else if (strcmp(input, "cd") == 0) {
            cdCommand(argv);
            continue;
        }
        else if (strcmp(input, "history") == 0) {
            historyCommand();
            continue;
        }
        else if (input[0] == '!') {
      		int j, result = 0;
			for (j = 1; input[j] != '\0'; j++) {
				if (isdigit(input[j]))
					result = result * 10 + (input[j] - '0');
				else
					break;
			}
        	execute(result);
        	continue;
        }
        
         myExecvp(argv);
         cout << "The above command is executed using execvp()" << endl;
    }
    
    
    int y;
    int x = history.size() - 1;
    while (history.size() > 0) {
		y = 0;
		while (history[y] != NULL) {
			free(history[x][y]);
			y++;
		}
		
		free(history[x]);
		history.pop_back();
		x--;
	}
	
    return 0;
}

void clearCommand(char **argv)
{
    write(1, "\33[H\33[2J", 7);
    // This escape sequence instructs the terminal to move the cursor to the top-left position (\033[H) and clear the entire screen (\033[2J)
}

void rmCommand(char **argv)
{
    // If no arguments are specified, print an error message
    if (argv[1] == NULL)
    {
        cerr << "C-Shell : At least one argument required" << endl;
        return;
    }

    // Loop through the available arguments (files)
    int i = 1;
    for (i = 1; argv[i] != NULL; i++)
    {
        // Try deleting the file
        if (unlink(argv[i]) < 0)
        {
            fprintf(stderr, "C-Shell: %s - ", argv[i]);
            perror("");
        }
    }
}



void echoCommand(char **argv)
{
    if (argv[1] == NULL)
    {
        cerr << "C-Shell: At least one argument required" << endl;
        return;
    }

    for (int i = 1; argv[i] != NULL; i++)
    {
        cout << argv[i] << " ";
    }
    cout << endl;
}


void pwdCommand(char **argv)
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        cout << cwd << endl;
    }
    else
    {
        perror("C-Shell: getcwd() error");
    }
}

void catCommand(char **argv)
{
    // If no arguments are specified, read from standard input
    if (argv[1] == NULL)
    {
        // Display data from input to output
        string line;
        while (getline(cin, line))
        {
            cout << line << endl;
        }
        return;
    }

    // Loop through the available arguments (files)
    int i = 1;
    string filename;
    for (i = 1; argv[i] != NULL; i++)
    {
        filename = argv[i];
        ifstream file(filename);
        if (!file)
        {
            cerr << "C-Shell: " << filename << ": Unable to open file" << endl;
            return;
        }

        string line;
        cout << endl
             << filename << ":" << endl
             << endl;
        while (getline(file, line))
        {
            cout << line << endl;
        }
        file.close();
    }
    cout << endl;
}


void rmdirCommand(char **argv)
{
    if (argv[1] == NULL)
    {
        fprintf(stderr, "C-Shell: Argument required\n");
        return;
    }
    int i = 1;
    for (i = 1; argv[i] != NULL; i++)
    {
        // Try deleting the directory
        if (rmdir(argv[i]) < 0)
        {
            fprintf(stderr, "C-Shell: %s - ", argv[i]);
            perror("");
        }
    }
}


void mkdirCommand(char **argv)
{
	if (argv[1] == NULL){
		fprintf(stderr, "minsh: Argument required\n");
		return;
	}
	int i = 1;
	for (i = 1; argv[i] != NULL; i++){
		// Try creating the directory
		if ( mkdir(argv[i], 0755) < 0 ){
			fprintf(stderr, "minsh: %s - ", argv[i]);
			perror("");
		}
	}
}



void touchCommand(char **argv)
{
    // If no arguments are specified, print an error message
    if (argv[1] == NULL)
    {
        cerr << "C-Shell: At least one argument required" << endl;
        return;
    }

    // Loop through the available arguments (files)
    int i = 1;
    for (i = 1; argv[i] != NULL; i++)
    {
        // Use open() system call to create or update the file
        int fd = open(argv[i], O_CREAT | O_WRONLY, 0644);
        if (fd < 0)
        {
            fprintf(stderr, "C-Shell: %s - ", argv[i]);
            perror("");
            continue;
        }
    }
}



void lnCommand(char **argv)
{
    char option = '\0';

    if (argv[1] == NULL || argv[2] == NULL)
    {
        fprintf(stderr, "C-Shell: Too few arguments\n");
        return;
    }

    if ((argv[1][0] != '-' && argv[3] != NULL))
    {
        fprintf(stderr, "C-Shell: Too many arguments\n");
        return;
    }

    // Check if an option is specified
    if (argv[1][0] == '-')
    {
        if (argv[4] != NULL)
        {
            fprintf(stderr, "C-Shell: Too many arguments\n");
            return;
        }
        option = argv[1][1];
    }

    switch (option)
    {
    case '\0': // Hard Link
        if (link(argv[1], argv[2]) < 0)
        {
            perror("ln");
        }
        break;
    case 's': // Soft Link
        if (argv[3] == NULL)
        {
            fprintf(stderr, "C-Shell: Too few arguments for symbolic link\n");
            return;
        }
        if (symlink(argv[2], argv[3]) < 0)
        {
            perror("ln");
        }
        break;
    default:
        fprintf(stderr, "C-Shell: Invalid argument\n");
        return;
    }
}






void lsCommand(char **argv) 
{
	const char* directoryPath = (argv[1] != NULL) ? argv[1] : ".";

    DIR* directory;
    struct dirent* entry;

    directory = opendir(directoryPath);
    if (directory) {
        while ((entry = readdir(directory)) != nullptr) {
            cout << entry->d_name << "  ";
        }
        cout << endl;
        closedir(directory);
    } 
    else {
        cout << "Error opening directory." << endl;
        return;
    }

}


void cdCommand(char **argv) 
{
	if (argv[2] != NULL) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        return;
    }

    if (chdir(argv[1]) == -1) {
        perror("cd");
        return;
    }
    if (strcmp(argv[1], "..") == 0) {
  		int i = currDir.length() - 1;
    	while (currDir[i] != '/') {
			currDir.pop_back();
			i--;
    	}
    	currDir[i] = ':';
    }
    else {
    	if (currDir != "C-Shell->")
    		currDir.pop_back();
    	currDir = currDir + '/' + argv[1] + ":";
    }
}


void historyCommand()
{
	for (int i = 0; i < history.size(); i++) {
		cout << i+1 << " ";
		int j = 0;
		while (history[i][j] != NULL) {
			cout << history[i][j] << " ";
			j++;
		}
		cout << endl;
	}
	cout << endl;
}

void execute(int cmdNo)
{
		char *input = history[cmdNo-1][0];
		char **argv = history[cmdNo-1];
		if (strcmp(input, "exit") == 0) {
            exit(0);
        }
        else if (strcmp(input, "\n") == 0) {
            return;
        }
        else if (strcmp(input, "cat") == 0) {
            catCommand(argv);
        }
        else if (strcmp(input, "rm") == 0) {
            rmCommand(argv);
        }
        else if (strcmp(input, "rmdir") == 0) {
            rmdirCommand(argv);
        }
        else if (strcmp(input, "mkdir") == 0) {
            mkdirCommand(argv);
        }
        else if (strcmp(input, "clear") == 0) {
            clearCommand(argv);
        }
        else if (strcmp(input, "pwd") == 0) {
            pwdCommand(argv);
        }
        else if (strcmp(input, "echo") == 0) {
            echoCommand(argv);
        }
        else if (strcmp(input, "ln") == 0) {
            lnCommand(argv);
        }
        else if (strcmp(input, "touch") == 0) {
            touchCommand(argv);
        }
        else if (strcmp(input, "ls") == 0) {
            lsCommand(argv);
        }
        else if (strcmp(input, "cd") == 0) {
            cdCommand(argv);
        }
}






void myExecvp(char **argv)
{
    pid_t pid;
    int status;
    int childStatus;
    pid = fork();
    if (pid == 0)
    {
        childStatus = execvp(*argv, argv);
        if (childStatus < 0)
        {
            cout << "ERROR:wrong input" << endl;
        }
        exit(0);
    }
    else if (pid < 0)
    {
        cout << "somthing went wrong!" << endl;
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
    }
}


void StrTokenizer(char *input, char **argv)
{
    char *stringTokenized;
    stringTokenized = strtok(input, " ");
    while (stringTokenized != NULL)
    {
        *argv++ = stringTokenized;
        stringTokenized = strtok(NULL, " ");
    }

    *argv = NULL;
}


