
#ifndef _VERSION_TABLE_H_
#define _VERSION_TABLE_H_

#define PART_NAME_MAX_LEN	15
#define IMG2_NAME	"bootloader"
#define IMG3_NAME	"bootimgs"
#define SYSCONF_NAME	"sysconf"
#define FLASHLESS_NAME	"flashless_data"

typedef struct _version_t_ {
	union {
		struct {
			unsigned minor_version;
			unsigned major_version;
		};
		unsigned long long version;
	};
} version_t __attribute__ ((aligned (4)));

typedef struct _ver_table_entry_t_ {
	char name[PART_NAME_MAX_LEN+1];
	version_t part1_version;
	unsigned part1_start_blkind;
	unsigned part1_blks;
	version_t part2_version;
	unsigned part2_start_blkind;
	unsigned part2_blks;
} ver_table_entry_t;

typedef struct _version_table_t_ {
	unsigned int magic;
	unsigned int ou_status;
	unsigned int num_entries;
	ver_table_entry_t table[];
} version_table_t;

#endif
