/**
 * 請把"lib\SDC00\"裡的檔案下載並加入專案中，並在makefile中的LIBSRC引用SDC00資料夾內
 * 所有.c檔。
 * ex LIBSRC += $(wildcard lib/SDC00/*.c)
 */

#include "ASA_Lib.h"
#include "lib\SDC00\ASA_lib_SDC00.h"

int main(void) {

	ASA_M128_set();
	printf("Test!! ");

	/* Variables for ASA lib */
	unsigned char ASA_ID = 1, Mask = 0xFF, Shift = 0, Setting = 0xFF;


	uint16_t date_rawdata;
	uint16_t time_rawdata;

	// The buffer used to swap data with SDC00
	uint8_t swap_buffer[64];
	memset(swap_buffer, 0, sizeof(swap_buffer));

	char check;						// module communication result state flag

	while(1) {
		// Select target file
		ASA_SDC00_put(ASA_ID, 64, 8, "testa");
		ASA_SDC00_put(ASA_ID, 72, 3, "txt");

		// Configure to open file
		Setting = 0x01;				// Setting mode for openFile (readonly)
		check = ASA_SDC00_set(ASA_ID, 200, Mask, Shift, Setting);// 送出旗標組合
		if( check != 0 ) {				// 檢查回傳值做後續處理
			printf("Debug point 1, error code <%d>\n", check);
			break;
		}

		unsigned char sz_buffer[4];
		unsigned char date_buffer[2];
		unsigned char time_buffer[2];
		// 讀取檔案參數
		check = ASA_SDC00_get(ASA_ID, 82, 1, sz_buffer);
		printf("<OUT>Sz: %ld\n", sz_buffer[0] + sz_buffer[1] >> 8 + sz_buffer[2] >> 16 + sz_buffer[3] >> 24);


		check = ASA_SDC00_get(ASA_ID, 78, 2, date_buffer);
		date_rawdata = date_buffer[0];
		date_rawdata += date_buffer[1] << 8;
		printf("<OUT>Date %0x %0x   -> %0x\n", date_buffer[1], date_buffer[0], date_rawdata);
		printf("<OUT>Date %u/%u/%u", 1980+(uint16_t)(date_rawdata>>9), (uint16_t)(date_rawdata>>5)& 0x0F, (uint16_t)(date_rawdata)& 0x1f);

		check = ASA_SDC00_get(ASA_ID, 76, 2, time_buffer);
		time_rawdata = time_buffer[0];
		time_rawdata += time_buffer[1] << 8;
		printf("<OUT>Time %0x%0x   -> %0x\n", time_buffer[1], time_buffer[0], time_rawdata);
		printf("<OUT>Time %u:%u:%u\n", time_rawdata>>11, (time_rawdata>>5) &0x3f, (time_rawdata & 0x1f) /2);


		printf("<Rec Raw data>fdate = %0x, ftime = %0x\n", date_rawdata, time_rawdata);

		printf("Start\n");
		int rec = 0;
		for(int i=0; i<4000; i++) {
		/*** Read data from SDC00 ****/
			if(rec = ASA_SDC00_get(ASA_ID, 0, 64, swap_buffer)) {
				printf("Reading finish! Get %ld  bytes %d\n", (long)i*64 + swap_buffer[63], rec);
				break;
			}

		}
		printf("End\n");

		// Configure to close file mode
		Setting = 0x00;
		check = ASA_SDC00_set(ASA_ID, 200, Mask, Shift, Setting);// 送出旗標組合
		if( check != 0 )				// 檢查回傳值做後續處理
			printf("Debug point 2, error code <%d>\n", check);

		return 0;
	}
	return 0;
}
