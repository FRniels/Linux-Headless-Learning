# Operating System and Kernel Information Commands

## Overview

This document provides essential commands for retrieving operating system and kernel information on Linux systems.

## Operating System Information

### Display OS Release Information

```bash
cat /etc/os-release
```

Displays distribution-specific information including name, version, and ID.

### Show LSB Release Details

```bash
lsb_release -a
```

Provides Linux Standard Base information about the distribution. The `-a` flag displays all available information.

### Display Hostname

```bash
hostnamectl
```

Shows system hostname and related operating system information including kernel version, architecture, and virtualization details.

## Kernel Information

### Display Kernel Version

```bash
uname -r
```

Shows the current kernel release version.

### Show All System Information

```bash
uname -a
```

Displays comprehensive system information including kernel name, network node hostname, kernel release, kernel version, machine hardware name, processor type, hardware platform, and operating system.

### Alternative Kernel Version Check

```bash
cat /proc/version
```

Provides detailed kernel version information including compiler version used to build the kernel.

## System Architecture

### Display System Architecture

```bash
arch
```

Shows the machine hardware architecture (e.g., aarch64 for Raspberry Pi 4B running 64-bit).

Alternatively:

```bash
uname -m
```

Produces identical output to the `arch` command.

## Boot and Uptime Information

### Display System Uptime

```bash
uptime
```

Shows how long the system has been running, number of users, and load averages.

### View Boot Messages

```bash
dmesg | less
```

Displays kernel ring buffer messages from system boot, useful for troubleshooting hardware and driver issues.

## Additional Resources

All commands listed are standard utilities available on Ubuntu Server 24.04 LTS and most Linux distributions. For detailed information about any command, consult the manual pages using `man <command>`.
