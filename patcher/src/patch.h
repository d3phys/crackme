#ifndef PATCH_H
#define PATCH_H

enum patch_info {
        INFO_UNPATCHED = 0,
        INFO_FAIL      = 1,
        INFO_UNSUPPORT = 2,
        INFO_PATCHED   = 3,
};

int patch(const char *file);
int get_patch_info(const char *file);

#endif /* PATCH_H */
