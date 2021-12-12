#include "mbed.h"
#include "uLCD_4DGL.h"

Serial pc(USBTX, USBRX);
Serial dev(p28,p27);
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);
DigitalOut sol1(p7);
DigitalOut sol2(p8);
uLCD_4DGL uLCD(p9, p10, p11);
DigitalOut trigger(p5);
DigitalIn echo(p6);
Timer sonar;

int main() {
    dev.baud(9600);
    int distance = 0;
    int correction = 0;
    
    uLCD.filled_circle(120, 6, 4, 0x333333);
    uLCD.text_width(3);
    uLCD.text_height(3);
    uLCD.text_mode(TRANSPARENT);
    uLCD.color(0xF40009);
    uLCD.locate(1,1);
    uLCD.printf("Coke");
    uLCD.locate(0,3);
    uLCD.color(0x008B47);
    uLCD.printf("Sprite");

    while(1) {
        while(dev.readable()) {
            int letter = dev.getc();
            if ((char) letter == 'c') {
                if ((char) dev.getc() == 'o') {
                    if ((char) dev.getc() == 'k') {
                        if ((char) dev.getc() == 'e') {
                            if ((char) dev.getc() == '\n'){
                                uLCD.filled_circle(10, 65, 3, 0x000000);
                                uLCD.filled_circle(10, 18, 3, 0xF40009);
                                sonar.reset();
                                sonar.start();
                                while (echo==2) {};
                                sonar.stop();
                                correction = sonar.read_us();
//                                printf("Approximate software overhead timer delay is %d uS\n\r",correction);
                                int i = 0;
                                while(1) {
                                    trigger = 1;
                                    sonar.reset();
                                    wait_us(10.0);
                                    trigger = 0;
                                    while (echo==0) {};
                                    sonar.start();
                                    while (echo==1) {};
                                    sonar.stop();
                                    distance = (sonar.read_us()-correction)/58;
                                    pc.printf(" %d cm \n\r",distance);
                                    if (distance < 25 && distance > 10) {
                                        uLCD.filled_circle(120, 6, 4, 0x00FF00);
                                        i++;
                                        if (i > 20){
                                            break;
                                        }
                                    } else {
                                        i = 0;
                                        uLCD.filled_circle(120, 6, 4, 0x333333);
                                    }
                                    wait(0.2);
                                }
                                uLCD.filled_rectangle(0, 13, 127, 55, 0xF40009);
                                uLCD.locate(1,1);
                                uLCD.color(0xFFFFFF);
                                uLCD.text_mode(TRANSPARENT);
                                uLCD.printf("Coke");
                                sol1 = 1;
                                led1 = 1;
                                wait(0.25);
                                sol1 = 0;
                                led1 = 0;
                                uLCD.locate(1,1);
                                uLCD.filled_rectangle(0, 13, 127, 55, 0x000000);
                                uLCD.color(0x880000);
                                uLCD.printf("Coke");
                                uLCD.filled_circle(120, 6, 4, 0x333333);
                            }
                        }
                    }
                }
            }
            if ((char) letter == 's') {
                if ((char) dev.getc() == 'p') {
                    if ((char) dev.getc() == 'r') {
                        if ((char) dev.getc() == 'i') {
                            if ((char) dev.getc() == 't') {
                                if ((char) dev.getc() == 'e') {
                                    if ((char) dev.getc() == '\n'){
                                        uLCD.filled_circle(10, 18, 3, 0x000000);
                                        uLCD.filled_circle(10, 65, 3, 0x008B47);
                                        sonar.reset();
                                        sonar.start();
                                        while (echo==2) {};
                                        sonar.stop();
                                        correction = sonar.read_us();
                                        int i = 0;
//                                        printf("Approximate software overhead timer delay is %d uS\n\r",correction);
                                        while(1) {
                                            trigger = 1;
                                            sonar.reset();
                                            wait_us(10.0);
                                            trigger = 0;
                                            while (echo==0) {};
                                            sonar.start();
                                            while (echo==1) {};
                                            sonar.stop();
                                            distance = (sonar.read_us()-correction)/58;
                                            pc.printf(" %d cm \n\r",distance);
                                            if (distance < 25 && distance > 10) {
                                                i++;
                                                uLCD.filled_circle(120, 6, 4, 0x00FF00);
                                                if (i > 20){
                                                    break;
                                                }
                                            } else {
                                                i = 0;
                                                uLCD.filled_circle(120, 6, 4, 0x333333);
                                            }
                                            wait(0.2);
                                        }
                                        uLCD.filled_rectangle(0, 60, 127, 105, 0x008B47);
                                        uLCD.locate(0,3);
                                        uLCD.color(0xFFFFFF);
                                        uLCD.text_mode(TRANSPARENT);
                                        uLCD.printf("Sprite");
                                        sol2 = 1;
                                        led2 = 1;
                                        wait(0.25);
                                        sol2 = 0;
                                        led2 = 0;
                                        uLCD.locate(0,3);
                                        uLCD.filled_rectangle(0, 55, 127, 105, 0x000000);
                                        uLCD.color(0x005514);
                                        uLCD.printf("Sprite");
                                        uLCD.filled_circle(120, 6, 4, 0x333333);
                                    }
                                }
                            }
                        }
                    }
                }
            }    
        }
    }
}
