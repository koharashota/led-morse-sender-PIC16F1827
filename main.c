/*
 * File:   mian.c
 * Author: shota
 *
 * Created on December 4, 2015, 11:59 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h> // include standard header file

// コンフィギュレーション１の設定
#pragma config FOSC = INTOSC    // 内部ｸﾛｯｸを使用する(INTOSC)
#pragma config WDTE = OFF       // ｳｵｯﾁﾄﾞｯｸﾞﾀｲﾏｰ無し(OFF)
#pragma config PWRTE = ON       // 電源ONから64ms後にﾌﾟﾛｸﾞﾗﾑを開始する(ON)
#pragma config MCLRE = ON      // 外部ﾘｾｯﾄ信号は使用せずにﾃﾞｼﾞﾀﾙ入力(RA5)ﾋﾟﾝとする(OFF)
#pragma config CP = OFF         // ﾌﾟﾛｸﾞﾗﾑﾒﾓﾘｰを保護しない(OFF)
#pragma config CPD = OFF        // ﾃﾞｰﾀﾒﾓﾘｰを保護しない(OFF)
#pragma config BOREN = ON       // 電源電圧降下常時監視機能ON(ON)
#pragma config CLKOUTEN = OFF   // CLKOUTﾋﾟﾝをRA6ﾋﾟﾝで使用する(OFF)
#pragma config IESO = OFF       // 外部・内部ｸﾛｯｸの切替えでの起動はなし(OFF)
#pragma config FCMEN = OFF      // 外部ｸﾛｯｸ監視しない(FCMEN_OFF)

// コンフィギュレーション２の設定
#pragma config WRT = OFF        // Flashﾒﾓﾘｰを保護しない(OFF)
#pragma config PLLEN = OFF      // 動作クロックを32MHzでは動作させない(OFF)
#pragma config STVREN = ON      // スタックがオーバフローやアンダーフローしたらリセットをする(ON)
#pragma config BORV = HI        // 電源電圧降下常時監視電圧(2.5V)設定(HI)
#pragma config LVP = OFF        // 低電圧プログラミング機能使用しない(OFF)

#define _XTAL_FREQ 16000000
#define led RB5	//赤外線led
#define testled RB1	//テスト用led
#define sw RB0	//スイッチ

static void pic_init(); //pic初期化
void outl(void); //赤外線Low出力
void outh(void); //赤外線Hi出力
void startbit(void); //スタートビット
void stopbit(void); //ストップビット

void send_shota(void); //shotaという信号を送る

 void main(void)
 {
   pic_init();
   int b=1;	//送信監視用
   int c=3;	//3回繰り返し信号を送信する
   while(1){
     if (sw == 0) {
       testled=0;
       led=0;
     } else {
       if(c > b){
         testled=1;
         // 押しっぱなしかどうかを判定
         if(sw != 0){
           b = 0;
         }
         //スタートビット
         startbit();
         // データを送信
         send_shota();
         // ストップビット
         stopbit();
         // 40ms休息
         __delay_ms(40);
         b++;
       }
     }
   }
 }

static void pic_init() {
  TRISB = 0x01;
  ANSELB = 0x00;	// set all RB to digital pin
  OSCCON = 0x7A;	//内部クロック16 MHz 0111 1010b
  PORTB = 0x00;
}

void startbit(){
	outh();
}

void stopbit(){
	outh();outl();outh();outl();outh();
}

//赤外線LED消灯
void outl(){
	led=0;
  __delay_us(598);
}

//赤外線LED点滅 38kHz
void outh(){
  for (int i = 0; i < 23; i++) {
    led=1;
    __delay_us(13);
    led=0;
    __delay_us(13);
  }
}

void send_shota(){
    // 1110011
    outh();outh();outh();outl();outl();outh();outh();
    //outl();outh();outl();outh();outl();outl();outh();
}
