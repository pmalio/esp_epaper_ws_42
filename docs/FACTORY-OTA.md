# Factory firmware and OTA

## Status and limitations

This design is an experimental engineering prototype, not a validated production recovery system. Validate and compile with the exact ESPHome version and test on a spare board before flashing a device that you rely on.

## Required hardware and files

- ESP32-WROOM-32 with confirmed 4 MB flash
- Waveshare 4.2" V2 display (GDEY042T81, 400×300)
- `firmware/epaper-production-display-template.yaml`
- `firmware/factory_reset.h`
- `hardware/epaper_partitions.csv`
- BMP asset referenced by the YAML, copied to the path configured under ESPHome
- A local `secrets.yaml` with a unique API encryption key

The YAML references the partition CSV and C++ header by relative filename. When using the ESPHome dashboard in Home Assistant, copy both files into the ESPHome configuration directory next to the YAML (or update the paths consistently). The BMP path in the YAML must also exist on the Home Assistant host.

## Partition layout

| Partition | Offset | Size |
|---|---:|---:|
| NVS | 0x9000 | 0x4000 |
| OTA metadata | 0xD000 | 0x2000 |
| PHY init | 0xF000 | 0x1000 |
| Factory app | 0x10000 | 0x150000 |
| OTA slot 0 | 0x160000 | 0x150000 |
| OTA slot 1 | 0x2B0000 | 0x150000 |

The last partition ends at 0x400000. Each application image must fit in 0x150000 bytes (1,376,256 bytes). Confirm the real flash size and compiled image size; do not blindly shrink partitions to make an image fit. The layout leaves no filesystem partition.

## Factory rollback behavior

The helper in `firmware/factory_reset.h` erases only OTA-selection metadata and restarts the ESP. ESP-IDF is expected to fall back to the factory app if a valid factory image was actually flashed to the factory partition. It does not erase NVS or Wi-Fi credentials, so factory firmware may reconnect to the previously configured Wi-Fi rather than show provisioning.

A normal ESPHome OTA update writes an OTA slot; it does not by itself install a factory image. A production factory-flash workflow must program a matching partition table and put the factory app at offset 0x10000. Determine the actual artifact paths and offsets from the generated build output rather than copying generic flash commands.

This is not immutable protection. A customer who flashes arbitrary firmware over USB, changes the partition table, or deliberately erases partitions may destroy the factory image. Strong protection requires a separate secure-boot/flash-encryption/signing design and is incompatible with unrestricted customer firmware uploads.

## Bring-up checklist

1. Back up the working firmware and retain USB/serial recovery access.
2. Confirm the chip's actual flash capacity is 4 MB.
3. Run ESPHome Validate, then Compile.
4. Confirm the application image is no larger than 0x150000 bytes and inspect the generated partition-table summary.
5. Test QR provisioning, Wi-Fi, Home Assistant API, all six display pages, unavailable sensors, GPIO27 page button, GPIO26 reset confirmation, OTA update and factory rollback on a spare device.
6. Test Wi-Fi loss, HA restart, failed OTA and repeated display refreshes.
7. Record build hashes and the exact tested ESPHome version.

Stop if validation, compilation, size checks or partition offsets do not match. Do not test factory rollback on the only working device.
