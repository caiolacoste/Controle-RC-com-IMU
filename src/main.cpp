/*
  Código do ESP32 do robô Atlas.
*/

#include <Arduino.h>
#include <PS4Controller.h> // Biblioteca PS4-esp32 por aed3

#include <locomocao/locomocaoNova.hpp>
#include <sensoriamento/IMU.hpp>
#include <controlePS4/controlePS4.hpp>

void setup() {

  Serial.begin(9600);

  // --------------------------------- FUNÇÕES INICIALIZADORAS --------------------------------

  // Inicia a comunicação com o controle
  controlePS4::initControlePS4();

  // Inicia a tomada de dados do IMU
  IMU::initIMU();

  // --------------------------------- FUNÇÕES pinMode ----------------------------------------

  // Configura o pino do LED interno como OUTPUT
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
}

void loop() {

  // Atualiza os valores angulares com o IMU
  IMU::updateGyro();

  // Verifica se o controle de PS4 está conectado ao ESP
  if (PS4.isConnected()) {
    
    // Funções que verificam o modo de operação (autônomo ou RC)
    // e se a locomoção está travado ou não
    controlePS4::verificaTravaDeLocomocao();
    controlePS4::verificaModoDeOperacao();

    // Altera a cor no controle de acordo com o estado do robô (travado/destravado)
    controlePS4::verificaCorDoControle();


    // Verifica se a locomoção não esta travada
    if (controlePS4::travaLocomocao == false) {

      // Se o modo de operação é o RC, libera a movimentação
      if(controlePS4::modoDeOperacao == "rc"){
        controlePS4::verificaComandosDeControleRCsemIMU();
      }
      else if(controlePS4::modoDeOperacao == "rc_comIMU"){
        controlePS4::verificaComandosDeControleRCcomIMU();
      }

    }
    else{
      locomocao::parar();
    }
  }
  else {
    locomocao::parar();
  }
}

