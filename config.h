#include <sys/types.h>

#define MOUNT_DIR "/mnt/%s"

// See chmod(2)
#define MOUNT_POINT_PERMISSIONS S_IRUSR | S_IWUSR // RW for owner
