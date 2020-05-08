#define T_DIR  1   // Directory
#define T_FILE 2   // File
#define T_DEV  3   // Device

#ifndef STAT_H_
#define STAT_H_

struct stat {
  short type;  // Type of file
  int dev;     // File system's disk device
  uint ino;    // Inode number

//  uint uid;    // File owner ID
//  uint gid;    // File group ID
//  uint mode;   // File mode

  ushort uid;               // owner ID
  ushort gid;               // group ID
//  union stat_mode_t mode;

  short nlink; // Number of links to file
  uint size;   // Size of file in bytes
};

#endif
