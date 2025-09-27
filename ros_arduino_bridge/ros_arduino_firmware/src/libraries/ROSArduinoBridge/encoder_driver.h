/* *************************************************************
   Encoder driver function definitions - by James Nugen
   ************************************************************ */
   
   
#ifdef ARDUINO_ENC_COUNTER
  //below can be changed, but should be PORTD pins; 
  //otherwise additional changes in the code are required
  #define LEFT_ENC_PIN_A PD2  //pin 2
  #define LEFT_ENC_PIN_B PD3  //pin 3
  
  //below can be changed, but should be PORTC pins
  #define RIGHT_ENC_PIN_A PC4  //pin A4
  #define RIGHT_ENC_PIN_B PC5   //pin A5

#elif defined ARDUINO_MY_COUNTER
  //定义引脚
  #define LEFT_A 19    //4
  #define LEFT_B 18   //5
  #define RIGHT_A 21 //2
  #define RIGHT_B 20 //3
  //声明函数
  //1.初始化函数，设置引脚操作模式
  void initEncoder();
  //中断函数
  void leftEncoderEventA();
  void leftEncoderEventB();
  void rightEncoderEventA();
  void rightEncoderEventA();

#endif
   
long readEncoder(int i);
void resetEncoder(int i);
void resetEncoders();
