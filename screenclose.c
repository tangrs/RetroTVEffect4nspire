#include <os.h>

#define SCREEN_BYTES_PERPIXEL (SCREEN_BYTES_SIZE/(SCREEN_WIDTH*SCREEN_HEIGHT))

int main() {
    int halfheight = SCREEN_HEIGHT/2, accel = 16, halfwidth = SCREEN_WIDTH/2;
    char * scrbase = SCREEN_BASE_ADDRESS;

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

        halfheight -= accel;
        accel = (accel/2 > 3) ? accel/2 : 3;

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

        halfwidth -= 2;
        sleep(1);
    }

    while (!isKeyPressed(KEY_NSPIRE_5)) idle();

    return 0;
}