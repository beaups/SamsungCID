#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "mmc.h"
#include <sys/reboot.h>
#include <linux/fs.h>
#include <sys/system_properties.h>


#define CID_SIZE 16
#define PROGRAM_CID_OPCODE 26
#define SAMSUNG_VENDOR_OPCODE 62
#define BLOCKS 0x1000
#define BLKSIZE 0x10000

#define CID1 "/sys/devices/msm_sdcc.1/mmc_host/mmc0/mmc0:0001/cid"
#define CID2 "/sys/devices/platform/msm_sdcc.1/mmc_host/mmc0/mmc0:0001/cid"
#define CID3 "/sys/class/mmc_host/mmc0/mmc0:0001/cid"
#define ABOOTNAME "/dev/block/platform/msm_sdcc.1/by-name/aboot"

char dev_cid[] = {0x15, 0x01, 0x00, 0x4d, 0x41, 0x47, 0x32, 0x47, 0x43, 0x00, 0xbd, 0x6d, 0x97, 0x16, 0x51, 0x00};
char dev_sig[] =   {0x76, 0x60, 0x8e, 0x21, 0xeb, 0x4f, 0x28, 0xcf, 0xbc, 0xe9, 0xa2, 0xc0, 0x9a, 0x8d, 0xbb, 0x16,
					0x68, 0x6c, 0x61, 0x86, 0x80, 0x98, 0x0d, 0x28, 0xf1, 0x63, 0x08, 0x2b, 0x66, 0xb7, 0xb9, 0x42,
					0x5e, 0x80, 0x36, 0xa8, 0xd4, 0xe4, 0xff, 0xe1, 0x16, 0xd3, 0x4a, 0xa8, 0x05, 0x77, 0x3c, 0x8c,
					0x94, 0x50, 0x25, 0x48, 0xa8, 0xb0, 0x01, 0x44, 0x29, 0xa4, 0x02, 0x26, 0xbc, 0xce, 0x10, 0xe2,
					0x37, 0x4e, 0xca, 0x99, 0x91, 0xd6, 0x73, 0x6d, 0x43, 0xa2, 0xd3, 0x60, 0x8b, 0x77, 0x43, 0x97,
					0x65, 0x3a, 0x98, 0x9d, 0x8e, 0xec, 0xa0, 0x66, 0xff, 0x48, 0x06, 0x7f, 0x3b, 0xdc, 0xe4, 0x61,
					0x4e, 0x48, 0x91, 0x17, 0xad, 0x42, 0x75, 0x50, 0xe6, 0x8f, 0x46, 0x36, 0xc1, 0x1c, 0x27, 0x54,
					0x88, 0x72, 0xa4, 0x94, 0x73, 0x06, 0x13, 0xbb, 0x20, 0xb6, 0x7a, 0x83, 0xc9, 0xf0, 0xd9, 0x1f,
					0x6d, 0x51, 0x2f, 0x64, 0x3b, 0x49, 0x0c, 0x51, 0x1d, 0x5f, 0xeb, 0x81, 0xf4, 0x72, 0x37, 0xec,
					0xcc, 0x20, 0x64, 0x01, 0x9c, 0x20, 0x3e, 0xb6, 0xed, 0xf2, 0xa5, 0x41, 0xc9, 0xfc, 0xc6, 0x27,
					0xb5, 0xa9, 0x2f, 0x15, 0x4a, 0x36, 0x95, 0xa9, 0xa3, 0xd5, 0xcc, 0x2c, 0x31, 0x4c, 0x45, 0x61,
					0x41, 0x24, 0xb8, 0x74, 0xc8, 0x02, 0x12, 0xf9, 0x6d, 0x11, 0x99, 0x9a, 0x4e, 0x47, 0xa5, 0x22,
					0xf3, 0x93, 0x84, 0x2a, 0x5a, 0x19, 0xe0, 0x92, 0xf7, 0xb7, 0x65, 0xec, 0xbf, 0x52, 0xa2, 0xf4,
					0xb8, 0xef, 0x5a, 0xae, 0x27, 0x71, 0x85, 0xed, 0x2c, 0x9f, 0xbe, 0x01, 0xba, 0xea, 0x08, 0xd6,
					0xf6, 0x37, 0x02, 0xf6, 0xb3, 0x66, 0x6f, 0x48, 0x46, 0xf9, 0x37, 0xab, 0x92, 0xf2, 0x26, 0xb7,
					0x5a, 0x8a, 0x6a, 0x74, 0x87, 0xef, 0xc2, 0xc7, 0x3e, 0x3b, 0xdd, 0xc2, 0xcb, 0x58, 0xbf, 0x24,
					0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

struct img_header {
	unsigned int type;
	unsigned int version;
	unsigned int whoknows;
	unsigned int memaddress;
	unsigned int size;
	unsigned int dunno[5];
};


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


int ioc_blockdev_size(const char* blockdev) {
	int fd;
	uint64_t fz;
	fd = open(blockdev, O_RDWR);
	if (fd <= 0) return 0;
	ioctl(fd, BLKGETSIZE64, &fz);
	close(fd);
	return (fz > 0) ? (int)fz : 0;
}


int backup_loaders(void) {
	int ret, i;
	FILE *inf, *outf;
	unsigned int size;
	char buf[BLKSIZE] = {0};

	inf = fopen("/dev/block/mmcblk0", "r");
	if(!inf) return -1;

	outf = fopen("/dev/block/mmcblk1", "r+");
	if(!outf) {
		fclose(inf);
		return -2;
	}

	for(i = 0; i < BLOCKS; i++) {
		if (!fread(buf, BLKSIZE, 1, inf)) {
			fclose(inf);
			fclose(outf);
			return -3;
		}
		if (!fwrite(buf, BLKSIZE, 1, outf)) {
			fclose(inf);
			fclose(outf);
			return -4;
		}
	}

	fsync(fileno(outf));
	fclose(inf);
	fclose(outf);
	return 0;
}


int find_signature_offset(void *buf, int size) {
	unsigned int offset;
	struct img_header *header;

	header = (struct img_header *)buf;
	if (header->version != 3) return -1;//some header are size 0x50, find an example and update code for that use case
	offset = header->size;
	if (offset < 0xf0000 || offset > 0x300000) return -2;//probably a useless sanity check
	return offset + sizeof(struct img_header);
}


int write_dev_signature(void *sig) {
	int ret, offset;
	FILE *f;
	char *buf;
	size_t size;

	size = ioc_blockdev_size(ABOOTNAME);
	if (!size) {
		printf("[-] couldn't stat aboot partition\n");
		ret = -1;
		goto out;
	}

	buf = calloc(size, 1);
	if(!buf) {
		printf("[-] out of memory, wtf?\n");
		ret = -1;
		goto out;
	}

	f = fopen(ABOOTNAME, "r+");
	if (!f) {
		printf("[-] couldn't open aboot\n");
		ret = -1;
		goto out;
	}

	if(!fread(buf, sizeof(struct img_header), 1, f)) {
		printf("[-] can't read aboot\n");
		ret = -1;
		goto out;
	}

	offset = find_signature_offset(buf, size);
	if (!offset) {
		printf("[-] couldn't find dev signature offset\n");
		ret = -1;
		goto out;
	}

	if(fseek(f, offset, SEEK_SET)) {
		printf("[-] fseek\n");
		ret = -1;
		goto out;
	}

	if(!fwrite(dev_sig, sizeof(dev_sig), 1, f)) {
		printf("[-] really bad time to fail.  Don't reboot, try again\n");
		ret = -1;
		goto out;
	}
	ret = 0;
out:
	if (f) {
		fsync(fileno(f));
		fclose(f);
	}
	if (buf)
		free(buf);
	return ret;
}


int program_dev_cid(void) {
	int fd, ret, i;

	printf("[+] programming new CID ");
	for(i = 0; i < CID_SIZE; i++){
		printf("%02x", dev_cid[i]);
	}
	printf("\n");

	fd = open("/dev/block/mmcblk0", O_RDWR);
	if(fd < 0){
		printf("[-] wtf\n");
		return fd;
	}

	ret = cid_backdoor(fd);
	if(ret){
		printf("[-] cid_backdoor failed %d\n", ret);
		return ret;
	}

	ret = program_cid(fd, dev_cid);
	if(ret){
		printf("[-] program_cid failed %d\n", ret);
		return ret;
	}
	return 0;
}

int nearly_useless_compat_check(void) {
	FILE *f;
	char *buf;
	int ret;

	buf = calloc(4096, 1);
	if(!buf) {
		printf("[-] wtf, can't get 4k???\n");
		return 0;
	}
	f = fopen("/proc/cmdline", "r");
	fread(buf, 1, 4096, f);
	ret =  (int)strstr(buf, "amsung");
	free(buf);
	return ret;
}


int do_dev_loaders(void) {
	int ret;

	printf("[+] backing up loaders, this will take a few minutes\n");
	ret = backup_loaders();
	if (ret == -2) {
		printf("[-] you MUST have a 1GB or greater sd card inserted\n");
		printf("[-] 4GB or greater is recommended\n");
		printf("[-] do NOT patch this out of code!\n");//dumbasses will do this anyhow
		return ret;
	}
	else if (ret) {
		printf("[-] something went wrong backing up loaders\n");
		printf("[-] figure it out\n");
		printf("[-] do NOT patch this out of code!\n");//dumbasses will do this anyhow
	}
	printf("[+] loaders successfully backed up\n");
	ret = write_dev_signature(dev_sig);
	if (ret) printf("[-] dev sig fail, error %d\n", ret);
	return ret;
}

int query_user(const char* question) {

	char response[5] = {0};

	printf("%s\n", question);
	do {
		printf("(Yes/No)\n");
		fflush(stdout);
		scanf("%03s", response);

		if (!strcasecmp(response, "yes")) {
			fflush(stdout);
			return 1;
		}

		if (!strcasecmp(response, "no")) {
			fflush(stdout);
			return 0;
		}

		printf("Type \"Yes\" or \"No\"\n");
		fflush(stdout);
	} while (1);

	return 0;
}


int prompt_user(void) {
	printf( "\n\n============================== samdunk unlock 0.0.1 ==============================\n\n "
			"this application comes with NO WARRANTY (express or implied)\n "
			"this binary may not be rehosted, repackaged, one-clicked, etc.\n"
			"there is no support provided for this application\n"
			"this application has been tested on the Verizon Galaxy S5 only\n"
			"it may work on the AT&T Galaxy S5, and possibly other similar Galaxy devices\n"
			"there are no compatibility checks, do your research first\n"
			"if run on an incompatible phone, it will likely permanently ruin the device\n"
			"we STRONGLY advise against running any binary not obtained from the official source\n"
			"official source is available at http://github.com/beaups/SamsungCID\n"
			"SD card is required, all data on the SD card will be destroyed\n"
			"changing to this developer CID may have other implications\n"
			"the psn derived from the CID may be used for critical services\n"
			"changing this psn may cause unexpected behavior or loss of services\n"
			"continue at your own risk, you've been warned\n\n\n"
			"aboot dev signature research credit to ryanbg\n"
			"http://forum.xda-developers.com/member.php?u=766721\n\n"
			"eMMC vulnerability, exploit, and the code you are running by beaups (sean beaupre)\n"
			"http://forum.xda-developers.com/member.php?u=711482\n\n");
	fflush(stdout);
	return query_user("Do you understand the implications of these warnings?\n");
}


void main(int argc, const char **argv) {
	int ret;
	char cid[CID_SIZE] = {0};
	char old_cid[CID_SIZE] = {0};

	if(getuid()) {
		printf("[-] you must be root to use this\n");
		return;
	}

	if(!prompt_user()) {
		printf("[-] sorry you can't agree to the terms :(\n");
		return;
	}

	if(!nearly_useless_compat_check()) {
		printf("[-] this is for (some) Samsung devices only\n");
		printf("[-] it WILL kill other devices.  go back to google\n");
		return;
	}

	if(get_cid(old_cid) < 0) {
		printf("[-] can't get current CID, find it and fix the code\n");
		return;
	}

	show_cid(old_cid);

	if(!is_samsung_emmc(old_cid)) {
		printf("[-] don't try this on non-samsung eMMC, seriously.\n");
		return;
	}

	if(memcmp(old_cid, dev_cid, CID_SIZE)) {
		printf("[+] device not yet dev CID, now changing to dev CID\n");
		if(!program_dev_cid()) {
			printf("[+] success! powering off device, power back on and verify CID\n");
			printf("[+] then run this binary again to finish the process\n");
			reboot(LINUX_REBOOT_CMD_POWER_OFF);
			return;//just in case reboot fails, we don't want to accidentally continue
		}
		else {
			printf("[-] fail\n");
			return;
		}
	}
	else {
		printf("[+] dev CID matching, proceding to unlock\n");
		if(!do_dev_loaders()) {
			printf("[+] success! powering off device, hopefully its not bricked!\n");
			reboot(LINUX_REBOOT_CMD_POWER_OFF);
		}
		else {
			printf("[-] fail\n");
			return;
		}
	}
}
