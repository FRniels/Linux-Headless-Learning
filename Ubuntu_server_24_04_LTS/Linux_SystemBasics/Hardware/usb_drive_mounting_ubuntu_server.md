# USB Drive mounting on Ubuntu Server

This document describes the correct and safe procedure for identifying, mounting, accessing, writing to, and removing a USB storage device on an Ubuntu Server system. It also explains why unmounting is required to prevent data loss.

---

## Overview

On Linux systems, removable storage devices such as USB drives must be mounted before their contents can be accessed. Mounting instructs the operating system to interpret the raw device data as a filesystem. Removing a USB drive without properly unmounting it can result in data loss or filesystem corruption.

---

## Why Unmounting Is Required

Even when no visible read or write operations are occurring, Linux may still hold pending data in memory due to write caching and delayed metadata updates. Removing a USB drive without unmounting it may cause:

- Loss of recently written files
- Incomplete or corrupted files
- Filesystem metadata corruption

Unmounting a device ensures that all cached data is flushed to the storage medium and that the filesystem is cleanly closed.

---

## USB Drive Workflow

### 1. Identify the USB Device

After inserting the USB drive, list all block devices:

```bash
lsblk
```

Example output:

```text
NAME   MAJ:MIN RM  SIZE RO TYPE MOUNTPOINTS
sda      8:0    1  32G  0 disk
└─sda1   8:1    1  32G  0 part
```

- `/dev/sda` refers to the entire USB device
- `/dev/sda1` refers to a partition containing a filesystem

In most cases, the partition (e.g., `sda1`) is what should be mounted.

---

### 2. Create a Mount Point

Create a directory that will be used as the access point for the USB drive:

```bash
sudo mkdir -p /mnt/usb
```

---

### 3. Mount the USB Drive

#### a) For Linux-native filesystems (ext4, ext3, ext2)

```bash
sudo mount /dev/sda1 /mnt/usb
```

#### b) For exFAT, NTFS, or FAT-based USB drives (to allow writing as the current user)

```bash
sudo umount /mnt/usb
sudo mount -t exfat -o uid=$(id -u),gid=$(id -g) /dev/sda /mnt/usb
```

This ensures the current user has write permissions. For NTFS, replace `-t exfat` with `-t ntfs3` if needed.

A successful mount produces no output.

---

### 4. Access and Write Files

List the contents of the USB drive:

```bash
ls /mnt/usb
```

Read files as needed:

```bash
cat /mnt/usb/example.txt
less /mnt/usb/largefile.log
```

Create or edit files as the current user:

```bash
nano /mnt/usb/first_usb_drive_mount.txt
```

---

### 5. Safely Unmount the USB Drive

Before physically removing the USB device, unmount it:

```bash
sudo umount /mnt/usb
```

This step guarantees that all pending writes are committed and the filesystem is left in a consistent state.

---

## Additional Notes

- Removing a USB drive without unmounting it can cause data loss, even if no activity is visible
- Journaling filesystems (e.g., ext4) improve resilience but do not eliminate risk
- Filesystem type can be inspected using:

```bash
lsblk -f
```

- Some filesystems (e.g., exFAT, NTFS) require additional packages or mount options to allow writing

---

## Summary

Always follow this sequence when working with removable storage:

1. Identify the device
2. Mount the filesystem (with proper ownership/permissions)
3. Access and write to files as needed
4. Unmount the filesystem
5. Remove the device

This process ensures data integrity and prevents filesystem corruption.
