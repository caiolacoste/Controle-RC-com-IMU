/*
    Arquivo responsável pelo controle de PS4
*/

#include <PS4Controller.h> // Biblioteca PS4-esp32 por aed3
#include <locomocao/locomocaoNova.hpp>
#include <sinalizacao/fitaDeLED.hpp> // Arquivo com função para ligar LED
#include <sensoriamento/IMU.hpp>
#include <cmath> // Para poder utilizar arctan

#pragma once


namespace controlePS4 {

    // Váriaveis para as velocidades linear e angular do robô no RC
    int velocidadeDireita;
    int velocidadeEsquerda;
    // Variáveis para pegar os valores direto dos analógicos do controle
    int analogLX;
    int analogLY;
    int analogRX;
    int analogRY;
    // Variáveis para cálculo do ângulo do analógico
    double eixoX;
    double eixoY;
    double anguloJoystick = 0.;


    // Número MAC do controle do Renan para conexão com o controle
    const char *ENDERECO_MAC = "00:1a:7d:da:71:13"; // dourado

    // Váriavel para travar ou destravar o movimento do robô nos modos
    // autônomo e RC 
    bool travaLocomocao = true;
    // Variável utilizada para indicar se devo dar o tranco ao parar
    bool carroAndando = false;

    // String que guarda o modo em que o robô está (autônomo ou RC)
    String modoDeOperacao = "rc";

    //
    void initControlePS4 () {

        PS4.begin(ENDERECO_MAC);

        PS4.attachOnConnect([] {
            PS4.setLed(255, 0, 0);

            PS4.sendToController();
        });
    }

    void verificaTravaDeLocomocao() {

        if (PS4.Share()) {
            travaLocomocao = true;
        }
        else if (PS4.Options()) {
            travaLocomocao = false;       
        }
    }

    // Função que altera a cor da luz traseira do controle
    void verificaCorDoControle() {

        if (travaLocomocao == false) {
            PS4.setLed(0, 255, 0);
            PS4.sendToController();
        }

        else {
            PS4.setLed(255, 0, 0);
            PS4.sendToController();            
        }
    }
    // Função que lê os botões de ombro e altera a variável modoDeOperacao
    void verificaModoDeOperacao() {

        if (PS4.R1()) {

            modoDeOperacao = "rc";
            PS4.setLed(255, 0, 255);
            PS4.sendToController();
            vTaskDelay(pdMS_TO_TICKS(100));
        }

        else if (PS4.L1()) {

            modoDeOperacao = "rc_comIMU";
            PS4.setLed(0, 0, 255);
            PS4.sendToController();
            vTaskDelay(pdMS_TO_TICKS(100));
        }      
    }

    double mapDouble(double x, double in_min, double in_max, double out_min, double out_max) {
        const double run = in_max - in_min;
        if(run == 0){
            log_e("map(): Invalid input range, min == max");
            return -1; // AVR returns -1, SAM returns 0
        }
        const double rise = out_max - out_min;
        const double delta = x - in_min;
        return (delta * rise) / run + out_min;
    }

    // Função que lê os valores dos analógicos e chama função de locomoção
    void verificaComandosDeControleRCsemIMU() {

        analogRY = PS4.RStickY();
        analogLY= PS4.LStickY();
        analogRX = PS4.RStickX();
        analogLX= PS4.LStickX();

        velocidadeDireita = map(analogRY, -128, 127, -255, 255);
        velocidadeEsquerda= map(analogLY, -128, 127, -255, 255);

        Serial.println("velocidade direita: "+ String(velocidadeDireita)+", velocidade esquerda: "+ String(velocidadeEsquerda));

        locomocao::dirigir(velocidadeEsquerda, velocidadeDireita);

        if (PS4.Cross()) {
            digitalWrite(2, HIGH);
        }
        else if (PS4.Circle()) {
            digitalWrite(2, LOW);
        }
    }
    // Função que utiliza o analógico esquerdo para rodar o robô e os botões L2/R2 para mover linearmente
    void verificaComandosDeControleRCcomIMU() {

        analogRY = PS4.RStickY();
        analogLY= PS4.LStickY();
        analogRX = PS4.RStickX();
        analogLX= PS4.LStickX();

        eixoY = mapDouble(analogLY, -128,127,-1,1);
        eixoX = mapDouble(analogLX, -128,127,-1,1);
        // Serial.println("eixo X: " + String(eixoX) + ", eixo Y: " + String(eixoY));

        if (abs(eixoX) >= 0.8 || abs(eixoY) >= 0.8){
            anguloJoystick = atan2(eixoY,eixoX) * 180 / M_PI;
        }
        Serial.print("Angulo do controle: ");
        Serial.print(anguloJoystick);
        Serial.print(", ângulo do IMU: ");
        Serial.println(IMU::anguloNormalizado);

        // Atualiza velocidade linear com os botões L2 e R2
        if (PS4.R2() || PS4.L2()){
            // Prevalece o comando para ir para frente
            if (PS4.R2Value() >= 50){
                velocidadeDireita = 255;
                velocidadeEsquerda = 255;
            }
            // Depois lê se vai para trás
            else if (PS4.L2Value() >= 50){
                velocidadeDireita = -255;
                velocidadeEsquerda = -255;
            }
        }
        else{
            velocidadeDireita = 0;
            velocidadeEsquerda = 0;
        }

        locomocao::dirigir(velocidadeEsquerda, velocidadeDireita);

        if (PS4.Cross()) {
            digitalWrite(2, HIGH);
        }
        else if (PS4.Circle()) {
            digitalWrite(2, LOW);
        }
    }

}