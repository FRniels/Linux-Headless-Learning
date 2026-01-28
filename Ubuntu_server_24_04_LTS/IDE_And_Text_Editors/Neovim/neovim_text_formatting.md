# Neovim text formatting

## Make a tab behave like 4 spaces

A literal tab character (\t) is displayed as 4 spaces.

```
set tabstop=4
```

Indentation operations (>>, <<, auto-indent) use 4 spaces.

```
set shiftwidth=4
```

Pressing <Tab> inserts spaces instead of a tab character.

```
set expandtab
```

Improves indentation for C-like languages.

```
set smartindent
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
set tabstop=4
set shiftwidth=4
set expandtab
set smartindent
```
