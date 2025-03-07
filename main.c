#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <bits/posix1_lim.h>
#include <uuid/uuid.h>

#include "config.h"

void get_mount_path(char *buf) {
   uuid_t bin_uuid;
   uuid_generate_random(bin_uuid);
   char uuid[37];
   uuid_unparse(bin_uuid, uuid);

   sprintf(buf, MOUNT_DIR, uuid);
}

int new_mount_dir(char *buf) {
   get_mount_path(buf);

   return mkdir(buf, MOUNT_POINT_PERMISSIONS);
}

int del_mount_dir(const char *dir) {
   char buf[50];
   return rmdir(buf);
}

int mount_drive(const char *dev, char *path_buf) {
   int mkdir_result = new_mount_dir(path_buf);
   int mount_result = mount(dev, path_buf, NULL, MS_BIND, NULL);

   return mkdir_result | mount_result;
}

int unmount_drive(const char *dir) {
   int umount_result = umount(dir);
   int del_result = del_mount_dir(dir);
   return umount_result | del_result;
}

int main() {
   char b[50];
   new_mount_dir(b);
   //   del_mount_dir("aaa");

   return EXIT_SUCCESS;
}
