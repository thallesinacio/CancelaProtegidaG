# Cancela Protegida: Controle de Acesso com BitDogLab

## 1. Visão Geral

Este projeto foi desenvolvido como parte da Residência Tecnológica em Sistemas Embarcados e demonstra o controle de um atuador (servo motor) com a placa BitDogLab.  "Cancela Protegida" é um protótipo de um sistema de controle de acesso que utiliza um teclado matricial para a entrada de uma senha numérica, que, se correta, aciona um servo motor para simular a abertura e fechamento de uma cancela ou fechadura.


## 2. Funcionalidades

### Autenticação por Senha: O acesso é liberado apenas após a inserção de uma senha correta de 4 dígitos.

Controle de Atuador: Utiliza um servo motor de rotação contínua para simular o movimento mecânico de uma cancela.

Feedback ao Usuário: O sistema fornece feedback em tempo real através do monitor serial, informando as teclas pressionadas e se a senha está correta ou incorreta.

Estrutura de Firmware Robusta: O código é estruturado para ser legível, comentado e para lidar com debounce de teclas, garantindo uma leitura precisa. 

## 3. Componentes de Hardware

Placa de desenvolvimento BitDogLab (baseada no RP2040)

Teclado Matricial 4x4

Servo Motor de Rotação Contínua

Fonte de alimentação externa para o servo motor

Jumpers para conexões

## 4. Estrutura do Firmware

O firmware, escrito em C, utiliza o SDK do Raspberry Pi Pico e é dividido em várias partes lógicas:

Inicialização (main): Configura a comunicação serial, inicializa os pinos GPIO para o teclado e o módulo PWM para o servo.

Leitura do Teclado (keypad_get_key): Implementa uma rotina de varredura que ativa uma linha do teclado por vez e lê o estado das colunas para identificar a tecla pressionada. Inclui tratamento de debounce para evitar leituras múltiplas.

Controle do Servo: Utiliza o hardware de PWM do RP2040 para gerar um sinal de 50Hz. A largura do pulso é manipulada para controlar o servo:

1500µs: Comando para "parar".

1600µs: Comando para "girar em sentido horário" (abrir).

1400µs: Comando para "girar em sentido anti-horário" (fechar).

Lógica de Senha (main): O loop principal aguarda por entradas válidas do teclado, armazena os dígitos, e ao receber 4 dígitos, realiza a comparação com a senha mestra.

## 5. Como Compilar e Usar

Configuração do Ambiente:

Certifique-se de ter o ambiente de desenvolvimento para o Raspberry Pi Pico (SDK) configurado em sua máquina.

Clone este repositório.

Compilação:

Crie um diretório build.

Acesse o diretório build e execute cmake .. seguido de make.

Upload:

Conecte a BitDogLab ao computador em modo bootloader (segurando o botão BOOTSEL).

Arraste o arquivo .uf2 gerado para o drive da placa.

Uso:

Abra um monitor serial para ver o feedback.

Digite a senha "5587" no teclado para acionar a cancela.

# 6. Vídeo de Demonstração

Para uma explicação detalhada e ver o projeto em funcionamento, acesse o vídeo de demonstração:
