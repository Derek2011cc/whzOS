/* bootpack�̃��C�� */

#include "bootpack.h"
#include <stdio.h>

void make_window8(unsigned char *buf, int xsize, int ysize, char *title);
void putfonts8_asc_sht(struct SHEET *sht, int x, int y, int c, int b, char *s, int l);

void HariMain(void)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	struct FIFO32 fifo;
	char s[40];
	int fifobuf[128];
	struct TIMER *timer, *timer2, *timer3;
	int mx, my, i,jsq=0,count = 0;
	unsigned int memtotal;
	struct MOUSE_DEC mdec;
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	struct SHTCTL *shtctl;
	struct SHEET *sht_back, *sht_mouse, *sht_win;
	unsigned char *buf_back, buf_mouse[256], *buf_win;
    int timeset=0,timetmp=0,timejc[10],timejcjs=0;
    int windosx=320,windosy=200,windosqx=0,windosqy=0,windosopen=0;
	init_gdtidt();
	init_pic();
	io_sti(); /* IDT/PIC�̏��������I������̂�CPU�̊��荞�݋֎~������ */
	fifo32_init(&fifo, 128, fifobuf);
	init_pit();
	init_keyboard(&fifo, 256);
	enable_mouse(&fifo, 512, &mdec);
	io_out8(PIC0_IMR, 0xf8); /* PIT��PIC1�ƃL�[�{�[�h������(11111000) */
	io_out8(PIC1_IMR, 0xef); /* �}�E�X������(11101111) */

	timer = timer_alloc();
	timer_init(timer, &fifo, 10);
	timer_settime(timer, 1000);
	timer2 = timer_alloc();
	timer_init(timer2, &fifo, 3);
	timer_settime(timer2, 300);
	timer3 = timer_alloc();
	timer_init(timer3, &fifo, 1);
	timer_settime(timer3, 50);

	memtotal = memtest(0x00400000, 0xbfffffff);
	memman_init(memman);
	memman_free(memman, 0x00001000, 0x0009e000); /* 0x00001000 - 0x0009efff */
	memman_free(memman, 0x00400000, memtotal - 0x00400000);

	init_palette();
	shtctl = shtctl_init(memman, binfo->vram, binfo->scrnx, binfo->scrny);
	sht_back  = sheet_alloc(shtctl);
	sht_mouse = sheet_alloc(shtctl);
	sht_win   = sheet_alloc(shtctl);
	buf_back  = (unsigned char *) memman_alloc_4k(memman, binfo->scrnx * binfo->scrny);
	buf_win   = (unsigned char *) memman_alloc_4k(memman, windosx * windosy);
	sheet_setbuf(sht_back, buf_back, binfo->scrnx, binfo->scrny, -1); /* �����F�Ȃ� */
	sheet_setbuf(sht_mouse, buf_mouse, 16, 16, 99);
    sheet_setbuf(sht_win, buf_win, windosx, windosy, -1);
	/* �����F�Ȃ� */
	init_screen8(buf_back, binfo->scrnx, binfo->scrny);
	init_mouse_cursor8(buf_mouse, 99);
	mx = (binfo->scrnx - 16) / 2; /* ��ʒ����ɂȂ�悤�ɍ��W�v�Z */
	my = (binfo->scrny - 28 - 16) / 2;
    make_window8(buf_win, windosx, windosy, "counter");
	sheet_slide(sht_mouse, mx, my);
    sheet_slide(sht_back, 0, 0);
    sheet_slide(sht_win, windosqx, windosqy);
	sheet_updown(sht_back,  1);
    sheet_updown(sht_win,0);
	sheet_updown(sht_mouse, 3);
	//sprintf(s, "(%3d, %3d)", mx, my);
	putfonts8_asc_sht(sht_back, 0, 0, COL8_FFFFFF, COL8_008484, s, 10);
	sprintf(s, "memory %dMB   free : %dKB",
			memtotal / (1024 * 1024), memman_total(memman) / 1024);
	putfonts8_asc_sht(sht_back, 0, 32, COL8_FFFFFF, COL8_008484, s, 40);

    boxfill8(buf_win, binfo->scrnx, 3, 257, 26, 303, 43);
    putfonts8_asc(buf_win,binfo->scrnx,260,28,0,"clean");
    boxfill8(buf_win, binfo->scrnx, 3, 257, 48, 303, 65);
    putfonts8_asc(buf_win,binfo->scrnx,260,50,0,"pause");
    sheet_updown(sht_win,2);
    sheet_updown(sht_mouse, 2);
	for (;;) {
		count++;
        if (timeset==1)
        {
            timerctl.count=timetmp;
        }
        sprintf(s, "%010d", timerctl.count);
        boxfill8(buf_win, binfo->scrnx, COL8_C6C6C6, 40, 28, 119, 43);
        putfonts8_asc(buf_win, binfo->scrnx, 40, 28, COL8_000000, s);
        putfonts8_asc_sht(sht_back, 32, 16, COL8_FFFFFF, COL8_008484, s, 15);
        sheet_refresh(sht_win, 40, 28, 120, 44);
		io_cli();
		if (fifo32_status(&fifo) == 0) {
			io_sti();
		} else {
			i = fifo32_get(&fifo);
			io_sti();
			if (256 <= i && i <= 511) { /* �L�[�{�[�h�f�[�^ */
				sprintf(s, "%02X", i - 256);
				putfonts8_asc_sht(sht_back, 0, 16, COL8_FFFFFF, COL8_008484, s, 2);
			} else if (512 <= i && i <= 767) { /* �}�E�X�f�[�^ */
				if (mouse_decode(&mdec, i - 512) != 0) {
					/* �f�[�^��3�o�C�g�������̂ŕ\�� */
					sprintf(s, "[lcr %4d %4d]", mdec.x, mdec.y);
					if ((mdec.btn & 0x01) != 0) {
						s[1] = 'L';

                        if (windosopen==1) {
                            //后台运行
                            /*if (mx > 298 && mx < 315 && my > 5 && my < 18) {
                                windosopen = 0;
                                sheet_updown(sht_win, 0);
                            }*/

                            //重置时间
                            if (mx > 257 && mx < 303 && my > 28 && my < 43) {
                                timerctl.count = 0;
                                timetmp=0;
                            }
/*
                            //时间计次 （未完成）
                            if (mx > 257 && mx < 303 && my > 70 && my < 87)
                            {
                                if (timejcjs<=9)
                                {
                                    timejcjs=0;
                                }
                                timejc[timejcjs]=timerctl.count;
                                timejcjs++;
                            }*/

                                //暂停时间
                            if (mx > 257 && mx < 303 && my > 48 && my < 65) {
                                if (timeset==1)
                                {
                                    boxfill8(buf_win, binfo->scrnx, COL8_C6C6C6, 240, 48, 315, 65);
                                    boxfill8(buf_win, binfo->scrnx, 3, 257, 48, 303, 65);
                                    putfonts8_asc(buf_win, binfo->scrnx, 260, 50, 0, "pause");
                                    sheet_updown(sht_win, 2);
                                    sheet_updown(sht_mouse, 2);
                                    timeset = 0;
                                }
                                else
                                {
                                    timeset = 1;
                                    timetmp = timerctl.count;
                                    boxfill8(buf_win, binfo->scrnx, COL8_C6C6C6, 240, 48, 315, 65);
                                    boxfill8(buf_win, binfo->scrnx, 3, 247, 48, 313, 65);
                                    putfonts8_asc(buf_win,binfo->scrnx,250,50,0,"continue");
                                    sheet_updown(sht_win, 2);
                                    sheet_updown(sht_mouse, 2);
                                }
                            }
                        }else {
                        windosopen = 1;
                        sheet_updown(sht_win, 2);
                        sheet_updown(sht_mouse, 2);
                        }






					}
					if ((mdec.btn & 0x02) != 0) {
						s[3] = 'R';
					}
					if ((mdec.btn & 0x04) != 0) {
						s[2] = 'C';
					}

                    sprintf(s, "%010d", timerctl.count);
                    boxfill8(buf_win, binfo->scrnx, COL8_C6C6C6, 40, 28, 119, 43);
                    putfonts8_asc(buf_win, binfo->scrnx, 40, 28, COL8_000000, s);
                    sheet_refresh(sht_win, 40, 28, 120, 44);
                    putfonts8_asc_sht(sht_back, 0, 0, COL8_FFFFFF, COL8_008484, s, 10);

					/* �}�E�X�J�[�\���̈ړ� */
					mx += mdec.x;
					my += mdec.y;
					if (mx < 0) {
						mx = 0;
					}
					if (my < 0) {
						my = 0;
					}
					if (mx > binfo->scrnx - 1) {
						mx = binfo->scrnx - 1;
					}
					if (my > binfo->scrny - 1) {
						my = binfo->scrny - 1;
					}
					sprintf(s, "(%3d, %3d)", mx, my);
					putfonts8_asc_sht(sht_back, 0, 0, COL8_FFFFFF, COL8_008484, s, 10);
					sheet_slide(sht_mouse, mx, my);
                    sprintf(s, "(%d)", jsq);
				}
			} else if (i==10) { /* 10�b�^�C�} */
				putfonts8_asc_sht(sht_back, 0, 64, COL8_FFFFFF, COL8_008484, "10[sec]", 7);
				sprintf(s, "%010d", count);
			} else if (i == 3) { /* 3�b�^�C�} */
				putfonts8_asc_sht(sht_back, 0, 80, COL8_FFFFFF, COL8_008484, "3[sec]", 6);
				count = 0; /* ����J�n */
			} else if (i == 1) { /* �J�[�\���p�^�C�} */
				timer_init(timer3, &fifo, 0); /* ����0�� */
				boxfill8(buf_back, binfo->scrnx, COL8_FFFFFF, 8, 96, 15, 111);
				timer_settime(timer3, 50);
				sheet_refresh(sht_back, 8, 96, 16, 112);
			} else if (i == 0) { /* �J�[�\���p�^�C�} */
				timer_init(timer3, &fifo, 1); /* ����1�� */
				boxfill8(buf_back, binfo->scrnx, COL8_008484, 8, 96, 15, 111);
                jsq++;
				timer_settime(timer3, 50);
				sheet_refresh(sht_back, 8, 96, 16, 112);
			}
		}
	}
}

void make_window8(unsigned char *buf, int xsize, int ysize, char *title)
{/*
	static char closebtn[14][16] = {
		"OOOOOOOOOOOOOOO@",
		"OQQQQQQQQQQQQQ$@",
		"OQQQQQQQQQQQQQ$@",
		"OQQQ@@QQQQ@@QQ$@",
		"OQQQQ@@QQ@@QQQ$@",
		"OQQQQQ@@@@QQQQ$@",
		"OQQQQQQ@@QQQQQ$@",
		"OQQQQQ@@@@QQQQ$@",
		"OQQQQ@@QQ@@QQQ$@",
		"OQQQ@@QQQQ@@QQ$@",
		"OQQQQQQQQQQQQQ$@",
		"OQQQQQQQQQQQQQ$@",
		"O$$$$$$$$$$$$$$@",
		"@@@@@@@@@@@@@@@@"
	};*/
    static char closebtn[14][16] = {
            "OOOOOOOOOOOOOOO@",
            "OQQQQQQQQQQQQQ$@",
            "OQQQQQQQQQQQQQ$@",
            "OQQQQQQQQQQQQQ$@",
            "OQQQQQQQQQQQQQ$@",
            "OQQQQQQQQQQQQQ$@",
            "OQQ@@@@@@@@@QQ$@",
            "0QQ@@@@@@@@@QQ$@",
            "0QQQQQQQQQQQQQ$@",
            "OQQQQQQQQQQQQQ$@",
            "OQQQQQQQQQQQQQ$@",
            "OQQQQQQQQQQQQQ$@",
            "O$$$$$$$$$$$$$$@",
            "@@@@@@@@@@@@@@@@"
    };
	int x, y;
	char c;
	boxfill8(buf, xsize, COL8_C6C6C6, 0,         0,         xsize - 1, 0        );
	boxfill8(buf, xsize, COL8_FFFFFF, 1,         1,         xsize - 2, 1        );
	boxfill8(buf, xsize, COL8_C6C6C6, 0,         0,         0,         ysize - 1);
	boxfill8(buf, xsize, COL8_FFFFFF, 1,         1,         1,         ysize - 2);
	boxfill8(buf, xsize, COL8_848484, xsize - 2, 1,         xsize - 2, ysize - 2);
	boxfill8(buf, xsize, COL8_000000, xsize - 1, 0,         xsize - 1, ysize - 1);
	boxfill8(buf, xsize, COL8_C6C6C6, 2,         2,         xsize - 3, ysize - 3);
	boxfill8(buf, xsize, COL8_000084, 3,         3,         xsize - 4, 20       );
	boxfill8(buf, xsize, COL8_848484, 1,         ysize - 2, xsize - 2, ysize - 2);
	boxfill8(buf, xsize, COL8_000000, 0,         ysize - 1, xsize - 1, ysize - 1);
	putfonts8_asc(buf, xsize, 24, 4, COL8_FFFFFF, title);
	for (y = 0; y < 14; y++) {
		for (x = 0; x < 16; x++) {
			c = closebtn[y][x];
			if (c == '@') {
				c = COL8_000084;
			} else if (c == '$') {
				c = COL8_000084;
			} else if (c == 'Q') {
				c = COL8_000084;
			} else {
				c = COL8_000084;
			}
			buf[(5 + y) * xsize + (xsize - 21 + x)] = c;
		}
	}
	return;
}

void putfonts8_asc_sht(struct SHEET *sht, int x, int y, int c, int b, char *s, int l)
{
	boxfill8(sht->buf, sht->bxsize, b, x, y, x + l * 8 - 1, y + 15);
	putfonts8_asc(sht->buf, sht->bxsize, x, y, c, s);
	sheet_refresh(sht, x, y, x + l * 8, y + 16);
	return;
}
