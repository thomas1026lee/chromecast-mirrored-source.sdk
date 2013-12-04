#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "stdio.h"
#include <fcntl.h>

#define	SIGN_TYPE_OFFSET	0x44
#define	SIGN_TYPE_RSA		0x2
#define	SIGN_LEN_OFFSET		0x45
#define	SIGN_LEN			0x80

int main(int argc, char *argv[]){
	unsigned char * image_buffer = NULL;
   	int * iPoint ; 	
	int i ; 
	int file ; 
	struct stat st ; 
	int image_size ; 

	int read_bytes ; 

	if( argc != 3){
		fprintf(stdout, "invalid arguments\n") ; 
		return -1 ; 
	}

	file = open(argv[1] , O_RDONLY) ; 
	if(file == -1){
		fprintf(stdout, "open file %s error\n" , argv[1]) ; 
		return -1 ; 
	}

	if(fstat(file , &st) == -1){
		fprintf(stdout, "get file stat errror\n") ; 
		return -1 ;
	}
	image_size = st.st_size ; 
	image_size = (image_size + 15) & 0xfffffff0 ; 	// 16 bytes aligment
	
	image_buffer = (unsigned char *)malloc(image_size +	256) ; 
	memset(image_buffer , 0 , (image_size +  256)) ;

	if(image_buffer == NULL){
		fprintf(stdout, "malloc error\n") ;
		close(file) ; 
		return -1 ; 
	}

	iPoint = (int *)(image_buffer+124) ; 
	*iPoint = st.st_size+128 ;

	read_bytes = read(file , (image_buffer+256), st.st_size) ;

	if(read_bytes < st.st_size){
		fprintf(stdout, "read %d bytes , less then %d bytes\n", read_bytes, st.st_size) ; 
		close(file) ;
		return -1 ;
	}	

	close(file) ; 

	file = open(argv[2] , O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IXUSR) ;

	if(file == -1){
		fprintf(stdout, "open %s error\n", argv[2]) ; 
		return -1 ;
	}

	//	update sign key type and sign len
	*(image_buffer+SIGN_TYPE_OFFSET) = SIGN_TYPE_RSA ; 
	*(image_buffer+SIGN_LEN_OFFSET) = SIGN_LEN ; 
	
	if( write(file , image_buffer , (image_size+256)) == -1 ){
		fprintf(stdout, "write Image fail\n");
	}

	close(file) ; 
	
	return 0 ; 
}
