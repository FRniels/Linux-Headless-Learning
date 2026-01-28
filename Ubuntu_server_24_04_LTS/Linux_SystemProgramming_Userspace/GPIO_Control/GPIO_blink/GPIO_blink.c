#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <time.h>

#define GPIO_PIN "529" // GPIO17 (physical pin 11) = 512 + 17
#define GPIO_PATH "/sys/class/gpio"

volatile sig_atomic_t keep_running = 1;

void handle_signal(int sig) {
    keep_running = 0;
}

void log_message(const char *level, const char *message) {
    time_t now = time(NULL);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
    printf("[%s] %s: %s\n", timestamp, level, message);
}

int write_file(const char *path, const char *value) {
    int fd = open(path, O_WRONLY);
    if (fd < 0) {
        char msg[256];
        snprintf(msg, sizeof(msg), "Failed to open %s: %s", path, strerror(errno));
        log_message("ERROR", msg);
        return -1;
    }
    
    ssize_t bytes_written = write(fd, value, strlen(value));
    if (bytes_written < 0) {
        char msg[256];
        snprintf(msg, sizeof(msg), "Failed to write to %s: %s", path, strerror(errno));
        log_message("ERROR", msg);
        close(fd);
        return -1;
    }
    
    close(fd);
    return 0;
}

int gpio_export(const char *pin) {
    // Try to export (might already be exported)
    if (write_file(GPIO_PATH "/export", pin) < 0) {
        log_message("WARN", "GPIO might already be exported");
    }
    usleep(100000); // Wait for sysfs
    return 0;
}

int gpio_unexport(const char *pin) {
    return write_file(GPIO_PATH "/unexport", pin);
}

int gpio_set_direction(const char *pin, const char *direction) {
    char path[64];
    snprintf(path, sizeof(path), GPIO_PATH "/gpio%s/direction", pin);
    return write_file(path, direction);
}

int gpio_set_value(const char *pin, int value) {
    char path[64];
    snprintf(path, sizeof(path), GPIO_PATH "/gpio%s/value", pin);
    return write_file(path, value ? "1" : "0");
}

void cleanup_gpio() {
    log_message("INFO", "Cleaning up GPIO");
    gpio_set_value(GPIO_PIN, 0);
    gpio_unexport(GPIO_PIN);
}

int main() {
    // Catch the CTRL + C close signal
    signal(SIGINT, handle_signal);
    
    log_message("INFO", "SysPulse v0.1 - LED Blink");
    log_message("INFO", "Starting LED on GPIO" GPIO_PIN " (Press Ctrl+C to stop)");
    
    // Initialize GPIO
    if (gpio_export(GPIO_PIN) < 0) {
        log_message("ERROR", "Failed to export GPIO");
        return 1;
    }
    
    if (gpio_set_direction(GPIO_PIN, "out") < 0) {
        log_message("ERROR", "Failed to set GPIO direction");
        cleanup_gpio();
        return 1;
    }
    
    // Main blink loop
    int state = 0;
    int blink_count = 0;
    
    while (keep_running) {
        state = !state;
        
        if (gpio_set_value(GPIO_PIN, state) < 0) {
            log_message("ERROR", "Failed to set GPIO value");
            break;
        }
        
        log_message("INFO", state ? "LED ON" : "LED OFF");
        blink_count++;
        
        sleep(1);
    }
    
    char msg[64];
    snprintf(msg, sizeof(msg), "Total blinks: %d", blink_count / 2);
    log_message("INFO", msg);
    
    cleanup_gpio();
    log_message("INFO", "Exiting");
    
    return 0;
}