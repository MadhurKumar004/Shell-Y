# Simple-shell

A simple shell created using the C language. This shell supports basic command execution, piping, redirection, and job control.

## Features

- Execute basic commands
- Support for piping (`|`)
- Input and output redirection (`<`, `>`)
- Job control (`jobs`, `fg`, `bg`)
- Built-in commands (`cd`, `exit`)

## How It Works

### Overview

The simple shell is implemented in C and follows a typical shell architecture. It includes a command-line interface that reads user input, parses the input into commands and arguments, and then executes the commands. The shell supports various features such as piping, redirection, and job control.

### Components

1. **Main Loop**: The shell runs in an infinite loop, displaying a prompt and waiting for user input. The loop continues until the user exits the shell.
    ```c
    while (1) {
        // Display prompt
        // Read input
        // Parse input
        // Execute command
    }
    ```

2. **Input Handling**: The shell reads input from the user using functions like `getline()` or `read()`. The input is then tokenized into commands and arguments.
    ```c
    char *input = NULL;
    size_t len = 0;
    getline(&input, &len, stdin);
    ```

3. **Parsing**: The input is parsed to identify commands, arguments, and special characters for piping and redirection. This is typically done using functions like `strtok()`.
    ```c
    char *token = strtok(input, " ");
    while (token != NULL) {
        // Process token
        token = strtok(NULL, " ");
    }
    ```

4. **Command Execution**: The shell uses `fork()` to create a new process for each command. The child process executes the command using `execvp()`, while the parent process waits for the child to complete.
    ```c
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        execvp(args[0], args);
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Parent process
        wait(NULL);
    }
    ```

5. **Piping**: The shell supports piping by creating a pipe using `pipe()`, and redirecting the output of one command to the input of another.
    ```c
    int pipefd[2];
    pipe(pipefd);
    if (fork() == 0) {
        // Child process 1
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        execvp(cmd1[0], cmd1);
    } else if (fork() == 0) {
        // Child process 2
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[1]);
        execvp(cmd2[0], cmd2);
    }
    ```

6. **Redirection**: The shell handles input and output redirection using `dup2()` to redirect file descriptors.
    ```c
    int fd = open("file.txt", O_WRONLY | O_CREAT, 0644);
    dup2(fd, STDOUT_FILENO);
    execvp(args[0], args);
    ```

7. **Job Control**: The shell supports job control commands like `jobs`, `fg`, and `bg` to manage background and foreground processes.
    ```c
    // List jobs
    // Bring job to foreground
    // Move job to background
    ```

## Usage

### Download and Build

1. Clone the repository:
    ```sh
    git clone <repository-url>
    cd <repository-directory>
    ```
    Replace `<repository-url>` with the URL of your repository and `<repository-directory>` with the name of the directory created by cloning the repository.

2. Build the project using `make`:
    ```sh
    make
    ```
    This command will compile the source code and generate the executable file in the [`bin`](bin ) directory.

### Running the Shell

1. Start the shell:
    ```sh
    ./bin/shell
    ```
    This command will start the shell, and you will see a prompt where you can enter commands.

2. You can now enter commands in the shell prompt:
    ```sh
    shell> ls -l
    ```
    This command will list the files in the current directory in long format.

    ```sh
    shell> cat file.txt | grep "search_term"
    ```
    This command will display the contents of `file.txt` and filter the lines containing `search_term`.

    ```sh
    shell> echo "Hello, World!" > output.txt
    ```
    This command will write "Hello, World!" to `output.txt`.

    ```sh
    shell> jobs
    ```
    This command will list all background jobs.

    ```sh
    shell> fg 1
    ```
    This command will bring the job with ID 1 to the foreground.

    ```sh
    shell> bg 2
    ```
    This command will move the job with ID 2 to the background.

    ```sh
    shell> exit
    ```
    This command will exit the shell.

## License

This project is licensed under the MIT License. See the [`LICENSE`](LICENSE ) file for details.