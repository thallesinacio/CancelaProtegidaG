#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/pwm.h"
#include <string.h>


// Definição dos pinos
#define SERVO_PIN 8
#define ROWS 4
#define COLS 4
int teste[10];

// SERVO (pulsos em microssegundos)
#define SERVO_MIN_US   500    // ~0°
#define SERVO_MAX_US   2500   // ~180°
#define SERVO_ABERTO_DEG  90  // alvo para “aberto”
#define SERVO_FECHADO_DEG 0   // alvo para “fechado”


uint row_pins[ROWS] = {4, 9, 16, 17};     // GPIOs das linhas
uint col_pins[COLS] = {18, 19, 20, 28};     // GPIOs das colunas

// Mapeamento das teclas
char keys[ROWS][COLS] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};


void keypad_init() {
    // Configura linhas como saída
    for (int i = 0; i < ROWS; i++) {
        gpio_init(row_pins[i]);
        gpio_set_dir(row_pins[i], GPIO_OUT);
        gpio_put(row_pins[i], 1); // começa em HIGH
    }
    // Configura colunas como entrada com pull-up
    for (int j = 0; j < COLS; j++) {
        gpio_init(col_pins[j]);
        gpio_set_dir(col_pins[j], GPIO_IN);
        gpio_pull_up(col_pins[j]);
    }
}

int idx = 0;

// Função para varrer teclado (VERSÃO MAIS LIMPA E COMUM)
char keypad_get_key() {
    char pressed_key = 0;
    for (int i = 0; i < ROWS; i++) {
        gpio_put(row_pins[i], false);
        for (int j = 0; j < COLS; j++) {
            if (gpio_get(col_pins[j]) == 0) {
                sleep_ms(50);
                while(gpio_get(col_pins[j]) == 0);
                pressed_key = keys[i][j];
            }
        }
        gpio_put(row_pins[i], true);
    }
    // idx++;  // ⬅️ REMOVA ESTA LINHA DAQUI
    return pressed_key;
}

void set_servo() {
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 125.0f);
    pwm_config_set_wrap(&config, 19999);
    pwm_init(slice_num, &config, true);
}

char pswd[5];
char chave[5] = "5587";

bool compara_senha() {
    return strcmp(pswd, chave) == 0;
}



int main() {
    int pass_idx = 0; // Use uma variável local para o índice da senha
    char pswd[5];    // Buffer para a senha
    char chave[5] = "5587"; // Senha correta

    stdio_init_all();
    keypad_init();
    set_servo();

    // Inicia o servo na posição "parado"
    pwm_set_gpio_level(SERVO_PIN, 1500);
    sleep_ms(500);

    printf("Digite a senha de 4 digitos:\n");

    while (true) {
        // 1. CHAMA A FUNÇÃO DE LEITURA APENAS UMA VEZ POR LOOP
        char key = keypad_get_key();

        // 2. SÓ FAZ ALGO SE UMA TECLA REALMENTE FOI PRESSIONADA (key != 0)
        if (key != 0) {
            printf("Tecla: %c\n", key);

            // 3. ARMAZENA A TECLA E INCREMENTA O ÍNDICE
            if (pass_idx < 4) {
                pswd[pass_idx] = key;
                pass_idx++;
            }
        }

        // 4. VERIFICA A SENHA APENAS QUANDO 4 DÍGITOS FOREM INSERIDOS
        if (pass_idx == 4) {
            // 5. ADICIONA O NULL TERMINATOR ANTES DE COMPARAR
            pswd[4] = '\0';
            
            printf("Senha digitada: %s\n", pswd);

            if (strcmp(pswd, chave) == 0) {
                printf("Senha correta! Abrindo...\n");
                // Gira para abrir
                pwm_set_gpio_level(SERVO_PIN, 1600);
                sleep_ms(1050);
                pwm_set_gpio_level(SERVO_PIN, 1500);

                sleep_ms(2000); // Espera 2 segundos aberto

                printf("Fechando...\n");
                // Gira para fechar
                pwm_set_gpio_level(SERVO_PIN, 1400);
                sleep_ms(1050);
                pwm_set_gpio_level(SERVO_PIN, 1500);
            } else {
                printf("Senha incorreta!\n");
            }

            // 6. RESETA O ÍNDICE PARA PERMITIR UMA NOVA TENTATIVA
            pass_idx = 0;
            printf("\nDigite a senha de 4 digitos:\n");
        }
        
        // Pequeno delay para não sobrecarregar o processador
        sleep_ms(20); 
    }
}
