# Ubuntu Server 24.04 LTS Initial Setup (Raspberry Pi 4)

## First Boot and Login

1. Connect monitor and keyboard to the Raspberry Pi
2. Wait for the login prompt to appear (approximately 1-2 minutes after boot)
3. Login with default credentials:
   - Username: `ubuntu`
   - Password: `ubuntu`
4. The system will immediately prompt for a password change
5. Enter the current password (`ubuntu`) and set a new password when prompted

## Keyboard Layout Configuration

Configure the keyboard layout to match the physical keyboard:

```bash
sudo dpkg-reconfigure keyboard-configuration
```

Follow the prompts:

1. Select keyboard model (Generic 105-key for most keyboards)
2. Select keyboard layout (e.g., Belgian, US English, etc.)
3. Accept defaults for remaining options

Reboot to apply changes:

```bash
sudo reboot
```

## WiFi Configuration

Edit the Netplan configuration file:

```bash
sudo nano /etc/netplan/50-cloud-init.yaml
```

Replace the contents with the following configuration:

```yaml
network:
  version: 2
  wifis:
    wlan0:
      dhcp4: true
      access-points:
        "SSID_NAME":
          password: "WIFI_PASSWORD"
```

Replace `SSID_NAME` with the WiFi network name and `WIFI_PASSWORD` with the actual password.

Apply the network configuration:

```bash
sudo netplan apply
```

Verify the connection by checking the assigned IP address:

```bash
ip a
```

The `wlan0` interface should display an assigned IP address.

## SSH Configuration

### Install and Enable SSH Server

```bash
sudo apt update
sudo apt install openssh-server => Did not have to install it
sudo systemctl enable ssh
sudo systemctl start ssh
```

### Enable Password Authentication

Ubuntu Server 24.04 uses cloud-init which overrides the main SSH configuration. Password authentication must be enabled in the cloud-init configuration file.

Edit the cloud-init SSH configuration:

```bash
sudo nano /etc/ssh/sshd_config.d/50-cloud-init.conf
```

Locate the following line:

```
PasswordAuthentication no
```

Change it to:

```
PasswordAuthentication yes
```

Save the file and restart the SSH service:

```bash
sudo systemctl restart ssh
```

### Obtain IP Address

Retrieve the IP address for SSH connections:

```bash
hostname -I
```

Note the IP address displayed (e.g., `192.168.0.139`).

## Remote SSH Access

### From Windows

Open PowerShell or Command Prompt and connect:

```bash
ssh ubuntu@192.168.0.139
```

Replace `192.168.0.139` with the actual IP address from the previous step.

### Clear Previous SSH Keys (if needed)

If connecting to a previously used IP address, clear the old SSH key (windows powershell):

```powershell
ssh-keygen -R 192.168.0.139
```

Replace `192.168.0.139` with the actual IP address.
