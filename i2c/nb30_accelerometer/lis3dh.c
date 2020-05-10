#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/ioctl.h>

#define WHO_AM_I     0x0F
#define CTRL_REG1    0x20
#define CTRL_REG4    0x23
#define STATUS_REG   0x27
#define TEMP_CFG_REG 0x1F

#define OUT_X_L      0x28
#define OUT_X_H      0x29
#define OUT_Y_L      0x2A
#define OUT_Y_H      0x2B
#define OUT_Z_L      0x2C
#define OUT_Z_H      0x2D

#define OUT_ADC3_L   0x0C
#define OUT_ADC3_H   0x0D

int file;

void print_reg()
{
    uint8_t i = 0x00;
    uint8_t reg = 0x00;
    
    printf(">> dispaly all registers\n         0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F");
    
    for(i = 0x00; i < 0x40; i++)
    {
        if(i%16==0) printf("\n   %02X | ", i);
        
        reg = i2c_smbus_read_byte_data(file, i);
        
        printf("%02X ", reg);
        
    }
}

void read_acc()
{
    uint8_t status = 0;
    
    uint8_t acc_x_l = 0;
    uint8_t acc_x_h = 0;
    uint8_t acc_y_l = 0;
    uint8_t acc_y_h = 0;
    uint8_t acc_z_l = 0;
    uint8_t acc_z_h = 0;
    
    int16_t acc_x = 0;
    int16_t acc_y = 0;
    int16_t acc_z = 0;
    
    uint8_t adc3_l = 0;
    uint8_t adc3_h = 0;
    int16_t tempera = 0;
    
    while( (status&0b00001000)==0  )
    {
       status = i2c_smbus_read_byte_data(file, STATUS_REG);
       //printf("\nwaiting...");
    }
    
    acc_x_l = i2c_smbus_read_byte_data(file, OUT_X_L);
    acc_x_h = i2c_smbus_read_byte_data(file, OUT_X_H);
    acc_y_l = i2c_smbus_read_byte_data(file, OUT_Y_L);
    acc_y_h = i2c_smbus_read_byte_data(file, OUT_Y_H);
    acc_z_l = i2c_smbus_read_byte_data(file, OUT_Z_L);
    acc_z_h = i2c_smbus_read_byte_data(file, OUT_Z_H);
    
    adc3_l = i2c_smbus_read_byte_data(file, OUT_ADC3_L);
    adc3_h = i2c_smbus_read_byte_data(file, OUT_ADC3_H);
    
    acc_x = (acc_x_h << 8)|acc_x_l;
    acc_y = (acc_y_h << 8)|acc_y_l;
    acc_z = (acc_z_h << 8)|acc_z_l;
    
    tempera = (adc3_h << 8)|adc3_l;
    
    //12bit, 0-4095/-2047-2048
    acc_x >>= 4;
    acc_y >>= 4;
    acc_z >>= 4;
    
    tempera >>= 8; // the sensor is only 8bit?
    
    //printf("H = %2X, L = %2X, temperature = %5d\n", adc3_h, adc3_l, tempera);
    printf("X = %5d, Y = %5d, Z = %5d, T = %5d\n", acc_x, acc_y, acc_z, tempera);

}


int main()
{

    char *buffer;
    //uint8_t addr = 0x1D; // 0b00011101        The I2C address of the lis331dl
    uint8_t addr = 0x19;   // 0b00011001        The I2C address of the lis3dh

    
    if ((file = open("/dev/i2c-6",O_RDWR)) < 0)
    {
        printf("\n>> Failed to open the bus.\n");
        exit(1);
    }

    if (ioctl(file,I2C_SLAVE,addr) < 0)
    {
        printf("\n>> Failed to acquire bus access and/or talk to slave.\n");
        exit(1);
    }
    
    // i2c read
    uint8_t return_t = 0;
    
    return_t = i2c_smbus_read_byte_data(file, WHO_AM_I);
    
    // ID = 00110011b
    if(return_t == 0x33)
    {
        printf("\n>> LIS3DH found ! ID = 0x%02X\n", return_t);
        
        print_reg();
        
        printf("\n\n>> please enter 'y' to set registers\n");
        while(getchar()!='y'){}
        
        //write 0x77 to CTRL_REG1, 400Hz
        printf("\n>> enable sensor");   
        i2c_smbus_write_byte_data(file, CTRL_REG1, 0x77);
        
        //write 0x88 to CTRL_REG4, HR, 2g, enable dac
        printf("\n>> enable high resolution");   
        i2c_smbus_write_byte_data(file, CTRL_REG4, 0x88);
        
        //write 0xC0 to TEMP_CFG_REG, enable temperature sensor
        printf("\n>> enable temperature sensor");   
        i2c_smbus_write_byte_data(file, TEMP_CFG_REG, 0xC0);
        
        //read CTRL_REG1
        return_t = i2c_smbus_read_byte_data(file, CTRL_REG1);
        printf("\n   CTRL_REG1 0x20 = 0x%2X", return_t);
        
        //read CTRL_REG4
        return_t = i2c_smbus_read_byte_data(file, CTRL_REG4);
        printf("\n   CTRL_REG4 0x23 = 0x%2X", return_t);
        
        //read TEMP_CFG_REG
        return_t = i2c_smbus_read_byte_data(file, TEMP_CFG_REG);
        printf("\n   TEMP_CFG_REG 0x1F = 0x%2X\n", return_t);
        
        printf("\n>> please enter 'y' to read from sensor\n");
        while(getchar()!='y'){}
        
        while(1)
        {
            read_acc();
        }
    }
    
    else
    { 
        printf("\n>> No LIS3DH found !\n");
    }
    
    return 0;
}

