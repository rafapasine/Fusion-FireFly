#include <FastLED.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define NUM_LEDS_RGB 18  // Número total de LEDs na fita RGB
#define DATA_PIN_RGB A0  // Pino de dados onde a fita RGB está conectada

// Definindo as portas de entrada
#define BUTTON_PIN_1 8
#define BUTTON_PIN_2 9
#define BUTTON_PIN_3 10
#define BUTTON_PIN_4 11

#define BUZZER_PIN 12  // Pino onde o buzzer está conectado

// Definindo as portas dos LEDs de alto brilho
#define LED_PIN_1 7
#define LED_PIN_2 6
#define LED_PIN_3 4
#define LED_PIN_4 5

CRGB ledsRGB[NUM_LEDS_RGB]; // Array para os LEDs da fita RGB

// Defina as cores para cada segmento
CRGB verde = CRGB::Green;
CRGB vermelho = CRGB::Red;
CRGB azul = CRGB::Blue;
CRGB rosa = CRGB::DeepPink;
CRGB amarelo = CRGB::Yellow;
CRGB laranja = CRGB::Orange;
CRGB roxo = CRGB::Purple;

// Adicione um array para armazenar a sequência do jogo
int sequenciaJogo[50];  // Aumente o tamanho se quiser uma sequência mais longa
int tamanhoSequencia = 0;
int indiceJogador = 0;  // Índice para acompanhar a posição da entrada do jogador
int pontuacao = 0; // Variável para armazenar a pontuação

// Inicialize o objeto LCD I2C com o endereço 0x27 (pode variar)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Protótipo de função
void lidarComEntradaJogador(int numeroBotao);
void acenderLedRGB(CRGB cor);
void andarLedsRGB();
void apagarLedsRGB();
void tocarSom(int segmento);
void pararSom();
void definirCorSegmento(int segmento);
void limparSegmento(int segmento);
bool verificarEntradaJogador(int entrada);
void gerarSequenciaAleatoria();
void atualizarPontuacaoNoDisplay();
void acenderLedBrilho(int led);
void apagarLedBrilho(int led);
void tocarBuzzer(int frequencia, int duracao);
void reproduzirSequenciaJogo();
void reiniciarJogo();

void setup() {
  // Inicialize a fita de LEDs
  FastLED.addLeds<NEOPIXEL, DATA_PIN_RGB>(ledsRGB, NUM_LEDS_RGB);

  // Inicialize as portas dos botões como entrada
  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);
  pinMode(BUTTON_PIN_3, INPUT_PULLUP);
  pinMode(BUTTON_PIN_4, INPUT_PULLUP);

  // Inicialize as portas dos LEDs de alto brilho como saída
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  pinMode(LED_PIN_3, OUTPUT);
  pinMode(LED_PIN_4, OUTPUT);

  // Inicialize o pino do buzzer
  pinMode(BUZZER_PIN, OUTPUT);

  // Configura o LCD com o número de colunas e linhas:
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();
  lcd.print("Bem-vindo ao");
  lcd.setCursor(0, 1);
  lcd.print("Fusion Firefly!");

  // Inicialize a semente do gerador de números aleatórios
  randomSeed(analogRead(0));

  // Aguarde a inicialização do jogo
  delay(2000);

  lcd.clear();
  delay(1000); // Adiciona o delay de 1 segundo antes de iniciar o jogo
  andarLedsRGB(); // Animação de início
  tocarBuzzer(1000, 500); // Buzzer apita ao iniciar o jogo
  pontuacao = 0;
  gerarSequenciaAleatoria(); // Gera uma nova sequência aleatória
  reproduzirSequenciaJogo();
}

// Função para configurar a cor do segmento
void definirCorSegmento(int segmento) {
  switch (segmento) {
    case 0:
      acenderLedBrilho(LED_PIN_1);
      break;
    case 1:
      acenderLedBrilho(LED_PIN_2);
      break;
    case 2:
      acenderLedBrilho(LED_PIN_3);
      break;
    case 3:
      acenderLedBrilho(LED_PIN_4);
      break;
    default:
      break;
  }
}

// Função para limpar a cor do segmento
void limparSegmento(int segmento) {
  switch (segmento) {
    case 0:
      apagarLedBrilho(LED_PIN_1);
      break;
    case 1:
      apagarLedBrilho(LED_PIN_2);
      break;
    case 2:
      apagarLedBrilho(LED_PIN_3);
      break;
    case 3:
      apagarLedBrilho(LED_PIN_4);
      break;
    default:
      break;
  }
}

// Função para gerar a sequência aleatória do jogo
void gerarSequenciaAleatoria() {
  for (int i = 0; i < 50; i++) {
    sequenciaJogo[i] = random(0, 4); // Ajustado para o número de botões
  }
  tamanhoSequencia = 1; // Começa com a primeira sequência
  indiceJogador = 0;
}

// Função para reproduzir a sequência do jogo
void reproduzirSequenciaJogo() {
  apagarLedsRGB(); // Apaga os LEDs RGB antes de iniciar a sequência
  for (int i = 0; i < tamanhoSequencia; i++) {
    tocarSom(sequenciaJogo[i]);
    definirCorSegmento(sequenciaJogo[i]);
    delay(500);
    pararSom();
    limparSegmento(sequenciaJogo[i]);
    delay(250);
  }
  atualizarPontuacaoNoDisplay(); // Atualiza o display com a pontuação atual após reproduzir a sequência
}

void loop() {
  // Verifique a entrada do jogador
  if (digitalRead(BUTTON_PIN_1) == LOW) {
    lidarComEntradaJogador(0);
  } else if (digitalRead(BUTTON_PIN_2) == LOW) {
    lidarComEntradaJogador(1);
  } else if (digitalRead(BUTTON_PIN_3) == LOW) {
    lidarComEntradaJogador(2);
  } else if (digitalRead(BUTTON_PIN_4) == LOW) {
    lidarComEntradaJogador(3);
  }

  // Pequeno delay para evitar leituras instáveis
  delay(50);
}

// Função para lidar com a entrada do jogador
void lidarComEntradaJogador(int numeroBotao) {
  definirCorSegmento(numeroBotao);  // Acende o LED correspondente
  tocarSom(numeroBotao);
  delay(500);
  limparSegmento(numeroBotao);
  pararSom();

  if (verificarEntradaJogador(numeroBotao)) {
    if (indiceJogador == tamanhoSequencia) { // Verifica se a sequência foi completada
      acenderLedRGB(verde); // Acende o LED verde para acerto
      lcd.clear();
      lcd.print("Correto!");
      tocarBuzzer(2000, 500); // Buzzer apita ao acertar
      delay(1000);
      acenderLedRGB(CRGB::Black); // Apaga o LED após o delay
      pontuacao++;  // Incrementa a pontuação antes de exibir no display
      atualizarPontuacaoNoDisplay();  // Atualiza o display com a nova pontuação
      tamanhoSequencia++; // Incrementa a sequência
      indiceJogador = 0; // Reinicia o índice do jogador para a nova sequência
      reproduzirSequenciaJogo(); // Reproduz a sequência atual
    }
  } else {
    acenderLedRGB(vermelho); // Acende o LED vermelho para erro
    lcd.clear();
    lcd.print("Errou!");
    tocarBuzzer(100, 1000); // Buzzer apita ao errar
    delay(1000);
    acenderLedRGB(CRGB::Black); // Apaga o LED após o delay
    reiniciarJogo(); // Reinicia o jogo
  }
}

// Função para reiniciar o jogo
void reiniciarJogo() {
  lcd.clear();
  lcd.print("Pontuacao: 0");
  gerarSequenciaAleatoria(); // Gera uma nova sequência aleatória
  tamanhoSequencia = 1; // Reinicia o jogo
  indiceJogador = 0; // Reinicia o índice do jogador
  pontuacao = 0; // Reinicia a pontuação
  atualizarPontuacaoNoDisplay();  // Atualiza o display com a pontuação reiniciada
  reproduzirSequenciaJogo(); // Reproduz a sequência atual
}

// Função para acender o LED RGB com a cor especificada
void acenderLedRGB(CRGB cor) {
  for (int i = 0; i < NUM_LEDS_RGB; i++) {
    ledsRGB[i] = cor;
  }
  FastLED.show();
}

// Função para apagar os LEDs RGB
void apagarLedsRGB() {
  for (int i = 0; i < NUM_LEDS_RGB; i++) {
    ledsRGB[i] = CRGB::Black;
  }
  FastLED.show();
}

// Função para fazer os LEDs RGB andarem
void andarLedsRGB() {
  CRGB coresCaminho[] = {CRGB::Red, CRGB::Orange, CRGB::Yellow, CRGB::Green, CRGB::Blue, CRGB::Indigo, CRGB::Violet, CRGB::White};
  int numCores = sizeof(coresCaminho) / sizeof(CRGB);
  
  // Inicializa todos os LEDs como apagados
  for (int i = 0; i < NUM_LEDS_RGB; i++) {
    ledsRGB[i] = CRGB::Black;
  }
  FastLED.show();
  
  // Acende os LEDs em um caminho colorido
  for (int i = 0; i < NUM_LEDS_RGB; i++) {
    ledsRGB[i] = coresCaminho[i % numCores];
    FastLED.show();
    delay(100);  // Ajuste o delay para a velocidade desejada
  }
}

// Função para tocar som baseado no segmento
void tocarSom(int segmento) {
  int frequencia;
  switch (segmento) {
    case 0:
      frequencia = 262; // C4
      break;
    case 1:
      frequencia = 294; // D4
      break;
    case 2:
      frequencia = 330; // E4
      break;
    case 3:
      frequencia = 349; // F4
      break;
    default:
      frequencia = 0;
      break;
  }
  if (frequencia > 0) {
    tone(BUZZER_PIN, frequencia);
  }
}

// Função para parar o som
void pararSom() {
  noTone(BUZZER_PIN);
}

// Função para verificar a entrada do jogador
bool verificarEntradaJogador(int entrada) {
  if (entrada == sequenciaJogo[indiceJogador]) {
    indiceJogador++;
    if (indiceJogador == tamanhoSequencia) {
      return true; // Retorna verdadeiro se a sequência estiver correta e completa
    }
  } else {
    return false; // Entrada incorreta
  }
  return true; // Entrada correta, sequência ainda não completa
}

// Função para atualizar a pontuação no display
void atualizarPontuacaoNoDisplay() {
  lcd.clear();
  lcd.print("Pontuacao: ");
  lcd.print(pontuacao);
}

// Função para acender os LEDs de alto brilho
void acenderLedBrilho(int led) {
  digitalWrite(led, HIGH);
}

// Função para apagar os LEDs de alto brilho
void apagarLedBrilho(int led) {
  digitalWrite(led, LOW);
}

// Função para tocar o buzzer
void tocarBuzzer(int frequencia, int duracao) {
  tone(BUZZER_PIN, frequencia);
  delay(duracao);
  noTone(BUZZER_PIN);
}
