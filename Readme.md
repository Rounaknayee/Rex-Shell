# Rex's Shell

Rex's Shell is a simple, custom command-line interpreter (shell) written in C. It supports basic shell functionalities such as executing commands, navigating directories, and handling files. Additionally, it includes support for piping commands.

## Features

- Execute common shell commands like `ls`, `cd`, `pwd`, `mkdir`, `rmdir`, `cat`, `touch`, and `grep`.
- Built-in `help` command to list available commands.
- Support for command piping using the `|` operator.
- Custom command parsing and execution.

## Getting Started

### Prerequisites

To build and run Rex's Shell, you need to have a C compiler installed on your system. The shell is developed and tested on a UNIX-like environment.

### Installation

1. Clone the repository:

```git clone https://github.com/your-username/rexs-shell.git```

2. Compile the shell:

```gcc -o shell shell.c```

3. Run the shell:

```./shell```

### Usage

After starting the shell, you can execute commands like you would in any standard shell. For example:

Rex's shell> ls
Rex's shell> cd /path/to/directory
Rex's shell> pwd


To use the piping feature:

Rex's shell> ls | grep filename