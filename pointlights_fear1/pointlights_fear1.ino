enum ledStates {WAVE, /*INCREASE,*/ DECREASE,/*, STAY , */OFF /*ON*/}; // Here we make nicknames for the different states our program supports.
enum ledStates ledState; // We define 'ledState' as type ledStates'
enum ledStates previousLedState = ledState;

unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;

int brightness = 0; // our main variable for setting the brightness of the LED
float velocity = 1.0; // the speed at which we change the brightness.
int ledPin = 11; // we use pin 11 for PWM
int p = 0; // use to keep track how often we plot
int plotFrequency = 3; // how often we plot, every Nth time.

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT); // set ledPin as an output.
  Serial.begin(9600); // initiate the Serial monitor so we can use the Serial Plotter to graph our patterns
}

void loop() {
  // put your main code here, to run repeatedly:
  compose();
  delay(10);
  analogWrite(ledPin, brightness);
  currentMillis = millis(); //store the current time since the program started
}

void compose() {
  // this is a state machine which allows us to decouple the various operations from timed loops. 
  // instead we just switch from state to state when particular conditions are met.
  // we switch states by calling the changeState() function.
  
  float rdm = random(1,100);
  float rdm2 = random(1,5);
  
  switch (ledState){

  /*case INCREASE:
    brightness = increase_brightness(brightness, 1);

    plot("INCREASING", brightness);
        
    if (brightness > 250){
      //ledState = WAVE;
      changeState(WAVE);
      }
    break;*/
   
  /*case DECREASE:
    brightness = decrease_brightness(brightness, 0.5);
    plot("DECREASING", brightness);
      if (brightness == 0){
      changeState(OFF);
      }
     break;*/

  case WAVE:
    plot("WAVE", brightness);
    
    brightness = sinewave(6000,rdm,rdm2); // you can tweak the parameters of the sinewave
    analogWrite(ledPin, brightness);
    
    /*if (currentMillis - startMillis >= 1000){ //change state after 5 secs by comparing the time elapsed since we last change state
      changeState(DECREASE);
      }*/
    break;
    
  /*case STAY:
    plot("STAY", brightness);
    brightness = brightness;
    break;

  case ON:
    plot("ON", brightness);
    brightness = 255;
    break;*/

  /*case OFF:
    plot("OFF", brightness);
    brightness = 0;
    if (currentMillis - startMillis >= 1000){
      changeState(WAVE);
      }
    break;*/
  }
}

void changeState(ledStates newState){
    // call to change state, will keep track of time since last state
    startMillis = millis();
    ledState = newState;
  }
  
void plot(char *state, int brightness){
    // use this function to plot a graph.
    // it will normalize the auto-scaling plotter

    if ((p % plotFrequency) == 0){
      Serial.print(state);
      Serial.print(", ");
      Serial.print(brightness);
      Serial.println(", 0, 300");
    }
    p++;
  }

int increase_brightness (int brightness, float velocity){
    return brightness = brightness + 1 * velocity;
  }

int decrease_brightness (int brightness, float velocity){
    return brightness = brightness - 1 * velocity;
  }

int sinewave(float duration, float amplitude, int offset){
    // Generate a sine oscillation, return a number.
    // In case you are using this for analogWrite, make sure the amplitude does not exceed 256
    float period = millis()/duration; // Duration in ms determines the wavelength.
    float midpoint = amplitude / 2; // set the midpoint of the wave at half the amplitude so there are no negative numbers
    int value = midpoint + midpoint * sin ( period * 2.0 * PI );
    value = value + offset; //offset allows you to move the wave up and down on the Y-axis. Should not exceed the value of amplitude to prevent clipping.
    return value;
  }
