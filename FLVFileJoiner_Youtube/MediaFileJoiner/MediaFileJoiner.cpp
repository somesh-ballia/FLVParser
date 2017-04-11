// FLVSplitTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#define INPUT_FILE_1		"D:\\Youtube_flv\\Single_File\\1"
#define INPUT_FILE_2		"D:\\Youtube_flv\\Single_File\\2"
#define INPUT_FILE_3		"D:\\Youtube_flv\\Single_File\\3"
#define INPUT_FILE_4		"D:\\Youtube_flv\\Single_File\\4"
#define INPUT_FILE_5		"D:\\Youtube_flv\\Single_File\\5"
#define INPUT_FILE_6		"D:\\Youtube_flv\\Single_File\\6"
#define INPUT_FILE_7		"D:\\Youtube_flv\\Single_File\\7"
#define INPUT_FILE_8		"D:\\Youtube_flv\\Single_File\\8"
#define INPUT_FILE_9		"D:\\Youtube_flv\\Single_File\\9"
#define INPUT_FILE_10		"D:\\Youtube_flv\\Single_File\\10"
#define INPUT_FILE_11		"D:\\Youtube_flv\\Single_File\\11"
#define INPUT_FILE_12		"D:\\Youtube_flv\\Single_File\\12"
#define INPUT_FILE_13		"D:\\Youtube_flv\\Single_File\\13"
#define INPUT_FILE_14		"D:\\Youtube_flv\\Single_File\\14"
#define INPUT_FILE_15		"D:\\Youtube_flv\\Single_File\\15"
#define INPUT_FILE_16		"D:\\Youtube_flv\\Single_File\\16"
#define INPUT_FILE_17		"D:\\Youtube_flv\\Single_File\\17"
#define INPUT_FILE_18		"D:\\Youtube_flv\\Single_File\\18"
#define INPUT_FILE_19		"D:\\Youtube_flv\\Single_File\\19"
#define INPUT_FILE_20		"D:\\Youtube_flv\\Single_File\\20"
#define INPUT_FILE_21		"D:\\Youtube_flv\\Single_File\\21"
#define INPUT_FILE_22		"D:\\Youtube_flv\\Single_File\\22"
#define INPUT_FILE_23		"D:\\Youtube_flv\\Single_File\\23"
#define INPUT_FILE_24		"D:\\Youtube_flv\\Single_File\\24"
#define INPUT_FILE_25		"D:\\Youtube_flv\\Single_File\\25"
#define INPUT_FILE_26		"D:\\Youtube_flv\\Single_File\\26"
#define INPUT_FILE_27		"D:\\Youtube_flv\\Single_File\\27"
#define INPUT_FILE_28		"D:\\Youtube_flv\\Single_File\\28"

#define OUTPUT_FILE_ONE		"D:\\Youtube_flv\\Single_File\\Outfile.flv"

#define BUFFER_SIZE	5120	
#define READ_SIZE	1

#define SUCCESS 1
#define FAILURE 0

int WriteFLVHeader(FILE *&fp_fileWriter)
{
	char pszBuffer[BUFFER_SIZE] = {0};

	// AUDIO HEADER
	pszBuffer[0]= 'F';
	pszBuffer[1]= 'L';
	pszBuffer[2]= 'V';
	pszBuffer[3]= 0x01;
	pszBuffer[4]= 0x05;
	pszBuffer[5]= 0x00;
	pszBuffer[6]= 0x00;
	pszBuffer[7]= 0x00;
	pszBuffer[8]= 0x09;
	pszBuffer[9]= 0x00;
	pszBuffer[10]= 0x00;
	pszBuffer[11]= 0x00;
	pszBuffer[12]= 0x00;
	fwrite(pszBuffer,sizeof(char),13,fp_fileWriter);
	return SUCCESS;
}

int CopyToFile(char* pszInputFile,FILE *&fp_fileWriter)
{
	FILE *pf_reader = fopen(pszInputFile,"rb");
	if(NULL != pf_reader)
	{
		int iLength = 0;
		fseek(pf_reader,0,SEEK_END);
		iLength = ftell(pf_reader);
		fseek(pf_reader,0,SEEK_SET);
		int iCounter = 0;
		while(iCounter < iLength)
		{
			char pszBuffer[READ_SIZE] = {0};
			if(0 < fread(pszBuffer,sizeof(char),READ_SIZE,pf_reader))
			{
				fwrite(pszBuffer,sizeof(char),READ_SIZE,fp_fileWriter);
				iCounter+=READ_SIZE;
			}
		}
		fclose(pf_reader);
	}	
	return SUCCESS;
}


int main()
{
	int iFileSize = 0;
	int iFileSizeHalf = 0;
	int iOriginalCounter = 0;
	int iNewCounter	= 0;

	FILE *fp_fileWriter = fopen(OUTPUT_FILE_ONE,"wb");

	if(NULL != fp_fileWriter)
	{ 
		if(SUCCESS == WriteFLVHeader(fp_fileWriter))
		{
			if (SUCCESS == CopyToFile(INPUT_FILE_1,fp_fileWriter))
			{
				if (SUCCESS == CopyToFile(INPUT_FILE_2,fp_fileWriter))
				{
					if (SUCCESS == CopyToFile(INPUT_FILE_3,fp_fileWriter))
					{
						if (SUCCESS == CopyToFile(INPUT_FILE_4,fp_fileWriter))
						{
							if (SUCCESS == CopyToFile(INPUT_FILE_5,fp_fileWriter))
							{
								if (SUCCESS == CopyToFile(INPUT_FILE_6,fp_fileWriter))
								{
									if (SUCCESS == CopyToFile(INPUT_FILE_7,fp_fileWriter))
									{
										if (SUCCESS == CopyToFile(INPUT_FILE_8,fp_fileWriter))
										{
											if (SUCCESS == CopyToFile(INPUT_FILE_9,fp_fileWriter))
											{
												if (SUCCESS == CopyToFile(INPUT_FILE_10,fp_fileWriter))
												{
													if (SUCCESS == CopyToFile(INPUT_FILE_11,fp_fileWriter))
													{
														if (SUCCESS == CopyToFile(INPUT_FILE_12,fp_fileWriter))
														{
															if (SUCCESS == CopyToFile(INPUT_FILE_13,fp_fileWriter))
															{
																if (SUCCESS == CopyToFile(INPUT_FILE_14,fp_fileWriter))
																{
																	if (SUCCESS == CopyToFile(INPUT_FILE_15,fp_fileWriter))
																	{
																		if (SUCCESS == CopyToFile(INPUT_FILE_16,fp_fileWriter))
																		{
																			if (SUCCESS == CopyToFile(INPUT_FILE_17,fp_fileWriter))
																			{
																				if (SUCCESS == CopyToFile(INPUT_FILE_18,fp_fileWriter))
																				{
																					if (SUCCESS == CopyToFile(INPUT_FILE_19,fp_fileWriter))
																					{
																						if (SUCCESS == CopyToFile(INPUT_FILE_20,fp_fileWriter))
																						{
																							if (SUCCESS == CopyToFile(INPUT_FILE_21,fp_fileWriter))
																							{
																								if (SUCCESS == CopyToFile(INPUT_FILE_22,fp_fileWriter))
																								{
																									if (SUCCESS == CopyToFile(INPUT_FILE_23,fp_fileWriter))
																									{
																										if (SUCCESS == CopyToFile(INPUT_FILE_24,fp_fileWriter))
																										{
																											if (SUCCESS == CopyToFile(INPUT_FILE_25,fp_fileWriter))
																											{
																												if (SUCCESS == CopyToFile(INPUT_FILE_26,fp_fileWriter))
																												{
																													if (SUCCESS == CopyToFile(INPUT_FILE_27,fp_fileWriter))
																													{
																														if (SUCCESS == CopyToFile(INPUT_FILE_28,fp_fileWriter))
																														{
																															printf("\nDONE");
																														}
																													}
																												}
																											}
																										}
																									}
																								}
																							}
																						}
																					}
																				}
																			}
																		}
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}		
	}

	fclose(fp_fileWriter);
	_getch();
	return 0;
}



