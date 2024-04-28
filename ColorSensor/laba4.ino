// Define color sensor pins
#define S0 2
#define S1 3
#define S2 4
#define S3 5
#define sensorOut 6 //TTL level square wave

int redMin = 14; 
int redMax = 175; 
int greenMin = 14; 
int greenMax = 212; 
int blueMin = 13; 
int blueMax = 180;

int redPW = 0;
int greenPW = 0;
int bluePW = 0;

int redValue;
int greenValue;
int blueValue;

void setup() {
	pinMode(S0, OUTPUT);
	pinMode(S1, OUTPUT);
	pinMode(S2, OUTPUT);
	pinMode(S3, OUTPUT);

	// Set output frequency scaling to 20%
	digitalWrite(S0,HIGH);
	digitalWrite(S1,LOW);

	pinMode(sensorOut, INPUT); //т. к. получаем прямоугольный сигнал

	Serial.begin(9600);
}

void loop() {
	redPW = getRedPW();
  redValue = map(redPW, redMin,redMax,255,0);
  redValue = constrain(redValue, 0, 255);
	delay(500);

	greenPW = getGreenPW();
  greenValue = map(greenPW, greenMin,greenMax,255,0);
  greenValue =  constrain(greenValue, 0, 255);
	delay(500);

	bluePW = getBluePW();
  blueValue = map(bluePW, blueMin,blueMax,255,0);
  blueValue = constrain(blueValue, 0, 255);
	delay(500);

	Serial.print("R = ");
	Serial.print(redValue);
	Serial.print("   G = ");
	Serial.print(greenValue);
	Serial.print("   B = ");
	Serial.println(blueValue);
}

int getRedPW() {
	digitalWrite(S2,LOW); //red photodiode type
	digitalWrite(S3,LOW);
	int PW;
	PW = pulseIn(sensorOut, LOW); //считываем длину сигнала на заданном порту
	return PW;
}

int getGreenPW() {
	digitalWrite(S2,HIGH); //greed photodiode type
	digitalWrite(S3,HIGH);
	int PW;
	PW = pulseIn(sensorOut, LOW);
	return PW;
}

int getBluePW() {
	digitalWrite(S2,LOW); //blue photodiode type
	digitalWrite(S3,HIGH);
	int PW;
	PW = pulseIn(sensorOut, LOW);
	return PW;
}
