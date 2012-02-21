#include <os.h>

#define SCREEN_BYTES_PERPIXEL (SCREEN_BYTES_SIZE/(SCREEN_WIDTH*SCREEN_HEIGHT))

void busy_sleep(unsigned ms) {
    volatile int i;
    for (i=0; i<0xfffff; i++);
}

/*void debug_dump() {
    volatile unsigned* debug_ptr = (volatile unsigned*)0xC0000000;
    char str[256];
    str[0] = 0;
    for (;debug_ptr<(volatile unsigned*)0xC0000028;debug_ptr++) {
        char buffer[128];
        sprintf(buffer, "%p = %x \n", (void*)debug_ptr, *debug_ptr);
        strcat(str,buffer);
    }

    FILE* fp = fopen("/documents/debug.txt.tns","w");
    fwrite(str, 1, strlen(str), fp);
    fclose(fp);

    *(volatile unsigned*)0xc000000c |= 0xc;
}*/

HOOK_DEFINE(screen_blip) {
    int halfheight = SCREEN_HEIGHT/2, accel = 16, halfwidth = SCREEN_WIDTH/2;
    unsigned savedcontrast = *(volatile unsigned*)0x900f0020;
    //unsigned savedlcd = 2; *(volatile unsigned*)0xc0000018;
    char * scrbase = SCREEN_BASE_ADDRESS;
    int intmask = TCT_Local_Control_Interrupts(-1);

    *(volatile unsigned*)0x900f0020 = 0x80; //Add contrast to the screen
    //*(volatile unsigned*)0xc0000018 |= 0x801;

    //*(volatile unsigned*)0x900B0018 |= ;

    __builtin_memset(scrbase, 0xff, SCREEN_BYTES_SIZE);
    while (halfheight > 1) {
        size_t blackamount, whiteamount;
        char * scrptr = scrbase;

        blackamount = SCREEN_WIDTH*SCREEN_BYTES_PERPIXEL;
        blackamount *= (SCREEN_HEIGHT/2) - halfheight;

        whiteamount = SCREEN_WIDTH*SCREEN_BYTES_PERPIXEL;
        whiteamount *= halfheight;

        __builtin_memset(scrptr, 0x00, blackamount);
        scrptr += blackamount;
        //__builtin_memset(scrptr, 0xff, whiteamount*2);
        scrptr += whiteamount*2;
        __builtin_memset(scrptr, 0x00, blackamount);

        halfheight -= accel;
        accel = (accel/2 > 3) ? accel/2 : 3;

        sleep(1);
    }

    __builtin_memset(scrbase, 0x00, SCREEN_BYTES_SIZE);

    scrbase = SCREEN_BASE_ADDRESS + (SCREEN_WIDTH*SCREEN_BYTES_PERPIXEL*((SCREEN_HEIGHT/2)-1));
    sleep(10);

    while (halfwidth > 1) {
        size_t blackamount, whiteamount;
        char * scrptr = scrbase;

        whiteamount = halfwidth*2*SCREEN_BYTES_PERPIXEL;

        blackamount = (SCREEN_WIDTH*SCREEN_BYTES_PERPIXEL) - whiteamount;
        blackamount /= 2;

        __builtin_memset(scrptr, 0x00, blackamount);
        scrptr += blackamount;
        __builtin_memset(scrptr, 0xff, whiteamount);
        scrptr += whiteamount;
        __builtin_memset(scrptr, 0x00, blackamount);
        scrptr += blackamount;

        __builtin_memset(scrptr, 0x00, blackamount);
        scrptr += blackamount;
        __builtin_memset(scrptr, 0xff, whiteamount);
        scrptr += whiteamount;
        __builtin_memset(scrptr, 0x00, blackamount);
        scrptr += blackamount;

        halfwidth -= 2;
        sleep(1);
    }
    //*(volatile unsigned*)0xc0000018 = savedlcd;
    *(volatile unsigned*)0x900f0020 = savedcontrast;



    TCT_Local_Control_Interrupts(intmask);
    HOOK_RESTORE_RETURN(screen_blip);

}

int main(int argc, char* argv[]) {
    if (argc > 1 && strncmp("hook",argv[1],4) == 0) {
        HOOK_INSTALL(0x100b97b4, screen_blip);
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

    volatile unsigned* debug_ptr = (volatile unsigned*)0xC0000000;
    char str[256];
    str[0] = 0;
    for (;debug_ptr<(volatile unsigned*)0xC0000028;debug_ptr++) {
        char buffer[128];
        sprintf(buffer, "%p = %x \n", (void*)debug_ptr, *debug_ptr);
        strcat(str,buffer);
    }

    show_msgbox("Debugging information!",str);


    return 0;
}