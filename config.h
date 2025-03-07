#include <sys/types.h>

#define MOUNT_DIR "/mnt"

// See chmod(2)
#define MOUNT_POINT_PERMISSIONS S_IRUSR | S_IWUSR // RW for owner
