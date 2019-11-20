/*
 * File:	boomer.cpp
 *
 * Author:	Bob Wilmes
 *
 */

#include "fsm.h"
#include "MicroBit.h"
#include "MicroBitSystemTimer.h"
#include <cmath>
#include <iostream>
#include <string>
#include <utility> // for std::move

#define	FALSE		 (0)
#define	TRUE		 (1)
#define	DEBUG		 (TRUE)
#define MAX_WRITE_STRING (256)

#ifndef ACCEL_WHOAMI_REGISTER 
#define ACCEL_WHOAMI_REGISTER (0x0f)
#endif

/*
 * Include Files
 *
 */

#include "LSM303Accelerometer.h"
#include "MMA8653.h"
#include "FXOS8700.h"

#ifndef MMA8653_WHOAMI_VAL
#define MMA8653_WHOAMI_VAL 0x5A
#endif

#ifndef LSM303_A_WHOAMI_VAL
#define LSM303_A_WHOAMI_VAL 0xC7
#endif

#ifndef FXOS8700_WHOAMI_VAL
#define FXOS8700_WHOAMI_VAL 0x33
#endif

#ifndef	I2C_STD_FREQUENCY
#define	I2C_STD_FREQUENCY	(100000)
#endif

/*
 * LMS303AGR	Symbolic Register Addresses
 *
 */

#ifndef	LMS303AGR_ACC_WHOAMI_REGISTER
#define LMS303AGR_ACC_WHOAMI_REGISTER		(0x0f)
#endif


#ifndef	LMS303AGR_ACC_OUTPUT_X_LOW_REGISTER
#define LMS303AGR_ACC_OUTPUT_X_LOW_REGISTER	(0x28)
#endif

#ifndef	LMS303AGR_ACC_OUTPUT_X_HIGH_REGISTER
#define LMS303AGR_ACC_OUTPUT_X_HIGH_REGISTER	(0x29)
#endif

#ifndef	LMS303AGR_ACC_OUTPUT_Y_LOW_REGISTER
#define LMS303AGR_ACC_OUTPUT_Y_LOW_REGISTER	(0x2A)
#endif

#ifndef	LMS303AGR_ACC_OUTPUT_Y_HIGH_REGISTER
#define LMS303AGR_ACC_OUTPUT_Y_HIGH_REGISTER	(0x2B)
#endif

#ifndef	LMS303AGR_ACC_OUTPUT_Z_LOW_REGISTER
#define LMS303AGR_ACC_OUTPUT_Z_LOW_REGISTER	(0x2C)
#endif

#ifndef	LMS303AGR_ACC_OUTPUT_Z_HIGH_REGISTER
#define LMS303AGR_ACC_OUTPUT_Z_HIGH_REGISTER	(0x2D)
#endif




/*
 * Error Codes
 *
 *
 */

#define BOOMER_ERROR_IC2_READ		(-4201);




/*
 * Global Variables
 *
 */

MicroBit 		uBit;
int		  	i2caddr = 0;
unsigned long gmax = 0;
uint64_t	systime = 0;

struct	coords {
	int xx;
	int yy;
	int zz;
	int scale;
	int click;
}	global;



/*
 * Start of code
 */

int Device_Initialization() {

	int acc_range  = 0;
	int acc_period = 0;

	global.xx = 0;
	global.yy = 0;
	global.zz = 0;
	global.scale = 1;
	global.click = 0;

	
	uBit.init();
	uBit.i2c = MicroBitI2C(I2C_SDA0, I2C_SCL0);
	uBit.i2c.frequency(I2C_STD_FREQUENCY);
	MicroBitAccelerometer accelerator = 
		MicroBitAccelerometer::autoDetect(uBit.i2c); 

	/*
	 * Kludge around missing whoami() method documented on
	 * bug request #447 lancaster-university/microbit-dal
	 * on github - June 26, 2019
	 *
	 */

	int isLSM303   = LSM303Accelerometer::isDetected(uBit.i2c);
	int isMMA8653  = MMA8653::isDetected(uBit.i2c);
	int isFXOS8700 = FXOS8700::isDetected(uBit.i2c);

/*	
	uBit.serial.printf("isLSM303    = (%d)\r\n",isLSM303);
	uBit.serial.printf("isMMA8653   = (%d)\r\n",isMMA8653);
	uBit.serial.printf("isFXOS8700  = (%d)\r\n",isFXOS8700);
*/
	int myaccel = 0;

	if      (isLSM303   == 1)  myaccel = LSM303_A_WHOAMI_VAL;
	else if (isMMA8653  == 1)  myaccel = MMA8653_WHOAMI_VAL;
	else if (isFXOS8700 == 1)  myaccel = FXOS8700_WHOAMI_VAL;
	else                       myaccel = 0; 


	if      (myaccel == LSM303_A_WHOAMI_VAL) {
		i2caddr   = LSM303_A_DEFAULT_ADDR; /*      */
		}
	else if	(myaccel == MMA8653_WHOAMI_VAL) {
		i2caddr   = MMA8653_DEFAULT_ADDR;  /* 0x3A */
		}	
	else if (myaccel == FXOS8700_WHOAMI_VAL) {
		i2caddr   = FXOS8700_DEFAULT_ADDR; /* 0x3C */
		}	
	else	 myaccel =  0;

	/*
	 * End of Kludge - sets myaccel to the WHOAMI_VAL
	 */
		
	if (myaccel == MMA8653_WHOAMI_VAL) {
	    uBit.serial.printf("Acclerometer: MMA8653\r\n");
	    }
	else
	    if (myaccel == LSM303_A_WHOAMI_VAL) {
	        uBit.serial.printf("Accelerometer: LSM303\r\n");
	    	}
	    else
		if (myaccel == FXOS8700_WHOAMI_VAL) {
	            uBit.serial.printf("Accelerometer: FXOS8700\r\n");
		    }
	        else
		    {
	            uBit.serial.printf("Accelerometer: Unknown\r\n");
		    }

	acc_period = uBit.accelerometer.getPeriod();
#ifdef	DEBUG
	uBit.serial.printf("Accelerometer period is (%d) ms\r\n", acc_period);
#endif
	acc_range  = uBit.accelerometer.getRange();
#ifdef	DEBUG
	uBit.serial.printf("Accelerometer range is (%d) G's\r\n", acc_range);
#endif

	acc_period = uBit.accelerometer.setPeriod(10);
	acc_range  = uBit.accelerometer.setRange(4);

	acc_period = uBit.accelerometer.getPeriod();
#ifdef	DEBUG
	uBit.serial.printf("Accelerometer period is (%d) ms\r\n", acc_period);
#endif
	acc_range    = uBit.accelerometer.getRange();
	global.scale = acc_range;
#ifdef	DEBUG
	uBit.serial.printf("Accelerometer range is (%d) G's\r\n", acc_range);
#endif
	systime = 0L;
	systime = system_timer_current_time();
#ifdef	DEBUG
	uBit.serial.printf("System Time is (%lu) \r\n", systime);
#endif
	
	return(0);
}


/*
 * function:	i2c_xyz_update
 *
 */

int	i2c_xyz_update()
{
	uint8_t	data[6];
	int	result = 0;
	int16_t	*x;
	int16_t	*y;
	int16_t	*z;
	uint64_t delta_time = 0L;

	if (global.scale < 1) global.scale = 1;

	result = uBit.i2c.MicroBitI2C::readRegister((uint8_t) i2caddr, LMS303AGR_ACC_OUTPUT_X_LOW_REGISTER | 0x80, data, 6);
	if (result != 0) {
#ifdef	DEBUG
		uBit.serial.printf("ERROR: i2c_xyz_update result = %X \r\n", result);
		wait(5);
#endif
		return MICROBIT_I2C_ERROR;
	}

	/*
	 * Read each value as a 16 bit little endian value and scale to 10 bits
	 *
	 */

	x = ((int16_t *) &data[0]);
	y = ((int16_t *) &data[2]);
	z = ((int16_t *) &data[4]);

	*x = *x / 32;
	*y = *y / 32;
	*z = *z / 32;

	/*
	 * Now scale to millig (approx) and align to ENU coordinates
	 */

	global.xx = -((int)(*y)) * global.scale;
	global.yy = -((int)(*x)) * global.scale;
	global.zz =  ((int)(*z)) * global.scale;

#ifdef	DEBUG
	if (global.zz > 5000)
	uBit.serial.printf("i2c_xyz_update x=%d y=%d z=%d\r\n", global.xx, global.yy, global.zz );
#endif
#ifdef	DEBUG
	if (global.zz > 2000) {
		global.click++;
		if (global.click >= 5) {
			if (global.xx < 0 | global.yy < 0)
			uBit.serial.printf("tap time %ld x=%d y=%d z=%d click=%d\r\n", system_timer_current_time(), global.xx, global.yy, global.zz, global.click );
		}
		global.click %= 8;
	}
	else global.click = 0;
#endif
	global.xx = 0;
	global.yy = 0;
	global.zz = 0;

	return MICROBIT_OK;

}



/*
 * function:	i2c_read_reg
 *
 */

int	i2c_read_reg(char buf[], uint8_t i2c_reg) {

	int status = 0;
	int i      = 0;
	int j      = 0;
	

	i = strlen((char *) &buf[0]);
	if (i < 0 || i > MAX_WRITE_STRING)  {
		return 0;
	}

	for (j = 0; j < i; j++) {
#ifdef	DEBUG2
	uBit.serial.printf("calling read register addr = %X, reg = %X, buf[%d] = %X, length = 1\r\n", (uint8_t) i2caddr, (uint8_t) i2c_reg, j, (uint8_t) buf[j]);
#endif
	status = 
	uBit.i2c.MicroBitI2C::readRegister((uint8_t) i2caddr, (uint8_t) i2c_reg,        (uint8_t*) &buf[j], (int) 1);

#ifdef	DEBUG2
	uBit.serial.printf("returned read register addr = %X, reg = %X, buf[%d] = %X, length = 1\r\n", (uint8_t) i2caddr, (uint8_t) i2c_reg, j, (uint8_t) buf[j]);
#endif
#ifdef	DEBUG
		if (status != MICROBIT_OK) {
		    uBit.serial.printf("i2c_read_reg failed status=%d (%d) %X\r\n", 
				status, j, buf[j]);
		    break;
		}
/*
	if (status == MICROBIT_OK)
	uBit.serial.printf("i2c_read_reg status = %d (MICROBIT_OK) (%x)\r\n", status, buf[j]);
	else {
	uBit.serial.printf("i2c_read_reg status = %d (ERROR) (%x)\r\n", status, buf[j]);
	return BOOMER_ERROR_IC2_READ;
	}
*/

#endif
	}
	return j;
}


/*
 * function:	i2c_write_reg
 *
 */

int	i2c_write_reg(char buf[], int i2c_reg) {

	int status = 0;
	int i      = 0;
	int j      = 0;
	

	i = strlen(buf);
	if (i < 0 || i > MAX_WRITE_STRING)  {
		return 0;
	}

	for (j = 0; j < i; j++) {
	status = 
	uBit.i2c.MicroBitI2C::writeRegister((uint8_t) i2caddr, (uint8_t) i2c_reg, (uint8_t) buf[j]); 
#ifdef	DEBUG
		if (status != MICROBIT_OK)
		    uBit.serial.printf("i2c_write_reg failed status=%d (%d) %X\r\n", 
				status, j, buf[j]);
#endif
	}
	return j;
}


/*
 * function:	i2c_whoami
 *
 */

int	i2c_whoami() {

	int i = 0;
	int status = 0;
	char buf[] = { 0x0f, 0x00 };
	char buf2[] = { 0xff, 0x00 };
	status = uBit.i2c.write(i2caddr, buf, 1, true);
	if (status == MICROBIT_I2C_ERROR) { /* status = -1010 */
	        uBit.serial.printf("i2c.write STATUS==MICROBIT_I2C_ERROR: status = %d i2c_whoami %X\r\n", status, buf[0]);
	}
	status = uBit.i2c.read(i2caddr, buf2, 1);
	if (status == MICROBIT_OK) {
		i = (int) buf2[0];
#ifdef	DEBUG
	        uBit.serial.printf("STATUS=MICROBIT_OK: i2c_whoami %X\r\n", buf2[0]);
#endif
	}
	else {
		i = -1;
		if (status == MICROBIT_I2C_ERROR) {
#ifdef	DEBUG
	        uBit.serial.printf("STATUS==MICROBIT_I2C_ERROR: status = %d i2c_whoami %X\r\n", status, buf2[0]);
#endif		
		}
	}
#ifdef	DEBUG
	uBit.serial.printf("i2c_whoami %X\r\n", buf2[0]);
#endif
	return i;
}



/*
 * function:	i2c_xyz()
 *
 */

int	i2c_xyz() {

	int	status = 0;
	int     i = 0;
	unsigned	x = 0;
	unsigned	y = 0;
	unsigned	z = 0;
	unsigned long	gforce = 0;

	char xyz_reg[] = {
	LMS303AGR_ACC_OUTPUT_X_LOW_REGISTER,
	LMS303AGR_ACC_OUTPUT_X_HIGH_REGISTER,
	LMS303AGR_ACC_OUTPUT_Y_LOW_REGISTER,
	LMS303AGR_ACC_OUTPUT_Y_HIGH_REGISTER,
	LMS303AGR_ACC_OUTPUT_Z_LOW_REGISTER,
	LMS303AGR_ACC_OUTPUT_Z_HIGH_REGISTER,
	0x00
	};

        char xyz_val[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };	
	char xyz_buf[] = { 0x00, 0x00 };

	


	for (i = 0; i < (sizeof(xyz_reg)/sizeof(char)); i++) {
		xyz_buf[0] = 0xff;
		xyz_buf[1] = 0x00;
		status = i2c_read_reg(&xyz_buf[0], xyz_reg[i]);
		if (status < 0) {
  		    uBit.serial.printf("Error: XYZ status=%d i=%d\r\n", status, i);
		    break;
		}
		else
		    xyz_val[i] = xyz_buf[0];
	}

	x = xyz_val[0] << 8 | xyz_val[1];
	y = xyz_val[2] << 8 | xyz_val[3];
	z = xyz_val[4] << 8 | xyz_val[5];
	
	if (x == 0) x = 1;
	if (y == 0) y = 1;
	if (z == 0) z = 1;

	gforce = (x * x + y * y + z * z)/10;

	/* uBit.serial.printf("g %ld\r\n", gforce); */

	if (gforce > gmax) {
		gmax = gforce;
		uBit.serial.printf("gmax %ld\r\n", gmax);
		wait(1);
	}


	uBit.serial.printf("%d %d %d\r\n", x, y, z);

  	return status;
}


/*
 * function:	i2c_info()
 *
 */

int	i2c_info() {

#ifdef	DEBUG
  uBit.serial.printf("MicroBit DAL version %s\r\n", microbit_dal_version());
  uBit.serial.printf("MicroBit Friendly Name %s\r\n", microbit_friendly_name());
  uBit.serial.printf("MicroBit Serial Number %ld\r\n", 
		  microbit_serial_number());
  uBit.serial.printf("Bluetooth Running: %s\r\n", 
		(ble_running() ? "true" : "false"));
#endif
	return 1;
}




/*
 * function:	i2c_temperature()
 *
 */

int	i2c_temperature() {

	int i = 0;

	char buf[] = { 0x0F, 0x00 };
	uBit.i2c.write(0x1C, buf, 1, true);
	uBit.i2c.read(0x1C, buf, 1);

	i = (int) buf[0];
	i = ((i*9)/5)+32;
#ifdef	DEBUG
	uBit.serial.printf("i2c_temperature (%d) %X\r\n", i,  (int) buf[0]);
#endif
	return i;
}


int main() {


	Device_Initialization();

	uBit.display.scroll("MicroBit Test");
	uBit.serial.printf("MicroBit Test\r\n");

	int status = 0;
	char buf[] = { 0xcf, 0x00 };
	status = i2c_read_reg(buf, LMS303AGR_ACC_WHOAMI_REGISTER);
	uBit.serial.printf("main_i2c_read_reg status = %d (%x)\r\n", status, buf[0]);
	i2c_info();
	while (1) {
	    /* i2c_xyz(); */
	    i2c_xyz_update();
	}

	release_fiber();
	return 0;
}

