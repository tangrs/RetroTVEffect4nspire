/*
    Retro TV effect for NSpire
    Copyright (C) 2012  Daniel Tang

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <os.h>

#define SCREEN_BYTES_PERPIXEL (SCREEN_BYTES_SIZE/(SCREEN_WIDTH*SCREEN_HEIGHT))
#define HOOK_ADDR 0x100b97b4 /* OS 3.1 TI-NSpire CX. nts: needs better hooking place */

HOOK_DEFINE(screen_blip) {
    int halfheight = SCREEN_HEIGHT/2, halfwidth = SCREEN_WIDTH/2;
    char * scrbase = SCREEN_BASE_ADDRESS;
    void * scr = malloc(SCREEN_BYTES_SIZE);
    if (scr) {
        memcpy(scr, SCREEN_BASE_ADDRESS, SCREEN_BYTES_SIZE);
    }

    memset(scrbase, 0xff, SCREEN_BYTES_SIZE);
    while (halfheight > 1) {
        size_t blackamount, whiteamount;
        char * scrptr = scrbase;

        blackamount = SCREEN_WIDTH*SCREEN_BYTES_PERPIXEL;
        blackamount *= (SCREEN_HEIGHT/2) - halfheight;

        whiteamount = SCREEN_WIDTH*SCREEN_BYTES_PERPIXEL;
        whiteamount *= halfheight;

        memset(scrptr, 0x00, blackamount);
        scrptr += blackamount;
        //memset(scrptr, 0xff, whiteamount*2);
        scrptr += whiteamount*2;
        memset(scrptr, 0x00, blackamount);

        halfheight -= 4;

        sleep(1);
    }

    memset(scrbase, 0x00, SCREEN_BYTES_SIZE);

    scrbase = SCREEN_BASE_ADDRESS + (SCREEN_WIDTH*SCREEN_BYTES_PERPIXEL*((SCREEN_HEIGHT/2)-1));
    sleep(10);

    while (halfwidth > 1) {
        size_t blackamount, whiteamount;
        char * scrptr = scrbase;

        whiteamount = halfwidth*2*SCREEN_BYTES_PERPIXEL;

        blackamount = (SCREEN_WIDTH*SCREEN_BYTES_PERPIXEL) - whiteamount;
        blackamount /= 2;

        memset(scrptr, 0x00, blackamount);
        scrptr += blackamount;
        memset(scrptr, 0xff, whiteamount);
        scrptr += whiteamount;
        memset(scrptr, 0x00, blackamount);
        scrptr += blackamount;

        memset(scrptr, 0x00, blackamount);
        scrptr += blackamount;
        memset(scrptr, 0xff, whiteamount);
        scrptr += whiteamount;
        memset(scrptr, 0x00, blackamount);
        scrptr += blackamount;

        halfwidth -= 4;
        sleep(1);
    }
    if (scr) {
        memcpy(SCREEN_BASE_ADDRESS, scr, SCREEN_BYTES_SIZE);
        free(scr);
    }

    HOOK_RESTORE_RETURN(screen_blip);
}

int is_already_hooked(unsigned* ptr) {
    return (memcmp(ptr,"\x00\x70\x86\xe5",4) != 0);
}

int main(int argc, char* argv[]) {
    if (argc == 2 && strncmp("hook",argv[1],4) == 0) {
        HOOK_INSTALL(HOOK_ADDR, screen_blip);
        return 0;
    }

    if (is_already_hooked((unsigned*)HOOK_ADDR)) {
        show_msgbox("Screen blip","Hook was already installed.");
        return 0;
    }

    struct stat stats;
    stat(argv[0], &stats);

    void* ptr = malloc(stats.st_size);
    if (!ptr) {
        char buffer[256];
        sprintf(buffer, "Failed to malloc %d bytes!", stats.st_size);
        show_msgbox("Error",buffer);
        return -1;
    }

    FILE* fp = fopen(argv[0], "rb");
    if (!fp) {
        free(ptr);
        show_msgbox("Error","Failed to fopen!");
        return -1;
    }
    fread(ptr, 1, stats.st_size, fp);
    fclose(fp);

    clear_cache();
    ((void (*)(int,char*[])) ptr)(2, (char*[]){ argv[0], "hook", NULL });
    show_msgbox("Screen blip","Hook installed successfully");


    return 0;
}