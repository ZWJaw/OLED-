#include <Arduino.h>
#include <U8g2lib.h>

#define OLED_SCK  16
#define OLED_SDA 17
#define UPBUTTON  18
#define DOWNBUTTON 19
#define ENTER 22
#define BACK 23

//声明函数
void Menu_Display(int index);

// 定义OLED对象，使用U8g2_SSD1306_128X64_NONAME_F_SW_I2C构造函数
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, OLED_SCK, OLED_SDA,U8X8_PIN_NONE);

//定义菜单结构体
struct MenuSample {
  const char *name; /*创建名为name指向char的指针，即name为一个char型地址，如果要访问name的数据用*name,比如*name = 'a'。
  如果char *name = "菜单",是把"菜单"的地址赋给name,不是在name上存字符串，也就是说 = 后面是地址*/
  struct MenuSample *subMenu;
};

//定义一级子菜单1结构体数组
MenuSample sub1Menu[] = {
  {"子菜单1-1",nullptr},
  {"子菜单1-2",nullptr},
  {nullptr,nullptr}//指针结束标志
};

//定义一级子菜单2结构体数组
MenuSample sub2Menu[] = {
  {"子菜单2-1",nullptr},
  {"子菜单2-2",nullptr},
  {nullptr,nullptr}//指针结束标志
};

//定义一级子菜单3结构体数组
MenuSample sub3Menu[] = {
  {"子菜单3-1",nullptr},
  {"子菜单3-2",nullptr},
  {nullptr,nullptr}//指针结束标志
};

//定义主菜单结构体型数组
MenuSample MainMenu[] = {
  {"主菜单1",sub1Menu},
  {"主菜单2",sub2Menu},
  {"主菜单3",sub3Menu},
  {nullptr,nullptr}//指针结束标志
};

//定义当前菜单
struct MenuSample *CurrentMenu = MainMenu;

//定义菜单栈以及索引，里面存菜单的地址,用来返回上一级
struct MenuSample *MenuStack[2];
int StackIndex ;

//定义索引
int Index = 0;

void setup(){
  Serial.begin(9600);
  //初始化对象
  u8g2.begin();
  //开启中文字符集
  u8g2.enableUTF8Print();
  //设置字体库
  u8g2.setFont(u8g2_font_wqy12_t_gb2312);
  //配置4个按键
  pinMode(UPBUTTON,INPUT_PULLUP);
  pinMode(DOWNBUTTON,INPUT_PULLUP);
  pinMode(ENTER,INPUT_PULLUP);
  pinMode(BACK,INPUT_PULLUP);
}

void loop(){

  Menu_Display(Index);
}

void Menu_Display(int index1){
  u8g2.firstPage();

  int Max_index = 0;

  do{
    //显示当前菜单并获取最大索引
    for(int i=0; CurrentMenu[i].name != nullptr; i++,Max_index++){
      if(i == index1){
        u8g2.setCursor(0,10+15*i);
        u8g2.print("-> ");
        u8g2.print(CurrentMenu[i].name);//传地址
      }
      else {
        u8g2.setCursor(0,10+15*i);
        u8g2.print(CurrentMenu[i].name);//传地址
      }
    }

    Serial.println(Max_index);

     //进入按键
    if(!digitalRead(ENTER) && CurrentMenu[index1].subMenu != nullptr){
      delay(20);
      while(!digitalRead(ENTER)) ;
      delay(20);
      //根据当前索引进入子菜单,其实就是把CurrentMenu存放的地址换成对应子菜单的地址,同时把地址记录到菜单栈里
      MenuStack[StackIndex++] = CurrentMenu;
      CurrentMenu = CurrentMenu[index1].subMenu;
      Index = 0;
    }

    //返回按键
    if(!digitalRead(BACK) && StackIndex > 0){
      delay(20);
      while(!digitalRead(BACK)) ;
      delay(20);
      //从菜单栈中取出上一级菜单的地址
      CurrentMenu = MenuStack[--StackIndex];
      Index = 0;
    }
    //向上移动
     if(!digitalRead(UPBUTTON)){
      delay(20);
      while(!digitalRead(UPBUTTON)) ;
      delay(20);
      Index++;
    }
    //向下移动
    if(!digitalRead(DOWNBUTTON)){
      delay(20);
      while(!digitalRead(DOWNBUTTON)) ;
      delay(20);
      Index--;
    }

    //限制索引范围
    if(Index > Max_index - 1) Index = 0;
    if(Index < 0) Index = Max_index - 1;

    Serial.println(Index);

  }while(u8g2.nextPage());
}