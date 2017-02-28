#include "config.h"
#include "board.h"
#include <string.h>
#include <stdlib.h>
#include "microrl.h"
#include <lcd_siem_s65.h>
#include <stdio.h>
#include <mdio_drv.h>



/*
AVR platform specific implementation routines (for Atmega8, rewrite for your MC)
*/
#define _AVR_DEMO_VER "1.0"

// definition commands word
#define _CMD_HELP		"help"
#define _CMD_CLEAR		"clear"
#define _CMD_S_M_PHY	"set_MDIO_PHY"
#define _CMD_S_M_DEV	"set_MDIO_dev"
	#define _SCMD_M_DEV_1	"PMA/PMD"
	#define _SCMD_M_DEV_2	"WIS"
	#define _SCMD_M_DEV_3	"PCS"
	#define _NUM_OF_SETCLEAR_SCMD 3
	
#define _CMD_S_M_REG		"set_MDIO_reg_addr"
#define _CMD_R_M			"read_MDIO"

#define _CMD_R_M_W		"read_MDIO_word"
	#define _SCMD_R_M_W_1	"8000"

#define _CMD_W_M_W		"write_MDIO_word"
	#define _SCMD_W_M_W_2	"data"
#define _CMD_PR_M_W		"post_read_MDIO_inc"
#define _CMD_PRINT_LCD	"print_LCD"
#define _CMD_R_GPIO	"GPIO_read"
// arguments for set/clear


#define _NUM_OF_CMD 11



//available  commands
char * keyworld [] = {_CMD_HELP, _CMD_CLEAR, _CMD_S_M_PHY, _CMD_S_M_DEV, _CMD_S_M_REG,_CMD_R_M,_CMD_R_M_W, 	\
					_CMD_W_M_W, _CMD_PR_M_W, _CMD_PRINT_LCD,_CMD_R_GPIO };
// 'set/clear' command argements
char * set_clear_key [] = {_SCMD_M_DEV_1, _SCMD_M_DEV_2, _SCMD_M_DEV_3};

// array for comletion
char * compl_world [_NUM_OF_CMD + 1];





extern uint8_t TXCDC[TXCDC_BUF_SIZE];//главный кольцевой буфер передачи
extern uint16_t inTXCDC, outTXCDC;	

extern PKT_MDIOM_T mdio_m;

// create microrl object and pointer on it
microrl_t rl;
microrl_t * prl = &rl;


//*****************************************************************************
void print (const char * srt)
{
	uint16_t i = 0;
	while (srt [i] != 0) {
		TXCDC[inTXCDC++] = srt[i++]; 
		inTXCDC &= TXCDC_BUF_MASK;
	}
}



//*****************************************************************************
void print_help (void)
{
	print ("Use TAB key for completion\n\rCommand:\n\r");
	print ("\033[31m clear\033[0m - clear screen\n\r");
	print ("\033[31m set_MDIO_PHY\033[0m addr - support only 3 bit or 5 bit\n\r");
	print ("\033[31m set_MDIO_dev\033[0m num - support only:\n\r");
	print ("\t\033[31m 1\033[0m - PMA/PMD\n\r");
	print ("\t\033[31m 2\033[0m - WIS\n\r");
	print ("\t\033[31m 3\033[0m - PCS\n\r");
	print ("\033[31m read_MDIO_word\033[0m addr - MDIO read word from HEX address\n\r");
	print ("\033[31m write_MDIO_word\033[0m data - MDIO write word to last address\n\r");
	print ("\033[31m post_read_MDIO_inc\033[0m addr num_w - MDIO read num_w from HEX address witch increment\n\r");
	print ("\033[31m print_LCD\033[0m text - print message on LCD\n\r");
}


//*****************************************************************************
// execute callback for microrl library
// do what you want here, but don't write to argv!!! read only!!
uint16_t execute (uint16_t argc, const char * const * argv)
{
	uint16_t i = 0, val,pin;
	uint32_t reg;
	char str[25] = {0,},*end;
	unsigned char * port;
	// just iterate through argv word and compare it with your commands
	while (i < argc) {
		if (strcmp (argv[i], _CMD_HELP) == 0) {
			print ("microrl v");
			print (MICRORL_LIB_VER);
			print (" library AVR DEMO v");
			print (_AVR_DEMO_VER);
			print("\n\r");
			print_help ();        // print help
		}
		else 
		if (strcmp (argv[i], _CMD_CLEAR) == 0) {
			print ("\033[2J");    // ESC seq for clear entire screen
			print ("\033[H");     // ESC seq for move cursor at left-top corner
		} 
		else 
		if (strcmp(argv[i], _CMD_S_M_PHY) == 0) {	//"set_MDIO_PHY"
			if (++i < argc) {
				val = atoi(argv[i]);
				mdio_m.vPortAddr = val;
				snprintf (str, 25, "set PHY %2d",mdio_m.vPortAddr);
				LCD_put_string(2,STR1,str,0, BLACK);
				
				print("set addr ");
				print(argv[i]);
				print("\n\r");
				return 0;
			} else {
					print ("use specify addr, use Tab\n\r");
				return 1;
			}
		}
		else
		if (strcmp (argv[i], _CMD_S_M_DEV) == 0) {	//"set_MDIO_dev"
			if (++i < argc) {
				if (strcmp(argv[i], _SCMD_M_DEV_1) == 0)
				{
					snprintf (str, 25, "set dev is %s",_SCMD_M_DEV_1);
					LCD_put_string(2,STR2,str,0, BLACK);
					mdio_m.vDevAddr = 1;
				}
				else
				if (strcmp(argv[i], _SCMD_M_DEV_2) == 0)
				{
					snprintf (str, 25, "set dev is %s",_SCMD_M_DEV_2);
					LCD_put_string(2,STR2,str,0, BLACK);
					mdio_m.vDevAddr = 2;
				}
				else
					if (strcmp(argv[i], _SCMD_M_DEV_3) == 0)
				{
					snprintf (str, 25, "set dev is %s",_SCMD_M_DEV_3);
					LCD_put_string(2,STR2,str,0, BLACK);
					mdio_m.vDevAddr = 3;
					
				}
				else
					{
					print ("use specify addr, use Tab\n\r");
					return 1;
				}
					
				print("set addr ");
				print(argv[i]);
				print("\n\r");
			return 0;
			}
		}
		else
		if (strcmp (argv[i], _CMD_R_M_W) == 0) {	//"read_MDIO_word"
			if (++i < argc) {
				val=strtol(argv[i],&end,16);
				//val = atoi(argv[i]);
				
				snprintf (str, 25, "read from 0x%04X",val);
				LCD_put_string(2,STR3,str,0, BLACK);
				mdio_m.vOffset = val;
				
				print("read from 0x");	print(argv[i]);	
				mdio_m.aData[0] = mdiobb_read(mdio_m.vPortAddr,((mdio_m.vDevAddr<<16)|(mdio_m.vOffset)));
//				if(PKT_MDIOM_Read() == PKT_REPLY_Done_ExecNoError){
					snprintf(str, 25, "...ok \t0x%04X\n\r",mdio_m.aData[0]);
					print(str);
//					print("\n\r");
//				}
//				else{
//					print("...err\n\r");
//					}
				return 0;
			}
			else
			{
				print("use specify addr\n\r");
				return 1;
			}
		}
		else
		if (strcmp(argv[i], _CMD_W_M_W) == 0) {		//"write_MDIO_word"
			if (++i < argc) {
				val=strtol(argv[i],&end,16);
				//val = atoi(argv[i]);
				
				snprintf (str, 25, "write 0x%04X",val);
				LCD_put_string(2,STR4,str,0, BLACK);

				//mdiobb_read(mdio_m.vPortAddr,((mdio_m.vDevAddr<<16)|(mdio_m.vOffset)));

				mdio_write_data(mdio_m.vPortAddr,mdio_m.vDevAddr,val);
				print("write 0x"); print(argv[i]); print("\n\r");
				return 0;
			}
			else
			{
				print("use specify addr\n\r");
				return 1;
			}
		}
		else
		if (strcmp (argv[i], _CMD_PR_M_W) == 0) {	//"post_read_MDIO_inc"
			if (i++ < argc) {
				LCD_put_string(2,STR5,"post rd addr",0, BLACK);
			}
			return 0;
		}
		else
		if (strcmp (argv[i], _CMD_PRINT_LCD) == 0) {	//"print_LCD"
			return 0;
		}
		else
		if (strcmp (argv[i], _CMD_R_GPIO) == 0) {	//GPIO read
			pin = mdiobb_get_bit();
			snprintf(str, 25, "0x%1X\n\r",pin);
					print(str);
		}
		else
		if (strcmp(argv[i], _CMD_S_M_REG) == 0) {	//"set_MDIO_address"
			if (++i < argc) {
				val=strtol(argv[i],&end,16);
				//val = atoi(argv[i]);
				mdio_m.vOffset = val;
				mdio_set_addr(mdio_m.vPortAddr,((mdio_m.vDevAddr<<16)|(mdio_m.vOffset)));
				
				snprintf (str, 25, "address 0x%04X",val);
				LCD_put_string(2,STR5,str,0, BLACK);
				
				print("set to 0x"); print(argv[i]); print("\n\r");
				return 0;
			}
			else
			{
				print("use specify addr\n\r");
				return 1;
			}
		}
		else
		if (strcmp(argv[i],_CMD_R_M ) == 0) {	//read from MDIO only DATA not set adress
			
			   mdio_m.aData[0] = mdio_read_data(mdio_m.vPortAddr,mdio_m.vDevAddr);
				if(mdio_m.aData[0] == 0xffff)
					snprintf(str, 25, "...error \t0x%04X\n\r",mdio_m.aData[0]);
				else 
					snprintf(str, 25, "...ok \t0x%04X\n\r",mdio_m.aData[0]);
				
			   print(str);
//			}
//			else
//			{
//				print("use specify addr\n\r");
//				return 1;
//			}
		}
		else
		{
			print ("command: '\033[31m"); print ((char*)argv[i]); print ("\033[0m' Not found.\n\r");
		}
		i++;
	}
	return 0;
}

#ifdef _USE_COMPLETE
//*****************************************************************************
// completion callback for microrl library
char ** complet (uint16_t argc, const char * const * argv)
{
	int j = 0,i=0;

	compl_world [0] = NULL;

	// if there is token in cmdline
	if (argc == 1) {
		// get last entered token
		char * bit = (char*)argv [argc-1];
		// iterate through our available token and match it
		for (i = 0; i < _NUM_OF_CMD; i++) {
			// if token is matched (text is part of our token starting from 0 char)
			if (strstr(keyworld [i], bit) == keyworld [i]) {
				// add it to completion set
				compl_world [j++] = keyworld [i];
			}
		}
	}
	else 
	if ((argc > 1) && ((strcmp (argv[0], _CMD_S_M_DEV)==0))) { // if command needs subcommands
		// iterate through subcommand
		for (i = 0; i < _NUM_OF_SETCLEAR_SCMD; i++) {
			if (strstr (set_clear_key [i], argv [argc-1]) == set_clear_key [i]) {
				compl_world [j++] = set_clear_key [i];
			}
		}
	}
	else 
	if ((argc > 1) && ((strcmp (argv[0], _CMD_R_M_W)==0))) { // if command needs subcommands
		// iterate through subcommand
		compl_world [j++] = _SCMD_R_M_W_1;
	}
	else 
	if ((argc > 1) && ((strcmp (argv[0], _CMD_W_M_W)==0))) { // if command needs subcommands
		// iterate through subcommand
	//	compl_world [j++] = _SCMD_W_M_W_1;
		compl_world [j++] = _SCMD_W_M_W_2;
	}
//	else 
//	if ((argc > 1) && ((strcmp (argv[0], _CMD_R_M_W)==0)||(strcmp (argv[0], _CMD_W_M_W))) { // if command needs subcommands
//		// iterate through subcommand
//		for (i = 0; i < _NUM_OF_SETCLEAR_SCMD; i++) {
//			if (strstr (set_clear_key [i], argv [argc-1]) == set_clear_key [i]) {
//				compl_world [j++] = set_clear_key [i];
//			}
//		}
//	} 
	else { // if there is no token in cmdline, just print all available token
		for (; j < _NUM_OF_CMD; j++) {
			compl_world[j] = keyworld [j];
		}
	}

	// note! last ptr in array always must be NULL!!!
	compl_world [j] = NULL;
	// return set of variants
	return compl_world;
}
#endif


//*****************************************************************************
void sigint (void)
{
	print ("^C catched!\n\r");
}


//*****************************************************************************
void init_u_consol (void)
{
	// call init with ptr to microrl instance and print callback
	microrl_init (prl, print);
	// set callback for execute
	microrl_set_execute_callback (prl, execute);
	// set callback for completion
	microrl_set_complete_callback (prl, complet);
	// set callback for Ctrl+C
	microrl_set_sigint_callback (prl, sigint);
}




