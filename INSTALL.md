# Installation and Environment Setup

Target platform: **Raspberry Pi 4B** running Raspberry Pi OS (Debian-based).

---

## 1. Enable SPI

Open the Raspberry Pi configuration tool and enable SPI:

```sh
sudo raspi-config
```

Navigate to **Interface Options > SPI** and enable it.  Reboot if prompted.

---

## 2. Add your user to the GPIO group

```sh
sudo apt-get install rpi.gpio-common
sudo usermod -aG gpio $USER
```

Log out and back in for the group change to take effect.

---

## 3. Install WiringPi

The version available through `apt` may be outdated.  Install from source instead:

```sh
git clone https://github.com/WiringPi/WiringPi.git
cd WiringPi
./build
```

Verify the installation:

```sh
gpio -v
gpio readall
```

---

## 4. Build the project

```sh
make ptx    # builds the PTX binary
make prx    # builds the PRX binary
make clean  # removes built binaries
```

---

## 5. Run

Both binaries must be run with sufficient privileges to access the SPI device and GPIO:

```sh
sudo ./ptx
sudo ./prx
```
