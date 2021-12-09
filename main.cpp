#include "mbed.h"
#include "Motor.h"
#include "rtos.h"
#include "XNucleo53L0A1.h"
#include <stdio.h>
#define VL53L0_I2C_SDA   p28
#define VL53L0_I2C_SCL   p27
#define action pair<int,float>
#include <vector>
#include <utility>
 
 
using namespace std;

vector<action> previousMoves;

 
 //lidar pins
Serial pc(USBTX,USBRX);
DigitalOut shdn(p26);
DigitalOut myled(LED1);
DigitalOut myled2(LED2);
DigitalOut myled3(LED3);
DigitalOut myled4(p9);
Ticker objectDetect;
Timer _time;

//motor speeds
double leftSpeed = 0;
double rightSpeed = 0;

//return home feature
bool returnHome = false;


static XNucleo53L0A1 *board=NULL;

//motor pins
Motor m_left(p22, p23, p24); // pwm, fwd, rev
Motor m_right(p25, p29, p11); // pwm, fwd, rev

//bluetooth pins
Serial blue(p13,p14);

//create threads
Thread wheelsThread;
Thread lidarThread;

int lidarDistance = 500; 
bool noMove = false;
bool running = false;

void lidar(void)
{
    int status;
    uint32_t distance;
    DevI2C *device_i2c = new DevI2C(VL53L0_I2C_SDA, VL53L0_I2C_SCL);
    /* creates the 53L0A1 expansion board singleton obj */
    board = XNucleo53L0A1::instance(device_i2c, A2, D8, D2);
    shdn = 0; //must reset sensor for an mbed reset to work
    wait(0.1);
    shdn = 1;
    wait(0.1);
    /* init the 53L0A1 board with default values */
    status = board->init_board();
    while (status) {
        myled = 1;
        pc.printf("Failed to init board! \r\n");
        status = board->init_board();
    }
    //loop taking and printing distance
    while (1) {
        myled2 = 1;
        wait(.1);
        myled2 = 0;
        status = board->sensor_centre->get_distance(&distance);
        lidarDistance = distance;
        if (lidarDistance < 10)
        {
            lidarDistance = 500;   
        }
        if (status == VL53L0X_ERROR_NONE) {
            //pc.printf("D=%ld mm\r\n", distance);
            pc.printf("D=%ld\r\n", lidarDistance);
        }
        Thread::wait(10);
    }
    
}

//wheels thread that uses bluetooth switch statements to control an RC car
void wheels(void)
{    
    char bnum=0;
    char bhit=0;
    while(1) {
        if (blue.getc()=='!') {
            if (blue.getc()=='B') { //button data packet
                bnum = blue.getc(); //button number
                bhit = blue.getc(); //1=hit, 0=release
                switch (bnum) {
                    case '1': //1 button tiggers go home 
                        returnHome = true;
                        while(previousMoves.size() > 0)
                        {
                            action currentAction = previousMoves.back();
                            printf("\r\naction: %d, duration: %f\r\n",previousMoves.back().first,previousMoves.back().second);
                            previousMoves.pop_back();
                            wait(1.5);
                            switch (currentAction.first) {
                                case 0: //go forward
                                    m_left.speed(1);
                                    m_right.speed(1);
                                    wait(currentAction.second);
                                    m_left.speed(0);
                                    m_right.speed(0);
                                    break;
                                case 1: //go back
                                    m_left.speed(-1);
                                    m_right.speed(-1);
                                    wait(currentAction.second);
                                    m_left.speed(0);
                                    m_right.speed(0);    
                                    break;
                                case 2: //turn right
                                    m_left.speed(1);
                                    m_right.speed(-1);
                                    wait(currentAction.second);
                                    m_left.speed(0);
                                    m_right.speed(0);
                                case 3: //turn left
                                    m_left.speed(-1);
                                    m_right.speed(1);
                                    wait(currentAction.second);
                                    m_left.speed(0);
                                    m_right.speed(0);
                            }
                        }
                        returnHome = false;
                        break;
                            
                    case '5': //button 5 up arrow go forward
                        if (bhit=='1') {
                            if (!noMove) {
                                _time.reset();
                                _time.start();
                                leftSpeed = 1;
                                rightSpeed = 1;
                                m_right.speed(rightSpeed);
                                m_left.speed(leftSpeed);
                            }
                        } else {                               
                            pc.printf("stopping\r\n");
                            _time.stop();
                            float moveTime = _time.read();
                            printf("\r\n,time: %f\r\n", moveTime);
                            previousMoves.push_back(make_pair<int, float>(1, moveTime));
                            _time.reset(); 
                            
                            leftSpeed = 0;
                            rightSpeed = 0;
                            m_right.speed(rightSpeed);
                            m_left.speed(leftSpeed);
                            //add release code here
                        }
                        break;
                    case '6': //button 6 down arrow go backward
                        if (bhit=='1') {
                            pc.printf("starting\r\n");
                            _time.start();
                            running = true;
                            
                            leftSpeed = -1;
                            rightSpeed = -1;
                            m_right.speed(rightSpeed);
                            m_left.speed(leftSpeed);
                        } else {
                            
                            pc.printf("stopping\r\n");
                            _time.stop();
                            float moveTime = _time.read();
                            printf("\r\n,time: %f\r\n", moveTime);
                            previousMoves.push_back(make_pair<int, float>(0, moveTime));
                            _time.reset(); 
                            
                            
                            leftSpeed = 0;
                            rightSpeed = 0;
                            m_right.speed(rightSpeed);
                            m_left.speed(leftSpeed);
                            //add release code here
                        }
                        break;
                        
                    case '7': //button 7 left arrow cart goes left
                        if (bhit=='1') {
                            _time.start();
                            leftSpeed = -1;
                            rightSpeed = 1;
                            m_right.speed(rightSpeed);
                            m_left.speed(leftSpeed);
                            //add hit code here
                        } else {
                            pc.printf("stopping\r\n");
                            _time.stop();
                            float moveTime = _time.read();
                            printf("\r\n,time: %f\r\n", moveTime);
                            previousMoves.push_back(make_pair<int, float>(2, moveTime));
                            _time.reset(); 
                            
                            
                            leftSpeed = 0;
                            rightSpeed = 0;
                            m_right.speed(rightSpeed);
                            m_left.speed(leftSpeed);
                        }
                        break;
                    case '8': //button 8 right arrow turns car right
                        if (bhit=='1') {
                            _time.reset();
                            _time.start();
                            
                            leftSpeed = 1;
                            rightSpeed = -1;
                            m_right.speed(rightSpeed);
                            m_left.speed(leftSpeed);
                        } else {
                            pc.printf("stopping\r\n");
                            _time.stop();
                            float moveTime = _time.read();
                            printf("\r\n,time: %f\r\n", moveTime);
                            previousMoves.push_back(make_pair<int, float>(3, moveTime));
                            _time.reset(); 
                            
                            leftSpeed = 0;
                            rightSpeed = 0;
                            m_right.speed(rightSpeed);
                            m_left.speed(leftSpeed);
                            //add release code here
                        }
                        break;
                    default:
                        break;
                }
            }
        }
        Thread::wait(10); 
    }  
}


//Timer attached to check if car too close to something
void detectObject(void) {
    if (lidarDistance < 200) 
    {
        if (!noMove) {
            leftSpeed = 0;
            rightSpeed = 0;
            m_right.speed(rightSpeed);
            m_left.speed(leftSpeed);
            noMove = true;
        }    
    }
    else
    {
        noMove = false;
    }
}

//main thread that starts the RC car
int main() {
    objectDetect.attach(&detectObject, 0.001);
    wheelsThread.start(wheels);
    lidarThread.start(lidar);
}