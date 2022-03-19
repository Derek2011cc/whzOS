// 桌面


boxfill8(binfo->vram,binfo->scrnx,COL8_008484, 0,0,binfo->scrnx- 1, binfo->scrny - 1);
putfonts8_asc(binfo->vram, binfo->scrnx, 131, 10, COL8_000000, "whz OS");
putfonts8_asc(binfo->vram, binfo->scrnx, 130, 10, COL8_FFFFFF, "whz OS");
putfonts8_asc(binfo->vram, binfo->scrnx, 126, 25, 6, "Desktop");
putfonts8_asc(binfo->vram, binfo->scrnx, 30, 40, COL8_FFFFFF, "1.file");
putfonts8_asc(binfo->vram, binfo->scrnx, 30, 55, COL8_FFFFFF, "2.app");
putfonts8_asc(binfo->vram, binfo->scrnx, 30, 70, COL8_FFFFFF, "3.off");
