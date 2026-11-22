#pragma once
// GPIO et rythme de mesures : compatibles avec le montage actuel.
static const int PIN_SOIL = 34;          // AOUT sonde capacitive, ADC1
static const int PIN_SDA = 21;           // AHT20, BMP280 et BH1750
static const int PIN_SCL = 22;
static const int PIN_DS18B20 = 27;       // A relier mercredi, 4.7k entre DATA et 3V3
static const uint32_t SAMPLE_MS = 5000; // acquisition reguliere
static const uint32_t DISK_MS = 300000; // ecriture sur flash toutes les 5 min
static const uint32_t LIVE_MS = 5000;
static const uint32_t CALIBRATION_MS = 300000; // 5 min
static const uint32_t CALIBRATION_STEP_MS = 20000;
