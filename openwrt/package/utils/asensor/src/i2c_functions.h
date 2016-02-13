/*
 Functions for communicating over the I2C bus
 Author: Adam Benda<adam@adasek.cz>

*/

//Write any number of Bytes to the device
static int i2c_wrnB(int file,unsigned char addr, unsigned char * data, unsigned char dataSize) {
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[1];

    messages[0].addr  = addr;
    messages[0].flags = 0;
    messages[0].len   = dataSize;
    messages[0].buf   = data;

    packets.msgs      = messages;
    packets.nmsgs     = 1;
    if(ioctl(file, I2C_RDWR, &packets) < 0) {
        return 1;
    }

    return 0;
}

//Writes 1 byte to the device
static int i2c_wr1B(int file,unsigned char addr, unsigned char data) {
return i2c_wrnB(file,addr,&data,1);

    unsigned char inbuf, outbuf;
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[1];

    outbuf = data;
    messages[0].addr  = addr;
    messages[0].flags = 0;
    messages[0].len   = sizeof(outbuf);
    messages[0].buf   = &outbuf;

    /* Send the request to the kernel and get the result back */
    packets.msgs      = messages;
    packets.nmsgs     = 1;
    if(ioctl(file, I2C_RDWR, &packets) < 0) {
        return 1;
    }

    return 0;
}




//Reads two bytes out of device
static int i2c_rd2B(int file,unsigned char addr, unsigned char * data) {
    unsigned char buf[4];
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[1];
	

    messages[0].addr  = addr;
    messages[0].flags = I2C_M_RD;
    messages[0].len   = 4;
    messages[0].buf   = buf;

    /* Send the request to the kernel and get the result back */
    packets.msgs      = messages;
    packets.nmsgs     = 1;
    if(ioctl(file, I2C_RDWR, &packets) < 0) {
        return 1;
    }
    data[0] = buf[0];
    data[1] = buf[1];
  
    return 0;
}


static int get_i2c_register(int file,unsigned char addr, unsigned char reg,unsigned char * data){
//first let's write 1B / register address
if (0!=i2c_wr1B(file,addr,reg)){
 return 1;
}

//now let's read from the device
if (0!=i2c_rd2B(file,addr, data)){
 return 2;
}
data[1]=0;

return 0;
}

static int set_i2c_register(int file,unsigned char addr, unsigned char reg,unsigned char data){
//first let's write 1B / register address
unsigned char dataToSend[2];
dataToSend[0]=reg;
dataToSend[1]=data;

if (0!=i2c_wrnB(file,addr,dataToSend,2)){
 return 1;
}


return 0;
}
