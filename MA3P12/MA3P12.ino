// MA3P12.ino
#define PWM_FREQ 250
#define PWM_RES 12
#define PWM_PIN 5

const uint16_t timePeriod = 1E6/PWM_FREQ;				// 4000 us
const uint16_t numLevels = pow(2, PWM_RES);				// 4096


uint8_t positionPer = 25;
uint32_t positionTime, currentTime, previousTime;
boolean risingEdge;

void setup() {
	pinMode(PWM_PIN, OUTPUT);
	digitalWrite(PWM_PIN, LOW);

	positionTime = ((long)positionPer*timePeriod)/100;
	currentTime = micros();
	previousTime = 0;
	risingEdge = false;

	Serial.begin(115200);
	Serial.print("MA-P12 simulator\nCurrent position: ");
	Serial.print(positionPer);
	Serial.print("% (T_on = ");
	Serial.print(positionTime);
	Serial.print(" us)\n");
}

void loop() {
	currentTime = micros();

	// flip to high every time period
	if((currentTime - previousTime) > timePeriod) {
		previousTime = currentTime;
		digitalWrite(PWM_PIN, HIGH);
		risingEdge = true;
	}

	// flip low after the predefined time
	if(risingEdge && (currentTime > (positionTime + previousTime))) {
		digitalWrite(PWM_PIN, LOW);
		risingEdge = false;

		uint16_t tmp = map(analogRead(A0), 0, 1023, 0, 100);
		if(tmp != positionPer) {
			positionPer = constrain(tmp, 1, 99);
			positionTime = ((long)positionPer*timePeriod)/100;
		}
	}

	 


}

