
# File System Checking

In this assignment, you will be developing a working file system checker, called
**xcheck**, for xv6 file system images. Starter code is provided in `xcheck.c`.

A checker reads in a file system image and makes sure that it is consistent. If
it isn't, the checker usually attempts to repair the problems it sees; however,
to keep this project a little simpler, you won't be doing any repairs.


## A Basic Checker

For this project, you will use the xv6 file system image as the basic image that
you will be reading and checking. The file `xv6/fs.h` includes the basic
structures you need to understand, including the superblock, on-disk inode
format (`struct dinode`), and directory entry format (`struct dirent`).  The
tool `xv6/mkfs.c` will also be useful to look at, in order to see how an empty
filesystem image is created.

Much of the work for this project will be figuring out the exact on-disk format
that xv6 uses for its simple file system, and then writing checks to see if
various parts of that structure are consistent. Thus, reading through
`xv6/mkfs.c` and the file system code itself (`xv6/fs.c`) will help you
understand how xv6 uses the bits in the image to record persistent information.

Your checker should examine the file system image and check for the following
types of problems. When a problem is detected, print the specified error message
to **standard error** and exit immediately with **exit code 1** (i.e., call
`exit(1)`).

Each number in the numbered list below corresponds to a function in the starter
code (`xcheck.c`) that you must write.

1. Each inode is either unallocated or has one of the valid types (`T_FILE`,
`T_DIR`, `T_DEV`). If not, print `ERROR: bad inode`. Note that unallocated
inodes have their `type` set to 0.

2. For in-use inodes, each address that is used by the inode is valid (points to
a valid datablock address within the image). If the direct block is used and is
invalid, print `ERROR: bad direct address in inode`.; if the indirect block is
in use and is invalid, print `ERROR: bad indirect address in inode`.  Note that
an address of 0 means "not used." Valid datablock addresses are in the range
[`sb.size - sb.nblocks`, `sb.size`).

3. The root directory exists, its inode number is 1, and the parent of the root
directory is itself. If not, print `ERROR: root directory does not exist`.
Remember that a directory is a file consisting of a sequence of directory
entries (`struct dirent`s). The end of the sequence is marked by a directory
entry with `inum` = 0.

4. Each directory contains `.` and `..` entries, and the `.` entry points to the
directory itself. If not, print `ERROR: directory not properly formatted`.

5. For in-use inodes, each address in use is also marked in use in the bitmap.
If not, print `ERROR: address used by inode but marked free in bitmap`.

6. For data blocks marked in-use in bitmap, the block should actually be in use
as a data block or indirect block. If not, print `ERROR: bitmap marks block in
use but it is not in use`.


## Other Specifications

Your checker program, called `xcheck`, must be invoked as follows:

```
$ ./xcheck file_system_image
```

If the checker detects any one of the 6 errors above, it should print the
specified error message to standard error and exit with error code 1.

If the checker detects none of the problems listed above, it should exit with
return code of 0 and not print anything.

## Hints

It should be very helpful to read Chapter 6 of the xv6 book
[here](https://pdos.csail.mit.edu/6.828/2014/xv6/book-rev8.pdf).

## Testing

The `images` directory contains several images that you can use for testing your
`xcheck` program:

- `goodfs.img` is a consistent xv6 filesystem image
- `badfs.1` has an inode with a bad type
- `badfs.2a` has an inode with a bad direct address
- `badfs.2b` has an inode with a bad indirect address
- `badfs.3a` has no root directory (inode number 1 is not a directory)
- `badfs.3b` has a bad root directory (its parent is not itself)
- `badfs.4a` has a directory with no `.` entry
- `badfs.4b` has a directory with no `..` entry
- `badfs.4c` has a directory whose `.` entry does not point to itself
- `badfs.5` has a data block address that is used by an inode but marked free in the bitmap
- `badfs.6` has a data block address that is marked as "in use" in the bitmap
  but is not in use as a data block or indirect block
