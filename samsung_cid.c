#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "mmc.h"
#include <sys/reboot.h>


#define CID_SIZE 16
#define PROGRAM_CID_OPCODE 26
#define SAMSUNG_VENDOR_OPCODE 62

#define CID1 "/sys/devices/msm_sdcc.1/mmc_host/mmc0/mmc0:0001/cid"
#define CID2 "/sys/devices/platform/msm_sdcc.1/mmc_host/mmc0/mmc0:0001/cid"
#define CID3 "/sys/class/mmc_host/mmc0/mmc0:0001/cid"

int mmc_movi_vendor_cmd(unsigned int arg, int fd) {
	int ret = 0;
	struct mmc_ioc_cmd idata = {0};

	idata.data_timeout_ns = 0x10000000;
	idata.write_flag = 1;
	idata.opcode = SAMSUNG_VENDOR_OPCODE;
	idata.arg = arg;
	idata.flags = MMC_RSP_R1B | MMC_CMD_AC;

	ret = ioctl(fd, MMC_IOC_CMD, &idata);

	return ret;
}


int cid_backdoor(int fd) {
	int ret;

	mmc_movi_vendor_cmd(0xEFAC62EC, fd);//enter vendor
	mmc_movi_vendor_cmd(0xEF50, fd);//cid backdoor set
	ret = mmc_movi_vendor_cmd(0x00DECCEE, fd);//exit vendor

	return ret;
}


int get_cid(char *cid) {
	/* this is hacky way to read CID from sysfs, ioctl doesn't seem to want to cooperate
	if your cid isn't in one of the predefined paths, update the code*/
	int fd, i;
	char buf[32] = {0};
	fd = open(CID1, O_RDONLY);
	if(fd < 0){
		fd = open(CID2, O_RDONLY);
		if(fd < 0) {
			fd = open(CID3, O_RDONLY);
			if(fd < 1) return -1;
		}
	}
	if(read(fd, buf, CID_SIZE*2) != CID_SIZE*2) return -1;
	for(i = 0; i < CID_SIZE; i++){
		sscanf(&buf[i*2], "%2hhx", &cid[i]);
	}

	return 0;
}



int program_cid(int fd, char *cid) {
	int ret;
	struct mmc_ioc_cmd idata = {0};

	idata.data_timeout_ns = 0x10000000;
	idata.write_flag = 1;
	idata.opcode = PROGRAM_CID_OPCODE;
	idata.arg = 0;
	idata.flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_ADTC;
	idata.blksz = CID_SIZE;
	idata.blocks = 1;
	idata.data_ptr = (__u64)(unsigned int)cid;

	ret = ioctl(fd, MMC_IOC_CMD, &idata);

	return ret;
}


int is_samsung_emmc(char *cid) {
	return (cid[0] == 0x15) ? 1 : 0;
}

void show_cid(char *cid) {
	int i;

	printf("[+] CID at boot time is/was: ");
	for(i = 0; i < CID_SIZE; i++){
		printf("%02x", cid[i]);
	}
	printf("\n");
}


void main(int argc, const char **argv) {
	int fd, ret, i;
	char cid[CID_SIZE] = {0};
	char old_cid[CID_SIZE] = {0};

	if(get_cid(old_cid) < 0) {
		printf("[-] can't get current CID, find it and fix the code\n");
		return;
	}

	show_cid(old_cid);

	if(!is_samsung_emmc(old_cid)) {
		printf("[-] don't try this on non-samsung eMMC, seriously.\n");
		return;
	}

	if(argc != 2 || strlen(argv[1]) != CID_SIZE*2) {
		printf("[-] RTFC!! ./samsung_cid [NEW_CID]\n");
		return;
	}

	for(i = 0; i < CID_SIZE; i++){
		ret = sscanf(&argv[1][i*2], "%2hhx", &cid[i]);
		if(!ret){
			printf("[-] CID should be HEX!\n");
			return;
		}
	}
	printf("[+] programming new CID %s\n", argv[1]);

	fd = open("/dev/block/mmcblk0", O_RDWR);
	if(fd < 0){
		printf("[-] wtf\n");
		return;
	}

	ret = cid_backdoor(fd);
	if(ret){
		printf("[-] cid_backdoor failed %d\n", ret);
		return;
	}

	ret = program_cid(fd, cid);
	if(ret){
		printf("[-] program_cid failed %d\n", ret);
	}

	printf("[+] success! powering off device, power back on and verify CID\n");
	reboot(LINUX_REBOOT_CMD_POWER_OFF);
}
	
	



