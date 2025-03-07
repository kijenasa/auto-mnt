#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/inotify.h>
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

int mount_drive(const char *dev, char *path_buf) {
   int mkdir_result = new_mount_dir(path_buf);
   int mount_result = mount(dev, path_buf, NULL, MS_BIND, NULL);

   return mkdir_result | mount_result;
}

int unmount_drive(const char *dir) {
   int umount_result = umount(dir);
   int del_result = rmdir(dir);
   return umount_result | del_result;
}

int main() {
   int fd = inotify_init();
   if(fd == -1)
      perror("inotify_init");

   int wd = inotify_add_watch(fd, DEV_DIR, IN_CREATE);
   if(wd == -1)
      perror("inotify_add_watch");

   char buf[100];
   while(1) {
      read(fd, buf, 100);
      struct inotify_event *event;
      event = (struct inotify_event *)buf;
      printf("%s\n", event->name);
   }

   system("notify-send AAAAAAAAA A");

   return EXIT_SUCCESS;
}
