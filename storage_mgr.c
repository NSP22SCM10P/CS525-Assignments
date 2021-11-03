#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<math.h>

#include "storage_mgr.h"

FILE *page_file;

/* Written by Nidhi */
extern void initStorageManager(void){
	page_file = NULL;
	printf("Storage manager Initialized!!!");
}

/* Written by Nidhi */
extern RC createPageFile(char *file_name){
    // opening the file stream in write and read mode
    // 'w+' creates an empty file
    page_file = fopen(file_name, "w+");
    if(page_file != NULL){
        //creating empty page in memory
        SM_PageHandle emptyPage = (SM_PageHandle)calloc(PAGE_SIZE, sizeof(char));
        //writing empty page into file
        if(fwrite(emptyPage,sizeof(char),PAGE_SIZE,page_file)<PAGE_SIZE){
			printf("Writing into file failed....!!!\n");
        }
        else{
			printf("Writing into file succeeded....!!!\n");
        }
        // freeing the allocated memory
        free(emptyPage);
		return RC_OK;
    }
    else{
		return RC_FILE_NOT_FOUND;
    }
}

/* Written by Nidhi */
extern RC openPageFile(char *file_name,SM_FileHandle*file_handle){
    //opening file in read mode
    //'r' opens file in read mode only
    page_file = fopen(file_name, "r");
    //checking whether file opened or not....!
    if(page_file!=NULL){
        //changing file handle's filename
        file_handle->fileName = file_name;
        // setting pointer to start of file
        file_handle->curPagePos = 0;
        //using stat (system call) to get file information
        struct stat file_info;
		stat(file_name,&file_info);
        file_handle->totalNumPages = file_info.st_size/ PAGE_SIZE;
		return RC_OK;
    }
    else{
        return RC_FILE_NOT_FOUND;
    }
}

/* Written by Nidhi */
/* Edited By Prashansa Line 74 */
extern RC closePageFile (SM_FileHandle *fHandle) {
	// Checking if the storage manager or the file pointer has been initialised. Close if it's been established.
	printf("test check\n");
	if(fopen(fHandle->fileName, "r") == NULL)
    {
		printf("\nFile not found!");
		return RC_FILE_NOT_FOUND;
	}
	else{
		//Update this section 
		//Previous code fclose(fHandle->mgmtInfo);
		//below is new code
        fclose(page_file);
        printf("\nFile Closed!");
		return RC_OK;
	}
}

/* Written by Nidhi */
extern RC destroyPageFile (char *fileName) {
	// In read mode, open the file stream.
	page_file = fopen(fileName, "r");

	if(page_file == NULL)
		return RC_FILE_NOT_FOUND;
	else{
	// removing the given file so that it is no longer available
	remove(fileName);
	printf("\nFile Destroyed!");
	return RC_OK;
	}
}


/* Written By Prashansa */
extern RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage) {
	// Checking if File exist if it is null
	if(open(fHandle->fileName,"r") != NULL){
		//Making sure the Page Number is > 0 and Less than Total Number of pages or else throwing an Non existing page error.
		if (pageNum > fHandle->totalNumPages || pageNum < 0)
        	return RC_READ_NON_EXISTING_PAGE;
		//Seeking to start of the page file to read
		fseek(page_file, (pageNum * PAGE_SIZE), SEEK_SET);
		//Reading from start of the page to end of the same page
		fread(memPage, sizeof(char), PAGE_SIZE, page_file);
		//updating the Current Page Position to the pagenum as the program keeps on reading the pagefile.
		fHandle->curPagePos = pageNum;
    	return RC_OK;
	}
	//if File not found.
	else{
		return RC_FILE_NOT_FOUND;
	}

}

/* Jainam */
extern int getBlockPos (SM_FileHandle *fHandle) {
	// Returning the current page position retrieved from the file handle
	return fHandle->curPagePos;
}

/* Jainam */

extern RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage) {
	// Re-directing (passing) to readBlock(...) function with pageNumber = 0 i.e. first block
	return readBlock(0, fHandle, memPage);
}

/* Written by Jainam */
extern RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage) {
	//Reading previous block by passing current postion-1 value to readblock.
	return readBlock(getBlockPos(fHandle) - 1, fHandle, memPage);
}

/* Written by Jainam */
extern RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage) {
	
	return readBlock(getBlockPos(fHandle), fHandle, memPage);
}

/* Written by Jainam */
extern RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
	//
	return readBlock(getBlockPos(fHandle) + 1, fHandle, memPage);
}

/* Written by Jainam */
extern RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
	//
	return readBlock(fHandle->totalNumPages, fHandle, memPage);
}



/* Written By Prashansa */
extern RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage) {
	//// Checking if File exist if it is null
	if(open(fHandle->fileName,"r") != NULL){

		//Ensuring that pagenum is less than total number of pages and pagenum is greater than 0.
		if (fHandle->totalNumPages >  pageNum || pageNum > 0){

			//opeining the file in write mode to write the content to the file
			page_file = fopen(fHandle->fileName, "w");
			//seeking the start of the file
			fseek(page_file, (pageNum * PAGE_SIZE), SEEK_SET);
			//Writing content to the file
			fwrite(memPage,PAGE_SIZE,1,page_file);
			//Updating the Current Page number after writing content to the page file.
			fHandle->curPagePos = pageNum;

			//Updating the Total Number of pages after compeleting writing operation
			fHandle->totalNumPages = ftell(page_file)/PAGE_SIZE;
			return RC_OK;
		}

		//If page Num is out of Bound then throwing write Fail error.
		else{
			return RC_WRITE_FAILED;
		}
	}
	//If page_file not found.
	else{
		return RC_FILE_NOT_FOUND;
	}
}


/* Written By Prashansa */
extern RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	return writeBlock(getBlockPos(fHandle), fHandle, memPage);
}


/* Written By Prashansa */
extern RC appendEmptyBlock (SM_FileHandle *fHandle) {
	//Checking if file exist
	if(fopen(fHandle->fileName,"w")!= NULL){
		//Writing a block of size Page Size.
		int i;
		char block[PAGE_SIZE];
		while (i<PAGE_SIZE)
		{
			block[i]=0;
			i++;
		}
		//Seeking the start of the file
		fseek(page_file, 0, SEEK_END);
		//Appending the empty block at the end of the file.
		fwrite(block, sizeof(char), PAGE_SIZE, page_file);
		//Updating the total number of pages in the page file after appending the empty block at the end of the file.
		fHandle->totalNumPages++;
		return RC_OK;
	}
	//if file not found Throwing error that file not found
	else{
		return RC_FILE_NOT_FOUND;
	}
}

/* Written By Prashansa */
extern RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle) {
	
	//checking of file is not empty
	if(fopen(fHandle->fileName, "r")!= NULL){
		//initializing variables
		int numofPagetoAppend,n;
		//storing the value of number pages to be aappended to end of page file to ensure capacity
		numofPagetoAppend = numberOfPages - fHandle->totalNumPages;
		n=1;
		//looping through to append empty block at the end of the file.
		while(n == numofPagetoAppend){
			appendEmptyBlock(fHandle);
			n++;
		}
	}
	else{
		return RC_FILE_NOT_FOUND;
	}
}
