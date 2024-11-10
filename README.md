# Operating System Shell Implementation

This project is a custom shell implemented in C, designed as a minimal command-line interface for executing commands, handling pipelines, and managing background processes. This shell supports several built-in commands and a subset of common shell functionalities, such as output redirection, multi-pipeline execution, and process management. 

## Table of Contents
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Built-in Commands](#built-in-commands)
  - `help`
  - `cd`
  - `pwd`
  - `echo`
  - `record`
  - `replay`
  - `cat`
- [Background and Pipeline Support](#background-and-pipeline-support)
- [License](#license)

## Features

- **Built-in Commands**: Includes `help`, `cd`, `pwd`, `echo`, `record`, `replay`, and `cat`.
- **Multi-Pipeline Execution**: Supports commands with multiple pipelines.
- **Background Execution**: Allows commands to be executed in the background and displays the process ID (PID) of the last command in a pipeline.
- **Output Redirection**: Redirects output to specified files.
- **Command Recording and Playback**: Records and replays command history.

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/Potassium-chromate/Operating_system_Shell_implement.git
   ```
2. Navigate to the project directory:
   ```bash
   cd Operating_system_Shell_implement
   ```
3. Compile the shell program:
   ```bash
   gcc -o shell shell.c
   ```

## Usage
Run the shell by executing:
```bash
./shell
```
The shell prompt will display, allowing you to enter commands interactively. Supported built-in commands are listed below, and you can also run most standard Linux commands.
To exit the shell, use the `exit` command.

## Built-in Commands
- `help`: Displays a list of all available commands with a brief description.
  ```bash
  help
  ```
- `cd`: Changes the current working directory.
  ```bash
  cd [directory]
  ```
- `pwd`: Displays the current working directory.
  ``` bash
  pwd
  ```
- `echo`: Prints a message to the console.
  ```bash
  echo [message]
  ```
- `record`: Records a command to the command history.
  ```bash
  record [command]
  ```
- `replay`: Replays a previously recorded command from the command history. Takes the command index as an argument.
  ```bash
  replay [index]
  ```
- `cat`: Displays the content of a file.
  ```bash
  cat [filename]
  ```
## Background and Pipeline Support
### Background Execution
Commands can be executed in the background by appending & at the end. The shell will display the PID of the right-most command in the pipeline when executed in the background.

Example:
```bash
ls -l &   # Runs `ls -l` in the background and displays its PID.
```
### Multi-Pipeline Commands
Multiple commands can be piped together, with each command's output feeding into the next.

Example:

```bash
ls | grep '.c' | wc -l
```
## License
This project is licensed under the MIT License.
