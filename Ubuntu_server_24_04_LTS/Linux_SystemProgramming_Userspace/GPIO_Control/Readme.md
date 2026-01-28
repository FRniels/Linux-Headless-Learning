# GPIO Control on Raspberry Pi 4B

## Retrieve Raspberry Pi 4B Pinout

Install GPIO tools and display pinout:

```bash
sudo apt install python3-gpiozero
pinout
```

## Understanding GPIO Numbering

Navigate to the GPIO driver directory:

```bash
cd /sys/class/gpio
ls
```

Check the GPIO chip configuration:

```bash
# Print GPIO base number (should output 512)
cat gpiochip512/base

# Print the number of available GPIOs
cat gpiochip512/ngpio
```

**Note:** On Raspberry Pi 4B, GPIO numbers are offset by 512. Physical GPIO pins must be accessed using this offset (e.g., GPIO17 = 512 + 17 = 529).

## Control GPIO via Bash Commands

### Basic GPIO Operations

Export GPIO17 (physical pin 11):

```bash
echo 529 | sudo tee export
```

Verify GPIO was exported (should display gpio529):

```bash
ls
```

Set GPIO as output:

```bash
echo out | sudo tee gpio529/direction
```

Control LED state:

```bash
# Turn LED on
echo 1 | sudo tee gpio529/value

# Turn LED off
echo 0 | sudo tee gpio529/value
```

### Continuous Blink Loop

Blink the LED continuously (press Ctrl+C to stop):

```bash
while :; do
    echo 1 | sudo tee gpio529/value
    sleep 1
    echo 0 | sudo tee gpio529/value
    sleep 1
done
```

### Cleanup

Unexport GPIO when finished:

```bash
echo 529 | sudo tee unexport
```

## Control GPIO via C Program

### LED blink

Create project directory and source file:

```bash
cd ~
mkdir SystemProgrammingBasics
cd SystemProgrammingBasics
nvim GPIO_blink.c
```

Source code reference: [GPIO_blink.c](GPIO_blink/GPIO_blink.c)

#### Compilation and Execution

Compile the source code:

```bash
gcc -o GPIO_blink GPIO_blink.c
```

Execute the program (requires root privileges):

```bash
sudo ./GPIO_blink
```

### LED wlan0 Rx indicator

Source code reference: [GPIO_blink_wlan_TxRx.c](GPIO_blink/GPIO_blink_wlan_TxRx.c)

## Bash vs C Execution Performance

The C program provides more stable and consistent timing compared to the bash loop for the following reasons:

**Process Overhead:**

- Bash creates multiple processes per iteration (echo, tee, sleep), introducing timing jitter
- C uses direct system calls without spawning additional processes

**File I/O Efficiency:**

- Bash opens and closes GPIO files on every write operation
- C maintains more efficient file descriptor management

**Timing Precision:**

- Bash sleep command accumulates overhead from process creation and pipe handling
- C sleep() system call provides more predictable timing behavior

**Result:** The C implementation exhibits lower CPU usage, reduced timing jitter, and more consistent LED blink intervals compared to the bash script approach.
