//Definir os paramtetros

int frequency = 50;
float offset = 2.5;
float period = 1.0/frequency;

float cut [] = {frequency, offset, 100, 2};
float blend_1 [] = {frequency, offset, 75, 2.25};
float blend_2 [] = {frequency, offset, 50, 2.5};
float blend_3 [] = {frequency, offset, 25, 2.25};
float coag [] = {frequency, offset, 5, 3};

// Define the pins for the resistor ladder
#define LSB 4 // Least significant bit
#define MSB 13 // Most significant bit

// Define the pins for the switches
#define SW1 A5 // Switch 1
#define SW2 A4 // Switch 2
#define SW3 A3 // Switch 3

// Define the input value variable
int value = (1024/5)*offset; // Initial value
void setup() {
  // Set the resistor ladder pins as outputs
  for (int i = LSB; i <= MSB; i++) {
    pinMode(i, OUTPUT);
  }

  // Set the switch pins as inputs with pull-up resistors
  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
  pinMode(SW3, INPUT_PULLUP);
}

int adc_d(int value){
  for (int i = LSB; i <= MSB; i++) {
    digitalWrite(i, bitRead(value, i - LSB));
  }}

int bist_d(float frequency, float offest, float duty, float amp){
	
  for (float z = 0; z<=period/((duty-100)/100); z += 0.001){
  for (float t = 0; t<=period; t += 0.001){
    value = (offset*(1024/5))+(1024/10)*amp*sin(2 * PI * frequency * t);
  adc_d(value);
  }value=(1024/5)*offset;
  adc_d(value);
    delay(1000*(duty-100)*period);}}

void loop() {

  if (digitalRead(SW1) == LOW) {bist_d(cut[0], cut[1], cut[2], cut[3]);}
  if (digitalRead(SW2) == LOW) {bist_d(blend_2 [0], blend_2 [1], blend_2 [2], blend_2 [3] );}
  if (digitalRead(SW3) == LOW) {bist_d(coag [0], coag [1], coag [2], coag [3]);}
  else {adc_d((1024/5)*offset);}

}