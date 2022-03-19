// 关机用
boxfill8(binfo->vram,binfo->scrnx,COL8_008484, 0,0,binfo->scrnx- 1, binfo->scrny - 1);
putfonts8_asc(binfo->vram, binfo->scrnx, 85, 60, COL8_000000, "Shutdown in progress");
for (i=0;i<250000000;i++);
break;