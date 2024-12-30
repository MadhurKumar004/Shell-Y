# Simple-shell

A simple shell created using the C language. This shell supports basic command execution, piping, redirection, and job control.

## Features

- Execute basic commands
- Support for piping (`|`)
- Input and output redirection (`<`, `>`)
- Job control (`jobs`, `fg`, `bg`)
- Built-in commands (`cd`, `exit`)

## How It Works

The shell follows a typical architecture:
1. Reads user input
2. Parses commands and arguments
3. Handles special operations (piping, redirection)
4. Executes commands using child processes
5. Manages job control

## Usage

### Setup

1. Clone and build:
    ```sh
    git clone <repository-url>
    cd <repository-directory>
    make
    ```

    **Note:** If compilation fails, use the pre-compiled executable:
    ```sh
    chmod +x ./bin/shell
    ```

2. Run the shell:
    ```sh
    ./bin/shell
    ```

### Example Commands

```sh
shell> ls -l                                  # List files
shell> cat file.txt | grep "word"            # Pipe commands
shell> echo "Hello" > output.txt             # Redirection
shell> sleep 100 &                           # Background process
shell> jobs                                  # List jobs
shell> fg 1                                  # Bring job to foreground
shell> exit                                  # Exit shell
```

## License

This project is licensed under the MIT License. See the [`LICENSE`](LICENSE) file for details.