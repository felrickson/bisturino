// Definir os par?metros
int frequency = 50;
float offset = 2.5;
float period = 1.0 / frequency;

float cut[] = {frequency, offset, 100, 2};
float blend_1[] = {frequency, offset, 75, 2.25};
float blend_2[] = {frequency, offset, 50, 2.5};
float blend_3[] = {frequency, offset, 25, 2.25};
float coag[] = {frequency, offset, 5, 3};

// Define os pins para a escada de resistores
#define LSB 4 // Bit menos significativo
#define MSB 13 // Bit mais significativo

// Define os pins para os bot?es
#define BTN_UP A5 // Bot?o para cima
#define BTN_DOWN A4 // Bot?o para baixo
#define BTN_SELECT A3 // Bot?o de sele??o

// Define a vari?vel de valor de entrada
int value = (1024 / 5) * offset; // Valor inicial

// Vari?veis do menu
int selectedMode = 0; // Modo selecionado
const int numModes = 5; // N?mero total de modos

void setup() {
  // Configura os pins da escada de resistores como sa?das
  for (int i = LSB; i <= MSB; i++) {
    pinMode(i, OUTPUT);
  }

  // Configura os pins dos bot?es como entradas com resistores pull-up
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);

  // Inicializa a comunica??o serial
  Serial.begin(9600);
}

int adc_d(int value) {
  // Implementa??o simples para atualizar a escada de resistores
  for (int i = LSB; i <= MSB; i++) {
    digitalWrite(i, bitRead(value, i - LSB));
  }
}

int bist_d(float frequency, float offset, float duty, float amp) {
  // Implementa??o simples para gerar uma forma de onda senoidal
  for (float t = 0; t <= period; t += 0.001) {
    value = (offset * (1024 / 5)) + (1024 / 10) * amp * sin(2 * PI * frequency * t);
    adc_d(value);
  }
  // Aguarda por algum tempo antes de encerrar
  delay(1000 * (duty - 100) * period);
}

void displayMenu() {
  Serial.println("Selecione um modo:");
  Serial.println("1. Cut");
  Serial.println("2. Blend 1");
  Serial.println("3. Blend 2");
  Serial.println("4. Blend 3");
  Serial.println("5. Coag");
}

void loop() {
  // Exibe o menu e aguarda a sele??o do modo
  displayMenu();

  while (selectedMode == 0) {
    if (digitalRead(BTN_UP) == LOW) {
      selectedMode = (selectedMode + numModes - 1) % numModes + 1;
      Serial.println(selectedMode);
      delay(200); // Debounce
    }
    if (digitalRead(BTN_DOWN) == LOW) {
      selectedMode = (selectedMode % numModes) + 1;
      Serial.println(selectedMode);
      delay(200); // Debounce
    }
    if (digitalRead(BTN_SELECT) == LOW) {
      Serial.println("executando");
      // Bot?o de sele??o pressionado, sai do loop
      break;
    }
  }

  // Executa o modo selecionado
  switch (selectedMode) {
    case 1:
      bist_d(cut[0], cut[1], cut[2], cut[3]);
      break;
    case 2:
      bist_d(blend_1[0], blend_1[1], blend_1[2], blend_1[3]);
      break;
    case 3:
      bist_d(blend_2[0], blend_2[1], blend_2[2], blend_2[3]);
      break;
    case 4:
      bist_d(blend_3[0], blend_3[1], blend_3[2], blend_3[3]);
      break;
    case 5:
      bist_d(coag[0], coag[1], coag[2], coag[3]);
      break;
  }

  // Reseta o modo selecionado
  selectedMode = 0;
}
