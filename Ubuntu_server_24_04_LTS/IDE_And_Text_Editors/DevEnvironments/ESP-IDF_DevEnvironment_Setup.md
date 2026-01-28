# ESP-IDF Development Environment Setup Guide

This guide provides instructions for setting up an ESP32 development environment using ESP-IDF, Neovim, and the Clangd Language Server Protocol (LSP) on Ubuntu.

## Table of Contents

- [Prerequisites](#prerequisites)
- [ESP-IDF Installation](#esp-idf-installation)
- [Neovim Editor Installation](#neovim-editor-installation)
- [Clangd LSP Installation](#clangd-lsp-installation)
- [Project Creation](#project-creation)
- [Neovim Configuration](#neovim-configuration)
- [Project Setup](#project-setup)
- [ESP32 Flash Configuration](#esp32-flash-configuration)
- [Build, Flash, and Monitor](#build-flash-and-monitor)
- [Development Workflow with tmux](#development-workflow-with-tmux)

---

## Prerequisites

Install Python virtual environment support:

```bash
sudo apt install python3.12-venv
```

---

## ESP-IDF Installation

Install the ESP-IDF v5.3.1 toolchain:

```bash
cd home/ubuntu
git clone -b v5.3.1 --recursive https://github.com/espressif/esp-idf.git
```

Configure the ESP-IDF environment:

```bash
cd esp-idf
./install.sh
source ./export.sh => Set environment paths/variables in current shell (works for each shell session)
. ~/.bashrc        => Load the new bashrc in the current shell
```

**Note:** Do not use `sudo` when running `./install.sh`. The `install.fish` script is available for users of the Fish shell.

---

## Neovim Editor Installation

Neovim is recommended for modern LSP support. The latest stable version (0.9.5) requires a specific version of nvim-lspconfig, which will be configured in the init.vim file.

Add the Neovim stable PPA and install:

```bash
sudo add-apt-repository ppa:neovim-ppa/stable
sudo apt update
sudo apt install neovim
```

Verify the installation:

```bash
nvim --version
```

Expected version: 0.9.5 or newer.

---

## Clangd LSP Installation

Install Clangd for syntax highlighting, error detection, and autocompletion:

```bash
sudo apt install clangd
```

**Note:** Install `clangd` (the LSP server), not `clang` (the compiler).

---

## Project Creation

Create a new ESP32 project:

```bash
idf.py create-project ~/esp-projects/hello_world
cd ~/esp-projects/hello_world
```

By default, `create-project` generates a `.c` file, a top-level `CMakeLists.txt`, and a `main` folder containing another `CMakeLists.txt`.

**Recommended project structure:**

```
esp-projects/
└─ hello_world/            # project root
   ├─ CMakeLists.txt       # top-level, project-level
   └─ main/                # main application component
       ├─ CMakeLists.txt   # registers source files for build
       └─ hello_world.c    # main source code
```

---

## Neovim Configuration

### Install vim-plug

```bash
curl -fLo ~/.local/share/nvim/site/autoload/plug.vim --create-dirs \
    https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim
```

### Create Neovim configuration directory

```bash
mkdir ~/.config/nvim
```

### Create initial init.vim

```bash
nano ~/.config/nvim/init.vim
```

Add the following minimal configuration:

```vim
" =========================
" Plugin management
" =========================
call plug#begin('~/.local/share/nvim/plugged')
" LSP support - pinned to version compatible with Neovim 0.9.5
Plug 'neovim/nvim-lspconfig', { 'tag': 'v0.1.8' }
call plug#end()

" =========================
" LSP configuration (Lua)
" =========================
lua << EOF
require('lspconfig').clangd.setup{
    cmd = { "clangd" }
}
EOF
```

### Install the plugin

Launch Neovim and install the plugin:

```bash
nvim
```

In Neovim, execute:

```vim
:PlugInstall
```

**Note:** An error may appear on the first launch before the plugin is installed. This is expected.

Quit and restart Neovim:

```vim
:q
:exit
```

```bash
nvim
```

Verify Clangd attachment (optional):

```vim
:LspInfo
```

### Add autocompletion support

Edit the init.vim file to add nvim-cmp:

```bash
nano ~/.config/nvim/init.vim
```

Replace the contents with:

```vim
" =========================
" Plugin management
" =========================
call plug#begin('~/.local/share/nvim/plugged')
" LSP support - pinned to version compatible with Neovim 0.9.5
Plug 'neovim/nvim-lspconfig', { 'tag': 'v0.1.8' }

" Completion plugins
Plug 'hrsh7th/nvim-cmp'
Plug 'hrsh7th/cmp-nvim-lsp'
call plug#end()

" =========================
" LSP configuration (Lua)
" =========================
lua << EOF
-- Setup completion
local cmp = require('cmp')
cmp.setup({
  mapping = cmp.mapping.preset.insert({
    ['<C-Space>'] = cmp.mapping.complete(),
    ['<CR>'] = cmp.mapping.confirm({ select = true }),
  }),
  sources = {
    { name = 'nvim_lsp' },
  }
})

-- Setup clangd with completion capabilities
local capabilities = require('cmp_nvim_lsp').default_capabilities()
require('lspconfig').clangd.setup{
    cmd = { "clangd" },
    capabilities = capabilities
}
EOF
```

Install the new plugins and restart Neovim:

```bash
nvim
```

```vim
:PlugInstall
:q
:exit
```

```bash
nvim
```

---

## Project Setup

### Configure top-level CMakeLists.txt

Edit `hello_world/CMakeLists.txt`:

```cmake
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.22)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(hello_world)
```

### Configure main/CMakeLists.txt

```bash
cat > main/CMakeLists.txt << 'EOF'
idf_component_register(SRCS "hello_world.c"
                    INCLUDE_DIRS ".")
EOF
```

### Create source code

```bash
cat > main/hello_world.c << 'EOF'
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
    int count = 0;

    while(1) {
        printf("Hello World! Count: %d\n", count++);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
EOF
```

**Note:** For optimal experience with Clangd autocomplete and LSP features, type the code manually in Neovim instead of using the `cat` command.

### Build the project

Build the project to generate `compile_commands.json`, which Clangd requires for intellisense, autocompletion, and error checking:

```bash
cd ~/esp-projects/hello_world
idf.py build
```

---

## ESP32 Flash Configuration

### Configure serial port permissions

Add the current user to the `dialout` group:

```bash
sudo usermod -a -G dialout $USER
```

Reboot to apply the group change:

```bash
sudo reboot
```

After rebooting, verify group membership:

```bash
groups
```

The output should include `dialout`.

### Identify the ESP32 serial port

```bash
ls -l /dev/ttyUSB*
```

If a device such as `/dev/ttyUSB0` appears, the ESP32 is connected and ready for flashing.

---

## Build, Flash, and Monitor

### Build the project

```bash
cd ~/esp-projects/hello_world
idf.py build
```

### Flash the ESP32

```bash
idf.py flash
```

### Monitor serial output

```bash
idf.py monitor
```

To exit the monitor, press `Ctrl + ]`.

---

## Development Workflow with tmux

Using tmux allows for an IDE-like experience with split panes for editing, building, and monitoring simultaneously.

### tmux commands

- **Split panes horizontally:** `Ctrl+b` then `%`
- **Split panes vertically:** `Ctrl+b` then `"`
- **Switch between panes:** `Ctrl+b` then arrow keys
- **Detach session:** `Ctrl+b` then `d`
- **Reattach session:** `tmux attach`

### Workflow example

Keep Neovim open in one pane for editing while building and flashing in another pane. This allows for rapid development iteration without switching contexts.

---

## Summary

This guide covered the complete setup of an ESP32 development environment with:

- ESP-IDF v5.3.1 toolchain
- Neovim with Clangd LSP and autocompletion
- Project configuration and build process
- Serial port setup and flashing
- Development workflow using tmux

The configured environment provides syntax highlighting, autocompletion, and error checking for efficient ESP32 firmware development.
