typedef unsigned int u32;
typedef unsigned int u8;

/* The list must start with an ATAG_CORE node */
#define ATAG_CORE		0x54410001
#define ATAG_MEM		0x54410002
#define ATAG_INITRD2    0x54420005
#define ATAG_REVISION   0x54410007
#define ATAG_CMDLINE	0x54410009
#define ATAG_NONE		0x00000000

struct tag_header {
	u32 size;
	u32 tag;
};

struct tag_core {
	u32 flags;		/* bit 0 = read-only */
	u32 pagesize;
	u32 rootdev;
};

struct tag_mem32 {
	u32	size;
	u32	start;	/* physical start address */
};

struct tag_initrd {
	u32 start;	/* physical start address */
	u32 size;	/* size of compressed ramdisk image in bytes */
};

struct tag_cmdline {
	char cmdline[1];	/* this is the minimum size */
};

struct tag_revision {
	u32 rev; /* chip/board revision */
};

struct tag {
	struct tag_header hdr;
	union {
		struct tag_core		core;
		struct tag_mem32	mem;
		struct tag_initrd	initrd;
		struct tag_cmdline	cmdline;
		struct tag_revision revision;
	} u;
};

static struct tag *params;
#define tag_next(t)	((struct tag *)((u32 *)(t) + (t)->hdr.size))
#define tag_size(type)	((sizeof(struct tag_header) + sizeof(struct type)) >> 2)

static void setup_start_tag(u32 bootparam_addr)
{
	params = (struct tag *)bootparam_addr;

	params->hdr.tag = ATAG_CORE;
	params->hdr.size = tag_size(tag_core);	/* in dword */

	params->u.core.flags = 0;
	params->u.core.pagesize = 0;
	params->u.core.rootdev = 0;

	params = tag_next(params);
}

static void setup_memory_tag(u32 mem_start, u32 mem_size)
{
	int i;

	params->hdr.tag = ATAG_MEM;
	params->hdr.size = tag_size(tag_mem32);

	params->u.mem.start = mem_start;
	params->u.mem.size = mem_size;

	params = tag_next(params);
}

static void setup_cmdline_tag(char *cmdline)
{
	char *p;
	int i;

	if (!cmdline)
		return;

	for (p = cmdline; *p == ' '; p++);

	if (*p == '\0') return;

	i = 0;
	while (p[i] != '\0') {
		params->u.cmdline.cmdline[i] = p[i];
		i++;
	}
	params->u.cmdline.cmdline[i] = '\0';
	
	params->hdr.tag = ATAG_CMDLINE;
	params->hdr.size = (sizeof(struct tag_header) + i + 1 + 4) >> 2;

	params = tag_next(params);
}

static void setup_initrd_tag (u32 initrd_start, u32 initrd_end)
{
	/* an ATAG_INITRD node tells the kernel where the compressed
	 * ramdisk can be found. ATAG_RDIMG is a better name, actually.
	 */
	params->hdr.tag = ATAG_INITRD2;
	params->hdr.size = tag_size (tag_initrd);

	params->u.initrd.start = initrd_start;
	params->u.initrd.size = initrd_end - initrd_start;

	params = tag_next (params);
}

static void setup_end_tag(void)
{
	params->hdr.tag = ATAG_NONE;
	params->hdr.size = 0;
}

static void setup_revision_tag (u32 revision)
{
	params->hdr.tag = ATAG_REVISION;
	params->hdr.size = tag_size (tag_revision);

	params->u.revision.rev = revision;

	params = tag_next (params);
}

/*
 * exported global entry
 * e.g.
 * #define STRING "macaddr=00:80:10:04:10:01 console=ttyS0,19200 root=/dev/nfs nfsroot=10.38.54.88:/home/galois/galois-rootfs,v3 ip=dhcp"
 * setup_linux_bootparam(0x0E000000, 0x0E000000, STRING);
 */
void setup_linux_bootparam(u32 mem_start, u32 mem_size, char *cmdline,
						   u8 chip_rev, u8 board_rev)
{
    lgpl_printf("linux parameter address 0x%08x\n", mem_start+0x100);
	setup_start_tag(mem_start+0x100);
	setup_revision_tag((board_rev << 8) | chip_rev);
	setup_memory_tag(mem_start, mem_size);
	setup_cmdline_tag(cmdline);
	setup_end_tag();
	return;
}

void setup_linux_bootparam_with_ramdisk(u32 mem_start, u32 mem_size, char *cmdline,
										u32 initrd_start, u32 initrd_size,
										u8 chip_rev, u8 board_rev)
{
	setup_start_tag(mem_start+0x100);
	setup_memory_tag(mem_start, mem_size);
	setup_revision_tag((board_rev << 8) | chip_rev);
	setup_cmdline_tag(cmdline);
	if (initrd_size) {
	      setup_initrd_tag(initrd_start, initrd_start + initrd_size);
	}
	setup_end_tag();
	return;
}

#if 0
int main(void)
{
 #define STRING "macaddr=00:80:10:04:10:01 console=ttyS0,19200 root=/dev/nfs nfsroot=10.38.54.88:/home/galois/galois-rootfs,v3 ip=dhcp"
	setup_linux_bootparam(0x0e000000, 0x0e000000, STRING);
}

#endif
