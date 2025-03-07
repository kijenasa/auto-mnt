#include <stdlib.h>
#include <sys/mount.h>

#include "config.h"

char *new_mount_dir() { return NULL; }

int del_mount_dir(const char *dir) { return -1; }

int mount_drive(const char *dev) {
   char *target = new_mount_dir();
   return mount(dev, target, NULL, MS_BIND, NULL);
}

int unmount_drive(const char *dir) {
   int result = umount(dir);
   del_mount_dir(dir);
   return result;
}

int main() { return EXIT_SUCCESS; }
