#include <U8g2lib.h>

const uint8_t deviceDescriptor[] = {
  0x12, 0x01, 0x10, 0x01, 0x01, 0x01, 0x01, 0x01,
  0x80, 0x01, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01,
  0x05, 0x01, 0x09, 0x06, 0x01, 0x01, 0x01, 0x01,
  0x95, 0x08, 0x75, 0x01, 0x95, 0x01, 0x75, 0x01,
  0x05, 0x07, 0x19, 0x01, 0x29, 0x01, 0x95, 0x01,
  0x75, 0x01, 0x05, 0x08, 0x19, 0x01, 0x29, 0x01,
  0x95, 0x01, 0x75, 0x01, 0x05, 0x07, 0x19, 0x01,
  0x29, 0x01, 0x95, 0x01, 0x75, 0x01
};

unsigned long startTime = 0;
unsigned long keyPressTime = 0;
bool keyboardDetected = false;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void setup() {
  Serial.begin(9600);
  startTime = millis();
  u8g2.begin();
}

void loop() {
  if (millis() - startTime >= 20000) {
    // Terminar el programa después de 20 segundos
    return;
  }

  unsigned long elapsedTime = millis() - startTime;
  unsigned long seconds = elapsedTime / 1000;

  // Muestra el tiempo transcurrido en el display
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.setCursor(0, 15);
  u8g2.print("Tiempo transcurrido:");
  u8g2.setCursor(0, 30);
  u8g2.print(seconds);
  u8g2.print(" segundos");

  // Verificar si la BIOS detecta el dispositivo como un teclado USB
  if (Serial.available() > 0) {
    char c = Serial.read();
    if (c == 'K') { // Caracter de respuesta de la BIOS
      keyboardDetected = true;
    }
  }

  if (keyboardDetected) {
    keyPressTime++;
    if (keyPressTime >= 20) {
      // Detener el bucle cuando se alcance el tiempo deseado
      return;
    }

    // Simular presión de tecla F8
    Serial.print(0x05); // Comando de tecla presionada
    Serial.print(0x46); // Vendor ID: Logitech (0x046D)
    Serial.print(0x0D); // Product ID: Logitech K120 (0xC051)
    Serial.print(0x04); // Tecla presionada: F8
    delay(50); // Tiempo de presión: 50 ms

    // Simular presión de tecla F11
    Serial.print(0x05); // Comando de tecla presionada
    Serial.print(0x46); // Vendor ID: Logitech (0x046D)
    Serial.print(0x0D); // Product ID: Logitech K120 (0xC051)
    Serial.print(0x0B); // Tecla presionada: F11
    delay(50); // Tiempo de presión: 50 ms
  }

  u8g2.sendBuffer();

  // Enviar comandos para que la BIOS detecte el dispositivo como un teclado USB
  if (millis() - startTime >= 100) { // Tiempo de detección: 100 ms
    Serial.print(0x05); // Comando de inicio de sesión
    Serial.print(0x01); // Tipo de dispositivo: teclado
    Serial.print(0x46); // Vendor ID: Logitech (0x046D)
    Serial.print(0x0D); // Product ID: Logitech K120 (0xC051)
    Serial.print(0x04); // Tecla presionada: F8
    delay(50); // Tiempo de presión: 50 ms

    Serial.print(0x05); // Comando de tecla presionada
    Serial.print(0x46); // Vendor ID: Logitech (0x046D)
    Serial.print(0x0D); // Product ID: Logitech K120 (0xC051)
    Serial.print(0x0B); // Tecla presionada: F11
    delay(50); // Tiempo de presión: 50 ms
  }

  if (millis() - startTime >= 1100) { // Tiempo de comprobación de la BIOS: 1 segundo
    Serial.print(0x05); // Comando de fin de sesión
    delay(1000); // Tiempo de comprobación de la BIOS: 1 segundo

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.setCursor(0, 15);
    u8g2.print("Programa terminado");
    u8g2.sendBuffer();
  }
}