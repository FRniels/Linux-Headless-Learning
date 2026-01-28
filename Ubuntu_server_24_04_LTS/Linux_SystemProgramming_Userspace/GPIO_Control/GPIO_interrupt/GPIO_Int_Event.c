#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <time.h>

#define GPIO_TRIGGER "529"  // GPIO17 (physical pin 11) = 512 + 17
#define GPIO_BUTTON "539"   // GPIO27 (physical pin 13) = 512 + 27
#define GPIO_PATH "/sys/class/gpio"

volatile sig_atomic_t keep_running = 1;
volatile sig_atomic_t interrupt_count = 0;
int button_fd;

void handle_sigint(int sig) {
    keep_running = 0;
}

void handle_sigio(int sig) {
    char value;
    lseek(button_fd, 0, SEEK_SET);
    read(button_fd, &value, 1);
    
    interrupt_count++;
    
    time_t now = time(NULL);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    printf("[%s] EVENT: GPIO INTERRUPT! Button state: %c (count: %d)\n", 
           timestamp, value, interrupt_count);
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
    if (write_file(GPIO_PATH "/export", pin) < 0) {
        log_message("WARN", "GPIO might already be exported");
    }
    usleep(100000);
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

int gpio_set_edge(const char *pin, const char *edge) {
    char path[64];
    snprintf(path, sizeof(path), GPIO_PATH "/gpio%s/edge", pin);
    return write_file(path, edge);
}

void cleanup_gpio() {
    log_message("INFO", "Cleaning up GPIOs");
    if (button_fd >= 0) {
        close(button_fd);
    }
    gpio_set_value(GPIO_TRIGGER, 0);
    gpio_unexport(GPIO_TRIGGER);
    gpio_unexport(GPIO_BUTTON);
}

int main() {
    signal(SIGINT, handle_sigint);
    signal(SIGIO, handle_sigio);
    
    log_message("INFO", "SysPulse v0.2 - GPIO Async Event Demo");
    log_message("INFO", "GPIO17 (pin 11) toggles every second");
    log_message("INFO", "GPIO27 (pin 13) triggers async interrupt on both edges");
    
    // Initialize GPIO17 (trigger output)
    if (gpio_export(GPIO_TRIGGER) < 0) {
        log_message("ERROR", "Failed to export GPIO trigger");
        return 1;
    }
    
    if (gpio_set_direction(GPIO_TRIGGER, "out") < 0) {
        log_message("ERROR", "Failed to set GPIO trigger direction");
        cleanup_gpio();
        return 1;
    }
    
    // Initialize GPIO27 (button input with interrupt)
    if (gpio_export(GPIO_BUTTON) < 0) {
        log_message("ERROR", "Failed to export GPIO button");
        cleanup_gpio();
        return 1;
    }
    
    if (gpio_set_direction(GPIO_BUTTON, "in") < 0) {
        log_message("ERROR", "Failed to set GPIO button direction");
        cleanup_gpio();
        return 1;
    }
    
    if (gpio_set_edge(GPIO_BUTTON, "both") < 0) {
        log_message("ERROR", "Failed to set GPIO button edge");
        cleanup_gpio();
        return 1;
    }
    
    // Open button value file for async I/O
    char value_path[64];
    snprintf(value_path, sizeof(value_path), GPIO_PATH "/gpio%s/value", GPIO_BUTTON);
    
    button_fd = open(value_path, O_RDONLY | O_NONBLOCK);
    if (button_fd < 0) {
        log_message("ERROR", "Failed to open button value file");
        cleanup_gpio();
        return 1;
    }
    
    // Set up async notification
    fcntl(button_fd, F_SETOWN, getpid());
    int flags = fcntl(button_fd, F_GETFL);
    fcntl(button_fd, F_SETFL, flags | O_ASYNC);
    
    // Initial read to clear
    char value;
    read(button_fd, &value, 1);
    
    log_message("INFO", "Async event handler configured");
    log_message("INFO", "Starting main loop (Press Ctrl+C to stop)");
    
    // Main loop: toggle GPIO17 every second
    // handle_sigio() fires ASYNCHRONOUSLY when GPIO27 changes!
    int state = 0;
    int toggle_count = 0;
    
    while (keep_running) {
        state = !state;
        
        if (gpio_set_value(GPIO_TRIGGER, state) < 0) {
            log_message("ERROR", "Failed to set GPIO trigger value");
            break;
        }
        
        char msg[64];
        snprintf(msg, sizeof(msg), "GPIO17 toggled: %s (count: %d)", 
                 state ? "HIGH" : "LOW", ++toggle_count);
        log_message("INFO", msg);
        
        sleep(1);
    }
    
    char final_msg[64];
    snprintf(final_msg, sizeof(final_msg), "Total interrupts received: %d", interrupt_count);
    log_message("INFO", final_msg);
    
    cleanup_gpio();
    log_message("INFO", "Exiting");
    
    return 0;
}