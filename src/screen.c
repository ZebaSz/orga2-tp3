/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"

void print(const char * text, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;
    int i;
    for (i = 0; text[i] != 0; i++) {
        p[y][x].c = (unsigned char) text[i];
        p[y][x].a = (unsigned char) attr;
        x++;
        if (x == VIDEO_COLS) {
            x = 0;
            y++;
        }
    }
}

void print_hex(unsigned int numero, int size, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia
    int i;
    char hexa[8];
    char letras[16] = "0123456789ABCDEF";
    hexa[0] = letras[ ( numero & 0x0000000F ) >> 0  ];
    hexa[1] = letras[ ( numero & 0x000000F0 ) >> 4  ];
    hexa[2] = letras[ ( numero & 0x00000F00 ) >> 8  ];
    hexa[3] = letras[ ( numero & 0x0000F000 ) >> 12 ];
    hexa[4] = letras[ ( numero & 0x000F0000 ) >> 16 ];
    hexa[5] = letras[ ( numero & 0x00F00000 ) >> 20 ];
    hexa[6] = letras[ ( numero & 0x0F000000 ) >> 24 ];
    hexa[7] = letras[ ( numero & 0xF0000000 ) >> 28 ];
    for(i = 0; i < size; i++) {
        p[y][x + size - i - 1].c = hexa[i];
        p[y][x + size - i - 1].a = attr;
    }
}

void print_int(unsigned int n, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia
    if( n > 9 ) {
      int a = n / 10;
      n -= 10 * a;
      print_int(a,x-1,y,attr);
    }
    p[y][x].c = '0'+n;
    p[y][x].a = attr;
}

void print_mapa() {
    // TODO: esto hay que hacerlo en asm con segmento fs 
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia
    for(int i = 0; i < VIDEO_COLS; i++) {
        p[0][i].a = C_BG_BLACK;
        p[0][i].c = 0;
    }
    for(int i = 1; i < VIDEO_COLS - 1; i++) {
        for(int j = 1; j <= SIZE_H; j++) {
            p[j][i].a = C_BG_GREEN;
            p[j][i].c = 0;
        }
    }

    for(int i = 1; i < VIDEO_COLS - 1; i++) {
        for(int j = SIZE_H + 1; j <= VIDEO_FILS; j++) {
            int attr;
            if (i >= 35 && i < 40) {
                attr = C_BG_RED;
            } else if (i >= 40 && i < 45) {
                attr = C_BG_BLUE;
            } else {
                attr = C_BG_BLACK;
            }
            p[j][i].a = attr;
            p[j][i].c = 0;
        }
    }

    for(int j = 1; j <= SIZE_H; j++) {
        p[j][0].a = C_BG_RED;
        p[j][0].c = 0;

        p[j][VIDEO_COLS - 1].a = C_BG_BLUE;
        p[j][VIDEO_COLS - 1].c = 0;
    }
}

void print_nombre_grupo() {
    int GRUPO_LEN = 23;
    char* GRUPO = "InsiSto gEnio zen acaba";
    print(GRUPO, (VIDEO_COLS - GRUPO_LEN), 0, C_FG_WHITE | C_BG_BLACK);
}
