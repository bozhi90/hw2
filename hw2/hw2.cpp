#include "mbed.h"
#include "uLCD_4DGL.h"			// uLCD函式庫 

uLCD_4DGL uLCD(D1, D0, D2);		// uLCD pin腳
DigitalIn Up_buttom(D10);		// 按鈕上
DigitalIn Down_buttom(D9);		// 按鈕下
DigitalIn Enter_buttom(D8);		// 確認按鈕
AnalogOut aout(PA_4);			// 波型輸出
AnalogIn ain(A0);			    // 輸入波
Thread scan_print;			    // thread 接收輸入波

void uLCD_initial(void) {		// uLCD初始化
    uLCD.reset();
    uLCD.background_color(BLACK);
    uLCD.textbackground_color(BLACK);
    uLCD.cls();
    uLCD.color(WHITE);
    uLCD.locate(1,2);
    uLCD.printf("freq :    Hz");
}

void sp (void) {			// 接收輸入，並輸出給電腦
    float Ain[500];			// 儲存500筆資料
    int sample = 500, i;	// 取樣頻率500Hz

    for (i = 0; i < 500; i++) {
        ThisThread::sleep_for(1000ms / sample);		// 延遲取樣週期
        Ain[i] = ain;					            // 紀錄
    }
    for (i = 0; i < 500; i++) {
        printf("%f\r\n", Ain[i]);			        // 輸出儲存的資料
        ThisThread::sleep_for(1ms);			        // 一小段延遲
    }
}

int main()
{
    int freq = 50;					        // 產生波的頻率

    uLCD_initial();					        // 初始化uLCD
    while (Enter_buttom != 1) {				// 按下確認之前一直判斷
        if (Up_buttom == 1) freq = 100;		// 按下上鍵就將頻率設成100
        if (Down_buttom == 1) freq = 50;	// 按下下鍵就將頻率設成50
        uLCD.locate(7,2);
    	uLCD.printf("%d ", freq);			// 顯示
    }
    scan_print.start(sp);				    // 開始接收輸入，並輸出
    while (1) {						        // 重複執行
        for (float j = 1.0f; j > 0.0f; j -= 0.1f) {
            ThisThread::sleep_for(100ms / freq);
	        aout = j;
        }
    }
    return 0;
}