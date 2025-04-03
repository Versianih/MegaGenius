int rodada = 0;
int leds[12] = {23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45};
int botoes[12] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44};
int start_led = 47;
int start_button = 46;
int buzzer = A1;
int tons[12] = {262, 294, 330, 349, 392, 440, 494, 523, 587, 659, 698, 784};
int passo = 0;
int botao_pressionado = 0;
int sequencia[32] = {};
bool game_over = false;
bool in_game = false;


void setup() {
  // Porta Serial
  Serial.begin(9600);

  // Buzzer (opcional)
  pinMode(buzzer, OUTPUT);

  // Leds
  for (int i = 0; i <= 11; i++) {
    pinMode(leds[i], OUTPUT);
  }
  pinMode(start_led, OUTPUT);

  // Botões
  for (int i = 0; i <= 11; i++) {
    pinMode(botoes[i], INPUT_PULLUP);
  }
  pinMode(start_button, INPUT_PULLUP);

  // Led indicando que não está em jogo
  digitalWrite(start_led, HIGH);
}

void loop() {
  if (in_game == true){
    proximaRodada();
    reproduzirSequencia();
    aguardarJogador();

    // Reset de variáveis após game_over
    if (game_over == true) {
      limparSequencia();
      rodada = 0;
      passo = 0;
      game_over = false;
      in_game = false;
      digitalWrite(start_led, HIGH);
    }
    delay(1000);
  } else if (digitalRead(start_button) == LOW){
      in_game = true;
      digitalWrite(start_led, LOW);
      delay(100);
  }
}

void limparSequencia() {
  for (int i = 0; i < 32; i++) {
    sequencia[i] = 0;
  }
}

void proximaRodada() {
  int sorteio = random(12);
  sequencia[rodada] = sorteio;
  rodada ++;
}

void reproduzirSequencia() {
  for (int i = 0; i < rodada; i++) {
    tone(buzzer, tons[sequencia[i]]);
    digitalWrite(leds[sequencia[i]], HIGH);
    delay(500);
    noTone(buzzer);
    digitalWrite(leds[sequencia[i]], LOW);
    delay(100);
  }
}

void aguardarJogador() {
  for (int i = 0; i < rodada; i++) {
    bool jogada_efetuada = false;
    while (!jogada_efetuada) {
      for (int i = 0; i <= 11; i++) {
        if (digitalRead(botoes[i]) == LOW) {
          botao_pressionado = i;
          tone(buzzer, tons[i]);
          digitalWrite(leds[i], HIGH);
          delay(300);
          digitalWrite(leds[i], LOW);
          noTone(buzzer);
          jogada_efetuada = true;
        }
      }
    }
    //verificar jogada
    if (sequencia[passo] != botao_pressionado) {
      for (int i = 0; i <= 11; i++) {
        tone(buzzer, 70);
        digitalWrite(leds[i], HIGH);
        delay(100);
        digitalWrite(leds[i], LOW);
        noTone(buzzer);
      }
      game_over = true;
      break;
    }
    passo++;
  }
  passo = 0;
}
