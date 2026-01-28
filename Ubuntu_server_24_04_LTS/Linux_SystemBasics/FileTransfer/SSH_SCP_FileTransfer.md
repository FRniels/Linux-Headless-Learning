# SSH SCP File transfer

## Copy Windows -> Linux

Run command on Windows side:

Single file

```
scp "C:\Users\11307533\Documents\Programming\Linux\GPIO_blink.c" ubuntu@192.168.1.50:/home/ubuntu/
```

Entire directory (recursive)

```
scp -r "C:\Users\11307533\Documents\Programming\Linux\GPIO_Control" ubuntu@192.168.1.50:/home/ubuntu/
```

## Copy Linux -> Windows

Run command on Linux side:

```
scp ubuntu@192.168.1.50:/home/ubuntu/GPIO_blink.c /mnt/c/Users/11307533/Documents/
```

This only works if:

- Using WSL
- Windows drive is mounted at /mnt/c

## Copy Linux -> Windows (pure Windows target)

When not using WSL, run command on Linux side:

```
scp GPIO_blink.c 11307533@192.168.1.100:/Users/11307533/Documents/
```

Where:

- 11307533 = windows machine name
- 192.168.1.100 = Windows machine IP
- OpenSSH Server must be enabled on Windows
