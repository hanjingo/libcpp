#include <gtest/gtest.h>
#include <codecvt>
#include <locale>
#include <libcpp/hardware/device.h>

std::string ws2s(const wchar_t* wstr) 
{
    if (!wstr) return "";
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(wstr);
}

void dev_print(device_info_t* info)
{
    std::cout << "{"
              << "path=" << info->path
              << ", vendor_id=" << info->vendor_id
              << ", product_id=" << info->product_id
              << ", serial_number=" << ws2s(info->serial_number)
              << ", release_number=" << info->release_number
              << ", manufacturer_string=" << ws2s(info->manufacturer_string)
              << ", product_string=" << ws2s(info->product_string)
              << ", usage_page=" << info->usage_page
              << ", usage=" << info->usage
              << ", interface_number=" << info->interface_number
              << ", bus_type=" << (int)(info->bus_type)
              << "}"
              << std::endl;
}

bool dev_range(device_info_t* info)
{
    dev_print(info);
    return true;
}

TEST(device, device_range)
{
    device_range(dev_range);
}