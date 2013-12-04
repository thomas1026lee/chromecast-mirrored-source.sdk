#include <stdio.h>
#include <stdlib.h>
//#include <file.h>

int main( void )
{
	FILE *fr, *fw;
	int data, size, i;

	printf( "hello world\n" );

	fr = fopen( "figo_sram.bin", "r" );
	fw = fopen( "figo_sram_bin.c", "w" );
//	fw = fopen( "build/SM_Code.c", "w" );

	if (NULL == fr || NULL == fw)
	{
		printf("can't open files!\n");
		return -1;
	}

	fprintf( fw, "#include \"com_type.h\"\n\nunsigned int FIGO_Sram_Code[]= {\n" );

	for( i = 0 ; ; i ++ )
	{
		size = fread( &data, 1, 4, fr );
		if( size == 0 ) break;

		fprintf( fw, "0x%8.8x, ", data );
		if( i % 8 == 7 )
			fprintf( fw, "\n" );
	}

	fprintf( fw, "};\n\nunsigned int Figo_Sram_Code_Size = %d;\n", i );

	fclose( fr );
	fclose( fw );

	return 0;
}
