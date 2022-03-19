/* bootpack�̃��C�� */

#include "bootpack.h"
#include <stdio.h>

struct MOUSE_DEC {
	unsigned char buf[3], phase;
	int x, y, btn;
};

extern struct FIFO8 keyfifo, mousefifo;
void enable_mouse(struct MOUSE_DEC *mdec);
void init_keyboard(void);
int mouse_decode(struct MOUSE_DEC *mdec, unsigned char dat);

void HariMain(void)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	char s[40], mcursor[256], keybuf[32], mousebuf[128];
	int mx, my, i;
	struct MOUSE_DEC mdec;
    int youjian=0,youjianx=0,youjiany =0,youjianx1=0,youjiany1=0,youjianweiz=0;

	init_gdtidt();
	init_pic();
	io_sti(); /* IDT/PIC�̏��������I������̂�CPU�̊��荞�݋֎~������ */
	fifo8_init(&keyfifo, 32, keybuf);
	fifo8_init(&mousefifo, 128, mousebuf);
	io_out8(PIC0_IMR, 0xf9); /* PIC1�ƃL�[�{�[�h������(11111001) */
	io_out8(PIC1_IMR, 0xef); /* �}�E�X������(11101111) */

	init_keyboard();

	init_palette();
	init_screen8(binfo->vram, binfo->scrnx, binfo->scrny);
	mx = (binfo->scrnx - 16) / 2; /* ��ʒ����ɂȂ�悤�ɍ��W�v�Z */
	my = (binfo->scrny - 28 - 16) / 2;
	init_mouse_cursor8(mcursor, COL8_008484);
	putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);
	sprintf(s, "(%3d, %3d)", mx, my);
	putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s);

	enable_mouse(&mdec);

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
    //PUTCHINESE(110, 60, 0,'我');
    for (i=0;i<250000000;i++);
    boxfill8(binfo->vram,binfo->scrnx,COL8_008484, 0,0,binfo->scrnx- 1, binfo->scrny - 1);
    putfonts8_asc(binfo->vram, binfo->scrnx, 131, 10, COL8_000000, "whz OS");
    putfonts8_asc(binfo->vram, binfo->scrnx, 130, 10, COL8_FFFFFF, "whz OS");
    putfonts8_asc(binfo->vram, binfo->scrnx, 126, 25, 6, "Desktop");
    putfonts8_asc(binfo->vram, binfo->scrnx, 30, 40, COL8_FFFFFF, "1.file");
    putfonts8_asc(binfo->vram, binfo->scrnx, 30, 55, COL8_FFFFFF, "2.shell");
    putfonts8_asc(binfo->vram, binfo->scrnx, 30, 70, COL8_FFFFFF, "3.off");

	for (;;) {
		io_cli();
		if (fifo8_status(&keyfifo) + fifo8_status(&mousefifo) == 0) {
			io_stihlt();
		} else {

			if (fifo8_status(&keyfifo) != 0) {
				i = fifo8_get(&keyfifo);
				io_sti();
				sprintf(s, "%02X", i);
				boxfill8(binfo->vram, binfo->scrnx, COL8_008484,  0, 16, 15, 31);
				putfonts8_asc(binfo->vram, binfo->scrnx, 0, 16, COL8_FFFFFF, s);
			} else if (fifo8_status(&mousefifo) != 0) {
#include "app/Desktop.h"
				i = fifo8_get(&mousefifo);
				io_sti();
				if (mouse_decode(&mdec, i) != 0) {
					/* �f�[�^��3�o�C�g�������̂ŕ\�� */

					sprintf(s, "[lcr %4d %4d]", mdec.x, mdec.y);
					if ((mdec.btn & 0x01) != 0) {
						s[1] = 'L';
					}
					if ((mdec.btn & 0x02) != 0) {
						s[3] = 'R';
                        youjian=1;
                        youjianx=mx;
                        youjiany=my;
                        youjianx1=mx+100;
                        youjiany1=my+100;
                        if (mx>220)
                        {
                            youjianx1=youjianx;
                            youjianx=youjianx-100;

                        }
                        if (my>100)
                        {
                            youjiany1=youjiany;
                            youjiany=youjiany-100;
                        }
                        else youjianweiz=0;
                        boxfill8(binfo->vram,binfo->scrnx,15, youjianx,youjiany, youjianx1,youjiany1);

					}
                    if (youjian==1) boxfill8(binfo->vram,binfo->scrnx,15, youjianx,youjiany, youjianx1,youjiany1);
					if ((mdec.btn & 0x04) != 0) {
						s[2] = 'C';
					}
					boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 32, 16, 32 + 15 * 8 - 1, 31);
					putfonts8_asc(binfo->vram, binfo->scrnx, 32, 16, COL8_FFFFFF, s);
					/* �}�E�X�J�[�\���̈ړ� */
					boxfill8(binfo->vram, binfo->scrnx, COL8_008484, mx, my, mx + 15, my + 15); /* �}�E�X���� */
					mx += mdec.x;
					my += mdec.y;
					if (mx < 0) {
						mx = 0;
					}
					if (my < 0) {
						my = 0;
					}
					if (mx > binfo->scrnx - 16) {
						mx = binfo->scrnx - 16;
					}
					if (my > binfo->scrny - 16) {
						my = binfo->scrny - 16;
					}
					sprintf(s, "(%3d, %3d)", mx, my);
					boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 0, 79, 15); /* ���W���� */
					putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s); /* ���W���� */
					putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16); /* �}�E�X�`�� */
				}
			}
		}
	}
}

#define PORT_KEYDAT				0x0060
#define PORT_KEYSTA				0x0064
#define PORT_KEYCMD				0x0064
#define KEYSTA_SEND_NOTREADY	0x02
#define KEYCMD_WRITE_MODE		0x60
#define KBC_MODE				0x47

void wait_KBC_sendready(void)
{
	/* �L�[�{�[�h�R���g���[�����f�[�^���M�\�ɂȂ�̂�҂� */
	for (;;) {
		if ((io_in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0) {
			break;
		}
	}
	return;
}

void init_keyboard(void)
{
	/* �L�[�{�[�h�R���g���[���̏����� */
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT, KBC_MODE);
	return;
}

#define KEYCMD_SENDTO_MOUSE		0xd4
#define MOUSECMD_ENABLE			0xf4

void enable_mouse(struct MOUSE_DEC *mdec)
{
	/* �}�E�X�L�� */
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);
	/* ���܂�������ACK(0xfa)�����M����Ă��� */
	mdec->phase = 0; /* �}�E�X��0xfa��҂��Ă���i�K */
	return;
}

int mouse_decode(struct MOUSE_DEC *mdec, unsigned char dat)
{
	if (mdec->phase == 0) {
		/* �}�E�X��0xfa��҂��Ă���i�K */
		if (dat == 0xfa) {
			mdec->phase = 1;
		}
		return 0;
	}
	if (mdec->phase == 1) {
		/* �}�E�X��1�o�C�g�ڂ�҂��Ă���i�K */
		if ((dat & 0xc8) == 0x08) {
			/* ������1�o�C�g�ڂ����� */
			mdec->buf[0] = dat;
			mdec->phase = 2;
		}
		return 0;
	}
	if (mdec->phase == 2) {
		/* �}�E�X��2�o�C�g�ڂ�҂��Ă���i�K */
		mdec->buf[1] = dat;
		mdec->phase = 3;
		return 0;
	}
	if (mdec->phase == 3) {
		/* �}�E�X��3�o�C�g�ڂ�҂��Ă���i�K */
		mdec->buf[2] = dat;
		mdec->phase = 1;
		mdec->btn = mdec->buf[0] & 0x07;
		mdec->x = mdec->buf[1];
		mdec->y = mdec->buf[2];
		if ((mdec->buf[0] & 0x10) != 0) {
			mdec->x |= 0xffffff00;
		}
		if ((mdec->buf[0] & 0x20) != 0) {
			mdec->y |= 0xffffff00;
		}
		mdec->y = - mdec->y; /* �}�E�X�ł�y�����̕�������ʂƔ��� */
		return 1;
	}
	return -1; /* �����ɗ��邱�Ƃ͂Ȃ��͂� */
}
