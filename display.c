#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Configuración de pines y dirección I2C
#define I2C_PORT i2c0
#define SDA_PIN  4
#define SCL_PIN  5
#define OLED_ADDR 0x3C

// Dimensiones de la pantalla OLED
#define OLED_WIDTH 128
#define OLED_HEIGHT 64

// Prototipos de funciones
void oled_init();
void oled_clear();
void oled_send_command(uint8_t cmd);
void oled_send_data(uint8_t *data, size_t len);
void oled_write_text(const char *text, int x, int y);

int main() {
    // Inicializar stdio y I2C
    stdio_init_all();
    i2c_init(I2C_PORT, 400 * 1000); // Velocidad de I2C a 400kHz
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    // Inicializar la pantalla OLED
    oled_init();

    // Borrar la pantalla OLED
    oled_clear();

    // Escribir texto en la pantalla OLED
    oled_write_text("Samuel de Jesus", 0, 0);
    oled_write_text("Lopez Agudo", 0, 16);

    while (1) {
        // Mantener el programa en funcionamiento
        sleep_ms(1000);
    }
    return 0;
}

void oled_init() {
    // Comandos de inicialización para SSD1306
    static const uint8_t init_cmds[] = {
        0xAE,         // Apaga la pantalla
        0xD5, 0x80,   // Configura el reloj de pantalla
        0xA8, 0x3F,   // Multiplex ratio
        0xD3, 0x00,   // Offset de la pantalla
        0x40,         // Línea de inicio de la pantalla
        0x8D, 0x14,   // Configuración de carga de la pantalla
        0x20, 0x00,   // Modo de dirección horizontal
        0xA1,         // Remapeo de segmentos
        0xC8,         // Dirección de salida COM
        0xDA, 0x12,   // Configuración de hardware COM
        0x81, 0xCF,   // Configuración de contraste
        0xD9, 0xF1,   // Configuración de pre-carga
        0xDB, 0x40,   // Nivel VCOM deseado
        0xA4,         // Pantalla en modo normal
        0xA6,         // Modo de visualización normal
        0xAF          // Enciende la pantalla
    };
    for (int i = 0; i < sizeof(init_cmds); i++) {
        oled_send_command(init_cmds[i]);
    }
}

void oled_clear() {
    // Borra la pantalla enviando ceros a todas las posiciones de memoria
    uint8_t clear_data[OLED_WIDTH * OLED_HEIGHT / 8] = {0};
    for (int i = 0; i < OLED_HEIGHT / 8; i++) {
        oled_send_command(0xB0 + i);    // Configura la página
        oled_send_command(0x00);        // Configura la columna baja
        oled_send_command(0x10);        // Configura la columna alta
        oled_send_data(clear_data, OLED_WIDTH);
    }
}

void oled_send_command(uint8_t cmd) {
    uint8_t buffer[2] = {0x00, cmd};  // Primer byte: 0x00 indica comando
    i2c_write_blocking(I2C_PORT, OLED_ADDR, buffer, 2, false);
}

void oled_send_data(uint8_t *data, size_t len) {
    uint8_t buffer[len + 1];
    buffer[0] = 0x40;  // Primer byte: 0x40 indica datos
    for (size_t i = 0; i < len; i++) {
        buffer[i + 1] = data[i];
    }
    i2c_write_blocking(I2C_PORT, OLED_ADDR, buffer, len + 1, false);
}

void oled_write_text(const char *text, int x, int y) {

}
