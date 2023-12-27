/*
    Cabeçalho responsável pela implementação do LED que acende quando o
    robô está sobre a plataforma.
*/

#pragma once

#include <Arduino.h>

#define PINO_DA_FITA_DE_LED 2

namespace led {
    
    // Configura o LED e garante que ele irá começar desligado. Deve ser chamada no setup()
    void initLed() {
        pinMode(PINO_DA_FITA_DE_LED, OUTPUT);
        digitalWrite(PINO_DA_FITA_DE_LED, LOW);
    }

    // Liga o LED
    void ligarLed() {
        digitalWrite(PINO_DA_FITA_DE_LED, HIGH);
    }


    // Desliga o LED
    void desligarLed() {
        digitalWrite(PINO_DA_FITA_DE_LED, LOW);
    }
    
} // namespace led
