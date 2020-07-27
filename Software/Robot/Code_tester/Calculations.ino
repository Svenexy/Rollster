const short Coxa = 40;
const short Femur = 80;
const short Tibia = 90;


int bodySideLenght = 100;

double initFeetPosX[6];
double initFeetPosY[6];
double initFeetPosZ[6];

double bodyCenterOffsetX[6];
double bodyCenterOffsetZ[6];


int legAnglesCoxa[6];
int legAnglesFemur[6];
int legAnglesTibia[6];


int posX = 0;
int posY = 0;
int posZ = 0;
int rotX = 0;
int rotY = 0;
int rotZ = 0;

double IKbodyX = 0;
double IKbodyY = 0;
double IKbodyZ = 0;

double IKTibiaAngle = 0;
double IKFemurAngle = 0;
double IKCoxaAngle = 0;

double newPosX = 0;
double newPosY = 0;
double newPosZ = 0;
double newTotalX = 0;
double newTotalY = 0;
double newTotalZ = 0;





void INIT() {
  initFeetPosition();
  coxaBodyCenterOffset();

  //  for (int i = 0; i < 6; i++) {
  newTotalZ = initFeetPosZ[0] + bodyCenterOffsetZ[0] + posZ;
  newTotalY = initFeetPosY[0];
  newTotalX = initFeetPosX[0] + bodyCenterOffsetX[0] + posX;
  Body(0, newTotalX, newTotalY, newTotalZ, rotX, rotY, rotZ);
  //  }


}

//void  NewCalc(){

// 	for (int i = 0; i < 6; i++) {
// 		newTotalZ = initFeetPosZ[i] + bodyCenterOffsetZ[i] + posZ;
// 		newTotalY = initFeetPosY[i];
// 		newTotalX = initFeetPosX[i] + bodyCenterOffsetX[i] + posX;
// 		body(i, newTotalX, newTotalY, newTotalZ, rotX, rotY, rotZ);
// 	}
// }




void initFeetPosition () {
  initFeetPosX[0] = cos(PI / 3) * (Coxa + Femur);
  initFeetPosY[0] = Tibia;
  initFeetPosZ[0] = sin(PI / 3) * (Coxa + Femur);
  initFeetPosX[1] = Coxa + Femur;
  initFeetPosY[1] = Tibia;
  initFeetPosZ[1] = 0;
  initFeetPosX[2] = cos(PI / 3) * (Coxa + Femur);
  initFeetPosY[2] = Tibia;
  initFeetPosZ[2] = sin(-PI / 3) * (Coxa + Femur);
  initFeetPosX[3] = -cos(PI / 3) * (Coxa + Femur);
  initFeetPosY[3] = Tibia;
  initFeetPosZ[3] = sin(-PI / 3) * (Coxa + Femur);
  initFeetPosX[4] = -(Coxa + Femur);
  initFeetPosY[4] = Tibia;
  initFeetPosZ[4] = 0;
  initFeetPosX[5] = -cos(PI / 3) * (Coxa + Femur);
  initFeetPosY[5] = Tibia;
  initFeetPosZ[5] = sin(PI / 3) * (Coxa + Femur);
  DBGSerial.println("initFeetPosX");
  printDoubleArray(initFeetPosX, 6);
  DBGSerial.println("initFeetPosY");
  printDoubleArray(initFeetPosY, 6);
  DBGSerial.println("initFeetPosZ");
  printDoubleArray(initFeetPosZ, 6);


}



void coxaBodyCenterOffset() {
  int CenterOffsetX = bodySideLenght / 2;
  int CenterOffsetZ = sqrt(pow(bodySideLenght, 2) - pow(CenterOffsetX, 2));
  int CenterOffset = bodySideLenght;

  bodyCenterOffsetX[0] = CenterOffsetX;
  bodyCenterOffsetZ[0] = CenterOffsetZ;
  bodyCenterOffsetX[1] = CenterOffset;
  bodyCenterOffsetZ[1] = 0;
  bodyCenterOffsetX[2] = CenterOffsetX;
  bodyCenterOffsetZ[2] = -CenterOffsetZ;
  bodyCenterOffsetX[3] = -CenterOffsetX;
  bodyCenterOffsetZ[3] = -CenterOffsetZ;
  bodyCenterOffsetX[4] = -CenterOffset;
  bodyCenterOffsetZ[4] = 0;
  bodyCenterOffsetX[5] = -CenterOffsetX;
  bodyCenterOffsetZ[5] = CenterOffsetZ;
  DBGSerial.println("bodyCenterOffsetX");
  printDoubleArray(bodyCenterOffsetX, 6);
  DBGSerial.println("bodyCenterOffsetZ");
  printDoubleArray(bodyCenterOffsetZ, 6);


}


void Leg(short leg, int x, int y, int z) {

  double gamma = atan2(x, z);

  double L1 = sqrt(x * x + z * z);
  double L = sqrt(pow(y, 2) + pow((L1 - Coxa), 2));

  double alpha1 = atan2((L1 - Coxa), y);
  double alpha2 = acos((pow(Tibia, 2) - pow(Femur, 2) - pow(L, 2)) / (-2 * Femur * L));

  double alpha = (alpha1 + alpha2);

  double beta =  (acos((pow(L, 2) - pow(Tibia, 2) - pow(Femur, 2)) / (-2 * Tibia * Femur)));



  IKTibiaAngle = PI / 2 - beta;
  IKFemurAngle = PI / 2 - alpha;
  IKCoxaAngle = PI / 2 - gamma;

  legAngle(leg, RadDeg(IKTibiaAngle), RadDeg(IKFemurAngle), RadDeg(IKCoxaAngle));


}

void Body(short leg, int x, int y, int z, int rotX, int rotY, int rotZ) {

  double DistBodyCenterFeet = sqrt(x * x + z * z);

  double AngleBodyCenterX = PI / 2 - atan2(x, z);

  double RollY = tan(rotZ * PI / 180) * x;
  double PitchY = tan(rotX * PI / 180) * z;

  IKbodyX = cos(AngleBodyCenterX + (rotY * PI / 180)) * DistBodyCenterFeet - x;
  IKbodyZ = (sin(AngleBodyCenterX + (rotY * PI / 180)) * DistBodyCenterFeet) - z;
  IKbodyY = RollY + PitchY;

  newPosX = initFeetPosX[leg] + posX + IKbodyX;
  newPosY = initFeetPosY[leg] + posY + IKbodyY;
  newPosZ = initFeetPosZ[leg] + posZ + IKbodyZ;

  Leg(leg, newPosX, newPosY, newPosZ);

}


void legAngle(short leg, float Tibia, float Femur, float Coxa) {
  legAnglesCoxa[leg] = Coxa * (PI / 6 * leg);
  legAnglesFemur[leg] = Femur;
  legAnglesTibia[leg] = Tibia;

  DBGSerial.println("legAnglesCoxa:");
  printDoubleArray(legAnglesCoxa, 6);
  DBGSerial.println("legAnglesFemur:");
  printDoubleArray(legAnglesFemur, 6);
  DBGSerial.println("legAnglesTibia:");
  printDoubleArray(legAnglesTibia, 6);

  moveServo(0, legAnglesCoxa[leg], 100);
  moveServo(1, legAnglesFemur[leg], 100);
  moveServo(2, legAnglesTibia[leg], 100);
}

// Radians to degrees
int RadDeg (float rad) {
  return (rad * (180 / PI));
}

// degrees to radians
float DegRad (int deg) {
  return (deg * (PI / 180));
}
