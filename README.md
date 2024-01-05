# Controle RC para robô utilizando sensor IMU

## Introdução

O cenário atual de radio controle na robótica, nas categorias combate e sumô, por exemplo, se assemelha a jogos da década de 90, como Resident Evil e adventure games, em que se utilizava o **controle de tanque** para movimentar o personagem no cenário. 

O controle de tanque consiste em movimentar o personagem com base somente na posição do personagem (ou robô). Em outras palavras, quando aponta para a direita no analógico, o personagem virará para a direita dele, independente da posição da câmera, então se o personagem estiver virado para a câmera e o jogador colocar o analógico para direita, ele virará para a esquerda do jogador!

![8839ac6b6e08fab30a6e9fc15090c483](https://github.com/caiolacoste/Controle-RC-com-IMU/assets/103116466/ecf90d29-f4d8-4db0-8f89-bcb6d583e441)

Resident Evil, 1996.

Mas esse controle é considerado ultrapassado em jogos atuais, visto que agora o personagem sempre se movimenta baseado na posição em relação à câmera, ou seja, quando o jogador coloca o analógico em uma posição, o personagem vai para aquela posição, independente da direção que ele estava voltado inicialmente. Isso deixa o controle muito mais responsivo e agradável para o jogador, que não precisa parar para pensar como o personagem está para saber em qual direção mover! 

![JZ_SkC](https://github.com/caiolacoste/Controle-RC-com-IMU/assets/103116466/3ce72be1-0bae-42d5-b70b-5ee22adda78b)

Metal Gear Solid, 1999. Um dos primeiros a aderir a tal controle.

### Objetivo

O objetivo desse repositório é introduzir tal tipo de controle com um código para ESP32, sendo possível de ser implementado utilizando o sensor IMU. O código também oferece a opção de controle tanque, alternando facilmente com L1/R1 entre o antigo e o novo.

## Metodologia

### Hardware
Foi utilizado um robô de 4 rodas (dá pra achar ele pesquisando robot 4WD) em que os 2 motores de cada lado são curtados de forma a fazer o mesmo movimento. Eles são conectados em uma ponte H L298N e sua energia vem de um pack de pilhas (4x1.2V=4.8V). O controle é feito por um ESP32. Com objetivo de simplificação, não foram utilizados os pinos enable da ponte H, porque só andar e parar já é suficiente (não precisa regular a velocidade). Para alimentar o ESP, foi utilizado o mesmo conjunto de baterias.

Na parte sensorial, o IMU foi conectado no ESP, sendo alimentado pelos pinos 3V3 e GND, e para a comunicação I2C foram utilizados os pinos D21 e D22, referentes aos pinos SDA e SCL, respectivamente.

![image](https://github.com/caiolacoste/Controle-RC-com-IMU/assets/103116466/d7dc6958-484a-40f7-887d-64cf36c1ca59)

### Funções de locomoção

Primeiramente, devemos importar a biblioteca **L298N**, responsável por fazer a conexão com o sensor. 

![image](https://github.com/caiolacoste/Controle-RC-com-IMU/assets/103116466/7c4b85c3-b33c-45c3-9899-85f24500170a)

Começamos declarando os pinos da ponte H e chamando as bibliotecas:

```cpp
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

namespace locomocao {
  // Definição dos objetos dos motores
  L298N motorEsquerdo(IN1,IN2);
  L298N motorDireito(IN3,IN4);
```

E são criadas duas funções de locomoção: Uma que recebe a velocidade de cada lado e manda para os motores e outra para simplesmente parar os motores, que é chamada caso o controle desconecte ou aperte o botão de failsafe.
```cpp
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
```


### Recebendo valores do IMU

Primeiramente, devemos importar a biblioteca **MPU6050_tockn**, responsável por fazer a conexão com o sensor. 

![image](https://github.com/caiolacoste/Controle-RC-com-IMU/assets/103116466/fdea694c-df27-4fe7-934c-106c7af7dcb8)

Para organização do projeto, foi criado um arquivo IMU.hpp contendo todas as funções que mexem com esse sensor. Começamos criando o namespace IMU e são declaradas as variáveis para guardar os ângulos nos 3 eixos (poderia fazer só o eixo Z, mas deixei todos). A variável anguloNormalizado é importante porque o cálculo do ângulo pelo IMU é com base na variação angular do sensor, então valores são acrescentados ou retirados conforme tem o movimento, fazendo com que pegue valores maiores que 360, por exemplo.

```cpp
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
```

É criada a função de inicialização, chamada no setup da main. Com a fuunção calcGyroOffsets, definimos o ângulo inicial como o referencial zero para fazer o cáculo do ângulo posteriormente:

```cpp
void initIMU() {
    Wire.begin();
    mpu6050.begin();
    // Permite que a posição ao iniciar seja o referencial zero
    mpu6050.calcGyroOffsets(true);
}
```

A função normalizarAngulo serve para pegar o ângulo vindo do IMU, que pode ter valores no intervalo (-infinito,+infinito) e retonra o ângulo no intervalo [-180,180]. Isso é feito de maneira simples pegando o valor e vendo o resto da divisão com 360, para ver valores somente no intervalo [-360,360] e depois faz uma subtração e soma para que acima de 180 vire -180 e abaixo de -180 vire 180!

```cpp
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
```

A outra função referente é a updateGyro, que utiliza a função getAngle...() da biblioteca para atualizar os valores de cada eixo. Essa função é chamada no loop da main!

```cpp
// Atualiza a orientação
void updateGyro() {
    mpu6050.update();
    // Printa os ângulos em graus
    angleX = mpu6050. getAngleX();
    angleY = mpu6050.getAngleY();
    angleZ = mpu6050.getAngleZ();
    // Normaliza o ângulo para o intervalo [-180,180]
    anguloNormalizado = normalizarAngulo(angleZ+90);
}
```

Nota-se que tem uma soma de 90 no ângulo original porque o controle de PS foi pensado para começar alinhado com o robô virado para frente, que é o equivalente a 90° no plano cartesiano.

### Recebendo comandos do controle

Como o foco está no IMU, os comandos referentes ao controle de PS4 não serão explicados. Seus comandos são simples, em que é especificado o endereço MAC do controle, no setup é tentada a conexão e depois na main as funções de locomoção são chamadas somente se o controle está conectado.

Agora com o ângulo do IMU tratado e com as funções de locomoção, é feita a função verificaComandosDeControleRCcomIMU, que deve ser chamada na main que basicamente atualiza o ângulo do joystick, calcuula o erro com base no ângulo do IMU e envia a velocidade para os motores corrigirem a orientação. Além disso, o movimento linear (frente e trás) é feito com os botões L2/R2.

Começamos calculando o ângulo do controle. Para isso é utilizado o joystick esquerdo. Com o valor do analógico no eixo X e Y, é posssível encontrar a direção. Como o valor vai de -128 até 127, primeiro é feita uma regra de três para transformarr esse intervalo em [-1,1]. Depois disso é utilizada a função atan2 (tem que usar #include <cmath> no topo do arquivo) para calcular o ângulo referente. atan2 é melhor que atan porque ele sabe se está no primeiro ou no terceiro e no segundo ou no quarto quadrante, por exemplo.

```cpp
analogRY = PS4.RStickY();
analogLY= PS4.LStickY();
analogRX = PS4.RStickX();
analogLX= PS4.LStickX();

eixoY = mapDouble(analogLY, -128,127,-1,1);
eixoX = mapDouble(analogLX, -128,127,-1,1);

// Atualiza o ângulo do joystick caso ele seja direcionado em alguma direção (valor maior que 0.8)
if (abs(eixoX) >= 0.8 || abs(eixoY) >= 0.8){
    anguloJoystick = atan2(eixoY,eixoX) * 180 / M_PI;
}
```

Finalmente é calculado o erro com base nesse ânguulo e no ângulo do IMU. Nota-se que o erro é a diferença entre o angulo desejado (joystick) e o ângulo real (IMU):

```cpp
// Atualiza o valor do erro com base no analógico e no carro (IMU)
erroAngular = anguloJoystick - IMU::anguloNormalizado;
```

Antes de corrigir o ângulo, são lidos os comandos para a velocidade linear. Só caso essa velocidade não seja alterada que o carro pode rodar:
```cpp
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
```
Caso não ande linearmente, zera a velocidade dos dois lados, para evitar que o carro continue andando se não precisar rodar. 

```cpp
// Caso não esteja indo para frente ou trás, faz o cálculo para rotação
else{
    velocidadeDireita = 0;
    velocidadeEsquerda = 0;
```

Como o ângulo pode passar de 180 para -180 e vice-versa, é necessária uma lógica robusta para evitar que o carro gire simplesmente pelo valor do erro, mas que ele reconheça qual dos lados está mais perto para se movimentar. Isso é feito da seguinte forma:

- Caso o erro seja maior que 180 (módulo), significa que o sentido de diminuir o erro não é o mais otimizado, e portanto ele deve procurar aumentar esse erro, ou seja, a diferença de ângulo do outro lado é menor! No caso do erro menor que 180, significa que diminuindo o erro já é o percurso mais otimizado a se fazer.

Dessa forma, é feito um if/else para ver esse valor e dependendo do lado, o carro vai para um sentido ou para o outro:

```cpp
// Caso o erro seja maior que 180, ele precisa andar para aumentar o erro
if (abs(erroAngular) > 180){
    if (erroAngular >=0){
        // Vira no sentido horário
        velocidadeEsquerda = 255;
        velocidadeDireita = -255;
    }
    else {
        // Vira no sentido anti-horário
        velocidadeEsquerda = -255;
        velocidadeDireita = 255;
    }
}
// Caso o erro seja menor que 180, ele precisa andar para diminuir o erro
else{
    if (abs(erroAngular) >=5 && erroAngular >= 0){
        // Vira no sentido anti-horário
        velocidadeDireita = 255;
        velocidadeEsquerda = -255;
    }
    else if (abs(erroAngular) >=5 && erroAngular < 0){
        // Vira no sentido horário
        velocidadeDireita = -255;
        velocidadeEsquerda = 255;
    }
}
```

Depois de atualizadas as velocidades, os valores são enviados para os motores com a função de locomoção

```cpp
locomocao::dirigir(velocidadeEsquerda, velocidadeDireita);
```

## Resultados

Os resultados no robô podem ser visto no vídeo abaixo:

https://youtu.be/ffggiAu96MU?si=m8NaGYVfthXrOxeu

## Conclusões

Mesmo sendo um sucesso na utilização do sensor, tal controle ainda precisa de otimizações para se tornar realmente competitivo para o cenário de combate. Entre as melhorias podemos citar:

- Botão para recalibrar a referência zero no meio da luta;
- Utilização dos outros eixos, além do Z, para saber se o robô está de cabeça para baixo, fazendo com que os comandos fiquem invertidos;
- Mixagem para que o robô possa andar para frente e trás ao mesmo tempo quue corrige a orientação.
