# Neovim: basic text operations

## Copy text (vim register, not system clipboard copying)

Enter normal mode

```
Press Esc
```

Enter visual mode: Character-wise selection

```
Press v
```

Or, enter visual mode: Line-wise selection

```
Press V (shift + v)
```

Select text

```
Arrow keys or other specific key commands
```

Yank (copy)

```
Press y
```

Or, copy current line

```
yy
```

Or, copy N lines

```
Nyy (N = amount of lines)
```

## Paste text

Enter normal mode

```
Press Esc
```

Set cursor

```
Arrow keys or other specific key commands
```

Paste after the cursor

```
Press p
```

Paste before the cursor

```
Press P (shift + p)
```

This works for:

- yanked text (y)
- deleted text (d)
- changed text (c)

### Visual-mode replace (paste over existing text)

Paste over a selection without overwriting the current yank:

Select text in Visual mode

```
See visual mode copy select
```

Paste text

```
Press p
```

This replaces the selection with the previously yanked text and keeps the yank intact.

## Delete

Enter normal mode

```
Press Esc
```

Enter visual mode: Character-wise selection

```
Press v
```

Or, enter visual mode: Line-wise selection (most common for code)

```
Press V (shift + v)
```

Select text

```
Arrow keys or other specific key commands
```

Delete (cut) the selection

```
Press d
```

## Undo / redo

Enter normal mode

```
Press Esc
```

Undo

```
Press u
```

Redo

```
Press ctrl + r
```

# Move cursor position on current line

Enter normal mode

```
Press Esc
```

Move to start (col 0) of the current line

```
Press 0
```

Move to the first 'real' character of the current line

```
Press ^
```

Move to the last character of the current line

```
Press $
```

## Move cursor to different lines => TO DO

## Find next occurence of a word

Enter normal mode

```
Press Esc
```

Set cursor before first character of the word

```
Arrow keys or other specific key commands
```

Step through occurences

```
Press * (repeat to step through)

```

## Note

These are just the most basic and necessary commands for normal text operations. See Neovim manual for all the possible commands and there options.
