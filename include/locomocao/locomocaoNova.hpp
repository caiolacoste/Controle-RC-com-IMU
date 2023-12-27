/*
    Cabeçalho responsável pelo acionamento dos motores para realizar a
    movimentação do robô.
*/

#pragma once

#include <Arduino.h>
#include <L298N.h> // Biblioteca responsável por controlar ponte H

// Pinagem dos motores
#define ENA 14
#define IN1 27
#define IN2 26
#define ENB 32
#define IN3 25
#define IN4 33


// Definição do intervalo do período (em microssegundos) do sinal de PWM utilizado pelo ESC e pelo motor servo
#define PERIODO_MINIMO_DO_SINAL_DE_PWM 1000
#define PERIODO_MAXIMO_DO_SINAL_DE_PWM 2000

namespace locomocao {
    // Definição dos objetos dos motores
    L298N motorEsquerdo(IN1,IN2);
    L298N motorDireito(IN3,IN4);

    /**
     * @brief Função para aplicar a velocidade linear e a velocidade angular nos motores.
     * 
     * @param velocidadeEsquerda Varia no intervalo [-255, 255]. Utilize o valor 0 para zerar a velocidade dos motores esquerdos;
     * @param velocidadeDireita Varia no intervalo [-255, 255]. Utilize o valor 0 para zerar a velocidade dos motores direitos.
     */
    void dirigir(int velocidadeEsquerda, int velocidadeDireita) {

        // motorDireito.setSpeed(abs(velocidadeDireita));
        // motorEsquerdo.setSpeed(abs(velocidadeEsquerda));

        // Atualização do movimento do motor direito
        if (velocidadeDireita >=10){
            motorDireito.forward();
        }
        else if (velocidadeDireita < 10 && velocidadeDireita >= -10){
            motorDireito.stop();
        }
        else{
            motorDireito.backward();
        }
        // Atualização do movimento do motor esquerdo
        if (velocidadeEsquerda >=10){
            motorEsquerdo.forward();
        }
        else if (velocidadeEsquerda < 10 && velocidadeEsquerda >= -10){
            motorEsquerdo.stop();
        }
        else{
            motorEsquerdo.backward();
        }
    }
    
    // Função para parar os motores
    void parar() {
        motorDireito.stop();
        motorEsquerdo.stop();
    }
} // namespace locomocao
