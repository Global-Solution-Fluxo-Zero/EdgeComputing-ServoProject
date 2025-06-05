# Sistema de Controle de Escotilha para Drenagem de Enchentes

## Visão Geral da Solução

Este projeto implementa um sistema embarcado para o controle automatizado de uma escotilha, representada por um servo motor. O objetivo principal é gerenciar o fluxo de água, por exemplo, para drenagem em cenários de enchente.

O sistema utiliza um sensor ultrassônico para monitorar continuamente a distância até uma superfície de referência. No contexto de uma enchente, o sensor é posicionado acima da área propensa a alagamento, e a "distância" medida diminui à medida que o nível da água sobe.

**Funcionamento Básico:**
1.  **Calibração Inicial:** Ao ser ligado, o sistema mede a distância atual do sensor até o "chão" (ou a superfície abaixo, antes da água subir). Com base na altura de perigo definida (`RAIN_CM_DANGER`), ele calcula um limite (`floor_limit`). Se a distância medida pelo sensor cair abaixo desse `floor_limit`, significa que o nível da água subiu perigosamente.
2.  **Modo Automático:**
    *   Se a distância medida pelo sensor for *menor* que o `floor_limit` (indicando que o nível da água subiu até a zona de perigo), o sistema aciona um alerta sonoro (buzzer), um alerta visual (LED) e **fecha a escotilha** (servo motor move-se para a posição 0 graus), conforme a lógica atual do código.
    *   Se a distância medida for *maior* que o `floor_limit` (nível da água seguro), o LED permanece apagado e a **escotilha é aberta** (servo motor move-se para a posição 90 graus).
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

4.  **Testar o Modo Automático:**
    *   Localize o sensor ultrassônico (`ultrasonic1`) no diagrama.
    *   Clique sobre ele. Um controle deslizante ("distance") aparecerá. Este valor representa a distância entre o sensor e a superfície abaixo dele (que seria a água).
    *   **Calibração Automática:** Na inicialização, o sensor faz uma leitura da distância inicial (ex: 400cm, o padrão no Wokwi se não alterado antes de iniciar). Se `RAIN_CM_DANGER = 5`, o `floor_limit` será `400 - 5 = 395 cm`. Isso significa que se a distância medida pelo sensor cair para *menos de 395cm*, o nível da água subiu mais de 5cm.
    *   **Simulando Enchente (Água Sobe):** Mova o controle deslizante "distance" para um valor *menor* que o `floor_limit` calculado (ex: para 390 cm). Isso simula a água subindo, diminuindo a distância até o sensor.
    *   **Observar:** Conforme o código atual, o buzzer deve soar, o LED deve acender e o servo motor deve mover-se para a posição de "escotilha fechada" (0 graus).
    *   **Simulando Nível Seguro (Água Baixa):** Mova o controle deslizante "distance" para um valor *maior* que o `floor_limit`.
    *   **Observar:** O buzzer deve parar, o LED deve apagar e o servo motor deve mover-se para a posição de "escotilha aberta" (90 graus).

5.  **Testar o Modo Manual:**
    *   Com a simulação em execução, clique no pushbutton (`btn1`).
    *   **Observar:** O LED deve acender (indicando modo manual). O servo motor deve inverter sua posição atual (se estava em 0 graus, vai para 90 graus; se estava em 90, vai para 0). Cada clique no botão alternará a posição.

**Nota sobre a Lógica da Escotilha e o Objetivo de Drenagem:**

Você está correto na sua observação sobre o funcionamento do sensor: quando a água sobe, a distância medida pelo sensor diminui.
*   O código atual, ao detectar que a água subiu (`readDistanceCM() < floor_limit`), executa `closeTrapdoor()`.
*   Para o contexto de "abrir uma escotilha para drenar água de enchentes", a ação esperada quando a água atinge um nível perigoso (distância medida diminui) seria **abrir** a escotilha para permitir a drenagem.
*   Portanto, se o objetivo primário é drenar a água acumulada, a lógica no `loop()` precisaria ser invertida:
    *   Quando `readDistanceCM() < floor_limit` (água alta, perigo), chamar `openTrapdoor()`.
    *   Caso contrário (água baixa, seguro), chamar `closeTrapdoor()` (ou mantê-la fechada, dependendo do caso de uso).

A implementação atual do código se comportaria mais como um sistema que visa *impedir a entrada* de água externa quando esta sobe, ao invés de drenar água acumulada internamente. Para a funcionalidade de drenagem descrita, essa inversão na ação do servo seria necessária.
