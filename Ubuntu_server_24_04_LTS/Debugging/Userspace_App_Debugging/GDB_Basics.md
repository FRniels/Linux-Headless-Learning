# GDB basics

## What is GDB

Debug C user programs.

## Why use GDB

- Setting breakpoints
- Stepping through code
- Inspecting variables
- Understanding call stacks
- Signal handling

## GDB commands

Install GDB

```
sudo apt install gdb
```

Build C application with debug symbols

```
gcc -g -O0 program.c -o program
```

In which:

- -g includes debug info
- -O0 disables optimizations (makes stepping predictable)

Open C application with GDB

```
gdb app_name
```

Exit GDB

```
exit
```

Help menu

```
help
```

Set breakpoint at line

```
break <line number>
```

Run debug session

```
run
```

Stop debug session

```
kill
```

Step to next line of execution

```
next or n
```

Print variable value

```
print <variable name>
```
