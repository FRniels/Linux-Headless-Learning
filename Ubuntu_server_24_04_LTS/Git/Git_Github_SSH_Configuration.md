# Git, GitHub, and SSH Configuration Guide

This guide covers the configuration of Git, GitHub authentication via SSH keys, and ssh-agent setup for secure repository management on headless Linux systems.

## Table of Contents

- [Initial Git Configuration](#initial-git-configuration)
- [SSH Key Generation](#ssh-key-generation)
- [Adding SSH Key to GitHub](#adding-ssh-key-to-github)
- [SSH Agent Configuration](#ssh-agent-configuration)
- [Repository Setup](#repository-setup)
- [Common Git Workflows](#common-git-workflows)
- [Troubleshooting](#troubleshooting)

---

## Initial Git Configuration

Configure Git with user identity information. This is required before making commits.

```bash
git config --global user.name "Your Name"
git config --global user.email "your.email@example.com"
```

Verify the configuration:

```bash
git config --global --list
```

---

## SSH Key Generation

SSH keys provide secure authentication to GitHub without requiring password entry for each operation.

### Generate SSH Key Pair

```bash
ssh-keygen -t ed25519 -C "your.email@example.com"
```

When prompted:

- **Enter file location**: Press Enter to accept default (`~/.ssh/id_ed25519`)
- **Enter passphrase**: Provide a strong passphrase for security
- **Confirm passphrase**: Re-enter the passphrase

The command generates two files:

- `~/.ssh/id_ed25519` - Private key (keep secure, never share)
- `~/.ssh/id_ed25519.pub` - Public key (safe to share)

### Display Public Key

```bash
cat ~/.ssh/id_ed25519.pub
```

Copy the entire output for use in the next section.

---

## Adding SSH Key to GitHub

### Via GitHub Web Interface

1. Navigate to GitHub.com and sign in
2. Click profile icon → **Settings**
3. Select **SSH and GPG keys** from left sidebar
4. Click **New SSH key**
5. Provide a descriptive title (e.g., "Raspberry Pi 4B")
6. Paste the public key content into the Key field
7. Click **Add SSH key**

### Verify SSH Connection

```bash
ssh -T git@github.com
```

Expected output:

```
Hi username! You've successfully authenticated, but GitHub does not provide shell access.
```

---

## SSH Agent Configuration

The ssh-agent caches SSH key passphrases, eliminating the need to re-enter the passphrase for each Git operation within a session.

### Manual SSH Agent Start

Start the agent and add the SSH key:

```bash
eval "$(ssh-agent -s)"
ssh-add ~/.ssh/id_ed25519
```

Enter the passphrase when prompted. The key remains cached until the agent process terminates.

### Automatic SSH Agent Initialization

Add the following to `~/.bashrc` to automatically start ssh-agent on login:

```bash
nano ~/.bashrc
```

Append the following lines:

```bash
# Auto-start ssh-agent
if ! pgrep -u "$USER" ssh-agent > /dev/null; then
    ssh-agent > ~/.ssh-agent-thing
fi
if [[ ! "$SSH_AUTH_SOCK" ]]; then
    eval "$(<~/.ssh-agent-thing)" > /dev/null
fi
```

Apply the changes:

```bash
source ~/.bashrc
```

Add the SSH key (required once per system boot):

```bash
ssh-add ~/.ssh/id_ed25519
```

### Persistent SSH Agent via systemd (Optional)

For ssh-agent to persist across multiple login sessions without requiring reboot:

Create systemd user service file:

```bash
mkdir -p ~/.config/systemd/user
nano ~/.config/systemd/user/ssh-agent.service
```

Add the following content:

```ini
[Unit]
Description=SSH key agent

[Service]
Type=simple
Environment=SSH_AUTH_SOCK=%t/ssh-agent.socket
ExecStart=/usr/bin/ssh-agent -D -a $SSH_AUTH_SOCK

[Install]
WantedBy=default.target
```

Enable and start the service:

```bash
systemctl --user enable ssh-agent
systemctl --user start ssh-agent
```

Add to `~/.bashrc`:

```bash
export SSH_AUTH_SOCK="$XDG_RUNTIME_DIR/ssh-agent.socket"
```

Apply changes and add key:

```bash
source ~/.bashrc
ssh-add ~/.ssh/id_ed25519
```

**Note**: The ssh-add command must be executed once per system boot for security purposes.

---

## Repository Setup

### Initialize Local Repository

```bash
cd ~/project-directory
git init
```

### Create Repository on GitHub

1. Navigate to GitHub.com
2. Click the **+** icon → **New repository**
3. Enter repository name
4. Select visibility (Public/Private)
5. Do NOT initialize with README if local repository already exists
6. Click **Create repository**

### Link Local Repository to GitHub

```bash
# Add GitHub remote using SSH URL
git remote add origin git@github.com:USERNAME/REPOSITORY.git

# Rename default branch to main (if needed)
git branch -M main

# Push local commits to GitHub
git push -u origin main
```

Replace `USERNAME` and `REPOSITORY` with appropriate values.

### Initial Commit

```bash
# Stage all files
git add .

# Create initial commit
git commit -m "Initial commit"

# Push to GitHub
git push -u origin main
```

---

## Common Git Workflows

### Pull Remote Changes

```bash
# Fetch and merge remote changes
git pull origin main
```

### Add and Commit Changes

```bash
# Stage specific file
git add filename.c

# Stage all modified files
git add .

# Commit with message
git commit -m "Descriptive commit message"

# Push to remote
git push
```

### Check Repository Status

```bash
# View modified files and staging status
git status

# View commit history
git log

# View compact commit history
git log --oneline
```

### Recover from Mistakes

```bash
# Discard local changes to a file
git checkout -- filename.c

# Undo last commit (keep changes staged)
git reset --soft HEAD~1

# Undo last commit (discard changes)
git reset --hard HEAD~1
```

**Warning**: `git reset --hard` permanently deletes uncommitted changes.

---

## Troubleshooting

### Unrelated Histories Error

When attempting to push to a repository that contains commits not present locally (e.g., README created on GitHub):

```bash
git pull origin main --allow-unrelated-histories
```

After resolving any merge conflicts, push the changes:

```bash
git push -u origin main
```

### Permission Denied (publickey)

Verify SSH key is added to ssh-agent:

```bash
ssh-add -l
```

If the key is not listed:

```bash
ssh-add ~/.ssh/id_ed25519
```

Test GitHub SSH connection:

```bash
ssh -T git@github.com
```

### Line Ending Warnings (CRLF/LF)

Configure Git to handle line endings automatically:

```bash
# On Linux
git config --global core.autocrlf input

# On Windows
git config --global core.autocrlf true
```

### Agent Connection Refused

If ssh-agent is not running:

```bash
eval "$(ssh-agent -s)"
ssh-add ~/.ssh/id_ed25519
```

---

## Security Best Practices

1. **Protect Private Key**: Never share or commit `~/.ssh/id_ed25519`
2. **Use Strong Passphrase**: Protect SSH key with a complex passphrase
3. **Regular Key Rotation**: Regenerate SSH keys periodically
4. **Revoke Compromised Keys**: Immediately remove compromised keys from GitHub
5. **Limit Key Scope**: Use deploy keys or fine-grained tokens for specific repositories when appropriate

---

## Summary

This guide covered:

- Git identity configuration
- SSH key generation with ed25519 encryption
- GitHub SSH key authentication setup
- ssh-agent configuration for passphrase caching
- Repository initialization and remote linking
- Common Git operations and troubleshooting

For headless Linux systems, SSH key authentication with ssh-agent provides secure, efficient access to GitHub repositories without requiring password entry for each operation.

## Source

Claude.ai
