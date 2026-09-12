// include the library
#include <LiquidCrystal.h>

int i=0;

int ultrasonic = 0;

long readUltrasonicDistance(int triggerPin, int echoPin)

{

pinMode(triggerPin, OUTPUT); // Clear the trigger

digitalWrite(triggerPin, LOW);

delayMicroseconds(2);

// Sets the trigger pin to HIGH state for 10 microseconds

digitalWrite(triggerPin, HIGH);

delayMicroseconds(10);

digitalWrite(triggerPin, LOW);

pinMode(echoPin, INPUT);

// Reads the echo pin, and returns the sound wave travel time in microseconds

return pulseIn(echoPin, HIGH);

}
// Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
byte Heart[8] = {
0b00000,
0b01010,
0b11111,
0b11111,
0b01110,
0b00100,
0b00000,
0b00000
};

//prepping up the variables
int counter_week = 0;
int ButtonState = 0;
bool Daytime=true;
int dist=10;
int counter_distance=62;
int hours_slept[7]={};


void setup() {
  pinMode(10, INPUT);
  pinMode(7, OUTPUT);
  lcd.createChar(0, Heart);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // Clears the LCD screen
  lcd.clear();
  Serial.begin(9600);
}

void loop() {

  if (Daytime==true){
    //once user wakes up last day of the week check average and display
    if (counter_week==6){
      lcd.clear();
      lcd.print("You slept an avg");
      lcd.setCursor(0, 1);
      lcd.print("of");
      int average=(hours_slept[0]+hours_slept[1]+hours_slept[2]+hours_slept[3]+hours_slept[4]+hours_slept[5]+hours_slept[6])/7;
      lcd.print(average);
      lcd.print("hours ts week");
      delay(10000);
      counter_week=0;
    }
    //if its not last day, display days for arrival counter and encouraging message
    lcd.clear();
    lcd.print("youve got this ");
    lcd.write(byte(0));
    lcd.setCursor(0, 1);
    lcd.print("Days left: ");
    lcd.print(counter_distance);
    //prep to recieve button press and change to night mode
    ButtonState=LOW;
    while (ButtonState!=HIGH){ButtonState = digitalRead(10);}
    Daytime=false;
    
  }

  else if (Daytime==false){
    unsigned long startTime = millis();
    lcd.clear();
    lcd.print("es de noche");
    lcd.print(counter_week);

    //check for phone presence 
    while (i==0){
      lcd.clear();
      int distance=(0.01723 * readUltrasonicDistance(8, 9));
      Serial.print(distance);
      if(distance>10){
        digitalWrite(7, HIGH);
        lcd.print("si");
        delay(200);
        digitalWrite(7, LOW);
      }
      else{
        lcd.clear();
        lcd.print("phone found");
        lcd.setCursor(0, 1);
        lcd.print("Go to eep");
        delay(200);
      }
      ButtonState = digitalRead(10);
      if (ButtonState==HIGH){
        delay(1000);
        i=1;
      }
    }
    i=0;
    //counter_week+=1;

    //calculating the time slept this night
    unsigned long elapsedTime = millis() - startTime;
    lcd.clear();
    lcd.print("Time slept: ");
    lcd.write(byte(0));
    lcd.setCursor(0, 1);
    lcd.print(elapsedTime/3600000);
    lcd.print("h");
    delay(10000);
    hours_slept[counter_week]=(elapsedTime/3600000);
    counter_week++;
    counter_distance-=1;
    Daytime=true;
  }
  
    

  
}