#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/ioctl.h>

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

//convert 2's complement to normal
int convert(uint8_t data)
{
    uint8_t  tmp = data;
    
    if( tmp>>7 )
    {
        tmp = ~tmp;
        tmp = tmp + 1;
        
        return (int)(-tmp);
    }
    else
    { return (int)tmp; }
}

void read_acc()
{
    uint8_t status = 0;
    
    uint8_t acc_x_raw = 0;
    uint8_t acc_y_raw = 0;
    uint8_t acc_z_raw = 0;
    
    int acc_x = 0;
    int acc_y = 0;
    int acc_z = 0;
    
    int resolu = 1;
    
    while( (status&0b00001000)==0  )
    {
       status = i2c_smbus_read_byte_data(file, 0x27);
       //printf("\nwaiting...");
    }
        
    acc_x_raw = i2c_smbus_read_byte_data(file, 0x29);
    acc_y_raw = i2c_smbus_read_byte_data(file, 0x2B);
    acc_z_raw = i2c_smbus_read_byte_data(file, 0x2D);
    
    acc_x = convert(acc_x_raw);
    acc_y = convert(acc_y_raw);
    acc_z = convert(acc_z_raw);
    
    printf("X = %3d, Y = %3d, Z = %3d\n", acc_x*resolu, acc_y*resolu, acc_z*resolu );
}


int main()
{

    char filename[40];
    char *buffer;
    uint8_t addr = 0x1D;     // 0b00011101    The I2C address of the lis331dl
    //uint8_t addr = 0x19;   // 0b00011001    The I2C address of the lis3dh

    sprintf(filename,"/dev/i2c-6");
    if ((file = open(filename,O_RDWR)) < 0)
    {
        printf("Failed to open the bus.");
        exit(1);
    }

    if (ioctl(file,I2C_SLAVE,addr) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
        exit(1);
    }
    
    // i2c read
    uint8_t return_t = 0;
    
    return_t = i2c_smbus_read_byte_data(file, 0x0f);
    
    if(return_t == 0x3b)
    {
        printf("\n>> LIS331DL found ! ID = 0x%02X\n", return_t);
        
        print_reg();
        
        printf("\n\n>> please enter 'y' to read from device\n");
        
        while(getchar()!='y'){}
        
        //read CTRL_REG1
        return_t = i2c_smbus_read_byte_data(file, 0x20);
        printf("\nreg 0x20 = 0x%2X\n\n", return_t);
        
        //     100KHz   400KHz
        // 2g  0x47     0xC7      
        // 8g  0x67     0xE7
        
        if( return_t != 0xC7 )
        {
            //write 0x47H to CTRL_REG1(0x20H)
            i2c_smbus_write_byte_data(file, 0x20, 0xC7);
            
            return_t = i2c_smbus_read_byte_data(file, 0x20);
            printf("enable sensor, reg 0x20 = 0x%2X\n\n", return_t);        
        }
        
        while(1)
        {
            read_acc();
        }
    }
    
    else
    { 
        printf("\n>> No LIS331DL found !\n");
    }
    
    return 0;
}

