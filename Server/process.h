#if !defined(process_h)
#define process_h
#include "message.h"

FileListUpdateResponse processFileListUpdate(FileListUpdateRequest,int);

ListFilesResponse processListFilesRequest(ListFilesRequest);

ListHostsResponse processListHostsRequest(ListHostsRequest);
#endif // process_h
