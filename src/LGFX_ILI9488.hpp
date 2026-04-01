#pragma once
#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_ILI9488  _panel_instance;
  lgfx::Bus_SPI        _bus_instance;
  lgfx::Light_PWM      _light_instance;  // optional backlight

public:
  LGFX(void) {
    // --- SPI bus config ---
    {
      auto cfg = _bus_instance.config();
      cfg.spi_host   = VSPI_HOST;   // VSPI_HOST or HSPI_HOST
      cfg.spi_mode   = 0;
      cfg.freq_write = 20000000;    // 40 MHz; try 20M if you get glitches
      cfg.freq_read  = 16000000;
      cfg.spi_3wire  = false;
      cfg.use_lock   = true;
      cfg.dma_channel = SPI_DMA_CH_AUTO;

      // --- Pin assignments (adjust to your wiring) ---
      cfg.pin_sclk = 18;
      cfg.pin_mosi = 23;
      cfg.pin_miso = 19;
      cfg.pin_dc   = 26;   // Data/Command

      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }

    // --- Panel config ---
    {
      auto cfg = _panel_instance.config();
      cfg.pin_cs   = 15;    // Chip Select
      cfg.pin_rst  = 4;    // Reset (-1 if tied to EN/3V3)
      cfg.pin_busy = -1;

      cfg.memory_width  = 320;
      cfg.memory_height = 480;
      cfg.panel_width   = 320;
      cfg.panel_height  = 480;

      cfg.offset_x     = 0;
      cfg.offset_y     = 0;
      cfg.offset_rotation = 0;

      cfg.readable      = true;
      cfg.invert        = true;
      cfg.rgb_order     = false;   // false = BGR (ILI9488 default)
      cfg.dlen_16bit    = false;   // important: keep false for ILI9488 SPI
      cfg.bus_shared    = false;

      _panel_instance.config(cfg);
    }

    // --- Backlight (optional) ---
    {
      auto cfg = _light_instance.config();
      cfg.pin_bl      = 25;   // BL pin, or -1 if always-on
      cfg.invert      = false;
      cfg.freq        = 44100;
      cfg.pwm_channel = 7;

      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);
    }

    setPanel(&_panel_instance);
  }
};