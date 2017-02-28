


#include <mdio_drv.h>






#define MDIO_SETUP_TIME 10
#define MDIO_HOLD_TIME 10

/* Minimum MDC period is 400 ns, plus some margin for error.  MDIO_DELAY
 * is done twice per period.
 */
#define MDIO_DELAY 250

/* The PHY may take up to 300 ns to produce data, plus some margin
 * for error.
 */
#define MDIO_READ_DELAY 350


extern PKT_MDIOM_T mdio_m;

///  MDIO pin definition (PA22).
//#define PIN_MDIO_MDC  {AT91C_PIO_PA21, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, (PIO_OPENDRAIN | PIO_PULLUP) }
#define PIN_MDIO_MDC  {AT91C_PIO_PA21, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, (PIO_OPENDRAIN | PIO_PULLUP) }
///  MDIO pin definition (PA23).
#define PIN_MDIO_MDIO  {AT91C_PIO_PA22, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, (PIO_OPENDRAIN | PIO_PULLUP)}
									
static const Pin MDIO_pins[] = {PIN_MDIO_MDC, PIN_MDIO_MDIO};



/* can support 3MHz MDC rate; (tested on C8051F321 @ 24MHz) */


#define HIGH			1
#define LOW				0
#define ENABLE			1
#define DISABLE			0


#define MDC_PP()       	MDIO_pins[0].pio->PIO_ODR = MDIO_pins[0].mask
#define MDC_OD()         
#define MDC_HIGH()    	MDIO_pins[0].pio->PIO_ODSR |= MDIO_pins[0].mask
#define MDC_LOW()       MDIO_pins[0].pio->PIO_ODSR &= (~MDIO_pins[0].mask)
#define MDC_SET(_v)     (_v) ? MDC_HIGH() :  MDC_LOW()
#define MDC_GET()       (MDIO_pins[0].pio->PIO_PDSR&MDIO_pins[0].mask)>>21

#define MDIO_PP()       MDIO_pins[1].pio->PIO_ODR = MDIO_pins[1].mask
#define MDIO_OD()        
#define MDIO_HIGH()    	MDIO_pins[1].pio->PIO_ODSR |= MDIO_pins[1].mask
#define MDIO_LOW()      MDIO_pins[1].pio->PIO_ODSR &= (!MDIO_pins[1].mask)
#define MDIO_SET(_v)    (_v) ? MDIO_HIGH() :  MDIO_LOW()
#define MDIO_GET()      ((MDIO_pins[1].pio->PIO_PDSR)>>22)&1






/* MDIO must already be configured as output. */
static void mdiobb_send_bit(uint8_t val)
{
//	MDIO_SET(val);
	if(val&1)
			MDIO_HIGH();
		else
			MDIO_LOW();
	__nop(); __nop(); __nop();
	MDC_HIGH();
	MDIO_GET();
	MDC_LOW();  
}

/* MDIO must already be configured as input. */
 uint8_t mdiobb_get_bit(void)
{
	uint8_t dat=0;

	MDIO_HIGH();
	MDC_LOW();  
	dat = MDIO_GET();
	MDC_HIGH();  
	return dat;
}

/* MDIO must already be configured as output. */
static void mdiobb_send_num( uint32_t val, uint8_t bits)
{
//	int_least8_t i;

//	for (i = bits - 1; i >= 0; i--)
//	{
//		mdiobb_send_bit(val >> i);
//	}
	uint8_t i=0;
	
	while(i<bits)
	{
		++i;
		mdiobb_send_bit(val >> (bits-i));		
	}
}

/* MDIO must already be configured as input. */
static uint16_t mdiobb_get_num( uint8_t bits)
{
	int_least8_t i;
	uint16_t ret = 0;
	
	   for (i = bits - 1; i >= 0; i--) {
		  ret <<= 1;
		  ret |= mdiobb_get_bit();
	  }

	return ret;
}

/* Utility to send the preamble, address, and
 * register (common to read and write).
 */
void mdiobb_cmd(uint32_t op, uint8_t phy, uint8_t DEVTYPE)
{
 //  uint8_t i;

   /*
	* Send a 32 bit preamble ('1's) with an extra '1' bit for good
	* measure.  The IEEE spec says this is a PHY optional
	* requirement.  The AMD 79C874 requires one after power up and
	* one after a MII communications error.  This means that we are
	* doing more preambles than we need, but it is safer and will be
	* much more robust.
	*/

	MDC_LOW();
	MDIO_GET();
//   for (i = 0; i < 32; i++)
//		mdiobb_send_bit(1);
	mdiobb_send_num(0xFFFFFFFF,32);

   /* send the start bit (01) and the read opcode (10) or write (10).
	  Clause 45 operation uses 00 for the start and 11, 10 for
	  read/write */
	mdiobb_send_num(0,2);
//	mdiobb_send_bit(0);
//	mdiobb_send_bit(0);// MDIO_C45
   
   mdiobb_send_num(op,2);
//   mdiobb_send_bit((op >> 1) & 1);
//   mdiobb_send_bit((op >> 0) & 1);

   mdiobb_send_num(phy, 5);
   mdiobb_send_num(DEVTYPE, 5);
}
/*
In clause 45 mode all commands are prefixed by MDIO_ADDR to specify the
  lower 16 bits of the 21 bit address. This transfer is done identically to a
  MDIO_WRITE except for a different code. To enable clause 45 mode or
  MII_ADDR_C45 into the address. Theoretically clause 45 and normal devices
  can exist on the same bus. Normal devices should ignore the MDIO_ADDR
  phase. */
uint16_t mdiobb_cmd_addr( uint16_t phy, uint32_t addr)
{
   uint16_t DEVTYPE = (addr >> 16) & 0x1F;
   uint32_t reg = addr & 0xFFFF;
   
   mdiobb_cmd(MDIO_C45_ADDR, phy, DEVTYPE);

   /* send the turnaround TA(10) */
	mdiobb_send_num(2,2);


   mdiobb_send_num(reg, 16);

	MDIO_HIGH();
	MDC_HIGH();  
	MDIO_GET();

   return DEVTYPE;
}
/*
reg 0...16 bit - address reg
	17...32
mdiobb_read(mdio_m.vPortAddr,((mdio_m.vDevAddr<<16)|(mdio_m.vOffset)));
*/
uint16_t mdiobb_read(uint16_t phy, uint32_t reg)
{
   uint16_t ret;

   if (reg & MDIO_C45_ADDR) {
	   reg = mdiobb_cmd_addr(phy, reg);
	   mdiobb_cmd(MDIO_C45_READ, phy, reg);
   } else
		mdiobb_cmd(MDIO_READ, phy, reg);

	MDC_HIGH();
	MDIO_GET();
   /* check the turnaround bit: the PHY should be driving it to zero, if this
	* PHY is listed in phy_ignore_ta_mask as having broken TA, skip that
	*/
   if (mdiobb_get_bit() != 0)
   {
		   /* PHY didn't drive TA low -- flush any bits it
			* may be trying to send.
			*/
		   mdiobb_get_num(64);

		   return 0xffff;
   }

   ret = mdiobb_get_num(16);
   return ret;
}
/*
*	
*
*/
uint16_t mdio_set_addr(uint16_t phy, uint32_t reg)
{
	if (reg & MDIO_C45_ADDR) {
	   mdiobb_cmd_addr(phy, reg);
		
	   }
	
	return 1;
}

/*
*	read data from mdio not set address
*/
uint16_t mdio_read_data(uint16_t phy, uint32_t reg)
{
   uint16_t ret;
	
	mdiobb_cmd(MDIO_C45_READ, phy, reg);
	MDC_HIGH();
   /* check the turnaround bit: the PHY should be driving it to zero, if this
	* PHY is listed in phy_ignore_ta_mask as having broken TA, skip that
	*/
   if (mdiobb_get_bit() != 0)
   {
	   /* PHY didn't drive TA low -- flush any bits it
		* may be trying to send.
		*/
		   mdiobb_get_num(32);

		   return 0xffff;
   }

   ret = mdiobb_get_num(16);
   return ret;
}

/*
*	write data to mdio slave not set address
*/
uint16_t mdio_write_data(uint16_t phy, uint32_t reg,uint16_t val)
{
   uint16_t ret;
	
	mdiobb_cmd(MDIO_C45_WRITE, phy, reg);

	MDC_HIGH();
/* send the turnaround (10) */
   mdiobb_send_bit(1);
   mdiobb_send_bit(0);

	mdiobb_send_num(val, 16);
//  mdiobb_get_bit();
	MDC_HIGH();
	MDIO_HIGH();
   return ret;
}


uint16_t mdiobb_write(uint16_t phy, uint32_t reg, uint16_t val)
{
 

   if (reg & MDIO_C45_ADDR) {
	   reg = mdiobb_cmd_addr(phy, reg);
	   mdiobb_cmd( MDIO_C45_WRITE, phy, reg);
   } else
		mdiobb_cmd(MDIO_WRITE, phy, reg);

   /* send the turnaround (10) */
   mdiobb_send_bit(1);
   mdiobb_send_bit(0);

   mdiobb_send_num(val, 16);

//   ctrl->ops->set_mdio_dir(0);
   MDC_HIGH();
   MDIO_GET();
//   mdiobb_get_bit();
   return 0;
}

uint16_t mdiobb_reset(void)
{
  // struct mdiobb_ctrl *ctrl = bus->priv;
//   if (ctrl->reset)
//		   ctrl->reset(bus);
   return 0;
}

/******************************************************************************
 * FUNCTION NAME:
 * DRV_MDIOM_Init
 * DESCRIPTION:
 * MDIO Driver Init.
 * PARAMETERS:
 *   N/A
 * RETURN:
 *   N/A
 * NOTES:
 *   N/A
 * HISTORY:
 *   2016.1.29        Panda.Xiong          Create
 *****************************************************************************/
void DRV_MDIOM_Init(void)
{
	PIO_Configure(MDIO_pins, PIO_LISTSIZE(MDIO_pins));
	//Синхронный вывод.
	MDIO_pins[0].pio->PIO_OWER = AT91C_PIO_PA21;
	MDIO_pins[1].pio->PIO_OWER = AT91C_PIO_PA22;

    /* stop frame */
    //drv_mdiom_StopFrame();
}





