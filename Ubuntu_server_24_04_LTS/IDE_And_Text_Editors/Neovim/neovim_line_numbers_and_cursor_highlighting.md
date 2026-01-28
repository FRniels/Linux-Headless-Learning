# Neovim Built-in Line Numbering and Cursor Highlighting

This document describes how to enable line numbering and active line highlighting in Neovim using only built-in functionality. No external plugins are required.

## Line Numbering

Neovim supports both absolute and relative line numbering.

### Absolute Line Numbers

Displays the absolute line number for each line.

```
:set number
```

### Relative Line Numbers

Displays line numbers relative to the current cursor position. This is useful for navigation commands.

```
:set relativenumber
```

### Combined Absolute and Relative Line Numbers

This configuration shows the absolute number on the current line and relative numbers on all other lines.

```
:set number relativenumber
```

## Active Line Highlighting

Neovim can highlight the line where the cursor is currently located.

```
:set cursorline
```

### Highlight Only the Line Number

For a more subtle visual effect, only the line number of the active line can be highlighted.

```
:set cursorlineopt=number
```

## Making Settings Persistent

To apply these settings automatically on startup, they must be added to the Neovim configuration file.

### Using init.vim

File location:

```
~/.config/nvim/init.vim
```

Configuration:

```
set number
set relativenumber
set cursorline
set cursorlineopt=number
```

### Using init.lua

File location:

```
~/.config/nvim/init.lua
```

Configuration:

```
vim.opt.number = true
vim.opt.relativenumber = true
vim.opt.cursorline = true
vim.opt.cursorlineopt = "number"
```
