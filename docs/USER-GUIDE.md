# User Guide

## Changing the firmware

This repository contains the ESPHome YAML template and its supporting files. Clone or fork it before changing the display pages, Home Assistant mappings, Wi-Fi provisioning, buttons, or other firmware behavior. Keep the YAML and its support files together; changing only a copy in the Home Assistant configuration directory will not update this repository.

## Install the project files

Copy the `firmware/` and `hardware/` directories into your ESPHome configuration directory while preserving their layout. For example, under `/config/esphome`, keep:

```text
/config/esphome/
├── firmware/
│   ├── epaper-production-display-template.yaml
│   ├── factory_reset.h
│   └── images/totoro_100x100_.bmp
└── hardware/
    └── epaper_partitions.csv
```

The YAML refers to the partition table as `../hardware/epaper_partitions.csv`, relative to the YAML file. The factory reset header is included from the same `firmware/` directory as the YAML.

The image definition in the YAML currently uses the absolute path `/config/esphome/images/totoro_100x100_.bmp`. Copy the provided BMP there as well, or change the YAML image path to the file’s location and make sure that path is available to ESPHome at build time.

## Configure each device

Before compiling, edit the `substitutions` at the top of the YAML for the specific device:

- Set a unique device name and friendly name.
- Set a unique provisioning access-point SSID and password. The provisioning QR encodes these values.
- Replace the example `sensor.replace_me_*`, `binary_sensor.replace_me_*`, and `person.replace_me_*` entity IDs with entities from your Home Assistant instance.
- Create a local `secrets.yaml` containing a unique, valid ESPHome API encryption key. The repository includes `secrets-production-example.yaml` as a placeholder example; replace its placeholder before compiling.

Keep real API keys, Wi-Fi passwords, and other credentials out of Git. Do not commit `secrets.yaml`.

## Controls and display pages

The template has six dashboard pages. GPIO27 advances the page when Wi-Fi is connected. GPIO26 starts a factory rollback confirmation; press it again within five seconds to confirm. The rollback only works if a valid factory firmware image has been flashed into the factory app partition. A normal OTA update does not install that factory image; see [Factory firmware and OTA](FACTORY-OTA.md).

## Validate before installing

Run ESPHome validation and compilation using the version configured by `min_version` or a compatible newer release. Confirm the image fits the application partition and test provisioning, Home Assistant sensors, all pages, buttons, OTA, and recovery on a spare device before updating hardware you rely on. See the [Factory/OTA notes](FACTORY-OTA.md) for the partition layout and recovery limitations.
