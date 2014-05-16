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

#include "fdfs.h"

void usage(char *argv[])
{
	printf("Usage: %s <config_file> <local_filename> " \
		"[storage_ip:port] [store_path_index]\n", argv[0]);
}

responseData delete_file(char *conf_filename, char *file_name)
{
	responseData res;
	ConnectionInfo *pTrackerServer;
	int result;
	char file_id[200];

	log_init();
	g_log_context.log_level = LOG_ERR;

	if ((result=fdfs_client_init(conf_filename)) != 0)
	{
		res.msg="fdfs_client_init failer";
		res.result=0;
		return res;
	}

	pTrackerServer = tracker_get_connection();
	if (pTrackerServer == NULL)
	{
		fdfs_client_destroy();
		res.msg="pTrackerServer connect failer";
		res.result=0;
		return res;
	}

	snprintf(file_id, sizeof(file_id), "%s", file_name);
	if ((result=storage_delete_file1(pTrackerServer, NULL, file_id)) != 0)
	{
		printf("delete file fail, " \
			"error no: %d, error info: %s\n", \
			result, STRERROR(result));
		res.msg="delete file fail";
		res.result=0;
	}

	res.msg="delete file success";
	res.result=1;
	tracker_disconnect_server_ex(pTrackerServer, true);
	fdfs_client_destroy();

	return res;

}


responseData upload_file(char *conf_filename, char *local_filename)
{
	responseData res;
	char group_name[FDFS_GROUP_NAME_MAX_LEN + 1];
	ConnectionInfo *pTrackerServer;
	int result;
	int store_path_index;
	ConnectionInfo storageServer;
	char file_id[200];

	log_init();
	g_log_context.log_level = LOG_ERR;

	if ((result=fdfs_client_init(conf_filename)) != 0)
	{
		res.msg="fdfs_client_init failer";
		res.result=0;
		return res;
	}

	pTrackerServer = tracker_get_connection();
	if (pTrackerServer == NULL)
	{
		fdfs_client_destroy();
		res.msg="pTrackerServer connect failer";
		res.result=0;
		return res;
	}

	*group_name = '\0';
	if ((result=tracker_query_storage_store(pTrackerServer, \
	                &storageServer, group_name, &store_path_index)) != 0)
	{
		fdfs_client_destroy();
		fprintf(stderr, "tracker_query_storage fail, " \
			"error no: %d, error info: %s\n", \
			result, STRERROR(result));
		res.msg="tracker_query_storage fail";
		res.result=0;
		return res;
	}

	result = storage_upload_by_filename1(pTrackerServer, \
			&storageServer, store_path_index, \
			local_filename, NULL, \
			NULL, 0, group_name, file_id);
	if (result == 0)
	{
		printf("file path:%s\n", file_id);
		res.msg=file_id;
		res.result=1;
	}
	else
	{
		fprintf(stderr, "upload file fail, " \
			"error no: %d, error info: %s\n", \
			result, STRERROR(result));
		res.msg="upload file fail";
		res.result=0;
	}

	tracker_disconnect_server_ex(pTrackerServer, true);
	fdfs_client_destroy();

	return res;
}
