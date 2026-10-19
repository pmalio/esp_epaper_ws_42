# ESP e-Paper Waveshare 4.2" V2

Projekt firmware ESPHome dla modułu Waveshare 4.2" V2 (GDEY042T81, 400×300) i ESP32-WROOM-32.

## Status

**Projekt eksperymentalny — nie traktuj konfiguracji partycji i odzyskiwania jako produkcyjnie zweryfikowanych.** Przed flashowaniem należy wykonać walidację i kompilację w dokładnie tej wersji ESPHome, sprawdzić rozmiar obrazu i przetestować procedurę odzyskiwania na urządzeniu testowym.

## Struktura

- `firmware/epaper-production-display-template.yaml` — szablon ESPHome z sześcioma stronami, QR konfiguracji Wi-Fi, OTA i przyciskami.
- `hardware/epaper_partitions.csv` — układ partycji dla pamięci Flash 4 MB.
- `firmware/factory_reset.h` — pomocniczy kod C++ do powrotu do obrazu fabrycznego przez wyczyszczenie metadanych OTA.
- `docs/USER-GUIDE.md` — konfiguracja i wdrożenie szablonu dla użytkownika.
- `docs/FACTORY-OTA.md` — ograniczenia i procedura testowania.
- `secrets-production-example.yaml` — przykładowe sekrety; prawdziwych kluczy i haseł nigdy nie commituj.

## Wdrożenie do Home Assistant

Skopiuj katalogi `firmware/` i `hardware/` do katalogu konfiguracji ESPHome, zachowując ich strukturę. YAML i `factory_reset.h` pozostają w `firmware/`, a tabela partycji w `hardware/`; ścieżka `../hardware/epaper_partitions.csv` jest liczona względem pliku YAML. Plik BMP musi być dostępny pod ścieżką skonfigurowaną w YAML. Zanim wdrożysz szablon na działającym urządzeniu, zrób kopię zapasową konfiguracji i firmware oraz sprawdź mapowanie encji Home Assistant. Nie flashuj konfiguracji, jeśli walidacja lub kompilacja nie przejdzie.

## Układ partycji

Każda partycja aplikacji (`factory`, `ota_0`, `ota_1`) ma 0x150000 bajtów. Każdy obraz firmware musi się zmieścić w tej wielkości. Układ zajmuje pełne 4 MB i zakłada rzeczywistą pamięć Flash 4 MB.

Szczegóły ryzyka i listę testów opisuje [dokumentacja Factory/OTA](docs/FACTORY-OTA.md).
