#ifndef MMC_H_
#define MMC_H_

#include <stdint.h>

/*
 * EXT_CSD field definitions
 */
#define EXT_CSD_HPI_SUPP                (1<<0)
#define EXT_CSD_HPI_IMPL                (1<<1)
#define EXT_CSD_CMD_SET_NORMAL          (1<<0)
#define EXT_CSD_BOOT_WP_B_PWR_WP_DIS    (0x40)
#define EXT_CSD_BOOT_WP_B_PERM_WP_DIS   (0x10)
#define EXT_CSD_BOOT_WP_B_PERM_WP_EN    (0x04)
#define EXT_CSD_BOOT_WP_B_PWR_WP_EN     (0x01)
#define EXT_CSD_BOOT_INFO_HS_MODE       (1<<2)
#define EXT_CSD_BOOT_INFO_DDR_DDR       (1<<1)
#define EXT_CSD_BOOT_INFO_ALT           (1<<0)
#define EXT_CSD_BOOT_CFG_ACK            (1<<6)
#define EXT_CSD_BOOT_CFG_EN             (0x38)
#define EXT_CSD_BOOT_CFG_ACC            (0x03)
#define EXT_CSD_PART_CONFIG_ACC_MASK      (0x7)
#define EXT_CSD_PART_CONFIG_ACC_BOOT0     (0x1)
#define EXT_CSD_PART_CONFIG_ACC_BOOT1     (0x2)
#define EXT_CSD_PART_CONFIG_ACC_USER_AREA (0x7)
#define EXT_CSD_PART_CONFIG_ACC_ACK       (0x40)

#define MMC_RSP_PRESENT (1 << 0)
#define MMC_RSP_136     (1 << 1)                
#define MMC_RSP_CRC     (1 << 2)               
#define MMC_RSP_BUSY    (1 << 3)               
#define MMC_RSP_OPCODE  (1 << 4)               
#define MMC_CMD_AC      (0 << 5)
#define MMC_CMD_ADTC    (1 << 5)

#define MMC_RSP_SPI_S1  (1 << 7)           
#define MMC_RSP_SPI_BUSY (1 << 10)         

#define MMC_CMD_BCR (3 << 5)

#define MMC_RSP_SPI_R1  (MMC_RSP_SPI_S1)
#define MMC_RSP_SPI_R1B (MMC_RSP_SPI_S1|MMC_RSP_SPI_BUSY)

#define MMC_RSP_R1      (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R1B     (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE|MMC_RSP_BUSY)
#define MMC_RSP_R2 		(MMC_RSP_PRESENT|MMC_RSP_136|MMC_RSP_CRC)

#include "ioctl.h"
#include "major.h"


#endif /* MMC_H_ */
