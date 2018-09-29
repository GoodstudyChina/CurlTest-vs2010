// CurlTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include "curl/include/curl.h"
#include <atlstr.h>
#include <string.h>
#include <iostream>
using namespace std; 
#include <comdef.h> 


std::string KS_UTF8_to_ANSI (const char* szUTF8)
{
	if (szUTF8 == NULL)
		return "" ;
	int     nLen = ::MultiByteToWideChar (CP_UTF8, 0, szUTF8, -1, NULL, 0) ;
	WCHAR   * pWstr = new WCHAR[nLen+1] ;
	ZeroMemory (pWstr, sizeof(WCHAR) * (nLen+1)) ;
	::MultiByteToWideChar (CP_UTF8, 0, szUTF8, -1, pWstr, nLen) ;
	std::string strAnsi (_bstr_t((wchar_t*)pWstr)) ;
	delete[] pWstr ;
	return strAnsi ;
}
size_t GetResult(void *buffer, size_t size, size_t nmemb, void *stream)
{
	long szdata = size*nmemb;
	string strTmp = KS_UTF8_to_ANSI((char*)buffer);
 	CString strData = CA2W(strTmp.c_str());
	CString* pstrdata = (CString*)stream;
	if (pstrdata)
	{
		*pstrdata += strData;
	}
	return nmemb*size;
}

bool getUrl(char *filename)
{
    CURL *curl;
    CURLcode res;
    FILE *fp;
    if ((fp = fopen(filename, "w+")) == NULL)  // 返回结果用文件存储
        return false;
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: Agent-007");
    curl = curl_easy_init();    // 初始化
    if (curl)
    {
        //curl_easy_setopt(curl, CURLOPT_PROXY, "10.99.60.201:8080");// 代理
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);// 改协议头
        curl_easy_setopt(curl, CURLOPT_URL,"http://www.baidu.com");
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp); //将返回的http头输出到fp指向的文件
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, fp); //将返回的html主体数据输出到fp指向的文件
        res = curl_easy_perform(curl);   // 执行
        if (res != 0) {

            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
        }
        fclose(fp);
        return true;
    }
}
bool postUrl(char *filename)
{

	CURLcode res;
	CURL *pDwHand = curl_easy_init();
	int iErrorCode = -1;
	CString strData;
	string strUrl("http://localhost:2774/Handler.ashx?action=getgamelist");
	string strParam="action=getgamelist";
	if (pDwHand)
	{
		//curl_easy_setopt(pDwHand, CURLOPT_SSL_VERIFYHOST, 0L);
		//curl_easy_setopt(pDwHand, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(pDwHand, CURLOPT_TIMEOUT,30);
		
		curl_easy_setopt(pDwHand, CURLOPT_URL,strUrl.c_str());
		curl_easy_setopt(pDwHand, CURLOPT_POSTFIELDS,strParam.c_str());//POSTQUOTE   POSTFIELDS
		curl_easy_setopt(pDwHand, CURLOPT_WRITEFUNCTION, GetResult);
		curl_easy_setopt(pDwHand, CURLOPT_WRITEDATA, &strData);
		//curl_easy_setopt(pDwHand, CURLOPT_VERBOSE, 1L);
		res = curl_easy_perform(pDwHand);
		if (CURLE_OK == res) 
		{

		}
		else if (res == CURLE_OPERATION_TIMEDOUT)
		{
			iErrorCode = 1;//超时
		}
	}
	curl_easy_cleanup(pDwHand);
	return strData;
}
int main(void)
{
    //getUrl("D:1.txt");
    postUrl("D:1.txt");
}

