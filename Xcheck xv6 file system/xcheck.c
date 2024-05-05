
#include "xv6/types.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>

// xv6 below stay here plz
#include "xv6/fs.h"

// from "xv6/stat.h"
#define T_DIR 1  // Directory
#define T_FILE 2 // File
#define T_DEV 3  // Device

// Disk layout:
// [ boot block | superblock | log | inode blocks | free bitmap | data blocks ]

char *fs_img;
struct superblock sb;

void check_inodes_unallocated_or_valid();
void check_datablocks_valid();
void check_root_dir();
void check_dir_format();
void check_alloced_blocks_marked();
void check_marked_blocks_in_use();
void print_bitmap();

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: xcheck <file_system_image>\n");
        exit(1);
    }
    int fd;
    if ((fd = open(argv[1], O_RDONLY)) == -1)
    {
        fprintf(stderr, "image not found.");
        exit(1);
    }
    struct stat s;
    if (stat(argv[1], &s) == -1)
    {
        fprintf(stderr, "image not found.");
        exit(1);
    }
    size_t size = s.st_size;
    // printf("fs size: %ld\n", size);

    if ((fs_img = mmap(0, size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
    {
        fprintf(stderr, "mmap failed");
        exit(1);
    }

    sb = *(struct superblock *) (fs_img + BSIZE);
    // printf("superblock\n");
    // printf("size:\t%d\n", sb.size);
    // printf("nblocks:\t%d\n", sb.nblocks);
    // printf("ninodes:\t%d\n", sb.ninodes);
    // printf("nlog:\t%d\n", sb.nlog);
    // printf("logstart:\t%d\n", sb.logstart);
    // printf("inodestart:\t%d\n", sb.inodestart);
    // printf("bmapstart:\t%d\n", sb.bmapstart);
    // printf("IPB:\t%ld\n", IPB);

    print_bitmap();

    check_inodes_unallocated_or_valid();
    check_datablocks_valid();
    check_root_dir();
    check_dir_format();
    check_alloced_blocks_marked();
    check_marked_blocks_in_use();

    return 0;
}

// PART 1
// Your code will be similar to this.
void check_inodes_unallocated_or_valid()
{
    // pointer to first inode in fs image
    struct dinode *inode = (struct dinode *) (fs_img + sb.inodestart * BSIZE);

    // for each inode, let's print its type
    for (int i = 0; i < sb.ninodes; i++)
    {
        if (inode->type < 4)
        {
            inode++; // advance to next inode
        }
        else
        {
            printf("ERROR: bad inode \n");
            exit(1);
        }

    }
}

// PART 2
// Check that every
void check_datablocks_valid()
{
    // TODO
    //     For each inode:
    //         if the inode is in use (i.e., type != 0):
    //             - check that each direct address is valid (if used)
    //               (addrs[0]..addrs[NDIRECT-1] contain direct addresses)
    //             - check that the indirect address is valid (if used)
    //               (addrs[NDIRECT] contains the indirect address)

    //     An address of 0 represents an unused address.

    //     In order to be valid, an address must be in the range
    //     [sb.size - sb.nblocks, sb.size).
    // */
    struct dinode *inode = (struct dinode *) (fs_img + sb.inodestart * BSIZE);
    // printf("print\n");
    for (int i = 0; i < sb.ninodes; i++)
    {
        // printf("inode: %d type: %d\n", i, inode->type);
        if (inode->type != 0)
        {
            // printf("inode: %d type: %d\n", i, inode->type);
            //-----------------------
            for (int j = 0; j < NDIRECT - 1; j++)
            {
                // printf("inode: %d type: %d address: %u\n", i, inode->type, inode->addrs[j]);
                if (inode->addrs[j] != 0 && (inode->addrs[j] < sb.size - sb.nblocks || inode->addrs[j] >= sb.size))
                {
                    printf("ERROR bad direct address in inode\n");
                }
            }
            //-----------------------------------------
            if (inode->addrs[NDIRECT] != 0 && (inode->addrs[NDIRECT] < sb.size - sb.nblocks || inode->addrs[NDIRECT] >= sb.size))
            {
                printf("ERROR: bad indirect address in inode %d type: %d address: %u\n", i, inode->type, inode->addrs[NDIRECT]);
            }
            //--------------------------------------------------
        }
        inode++;
    }
}

// PART 3
void check_root_dir()
{

    // TODO  has a bad root directory (its parent is not itself)
    // it should only check directory  entries for the root folders only:
    /*
        - Check that the root inode is a directory (its type is T_DIR).
          Note that the root inode is inode number ROOTINO.

        - Check that the root directory contains an entry whose name is ".."
          and check that that entry points to the root directory itself (its inum is ROOTINO).
    */

       struct dinode *inode = (struct dinode *) (fs_img + sb.inodestart * BSIZE);

        for (int i = 0; i < sb.ninodes; i++){
            // printf("%d\n", inode->type);
          //  - Check that the root inode is a directory (its type is T_DIR).
            if (i == 1 && inode->type == T_DIR){

                // printf("---------inode#%d is a directory-----------\n", i);
                // printf("---------directory entries are-----------\n");
                uint block_num = inode -> addrs[0];
                struct dirent *de = (struct dirent *) (fs_img + block_num * BSIZE);
                while (de-> inum !=0){
                    // printf("inum: %d, name: %s\n", de->inum, de->name);

                    bool rtdir = strcmp(de->name, ".");
                    bool currdir = strcmp(de->name, "..");

                    if (currdir == 0){
                        // printf("%d \n", de->inum);
                        if (!(de->inum == ROOTINO && rtdir == 0) && !(de->inum == ROOTINO && currdir == 0)){
                            printf("%d \t ERROR: root directory does not exist\n", rtdir);
                            exit(1);
                        }
                    }

                    de++;

                }
            }

            if (i == 1 && inode->type != T_DIR){
                printf("ERROR: Root Directory does not exist \n");
                // exit(1);
            }
            inode++;
        }
}

// PART 4
void check_dir_format()
{
    // printf("PART FOUR: Check Directory FORMAT\n");
    /*
        For each i (0 <= i < sb.ninodes):
            - if inode i is a directory (its type is T_DIR):
                - check that the directory contains an entry whose name is "."
                  and check that that entry points to inode i (its inum is i).
                - check that the directory contains an entry whose name is ".."
    */
    struct dinode *inode = (struct dinode *)(fs_img + sb.inodestart * BSIZE);

    for (int i = 0; i < sb.ninodes; i++) {
        if (inode->type == T_DIR) {
            // printf("---------inode#%d is a directory-----------\n", i);
            // printf("---------directory entries are-----------\n");

            uint block_num = inode->addrs[0];
            struct dirent *de = (struct dirent *)(fs_img + block_num * BSIZE);

            bool rtdir = false;
            bool currdir = false;

            while (de->inum != 0) {
                if (strcmp(de->name, ".") == 0) {
                    if (de->inum != i) {
                        printf("ERROR: directory not properly formatted (entry does not point to itself)\n");
                        exit(1);
                    } else {
                        rtdir = true;
                    }
                }
                else if (strcmp(de->name, "..") == 0) {
                    // Check if the .. entry points to the correct parent directory
                    currdir = true;
                }

                // printf("inum: %d, name: %s\n", de->inum, de->name);
                de++;
            }

            if (!rtdir || !currdir) {
                printf("ERROR: directory not properly formatted)\n");
                exit(1);
            }
        }

        inode++;
    }
}

// PART 5
void check_alloced_blocks_marked()
{
    // For in-use inodes, each address in use is also marked in use in the bitmap.
    // If not, print ERROR: address used by inode but marked free in bitmap.

        struct dinode *inode = (struct dinode *)(fs_img + sb.inodestart * BSIZE);
        for (int i = 0; i < sb.ninodes; i++) {
            if (inode->type != 0) {
                for (int j = 0; j < NDIRECT; j++) {
                    printf("inode: %d type: %d address: %u\n", i, inode->type, inode->addrs[j]);
                    //----------------------------------------
                    if (inode->addrs[j] != 0) {

                    char *bmap = fs_img + BSIZE * BBLOCK(0, sb);

                        if (!(bmap[inode->addrs[j] / 8] & (1 << (inode->addrs[j] % 8)))) {
                        printf("ERROR: address %d used by inode %d but marked free in bitmap\n", inode->addrs[j], i);
                        }
                    }
                    //-----------------------
                    if (inode->addrs[NDIRECT] != 0 && (inode->addrs[NDIRECT] < sb.size - sb.nblocks || inode->addrs[NDIRECT] >=
                    sb.size)) {
                }
            }
            inode++;
        }
    }
}



// // PART 6
void check_marked_blocks_in_use() {
    /*
        1. Create a list of blocks that are in use. A block is in use if:
            - it is not a data block (i.e., its block number is less
              than sb.size - sb.nblocks), or
            - it appears in the list of some inode's direct addresses, or
            - it is the the indirect address of some inode, or
            - it appears in the list of addresses in some inode's indirect block

        2. For each bit in the bitmap:
            - if the bit is 1:
                - check that the corresponding data block is in the list that
                  you created in step 1.
    */
    int used_blocks[sb.size];// data blocks
    memset(used_blocks, 0, sizeof(used_blocks));

        // Step 1: Create a list of blocks that are in use
    struct dinode *inode = (struct dinode *) (fs_img + sb.inodestart * BSIZE);


    for (int i =0; i < sb.size-sb.nblocks; i++){ // set non data blocks as in-used  (0-58)
        used_blocks[i] = 1;
    }


    for (int i = 0; i < sb.ninodes; i++) { // for each inode

        if (inode->type != 0) // check if inode is in use
        {
            //----------------------- Direct-------------  COMPLETED - OKAY
            for (int j = 0; j < NDIRECT-1; j++) {
                if (inode->addrs[j] != 0) {
                     //printf("Before setting used_blocks[%d] to 1: %d\n", inode->addrs[j], used_blocks[inode->addrs[j]]);
                    used_blocks[inode->addrs[j]] = 1;
                    // printf("After setting used_blocks[%d] to 1: %d\n", inode->addrs[j], used_blocks[inode->addrs[j]]);
                }
            }
            //---------------------- Indirect-------------
            if (inode->addrs[NDIRECT] != 0)
            {
                used_blocks[inode->addrs[NDIRECT]] = 1; // SET INDIRECT ADDRESS TO IN USE

                //printf("indirect address in inode %d type: %d address: %u\n", i, inode->type, inode->addrs[NDIRECT]);
            }
            //--------------------Indirect Blocks------------------
            if (inode->addrs[NDIRECT] != 0)
            {
                uint *indirectblock = (uint *)(fs_img + inode->addrs[NDIRECT]*BSIZE);
                for (int k = 0; k <NINDIRECT; k++){
                    if ((inode->addrs[k]!=0) && (inode->addrs[k] > sb.size - sb.nblocks && inode->addrs[k] <= sb.size)){
                        //printf("indirect inode block: %d \n", inode->addrs[k]);
                        used_blocks[inode->addrs[k]] = 1;
                    }


                    if (*indirectblock !=0){
                        used_blocks[*indirectblock] = 1;
                        indirectblock++;
                    }
                }
            }
        }
        inode++;
    }

    // lets print contents of used_blocks
    char* bmap = fs_img + BSIZE * BBLOCK(0, sb);
    for (int i = 0; i<sb.size; i++){
        if ((bmap[i / 8] & (1 << (i % 8)))){
            if (used_blocks[i] == 0){
                printf("ERROR\n");
            }
        }
    }



}





void print_bitmap()
    {
        char *bmap = fs_img + BSIZE * BBLOCK(0, sb);
        for (int i = 0; i < sb.size; i++)
        {
            if (i % 64 == 0)
                printf("%3d: ", i);
            if (bmap[i / 8] & (1 << (i % 8)))
                printf("1");
            else
                printf("0");
            if (i % 8 == 7)
                printf(" ");
            if (i % 64 == 63)
                printf("\n");
        }
        printf("\n");
    }


