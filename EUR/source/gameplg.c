#include "global.h"
#include "stdlib.h"

#define WRITEU8(addr, data) *(vu8*)(addr) = data
#define WRITEU16(addr, data) *(vu16*)(addr) = data
#define WRITEU32(addr, data) *(vu32*)(addr) = data
#define READU8(addr) *(vu8*)(addr)
#define READU16(addr) *(vu16*)(addr)
#define READU32(addr) *(vu32*)(addr)

u32 threadStack[0x1000];
Handle fsUserHandle;
FS_archive sdmcArchive;

u32 plgGetIoBase(u32 IoType);
#define IO_BASE_PAD		1
#define IO_BASE_LCD		2
#define IO_BASE_PDC		3
#define IO_BASE_GSPHEAP		4

u32 IoBasePad = 0xFFFD4000;

u32 getKey() {
	return (*(vu32*)(IoBasePad) ^ 0xFFF) & 0xFFF;
}

void waitKeyUp() {
	while (getKey() != 0) {
		svc_sleepThread(100000000);
	}
}

int isNewNtr = 0;

void gamePluginEntry() {
	u32 ret, key;
	INIT_SHARED_FUNC(plgGetIoBase, 8);
	// wait for game starts up (5 seconds)
	svc_sleepThread(5000000000);

	if (((NS_CONFIG*)(NS_CONFIGURE_ADDR))->sharedFunc[8]) {
		isNewNtr = 1;
	} else {
		isNewNtr = 0;
	}
	
	if (isNewNtr) {
		IoBasePad = plgGetIoBase(IO_BASE_PAD);
	}
	while (1) {
		svc_sleepThread(100000000);
		
		key = getKey();
		if (key == BUTTON_R) {
      u16 *id = (u16*)0x16F4C460;
      char id_str[5] = {0};
      for (size_t i = 0; i < 4u; ++i)
      {
        id_str[i] = (char)READU16(id + i);
      }
      
      u16 result = (u16)strtoul(id_str, NULL, 16);
      WRITEU16(0x15FBEDD0, result);
			waitKeyUp();
		}

	}
}