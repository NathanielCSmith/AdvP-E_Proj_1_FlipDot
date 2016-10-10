// board protocol
//-------
// Header   0x80
//-------
// command:
//          0x87  : show data on a display immediately
//          0x88  : send data  and wait for refresh
//          0x82  : refresh (show data from memory)
//-------
// address
//-------
// data x7 (NULL if 0x82)
//-------
// End byte       0x8F


// Arduino Duo: Controller and Communicator to Interface
// rs485 breakout
// connections:
// Breakout - Arduino
// GND  -   GND
// RTS  -   3V3
// TX-O  -   not connected
// RX-I  -   TX (pin 1 digital)
// 3-5V  -   5V
// Button - normally open **not sure what this is?
// between  pin 3 and GND **this is false for my board set up

// set time here
//int offset = 0;
//int offset_dir = 1;
//int godz = 10;
//int godz_dz = 0;
//int godz_jd = 0;
//int mnt = 25;
//int mnt_dz = 0;
//int mnt_jd = 0;
//int sek = 21;
//int sek_dz = 0;
//int sek_jd = 0;

byte send_data_buffer [] = { 0x80, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8F }; // template and 'all black' defintion for display 0, immediate refresh

byte _display[40] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} ;

//hex code array example
//can comment out, but need to pull something else out below
//static unsigned char Font5x7[] = {
//  0x3E, 0x45, 0x49, 0x51, 0x3E, 0x00, // 0 ok
//  0x00, 0x21, 0x7F, 0x01, 0x00, 0x00, // 1 ok
//  0x21, 0x43, 0x45, 0x49, 0x31, 0x00, // 2 ok
//  0x42, 0x41, 0x51, 0x69, 0x46, 0x00, // 3 ok
//  0x0C, 0x14, 0x24, 0x7F, 0x04, 0x00, // 4 OK
//  0x72, 0x51, 0x51, 0x51, 0x4E, 0x00, // 5 OK
//  0x1E, 0x29, 0x49, 0x49, 0x06, 0x00, // 6 ok
//  0x40, 0x47, 0x48, 0x50, 0x60, 0x00, // 7 OK
//  0x36, 0x49, 0x49, 0x49, 0x36, 0x00, // 8 OK
//  0x30, 0x49, 0x49, 0x4A, 0x3C, 0x00, // 9
//  0x00, 0x36, 0x36, 0x00, 0x00, 0x00  // :
//};

static unsigned char sequence[] = {
  0x80,
  0x87,
  0x00,
  0b01111111,
  0b01000001,
  0b01000001,
  0b01000001,
  0b01000001,
  0b01000001,
  0b01111111,
  0x8f
};

static unsigned char sequence2[] = {
  0x80,
  0x87,
  0x00,
  0b00000000,
  0b00111110,
  0b00100010,
  0b00100010,
  0b00100010,
  0b00111110,
  0b00000000,
  0x8f
};

static unsigned char sequence3[] = {
  0x80,
  0x87,
  0x00,
  0b00000000,
  0b00000000,
  0b00011100,
  0b00010100,
  0b00011100,
  0b00000000,
  0b00000000,
  0x8f
};

static unsigned char sequence4[] = {
  0x80,
  0x87,
  0x00,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00001000,
  0b00000000,
  0b00000000,
  0b00000000,
  0x8f
};

// show content of display array on a display address n, starting from column z

void show_display(byte n, byte z)
{

  int x = 0;
  int y = 0;
  byte _data[8] = {0, 0, 0, 0, 0, 0, 0, 0};

  for (x = 0; x < 7; x++)_data[x] = _display[z + x];

  //Serial.print("całość:\n");
  //for (x=0;x<40;x++) Serial.println(_display[x]);
  //Serial.print("7x7\n");
  //for (x=0;x<7;x++) Serial.println(_data[x]);
  //Serial.print("---\n");


  Serial.write(0x80);
  Serial.write(0x87);
  Serial.write(n);

  for (x = 0; x < 7; x++) Serial.write(_data[x]);


  Serial.write(0x8F);

};


void set_display(byte state, byte n)

{

  int x;

  Serial.write(0x80);
  Serial.write(0x87);
  Serial.write(n);

  for (x = 0; x < 7; x++) Serial.write(state);

  Serial.write(0x8F);

};



void show_char(char letter, byte n)
{
  int x;
  Serial.write(0x80); // header
  Serial.write(0x87); // header
  Serial.write(n);    // address

//hex code array example with a for loop call
//  for (x = 0; x < 5; x++) Serial.write(Font5x7[letter + x]); //send 5 bytes from char definition
//  for (x = 5; x < 7; x++) Serial.write(0x00); // send remaing 2 to make total of 7

  Serial.write(0x8F); // eot
};

void setup() {
  Serial.begin(9600);
}


void loop()
{
  int a;
  set_display(255 , 0); //set display 0 to 255 - all dots set
  //  set_display(64 | 16 , 0); //set display 0 to 255 - all dots set
  // this is the bitwise or operator (read about me!)
  delay(1000);
  set_display(0, 0); // set display 0 to 0 - all dots reset
  delay(1000);
  set_display(0, 255); //extra
  delay(1000);
  for (int i = 0; i < sizeof(sequence); i++) { //counter, repeat body of loop as long as i < seq (array), i=i+1
    Serial.write(sequence[i]);  //select byte #i of sequence
  }
  delay(1000);
  for (int i = 0; i < sizeof(sequence2); i++) { //counter, repeat body of loop as long as i < seq (array), i=i+1
    Serial.write(sequence2[i]);  //select byte #i of sequence
  }
  delay(1000);
  for (int i = 0; i < sizeof(sequence3); i++) { //counter, repeat body of loop as long as i < seq (array), i=i+1
    Serial.write(sequence3[i]);  //select byte #i of sequence
  }
  delay(1000);
  for (int i = 0; i < sizeof(sequence4); i++) { //counter, repeat body of loop as long as i < seq (array), i=i+1
    Serial.write(sequence4[i]);  //select byte #i of sequence
  }
  delay(1000);
  for (int i = 0; i < sizeof(sequence3); i++) { //counter, repeat body of loop as long as i < seq (array), i=i+1
    Serial.write(sequence3[i]);  //select byte #i of sequence
  }
  delay(1000);
  for (int i = 0; i < sizeof(sequence2); i++) { //counter, repeat body of loop as long as i < seq (array), i=i+1
    Serial.write(sequence2[i]);  //select byte #i of sequence
  }
  delay(1000);
  for (int i = 0; i < sizeof(sequence); i++) { //counter, repeat body of loop as long as i < seq (array), i=i+1
    Serial.write(sequence[i]);  //select byte #i of sequence
  }
  delay(1000);
}

//debug log:
      // these are blank transmissions for various controller configurations: CE - one controller + one extension, 2C - two controllers (the most popular), C3E - one controller + threee extensions
      // you can use them by putting data bytes
      
      //byte sign_CE[] = {0x80, 0x85, 0x01, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      //                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8F
      //                 };
      
      // dark / bright transmissions for configurations above
      //I changed one (5) pixel code to see if it worked and I can tell it from reset
      //debug log 1-these didn't do anything
      //  byte all_bright_CE[] = {0x80, 0x85, 0xFF, 0x7F, 0x00, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
      //                          0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x8F
      //                         };
      //
      //  byte all_dark_CE[] = {0x80, 0x85, 0xFF, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      //                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8F
      //                       };
// FINAL SOLUTION:
// BUILD AN ARRAY OF DATA BUT DONT USE HEX TRANSFORM HEX TO BIANARY AND MAKE THE 
// ARRAY IN BIANARY, ITS WAY EASIER TO UNDERSTAND AND LOOK AT 
// FOR YOUR REFERENCE GO LOOK UP THE BIANARY HEX REFERENCE
    // HEXIDECIMAL REFERENCE: https://www.prepressure.com/library/technology/ascii-binary-hex
    // HEX BIANARY REFERENCE2: http://www.computerhope.com/binhex.htm


