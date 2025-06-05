# Sistema de Controle de Escotilha para Drenagem de Enchentes

## Visão Geral da Solução

Este projeto implementa um sistema embarcado para o controle automatizado de uma escotilha, representada por um servo motor. O objetivo principal é **abrir a escotilha para drenar água** quando o nível atinge um ponto crítico em cenários de enchente.

O sistema utiliza um sensor ultrassônico para monitorar continuamente a distância até uma superfície de referência. No contexto de uma enchente, o sensor é posicionado acima da área propensa a alagamento, e a "distância" medida diminui à medida que o nível da água **sobe**.

**Funcionamento Básico (para Drenagem):**
1.  **Calibração Inicial:** Ao ser ligado, o sistema mede a distância atual do sensor até o "chão" (ou a superfície abaixo, antes da água subir). Com base na altura de perigo definida (`RAIN_CM_DANGER`), ele calcula um limite (`floor_limit`). Se a distância medida pelo sensor cair abaixo desse `floor_limit`, significa que o nível da água **subiu** perigosamente, indicando a necessidade de drenagem.
2.  **Modo Automático (Drenagem):**
    *   Se a distância medida pelo sensor for *menor* que o `floor_limit` (indicando que o nível da água **subiu** até a zona de perigo), o sistema aciona um alerta sonoro (buzzer), um alerta visual (LED) e **abre a escotilha** para permitir a drenagem (servo motor move-se para a posição 90 graus).
    *   Se a distância medida for *maior* que o `floor_limit` (nível da água **desceu**, tornando a drenagem menos crítica ou desnecessária), o LED permanece apagado e a **escotilha é fechada** (servo motor move-se para a posição 0 graus).
3.  **Modo Manual:** Um botão permite ao usuário intervir e alternar manualmente a posição da escotilha (abrir se estiver fechada, fechar se estiver aberta). O LED acende para indicar que o modo manual foi ativado.

## Componentes Utilizados

Conforme o `diagram.json` e o código:
*   Arduino Uno
*   1x Servo Motor (SG90 ou similar)
*   1x Sensor Ultrassônico HC-SR04
*   1x LED (Vermelho)
*   1x Buzzer
*   1x Pushbutton (Botão de pulso)
*   1x Resistor de 220Ω (para o LED)
*   Protoboard e fios de conexão

## Conexões Principais (Pinos do Arduino)

*   **LED:** Pino 13
*   **Buzzer:** Pino 12
*   **Sensor Ultrassônico:**
    *   `TRIG`: Pino 3
    *   `ECHO`: Pino 2
*   **Servo Motor:** Pino 5
*   **Botão (Pushbutton):** Pino 7

As conexões de alimentação (5V e GND) são distribuídas pela protoboard conforme o `diagram.json`.

## Como Executar e Testar no Simulador (Wokwi)
Link do projeto: <a src="https://wokwi.com/projects/432609669649223681">https://wokwi.com/projects/432609669649223681</a>
1.  **Carregar o Projeto:**
    *   Abra o Wokwi.
    *   Carregue o arquivo `diagram.json` (isso montará o circuito).
    *   Cole o código fornecido no editor de código do Arduino Uno.

2.  **Definir o Nível de Perigo (Opcional):**
    *   No código, a constante `RAIN_CM_DANGER` define a "espessura" da camada de água (em centímetros), medida a partir do chão, que é considerada perigosa. O valor padrão é `5`.
        ```c++
        #define RAIN_CM_DANGER 5 //especifica em CM o nível que deve acionar a segurança
        ```
    *   Você pode alterar este valor antes de iniciar a simulação para testar diferentes limiares.

3.  **Iniciar a Simulação:**
    *   Clique no botão "Start the simulation" (ícone de play).

4.  **Testar o Modo Automático (para Drenagem):**
    *   Localize o sensor ultrassônico (`ultrasonic1`) no diagrama.
    *   Clique sobre ele. Um controle deslizante ("distance") aparecerá. Este valor representa a distância entre o sensor e a superfície abaixo dele (que seria a água).
    *   **Calibração Automática:** Na inicialização, o sensor faz uma leitura da distância inicial (ex: 400cm, o padrão no Wokwi se não alterado antes de iniciar). Se `RAIN_CM_DANGER = 5`, o `floor_limit` será `400 - 5 = 395 cm`. Isso significa que se a distância medida pelo sensor cair para *menos de 395cm*, o nível da água **subiu** mais de 5cm.
    *   **Simulando Enchente (Água Sobe, Necessidade de Drenagem):** Mova o controle deslizante "distance" para um valor *menor* que o `floor_limit` calculado (ex: para 390 cm). Isso simula a água subindo, diminuindo a distância até o sensor.
    *   **Observar (Comportamento Esperado para Drenagem):** O buzzer deve soar, o LED deve acender e o servo motor deve mover-se para a posição de "escotilha **aberta**" (90 graus) para permitir a drenagem. *(Veja a Nota abaixo sobre como ajustar o código para este comportamento).*
    *   **Simulando Nível Seguro (Água Desce):** Mova o controle deslizante "distance" para um valor *maior* que o `floor_limit`.
    *   **Observar (Comportamento Esperado para Drenagem):** O buzzer deve parar, o LED deve apagar e o servo motor deve mover-se para a posição de "escotilha **fechada**" (0 graus).

5.  **Testar o Modo Manual:**
    *   Com a simulação em execução, clique no pushbutton (`btn1`).
    *   **Observar:** O LED deve acender (indicando modo manual). O servo motor deve inverter sua posição atual (se estava em 0 graus, vai para 90 graus; se estava em 90, vai para 0). Cada clique no botão alternará a posição.

6. **Mídias do Projeto:**
   *   Youtube:
   *   Imagem:
   <img src="https://github.com/Global-Solution-Fluxo-Zero/EdgeComputing-ServoProject/blob/main/projetoesquema.png?raw=true">
