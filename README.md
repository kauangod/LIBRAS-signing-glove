# Luva Tradutora de LIBRAS / LIBRAS Signing Glove
Este projeto foi desenvolvido como parte da disciplina Oficina de Integração 1, oferecida no terceiro período do curso de Engenharia de Computação da Universidade Tecnológica Federal do Paraná (UTFPR). O principal objetivo foi criar uma luva capaz de reconhecer as letras do alfabeto em LIBRAS (Língua Brasileira de Sinais) gesticuladas pelo usuário, e exibi-las na tela de um dispositivo móvel. O código para controlar a luva foi escrito utilizando a Arduino IDE, empregando uma linguagem próxima ao C++. Esse sistema buscou facilitar a comunicação entre pessoas com deficiência auditiva e ouvintes, oferecendo uma interface prática e acessível.

English version: This project was developed as part of the Integration Workshop 1 course, offered in the fourth semester of the Computer Engineering course at the Federal University of Technology – Paraná (UTFPR). The main objective was to create a glove capable of recognizing letters of the alphabet in LIBRAS (Brazilian Sign Language) gestured by the user, and displaying them on a mobile device screen. The code to control the glove was written using the Arduino IDE, employing a language similar to C++. This system aims to facilitate communication between deaf individuals and hearing people by providing a practical and accessible interface.

<p align="center">
  <img src="https://github.com/kauangod/LIBRAS-signing-glove/blob/main/Images/glove.png" width="351" height="200" />
</p>

## Principais Componentes / Main Components
- ESP32;
- 5 Sensores de flexão / Flex sensors;
- 2 Sensores de contato / Contact sensors;
- 1 Chave táctil / Push button.

## Identificação das Letras / Letter Identification
A identificação das letras foi realizada utilizando o metódo de árvore de decisão. A mesma pode ser encontrada abaixo.

English: The identification of the letters was performed using the decision tree method. The tree can be seen below.

<p align="center">
  <img src="https://github.com/kauangod/LIBRAS-signing-glove/blob/main/Images/tree.png" width="546" height="300" />
</p>

## Vídeo de Demonstração / Showcase Video
Como parte do projeto, um vídeo demonstrando o funcionamento foi requisitado. Você pode assistir a ele clicando [aqui](https://youtu.be/lgx5FzQMqeU).

English: As part of the project, a video showcasing its funcionality was required. You may watch it by clicking [here](https://youtu.be/lgx5FzQMqeU) (video in Brazilian Portuguese).


## Colaboradores / Collaborators
- Kauan Godoy Nunes
- Pedro Henrique Chagas Fallgatter
- Omar Zagonel El Laden
