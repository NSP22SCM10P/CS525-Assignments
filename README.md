# CS525-Assignment-1
Assignment-1 Storage manager
Created by:

Group 26
Nidhi Patel
Prashansa Chaudhary
Jainam Shah


Description:

	Writing a program to read and write a block of data to and from memory.

Instructions to run the code:

	Using the git, clone the repository.

	Use the command "make test1." -> converts the source file to the executable format(.o) and execute it.

	Use the command "make test2" ->  Run test case 2 file.

	Use the command "make clean" ->  for cleaning all the object files created in above commands.
	
Functions and Desciptions:

createPageFile(): 
		This method creates a new file, appends the number '0' to it, and moves the pointer to the end of it.

openPageFile():

		To access the preceding function's file and update all of the fHandle's meta data arguments, such as
		The total number of pages, current page position, file name, and file location are all stored in the buffer variable mgmtInfo for later use.

closePageFile(): 
		This function is used to close an existing file.

destroyPageFile(): 
		This method is used to delete an existing file.

readBlock(): 
		After reading a specific block of data from a file, it moves it to memory and changes the current page position to the new location.

getBlockPos(): 
		Returns the current location of the block.


readFirstBlock():

		By using the read block method directly and returning the value from the read block, you may read the first block of data in the file.



readCurrentBlock():

		Reading the current block of data from a file using the read block method directly and supplying the current index or filename.
		Returning the value from the read block as well as the location of the next block to be read.

readNextBlock(): 
		This method uses the readBlock() and getBlockPos() function directly and supplies the next index read the next block of data in the file, and then returns the read block result.



readLastBlock():

		We can read the last block of data in a file by using the read block method and providing the file's last index or position.


writeBlock(): 
		Checks that the page number does not exceed the total number of pages limit or is less than zero before writing each page to disc.



writeCurrentBlock():

		We can write the page at the current location by passing the current position as a parameter to the writeBlock.



appendEmptyBlock():

		Append a new page to the end of the file using the array of PAGE SIZE and write it with zero bytes.

ensureCapacity(): 
		Checks if the number of pages in the file is less than the number of pages required, add one additional page or the required number of pages.



Case Studies:

		The program test assign1 1.c verifies that all of the test conditions are met and that the code is error-free.
