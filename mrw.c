#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char *argv[])
{
	if(argc!=3 && argc!=4)
	{
		fprintf(stderr, "Usage: mrw b|w|d <address> [value]\n");
		return 1;
	}

	unsigned long addr = strtoul(argv[2], 0, 0);
	unsigned long base_addr = addr & 0xFFFFF000; /* round to 4k boundary */
	unsigned long offset = addr-base_addr;
	unsigned long value;
    if(argc==4)
        value = strtoul(argv[3], 0, 0);

	int fd = open("/dev/mem", O_RDWR);
	if(-1==fd)
	{
		fprintf(stderr, "Open /dev/mem failed!\n");
		return 1;
	}

	volatile unsigned char *p = mmap(NULL, 0x1000, PROT_WRITE|PROT_READ, MAP_SHARED, fd, base_addr);
	if(MAP_FAILED == p)
	{
		fprintf(stderr, "Creading file mapping failed!\n");
		return 1;
	}

	//printf("addr: %08X  base: %08X  offset: %08X\n", addr, base_addr, offset);

	char t = argv[1][0];
	switch(t)
	{
	case 'b':
		if(argc == 3)
            printf("0x%02X\n", *(p+offset));
        else
            *(p+offset) = value;
		break;
	case 'w':
		if(argc == 3)
            printf("0x%04X\n", *(u_int16_t*)(p+offset));
        else
            *(u_int16_t*)(p+offset) = value;
		break;
	case 'd':
		if(argc == 3)
            printf("0x%08X\n", *(u_int32_t*)(p+offset));
        else
            *(u_int32_t*)(p+offset) = value;
		break;
	default:
		fprintf(stderr, "Wrong access method!\n");
		return 1;
	}

	return 0;
}
