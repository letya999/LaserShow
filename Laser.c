void sendto (float x_target, float y_target);
void do_pwm();
void hold(long length);
void laser_on();
void laser_off();
void draw_letter(byte letter);
 
float x_level;
float y_level;
float move_scale = 2.4;
int laser_pin = 12;
int x_speaker_pin = 5;
int y_speaker_pin = 6;
 
float exp_move = 2.7; 
float min_move = 0.5; 
float max_move = 30;                   
float laser_toggle_delay = 11;
float line_delay = 8;                      
float x_min_value = 5; 
float y_min_value = 5; 
float x_max_value = 190;
float y_max_value = 190; 
                                              
void setup()   {               
  pinMode(laser_pin, OUTPUT);
  pinMode(x_speaker_pin, OUTPUT);
  pinMode(y_speaker_pin, OUTPUT);
  setPwmFrequency(x_speaker_pin, 1);
  setPwmFrequency(y_speaker_pin, 1);
}
 
// sendto (X, Y) - отправляет лазер на координаты X и Y - масштаб всегда от 0 до 255 (хотя фактическое разрешение может быть ниже, чем это в зависимости от указанных выше значений конфигурации)
  // перемещение (X, Y) - перемещает лазер к координатам X и Y относительно текущей позиции
  // move_scale = X - устанавливает масштабирование - используется только функциями "move" и "draw_letter"
                     // масштабирование 2.8 допускает 5 символов
  // draw_letter ("X") - рисует одну букву в одной позиции (только в верхнем регистре - см. подпрограмму для других поддерживаемых символов)
  // draw_string ("STRING", X, Y, COUNT) - рисует строку в X, Y для времени COUNT
  // marquee ("HELLO WORLD", CHARACTERS) - рисует прокручиваемый шрифт long CHARACTERS long - текст автоматически масштабируется, чтобы соответствовать (длина от 4 до 7 символов работает лучше всего)
  // laser_off () - выключает лазер
  // laser_on () - включает лазер
  // hold (time) - указывает лазеру удерживать текущую позицию в течение примерно 20 микросекунд (полезно, если вам нужно заставить лазер «догнать» после перемещения на большое расстояние)
 
// поместите код управления лазером сюда
 
void loop() {
    demo();
}
 
void demo() { 
    marquee("THIS ARDUINO LASER SHOW USES AUDIO SPEAKERS FOR GALVOS!", 5);
    for (float loop = 0; loop < 25; loop ++) {
       move_scale = loop / 5; 
       laser_off();
       sendto (1,1);
       move (17,5);
       laser_on();
       move (13,24);
       move (-30,-18);
       move (34,0);
       move (-30,18);
       move (13,-24);
    }
    for (float loop = 25; loop > 0; loop --) {
       move_scale = loop / 5; 
       laser_off();
       sendto (1,1);
       move (17,5);
       laser_on();
       move (13,24);
       move (-30,-18);
       move (34,0);
       move (-30,18);
       move (13,-24);
    }

   move_scale = 1.8;  
   float exp_move_holder;
   exp_move_holder = exp_move;
   for (exp_move = 8; exp_move >= 1; exp_move = exp_move - exp_move / 20)
     draw_string("SPEED", 1, 1, 1);
   for (exp_move = 1; exp_move <= 8; exp_move = exp_move + exp_move / 20)
     draw_string("TEST", 1, 1, 1);
   exp_move = exp_move_holder;
   move_scale = 2.3;
   for (int x = 0; x < 10; x++) {
     draw_string("prrr", 1, 1, 2);
     draw_string("qsss", 1, 1, 2);
   }
   draw_string("prrg", 1, 1, 2);
   draw_string("qsg", 1, 1, 3);
   draw_string("pg", 1, 1 , 3);
   draw_string("q", 1, 1, 1);
   draw_string("g", move_scale * 16 / 1.3, 1, 1);
   draw_string("q", 1, 1, 1);
   draw_string("g", move_scale * 16 / 1.4, 1, 1);
   draw_string("ph", 1, 1, 3);
   for (int x = 0; x < 6; x++) {
     draw_string("p", 1, 1, 5);
     draw_string("q", 1, 1, 5);
   }
   marquee( "instruction" , 5);
}
 
 
void move (float x_movedir, float y_movedir) {
  x_movedir = x_movedir * move_scale;
  y_movedir = y_movedir * move_scale;
  sendto (x_level + x_movedir, y_level + y_movedir);
}
 
void sendto (float x_target, float y_target) {
    float x_move;
    float y_move;     
    float temp_exp_move = exp_move;
    while (fabs(x_target - x_level) > min_move || fabs(y_target - y_level) > min_move) {
        x_move = (x_target - x_level) / temp_exp_move;
        y_move = (y_target - y_level) / temp_exp_move;          
        while (fabs(x_move) > max_move || fabs(y_move) > max_move) {
            x_move = x_move *.5;
            y_move = y_move *.5;
            temp_exp_move = temp_exp_move * 1.5;
        }
        x_level = x_level + x_move;
        y_level = y_level + y_move;
        do_pwm();
    }
    x_level = x_target;
    y_level = y_target;
    do_pwm();
    hold(line_delay);
}
 
void laser_on() {
  hold(laser_toggle_delay);
  digitalWrite(laser_pin, HIGH);
}
 
void laser_off() {
  hold(laser_toggle_delay);
  digitalWrite(laser_pin, LOW);
}
 
void do_pwm() {
    float x_adjust;
    float y_adjust;
    x_adjust = x_min_value + (x_level * ((x_max_value - x_min_value) / 255)); 
    y_adjust = y_min_value + ((y_level) * ((y_max_value - y_min_value) / 255));
    if (x_adjust < 1) x_adjust = 1;
    if (y_adjust < 1) y_adjust = 1;
    if (x_adjust > x_max_value) x_adjust = x_max_value;
    if (y_adjust > y_max_value) y_adjust = y_max_value;
    analogWrite(y_speaker_pin, y_adjust);
    analogWrite(x_speaker_pin, x_adjust);
}
 
void hold(long length) {
    long loop2 = 0;
	length = length * 2;  
    while (loop2 < length) {
        delay (1); //we'v messed with timers - so this is a bunch smaller than 1ms delay
        loop2 ++;
    }
}
 
void draw_string(String text, int x, int y, int count) {
  for (int loop = 0; loop < count; loop ++) {
    sendto (x,y);
    int i = 0;
    while (text.charAt(i) != '\0') {
       draw_letter(text.charAt(i)); 
       i ++;
    }
  }
}
  
void marquee(String text, int characters) {//функция "бегущей строки", прокручивает текст text
    floatmove_scale_holder = move_scale; //текущий изменяемый масштаб, присвоили ему глобальный  
    move_scale = 256 / (characters * 18); //поменяли глобальный в зависимости от количества символов
    for (int x=0; x < characters + 2; x++) //в цикле увеличиваем количество отступов (пробелов " ") слева и справа от текста
        text = String(" " + text + " ");     
    for (int loop = 0; loop < (text.length() - 5); loop ++) { //в цикле пока итерационная переменная меньше длины текста - 5
      draw_string(text.substring(loop,loop + characters),move_scale * 9,1,1);  //отрисовываем подстростроку в горизонтальной плоскости
      draw_string(text.substring(loop,loop + characters),1,1,1);  //отрисовываем ту же но сдвинутую на положение 1, тем самым создавая эффект бегущей строки
    }
    move_scale = move_scale_holder;
}
 
void draw_letter(byte letter) {
  float temp_x_level = x_level;
  float temp_y_level = y_level;
  laser_off();
  switch (letter) {
    case 'A':        
        move (0,16);
        laser_on();
        move (7,-16);
        move (7,16);
        laser_off();
        move (-10,-7);
        laser_on();
        move (9,0);
        break;

    case 'B': 
        laser_on();
        move (0,16);
        move (11,-3);
        move (-7,-5);
        move (7,-5);
        move (-11,-3);
        break;
 
    case 'C': 
        move (11,16);
        laser_on();
        move (-11,-2);
        move (0,-12);
        move (11,-2);
        break;
 
    case 'D': 
        laser_on();
        move (0,16);
        move (9,-4);
        move (0,-8);
        move (-9,-4);
        break;
 
    case 'E': 
        move (10,0);
        laser_on();  
        move (-10,0);
        move (0,14);
        move (10,2);
        laser_off();
        move (-6,-10);
        laser_on();
        move (-6,0);
        break;
 
    case 'F':         
        move (10,0);
        laser_on();  
        move (-10,0);
        move (0,16);
        laser_off();
        move (0,-8);
        laser_on();
        move (5,0);
        break;
 
    case 'G': 
        move (9,8);
        laser_on();
        move (4,0);
        move (-1,8);
        move (-12,-2);
        move (0,-12);
        move (9,-2);
       break;
 
    case 'H': 
        move (2,0);
        laser_on();
        move (0,16);
        laser_off();
        move (0,-8);
        laser_on();
        move (9,0);
        laser_off();
        move (0,8);
        laser_on();
        move (0,-16);
        break;
 
    case 'I':    
        move (5,0);
        laser_on();
        move (0,16);
        break;
 
    case 'J':    
        move (0,8);
        laser_on();
        move (2,8);
        move (9,0);
        move (0,-16);
        break;
 
    case 'K':    
        laser_on();
        move (0,16);
        laser_off();
        move (11,0);
        laser_on();
        move (-9,-8);
        move (9,-8);
        break;
 
    case 'L':    
        laser_on();
        move (0,16);
        move (11,0);
        break;
 
    case 'M':    
        move (0,16);
        laser_on();
        move (0,-16);
        move (7,8);
        move (7,-8);
        move (0,16);
        break;
 
   case 'N':    
        move (0,16);
        laser_on();
        move (0,-16);
        move (11,16);
        move (0,-16);
        break;
 
    case 'O':      
        laser_on();
        move (11,0);
        move (0,16);
        move (-11,0);
        move (0,-16);
        break;
 
 
    case 'P':      
        move (0,16);
        laser_on();
        move (0,-16);
        move (11,4);
        move (-11,4);
        break;
 
    case 'Q':      
 
        move (6,0);
        laser_on();
        move (-6,8);
        move (6,8);
        move (6,-8);
        move (-6,-8);
        laser_off();
        move (0,7);
        laser_on();
        move (7,7);
        
        break;
 
   case 'R':      
        move (0,16);
        laser_on();
        move (0,-16);
        move (11,3);
        move (-11,5);
        move (11,10);
        break;
 
  case 'S':      
        move (11,0);
        laser_on();
        move (-11,4);
        move (11,6);
        move (-11,6);
        break;
 
case 'T':      
       laser_on();
       move (13,0);
       laser_off();
       move (-8,0);
       laser_on();
       move (0,16);
       break;
 
  case 'U':      
        laser_on();
        move (2,16);
        move (9,0);
        move (2,-16);
        break;
 
  case 'V':      
        laser_on();
        move (5,16);
        move (5,-16);
        break;
 
  case 'W':      
        laser_on();
        move (3,16);
        move (4,-8);
        move (4,8);
        move (3,-16);
        break;
 
case 'X':      
        laser_on();
        move (12,16);
        laser_off();
        move (-12,0);
        laser_on();
        move (12,-16);
        break;
 
case 'Y':      
        laser_on();
        move (6,7);
        move (0,9);
        laser_off();
        move (0,-9);
        laser_on();
        move (6,-7);
        break;
 
case 'Z':      
        laser_on();
        move (12,0);
        move (-12,16);
        move (12,0);
        break;
 
    case ' ':    
        break;
 
    case '-':    
        move (2,8); 
        laser_on();
        move (10,0);
        break;
 
    case '\'':    
        move (8,0); 
        laser_on();
        move (0,4);
        break;
 
    case '!':    
        move (5,0);  //tweaked to make sure doesn't look like '?'
        laser_on();
        move (-1,3);
        move (1,6);
        laser_off();
        move (0,5);
        laser_on();
        move (0,2);
        break;
        
    case '?':
        laser_on();
        move (12,1);
        move (-6,6);
        move (0,2);
        laser_off();
        move (0,5);
        laser_on();
        move (0,2);
        break;
 
    case '.':    
        move (5,14);
        laser_on();
        move (0,2);
        move (0,-2);
        break;
 
    case '*':    
        move (5,8);
        laser_on();
        move (0,2);
        move (0,-2);
        break;
 
    case ':':
        move (5,2);
        laser_on();
        move (0,3);
        laser_off();
        move (0,5);
        laser_on();
        move (0,3);
        break;
 
    case '/':    
        move (0,16);
        laser_on();
        move (12,-16);
        break;
 
    case '0':      
        laser_on();
        move (11,0);
        move (0,16);
        move (-11,0);
        move (0,-16);
        break;
        
    case '1':    
        move (5,0);
        laser_on();
        move (0,16);
        break;
 
    case '2':    
        laser_on();
        move (12,2);
        move (-12,14);
        move (12,0);
        break;
 
    case '3':    
        laser_on();
        move (12,3);
        move (-7,6);
        move (7,6);
        move (-12,3);
        break;
 
    case '4':    
        laser_on();
        move (0,7);
        move (12,0);
        laser_off();
        move (0,-7);
        laser_on();
        move (0,16);
        break;
 
    case '5':    
        move (0,16);
        laser_on();
        move (12,-5);
        move (0,-5);
        move (-12,0);
        move (0,-6);
        move (12,0);
        break;
 
    case '6': 
        move (0,6);
        laser_on();
        move (12,3);
        move (0,6);
        move (-12,1);
        move (0,-16);
        move (10,0);
       break;
 
    case '7': 
        laser_on();
        move (12,0);
        move (-10,16);
       break;
 
    case '8': 
        move (0,2);
        laser_on();
        move (12,12);
        move (-6, 2);
        move (-6,-2);
        move (12,-12);
        move (-6, -2);
        move (-6, 2);
       break;
 
    case '9': 
        move (12,6);
        laser_on();
        move (-12,-3);
        move (0,-3);
        move (12,4);
        move (0,12);
       break;

    case 'p': 
        move (6,0);
        laser_on();
        move (6,2);
        move (-6,6);
        move (6,6);
        move (-6,2);
        move (-6,-8);
        move (6,-8);
       break;
 
    case 'q': 
        move (6,0);
        laser_on();
        move (6,7);
        move (-7,1);
        move (7,1);
        move (-6,7);
        move (-6,-8);
        move (6,-8);
       break;
   
    case 'r':    
        move (0,8);
        laser_on();
        move (0,2);
        move (0,-2);
        break;
 
    case 's':    
        move (9,8);
        laser_on();
        move (0,2);
        move (0,-2);
        break;
 
    case 'g': 
        move (8,0);
        laser_on();
        move (8,3);
        move (0,13);
        move (-4,-5);
        move (-4,5);
        move (-4,-5);
        move (-4,5);
        move (0,-13);
        move (8,-3);
       break;

    case 'h': 
        move (-7,8);
        laser_on();
        move (3,3);
        move (-6,-6);
        move (3,3);
        move (3,-3);
        move (-6,6);
       break;
  }

  laser_off();
  sendto (temp_x_level + (18 * move_scale) , temp_y_level);
}
 
void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x7; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}
