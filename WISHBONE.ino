//WISHBONE
//Programmed by Jacob Surovsky
//Who can snag the bigger half of the wishbone and be the victor?
//long press the center of the wishbone to become the "crown"

//test

#define BADRED makeColorHSB(8, 255, 230)
#define GOODGREEN makeColorHSB(69, 255, 255)
#define CROWNGREEN makeColorHSB(56, 255, 255)
#define BONEBLUE makeColorHSB(200, 255, 150)

enum blinkRoles {LEG, CROWN};
byte blinkRole = LEG;

enum signalStates {INERT, IM_LEG, IM_CROWN, LOSE, WIN};
byte signalState = INERT;

bool bChangeRole = false;
bool bLongPress = false;

bool bInert;

//LEG 
byte legTouchingFace;


//CROWN
bool bWin;
byte faceCount;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

if(hasWoken()){
    bLongPress = false;
    }

    if (buttonLongPressed()) {
    bLongPress = true;
  }

  if(buttonReleased()) {
    if(bLongPress) {
      // now change the role
      bChangeRole = true;
      bLongPress = false;
    }

  }
    switch (blinkRole) {
    case LEG:
    legLoop();
      break;
    case CROWN:
    crownLoop();
      break;
  }

   if(bLongPress){ 
    //transition color
      setColor(WHITE);
      } 

}

//LEG BEHAVIOR -------------------------------

void legLoop(){
   if(bChangeRole){
    blinkRole = CROWN;
    bChangeRole = false;
    }

    faceCount = 0;
    bInert = false;
    bWin = false;
    
  setColor(BONEBLUE);

  setValueSentOnAllFaces(IM_LEG);

  FOREACH_FACE(f){
    if(!isValueReceivedOnFaceExpired(f)){ //I have a neighbor

    faceCount = faceCount + 1; //count the number of faces I'm touching
      
      if(getLastValueReceivedOnFace(f) == IM_CROWN){  //am I touching a crown?

        bWin = false;
        }

        if(getLastValueReceivedOnFace(f) == IM_LEG){ //am I touching a leg? 

          legTouchingFace = f; //saving this face for later reference
          }

          if(getLastValueReceivedOnFace(f) == INERT){ 
          //I know my leg is still touching the crown
          bInert = true;
        }

        if(getLastValueReceivedOnFace(f) == WIN){
          //I won!
          bWin = true;
        }
          
    } 
 }

if(faceCount == 2){ //if we're touching a crown and a leg

if(bWin == true){ //if the crown is telling me I won
      setColor(GOODGREEN);
      setValueSentOnFace(WIN, legTouchingFace); //tell my leg neighbor I won
      } else {
       setColor(BONEBLUE);
       boneColor();
       setValueSentOnFace(INERT, legTouchingFace); //tell my leg neighbor to be normal
      }
  }

if(faceCount == 1){
  
  if(bInert == true){ //if my neighbor has told me to be normal, be normal
    setColor(BONEBLUE);
    endBone();
    } 
    else if (bWin == true){ //if my neighbor told me we won, turn green!
      setColor(GOODGREEN);
    
      }
    else if (bInert == false) { //if my neighbor didn't tell me to be normal, then we lost.
      setColor(BADRED);
      setValueSentOnAllFaces(IM_LEG);

      }
  }

if(faceCount == 0){
  //ya goofed up buddy
  setColor(BADRED);
    }

 
}




//CROWN BEHAVIOR -------------------------------

void crownLoop(){
   if(bChangeRole){
    blinkRole = LEG;
    bChangeRole = false;
    }

    faceCount = 0; 

    setColor(BLUE);
  
  setValueSentOnAllFaces(IM_CROWN);

   FOREACH_FACE(f){
    if(!isValueReceivedOnFaceExpired(f)){ //a neighbor

        faceCount = faceCount + 1; //count how many faces I'm touching 

      } 

if(faceCount >= 2){ //If I'm touching 2 (or more) faces, business as usual
  setColor(BLUE);
  boneColor();
  setValueSentOnAllFaces(IM_CROWN);
  }

if(faceCount == 1){ //if I'm touching one face, then that face will be told it's the winner
  setColor(CROWNGREEN);

  setColorOnFace(CROWNGREEN, 0);
  setColorOnFace(GOODGREEN, 1);
  setColorOnFace(CROWNGREEN, 2);
  setColorOnFace(GOODGREEN, 3);
  setColorOnFace(CROWNGREEN, 4);
  setColorOnFace(GOODGREEN, 5);
  
  setValueSentOnAllFaces(WIN);
  }

  if(faceCount == 0){ //if I'm touching no faces, then just be normal
  setColor(BLUE);
  }

    }
  
  }


void boneColor(){
  FOREACH_FACE(f){
 if(!isValueReceivedOnFaceExpired(f)){ 
  setColorOnFace(WHITE, f);
    } 
  }
}

void endBone(){
 FOREACH_FACE(f){
 if(!isValueReceivedOnFaceExpired(f)){ 
    
    legTouchingFace = f;
   
    }

  }

  
    byte boneEnd1 = legTouchingFace + 1 % 6;
    byte boneEnd2 = legTouchingFace + 2 % 6;
    byte boneEnd4 = legTouchingFace + 4 % 6;
    byte boneEnd5 = legTouchingFace + 5 % 6;

      setColorOnFace(WHITE, legTouchingFace);
      setColorOnFace(WHITE, boneEnd1);
      setColorOnFace(WHITE, boneEnd2);
      setColorOnFace(WHITE, boneEnd4);
      setColorOnFace(WHITE, boneEnd5);
  
  }
