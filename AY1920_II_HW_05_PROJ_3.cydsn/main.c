/**
* \brief Main source file for the I2C-Master project.
*
* In this project we set up a I2C master device with
* to understand the I2C protocol and communicate with a
* a I2C Slave device (LIS3DH Accelerometer).
*
* \author Gabriele Belotti
* \date , 2020*/

// Include required header files
#include "I2C_Interface.h"
#include "project.h"
#include "stdio.h"

//brief 7-bit I2C address of the slave device.
#define LIS3DH_DEVICE_ADDRESS 0x18

//brief Address of the WHO AM I register
#define LIS3DH_WHO_AM_I_REG_ADDR 0x0F

//brief Address of the Status register
#define LIS3DH_STATUS_REG 0x27

//brief Address of the Control register 1
#define LIS3DH_CTRL_REG1 0x20

//brief Hex value to set high resolution mode 100Hz to the accelerator
#define LIS3DH_NORMAL_MODE_100_CTRL_REG1 0x57

//brief Address of the Control register 4
#define LIS3DH_CTRL_REG4 0x23

//output registers updated after MSB and LSB reading, FSR [-4g;+4g], high resolution mode
#define LIS3DH_CTRL_REG4_BDU_ACTIVE 0x98 

//brief Address of the out_x_l register
#define LIS3DH_OUT_X_L  0x28

//brief Address of the out_y_l register
#define LIS3DH_OUT_Y_L  0x2A

//brief Address of the out_z_l register
#define LIS3DH_OUT_Z_L  0x2C

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    I2C_Peripheral_Start(); //I2C enabled
    UART_Debug_Start(); // UART enabled
    
    CyDelay(5); //"The boot procedure is complete about 5 milliseconds after device power-up."
    
    // String to print out messages on the UART
    char message[50];

    // Check which devices are present on the I2C bus
    for (int i = 0 ; i < 128; i++)
    {
        if (I2C_Peripheral_IsDeviceConnected(i))
        {
            // print out the address is hex format
            sprintf(message, "Device 0x%02X is connected\r\n", i);
            UART_Debug_PutString(message); 
        }
        
    }
    
    /******************************************/
    /*            I2C Reading                 */
    /******************************************/
    
    /* Read WHO AM I REGISTER register */
    uint8_t who_am_i_reg;
    ErrorCode error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                                  LIS3DH_WHO_AM_I_REG_ADDR, 
                                                  &who_am_i_reg);
    if (error == NO_ERROR)
    {
        sprintf(message, "WHO AM I REG: 0x%02X [Expected: 0x33]\r\n", who_am_i_reg);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm\r\n");   
    }
    
    /*      I2C Reading Status Register       */
    
    uint8_t status_register; 
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_STATUS_REG,
                                        &status_register);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "STATUS REGISTER: 0x%02X\r\n", status_register);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read status register\r\n");   
    }
    
    /******************************************/
    /*        Read Control Register 1         */
    /******************************************/
    uint8_t ctrl_reg1; 
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG1,
                                        &ctrl_reg1);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 1: 0x%02X\r\n", ctrl_reg1);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read control register 1\r\n");   
    }
    
    /******************************************/
    /*            I2C Writing                 */
    /******************************************/
    
        
    UART_Debug_PutString("\r\nWriting new values..\r\n");
    
    if (ctrl_reg1 != LIS3DH_NORMAL_MODE_100_CTRL_REG1)
    {
        ctrl_reg1 = LIS3DH_NORMAL_MODE_100_CTRL_REG1;
    
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_CTRL_REG1,
                                             ctrl_reg1);
    
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 1 successfully written as: 0x%02X\r\n", ctrl_reg1);
            UART_Debug_PutString(message); 
        }
        else
        {
            UART_Debug_PutString("Error occurred during I2C comm to set control register 1\r\n");   
        }
    }
    
    /******************************************/
    /*     Read Control Register 1 again      */
    /******************************************/

    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG1,
                                        &ctrl_reg1);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 1 after overwrite operation: 0x%02X\r\n", ctrl_reg1);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read control register 1\r\n");   
    }
    
     /******************************************/
     /* Read Control Register 4 */
     /******************************************/

    
    
    uint8_t ctrl_reg4;

    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG4,
                                        &ctrl_reg4);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 4: 0x%02X\r\n", ctrl_reg4);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read control register4\r\n");   
    }
    
    
    ctrl_reg4 = LIS3DH_CTRL_REG4_BDU_ACTIVE; 
    
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_CTRL_REG4,
                                         ctrl_reg4);
    
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG4,
                                        &ctrl_reg4);
    
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 4 after being updated: 0x%02X\r\n", ctrl_reg4);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read control register4\r\n");   
    }
    
    /*int16_t Out_X;
    int16_t Out_Y;
    int16_t Out_Z;
    int16_t Out_X_mg;
    int16_t Out_Y_mg;
    int16_t Out_Z_mg;
    uint8_t header = 0xA0;
    uint8_t footer = 0xC0;
    uint8_t OutArray[8]; 
    uint8_t X_Data[2];
    uint8_t Y_Data[2];
    uint8_t Z_Data[2];
    uint8_t status_reg;
    
    OutArray[0] = header;
    OutArray[7] = footer;
    */
    /*for(;;)
    {error= I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS, //read the status register
                                        LIS3DH_STATUS_REG,
                                        &status_reg);
        //CyDelay(5); //output data at 100Hz = data available every 10ms, so the delay must be lower
        while(!(status_reg & 0x08))//check if new data is available on all axes
        
        {error= I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS, //read the status register until new data is available
                                            LIS3DH_STATUS_REG,
                                           &status_reg);}
        if (error==NO_ERROR)
         {
            error = I2C_Peripheral_ReadRegisterMulti(LIS3DH_DEVICE_ADDRESS,
                                            LIS3DH_OUT_X_L,
                                            2,
                                            &X_Data[0]);
            
            error = I2C_Peripheral_ReadRegisterMulti(LIS3DH_DEVICE_ADDRESS,
                                            LIS3DH_OUT_Y_L,
                                            2,
                                            &Y_Data[0]);
            
            error = I2C_Peripheral_ReadRegisterMulti(LIS3DH_DEVICE_ADDRESS,
                                            LIS3DH_OUT_Z_L,
                                            2,
                                            &Z_Data[0]);
                
             
            if (error == NO_ERROR)
            {sprintf(message, "OUT_X_L: 0x%02X\r\n", X_Data[0]);
             UART_Debug_PutString(message); 
             sprintf(message, "OUT_X_H: 0x%02X\r\n", X_Data[1]);
             UART_Debug_PutString(message); 
            }
            if(error == NO_ERROR)
              {
               Out_X = (int16)((X_Data[0] | (X_Data[1]<<8)))>>6;//out_x in digit (10bit)
               Out_X_mg= Out_X*4;//out_x in mg (12 bits needed [-2048;+2048])
               OutArray[1] = (uint8_t)(Out_X_mg & 0xFF);
               OutArray[2] = (uint8_t)(Out_X_mg >> 8);
               
               Out_Y = (int16)((Y_Data[0] | (Y_Data[1]<<8)))>>6;//out_y in digit (10bit)
               Out_Y_mg= Out_Y*4;//out_y in mg, the sensitivity is 4mg/digit in this operation mode(12 bits needed)
               OutArray[3] = (uint8_t)(Out_Y_mg & 0xFF);
               OutArray[4] = (uint8_t)(Out_Y_mg >> 8);
            
               Out_Z = (int16)((Z_Data[0] | (Z_Data[1]<<8)))>>6;//out_z in digit (10bit)
               Out_Z_mg= Out_Z*4;//out_z in mg (12 bits needed)
               OutArray[5] = (uint8_t)(Out_Z_mg & 0xFF);
               OutArray[6] = (uint8_t)(Out_Z_mg>> 8);
               //sprintf(message, "OUT_Z: %d\r\n OUT_Z_MG: %d\r\n",Out_Z, Out_Z_mg);
               //UART_Debug_PutString(message);
               UART_Debug_PutArray(OutArray, 8);
              }
            }
         }*/
   }
/* [] END OF FILE */
