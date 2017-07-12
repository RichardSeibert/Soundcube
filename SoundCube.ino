void play_track(int, int, unsigned char);
int check_button(void);
int freq_note_map(char);

void setup() {
  Serial.begin(9600);
  DDRD |= B00000100; //set pin 2 as output for speaker, all others are inputs
}


void loop() {
  int freq = check_button();
  static int track_pos = -1;
  static int track[256];//= {660, 660, 660, 510, 660, 770, 380, 510, 380, 320, 440, 480, 450, 430, 380, 660, 760, 860, 700, 760, 660, 520, 580, 480}; //mario, 23 track_pos
  unsigned const char tone_length = 160;
  
  if(freq==-1){play_track(track, track_pos, tone_length);}
  else if(freq==-2 && track_pos>=0){track_pos--; tone(2, track[track_pos], 50); delay(50);}
  else if(track_pos!=255 && freq>=0){track_pos++; track[track_pos] = freq; tone(2, track[track_pos], 50); delay(50);}
  else{}
}




void play_track(int track[], int track_pos, unsigned char tone_length){
  for(int i=0; i<=track_pos; i++){
    int real_tone_length = (track[i]!=0) ? tone_length : (tone_length/4); //if its a rest, make it a quater rest by defualt
    tone(2, track[i], real_tone_length); delay(real_tone_length);
    while(digitalRead(5)==HIGH){i=track_pos;} //stops playing track if play button is pressed
  }
}




int check_button(void){
  int freq;
  static char octave = 4;
  bool add = digitalRead(3);
  bool delete1 = digitalRead(4);
  bool play = digitalRead(5);
  
  if(add==HIGH && delete1==HIGH){
  octave = (octave>=9) ? 1 : octave+1; 
    for(int i; i<=octave; i++){
      tone(2, 1000, 300); delay(500);
    }
  }
  else if(add==HIGH){freq = freq_note_map(octave); delay(100);} //add tone button
  else if(delete1==HIGH){freq = -2; delay(100);} //delete button
  else if(play==HIGH){freq = -1; delay(100);} //play track button
  else{freq = -3;} //no button pressed

  return freq;
}




int freq_note_map(char octave){
  int freq = analogRead(A0);
  int mult = 1;
  for(int i=1; i<=octave; i++){mult=mult*2;} //octave multiplier

  const float notes[8] = {0, 16.35, 18.35, 20.6, 21.83, 24.5, 27.5, 30.87}; //notes in octave 0 including rest----- rest,C,D,E,F,G,A,B
  freq = notes[freq*8/1023]*mult;
  return freq;
}
