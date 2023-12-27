/*
    Cabeçalho responsável pelo acionamento dos motores para realizar a
    movimentação do robô.
*/

#pragma once

#include <Arduino.h>
#include <ESP32Servo.h> // Biblioteca ESP32Servo por Kevin Harrington

// Definição dos pinos dos motores
#define PINO_ESC   23 // Pino do ESC
#define PINO_SERVO 18 // Pino do motor servo

// Definição da frequência (em Hz) dos sinais de PWM utilizados para controlar os motores
#define FREQUENCIA_DO_SINAL_DE_PWM 50

// Definição do intervalo do período (em microssegundos) do sinal de PWM utilizado pelo ESC e pelo motor servo
#define PERIODO_MINIMO_DO_SINAL_DE_PWM 1000
#define PERIODO_MAXIMO_DO_SINAL_DE_PWM 2000

namespace locomocao {
    // Definição dos objetos dos motores
    Servo ESC;   // Objeto do ESC
    Servo servo; // Objeto do servo motor

    // Função para iniciar os motores
    void initMotores() {
        // Aloca os canais de PWM utilizados pelos motores
        ESP32PWM::allocateTimer(0);
        ESP32PWM::allocateTimer(1);
        ESP32PWM::allocateTimer(2);
        ESP32PWM::allocateTimer(3);

        // Define a frequência dos sinais de PWM utilizados para controlar os motores
        ESC.setPeriodHertz(FREQUENCIA_DO_SINAL_DE_PWM);
        servo.setPeriodHertz(FREQUENCIA_DO_SINAL_DE_PWM);

        // Configura os pinos para o controle dos motores
        ESC.attach(PINO_ESC, PERIODO_MINIMO_DO_SINAL_DE_PWM, PERIODO_MAXIMO_DO_SINAL_DE_PWM);     // Configura o pino do ESC
        servo.attach(PINO_SERVO, PERIODO_MINIMO_DO_SINAL_DE_PWM, PERIODO_MAXIMO_DO_SINAL_DE_PWM); // Configura o pino do servo motor
    }

    /**
     * @brief Função para aplicar a velocidade linear e a velocidade angular nos motores.
     * 
     * @param velocidadeLinear Varia no intervalo [0, 180]. Utilize o valor 90 para zerar a velocidade linear;
     * @param velocidadeAngular Varia no intervalo [0, 180]. Utilize o valor 90 para zerar a velocidade angular.
     */
    void dirigir(int velocidadeLinear, int velocidadeAngular) {
        // Aplica no ESC a velocidade linear
        ESC.write(velocidadeLinear);

        // Aplica no servo a velocidade angular
        servo.write(velocidadeAngular);
    }
    
    // Função para parar os motores
    void parar() {
        ESC.write(90);
        
    }

    // Função que garante que o carro freie dando um tranco para trás
    // A entrada booleana é obrigatória para não dar problema de um arquivo importar o outro
    int tempoDeFreio = 400;
    void frear(bool estavaAndando) {
        int tempoFreando = 0;
        if (estavaAndando){
            tempoFreando = millis();
        }
        while (millis() - tempoFreando <= tempoDeFreio){
            dirigir(0,90);
        }
        dirigir(90,90);
    }
} // namespace locomocao
