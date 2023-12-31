# Controle RC para robô utilizando sensor IMU

[TOC]

## Introdução

O cenário atual de radio controle na robótica, nas categorias combate e sumô, por exemplo, se assemelha a jogos dos anos 1990 e 2000, como Resident Evil e adventure games, em que se utilizava o **controle de tanque** para movimentar o personagem no cenário. 

O controle de tanque consiste em movimentar o personagem com base somente na posição do personagem (ou robô). Em outras palavras, quando aponta para a direita no analógico, o personagem virará para a direita dele, independente da posição da câmera, então se o personagem estiver virado para a câmera e o jogador colocar o analógico para direita, ele virará para a esquerda do jogador!

![image](https://github.com/caiolacoste/Controle-RC-com-IMU/assets/103116466/cddcbf2e-b770-4d9d-b29c-c53d645fb6f1)
Resident Evil 3, 1999.

Mas esse controle é considerado ultrapassado em jogos atuais, visto que agora o personagem sempre se movimenta baseado na posição da câmera em relação ao personagem, ou seja, quando o jogador coloca o analógico em uma posição, o personagem vai para aquela posição, independente da direção que ele estava voltado inicialmente. Isso deixa o controle muito mais responsivo e agradável para o jogador, que não precisa parar para pensar como o personagem está para saber em qual direção mover!


