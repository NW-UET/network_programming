#include "../message.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{
    struct list_files_response a;
    a.n_files = 1;
    struct filename filename;
    filename.filename_length = 2;
    filename.filename = "filename";
    a.filename_list = &filename;
    printf("%d %d", a.n_files, a.filename_list->filename_length);
    return 0;
}
