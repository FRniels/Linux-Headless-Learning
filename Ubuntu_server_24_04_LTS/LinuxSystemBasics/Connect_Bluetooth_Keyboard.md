# Bluetooth Keyboard Setup on Ubuntu Server 24.04 LTS (Raspberry Pi 4)

## Installation

Install required Bluetooth packages:

```bash
sudo apt install bluez pi-bluetooth
sudo systemctl enable bluetooth
sudo systemctl start bluetooth
```

## Pairing Process

### Prerequisites

- SSH connection to the Raspberry Pi established
- Keyboard in Bluetooth mode (not connected via USB cable)
- Keyboard powered on

### Pairing Steps

1. Launch the Bluetooth control utility:

```bash
bluetoothctl
```

2. Enable the Bluetooth agent and begin scanning:

```bash
power on => Did not have to do this
agent on => Did not have to do this
scan on
```

3. Put the keyboard into pairing mode by holding `Fn + 1` for 3-4 seconds until the LED indicator blinks.

4. Wait for the keyboard to appear in the scan results. The output will display:

```
[NEW] Device XX:XX:XX:XX:XX:XX Keychron Q6 Pro
```

Note the MAC address (XX:XX:XX:XX:XX:XX format).

5. Stop scanning before initiating pairing:

```bash
scan off
```

6. Initiate pairing with the keyboard:

```bash
pair XX:XX:XX:XX:XX:XX
```

Replace `XX:XX:XX:XX:XX:XX` with the actual MAC address from step 4.

7. If a PIN code is displayed, type it on the keyboard and press Enter within the timeout period (approximately 10-15 seconds).

Connecting a Keychron Q10 pro keyboard did not ask for a PIN, connecting a laptop over bluetooth hower did ask for entering a PIN on the connecting laptop side.

8. Once pairing is successful, trust and connect to the device:

```bash
trust XX:XX:XX:XX:XX:XX
connect XX:XX:XX:XX:XX:XX
```

9. Exit the Bluetooth utility:

```bash
quit
```

## Verification

The keyboard should now be functional. On subsequent boots, the keyboard will automatically reconnect to the Raspberry Pi.

## Troubleshooting

### Common Issues

- **Keyboard not discovered**: Ensure the keyboard is in pairing mode (Fn + channel num and LED blinking)
- **Connection timeout**: Stop scanning before pairing (`scan off` before `pair` command)
- **Authentication timeout**: Enter the PIN code quickly when prompted

### Keychron Q10 Pro Specific Notes

The Keychron Q10 Pro keyboard supports multiple Bluetooth profiles accessible via `Fn + 1`, `Fn + 2`, and `Fn + 3`.
