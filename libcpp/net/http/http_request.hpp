#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <httplib.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__)
#include <Windows.h>
#endif

namespace libcpp
{

using http_request = httplib::Request;
using http_content_reader = httplib::ContentReader;

using http_params = httplib::Params;

using http_multipart_form_data_items = httplib::MultipartFormDataItems;
using http_multipart_form_data = httplib::MultipartFormData;

}

#endif