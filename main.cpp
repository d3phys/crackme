/*
 * One more crackme test.
 * The main idea is buffer overflow. Again :) 
 * There are at least two ways using the same idea.
 * 
 * Make sure you compile with the correct options.
 * Check Makefile for more info...
 *
 * Written 2022 by d3phys 
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

const unsigned BUFSIZE  = 0x10;

static void  access();
static void failure();

static uint16_t fletcher16(const char *region, size_t size);

int main()
{
        /*
         * Should wrap locals to place
         * them in the same order after compilation.
         * Hello Camil :)
         */ 
        struct {
                char buf[BUFSIZE] = {0};
                void (*fail)() = failure;
                void (*succ)() = access;
                uint16_t (*hash)(const char *region, size_t size) = fletcher16;
        } locals;
       
        printf("Enter password:\n");
        scanf("%s", locals.buf);
        size_t size = strlen(locals.buf);
        if (locals.hash(locals.buf, size) * size != 0x3639 * size) {
                locals.fail();
                return EXIT_FAILURE;
        }        

        locals.succ();        
        return 0;
}

/*
 * Success/denied function handlers.
 */
static void  access() { printf("Success\n");       }
static void failure() { printf("Access denied\n"); }

/* 
 * Fletcher's checksum is used.
 * <https://en.wikipedia.org/wiki/Fletcher%27s_checksum>
 */
static uint16_t fletcher16(const char *region, size_t size)
{
        uint16_t sum1 = 0;
        uint16_t sum2 = 0;
        
        for (size_t i = 0; i < size; ++i) {
                sum1 = (sum1 + region[i]) % 255;
                sum2 = (sum2 + sum1)      % 255;
        }

        return (sum2 << 8) | sum1;
}
