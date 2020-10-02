
enum ledStates {INCREASE, DECREASE, STAY}; // Here we make nicknames for the different states our program supports.
enum ledStates ledState; // We define 'ledState' as type ledStates'
enum ledStates previousLedState = ledState;

//unsigned long startMillis;  //some global variables available anywhere in the program
//unsigned long currentMillis;

int brightness = 0; // our main variable for setting the brightness of the LED
float velocity = 1.0; // the speed at which we change the brightness.
int ledPin = 9; // we use pin 9 for PWM
int TRIG = 10;
int ECHO = 11;
int DISTANCE;
int DURATION;

const int numReadings = 15;

int readings[numReadings];      // the readings from the input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

int p = 0; // use to keep track how often we plot
int plotFrequency = 3; // how often we plot, every Nth time.

//==========================================================
//                     FUNCTIONS
//==========================================================

int increase_brightness (int brightness, float velocity){
    return brightness = brightness + 1 * velocity;
  }

int decrease_brightness (int brightness, float velocity){
    return brightness = brightness - 1 * velocity;
  }

 void changeState(ledStates newState){
    // call to change state, will keep track of time since last state
    //startMillis = millis();
    ledState = newState;
  }

void plot(char *state, int brightness){
    // use this function to plot a graph.
    // it will normalize the auto-scaling plotter

    if ((p % plotFrequency) == 0){
      Serial.print(state);
      Serial.print(", ");
      Serial.print(brightness);
      Serial.print(", ");
      Serial.print(DISTANCE);
      Serial.println(", 0, 300");
      
    }
    p++;
  }

//==================================================
//                  MAIN CODE
//==================================================

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT); // set ledPin as an output.
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  Serial.begin(9600); // initiate the Serial monitor so we can use the Serial Plotter to graph our patterns
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(TRIG, HIGH);
  compose();
  delay(10);
  //digitalWrite(TRIG, LOW);
  DURATION = pulseIn(ECHO, HIGH);
  DISTANCE = 0.017 * DURATION;


  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = DISTANCE;
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

    // calculate the average:
    average = total / numReadings;
    DISTANCE = average;
  
    brightness = map(average, 300, 0, 0, 255);
    analogWrite(ledPin, brightness); 
    delay(1);        // delay in between reads for stability
}

void compose() {
  // this is a state machine which allows us to decouple the various operations from timed loops. 
  // instead we just switch from state to state when particular conditions are met.
  // we switch states by calling the changeState() function.

  
  switch (ledState){
    
  case INCREASE:
   if(readings[0] > readings[2]) {
    brightness = increase_brightness(brightness, 1);
    plot("INCREASING", brightness);
   } else if(readings[0] == readings[2]){
      changeState(STAY);
    } else {
      changeState(DECREASE);
    }
    break;
   
  case DECREASE:
     if(readings[0] < readings[2]) {
      brightness = decrease_brightness(brightness, 1); 
      plot("DECREASING", brightness);
     } else if(readings[0] == readings[2]){
       changeState(STAY);
     } else {
      changeState(INCREASE);
     }
     break;
     
   case STAY:
    plot("STAY", brightness);
    brightness = brightness; 
    if(readings[0] > readings[2]){
      changeState(DECREASE);
    }else if(readings[0] < readings[2]){
      changeState(INCREASE);
    }
    break;
  }
}
