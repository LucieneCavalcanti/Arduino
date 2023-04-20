#include <FastLED.h>
#define NUM_LEDS 31
#define DATA_PIN 8
//pride
#define COLOR_ORDER GRB
#define BRIGHTNESS  255
#define LED_TYPE    WS2811
//FIRE
#define SPARKING 120
#define COOLING  55
CRGBPalette16 gPal;
#define FRAMES_PER_SECOND 60
bool gReverseDirection = false;
//jogo
CRGB leds[NUM_LEDS];
int velocidade = 300;
int rodadas = 30;
int apertou = 1;
bool parado = false;
bool iniciar = false;
int acertos = 0;
int speaker = 10;

void setup() { 
    pinMode(12, INPUT_PULLUP);
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
    Serial.begin(9600);
    Serial.println("inicio");
    iniciar=false;
}

void loop() { 
  Serial.print("Acertos:");
  Serial.println(acertos);  
  Serial.print("Velocidade: ");
  Serial.println(velocidade);
  piscarTodosInicio();
  apertou = digitalRead(12);
  if(apertou==LOW) { //APERTOU O BOTÃO
    iniciar=true;
    Serial.println("--------Iniciou-------------");
    apagarTodos();
    Serial.println("------Apagou todos----------");
  }
  if(iniciar==true){
    Serial.print("Variável iniciar: ");
    Serial.println(iniciar);
  apertou=HIGH; //LIMPAR A VARIÁVEL
    Serial.print("Variável apertou: ");
    Serial.println(apertou);
  for(int controdadas=0;controdadas<rodadas;controdadas++){
    // Turn the LED on, then pause
    for(int nled=0;nled<NUM_LEDS;nled++){
      Serial.print("Variável Rodadas: ");
      Serial.println(controdadas);
      Serial.print("Variável Número do led: ");
      Serial.println(nled);
      Serial.print("Variável parado: ");
      Serial.println(parado);
      Serial.print("Acertos:");
      Serial.println(acertos);  
      Serial.print("Velocidade: ");
      Serial.println(velocidade);
      if(parado==false){ //não apertou
        leds[nled] = CRGB::Red;
        FastLED.show();
        apertou = digitalRead(12);
        if(apertou==HIGH){ //não apertou
          delay(velocidade);
          // Now turn the LED off, then pause
          leds[nled] = CRGB::Black;
          FastLED.show();
          delay(velocidade);
        } else { //apertou
          //Serial.println("parado");
          leds[nled] = CRGB::Yellow;
          FastLED.show();
          delay(5000);
          leds[nled] = CRGB::Black;
          //COMPARA SE DEU CERTO NO MEIO E CONTINUA...
          if(nled==15){
            acertos++;
            //emitir som
            acenderTodos();
            delay(1000);
            apagarTodos();
            velocidade-=25;
            nled=0;
            controdadas++;
            continue;
          }else{
            acertos=0;
            controdadas=0;
            nled=0;
            piscarTodosErro();
            tocarSirene(200,10);
            delay(1000);
            apagarTodos();
            velocidade=500;
            break;
          }
        }
    }
    }
  //velocidade-=25;
  }
  }
  
}
void piscarTodosInicio(){
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
    static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;
 
  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);
  
  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5,9);
  uint16_t brightnesstheta16 = sPseudotime;
  
  for( uint16_t i = 0 ; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);
    
    CRGB newcolor = CHSV( hue8, sat8, bri8);
    
    uint16_t pixelnumber = i;
    pixelnumber = (NUM_LEDS-1) - pixelnumber;
    
    nblend( leds[pixelnumber], newcolor, 64);
  }
  FastLED.show();  
}
void apagarTodos(){
  for(int nled=0;nled<NUM_LEDS;nled++){
    leds[nled] = CRGB::Black;
    FastLED.show();
  }
}
void acenderTodos(){
  for(int nled=0;nled<NUM_LEDS;nled++){
    leds[nled] = CRGB::Blue;
    FastLED.show();
  }
}
void piscarTodosErro(){
  FastLED.setBrightness( BRIGHTNESS );
  gPal = HeatColors_p;
  for(int teste=0;teste<150;teste++){
  random16_add_entropy( random());
  //Fire2012WithPalette(); // run simulation frame, using palette colors
  

// Array of temperature readings at each simulation cell
  static uint8_t heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      uint8_t colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
    }
  
  FastLED.show(); // display this frame
  FastLED.delay(1000 / FRAMES_PER_SECOND);
  }
}
  void tocarSirene(int frequencia, int duracao){
    for (int freq = frequencia; freq < frequencia*10; freq++) 
  {
    tone(speaker, freq, duracao); 
    delay(1);
  }
  
  for (int freq = frequencia*10; freq > frequencia; freq --) 
  {
    tone(speaker, freq, duracao); 
    delay(1);
  }
  }
  void tocarTemaMario(){
    //melodia do MARIO THEME
    int melodia[] = {660,660,660,510,660,770,380,510,380,320,440,480,450,430,380,660,760,860,700,760,660,520,580,480,510,380,320,440,480,450,430,380,660,760,860,700,760,660,520,580,480,500,760,720,680,620,650,380,430,500,430,500,570,500,760,720,680,620,650,1020,1020,1020,380,500,760,720,680,620,650,380,430,500,430,500,570,585,550,500,380,500,500,500,500,760,720,680,620,650,380,430,500,430,500,570,500,760,720,680,620,650,1020,1020,1020,380,500,760,720,680,620,650,380,430,500,430,500,570,585,550,500,380,500,500,500,500,500,500,500,580,660,500,430,380,500,500,500,500,580,660,870,760,500,500,500,500,580,660,500,430,380,660,660,660,510,660,770,380};

    //duraçao de cada nota
    int duracaodasnotas[] = {100,100,100,100,100,100,100,100,100,100,100,80,100,100,100,80,50,100,80,50,80,80,80,80,100,100,100,100,80,100,100,100,80,50,100,80,50,80,80,80,80,100,100,100,100,150,150,100,100,100,100,100,100,100,100,100,100,150,200,80,80,80,100,100,100,100,100,150,150,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,150,150,100,100,100,100,100,100,100,100,100,100,150,200,80,80,80,100,100,100,100,100,150,150,100,100,100,100,100,100,100,100,100,100,100,100,100,60,80,60,80,80,80,80,80,80,60,80,60,80,80,80,80,80,60,80,60,80,80,80,80,80,80,100,100,100,100,100,100,100};

      //for para tocar as 156 notas começando no 0 ate 156 ++ incrementado
            for (int nota = 0; nota < 156; nota++) {

                  int duracaodanota = duracaodasnotas[nota];
                  tone(speaker, melodia[nota],duracaodanota);
      //pausa depois das notas
                  int pausadepoisdasnotas[] ={150,300,300,100,300,550,575,450,400,500,300,330,150,300,200,200,150,300,150,350,300,150,150,500,450,400,500,300,330,150,300,200,200,150,300,150,350,300,150,150,500,300,100,150,150,300,300,150,150,300,150,100,220,300,100,150,150,300,300,300,150,300,300,300,100,150,150,300,300,150,150,300,150,100,420,450,420,360,300,300,150,300,300,100,150,150,300,300,150,150,300,150,100,220,300,100,150,150,300,300,300,150,300,300,300,100,150,150,300,300,150,150,300,150,100,420,450,420,360,300,300,150,300,150,300,350,150,350,150,300,150,600,150,300,350,150,150,550,325,600,150,300,350,150,350,150,300,150,600,150,300,300,100,300,550,575};
                  delay(pausadepoisdasnotas[nota]);}

                  noTone(speaker);
    }

