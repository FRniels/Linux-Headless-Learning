# KGDB vs. KDB: Official Terminology

The official name of the Linux kernel debugger is **KGDB**.

## Definitions

- **KGDB (Kernel GNU Debugger)**  
  The official in-kernel debugging infrastructure for the Linux kernel. It enables debugging the kernel using a remote GNU Debugger (GDB) session.

- **KDB (Kernel Debugger)**  
  An optional interactive debugging shell that operates on top of KGDB. It provides a local command-line interface for inspecting and controlling kernel execution without requiring a remote GDB connection.

## Terminology Notes

- **KGDB** is the correct and official name for the Linux kernel debugger.
- **KDB** is a frontend that depends on KGDB; it is not a separate or competing debugger.
- The phrase **“kdb debugger”** is informal and not an official term.
- Referring to **“KGDB vs. KDB”** is technically inaccurate, as KDB is built on top of KGDB rather than an alternative to it.

## Recommended Usage

A precise and correct description is:

> Linux kernel debugging using **KGDB**, with the **KDB** frontend enabled.
