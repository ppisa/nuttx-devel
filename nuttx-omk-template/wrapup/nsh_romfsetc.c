/****************************************************************************
 * apps/nshlib/nsh_romfsetc.c
 *
 *   Copyright (C) 2008-2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <sys/mount.h>
#include <sys/types.h>
#include <syslog.h>
#include <debug.h>
#include <errno.h>

#include <nuttx/drivers/ramdisk.h>

extern uint8_t romfs_img_start[];
extern uint8_t romfs_img_end[];
extern uint8_t romfs_img_size[];

#ifdef CONFIG_ETC_ROMFS

const unsigned int romfs_img_len = (uintptr_t)romfs_img_size;

#elif defined(CONFIG_NSH_ROMFSMOUNTPT) && defined(CONFIG_NSH_ROMFSDEVNO)

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define NSECTORS(b)        (((b)+CONFIG_NSH_ROMFSSECTSIZE-1)/CONFIG_NSH_ROMFSSECTSIZE)
#define STR_RAMDEVNO(m)    #m
#define MKMOUNT_DEVNAME(m) "/dev/ram" STR_RAMDEVNO(m)
#define MOUNT_DEVNAME      MKMOUNT_DEVNAME(CONFIG_NSH_ROMFSDEVNO)


/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: nsh_romfsetc
 ****************************************************************************/

int nsh_romfsetc(void)
{
  int  ret;

  syslog(LOG_ERR, "romdisk_register(%d, %p, %d, %d)\n",
                   CONFIG_NSH_ROMFSDEVNO, romfs_img_start,
                   NSECTORS((unsigned int)(romfs_img_end - romfs_img_start)),
                   CONFIG_NSH_ROMFSSECTSIZE);

  /* Create a ROM disk for the /etc filesystem */

  ret = romdisk_register(CONFIG_NSH_ROMFSDEVNO, romfs_img_start,
                         NSECTORS((unsigned int)(romfs_img_end - romfs_img_start)),
                         CONFIG_NSH_ROMFSSECTSIZE);
  if (ret < 0)
    {
      syslog(LOG_ERR, "ERROR: romdisk_register failed: %d\n", -ret);
      return ERROR;
    }

  /* Mount the file system */

  syslog(LOG_ERR, "Mounting ROMFS filesystem at target=%s with source=%s\n",
        CONFIG_NSH_ROMFSMOUNTPT, MOUNT_DEVNAME);

  ret = mount(MOUNT_DEVNAME, CONFIG_NSH_ROMFSMOUNTPT, "romfs", MS_RDONLY, NULL);
  if (ret < 0)
    {
      syslog(LOG_ERR, "ERROR: mount(%s,%s,romfs) failed: %d\n",
           MOUNT_DEVNAME, CONFIG_NSH_ROMFSMOUNTPT, errno);
      return ERROR;
    }
  return OK;
}

#else
#warning check that CONFIG_FS_ROMFS is enabled (set to "y")
#warning CONFIG_ETC_ROMFS and CONFIG_ETC_ROMFSMOUNTPT are required for NuttX 12.4+
#warning CONFIG_NSH_ROMFSETC and CONFIG_NSH_ROMFSMOUNTPT are required for older systems
#error Can not setup initial /etc content, aborting compilation
#endif
