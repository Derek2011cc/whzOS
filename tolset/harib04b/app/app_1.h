//显示按键
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
            i = fifo8_get(&mousefifo);
            io_sti();
            if (mouse_decode(&mdec, i) != 0) {
                sprintf(s, "[lcr %4d %4d]", mdec.x, mdec.y);
                if ((mdec.btn & 0x01) != 0) {
                    s[1] = 'L';
                }
                if ((mdec.btn & 0x02) != 0) {
                    s[3] = 'R';
                }
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


