#include "ObjectDecompiler.h"

// static

time_t ObjectDec::log_time;
tm * ObjectDec::tml(nullptr);
char ObjectDec::inifile[] = ".\\material_full.ini\0";
int ObjectDec::ini_count(0);
ObjectDec::ini_str ObjectDec::ini_sting[MAX_ini_str];

// methods
// private

unsigned long ObjectDec::searchEndString(char *buffer, unsigned long buff_ptr, unsigned long maxsize)
{
	unsigned long null_ptr;
	for (null_ptr = buff_ptr; null_ptr < maxsize; null_ptr++)
		if (buffer[null_ptr] == 0x00)
			break;
	return null_ptr - buff_ptr;
}

void ObjectDec::StrTOLower(char *a)
{
	int i = 0;
	while (a[i])
	{
		a[i] = (tolower(a[i]));
		i++;
	}
}

int ObjectDec::ininame_search(char* ininame) 
{
	int i = 0;

	while (i<ini_count)
	{
		if ((strlen(ininame) == strlen(ini_sting[i].t_path)) &&
			(!strncmp(ini_sting[i].t_path, ininame, strlen(ininame)))
			)
			break;
		//strcpy(ini_sting[ini_count].t_path,name);
		//strcpy(ini_sting[ini_count].value,value);
		i++;
	}
	if (i >= ini_count) i = NULL;
	return i;
}

// public

int ObjectDec::stlk_object_mtpatch(System::Windows::Forms::RichTextBox^ % log, short mode)
{
	//mode :
	// 1 - LOD
	// 2 - HOM

	if (mode == 1 || mode == 2)
	{
		std::string new_file = this->file_name;
		new_file = new_file.insert(new_file.find_last_of("."), "_oldest");
		System::IO::File::Copy(gcnew String(this->file_name.c_str()), gcnew String(new_file.c_str()));
	}
	const char* in_file_name = this->file_name.c_str();
	int ret;
	FILE *fp_i = NULL;
	char *buffer;
	//static int             buff_MAX=1024;
	unsigned long block_rw;
	u_32 int_32;
	u_32 block = 0;
	u_32 blockSize = 0;
	int block_num = 0;
	char msg[256];
	char tmpmsg[256];
	unsigned long lSize; //size of inp file
	unsigned long buff_ptr = 0;  //buffer offset
	unsigned long null_ptr = 0;  //buffer offset
	unsigned long start_ptr = 0;  //buffer offset
	u_32 int_tmp;
	int model_type = 0; //model type для патча
	int flags_2_sided = 0; //flags (2 sided, etc) для патча

	ret = 0;
	block_num = 0;
	buff_ptr = 0;

	/* open the input file for read */
	fp_i = fopen(in_file_name, "rb+");
	if (fp_i == NULL)
	{
		sprintf(msg, "Ошибка открытия файла %s \r\n", in_file_name);
		log->AppendText(gcnew String(msg));
		ret = 1;
		if (fp_i != NULL)
		{
			fclose(fp_i);
			fp_i = NULL;
		}
		free(buffer);
		return ret;
	}

	fseek(fp_i, 0, SEEK_END);
	lSize = ftell(fp_i);
	fseek(fp_i, 0, SEEK_SET);
	//rewind (fp_i);

	sprintf(msg, "файл: %s\r\n", in_file_name);
	log->AppendText(gcnew String(msg));
	sprintf(msg, "размер: %ld\r\n", lSize);
	log->AppendText(gcnew String(msg));

	if (lSize<12)
	{
		sprintf(msg, "Файл слишком маленький.\r\n");
		log->AppendText(gcnew String(msg));
		ret = 1;
		fclose(fp_i);
		if (fp_i != NULL)
		{
			fclose(fp_i);
			fp_i = NULL;
		}
		free(buffer);
		return ret;
	}

	//Allocate memory
	buffer = (char *)malloc(lSize + 1);
	if (!buffer)
	{
		sprintf(msg, "Memory error!\r\n");
		log->AppendText(gcnew String(msg));
		ret = 1;
		fclose(fp_i);
		if (fp_i != NULL)
		{
			fclose(fp_i);
			fp_i = NULL;
		}
		free(buffer);
		return ret;
	}

	//Read file contents into buffer
	fread(buffer, lSize, 1, fp_i);

	//Do what ever with buffer

	block_rw = 4;

	//memset(buff, 0, sizeof(buff));
	//buff_len = fread(buff, 1, block_rw, fp_i);
	memcpy(&block, buffer + buff_ptr, 4); //ID блока
	buff_ptr += 4;
	memcpy(&blockSize, buffer + buff_ptr, 4); //размер блока
	buff_ptr += 4;

	if (block != 0x7777)
	{
		sprintf(msg, "Ошибка! файла не соответсвует нужной структуре.\r\n");
		log->AppendText(gcnew String(msg));
		ret = 1;
		fclose(fp_i);
		if (fp_i != NULL)
		{
			fclose(fp_i);
			fp_i = NULL;
		}
		free(buffer);
		return ret;
	}

	if ((blockSize + 8) != lSize)
	{
		sprintf(msg, "Ошибка! Размер данных не совпадает с размером файла.\r\n");
		log->AppendText(gcnew String(msg));
		ret = 1;
		fclose(fp_i);
		if (fp_i != NULL)
		{
			fclose(fp_i);
			fp_i = NULL;
		}
		free(buffer);
		return ret;
	}

	//готовим  переменные для патча
	switch (mode)
	{
	case 1:
		model_type = 0x14;
		flags_2_sided = 0x00;
		break;

	case 2:
		model_type = 0x08;
		flags_2_sided = 0x01;
		break;
	}

	//начинаем читать блоки пока не закончится буфер
	while (buff_ptr < lSize)
	{
		//DEBUG_L("\n===\nCDR: # %d\n" ,block_num);


		memcpy(&block, buffer + buff_ptr, 4); //ID блока
		buff_ptr += 4;
		memcpy(&blockSize, buffer + buff_ptr, 4); //размер блока
		buff_ptr += 4;

		//            DEBUG_L("%02x %02x\n" ,buffer[buff_ptr],buffer[buff_ptr+1]);
		//                     1         2          3       4
		sprintf(msg, "block №%d ID: %08X размер: %d (0x%02X)\r\n",
			block_num + 1,  //1
			block,        //2
			blockSize,    //3
			blockSize     //4
		);
		log->AppendText(gcnew String(msg));
		block_num++;

		switch (block)
		{
		case 0x0903: sprintf(msg, "[Object Type Block]\r\n"); log->AppendText(gcnew String(msg));

			memcpy(&int_32, buffer + buff_ptr, 4); //int
			sprintf(msg, "model type = 0x%08X\r\n", int_32); log->AppendText(gcnew String(msg));

			block_rw = fseek(fp_i, buff_ptr, SEEK_SET);
			block_rw = fread(&int_tmp, 1, 4, fp_i);
			//sprintf(msg,"from file model type = 0x%08X\r\n",int_tmp); log->AppendText(gcnew String(msg));
			if ((mode == 1) || (mode == 2)) //если 0 только смотрим
			{
				if (int_32 != model_type)
				{
					int_tmp = model_type;
					block_rw = fseek(fp_i, buff_ptr, SEEK_SET);
					block_rw = fwrite(&int_tmp, 1, 4, fp_i);
					sprintf(msg, "=Ok!= Пропатчили 'model type'.\r\n"); log->AppendText(gcnew String(msg));
				}
				else
				{
					sprintf(msg, "Нет необходимости.\r\n"); log->AppendText(gcnew String(msg));
				}
			}

			buff_ptr += blockSize;
			break;
		case 0x0907: sprintf(msg, "[Materials Block]\r\n\0"); log->AppendText(gcnew String(msg));

			start_ptr = buff_ptr;

			memcpy(&int_32, buffer + buff_ptr, 4); //int
			sprintf(msg, "materials count = %d [0x%02X]\r\n", int_32, int_32); log->AppendText(gcnew String(msg));
			buff_ptr += 4;


			for (int i = 1; i <= int_32; ++i)
			{
				u_32 int_tmp;
				null_ptr = searchEndString(buffer, buff_ptr, lSize);
				memcpy(&tmpmsg, buffer + buff_ptr, null_ptr + 1); //копируем строку
				sprintf(msg, "material №%d name = %s\r\n", i, tmpmsg); log->AppendText(gcnew String(msg));
				buff_ptr += null_ptr + 1;

				null_ptr = searchEndString(buffer, buff_ptr, lSize);
				memcpy(&tmpmsg, buffer + buff_ptr, null_ptr + 1); //копируем строку
				sprintf(msg, "engine shader = %s\r\n", tmpmsg); log->AppendText(gcnew String(msg));
				buff_ptr += null_ptr + 1;

				null_ptr = searchEndString(buffer, buff_ptr, lSize);
				memcpy(&tmpmsg, buffer + buff_ptr, null_ptr + 1); //копируем строку
				sprintf(msg, "compiler shader = %s\r\n", tmpmsg); log->AppendText(gcnew String(msg));
				buff_ptr += null_ptr + 1;

				null_ptr = searchEndString(buffer, buff_ptr, lSize);
				memcpy(&tmpmsg, buffer + buff_ptr, null_ptr + 1); //копируем строку
				sprintf(msg, "game material = %s\r\n", tmpmsg); log->AppendText(gcnew String(msg));
				buff_ptr += null_ptr + 1;

				null_ptr = searchEndString(buffer, buff_ptr, lSize);
				memcpy(&tmpmsg, buffer + buff_ptr, null_ptr + 1); //копируем строку
				sprintf(msg, "texture path = %s\r\n", tmpmsg); log->AppendText(gcnew String(msg));
				buff_ptr += null_ptr + 1;

				null_ptr = searchEndString(buffer, buff_ptr, lSize);
				memcpy(&tmpmsg, buffer + buff_ptr, null_ptr + 1); //копируем строку
				sprintf(msg, "texture = %s", tmpmsg); log->AppendText(gcnew String(msg));
				buff_ptr += null_ptr + 1;

				memcpy(&int_tmp, buffer + buff_ptr, 4); //int
				sprintf(msg, "flags (2 sided, etc) = 0x%08X\r\n\r\n", int_tmp); log->AppendText(gcnew String(msg));

				block_rw = fseek(fp_i, buff_ptr, SEEK_SET);
				block_rw = fread(&int_tmp, 1, 4, fp_i);
				//sprintf(msg,"from file model type = 0x%08X\r\n",int_tmp); log->AppendText(gcnew String(msg));
				if (mode == 2) //если 0 только смотрим
				{
					if (int_32 != flags_2_sided)
					{
						int_tmp = flags_2_sided;
						block_rw = fseek(fp_i, buff_ptr, SEEK_SET);
						block_rw = fwrite(&int_tmp, 1, 4, fp_i);
						sprintf(msg, "=Ok!= Пропатчили 'flags'.\r\n"); log->AppendText(gcnew String(msg));
					}
					else
					{
						sprintf(msg, "Нет необходимости.\r\n"); log->AppendText(gcnew String(msg));
					}
				}
				buff_ptr += 4;

				memcpy(&int_tmp, buffer + buff_ptr, 4); //int
														//sprintf(msg,"unknown data1 = 0x%08X",int_tmp); log->AppendText(gcnew String(msg));
				buff_ptr += 4;
				memcpy(&int_tmp, buffer + buff_ptr, 4); //int
														//sprintf(msg,"unknown data2 = 0x%08X\n",int_tmp); log->AppendText(gcnew String(msg));
				buff_ptr += 4;

			}

			//buff_ptr+=blockSize;
			buff_ptr = start_ptr + blockSize;
			break;
		case 0x0912: sprintf(msg, "[UserData Block]\r\n\0"); log->AppendText(gcnew String(msg));

			buff_ptr += blockSize;
			break;
		case 0x0910: sprintf(msg, "[Geometry Block]\r\n\0"); log->AppendText(gcnew String(msg));

			buff_ptr += blockSize;
			break;
		case 0x0922: sprintf(msg, "[Author Block]\r\n\0"); log->AppendText(gcnew String(msg));
			null_ptr = searchEndString(buffer, buff_ptr, lSize);
			memcpy(&tmpmsg, buffer + buff_ptr, null_ptr + 1); //копируем строку
			sprintf(msg, "author name = %s\r\n", tmpmsg); log->AppendText(gcnew String(msg));
			buff_ptr += null_ptr + 1;

			//memcpy(&int_32,buffer+buff_ptr,4); //Date
			memcpy(&log_time, buffer + buff_ptr, 4); //Date
			tml = gmtime(&log_time);
			//fprintf(fp_out,"date = 0x%08X\n",int_32);
			sprintf(msg, "date =  %04d.%02d.%02d %02d:%02d:%02d\r\n", 1900 + tml->tm_year, 1 + tml->tm_mon, tml->tm_mday, tml->tm_hour, tml->tm_min, tml->tm_sec);
			log->AppendText(gcnew String(msg));
			buff_ptr += 4; //date

			null_ptr = searchEndString(buffer, buff_ptr, lSize);
			memcpy(&tmpmsg, buffer + buff_ptr, null_ptr + 1); //копируем строку
			sprintf(msg, "modifier name = %s\r\n", tmpmsg); log->AppendText(gcnew String(msg));
			buff_ptr += null_ptr + 1;

			memcpy(&int_32, buffer + buff_ptr, 4); //Date
			memcpy(&log_time, buffer + buff_ptr, 4); //Date
			tml = gmtime(&log_time);
			//fprintf(fp_out,"date = 0x%08X\n",int_32);
			sprintf(msg, "date = %04d.%02d.%02d %02d:%02d:%02d\r\n", 1900 + tml->tm_year, 1 + tml->tm_mon, tml->tm_mday, tml->tm_hour, tml->tm_min, tml->tm_sec);
			log->AppendText(gcnew String(msg));
			buff_ptr += 4; //date

						   //buff_ptr+=blockSize;
						   //buff_ptr+=blockSize;
			break;

		case 0x0925: sprintf(msg, "[LOD Block]\r\n\0"); log->AppendText(gcnew String(msg));
			buff_ptr += blockSize;
			break;

		default:     sprintf(msg, "[unknown data]\r\n\0"); log->AppendText(gcnew String(msg));
			buff_ptr += blockSize;
			break;
		}

		sprintf(msg, "\r\n\0"); log->AppendText(gcnew String(msg));

	}

	if (fp_i != NULL)
	{
		fclose(fp_i);
		fp_i = NULL;
	}

	free(buffer);
	sprintf(msg, "найдено блоков: %ld\r\n\0", block_num); log->AppendText(gcnew String(msg));
	return ret;
}

int ObjectDec::stlk_object_parce(System::Windows::Forms::RichTextBox^ % log, const char *out_file_name)
{
	const char *in_file_name = this->file_name.c_str();
	int ret;
	FILE *fp_i = NULL;
	FILE *fp_out = NULL;
	char *buffer;
	static int buff_MAX = 1024;
	unsigned long block_rw;
	//u_8                    buff[buff_MAX];
	//int                    buff_len;
	u_32 int_32;
	u_32 block = 0;
	u_32 blockSize = 0;
	int block_num = 0;
	char msg[500];
	unsigned long lSize; //size of inp file
	unsigned long buff_ptr = 0;  //buffer offset
	int null_ptr = 0;  //buffer offset
    //unsigned long          start_ptr=0;  //buffer offset

	ret = 0;
	block_num = 0;
	buff_ptr = 0;

	/* open the input file for read */
	fp_i = fopen(in_file_name, "rb");
	if (fp_i == NULL)
	{
		sprintf(msg, "Ошибка открытия вх. файла %s \r\n", in_file_name);
		log->AppendText(gcnew String(msg));
		ret = 1;
		if (fp_out != NULL)
		{
			fclose(fp_out);
			fp_out = NULL;
		}
		free(buffer);
	}

	/* open the output file for write */
	fp_out = fopen(out_file_name, "wb");
	if (fp_out == NULL)
	{
		sprintf(msg, "Ошибка открытия вых. файла %s \r\n", out_file_name);
		log->AppendText(gcnew String(msg));
		ret = 1;
		if (fp_out != NULL)
		{
			fclose(fp_out);
			fp_out = NULL;
		}
		free(buffer);
	}

	fseek(fp_i, 0, SEEK_END);
	lSize = ftell(fp_i);
	fseek(fp_i, 0, SEEK_SET);
	//rewind (fp_i);

	sprintf(msg, "файл: %s\r\n", in_file_name);
	log->AppendText(gcnew String(msg));
	sprintf(msg, "размер: %ld\r\n\r\n", lSize);
	log->AppendText(gcnew String(msg));

	fprintf(fp_out, "fsize: %ld file:%s\n\n", lSize, in_file_name);

	if (lSize<12)
	{
		sprintf(msg, "Файл слишком маленький.\r\n");
		log->AppendText(gcnew String(msg));
		ret = 1;
		fclose(fp_i);
		if (fp_out != NULL)
		{
			fclose(fp_out);
			fp_out = NULL;
		}
		free(buffer);
	}

	//Allocate memory
	buffer = (char *)malloc(lSize + 1);
	if (!buffer)
	{
		sprintf(msg, "Ошибка выделения памяти!\r\n");
		log->AppendText(gcnew String(msg));
		ret = 1;
		fclose(fp_i);
		if (fp_out != NULL)
		{
			fclose(fp_out);
			fp_out = NULL;
		}
		free(buffer);
	}

	//Read file contents into buffer
	fread(buffer, lSize, 1, fp_i);
	if (fp_i != NULL)
	{
		fclose(fp_i);
		fp_i = NULL;
	}

	//Do what ever with buffer

	block_rw = 4;

	//memset(buff, 0, sizeof(buff));
	//buff_len = fread(buff, 1, block_rw, fp_i);
	memcpy(&block, buffer + buff_ptr, 4); //ID блока
	buff_ptr += 4;
	memcpy(&blockSize, buffer + buff_ptr, 4); //размер блока
	buff_ptr += 4;

	if (block != 0x7777)
	{
		sprintf(msg, "Ошибка! файла не соответсвует нужной структуре.\r\n");
		log->AppendText(gcnew String(msg));
		ret = 1;
		if (fp_out != NULL)
		{
			fclose(fp_out);
			fp_out = NULL;
		}
		free(buffer);
	}

	fprintf(fp_out, "HEADER: 0x%08X\n\n", block);

	if ((blockSize + 8) != lSize)
	{
		sprintf(msg, "Ошибка! Размер данных не совпадает с размером файла.\r\n");
		log->AppendText(gcnew String(msg));
		ret = 1;
		if (fp_out != NULL)
		{
			fclose(fp_out);
			fp_out = NULL;
		}
		free(buffer);
	}

	//начинаем читать блоки пока не закончится буфер
	while (buff_ptr < lSize)
	{
		//DEBUG_L("\n===\nCDR: # %d\n" ,block_num);


		memcpy(&block, buffer + buff_ptr, 4); //ID блока
		buff_ptr += 4;
		memcpy(&blockSize, buffer + buff_ptr, 4); //размер блока
		buff_ptr += 4;

		//            DEBUG_L("%02x %02x\n" ,buffer[buff_ptr],buffer[buff_ptr+1]);
		//                     1         2          3       4
		fprintf(fp_out, "block %d\nID: %08X размер: %d (0x%02X) / (0x%08x)\n",
			block_num + 1,  //1
			block,        //2
			blockSize,    //3
			blockSize,    //4
			buff_ptr      //5
		);
		block_num++;

		/*if ((blockSize-buff_ptr)>16)
		int_32=16;
		else
		int_32=blockSize-buff_ptr;

		if (int_32)
		{
		fprintf(fp_out,"DUMP: ",);
		for (int i = 0;  i < int_32;  ++i) {fprintf(fp_out,"%02X ",buffer[buff_ptr+i]);}
		fprintf(fp_out,"\n");
		}*/

		switch (block)
		{
		case 0x0903: fprintf(fp_out, "[Object Type Block]\n");

			memcpy(&int_32, buffer + buff_ptr, 4); //int
			fprintf(fp_out, "model type = 0x%08X\n", int_32);

			buff_ptr += blockSize;
			break;
		case 0x0907: fprintf(fp_out, "[Materials Block]\n");

			//start_ptr=buff_ptr;

			memcpy(&int_32, buffer + buff_ptr, 4); //int
			fprintf(fp_out, "materials count = %d [0x%02X]\n\n", int_32, int_32);
			buff_ptr += 4;


			for (int i = 1; i <= int_32; ++i)
			{
				u_32 int_tmp;
				null_ptr = searchEndString(buffer, buff_ptr, lSize);
				memcpy(&msg, buffer + buff_ptr, null_ptr + 1); //копируем строку
				fprintf(fp_out, "material №%d name = %s\n", i, msg);
				buff_ptr += null_ptr + 1;

				null_ptr = searchEndString(buffer, buff_ptr, lSize);
				memcpy(&msg, buffer + buff_ptr, null_ptr + 1); //копируем строку
				fprintf(fp_out, "engine shader = %s\n", msg);
				buff_ptr += null_ptr + 1;

				null_ptr = searchEndString(buffer, buff_ptr, lSize);
				memcpy(&msg, buffer + buff_ptr, null_ptr + 1); //копируем строку
				fprintf(fp_out, "compiler shader = %s\n", msg);
				buff_ptr += null_ptr + 1;

				null_ptr = searchEndString(buffer, buff_ptr, lSize);
				memcpy(&msg, buffer + buff_ptr, null_ptr + 1); //копируем строку
				fprintf(fp_out, "game material = %s\n", msg);
				buff_ptr += null_ptr + 1;

				null_ptr = searchEndString(buffer, buff_ptr, lSize);
				memcpy(&msg, buffer + buff_ptr, null_ptr + 1); //копируем строку
				fprintf(fp_out, "texture path = %s\n", msg);
				buff_ptr += null_ptr + 1;

				null_ptr = searchEndString(buffer, buff_ptr, lSize);
				memcpy(&msg, buffer + buff_ptr, null_ptr + 1); //копируем строку
				fprintf(fp_out, "texture = %s\n", msg);
				buff_ptr += null_ptr + 1;

				memcpy(&int_tmp, buffer + buff_ptr, 4); //int
				fprintf(fp_out, "flags (2 sided, etc) = 0x%08X\n", int_tmp);
				buff_ptr += 4;

				memcpy(&int_tmp, buffer + buff_ptr, 4); //int
				fprintf(fp_out, "unknown data1 = 0x%08X\n", int_tmp);
				buff_ptr += 4;
				memcpy(&int_tmp, buffer + buff_ptr, 4); //int
				fprintf(fp_out, "unknown data2 = 0x%08X\n\n", int_tmp);
				buff_ptr += 4;

			}

			//buff_ptr=start_ptr+blockSize;
			break;
		case 0x0912: fprintf(fp_out, "[UserData Block]\n");

			null_ptr = searchEndString(buffer, buff_ptr, lSize);
			memcpy(&msg, buffer + buff_ptr, null_ptr + 1); //копируем строку
			fprintf(fp_out, "user data = %s\n", msg);
			buff_ptr += null_ptr + 1;

			break;
		case 0x0910: fprintf(fp_out, "[Geometry Block]\n");
			fprintf(fp_out, "geometry\n");
			//start_ptr=buff_ptr;

			buff_ptr += blockSize;
			break;
		case 0x0922: fprintf(fp_out, "Author Block \n");

			null_ptr = searchEndString(buffer, buff_ptr, lSize);
			memcpy(&msg, buffer + buff_ptr, null_ptr + 1); //копируем строку
			fprintf(fp_out, "author name = %s\n", msg);
			buff_ptr += null_ptr + 1;

			//memcpy(&int_32,buffer+buff_ptr,4); //Date
			memcpy(&log_time, buffer + buff_ptr, 4); //Date
			tml = gmtime(&log_time);
			//fprintf(fp_out,"date = 0x%08X\n",int_32);
			fprintf(fp_out, "date =  %04d.%02d.%02d %02d:%02d:%02d\r\n", 1900 + tml->tm_year, 1 + tml->tm_mon, tml->tm_mday, tml->tm_hour, tml->tm_min, tml->tm_sec);
			buff_ptr += 4; //date

			null_ptr = searchEndString(buffer, buff_ptr, lSize);
			memcpy(&msg, buffer + buff_ptr, null_ptr + 1); //копируем строку
			fprintf(fp_out, "modifier name = %s\n", msg);
			buff_ptr += null_ptr + 1;

			memcpy(&int_32, buffer + buff_ptr, 4); //Date
			memcpy(&log_time, buffer + buff_ptr, 4); //Date
			tml = gmtime(&log_time);
			//fprintf(fp_out,"date = 0x%08X\n",int_32);
			fprintf(fp_out, "date = %04d.%02d.%02d %02d:%02d:%02d\r\n", 1900 + tml->tm_year, 1 + tml->tm_mon, tml->tm_mday, tml->tm_hour, tml->tm_min, tml->tm_sec);
			buff_ptr += 4; //date

						   //buff_ptr+=blockSize;
			break;

		case 0x0925: fprintf(fp_out, "[LOD Block]\n");

			null_ptr = searchEndString(buffer, buff_ptr, lSize);
			memcpy(&msg, buffer + buff_ptr, null_ptr + 1); //копируем строку
			fprintf(fp_out, "lod reference = %s\n", msg);

			buff_ptr += blockSize;
			break;

		default:     fprintf(fp_out, "[unknown data]\n");
			buff_ptr += blockSize;
			break;
		}

		fprintf(fp_out, "\n");

		/*		         	            int_32=8;
		if (int_32)
		{
		fprintf(fp_out,"DUMP: ");
		for (int i = 0;  i < int_32;  ++i) {fprintf(fp_out,"%02X ",buffer[buff_ptr+null_ptr+i]);}
		fprintf(fp_out,"\n\n");
		}
		*/

	}

	if (fp_out != NULL)
	{
		fclose(fp_out);
		fp_out = NULL;
	}
	free(buffer);

	sprintf(msg, "выходной файл : %s\r\n", out_file_name); log->AppendText(gcnew String(msg));
	sprintf(msg, "найдено блоков: %ld\r\n", block_num); log->AppendText(gcnew String(msg));

	return ret;
}

int ObjectDec::stlk_object_gmpatch(System::Windows::Forms::RichTextBox^ % log, const char *out_file_name, std::ofstream& Log, int mod)
{
	const char *in_file_name = this->file_name.c_str();
	if(!mod) // если флаг 0, для одиночной конвертации файлов(чистый *.object)
		rename(out_file_name, in_file_name);

	// in_file_name - имя файла для декомпиляции
	// out_file_name - выходное имя файла
	// _log - имя лог - файла

	char log_buf[2000];

	int ret;
	FILE *fp_i = NULL;
	FILE *fp_out = NULL;
	char *buffer;
	const int buff_MAX = 65536; //65536
	unsigned long block_rw;
	char buff[buff_MAX];
	unsigned int buff_len;
	//u_32                   int_32;
	u_32 mt_cnt;
	u_32 block = 0;
	u_32 blockSize = 0;
	int block_num = 0;
	char msg[500];
	char tmp_msg[100];
	unsigned long lSize; //size of inp file
	unsigned long lDataSize; //size of inp file
	unsigned long buff_ptr = 0;  //buffer offset
	unsigned long null_ptr = 0;  //length of string
	unsigned long MTBlock_ptr = 0;  //offset to [Materials Block] size
	unsigned int int_tmp;
	//        unsigned int           MTBlock_len=0;  //length of [Materials Block] (without material count)

	char engine_shader[MAX_e_shader];
	char compiler_shader[MAX_c_shader];
	char game_material[MAX_g_material];
	char texture_path[MAX_t_path];
	int  ini_srch = NULL;

	ret = 0;
	block_num = 0;
	buff_ptr = 0;
	//        MTBlock_len  = 0;

	/* open the input file for read */
	fp_i = fopen(in_file_name, "rb");
	if (fp_i == NULL)
	{
		sprintf(msg, "Ошибка открытия вх. файла %s \r\n", in_file_name);
		log->AppendText(gcnew String(msg));
		ret = 1;
		if (fp_i != NULL)
		{
			fclose(fp_i);
			fp_i = NULL;
		}
		if (fp_out != NULL)
		{
			fclose(fp_out);
			fp_out = NULL;
		}
		free(buffer);
	}

	/* open the output file for write */
	fp_out = fopen(out_file_name, "wb");
	if (fp_out == NULL)
	{
		sprintf(msg, "Ошибка открытия вых. файла %s \r\n", out_file_name);
		log->AppendText(gcnew String(msg));
		ret = 1;
		if (fp_i != NULL)
		{
			fclose(fp_i);
			fp_i = NULL;
		}
		if (fp_out != NULL)
		{
			fclose(fp_out);
			fp_out = NULL;
		}
		free(buffer);
	}

	fseek(fp_i, 0, SEEK_END);
	lSize = ftell(fp_i);
	fseek(fp_i, 0, SEEK_SET);
	//rewind (fp_i);


	sprintf(msg, "файл: %s\r\n", in_file_name);
	if (mod) log->AppendText(gcnew String(msg));

	sprintf(msg, "размер: %ld\r\n\r\n", lSize);
	if (mod) log->AppendText(gcnew String(msg));

	sprintf(log_buf, "файл: %s ; размер: %ld\r\n", in_file_name, lSize);
	if(Log)
Log << log_buf;
 log->AppendText(gcnew String(log_buf));

	if (lSize<12)
	{
		sprintf(msg, "Файл слишком маленький.\r\n");
		if (mod) log->AppendText(gcnew String(msg));
		sprintf(log_buf, "Файл слишком маленький.\r\n");
		if(Log)
Log << log_buf;
 log->AppendText(gcnew String(log_buf));

		ret = 1;
		fclose(fp_i);
		if (fp_i != NULL)
		{
			fclose(fp_i);
			fp_i = NULL;
		}
		if (fp_out != NULL)
		{
			fclose(fp_out);
			fp_out = NULL;
		}
		free(buffer);
	}

	//Allocate memory
	buffer = (char *)malloc(lSize + 1);
	if (!buffer)
	{
		sprintf(msg, "Ошибка выделения памяти!\r\n");
		log->AppendText(gcnew String(msg));
		sprintf(log_buf, "Ошибка выделения памяти!\r\n");
		if(Log)
Log << log_buf;
 log->AppendText(gcnew String(log_buf));

		ret = 1;
		fclose(fp_i);
		if (fp_i != NULL)
		{
			fclose(fp_i);
			fp_i = NULL;
		}
		if (fp_out != NULL)
		{
			fclose(fp_out);
			fp_out = NULL;
		}
		free(buffer);
	}

	//Read file contents into buffer
	fread(buffer, lSize, 1, fp_i);

	//Do what ever with buffer

	block_rw = 4;

	//memset(buff, 0, sizeof(buff));
	//buff_len = fread(buff, 1, block_rw, fp_i);
	memcpy(&block, buffer + buff_ptr, 4); //ID блока
	buff_ptr += 4;
	memcpy(&blockSize, buffer + buff_ptr, 4); //размер блока
	buff_ptr += 4;

	if (block != 0x7777)
	{
		sprintf(msg, "Ошибка! файла не соответсвует нужной структуре.\r\n");
		log->AppendText(gcnew String(msg));
		sprintf(log_buf,  "Ошибка! файла не соответсвует нужной структуре.\r\n");
		if(Log)
Log << log_buf;
 log->AppendText(gcnew String(log_buf));

		ret = 1;
		if (fp_i != NULL)
		{
			fclose(fp_i);
			fp_i = NULL;
		}
		if (fp_out != NULL)
		{
			fclose(fp_out);
			fp_out = NULL;
		}
		free(buffer);
	}

	if ((blockSize + 8) != lSize)
	{
		sprintf(msg, "Ошибка! Размер данных не совпадает с размером файла.\r\n");
		log->AppendText(gcnew String(msg));
		sprintf(log_buf, "Ошибка! Размер данных не совпадает с размером файла.\r\n");
		if(Log)
Log << log_buf;
 log->AppendText(gcnew String(log_buf));

		ret = 1;
		if (fp_i != NULL)
		{
			fclose(fp_i);
			fp_i = NULL;
		}
		if (fp_out != NULL)
		{
			fclose(fp_out);
			fp_out = NULL;
		}
		free(buffer);
	}
	lDataSize = blockSize;

	block_rw = fwrite(buffer, 1, buff_ptr, fp_out); //пишем в выходной файл

													//начинаем читать блоки пока не закончится буфер
	while (buff_ptr < lSize)
	{
		//DEBUG_L("\n===\nCDR: # %d\n" ,block_num);


		memcpy(&block, buffer + buff_ptr, 4); //ID блока
		buff_ptr += 4;
		memcpy(&blockSize, buffer + buff_ptr, 4); //размер блока
		buff_ptr += 4;

		//            DEBUG_L("%02x %02x\n" ,buffer[buff_ptr],buffer[buff_ptr+1]);
		//                     1         2          3       4
		sprintf(msg, "block №%d ID: %08X размер: %d (0x%02X)\r\n",
			block_num + 1,  //1
			block,        //2
			blockSize,    //3
			blockSize     //4
		);
		if (mod) log->AppendText(gcnew String(msg));
		block_num++;

		switch (block)
		{
		case 0x0903: 
			sprintf(log_buf, "[Object Type Block] %s", msg);
			if(Log)
Log << log_buf;
 log->AppendText(gcnew String(log_buf));

			sprintf(msg, "[Object Type Block]\r\n\0"); if (mod) log->AppendText(gcnew String(msg));
			block_rw = fwrite(buffer + buff_ptr - 8, 1, blockSize + 8, fp_out); //пишем в выходной файл
			buff_ptr += blockSize;
			break;
		case 0x0907:  
			sprintf(log_buf, "[Materials Block] %s", msg);
			if(Log)
Log << log_buf;
 log->AppendText(gcnew String(log_buf));

			sprintf(msg, "[Materials Block]\r\n\0"); if (mod) log->AppendText(gcnew String(msg));


			//здесь начинаем писать в оригинал все что нашли до этого

			MTBlock_ptr = buff_ptr; //ссылка на [Materials Block] count

			block_rw = fwrite(buffer + buff_ptr - 8, 1, 4, fp_out); //пишем первые 4 байта ID [Materials Block]
			if (block_rw != 4) { sprintf(msg, "Ошибка записи [%ld]байт в выходной файл , нужно [%d]\0", buff_ptr, block_rw); if (mod) log->AppendText(gcnew String(msg)); }

			buff_len = 0;
			memset(buff, 0, sizeof(buff));
			memcpy(&buff, buffer + buff_ptr - 4, 8); //[Materials Block] size + count
			buff_len += 8;
			//sprintf(msg,"buflen=%ld [0x%02X%02X%02X%02X%02X%02X%02X%02X]",buff_len,buff[0],buff[1],buff[2],buff[3],buff[4],buff[5],buff[6],buff[7]); if (mod) log->AppendText(gcnew String(msg));

			memcpy(&mt_cnt, buffer + buff_ptr, 4); //int
			sprintf(msg, "materials count = %d [0x%02X]\n\n", mt_cnt, mt_cnt); if (mod) log->AppendText(gcnew String(msg));
			buff_ptr += 4;


			if (mt_cnt*(MAX_t_path + MAX_e_shader + MAX_c_shader + MAX_g_material + 4)>buff_MAX)//упс памяти маловато будет
			{
				sprintf(msg, "Ошибка! Нехватка памяти буфера. L:%d.\r\n", __LINE__);
				log->AppendText(gcnew String(msg));
				sprintf(log_buf, "Ошибка! Нехватка памяти буфера для обработки данных. L:%d.\r\n", __LINE__);
				if(Log)
Log << log_buf;
 log->AppendText(gcnew String(log_buf));

				ret = 1;
				if (fp_i != NULL)
				{
					fclose(fp_i);
					fp_i = NULL;
				}
				if (fp_out != NULL)
				{
					fclose(fp_out);
					fp_out = NULL;
				}
				free(buffer);
			}

			{


				for (int i = 1; i <= mt_cnt; ++i)
				{ //u_32 int_tmp;

					null_ptr = searchEndString(buffer, buff_ptr, lSize);
					memset(msg, 0, sizeof(msg));
					if (null_ptr + 1 > sizeof(msg))
					{
						sprintf(msg, "Параметр длинее переменной! L:%d\n", __LINE__); log->AppendText(gcnew String(msg));
						sprintf(log_buf, "%ы\r\n", msg);
						if(Log)
Log << log_buf;
 log->AppendText(gcnew String(log_buf));

						ret = 1;
						if (fp_i != NULL)
						{
							fclose(fp_i);
							fp_i = NULL;
						}
						if (fp_out != NULL)
						{
							fclose(fp_out);
							fp_out = NULL;
						}
						free(buffer);
					}
					memcpy(&msg, buffer + buff_ptr, null_ptr + 1); //копируем строку
																   //fprintf(fp_out,"material №%d name = %s\n",i,msg); if (mod) log->AppendText(gcnew String(msg));
																   //memcpy(&buff+buff_len,buffer+buff_ptr,null_ptr+1);
					strncpy(buff + buff_len, msg, null_ptr + 1); //material № name
					buff_len += null_ptr + 1;
					buff_ptr += null_ptr + 1;

					null_ptr = searchEndString(buffer, buff_ptr, lSize);
					memset(engine_shader, 0, sizeof(engine_shader));
					if (null_ptr + 1 > sizeof(engine_shader)) 
					{
						sprintf(msg, "Параметр длинее переменной! L:%d\n", __LINE__); log->AppendText(gcnew String(msg));  
						sprintf(log_buf, "%ы\r\n", msg);
						if(Log)
Log << log_buf;
 log->AppendText(gcnew String(log_buf));
						ret = 1;
					if (fp_i != NULL)
					{
						fclose(fp_i);
						fp_i = NULL;
					}
					if (fp_out != NULL)
					{
						fclose(fp_out);
						fp_out = NULL;
					}
					free(buffer);
					}
					memcpy(&engine_shader, buffer + buff_ptr, null_ptr + 1); //копируем строку  //engine shader
																			 //fprintf(fp_out,"engine shader = %s\n",msg); if (mod) log->AppendText(gcnew String(msg));
					buff_ptr += null_ptr + 1;

					null_ptr = searchEndString(buffer, buff_ptr, lSize);
					memset(compiler_shader, 0, sizeof(compiler_shader));
					if (null_ptr + 1 > sizeof(compiler_shader))
					{
						sprintf(msg, "Параметр длинее переменной! L:%d\n", __LINE__); log->AppendText(gcnew String(msg)); 
						sprintf(log_buf, "%ы\r\n", msg);  
						if(Log)
Log << log_buf;
 log->AppendText(gcnew String(log_buf));
						ret = 1;
						
						if (fp_i != NULL)
						{
							fclose(fp_i);
							fp_i = NULL;
						}
						if (fp_out != NULL)
						{
							fclose(fp_out);
							fp_out = NULL;
						}
						free(buffer);
					}
					memcpy(&compiler_shader, buffer + buff_ptr, null_ptr + 1); //копируем строку //compiler shader
																			   //fprintf(fp_out,"compiler shader = %s\n",msg); if (mod) log->AppendText(gcnew String(msg));
					buff_ptr += null_ptr + 1;

					null_ptr = searchEndString(buffer, buff_ptr, lSize);
					memset(game_material, 0, sizeof(game_material));
					if (null_ptr + 1 > sizeof(game_material)) 
					{ 
						sprintf(msg, "Параметр длинее переменной! L:%d\n", __LINE__);
						log->AppendText(gcnew String(msg));  
						sprintf(log_buf, "%ы\r\n", msg);
						if(Log)
Log << log_buf;
 log->AppendText(gcnew String(log_buf));

						ret = 1; 
						if (fp_i != NULL)
						{
							fclose(fp_i);
							fp_i = NULL;
						}
						if (fp_out != NULL)
						{
							fclose(fp_out);
							fp_out = NULL;
						}
						free(buffer);
					}
					memcpy(&game_material, buffer + buff_ptr, null_ptr + 1); //копируем строку //game material
																			 //fprintf(fp_out,"game material = %s\n",msg); if (mod) log->AppendText(gcnew String(msg));
					buff_ptr += null_ptr + 1;

					null_ptr = searchEndString(buffer, buff_ptr, lSize);
					memset(texture_path, 0, sizeof(texture_path));
					if (null_ptr + 1 > sizeof(texture_path))
					{ 
						sprintf(msg, "Параметр длинее переменной! L:%d\n", __LINE__); 
						log->AppendText(gcnew String(msg)); 
						sprintf(log_buf, "%ы\r\n", msg); 
						if(Log)
Log << log_buf;
 log->AppendText(gcnew String(log_buf));
						ret = 1;
						if (fp_i != NULL)
						{
							fclose(fp_i);
							fp_i = NULL;
						}
						if (fp_out != NULL)
						{
							fclose(fp_out);
							fp_out = NULL;
						}
						free(buffer);
					}
					memcpy(&texture_path, buffer + buff_ptr, null_ptr + 1); //копируем строку //texture path
																			//fprintf(fp_out,"texture path = %s\n",msg); if (mod) log->AppendText(gcnew String(msg));
					buff_ptr += null_ptr + 1;

					if ((!strncmp(game_material, "materials\\", 10)) || (!strncmp(game_material, "materials/", 10)))
					{
						sprintf(log_buf, "%2d !#!#!# game_material = %s \r\n", i, game_material);
						if(Log)
Log << log_buf;
 log->AppendText(gcnew String(log_buf));

						//strncpy(buff+buff_len,engine_shader,strlen(engine_shader)+1); //engine shader
						//buff_len+=strlen(engine_shader)+1;
						//strncpy(buff+buff_len,compiler_shader,strlen(compiler_shader)+1); //compiler shader
						//buff_len+=strlen(compiler_shader)+1;
						//strncpy(buff+buff_len,game_material,strlen(game_material)+1); //game material
						//buff_len+=strlen(game_material)+1;
						//sprintf(log_buf, "%2d !!!### game_material = materials\\ , ничего не меняем\r\n",i);
					}
					//else
					//{
					memset(tmp_msg, 0, sizeof(tmp_msg));
					strcpy(tmp_msg, texture_path);
					StrTOLower(tmp_msg);
					ini_srch = ininame_search(tmp_msg);
					if (ini_srch == NULL) //не нашли по полному пути, ищем по начальному "каталогу"
						for (int j = 1; j < strlen(tmp_msg); j++)
						{
							if (tmp_msg[j] == '\\')
							{
								tmp_msg[j + 1] = 0x00;
								break;
							}
						}

					//ini_srch=ininame_search(tmp_msg) !!!!!!!!!!!!!!!!!!!!!!!!!!!
					ini_srch = ininame_search(tmp_msg);
					if (ini_srch == NULL)
					{
						sprintf(log_buf, "%2d !!!### texture path = [%s] не найден в ini\r\n", i, tmp_msg);
						if(Log)
Log << log_buf;
 log->AppendText(gcnew String(log_buf));

						strncpy(buff + buff_len, "default\0", 8); ///engine shader
						buff_len += 8;
						strncpy(buff + buff_len, "default\0", 8); //compiler shader
						buff_len += 8;
						strncpy(buff + buff_len, "default\0", 8); //game material
						buff_len += 8;
					}
					else
					{
						strncpy(buff + buff_len, ini_sting[ini_srch].e_shader, strlen(ini_sting[ini_srch].e_shader) + 1); //engine shader
						buff_len += strlen(ini_sting[ini_srch].e_shader) + 1;

						strncpy(buff + buff_len, ini_sting[ini_srch].c_shader, strlen(ini_sting[ini_srch].c_shader) + 1); //compiler shader
						buff_len += strlen(ini_sting[ini_srch].c_shader) + 1;

						strncpy(buff + buff_len, ini_sting[ini_srch].g_material, strlen(ini_sting[ini_srch].g_material) + 1); //game material
						buff_len += strlen(ini_sting[ini_srch].g_material) + 1;

					}

					//}

					strncpy(buff + buff_len, texture_path, strlen(texture_path) + 1); //texture path
					buff_len += strlen(texture_path) + 1;

					//sprintf(msg,"tmp_msg=[%s] i:[%d]",tmp_msg,int_tmp); if (mod) log->AppendText(gcnew String(msg));

					null_ptr = searchEndString(buffer, buff_ptr, lSize);
					memcpy(&msg, buffer + buff_ptr, null_ptr + 1); //копируем строку
																   //fprintf(fp_out,"texture = %s\n",msg); if (mod) log->AppendText(gcnew String(msg));
					strncpy(buff + buff_len, msg, null_ptr + 1); //(Texture = 8)
					buff_len += null_ptr + 1;
					buff_ptr += null_ptr + 1;

					memcpy(&int_tmp, buffer + buff_ptr, 4); //int flags
					if ((int_tmp != 0) && (int_tmp != 1))
					{
						sprintf(log_buf, "%2d ###!#!#!# flags = [%d] (0x%08X) !\r\n", i, int_tmp, int_tmp);
						if(Log)
Log << log_buf;
 log->AppendText(gcnew String(log_buf));
					}

					sprintf(log_buf, "%3d  %s =", i, texture_path);
					if(Log)
Log << log_buf;
 log->AppendText(gcnew String(log_buf));

					sprintf(log_buf, " %s ; %s ; %s ; 0x%08X\r\n", engine_shader, compiler_shader, game_material, int_tmp);
					if(Log)
Log << log_buf;
 log->AppendText(gcnew String(log_buf));

					if (ini_srch != NULL)
					{
						sprintf(log_buf, "  => %s /%d/ = %s ; %s ; %s ; 0x%08X\r\n", texture_path,
							ini_srch,
							ini_sting[ini_srch].e_shader,
							ini_sting[ini_srch].c_shader,
							ini_sting[ini_srch].g_material,
							ini_sting[ini_srch].flags);
						if(Log)
Log << log_buf;
 log->AppendText(gcnew String(log_buf));
					}
					else
					{
						sprintf(log_buf, " => %s = default ; default ; default ;\r\n", texture_path);
						if(Log)
Log << log_buf;
 log->AppendText(gcnew String(log_buf));
					}
					//fprintf(fp_out,"flags (2 sided, etc) = 0x%08X\n",int_tmp); if (mod) log->AppendText(gcnew String(msg));
					if ((ini_srch != NULL) && (int_tmp != ini_sting[ini_srch].flags))
						memcpy(buff + buff_len, &ini_sting[ini_srch].flags, 4);
					else
						strncpy(buff + buff_len, buffer + buff_ptr, 4); //flags (4)
					buff_len += 4;
					buff_ptr += 4;

					//memcpy(&int_tmp,buffer+buff_ptr,4); //int
					//fprintf(fp_out,"unknown data1 = 0x%08X\n",int_tmp); if (mod) log->AppendText(gcnew String(msg));
					strncpy(buff + buff_len, buffer + buff_ptr, 4); //unknown data1
					buff_len += 4;
					buff_ptr += 4;

					//memcpy(&int_tmp,buffer+buff_ptr,4); //int
					//fprintf(fp_out,"unknown data2 = 0x%08X\n\n",int_tmp); if (mod) log->AppendText(gcnew String(msg));
					strncpy(buff + buff_len, buffer + buff_ptr, 4); //unknown data2
					buff_len += 4;
					buff_ptr += 4;

					if (buff_len + (MAX_t_path + MAX_e_shader + MAX_c_shader + MAX_g_material + 4)>buff_MAX)//упс памяти маловато будет
					{
						sprintf(msg, "Ошибка! Нехватка буфера памяти L:%d.\r\n", __LINE__);
						log->AppendText(gcnew String(msg));
						sprintf(log_buf, "Ошибка! Нехватка памяти буфера для обработки данных. L:%d.\r\n", __LINE__);
						if(Log)
Log << log_buf;
 log->AppendText(gcnew String(log_buf));

						ret = 1;
						if (fp_i != NULL)
						{
							fclose(fp_i);
							fp_i = NULL;
						}
						if (fp_out != NULL)
						{
							fclose(fp_out);
							fp_out = NULL;
						}
						free(buffer);
					}

				}

				//MTBlock_len+=buff_len;
				buff_len -= 4;
				//sprintf(msg,"buflen=%ld [0x%02X%02X%02X%02X%02X%02X%02X%02X]",buff_len,buff[0],buff[1],buff[2],buff[3],buff[4],buff[5],buff[6],buff[7]); log->AppendText(gcnew String(msg));
				memcpy(buff, &buff_len, 4); //обновляем размер буфера
											//sprintf(msg,"buflen=%ld [0x%02X%02X%02X%02X%02X%02X%02X%02X]",buff_len,buff[0],buff[1],buff[2],buff[3],buff[4],buff[5],buff[6],buff[7]); log->AppendText(gcnew String(msg));
				buff_len += 4;

				block_rw = fwrite(buff, 1, buff_len, fp_out); //пишем буфер с обновленным [Materials Block]
				if (block_rw != buff_len) { sprintf(msg, "Ошибка записи [%d]байт в выходной файл , нужно [%d]\0", buff_len, block_rw); if (mod) log->AppendText(gcnew String(msg)); }
				else { sprintf(msg, "[Materials Block] новый размер:  %d (0x%02X).\0", buff_len - 4, buff_len - 4); if (mod) log->AppendText(gcnew String(msg)); }


				if ((MTBlock_ptr + blockSize) != buff_ptr) { sprintf(msg, "Ошибка расчета в блоке in[%ld] out[%ld]\0", MTBlock_ptr + blockSize, buff_ptr); if (mod) log->AppendText(gcnew String(msg)); }

			}

			//добить до конца
			//block_rw=fwrite(buffer+buff_ptr, 1, lSize-buff_ptr, fp_out);

			/* нельзя ползать по записываемуому файлу
			//fseek(fp_out , MTBlock_ptr ,SEEK_SET);
			//block_rw=fwrite(&MTBlock_len, 1, 4, fp_out);
			//if (block_rw!=4) {sprintf(msg,"Ошибка записи в выходной файл \0"); log->AppendText(gcnew String(msg));}
			//fseek(fp_out , 0 , SEEK_END);
			*/

			lDataSize += buff_len - 4 - blockSize; //пересчитали заново размер всех данных

			break;
		case 0x0912:  
			sprintf(log_buf, "[UserData Block] %s", msg);
			if(Log)
Log << log_buf;
 log->AppendText(gcnew String(log_buf));

			sprintf(msg, "[UserData Block]\r\n\0"); if (mod) log->AppendText(gcnew String(msg));

			block_rw = fwrite(buffer + buff_ptr - 8, 1, blockSize + 8, fp_out); //пишем в выходной файл
			buff_ptr += blockSize;
			break;
		case 0x0910:  
			sprintf(log_buf, "[Geometry Block] %s", msg);
			if(Log)
Log << log_buf;
 log->AppendText(gcnew String(log_buf));

			sprintf(msg, "[Geometry Block]\r\n\0"); if (mod) log->AppendText(gcnew String(msg));


			block_rw = fwrite(buffer + buff_ptr - 8, 1, blockSize + 8, fp_out); //пишем в выходной файл
			buff_ptr += blockSize;
			break;
		case 0x0922:  
			sprintf(log_buf, "[Author Block] %s", msg);
			if(Log)
Log << log_buf;
 log->AppendText(gcnew String(log_buf));

			int_tmp = 0;
			sprintf(msg, "[Author Block]\r\n\0"); if (mod) log->AppendText(gcnew String(msg));

			//block_rw=fwrite(buffer+buff_ptr-8, 1, 4, fp_out); //пишем первые 4 байта ID [Author Block]
			buff_len = 0;
			memset(buff, 0, sizeof(buff));

			null_ptr = searchEndString(buffer, buff_ptr, lSize);
			memcpy(&buff, buffer + buff_ptr - 8, null_ptr + 13); //копируем ID + size + строку создателя вместе с датой
																 //fprintf(fp_out,"author name = %s\n",msg);
			buff_len += null_ptr + 13;
			int_tmp += null_ptr + 5;

			null_ptr = searchEndString(buffer, buff_ptr + int_tmp, lSize);
			int_tmp += null_ptr + 5;
			if (blockSize != int_tmp)
			{
				sprintf(msg, "Ошибка определения длинны [Author Block]%d / %d\r\n", blockSize, int_tmp);
				if (mod) log->AppendText(gcnew String(msg));
			}

			memset(msg, 0, sizeof(msg));
			sprintf(msg, "L.W.C.team\0");
			int_tmp = strlen(msg);
			strncpy(buff + buff_len, msg, int_tmp + 1); //flags (4)
			buff_len += int_tmp + 1;
			time(&log_time);
			memcpy(buff + buff_len, &log_time, 4); //insert date
			buff_len += 4;

			buff_len -= 8;
			memcpy(buff + 4, &buff_len, 4); //обновляем размер блока данных

			block_rw = fwrite(buff, 1, buff_len + 8, fp_out); //пишем в выходной файл

															  //int_tmp+=buff_len-null_ptr;
			lDataSize += buff_len - blockSize; //пересчитали заново размер всех данных


											   //block_rw=fwrite(buffer+buff_ptr-8, 1, blockSize+8, fp_out); //пишем в выходной файл
			buff_ptr += blockSize;
			break;

		case 0x0925: 
			sprintf(log_buf, "[LOD Block] %s", msg);
			if(Log)
Log << log_buf;
 log->AppendText(gcnew String(log_buf));

			sprintf(msg, "[LOD Block]\r\n\0"); if (mod) log->AppendText(gcnew String(msg));


			block_rw = fwrite(buffer + buff_ptr - 8, 1, blockSize + 8, fp_out); //пишем в выходной файл
			buff_ptr += blockSize;
			break;

		default:    
			sprintf(log_buf, "[unknown Block] %s", msg);
			if(Log)
Log << log_buf;
 log->AppendText(gcnew String(log_buf));

			sprintf(msg, "[unknown data]\r\n\0"); if (mod) log->AppendText(gcnew String(msg));


			block_rw = fwrite(buffer + buff_ptr - 8, 1, blockSize + 8, fp_out); //пишем в выходной файл
			buff_ptr += blockSize;
			break;
		}

		sprintf(msg, "\r\n\0"); if (mod) log->AppendText(gcnew String(msg));
		sprintf(log_buf, "\r\n", msg);
		if(Log)
Log << log_buf;
 log->AppendText(gcnew String(log_buf));


	}

	fclose(fp_out);
	fp_out = fopen(out_file_name, "rb+");
	if (fp_out == NULL)
	{
		sprintf(msg, "Ошибка переоткрытия файла %s \r\n", out_file_name);  if (mod) log->AppendText(gcnew String(msg));
	}
	else
	{
		block_rw = fseek(fp_out, 4, SEEK_SET);
		block_rw = fwrite(&lDataSize, 1, 4, fp_out);
		block_rw = fseek(fp_out, 0, SEEK_END);
	}

	if (fp_i != NULL)
	{
		fclose(fp_i);
		fp_i = NULL;
	}
	if (fp_out != NULL)
	{
		fclose(fp_out);
		fp_out = NULL;
	}
	free(buffer);
	sprintf(msg, "найдено блоков: %ld\r\n\0", block_num); if (mod) log->AppendText(gcnew String(msg));

	return ret;
}

void ObjectDec::scandir(System::Windows::Forms::RichTextBox^ % log, char* _log, char *dir, char *outdir)
{
	char fname[MAX_PATH];
	char oname[MAX_PATH];
	char path[MAX_PATH];
	char msg[256];

	std::ofstream l;
	if (_log && strcmp(_log, ""))
		l.open(_log);

	//int spaces = depth*2;

	sprintf(msg, "Входной каталог: %s\n", dir);  log->AppendText(gcnew String(msg));
	sprintf(msg, "Выходной каталог %s\r\n\r\n", outdir); log->AppendText(gcnew String(msg));
	sprintf(msg, "Входной каталог: %s\r\n Выходной каталог %s\r\n\r\n", dir, outdir);
	if (l) l << msg;

	String^ _dir = gcnew String(dir);

	if (!System::IO::Directory::Exists(gcnew String(_dir)))
	{
		sprintf(msg, "cannot open directory: %s\n", dir);
		if (l) l << msg;
		return;
	}
	System::IO::Directory::SetCurrentDirectory(_dir);
	for each(auto % c in System::IO::Directory::GetFiles(_dir))
	{
		if (c == "." || c == "..") // ссылки на предыдущий и следующий каталоги
			continue;

		std::string _name = Lines::systemToStdString(c);
		std::string _path = Lines::systemToStdString(System::IO::Directory::GetCurrentDirectory());
		sprintf(fname, "%s", _name.c_str());
		if (c->Substring(c->LastIndexOf(".") != -1 ? c->LastIndexOf(".") + 1 : 0) == "object")
		{
			sprintf(oname, "%s\\%s", outdir, _name.substr(_name.find_last_of("\\") != -1 ?
				_name.find_last_of("\\") + 1 : 0).c_str());
			sprintf(msg, "[%s] => [%s]\r\n", fname, oname);
			log->AppendText(gcnew String(msg));
			sprintf(msg, "[%s] => [%s]\r\n", fname, oname);
			if (l) l << msg;
			this->file_name = fname;
			this->stlk_object_gmpatch(log, oname, l, 1);
			if (l) l << msg << "\r\n";
		}

	}
	System::IO::Directory::SetCurrentDirectory("..");
}


// static

int ObjectDec::ini_parse(void * user)
{
	FILE* file;
	int error;

	file = fopen(ObjectDec::inifile, "r");
	if (!file)
		return -1;
	error = ObjectDec::ini_parse_file(file, ObjectDec::iniTOarray, user);
	fclose(file);
	return error;
}

int ObjectDec::ini_parse_file(FILE* file, int(*handler)(void*, const char*, const char*,const char*), void* user)
{
	/* Uses a fair bit of stack (use heap instead if you need to) */

	// DEFINE CONST!!!!!

#if INI_USE_STACK
	char line[INI_MAX_LINE];
#else
	char* line;
#endif
	char section[MAX_SECTION] = "";
	char prev_name[MAX_NAME] = "";

	char* start;
	char* end;
	char* name;
	char* value;
	int lineno = 0;
	int error = 0;

#if !INI_USE_STACK
	line = (char*)malloc(INI_MAX_LINE);
	if (!line) {
		return -2;
	}
#endif

	/* Scan through file line by line */
	while (fgets(line, INI_MAX_LINE, file) != NULL) {
		lineno++;

		start = line;
#if INI_ALLOW_BOM
		if (lineno == 1 && (unsigned char)start[0] == 0xEF &&
			(unsigned char)start[1] == 0xBB &&
			(unsigned char)start[2] == 0xBF) {
			start += 3;
		}
#endif
		start = ObjectDec::lskip(ObjectDec::rstrip(start));

		if (*start == ';' || *start == '#') {
			/* Per Python ConfigParser, allow '#' comments at start of line */
		}
#if INI_ALLOW_MULTILINE
		else if (*prev_name && *start && start > line) {
			/* Non-black line with leading whitespace, treat as continuation
			of previous name's value (as per Python ConfigParser). */
			if (!handler(user, section, prev_name, start) && !error)
				error = lineno;
		}
#endif
		else if (*start == '[') {
			/* A "[section]" line */
			end = find_char_or_comment(start + 1, ']');
			if (*end == ']') {
				*end = '\0';
				strncpy0(section, start + 1, sizeof(section));
				*prev_name = '\0';
			}
			else if (!error) {
				/* No ']' found on section line */
				error = lineno;
			}
		}
		else if (*start && *start != ';') {
			/* Not a comment, must be a name[=:]value pair */
			end = find_char_or_comment(start, '=');
			if (*end != '=') {
				end = find_char_or_comment(start, ':');
			}
			if (*end == '=' || *end == ':') {
				*end = '\0';
				name = rstrip(start);
				value = lskip(end + 1);
				end = find_char_or_comment(value, '\0');
				if (*end == ';')
					*end = '\0';
				rstrip(value);

				/* Valid name[=:]value pair found, call handler */
				strncpy0(prev_name, name, sizeof(prev_name));
				if (!handler(user, section, name, value) && !error)
					error = lineno;
			}
			else if (!error) {
				/* No '=' or ':' found on name[=:]value line */
				error = lineno;
			}
		}

#if INI_STOP_ON_FIRST_ERROR
		if (error)
			break;
#endif
	}

#if !INI_USE_STACK
	free(line);
#endif

	return error;
}

char* ObjectDec::lskip(const char* s)
{
	while (*s && isspace((unsigned char)(*s)))
		s++;
	return (char*)s;
}

char* ObjectDec::rstrip(char* s)
{
	char* p = s + strlen(s);
	while (p > s && isspace((unsigned char)(*--p)))
		*p = '\0';
	return s;
}

char* ObjectDec::find_char_or_comment(const char* s, char c)
{
	int was_whitespace = 0;
	while (*s && *s != c && !(was_whitespace && *s == ';')) {
		was_whitespace = isspace((unsigned char)(*s));
		s++;
	}
	return (char*)s;
}

char* ObjectDec::strncpy0(char* dest, const char* src, size_t size)
{
	strncpy(dest, src, size);
	dest[size - 1] = '\0';
	return dest;
}

int ObjectDec::iniTOarray(void* user, const char* section, const char* name, const char* value)
{
	//static char prev_section[50] = "";
	char        msg[500];
	int         cnt, offset, istr;
	char * pch;

	/*if (strcmp(section, prev_section)) {
	//printf("%s[%s]\n", (prev_section[0] ? "\n" : ""), section);
	//sprintf(msg,"%s[%s]\n", (prev_section[0] ? "\n" : ""), section);
	//MsgAddToTEXT(msg);

	strncpy(prev_section, section, sizeof(prev_section));
	prev_section[sizeof(prev_section) - 1] = '\0';
	}*/

	//sprintf(msg,"%s = %s\n", name, value); MsgAddToTEXT(msg);

	/*
	#define MAX_t_path 55
	#define MAX_e_shader 35
	#define MAX_c_shader 40
	#define MAX_g_material 35

	char t_path[55];
	char e_shader[35];
	char c_shader[40];
	char g_material[35];
	int  flags;

	*/
	if (strlen(name)<MAX_t_path) strcpy(ini_sting[ini_count].t_path, name);
	else return 0;

	StrTOLower(ini_sting[ini_count].t_path);

	strcpy(ini_sting[ini_count].e_shader, "default\0");
	strcpy(ini_sting[ini_count].c_shader, "default\0");
	strcpy(ini_sting[ini_count].g_material, "default\0");

	if (strlen(value))
	{
		memset(msg, 0, sizeof(msg));
		strcpy(msg, value);
		strcat(msg, ";\0"); //подправляем косяки INI файла
		cnt = 0;
		offset = 0;
		pch = strchr(msg, ';');
		while (pch != NULL)
		{
			//printf ("found at %d\n",pch-str+1);
			istr = (int)(pch - msg) - offset;
			//sprintf(msg,"[%d] found at %d l:%d",cnt,pch-value+1,istr); MsgAddToTEXT(msg);
			if (istr)
			{
				switch (cnt)
				{
				case 0: if (istr>MAX_e_shader) return 0;
					strncpy(ini_sting[ini_count].e_shader, msg + offset, istr);
					break;
				case 1: if (istr>MAX_c_shader) return 0;
					strncpy(ini_sting[ini_count].c_shader, msg + offset, istr);
					break;
				case 2: if (istr>MAX_g_material) return 0;
					strncpy(ini_sting[ini_count].g_material, msg + offset, istr);
					break;
				case 3: if (istr>1) return 0;
					ini_sting[ini_count].flags = msg[offset] - 0X30;
					break;
				case 4: break;
				default:
					return 0;
					break;
				}
			}
			cnt++;
			offset = (int)(pch - msg) + 1;
			pch = strchr(pch + 1, ';');
		}

	}
	StrTOLower(ini_sting[ini_count].e_shader);
	StrTOLower(ini_sting[ini_count].c_shader);
	StrTOLower(ini_sting[ini_count].g_material);


	//strcpy(ini_sting[ini_count].value,value);
	ini_count++;

	if (ini_count>MAX_ini_str) 
	{
		System::Windows::Forms::MessageBox::Show("Переполнение памяти для INI файла.", "Ошибка");
		return 0;
	}
	return 1;
}
