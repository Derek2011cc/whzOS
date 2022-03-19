
#include "bootpack.h"
#include <stdio.h>


extern struct KEYBUF keybuf;


void HariMain(void)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	char s[40], mcursor[256];
	int mx, my, i;

	//void init_screen(char *vram, int x, int y)
	init_palette();
	//init_screen(binfo->vram, binfo->scrnx, binfo->scrny);
    boxfill8(binfo->vram,binfo->scrnx,COL8_008484, 0,0,binfo->scrnx- 1, binfo->scrny - 1);
    putfonts8_asc(binfo->vram, binfo->scrnx,  120,  15,13, "hello world");
    putfonts8_asc(binfo->vram, binfo->scrnx, 131, 36, COL8_000000, "whz OS");
    putfonts8_asc(binfo->vram, binfo->scrnx, 130, 35, COL8_FFFFFF, "whz OS");
    putfonts8_asc(binfo->vram, binfo->scrnx, 120, 60, COL8_FFFFFF, "loading....");

    for (i=0;i<25000000;i++) if (i%100000==0) putfonts8_asc(binfo->vram, binfo->scrnx, i/100000+30, 135, COL8_000000, "|");
    putfonts8_asc(binfo->vram, binfo->scrnx, 295, 135, COL8_FFFFFF, "OK");
    for (i=0;i<250000000;i++);
    putfonts8_asc(binfo->vram, binfo->scrnx, 120, 60,14, "loading....");
    boxfill8(binfo->vram,binfo->scrnx,COL8_008484, 0,0,binfo->scrnx- 1, binfo->scrny - 1);
    putfonts8_asc(binfo->vram, binfo->scrnx, 131, 60, COL8_000000, "whz OS");
    putfonts8_asc(binfo->vram, binfo->scrnx, 130, 60, COL8_FFFFFF, "whz OS");
    int z=0;
    for (i=0;i<250000000;i++);
    boxfill8(binfo->vram,binfo->scrnx,COL8_008484, 0,0,binfo->scrnx- 1, binfo->scrny - 1);
    putfonts8_asc(binfo->vram, binfo->scrnx, 131, 10, COL8_000000, "whz OS");
    putfonts8_asc(binfo->vram, binfo->scrnx, 130, 10, COL8_FFFFFF, "whz OS");
    putfonts8_asc(binfo->vram, binfo->scrnx, 126, 25, 6, "Desktop");
    putfonts8_asc(binfo->vram, binfo->scrnx, 30, 40, COL8_FFFFFF, "1.file");
    putfonts8_asc(binfo->vram, binfo->scrnx, 30, 55, COL8_FFFFFF, "2.app");
    putfonts8_asc(binfo->vram, binfo->scrnx, 30, 70, COL8_FFFFFF, "3.off");
	
	init_gdtidt();
	init_pic();
	io_sti();

	io_out8(PIC0_IMR, 0xf9);
	io_out8(PIC1_IMR, 0xef);

	init_palette();

	mx = (binfo->scrnx - 16) / 2;
	my = (binfo->scrny - 28 - 16) / 2;
	init_mouse_cursor8(mcursor, COL8_008484);
	putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);
	sprintf(s, "(%d, %d)", mx, my);
	//putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s);
	int app_1=0;
    while (1) {
		io_cli();
		if (keybuf.flag == 0) {
			io_stihlt();
		} else {
			i = keybuf.data;
			keybuf.flag = 0;
			io_sti();
			sprintf(s, "%02X", i);
			
					
			if (s[0]=='0' && s[1]=='3')
			{
                #include "app/app_1.h"
			}
			if (s[0]=='0' && s[1]=='4')
            {
                #include "app/off.h"
            }
		
			if (s[1]=='1' && s[0]=='0')
            {
                #include "app/Desktop.h"
			}
		}
	}
}
