#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>

#include "patch.h"

static off_t get_size(const char *const file);
static char* patch_mmap(const char *file, off_t *const len);

static char* patch_mmap(const char *file, off_t *const len)
{
        assert(file);
        assert(len);
        
        int fd = open(file, O_RDWR); 
        if (fd == -1) {
                perror("Can't open file"); 
                return nullptr;
        }

        *len = get_size(file);
        char *buf = (char *)mmap(nullptr, *len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (buf == MAP_FAILED) {
                perror("Can't map file");
                return nullptr;
        }
        
        if (fd > 0)
                close(fd);
                
        return buf;
}

static off_t get_size(const char *const file) 
{
        assert(file);

        struct stat buf = {};
        int err = stat(file, &buf);
        if (err)
                return -1;

        return buf.st_size;
}

int patch(const char *file)
{
        assert(file);
        
        off_t len = 0;
        char *binary = patch_mmap(file, &len);
        if (!binary)
                return -1;

        /* mov rsi, rax */
        const unsigned char mov[3] = {0x48, 0x89, 0xce}; 

        /* Start of the mov command */
        ptrdiff_t cmd_addr = 0x1246;
        memcpy(binary + cmd_addr, mov, 3);

        munmap(binary, len);
        return 0;
}

int get_patch_info(const char *file)
{
        off_t len = 0;
        char *binary = patch_mmap(file, &len);
        if (!binary) {
                munmap(binary, len); 
                return INFO_FAIL;
        }

        /* ELF magic number 0x7F 'E' 'L' 'F' */
        unsigned elf_magic = 0x464c457f;
        if (*(unsigned *)binary != elf_magic) {
                munmap(binary, len); 
                return INFO_UNSUPPORT;
        }

        /* mov rsi, rax */
        const unsigned char mov[3] = {0x48, 0x89, 0xce};

        /* Start of the mov command */
        ptrdiff_t cmd_addr = 0x1246;
        if (!memcmp(binary + cmd_addr, mov, 3)) {
                munmap(binary, len);
                return INFO_PATCHED;
        }
                
        munmap(binary, len);
        return 0;
}
