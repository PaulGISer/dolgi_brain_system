#include <Wire.h>
#include <EEPROM.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_I2CDevice.h>

Adafruit_SSD1306 display(-1);

/* Функция перекодировки русских букв из UTF-8 в Win-1251 */
String utf8rus(String source)
{
  int i,k;
  String target;
  unsigned char n;
  char m[2] = { '0', '\0' };
  k = source.length(); i = 0;
  while (i < k) {
    n = source[i]; i++;
    if (n >= 0xC0) {
      switch (n) {
        case 0xD0: {
          n = source[i]; i++;
          if (n == 0x81) { n = 0xA8; break; }
          if (n >= 0x90 && n <= 0xBF) n = n + 0x30;
          break;
        }
        case 0xD1: {
          n = source[i]; i++;
          if (n == 0x91) { n = 0xB8; break; }
          if (n >= 0x80 && n <= 0x8F) n = n + 0x70;
          break;
        }
      }
    }
    m[0] = n; target = target + String(m);
  }
return target;
}


bool red_pressable = true;
bool yellow_pressable = true;
bool green_pressable = true;
bool blue_pressable = true;
int game = 1;
int buttons_order = 1; // порядок отбивки в свояке
bool game_started;

unsigned long print_time;
unsigned long game_start_time;
unsigned long start_button_press_moment;
unsigned long train_start_moment;
bool start_button_pressed;

int game_start_delay;
int train_start_delay;

float press_moment;
float time_remaind;
float current_time;

void system_reset(){
digitalWrite(6, LOW);
digitalWrite(7, LOW);
digitalWrite(8, LOW);
digitalWrite(9, LOW);
red_pressable = true;
yellow_pressable = true;
green_pressable = true;
blue_pressable = true;

      if (game == 1){
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0,0);
        display.println(utf8rus("БРЕЙН-РИНГ"));
        display.display();
               
        digitalWrite(A0, LOW);
        digitalWrite(A1, LOW);

        game_started = false;
        start_button_pressed = false;
        time_remaind = 60;
        
        display.setCursor(80,0);
        display.setTextSize(2);
        display.println(time_remaind);
        display.display();
      }
      if (game == 2){        
        
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0,0);
        display.println(utf8rus("СИ С ПЕРЕОТ."));
        display.display();

        display.setTextSize(2);
        display.setCursor(1,16);
        display.println(utf8rus("К"));
        display.setCursor(32,16);
        display.println(utf8rus("Ж"));
        display.setCursor(64,16);
        display.println(utf8rus("З"));
        display.setCursor(96,16);
        display.println(utf8rus("С"));
        display.display();
         
      }
      if (game == 3){
        buttons_order = 1;         
               
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0,0);
        display.println(utf8rus("СИ БЕЗ ПЕРЕОТ."));
        display.display();

        display.setTextSize(2);
        display.setCursor(1,16);
        display.println(utf8rus("К"));
        display.setCursor(32,16);
        display.println(utf8rus("Ж"));
        display.setCursor(64,16);
        display.println(utf8rus("З"));
        display.setCursor(96,16);
        display.println(utf8rus("С"));
        display.display();     
      }
      if (game == 4){
        train_start_delay = random(5000, 15000);  // случайная задержка звукового сигнала, чтобы игроки не могли реагировать на щелчок кнопки
        train_start_moment = millis();      
        start_button_pressed = true;
        game_started = false;
        time_remaind = 60;
        digitalWrite(A0, LOW);
        digitalWrite(A1, LOW);
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0,0);
        display.println(utf8rus("ТРЕНИРОВКА"));
        display.setTextSize(2);
        
        display.setCursor(80,0);
        display.println(time_remaind);
        display.display();
      }
}

void choose_game(){
  if (game == 1){
        time_remaind = 60;
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0,0);
        display.println(utf8rus("БРЕЙН-РИНГ"));
        display.display();
        
        display.setCursor(80,0);
        display.println(time_remaind);
        display.display();
        
        delay(500);           
      }
      if (game == 2){
        
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0,0);
        display.println(utf8rus("СИ С ПЕРЕОТ."));        

        display.setTextSize(2);
        display.setCursor(1,16);
        display.println(utf8rus("К"));
        display.setCursor(32,16);
        display.println(utf8rus("Ж"));
        display.setCursor(64,16);
        display.println(utf8rus("З"));
        display.setCursor(96,16);
        display.println(utf8rus("С"));
        display.display();
                 
        delay(500);     
      }
      if (game == 3){
        buttons_order = 1;  
        
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0,0);
        display.println(utf8rus("СИ БЕЗ ПЕРЕОТ."));        

        display.setTextSize(2);
        display.setCursor(1,16);
        display.println(utf8rus("К"));
        display.setCursor(32,16);
        display.println(utf8rus("Ж"));
        display.setCursor(64,16);
        display.println(utf8rus("З"));
        display.setCursor(96,16);
        display.println(utf8rus("С"));
        display.display();       
        
        delay(500);    
      }
      if (game == 4){
       
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0,0);
        display.println(utf8rus("ТРЕНИРОВКА"));
        display.display();
        
        delay(500);
        train_start_delay = random(5000, 15000);
      }
      EEPROM.update(0,game);
}

void setup() {
  // put your setup code here, to run once:
  
pinMode(2, INPUT_PULLUP);
pinMode(3,INPUT_PULLUP);
pinMode(4,INPUT_PULLUP);
pinMode(5,INPUT_PULLUP);
pinMode(6,OUTPUT); 
pinMode(7,OUTPUT);
pinMode(8,OUTPUT);
pinMode(9,OUTPUT);
pinMode(10, OUTPUT);

pinMode(A0,OUTPUT);
pinMode(A1,OUTPUT);

pinMode(A2, INPUT_PULLUP);
pinMode(A3, INPUT_PULLUP);

display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
display.cp437(true);


display.clearDisplay();
display.setTextSize(2);
display.setTextColor(WHITE);
display.setCursor(0,0);
display.println(utf8rus("ПРИВЕТ!"));
display.display();
delay(500);
display.setCursor(0,16);
display.println(utf8rus("ПОИГРАЕМ?"));
display.display();

delay(2000);

Serial.begin(9600);

game = EEPROM.read(0);
choose_game();

}

void loop() {
  
  // выбор режима игры
  if (digitalRead(A2) == LOW && digitalRead(A3)==LOW){ 
    long game_set_moment = millis();
  while (digitalRead(A2) == LOW && digitalRead(A3)==LOW){   
      
    if (millis() - game_set_moment > 2000){
      game += 1;
      choose_game();
      system_reset();
      if (game == 5){
        game = 0;
      }
      }      
      continue;
      }
  }

// БРЕЙН-РИНГ
  if (game == 1){
    // старт таймера
    if (digitalRead(A2) == LOW){
      game_start_delay = random(500, 1500);     // случайная задержка звукового сигнала, чтобы игроки не могли реагировать на щелчок кнопки
      start_button_press_moment = millis();      
      start_button_pressed = true;
    }

    if (!game_started && start_button_pressed && millis() - start_button_press_moment >= game_start_delay){ // звуковой сигнал не должен работать при повторном нажатии кнопки старта
          game_started = true;
          game_start_time = millis();
          tone(10, 800, 500);   
          start_button_pressed = false;
          display.setTextColor(WHITE, BLACK);
          display.setTextSize(2);    
      }

      // таймер
    if (game_started && (millis() - game_start_time) % 10 == 0){
      time_remaind -= 0.01;
      Serial.println(time_remaind);
      //if ((millis() - game_start_time) % 1000 == 0){
      //display.setCursor(80, 0);      
     // display.println(time_remaind);
     // display.display();
     // }
    }
    
    if (time_remaind <= 0.0){ // сигнал об окончании времени
      tone(10, 800, 1000);
      system_reset();
    }

    // сигналы за 5 секунд до окончания времени
    if (time_remaind <= 10 && time_remaind > 9.95){
      tone(10, 800, 50);
    }

    if (time_remaind <= 5 && time_remaind > 4.95){
      tone(10, 800, 20);
    }

    if (time_remaind <= 4 && time_remaind > 3.95){
      tone(10, 800, 20);
    }

    if (time_remaind <= 3 && time_remaind > 2.95){
      tone(10, 800, 20);
    }
    if (time_remaind <= 2 && time_remaind > 1.95){
      tone(10, 800, 20);
    }
    if (time_remaind <= 1 && time_remaind > 0.95){
      tone(10, 800, 100);
    }

    // сброс системы во время работы таймера
    if (digitalRead(A3) == LOW && game_started){
      game_started = false;
      system_reset();
    }

    // красная кнопка
      if (digitalRead(2) == LOW && red_pressable){
      red_pressable = false;
      yellow_pressable = false;
      green_pressable = false;
      blue_pressable = false;
      digitalWrite(6,HIGH);
      if (!game_started){             // случай фальстарта
          digitalWrite(A0, HIGH);  
          tone(10, 600, 1000);  
          
          display.setCursor(1,16);
          display.setTextSize(2);
          display.setTextColor(WHITE);
          display.println(utf8rus("K ФАЛЬСТАРТ!"));
          display.display();
          while(true){
            if (digitalRead(A3) == LOW){
              system_reset();
              break;
            }
      }
      }else{                            // красная кнопка без фальстарта
        digitalWrite(A1, HIGH);
        tone(10, 800, 800);
       
        display.setTextSize(2);
        display.setCursor(1,16);
        display.println(utf8rus("K "));
        display.setCursor(32,16);
        display.println(60-time_remaind);
        display.display();
        while(true){
          if (digitalRead(A3) == LOW){
              system_reset();
              break;
            }
          if (digitalRead(A2) == LOW){
            float time_remaind_backup = time_remaind;
            system_reset();
            time_remaind =  time_remaind_backup;
            display.setCursor(80, 0);
            display.setTextSize(1);
            display.setTextColor(WHITE, BLACK);
            display.println(time_remaind);
            display.display();
            break;
          }
        }
      }
    }

// желтая кнопка
if (digitalRead(3) == LOW && yellow_pressable){
      red_pressable = false;
      yellow_pressable = false;
      green_pressable = false;
      blue_pressable = false;
      digitalWrite(7,HIGH);
      if (!game_started){               // случай фальстарта
          digitalWrite(A0, HIGH);    
          tone(10, 600, 1000);
          
          display.setCursor(1,16);
          display.setTextSize(2);
          display.setTextColor(WHITE);
          display.println(utf8rus("Ж ФАЛЬСТАРТ!"));
          display.display();
          
          while(true){
            if (digitalRead(A3) == LOW){
              system_reset();
              break;
            }
      }
      }else{                          // желтая кнопка без фальстарта
        digitalWrite(A1, HIGH);
        tone(10,800, 800);
        
        display.setTextSize(2);
        display.setCursor(1,16);
        display.println(utf8rus("Ж "));
        display.setCursor(32,16);
        display.println(60-time_remaind);
        display.display();
        while(true){
          if (digitalRead(A3) == LOW){
              system_reset();
              break;
            }
          if (digitalRead(A2) == LOW){
            float time_remaind_backup = time_remaind;
            system_reset();
            time_remaind =  time_remaind_backup;            
            display.setCursor(80, 0);
            display.setTextSize(1);
            display.setTextColor(WHITE, BLACK);
            display.println(time_remaind);
            display.display();
            break;
          }
        }
      }
    }


// зеленая кнопка
    if (digitalRead(4) == LOW && green_pressable){
      red_pressable = false;
      yellow_pressable = false;
      green_pressable = false;
      blue_pressable = false;
      digitalWrite(8,HIGH);
      if (!game_started){           // случай фальстарта
          digitalWrite(A0, HIGH);
          tone(10, 600, 1000);
          
          display.setCursor(1,16);
          display.setTextSize(2);
          display.setTextColor(WHITE);
          display.println(utf8rus("З ФАЛЬСТАРТ!"));
          display.display();
          
          while(true){
            if (digitalRead(A3) == LOW){
              system_reset();
              break;
            }
      }
      }else{                        // зеленая кнопка без фальстарта
        digitalWrite(A1, HIGH);
        tone(10, 800, 800);
        
        display.setTextSize(2);
        display.setCursor(1,16);
        display.println(utf8rus("З "));
        display.setCursor(32,16);
        display.println(60-time_remaind);
        display.display();
        
        while(true){
          if (digitalRead(A3) == LOW){
              system_reset();
              break;
            }
          if (digitalRead(A2) == LOW){
            float time_remaind_backup = time_remaind;
            system_reset();
            time_remaind =  time_remaind_backup;
            display.setCursor(80, 0);
            display.setTextSize(1);
            display.setTextColor(WHITE, BLACK);
            display.println(time_remaind);
            display.display();
            break;
          }
        }
      }
    }


// синяя кнопка
    if (digitalRead(5) == LOW && blue_pressable){
      red_pressable = false;
      yellow_pressable = false;
      green_pressable = false;
      blue_pressable = false;
      digitalWrite(9,HIGH);
      if (!game_started){                 // случай фальстарта
          digitalWrite(A0, HIGH);   
          tone(10, 600, 1000); 
          display.setCursor(1,16);
          display.setTextSize(2);
          display.setTextColor(WHITE);
          display.println(utf8rus("С ФАЛЬСТАРТ!"));
          display.display();
          while(true){
            if (digitalRead(A3) == LOW){
              system_reset();
              break;
            }
      }
      }else{                              // синяя кнопка без фальстарта
        digitalWrite(A1, HIGH);
        tone(10,800,800);
        display.setTextSize(2);
        display.setCursor(1,16);
        display.println(utf8rus("С "));
        display.setCursor(32,16);
        display.println(60-time_remaind);
        display.display();
        while(true){
          if (digitalRead(A3) == LOW){
              system_reset();
              break;
            }
          if (digitalRead(A2) == LOW){
            float time_remaind_backup = time_remaind;
            system_reset();
            time_remaind =  time_remaind_backup;
            display.setCursor(80, 0);
            display.setTextSize(1);
            display.setTextColor(WHITE, BLACK);
            display.println(time_remaind);
            display.display();
            break;
          }            
        }
      }      
    }    
  }
  
// СВОЯК С ПЕРЕОТБИВКАМИ
  if (game == 2){
    if (digitalRead(2) == LOW && red_pressable){
      red_pressable = false;
      yellow_pressable = false;
      green_pressable = false;
      blue_pressable = false;
      digitalWrite(6, HIGH);
      tone(10, 800, 800);

      display.setTextSize(2);
      display.fillRect(0,15,15,16,WHITE);
      display.setTextColor(BLACK);
      display.setCursor(2,16);  
      display.println(utf8rus("К"));
      display.display();      
      
      while(true){
        if (digitalRead(A3) == LOW){
          system_reset();
          break;
        }
      }
    }

    if (digitalRead(3) == LOW && yellow_pressable){
      red_pressable = false;
      yellow_pressable = false;
      green_pressable = false;
      blue_pressable = false;
      digitalWrite(7, HIGH);
      tone(10, 800, 800);

      display.setTextSize(2);
      display.fillRect(30,15,15,16,WHITE);
      display.setTextColor(BLACK);
      display.setCursor(32,16);  
      display.println(utf8rus("Ж"));
      display.display();
      
      while(true){
        if (digitalRead(A3) == LOW){
          system_reset();
          break;
        }
      }
    }

    if (digitalRead(4) == LOW && green_pressable){
      red_pressable = false;
      yellow_pressable = false;
      green_pressable = false;
      blue_pressable = false;
      digitalWrite(8, HIGH);
      tone(10, 800, 800);

      display.setTextSize(2);
      display.fillRect(63,15,15,16,WHITE);
      display.setTextColor(BLACK);
      display.setCursor(64,16);  
      display.println(utf8rus("З"));
      display.display();
      
      while(true){
        if (digitalRead(A3) == LOW){
          system_reset();
          break;
        }
      }
    }

    if (digitalRead(5) == LOW && blue_pressable){
      red_pressable = false;
      yellow_pressable = false;
      green_pressable = false;
      blue_pressable = false;
      digitalWrite(9, HIGH);
      tone(10, 800, 800);

      display.setTextSize(2);
      display.fillRect(95,15,15,16,WHITE);
      display.setTextColor(BLACK);
      display.setCursor(96,16);  
      display.println(utf8rus("С"));
      display.display();
      
      while(true){
        if (digitalRead(A3) == LOW){
          system_reset();
          break;
        }
      }
    }    
  }


  // СВОЯК БЕЗ ПЕРЕОТБИВОК
  if (game == 3){
    if (digitalRead(2) == LOW && red_pressable){
      red_pressable = false;     
      digitalWrite(6, HIGH);
      tone(10, 800, 800);

      display.setTextSize(2);
      display.fillRect(0,15,15,16,WHITE);
      display.setTextColor(BLACK);
      display.setCursor(2,16);  
      display.println(utf8rus("К"));
      display.setTextColor(WHITE);
      display.setCursor(16,16);        
      display.println(buttons_order); 
      display.display();      
         
      buttons_order += 1;
    }

    if (digitalRead(3) == LOW && yellow_pressable){
     
      yellow_pressable = false;
      
      digitalWrite(7, HIGH);
      tone(10, 800, 800);
      
      display.setTextSize(2);
      display.fillRect(30,15,15,16,WHITE);
      display.setTextColor(BLACK);
      display.setCursor(32,16);  
      display.println(utf8rus("Ж"));
      display.setTextColor(WHITE);
      display.setCursor(48,16);        
      display.println(buttons_order); 
      display.display();          
        
      buttons_order += 1;      
    }

    if (digitalRead(4) == LOW && green_pressable){
      
      green_pressable = false;      
      digitalWrite(8, HIGH);
      tone(10, 800, 800);
      
      display.setTextSize(2);
      display.fillRect(63,15,15,16,WHITE);
      display.setTextColor(BLACK);
      display.setCursor(64,16);  
      display.println(utf8rus("З"));
      display.setTextColor(WHITE);
      display.setCursor(80,16);        
      display.println(buttons_order); 
      display.display();              
       
      buttons_order += 1;     
    }

    if (digitalRead(5) == LOW && blue_pressable){
     
      blue_pressable = false;
      digitalWrite(9, HIGH);
      tone(10, 800, 800);
      
      display.setTextSize(2);
      display.fillRect(95,15,15,16,WHITE);
      display.setTextColor(BLACK);
      display.setCursor(96,16);  
      display.println(utf8rus("С"));
      display.setTextColor(WHITE);
      display.setCursor(112,16);        
      display.println(buttons_order); 
      display.display();          
         
      buttons_order += 1;      
    }    
    if (digitalRead(A3) == LOW && (!red_pressable || !yellow_pressable || !green_pressable || !blue_pressable)){
      system_reset();
    }
  }
  
  // ТРЕНИРОВКА
  if (game == 4){
    // старт таймера  

    if (!game_started && start_button_pressed && millis() - train_start_moment >= train_start_delay){ // звуковой сигнал не должен работать при повторном нажатии кнопки старта
          game_started = true;
          game_start_time = millis();
          tone(10, 800, 500);   
          start_button_pressed = false;    
      }

      // таймер
    if (game_started && (millis() - game_start_time) % 10 == 0){
      time_remaind -= 0.01;
      
      display.setCursor(80, 0);
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.println(time_remaind);
      display.display();
    }
    
    if (time_remaind <= 0.0){ // сигнал об окончании времени
      tone(10, 800, 1000);
      system_reset();
    }

    // сигналы за 5 секунд до окончания времени
    if (time_remaind < 10 && time_remaind > 9.95){
      tone(10, 800, 200);
    }

    if (time_remaind < 5 && time_remaind > 4.95){
      tone(10, 800, 150);
    }

    if (time_remaind < 4 && time_remaind > 3.95){
      tone(10, 800, 150);
    }

    if (time_remaind < 3 && time_remaind > 2.95){
      tone(10, 800, 150);
    }
    if (time_remaind < 2 && time_remaind > 1.95){
      tone(10, 800, 150);
    }
    if (time_remaind < 1 && time_remaind > 0.95){
      tone(10, 800, 200);
    }

      // красная кнопка
      if (digitalRead(2) == LOW && red_pressable){
      red_pressable = false;
      yellow_pressable = false;
      green_pressable = false;
      blue_pressable = false;
      digitalWrite(6,HIGH);
      if (!game_started){             // случай фальстарта
          digitalWrite(A0, HIGH);  
          tone(10, 600, 1000);  
          
          display.setCursor(1,16);
          display.setTextSize(2);
          display.setTextColor(WHITE);
          display.println("K ФАЛЬСТАРТ!");
          display.display();
          
          delay(5000);
          system_reset();
      }else{                            // красная кнопка без фальстарта
        digitalWrite(A1, HIGH);
        tone(10, 800, 800);
        
        display.setTextSize(2);
        display.setCursor(1,16);
        display.println("K ");
        display.setCursor(32,16);
        display.println(60-time_remaind);
        display.display();
        
        delay(5000);
          system_reset();
      }
    }

// желтая кнопка
if (digitalRead(3) == LOW && yellow_pressable){
      red_pressable = false;
      yellow_pressable = false;
      green_pressable = false;
      blue_pressable = false;
      digitalWrite(7,HIGH);
      if (!game_started){               // случай фальстарта
          digitalWrite(A0, HIGH);    
          tone(10, 600, 1000);
          
          display.setCursor(1,16);
          display.setTextSize(2);
          display.setTextColor(WHITE);
          display.println("Ж ФАЛЬСТАРТ!");
          display.display();
          
          delay(5000);
          system_reset();
      }else{                          // желтая кнопка без фальстарта
        digitalWrite(A1, HIGH);
        tone(10,800, 800);
        
        display.setTextSize(2);
        display.setCursor(1,16);
        display.println("Ж ");
        display.setCursor(32,16);
        display.println(60-time_remaind);
        display.display();
        
        delay(5000);
        system_reset();
      }
    }


// зеленая кнопка
    if (digitalRead(4) == LOW && green_pressable){
      red_pressable = false;
      yellow_pressable = false;
      green_pressable = false;
      blue_pressable = false;
      digitalWrite(8,HIGH);
      if (!game_started){           // случай фальстарта
          digitalWrite(A0, HIGH);
          tone(10, 600, 1000);
          
          display.setCursor(1,16);
          display.setTextSize(2);
          display.setTextColor(WHITE);
          display.println("З ФАЛЬСТАРТ!");
          display.display();
          
          delay(5000);
          system_reset();
      }else{                        // зеленая кнопка без фальстарта
        digitalWrite(A1, HIGH);
        tone(10, 800, 800);
        
        display.setTextSize(2);
        display.setCursor(1,16);
        display.println("З ");
        display.setCursor(32,16);
        display.println(60-time_remaind);
        display.display();
        
        delay(5000);
        system_reset();
      }
    }

// синяя кнопка
    if (digitalRead(5) == LOW && blue_pressable){
      red_pressable = false;
      yellow_pressable = false;
      green_pressable = false;
      blue_pressable = false;
      digitalWrite(9,HIGH);
      if (!game_started){                 // случай фальстарта
          digitalWrite(A0, HIGH);   
          tone(10, 600, 1000); 
          
          display.setCursor(1,16);
          display.setTextSize(2);
          display.setTextColor(WHITE);
          display.println("С ФАЛЬСТАРТ!");
          display.display();
          
          delay(5000);
          system_reset();
      }else{                              // синяя кнопка без фальстарта
        digitalWrite(A1, HIGH);
        tone(10,800,800);
        
        display.setTextSize(2);
        display.setCursor(1,16);
        display.println("С ");
        display.setCursor(32,16);
        display.println(60-time_remaind);
        display.display();
        
        delay(5000);
        system_reset();
        }      
      }    
    }
  }
  
  


