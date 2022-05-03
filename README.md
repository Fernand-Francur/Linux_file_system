Copyright Timothy Borunov 2022

PROGRAM: fs.c //Program is a library of functions which permit the creation, mounting, and manipulation of a file system on top of the Linux file system, using a file in linux as a disk for the file system.

FILES: fs.c fs.h disk.c disk.h //Build library objects fs.o and disk.o using make.

FUNCTIONS: fs_make: creates a file which serves as the disk and writes initial metadata to it
	   fs_mount: mounts a created filesystem on an existing disk
	   fs_umount: unmounts the created filesystem, writing metadata back to disk
	   fs_create: creates a file on the filesystem
	   fs_delete: deletes a file from the filesystem
	   fs_open: Opens a file providing a file descriptor to be used in other functions
	   fs_close: closes a file, inactivating a file descriptor
	   fs_write: writes a buffer cache of specific length to current file pointer in file
	   fs_read: reads to a buffer cache of a specific length from a file pointer in the file
	   fs_truncate: cuts down file to a specified length
	   fs_listfiles: lists all existing names of files in the filesystem
	   fs_lseek: changes the location of a file pointer in a specified file
	   fs_get_filesize: returns the size of specified file