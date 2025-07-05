#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(128, 32, &Wire, -1);
int SW1 = D6; // bottom left
int SW2 = D0; // top left
int SW3 = D9; // top right
int SW4 = D7; // bottom right

int LED1 = D1; // top left
int LED2 = D3; // bottom left
int LED3 = D8; // bottom right
int LED4 = D10;// top right

int SW[] = {0, SW1, SW2, SW3, SW4}; // what the jank
int LED[] = {0, LED1, LED2, LED3, LED4};

int current = 1;
int next = 1;
int score = 0;
long lastTime;
long averageTime = 0;
long times[5];

void setup() {
  Serial.begin(9600);
  // initialize the OLED object
 if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
   Serial.println(F("SSD1306 allocation failed"));
   for(;;); // Don't proceed, loop forever
  }
  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  pinMode(SW3, INPUT);
  pinMode(SW4, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  
  lastTime = millis(); 
}

// the loop function runs over and over again forever
void loop() {
  if (digitalRead(SW[current]) == HIGH) {
    digitalWrite(LED[current], LOW);
    current = next;
    next = random(1,5); // ts is supposed to give me a random number between 1 and 4
    while (next == current) {
      next = random(1,5); // if you're infinitely lucky this will be an infinite loop
    }
    digitalWrite(LED[next], HIGH);
    score++;
    long timeTaken = millis() - lastTime;
    lastTime = millis();

    if (score <= 6 && score > 1) {  
      times[(score - 2)] = timeTaken;

      long sum = 0;
      for (int i = 0; i < (score - 1); i++) {
        sum += times[i];
      }
      averageTime = sum / (score - 1);
    }

    // next we have to update the oled based on score and timeTaken - averageTime
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("Score: ");
    display.print(score);

    // now for osu style scoring
    display.setCursor(0, 16);
    if (abs(timeTaken - averageTime) < 17) {
      display.print("Marvelous!");
    } else if (abs(timeTaken - averageTime) < 50) {
      display.setTextColor(YELLOW);
      display.print("Perfect!");
    } else if (abs(timeTaken - averageTime) < 80) {
      display.setTextColor(GREEN);
      display.print("Great");
    } else if (abs(timeTaken - averageTime) < 110) {
      display.setTextColor(BLUE);
      display.print("Good");
    } else if (abs(timeTaken - averageTime) < 150) {
      display.setTextColor(MAGENTA);
      display.print("Bad");
    } else {
      display.setTextColor(RED);
      display.print("Missed!");
    }
    display.display();
  }
}
