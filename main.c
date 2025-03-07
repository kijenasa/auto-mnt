#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <uuid/uuid.h>

#include "config.h"

void notication_send(const char *title, const char *dev, const char *dir) {
   char buf[100];
   sprintf(buf, "notify-send \"%s\" \"%s at %s\"", title, dev, dir);
   printf("%s\n", buf);
   // system(buf);
}

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

int mount_drive(const char *dev) {
   char dev_buf[50];
   sprintf(dev_buf, "%s%s", DEV_DIR, dev);

   char path_buf[50];
   int mkdir_result = new_mount_dir(path_buf);

   notication_send("Mounting", dev_buf, path_buf);
   int mount_result = mount(dev_buf, path_buf, NULL, MS_BIND, NULL);

   return mkdir_result | mount_result;
}

int unmount_drive(const char *dir) {
   int umount_result = umount(dir);
   int del_result = rmdir(dir);
   return umount_result | del_result;
}

int main() {
   int fd = inotify_init();
   if(fd == -1) {
      perror("inotify_init");
      exit(EXIT_FAILURE);
   }

   int wd = inotify_add_watch(fd, DEV_DIR, IN_CREATE | IN_DELETE);
   if(wd == -1) {
      perror("inotify_add_watch");
      exit(EXIT_FAILURE);
   }

   uint8_t buf[1024];
   while(1) {
      int read_result = read(fd, buf, sizeof(buf));
      if(read_result == -1) {
         perror("read");
         continue;
      }

      for(int i = 0; i < read_result;) {
         struct inotify_event *event = (struct inotify_event *)&buf[i];

         if(event->mask & IN_CREATE) {
            printf("CREATE %s\n", event->name);
            mount_drive(event->name);
         } else if(event->mask & IN_DELETE) {
            printf("DELETE %s\n", event->name);
         }

         i += sizeof(struct inotify_event) + event->len;
      }
   }

   return EXIT_SUCCESS;
}
