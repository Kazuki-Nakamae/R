/*//////////////////簡易カウントアップ//////////////////////////*/
/* Name: main.c
 * Author: Kazuki Nakamae
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

/******************************ライブラリ**********************************/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/******************************関数の宣言**********************************/

void lcd_init(void);
void lcd_out(char code,char rs);
void lcd_cmd(char cmd);
void lcd_data(char asci);
void lcd_pos(char line,char col);
void lcd_clear(void);
void lcd_str(char *str);
int lcd_count(int start);

/******************************マクロの定義**********************************/

#define LCD_RS 0b00000001
#define LCD_E 0b00000010
#define LCD_PORT PORTD

/****************************グローバル変数**********************************/

int sec[]={0x30,0x30},  /*秒*/
    min[]={0x30,0x30},  /*分*/
    before=0x30;    /*フラグ*/

/*******************************関数の定義**********************************/


/*タクトスイッチを押すとカウントが初期化(割り込み)*/
ISR( TIMER0_COMPA_vect ){
    if ((PINC&0b00000001)==0b00000001) {
        int i=0;
        for (i=0; i<2; i++) {   /*時間の初期化*/
            sec[i]=0x30;
            min[i]=0x30;
        }
    before=min[0];
    }
}

/*文字列表示*/
void lcd_str(char *str){
    while(*str!='\0'){
        lcd_data(*str);
        str++;
    }
}

/*カウントアップ*/
int lcd_count(int start){
    if (start==0x39) {/*繰り上げ*/
        start=0x30;
    }
    else{
        start++;
    }
    return start;
}

/*表示全消去*/
void lcd_clear(void){
    lcd_cmd(0x01);
}

/*表示位置指定*/
void lcd_pos(char line,char col){
    if(line==1)
        lcd_cmd(0x80+col-1);
    else if(line==2)
        lcd_cmd(0xC0+col-1);
}

/*LCD送信処理*/
void lcd_out(char code,char rs){
    LCD_PORT=(code & 0xF0)|(LCD_PORT&0x0F);
    if(rs==0)/*コマンド入力モード*/
        LCD_PORT=LCD_PORT&~LCD_RS;
    else/*文字入力モード*/
        LCD_PORT=LCD_PORT|LCD_RS;
    
    _delay_ms(1);
    LCD_PORT=LCD_PORT|LCD_E;
    _delay_ms(1);
    LCD_PORT=LCD_PORT&~LCD_E;
    
}


/*4ビットモードでのコマンド送信処理*/
void lcd_cmd(char cmd){
    lcd_out(cmd,0);/*上位4ビット送信*/
    lcd_out(cmd<<4,0);/*下位4ビット送信*/
    _delay_ms(2);
}

/*4ビットモードでのデータ送信処理*/
void lcd_data(char asci){
    lcd_out(asci,1);/*上位4ビット送信*/
    lcd_out(asci<<4,1);/*下位4ビット送信*/
    _delay_ms(0.05);
}

/*LCD初期化処理*/
void lcd_init(void){
    _delay_ms(15);
    lcd_out(0x30,0);/* 8ビットモード指定 */
    _delay_ms(5);
    lcd_out(0x30,0);/* 8ビットモード指定 */
    _delay_ms(1);
    lcd_out(0x30,0);/* 8ビットモード指定 */
    _delay_ms(1);
    lcd_out(0x20,0);/* 4ビットモード指定 */
    _delay_ms(1);
    
    lcd_cmd(0x28);
    lcd_cmd(0x08);
    lcd_cmd(0x0c);
    lcd_cmd(0x06);
    lcd_cmd(0x02);
    lcd_cmd(0x01);
}

/******************************メインストリーム**********************************/

int main(void)
{
    /***初期設定****/
    
    PORTD=0b00000000;
    DDRD=0b11111111;
    DDRC=0b11111110;    //PINCはスイッチ用に使用
    PORTC=0b00000000;   //出力設定　※これと入力をあわせたものはPINC
    
    /*LCD初期化*/
    lcd_init();
    
    int i;  /*LCD表示処理に使う*/
    
    /*割り込み用レジスタ設定*/
    TCCR0A  =0b00000010;    /*CTC*/
    TCCR0B  =0b00000101;    /*約1MHz*/
    OCR0A   =4;             /*5msで割り込み*/
    TIMSK0  =0b00000010;    /*COMPA割り込み*/
    
    sei();  /*割り込み許可*/

    /***カウントアップループ****/
    while(1){
        _delay_ms(907.5);/*処理時間を差し引いた一秒カウント　※実測による補正値*/
        sec[0]=lcd_count(sec[0]);               /*秒の一の位*/
        if (sec[0]==0x30) {                     /*秒の十の位*/
            sec[1]=lcd_count(sec[1]);
        }
        else{                                   /*処理がない時の時間稼ぎ*/
            _delay_ms(0.05);
        }
        if (sec[1]==0x36) {                     /*分の一の位*/
            min[0]=lcd_count(min[0]);
            sec[1]=0x30;
        }
        else{                                   /*処理がない時の時間稼ぎ*/
            _delay_ms(0.05);
        }
        if ((min[0]==0x30)&&(min[0]!=before)) { /*分の十の位*/
            min[1]=lcd_count(min[1]);
        }
        else{                                   /*処理がない時の時間稼ぎ*/
            _delay_ms(0.05);
        }
        before=min[0];
        /*表示の初期化*/
        lcd_clear();
        /*時間の表示*/
        for (i=0; i<2; i++) {
            lcd_pos(1,11-i);
            lcd_data(sec[i]);
            lcd_pos(1,2-i);
            lcd_data(min[i]);
        }
        /*文字の表示*/
        lcd_pos(1,13);
        lcd_str("sec");
        lcd_pos(1,4);
        lcd_str("min");
    }
    return 0;   /* never reached */
}
/*********************************終了***********************************/
