# Multiple Shells In One SSH Session tmux.

This document provides a formal reference for using tmux, a terminal multiplexer, on Ubuntu Server systems. It includes instructions for splitting panes, switching between them, detaching, reattaching, and closing sessions.

---

## 1. Starting Tmux

To start a new tmux session:

```bash
tmux
```

This opens a new session with a single terminal window.

---

## 2. Prefix Key

Tmux uses a prefix key to distinguish commands from regular keystrokes. The default prefix is **Ctrl+b**.

- Press **Ctrl+b** together, then release both keys.
- The next key pressed is interpreted as a tmux command.

---

## 3. Splitting Panes

### Horizontal Split (side-by-side panes)

Command: `%`

**AZERTY keyboards:** press **Shift+5** to produce `%`.

Sequence:

```text
Ctrl+b then %
```

### Vertical Split (top-bottom panes)

Command: `"`

Sequence:

```text
Ctrl+b then "
```

---

## 4. Switching Between Panes

Use the arrow keys after the prefix to move between panes:

```text
Ctrl+b then Left/Right/Up/Down arrow
```

---

## 5. Detaching a Session

Detaching a session allows tmux to continue running commands in the background while returning to the regular shell.

Command:

```text
Ctrl+b then d
```

- All panes and processes continue to run in the detached session.
- The terminal is returned to the normal shell prompt.

---

## 6. Reattaching a Session

To reconnect to a detached tmux session:

```bash
tmux attach
```

- Reattaches the most recently detached session.
- If multiple sessions exist, use:

```bash
tmux attach -t <session_name>
```

- List all sessions:

```bash
tmux ls
```

---

## 7. Closing a Tmux Session

To completely close a tmux session and all its panes:

1. Exit all processes in each pane (e.g., type `exit` or press `Ctrl+d`).
2. Once all panes are closed, the session automatically terminates.

Alternatively, from a detached session, you can kill it directly:

```bash
tmux kill-session -t <session_name>
```

After closing the session, you return to a normal SSH terminal prompt.

---

## 8. Practical Workflow Example

1. Start tmux:

```bash
tmux
```

2. Split pane horizontally (side-by-side):

```text
Ctrl+b then %
```

3. Split the left pane vertically (top-bottom):

```text
Ctrl+b then "
```

4. Switch between panes using arrow keys:

```text
Ctrl+b then arrow keys
```

5. Detach the session when leaving:

```text
Ctrl+b then d
```

6. Reattach later:

```bash
tmux attach
```

7. Close the session when done:

- Exit all panes, or
- Kill current session: `tmux kill-session`
- Kill the session: `tmux kill-session -t <session_name>`

---

## 9. Notes

- Tmux sessions persist even if the SSH connection is lost.
- All running processes inside tmux continue in detached sessions.
- Useful for monitoring, editing, and running multiple commands side by side in a single terminal window.
