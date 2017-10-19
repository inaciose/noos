/*
struct FILEHANDLE {
	char *buf;
	int size;
	int pos;
};
*/

struct FILEINFO {
  uint8_t name[8], ext[3], type;
  char reserve[10];
  uint16_t time, date, clustno;
  uint32_t size;
};

void file_readfat(struct MEMMAN *memman, uint32_t *fat, long long img);
void file_loadfile(int clustno, int size, char *buf, uint32_t *fat, long long img);
struct FILEINFO *file_search(char *name, struct FILEINFO *finfo, int max);
