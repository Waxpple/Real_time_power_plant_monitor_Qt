/*
 ============================================================================
 Name        : B10630018_DC.c
 Author      : B10630018
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include <modbus/modbus.h>

#include<windows.h>
int main(void) {
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */

	modbus_t *ctx;

	ctx = modbus_new_rtu("\\\\.\\COM3", 9600, 'N', 8, 1);
	if (ctx == NULL) {
	    fprintf(stderr, "Unable to create the libmodbus context\n");
	    return -1;
	}

	modbus_set_slave(ctx, 1);

	if (modbus_connect(ctx) == -1) {
	    fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
	    modbus_free(ctx);
	    return -1;
	}

	int x;
	x=-3;

	while(x<=150){
			x +=3;
			if(x>150){
			x =150;
			}
			modbus_write_register(ctx, 0x01, x);
			uint16_t tab_reg[64];
			int rc;
			int i;
			rc = modbus_read_registers(ctx, 0x01, 1, tab_reg);
			if (rc == -1) {
			    fprintf(stderr, "%s\n", modbus_strerror(errno));
			    return -1;
			}

			for (i=0; i < rc; i++) {
			    printf("reg[%d]=%d (0x%X)\n", i, tab_reg[i], tab_reg[i]);
			}

			if(x==150){
			break;
			}
			Sleep(1000);
	}


	modbus_close(ctx);
	modbus_free(ctx);

	return EXIT_SUCCESS;
}
