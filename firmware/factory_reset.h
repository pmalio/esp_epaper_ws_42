#pragma once

#include "esp_partition.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_system.h"

// Clears only OTA selection metadata. On the next boot, ESP-IDF should select
// the factory app partition if it contains a valid image. Wi-Fi/NVS settings
// are intentionally preserved. This is a firmware rollback, not a full data wipe.
inline void epaper_boot_factory_firmware() {
  const esp_partition_t *otadata = esp_partition_find_first(
      ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_OTA, nullptr);
  if (otadata == nullptr) {
    ESP_LOGE("factory_reset", "OTA data partition not found; cannot select factory image");
    return;
  }

  const esp_err_t err = esp_partition_erase_range(otadata, 0, otadata->size);
  if (err != ESP_OK) {
    ESP_LOGE("factory_reset", "Could not erase OTA metadata: %s", esp_err_to_name(err));
    return;
  }

  ESP_LOGW("factory_reset", "OTA metadata erased; rebooting to factory image. Wi-Fi settings remain stored.");
  delay(300);
  esp_restart();
}