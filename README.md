# Sistema de Controle de Escotilha para Drenagem de Enchentes

|RM|Nome|
|---|---|
|563995|Azor Biagioni Tartuce|
|566284|Daniel Oliveira de Souza|
|562757|Lucas de Almeida Pires|

Este projeto foi desenvolvido para oferecer uma solução automatizada e inteligente para o controle de escotilhas em cenários de risco de enchentes. Enchentes podem causar danos significativos a propriedades e infraestruturas, e um sistema de drenagem eficiente é crucial para mitigar esses impactos.

Este dispositivo utiliza um sensor ultrassônico para monitorar o nível da água em tempo real, permitindo que a escotilha seja operada automaticamente para facilitar a drenagem quando necessário, ou controlada manualmente conforme a situação exigir.

**Link para o Wokwi:** [https://wokwi.com/projects/432609669649223681](https://wokwi.com/projects/432609669649223681) <br>
**Mídias do Projeto:**
*   **Vídeo Demonstrativo (YouTube):** [Link para seu vídeo aqui, se houver]
*   **Imagem do Circuito/Protótipo:**
  <img src="https://github.com/Global-Solution-Fluxo-Zero/EdgeComputing-ServoProject/blob/main/projetoesquema.png?raw=true">

---

## 🎯 Objetivo

O objetivo principal deste sistema é **automatizar a abertura de uma escotilha para facilitar a drenagem de água** quando o nível atinge um ponto crítico, ajudando a prevenir ou minimizar os danos causados por enchentes. Adicionalmente, oferece um modo de controle manual para intervenção direta.

---

## 🛠 Tecnologias e Componentes Utilizados

*   **Placa Controladora:** Arduino Uno (ou compatível)
*   **Atuador:** 1x Servo Motor (SG90 ou similar, para operar a escotilha)
*   **Sensor de Nível:** 1x Sensor Ultrassônico HC-SR04
*   **Sinalização Visual:** 1x LED (Vermelho)
*   **Sinalização Sonora:** 1x Buzzer
*   **Interface Manual:** 1x Pushbutton (Botão de pulso)
*   **Componente de Apoio:** 1x Resistor de 220Ω (para o LED)
*   **Montagem:** Protoboard e fios de conexão (Jumpers)

---

## 📦 Funcionalidades Principais

Este sistema embarcado oferece as seguintes funcionalidades:

### 🌊 Monitoramento Contínuo do Nível d'Água
*   Utiliza um sensor ultrassônico para medir constantemente a distância até a superfície da água.
*   À medida que o nível da água **sobe**, a distância medida pelo sensor **diminui**.
*   **Calibração Inicial:** Ao ser ligado, o sistema determina a distância base ("chão") e calcula um `floor_limit` (nível de perigo) com base no `RAIN_CM_DANGER` definido.

### ⚙️ Controle Automático da Escotilha (Modo Drenagem)
*   **Abertura para Drenagem:** Se a distância medida pelo sensor for *menor* que o `floor_limit` (indicando que o nível da água **subiu** até a zona de perigo), o sistema automaticamente **abre a escotilha** (servo motor move-se para 90 graus) para permitir a drenagem.
*   **Fechamento Pós-Drenagem:** Se a distância medida for *maior* que o `floor_limit` (indicando que o nível da água **desceu** e está seguro), a **escotilha é fechada** (servo motor move-se para 0 graus).

### 👆 Modo de Operação Manual
*   Um botão (pushbutton) permite ao usuário intervir e **alternar manualmente a posição da escotilha** (abrir se estiver fechada, fechar se estiver aberta).
*   Ideal para testes, manutenção ou situações que exigem controle direto.

### 🔊💡 Alertas Sonoros e Visuais
*   **Alerta de Perigo:** Quando o nível da água atinge a zona de perigo (acionando a abertura automática da escotilha), o sistema dispara um alerta sonoro (buzzer) e acende um LED, notificando sobre a condição de risco e a ação de drenagem em curso.
*   **Indicação de Modo Manual:** O LED também acende quando o modo manual é ativado pelo botão.

---

## 🔌 Conexões Principais (Pinos do Arduino)

Para referência rápida, as principais conexões são:
*   **LED:** Pino 13
*   **Buzzer:** Pino 12
*   **Sensor Ultrassônico:**
    *   `TRIG`: Pino 3
    *   `ECHO`: Pino 2
*   **Servo Motor (PWM):** Pino 5
*   **Botão (Pushbutton):** Pino 7

As conexões de alimentação (5V e GND) são distribuídas pela protoboard conforme o `diagram.json`.

---

## 🚀 Como Usar e Testar no Simulador (Wokwi)

1.  **Acessar o Projeto:**
    *   Abra o projeto no Wokwi através do link: [https://wokwi.com/projects/432609669649223681](https://wokwi.com/projects/432609669649223681)
    *   Se estiver montando do zero, carregue o arquivo `diagram.json` (para o circuito) e cole o código fornecido no editor do Arduino Uno.

2.  **Configurar o Limite de Perigo (Opcional):**
    *   No código, a constante `RAIN_CM_DANGER` define a "espessura" da camada de água (em centímetros), medida a partir do chão, que é considerada perigosa. O valor padrão é `5`.
        ```c++
        #define RAIN_CM_DANGER 5 //especifica em CM o nível que deve acionar a segurança
        ```
    *   Altere este valor, se desejar, para testar diferentes sensibilidades do sistema.

3.  **Iniciar a Simulação:**
    *   Clique no botão "Start the simulation" (ícone de play ▶️) no Wokwi.

4.  **Testar o Modo Automático (Drenagem):**
    *   Localize o sensor ultrassônico (`ultrasonic1`) no diagrama.
    *   Clique sobre ele para revelar o controle deslizante ("distance"). Este valor simula a distância entre o sensor e a superfície da água.
    *   **Calibração:** O sistema, ao iniciar, lê a distância configurada no sensor (padrão 400cm no Wokwi se não alterado antes do play). Se `RAIN_CM_DANGER = 5`, o `floor_limit` será `400 - 5 = 395 cm`.
    *   **Simulando Água Subindo (Ativar Drenagem):** Reduza o valor "distance" no sensor para *menos de 395 cm* (ex: 390 cm). Isso simula a água subindo.
        *   **Observar:** O buzzer deve soar, o LED deve acender, e a escotilha (servo) deve **abrir** (ir para 90 graus).
    *   **Simulando Água Descendo (Parar Drenagem):** Aumente o valor "distance" para *mais de 395 cm*.
        *   **Observar:** O buzzer deve parar, o LED deve apagar, e a escotilha deve **fechar** (ir para 0 graus).

5.  **Testar o Modo Manual:**
    *   Com a simulação em execução, clique no pushbutton (`btn1`).
    *   **Observar:** O LED deve acender. A escotilha deve inverter sua posição atual (de 0 para 90 graus, ou de 90 para 0 graus).

