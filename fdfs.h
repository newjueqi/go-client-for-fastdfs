// Copyright © 2014 newjueqi(http://www.newjueqi.com) All rights reserved.
// This file is part of go-client-for-fastdfs.

// go-client-for-fastdfs is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// go-client-for-fastdfs is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with go-client-for-fastdfs.  If not, see <http://www.gnu.org/licenses/>.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "fdfs_client.h"
#include "logger.h"

typedef struct {
    char *msg; //当成功的时候,是返回图片的id,example:"group1/M00/00/00/wKgBP1NxvSqH9qNuAAAED6CzHYE179.jpg",当失败的时候是返回错误消息
    int  result; //１表示成功,０表示失败
}responseData;


responseData upload_file(char *conf_filename, char *local_filename);
responseData delete_file(char *conf_filename, char *file_name);
