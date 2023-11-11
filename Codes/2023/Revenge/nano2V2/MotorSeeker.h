class MotorSeeker {
private:
    int Max = 255;
    int RPWMA, LPWMA, ENA, RPWMB, LPWMB, ENB;

public:
    MotorSeeker(int RPWMA, int LPWMA, int ENA, int RPWMB, int LPWMB, int ENB);
    ~MotorSeeker();
    void move(int Velleft, int Velright);
};

MotorSeeker::MotorSeeker(int RPWMA, int LPWMA, int ENA, int RPWMB, int LPWMB, int ENB) {
    // Initialize member variables
    this->RPWMA = RPWMA;
    this->LPWMA = LPWMA;
    this->ENA = ENA;
    this->RPWMB = RPWMB;
    this->LPWMB = LPWMB;
    this->ENB = ENB;

    // Set pins as OUTPUT
    pinMode(RPWMA, OUTPUT);
    pinMode(LPWMA, OUTPUT);
    pinMode(ENA, OUTPUT);
    pinMode(RPWMB, OUTPUT);
    pinMode(LPWMB, OUTPUT);
    pinMode(ENB, OUTPUT);

    // Stop motors initially
    digitalWrite(RPWMA, LOW);
    digitalWrite(LPWMA, LOW);
    digitalWrite(ENA, LOW);
    digitalWrite(RPWMB, LOW);
    digitalWrite(LPWMB, LOW);
    digitalWrite(ENB, LOW);
}

MotorSeeker::~MotorSeeker() {
    // Add any cleanup code if needed
}

void MotorSeeker::move(int Velleft, int Velright) {
    if (Velleft < 0) {
        analogWrite(RPWMA, 0);
        analogWrite(LPWMA, -Velleft);
    } else {
        analogWrite(RPWMA, Velleft);
        analogWrite(LPWMA, 0);
    }

    if (Velright < 0) {
        analogWrite(RPWMB, 0);
        analogWrite(LPWMB, -Velright);
    } else {
        analogWrite(RPWMB, Velright);
        analogWrite(LPWMB, 0);
    }

    if (Velleft == 0 && Velright == 0) {
        digitalWrite(ENA, LOW);
        digitalWrite(ENB, LOW);
    } else {
        digitalWrite(ENA, HIGH);
        digitalWrite(ENB, HIGH);
    }
}
