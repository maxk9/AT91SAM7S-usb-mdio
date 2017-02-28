#ifndef __MDIO_BITBANG_H
#define __MDIO_BITBANG_H
 
#include <board.h>
#include <pio.h>
 

#define MDIO_READ 2
#define MDIO_WRITE 1

#define MDIO_C45 (1<<15)	//0x8000
#define MDIO_C45_ADDR (MDIO_C45 | 0)
#define MDIO_C45_READ (MDIO_C45 | 3)
#define MDIO_C45_WRITE (MDIO_C45 | 1)


#define PKT_PayloadMaxLen	2

 
typedef struct
{
    uint8_t   vPortAddr;
    uint8_t   vDevAddr;
    uint16_t  vOffset;
    uint16_t  vDataCount;
    uint16_t  aData[2];
} PKT_MDIOM_T;


uint16_t mdiobb_cmd_addr( uint16_t phy, uint32_t addr);
uint16_t mdiobb_read(uint16_t phy, uint32_t reg);
uint16_t mdiobb_write(uint16_t phy, uint32_t reg, uint16_t val);
uint16_t mdiobb_reset(void);
 uint8_t mdiobb_get_bit(void);
void mdiobb_cmd(uint32_t op, uint8_t phy, uint8_t DEVTYPE);
uint16_t mdio_read_data(uint16_t phy, uint32_t reg);
uint16_t mdio_set_addr(uint16_t phy, uint32_t reg);
uint16_t mdio_write_data(uint16_t phy, uint32_t reg,uint16_t val);



#endif






