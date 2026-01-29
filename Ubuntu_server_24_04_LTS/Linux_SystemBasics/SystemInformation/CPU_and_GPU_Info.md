# CPU and GPU Information Commands

## Overview

This document provides essential commands for retrieving processor and graphics processing unit information on Linux systems.

## CPU Information

### Display Detailed CPU Information

```bash
lscpu
```

Provides comprehensive CPU architecture information including number of cores, threads, sockets, CPU family, model name, and supported instruction sets.

### View CPU Specifications from Proc Filesystem

```bash
cat /proc/cpuinfo
```

Displays detailed information for each processor core including vendor ID, model name, CPU MHz, cache size, and flags.

### Monitor CPU Frequency

```bash
cat /sys/devices/system/cpu/cpu*/cpufreq/scaling_cur_freq
```

Shows current frequency for each CPU core in kHz. Note that cpufreq may not be available on all systems.

### Check CPU Temperature

```bash
cat /sys/class/thermal/thermal_zone*/temp
```

Displays temperature readings in millidegrees Celsius. Divide output by 1000 to obtain degrees Celsius.

Alternatively, if installed:

```bash
vcgencmd measure_temp
```

Raspberry Pi-specific command showing GPU temperature, which typically correlates with CPU temperature on the SoC.

## GPU Information

### Display GPU Information via System Utilities

For Raspberry Pi systems with VideoCore GPU:

```bash
vcgencmd get_mem gpu
```

Shows GPU memory allocation.

```bash
vcgencmd measure_clock core
```

Displays GPU core clock frequency.

### View Graphics Controller Information

```bash
lspci | grep -i vga
```

Lists VGA-compatible controllers. Note that Raspberry Pi 4B uses a SoC design where GPU information may not appear via PCI enumeration.

### Check GPU Memory Split

```bash
vcgencmd get_mem arm && vcgencmd get_mem gpu
```

Displays memory allocation between ARM CPU and GPU on Raspberry Pi systems.

## Performance Monitoring

### Real-time CPU Usage

```bash
top
```

Displays real-time process information and CPU utilization. Press `q` to exit.

```bash
htop
```

Enhanced interactive process viewer with improved visualization. Requires installation via `apt install htop`.

### CPU Usage Statistics

```bash
mpstat
```

Reports processor statistics. Part of the `sysstat` package, which may require installation via `apt install sysstat`.

## Installation Notes

Some commands require additional packages:

- `vcgencmd`: Included in Raspberry Pi OS by default; may require `libraspberrypi-bin` package on Ubuntu
- `htop`: Install with `sudo apt install htop`
- `mpstat`: Install with `sudo apt install sysstat`

## Additional Resources

For detailed information about any command, consult the manual pages using `man <command>`. Raspberry Pi-specific documentation is available at the official Raspberry Pi documentation website.
