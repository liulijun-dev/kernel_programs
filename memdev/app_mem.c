#include<stdio.h>
#include<string.h>

int main(void){
    FILE *fp0 = NULL;
	char buf[4096];

	strcpy(buf, "mem is char dev");
	printf("BUF:%s\n", buf);

	fp0 = fopen("/dev/memdev0", "r+");
	if(fp0 == NULL){
        printf("Open memdev0 error!\n");
		return -1;
	}

	fwrite(buf, sizeof(buf), 1, fp0);

	fseek(fp0, 0, SEEK_SET);

	strcpy(buf, "buf is null");
	printf("BUF:%s\n", buf);


	fread(buf, sizeof(buf), 1, fp0);
    printf("BUF:%s\n", buf);

	return 0;
}
