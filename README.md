# Snake Game

<br>

#### Snake Game is an arduino game project built with ESP32-S3, TFT LCD display, and an analog joystick.

<br>

---

## Development Environment

- **Board**: ESP32  
- **IDE**: Arduino IDE  
- **Display**: TFT LCD ILI9341 (320x240)  
- **Libraries**: TFT_eSPI, SPI  
- **Input Device**: Analog Joystick  

<br>

---

## Joystick Pins (GPIO)
> **Note:** Adjust pin numbers according to your board.

| Function | Pin |
|----------|-----|
| X-axis   | 14  |
| Y-axis   | 13  |
| Button   | 12  |

<br>

---

## TFT Display Pins (GPIO)  
> **Note:** If you are using the **TFT_eSPI** library, make sure to define the display pins in the library (User_Setup.h).

| Function | Pin |
|----------|-----|
| CS       | 8   |
| DC       | 5   |
| RST      | 7   |
| MOSI     | 38  |
| MISO     | 47  |
| SCLK     | 48  |
