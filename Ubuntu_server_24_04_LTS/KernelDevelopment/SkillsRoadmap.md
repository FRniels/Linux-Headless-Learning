# Linux kernel development foundational skills

## 1. Linux System Programming (User Space)

Before touching kernel space, get comfortable with Linux user space programming:

File I/O: open(), read(), write(), close(), ioctl()
Process management: fork(), exec(), wait()
Inter-process communication (IPC): pipes, shared memory, signals
Multithreading: pthreads
Understanding /dev, /sys, /proc filesystems

Why: You need to understand how user space interacts with kernel space before writing kernel code.
Practice project: Write a user-space program that reads from /sys/class/gpio/ to control GPIO (the existing sysfs GPIO interface).

## 2. Character Device Drivers (Simplest Kernel Module)

Start with the absolute basics of kernel modules:

Hello World kernel module: Load/unload modules with insmod/rmmod
printk() vs printf(): Kernel logging
Understanding /dev device files
Character device registration: create_char_dev(), file_operations structure
Basic read/write operations from user space to kernel space

Why: Character devices are the simplest driver type and teach you kernel module basics.
Practice project: Write a simple character device that echoes back what you write to it (/dev/mydevice).

## 3. Understanding Linux Device Model

Learn how Linux organizes devices:

Device tree basics (especially important for Raspberry Pi)
Platform drivers vs character drivers
Probing and device matching
sysfs and device attributes

Why: Modern Linux uses device trees to describe hardware, especially on ARM platforms like Raspberry Pi.
Practice project: Read your Raspberry Pi's device tree to understand how hardware is described.

## 4. Memory Management in Kernel Space

Kernel memory is different from user space:

kmalloc() vs vmalloc()
Physical vs virtual addresses
Memory mapping (mmap)
Direct hardware register access (ioremap)
DMA basics

Why: You'll need to map GPIO registers into kernel virtual memory.

## 5. Concurrency in Kernel Space

The kernel is highly concurrent:

Spinlocks, mutexes, semaphores
Atomic operations
Interrupt context vs process context
Bottom halves (softirqs, tasklets, workqueues)

Why: Drivers often handle interrupts and must be thread-safe.

## 6. GPIO Subsystem Study

Before writing a GPIO driver, understand the existing one:

Study the Linux GPIO subsystem (drivers/gpio/ in kernel source)
gpiolib framework
GPIO consumer vs provider APIs
Understanding /sys/class/gpio/

Why: You shouldn't reinvent the wheel - learn from existing GPIO drivers.
Practice: Use the existing GPIO driver through sysfs before writing your own.

## Suggested Learning Path (Ordered):

Week 1-2: Linux system programming (user space)

Write programs that use file I/O, processes, threads
Control Raspberry Pi GPIO from user space via sysfs

Week 3: Simple "Hello World" kernel module

Learn module loading/unloading
Use printk and check dmesg

Week 4: Simple character device driver

Create /dev/mychar that you can read/write from user space
Learn about file_operations structure

Week 5-6: Study Raspberry Pi device tree and GPIO subsystem

Read BCM2835/BCM2711 datasheets
Study existing GPIO drivers in kernel source

Week 7+: Attempt a simple GPIO driver

Start with a platform driver that just registers itself
Gradually add GPIO functionality

Key differences from bare-metal/FreeRTOS you'll encounter:
AspectBare-metal/FreeRTOSLinux KernelMemoryDirect register accessioremap() requiredDelaysBusy loops OKMust use proper sleep functionsInterruptsDirect ISR registrationRequest_irq(), threaded handlersConcurrencySimple mutexes/semaphoresComplex locking (spinlocks in IRQ context)DebuggingJTAG, printfprintk, /proc, /sys, ftrace, kgdbFailureSystem hangsKernel panic (whole system down)

## Recommended resources:

Book: "Linux Device Drivers" (3rd edition) - freely available online
Book: "Linux Kernel Development" by Robert Love
Practice: Cross-compile kernel modules for your Raspberry Pi
Study: Existing drivers in /drivers/gpio/ of Linux kernel source
