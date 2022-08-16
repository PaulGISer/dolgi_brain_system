#include <Wire.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>

byte bukva_B[8]   = {B11110,B10000,B10000,B11110,B10001,B10001,B11110,B00000,}; // Буква "Б"
byte bukva_G[8]   = {B11111,B10001,B10000,B10000,B10000,B10000,B10000,B00000,}; // Буква "Г"
byte bukva_D[8]   = {B01111,B00101,B00101,B01001,B10001,B11111,B10001,B00000,}; // Буква "Д"
byte bukva_ZH[8]  = {B10101,B10101,B10101,B11111,B10101,B10101,B10101,B00000,}; // Буква "Ж"
byte bukva_Z[8]   = {B01110,B10001,B00001,B00010,B00001,B10001,B01110,B00000,}; // Буква "З"
byte bukva_I[8]   = {B10001,B10011,B10011,B10101,B11001,B11001,B10001,B00000,}; // Буква "И"
byte bukva_IY[8]  = {B01110,B00000,B10001,B10011,B10101,B11001,B10001,B00000,}; // Буква "Й"
byte bukva_L[8]   = {B00011,B00111,B00101,B00101,B01101,B01001,B11001,B00000,}; // Буква "Л"
byte bukva_P[8]   = {B11111,B10001,B10001,B10001,B10001,B10001,B10001,B00000,}; // Буква "П"
byte bukva_Y[8]   = {B10001,B10001,B10001,B01010,B00100,B01000,B10000,B00000,}; // Буква "У"
byte bukva_F[8]   = {B00100,B11111,B10101,B10101,B11111,B00100,B00100,B00000,}; // Буква "Ф"
byte bukva_TS[8]  = {B10010,B10010,B10010,B10010,B10010,B10010,B11111,B00001,}; // Буква "Ц"
byte bukva_CH[8]  = {B10001,B10001,B10001,B01111,B00001,B00001,B00001,B00000,}; // Буква "Ч"
byte bukva_Sh[8]  = {B10101,B10101,B10101,B10101,B10101,B10101,B11111,B00000,}; // Буква "Ш"
byte bukva_Shch[8]= {B10101,B10101,B10101,B10101,B10101,B10101,B11111,B00001,}; // Буква "Щ"
byte bukva_Mz[8]  = {B10000,B10000,B10000,B11110,B10001,B10001,B11110,B00000,}; // Буква "Ь"
byte bukva_IYI[8] = {B10001,B10001,B10001,B11001,B10101,B10101,B11001,B00000,}; // Буква "Ы"
byte bukva_Yu[8]  = {B10010,B10101,B10101,B11101,B10101,B10101,B10010,B00000,}; // Буква "Ю"
byte bukva_Ya[8]  = {B01111,B10001,B10001,B01111,B00101,B01001,B10001,B00000,}; // Буква "Я"


LiquidCrystal_I2C lcd(0x27, 16, 2);
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
        lcd.clear();
        lcd.print(char(5)); 
        lcd.print("PE");
        lcd.print(char(4));
        lcd.print("H"); 
        digitalWrite(A0, LOW);
        digitalWrite(A1, LOW);

        game_started = false;
        start_button_pressed = false;
        time_remaind = 60.00;
        
        lcd.setCursor(7,0);
        lcd.print(time_remaind);
      }
      if (game == 2){
        lcd.clear();
        
        lcd.print("C");
        lcd.print(char(1));
        lcd.print(" C ");
        lcd.print(char(0));
        lcd.print("EPEOT.");
        
        lcd.setCursor(0,1);
        lcd.print("K");
        lcd.setCursor(4,1);
        lcd.print(char(6));
        lcd.setCursor(8,1);
        lcd.print(char(7));
        lcd.setCursor(12,1);
        lcd.print("C");
         
      }
      if (game == 3){
        lcd.clear();        
        lcd.print("C");
        lcd.print(char(1));
        lcd.print(" ");
        lcd.print(char(5));        
        lcd.print("E");
        lcd.print(char(7));
        lcd.print(" ");
        lcd.print(char(0));
        lcd.print("EPEOT.");        
        buttons_order = 1;        
        lcd.setCursor(0,1);
        lcd.print("K");
        lcd.setCursor(4,1);
        lcd.print(char(6));
        lcd.setCursor(8,1);
        lcd.print(char(7));
        lcd.setCursor(12,1);
        lcd.print("C");        
      }
      if (game == 4){
        train_start_delay = random(5000, 15000);  // случайная задержка звукового сигнала, чтобы игроки не могли реагировать на щелчок кнопки
        train_start_moment = millis();      
        start_button_pressed = true;
        game_started = false;
        time_remaind = 60.00;
        digitalWrite(A0, LOW);
        digitalWrite(A1, LOW);
        lcd.clear();
        lcd.print("TPEH");
        lcd.print(char(0));
        lcd.print("POBKA");
        lcd.setCursor(11,0);
        lcd.print(time_remaind);
      }
}

void choose_game(){
  if (game == 1){
        create_brain_chars_set();
        lcd.clear();
        lcd.print(char(5)); 
        lcd.print("PE");
        lcd.print(char(4));
        lcd.print("H"); 
        time_remaind = 60.00;
        lcd.setCursor(7,0);
        lcd.print(time_remaind);
        delay(500);           
      }
      if (game == 2){
        create_chars_set();
        lcd.clear();
        lcd.print("C");
        lcd.print(char(1));
        lcd.print(" C ");
        lcd.print(char(0));
        lcd.print("EPEOT.");        
        lcd.setCursor(0,1);
        lcd.print("K");
        lcd.setCursor(4,1);
        lcd.print(char(6));
        lcd.setCursor(8,1);
        lcd.print(char(7));
        lcd.setCursor(12,1);
        lcd.print("C");           
        delay(500);     
      }
      if (game == 3){
        create_chars_set();
        lcd.clear();
        lcd.print("C");
        lcd.print(char(1));
        lcd.print(" ");
        lcd.print(char(5));        
        lcd.print("E");
        lcd.print(char(7));
        lcd.print(" ");
        lcd.print(char(0));
        lcd.print("EPEOT.");
        buttons_order = 1;        
        lcd.setCursor(0,1);
        lcd.print("K");
        lcd.setCursor(4,1);
        lcd.print(char(6));
        lcd.setCursor(8,1);
        lcd.print(char(7));
        lcd.setCursor(12,1);
        lcd.print("C");          
        
        delay(500);    
      }
      if (game == 4){
        create_brain_chars_set();
        lcd.clear();
        lcd.print("TPEH");
        lcd.print(char(0));
        lcd.print("POBKA");
        delay(500);
        train_start_delay = random(5000, 15000);
      }
      EEPROM.update(0,game);
}

void create_chars_set(){
  lcd.createChar(0,bukva_P);
  lcd.createChar(1,bukva_I);
  lcd.createChar(2,bukva_IYI);
  lcd.createChar(3,bukva_G);
  lcd.createChar(4,bukva_IY);
  lcd.createChar(5,bukva_B);
  lcd.createChar(6, bukva_ZH);
  lcd.createChar(7, bukva_Z);
}

void create_brain_chars_set(){
  lcd.createChar(0,bukva_I);
  lcd.createChar(1,bukva_Mz);
  lcd.createChar(2,bukva_F);
  lcd.createChar(3,bukva_L);
  lcd.createChar(4,bukva_IY);
  lcd.createChar(5,bukva_B);
  lcd.createChar(6, bukva_ZH);
  lcd.createChar(7, bukva_Z);
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

lcd.init();
lcd.backlight();

create_chars_set();
lcd.home();

lcd.print(char(0));
lcd.print("P");
lcd.print(char(1));
lcd.print("BET, C");
lcd.print(char(2));
lcd.print(char(3));
lcd.print("PAEM?");

delay(2000);

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
      }

      // таймер
    if (game_started && (millis() - game_start_time) % 10 == 0){
      time_remaind -= 0.01;
      lcd.setCursor(7, 0);
      lcd.print(time_remaind);
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
          lcd.setCursor(0,1);
          lcd.print("K ");
          lcd.print(char(2));
          lcd.print("A");
          lcd.print(char(3));
          lcd.print(char(1));
          lcd.print("CTAPT!");
          while(true){
            if (digitalRead(A3) == LOW){
              system_reset();
              break;
            }
      }
      }else{                            // красная кнопка без фальстарта
        digitalWrite(A1, HIGH);
        tone(10, 800, 800);
        lcd.setCursor(0,1);
        lcd.print("K ");
        lcd.print(60-time_remaind);
        while(true){
          if (digitalRead(A3) == LOW){
              system_reset();
              break;
            }
          if (digitalRead(A2) == LOW){
            float time_remaind_backup = time_remaind;
            system_reset();
            time_remaind =  time_remaind_backup;
            lcd.setCursor(7,0);
            lcd.print(time_remaind);
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
          lcd.setCursor(0,1);
          lcd.print(char(6));
          lcd.print(" ");
          lcd.print(char(2));
          lcd.print("A");
          lcd.print(char(3));
          lcd.print(char(1));
          lcd.print("CTAPT!");
          while(true){
            if (digitalRead(A3) == LOW){
              system_reset();
              break;
            }
      }
      }else{                          // желтая кнопка без фальстарта
        digitalWrite(A1, HIGH);
        tone(10,800, 800);
        lcd.setCursor(0,1);
        lcd.print(char(6));
        lcd.print(" ");
        lcd.print(60 - time_remaind);
        while(true){
          if (digitalRead(A3) == LOW){
              system_reset();
              break;
            }
          if (digitalRead(A2) == LOW){
            float time_remaind_backup = time_remaind;
            system_reset();
            time_remaind =  time_remaind_backup;
            lcd.setCursor(7,0);
            lcd.print(time_remaind);
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
          lcd.setCursor(0,1);
          lcd.print(char(7));
          lcd.print(" ");
          lcd.print(char(2));
          lcd.print("A");
          lcd.print(char(3));
          lcd.print(char(1));
          lcd.print("CTAPT!");
          while(true){
            if (digitalRead(A3) == LOW){
              system_reset();
              break;
            }
      }
      }else{                        // зеленая кнопка без фальстарта
        digitalWrite(A1, HIGH);
        tone(10, 800, 800);
        lcd.setCursor(0,1);
        lcd.print(char(7));
        lcd.print(" ");
        lcd.print(60 - time_remaind);
        while(true){
          if (digitalRead(A3) == LOW){
              system_reset();
              break;
            }
          if (digitalRead(A2) == LOW){
            float time_remaind_backup = time_remaind;
            system_reset();
            time_remaind =  time_remaind_backup;
            lcd.setCursor(7,0);
            lcd.print(time_remaind);
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
          lcd.setCursor(0,1);          
          lcd.print("C ");
          lcd.print(char(2));
          lcd.print("A");
          lcd.print(char(3));
          lcd.print(char(1));
          lcd.print("CTAPT!");
          while(true){
            if (digitalRead(A3) == LOW){
              system_reset();
              break;
            }
      }
      }else{                              // синяя кнопка без фальстарта
        digitalWrite(A1, HIGH);
        tone(10,800,800);
        lcd.setCursor(0,1);
        lcd.print("C ");
        lcd.print(60 - time_remaind);
        while(true){
          if (digitalRead(A3) == LOW){
              system_reset();
              break;
            }
          if (digitalRead(A2) == LOW){
            float time_remaind_backup = time_remaind;
            system_reset();
            time_remaind =  time_remaind_backup;
            lcd.setCursor(7,0);
            lcd.print(time_remaind);
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
      lcd.setCursor(2,1);
      lcd.print(char(0xFF));
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
      lcd.setCursor(6,1);
      lcd.print(char(0xFF));
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
      lcd.setCursor(10,1);
      lcd.print(char(0xFF));
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
      lcd.setCursor(14,1);
      lcd.print(char(0xFF));
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
      lcd.setCursor(2,1);
      lcd.print(buttons_order);      
      buttons_order += 1;
    }

    if (digitalRead(3) == LOW && yellow_pressable){
     
      yellow_pressable = false;
      
      digitalWrite(7, HIGH);
      tone(10, 800, 800);
      lcd.setCursor(6,1);
      lcd.print(buttons_order);      
      buttons_order += 1;      
    }

    if (digitalRead(4) == LOW && green_pressable){
      
      green_pressable = false;      
      digitalWrite(8, HIGH);
      tone(10, 800, 800);
      lcd.setCursor(10,1);
      lcd.print(buttons_order);      
      buttons_order += 1;     
    }

    if (digitalRead(5) == LOW && blue_pressable){
     
      blue_pressable = false;
      digitalWrite(9, HIGH);
      tone(10, 800, 800);
      lcd.setCursor(14,1);
      lcd.print(buttons_order);      
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
      lcd.setCursor(11, 0);
      lcd.print(time_remaind);
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
          lcd.setCursor(0,1);
          lcd.print("K ");
          lcd.print(char(2));
          lcd.print("A");
          lcd.print(char(3));
          lcd.print(char(1));
          lcd.print("CTAPT!");
          delay(5000);
          system_reset();
      }else{                            // красная кнопка без фальстарта
        digitalWrite(A1, HIGH);
        tone(10, 800, 800);
        lcd.setCursor(0,1);
        lcd.print("K ");
        lcd.print(60-time_remaind);
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
          lcd.setCursor(0,1);
          lcd.print(char(6));
          lcd.print(" ");
          lcd.print(char(2));
          lcd.print("A");
          lcd.print(char(3));
          lcd.print(char(1));
          lcd.print("CTAPT!");
          delay(5000);
          system_reset();
      }else{                          // желтая кнопка без фальстарта
        digitalWrite(A1, HIGH);
        tone(10,800, 800);
        lcd.setCursor(0,1);
        lcd.print(char(6));
        lcd.print(" ");
        lcd.print(60 - time_remaind);
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
          lcd.setCursor(0,1);
          lcd.print(char(7));
          lcd.print(" ");
          lcd.print(char(2));
          lcd.print("A");
          lcd.print(char(3));
          lcd.print(char(1));
          lcd.print("CTAPT!");
          delay(5000);
          system_reset();
      }else{                        // зеленая кнопка без фальстарта
        digitalWrite(A1, HIGH);
        tone(10, 800, 800);
        lcd.setCursor(0,1);
        lcd.print(char(7));
        lcd.print(" ");
        lcd.print(60 - time_remaind);
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
          lcd.setCursor(0,1);          
          lcd.print("C ");
          lcd.print(char(2));
          lcd.print("A");
          lcd.print(char(3));
          lcd.print(char(1));
          lcd.print("CTAPT!");
          delay(5000);
          system_reset();
      }else{                              // синяя кнопка без фальстарта
        digitalWrite(A1, HIGH);
        tone(10,800,800);
        lcd.setCursor(0,1);
        lcd.print("C ");
        lcd.print(60 - time_remaind);
        delay(5000);
        system_reset();
        }      
      }    
    }
  }
  
  


