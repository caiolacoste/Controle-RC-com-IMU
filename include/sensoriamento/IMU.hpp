/*
    Cabeçalho responsável pela implementação do sensor IMU, para utilizar o giroscópio no controle RC.
*/

#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <MPU6050_tockn.h>

namespace IMU {

    MPU6050 mpu6050(Wire);

    // Variáveis que guardam a orientação do IMU
    int angleX;
    int angleY;
    int angleZ;
    // Variável do ângulo do carro depois de passar para o intervalo [-180,180]
    int anguloNormalizado;
    
    // Define o offset do sensor. Deve ser chamada no setup()
    void initIMU() {
        Wire.begin();
        mpu6050.begin();
        // Permite que a posição ao iniciar seja o referencial zero
        mpu6050.calcGyroOffsets(true);
    }

    // Função para normalizar angulo do IMU, deixando no intervalo [-180, 180]
    int normalizarAngulo(int angulo){
        int anguloNormalizado = angulo % 360; //Reduz o valor para o intervalo [0, 360)

        if (anguloNormalizado >= 180){
            anguloNormalizado -= 360; // Converte valores acima de 180 para valores negativos correspondentes
        }
        else if (anguloNormalizado <= -180){
            anguloNormalizado += 360; // Converte valores acima de 180 para valores negativos correspondentes
        }

        return anguloNormalizado;
    }

    // Atualiza a orientação
    void updateGyro() {
        mpu6050.update();
        // Printa os ângulos em graus
        angleX = mpu6050. getAngleX();
        // Serial.print("angleX: ");
        // Serial.print(angleX);

        angleY = mpu6050.getAngleY();
        // Serial.print("\tangleY: ");
        // Serial.print(angleY);

        angleZ = mpu6050.getAngleZ();
        anguloNormalizado = normalizarAngulo(angleZ+90);

        // Serial.print("\tangleZ: ");
        // Serial.print(angleZ);
    }

} // namespace IMU
