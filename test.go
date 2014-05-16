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

package main
 
/* 
#cgo LDFLAGS: -lfdfs -lfastcommon -lfdfsclient  -lpthread -ldl -L/usr/local/lib 
#cgo CFLAGS: -I/usr/local/include/fastcommon -I/usr/local/include/fastdfs 
#include "fdfs.h"
*/
import "C"
import "fmt"
import "path/filepath"
import "strings"
import "errors"

//上传文件到fdfs
//conf:配置文件的文件,example:/etc/fdfs/client.conf
//imagePath:需要上传文件的完整路径,example:"/root/Desktop/logo.jpg"
func FdfsUploadFile(conf string,imagePath string)(result map[string]interface{},err error){
	
    result = make(map[string]interface{})
    var resData C.responseData=C.upload_file(C.CString(conf),C.CString(imagePath));
    
    fmt.Println("upload file msg:", C.GoString(resData.msg)) ////当成功的时候，是返回图片的id,example:group1/M00/00/00/wKgBP1NxvSqH9qNuAAAED6CzHYE179.jpg ,当失败的时候是返回错误消息
    fmt.Println("upload file result:", resData.result) //１表示成功，０表示失败
    
    if resData.result==0 { //上传失败, 返回例子:{"r":{"r":false}}
    
    	err = errors.New(C.GoString(resData.msg))
    	return
    } else { //上传成功, 返回例子:"filename":"scree.jpg","group":"group1","url":"M00\/00\/00\/wKgBP1NxvIPf81_1AABJuZk6wJM879.jpg"
    	
    	filename:=filepath.Base(imagePath)
    	
    	strPath:=C.GoString(resData.msg)
		splitStr:= strings.Split(strPath,"/")
		groupName:=splitStr[0]
		path:=""
	    joinStr:=""
		for i:=1;i<len(splitStr);i++ {
		   path=path+joinStr+splitStr[i]
		   joinStr="/"	
		}
		fmt.Println("group :"+groupName+"")
		fmt.Println("path: "+path)    	
    	
    	result["filename"]=filename;
    	result["group"]=groupName;
    	result["url"]=path;
    	
    	return result,nil
    }
    
}
 
 //删除fdfs文件
//conf:配置文件的文件,example:/etc/fdfs/client.conf
//imagePath:图片的id,example:"group1/M00/00/00/wKgBP1Nx1S7bSab8AAAED6CzHYE352.jpg"
func FdfsDeleteFile(conf string,fileId string)(result map[string]interface{},err error){

    result = make(map[string]interface{})
    var resData C.responseData=C.delete_file(C.CString(conf),C.CString(fileId));
    
    fmt.Println("upload file msg:", C.GoString(resData.msg)) ////当成功的时候，是返回图片的id,example:group1/M00/00/00/wKgBP1NxvSqH9qNuAAAED6CzHYE179.jpg ,当失败的时候是返回错误消息
    fmt.Println("upload file result:", resData.result) //１表示成功，０表示失败
    
    if resData.result==0 { //上传失败, 返回例子:{"r":{"r":false}}
    	err = errors.New(C.GoString(resData.msg))
    	return
    } else { //上传成功, 
    	result["msg"]=C.GoString(resData.msg)
    	return result,nil
    }
   
}
 
 
func main(){

   //上传文件
   str,err:=FdfsUploadFile("/etc/fdfs/client.conf","/root/Desktop/scree.jpg")
   
   //删除文件
//   str,err:=FdfsDeleteFile("/etc/fdfs/client.conf","group1/M00/00/01/wKgBP1N11RTg6-hYAABJuZk6wJM515.jpg")
   fmt.Println("file upload result:",str)
   fmt.Println("file upload err:",err)
   
}
