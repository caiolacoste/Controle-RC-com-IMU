# Controle RC para robô utilizando sensor IMU

[TOC]

## Introdução

O cenário atual de radio controle na robótica, nas categorias combate e sumô, por exemplo, se assemelha a jogos da década de 90, como Resident Evil e adventure games, em que se utilizava o **controle de tanque** para movimentar o personagem no cenário. 

O controle de tanque consiste em movimentar o personagem com base somente na posição do personagem (ou robô). Em outras palavras, quando aponta para a direita no analógico, o personagem virará para a direita dele, independente da posição da câmera, então se o personagem estiver virado para a câmera e o jogador colocar o analógico para direita, ele virará para a esquerda do jogador!

![8839ac6b6e08fab30a6e9fc15090c483](https://github.com/caiolacoste/Controle-RC-com-IMU/assets/103116466/ecf90d29-f4d8-4db0-8f89-bcb6d583e441)

Resident Evil, 1996.

Mas esse controle é considerado ultrapassado em jogos atuais, visto que agora o personagem sempre se movimenta baseado na posição em relação à câmera, ou seja, quando o jogador coloca o analógico em uma posição, o personagem vai para aquela posição, independente da direção que ele estava voltado inicialmente. Isso deixa o controle muito mais responsivo e agradável para o jogador, que não precisa parar para pensar como o personagem está para saber em qual direção mover! 

![mgs1_grayfox_04](https://github.com/caiolacoste/Controle-RC-com-IMU/assets/103116466/cca66270-d52e-4c6d-b895-b8fba8897712)

Metal Gear Solid, 1999. Um dos primeiros a aderir a tal controle.

### Objetivo

O objetivo desse repositório é introduzir tal tipo de controle com um código para ESP32, sendo possível de ser implementado utilizando o sensor IMU. O código também oferece a opção de controle tanque, alternando facilmente com L1/R1 entre o antigo e o novo.






