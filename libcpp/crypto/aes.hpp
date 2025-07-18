/*
 *  This file is part of libcpp.
 *  Copyright (C) 2025 hanjingo <hehehunanchina@live.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef AES_HPP
#define AES_HPP

// disable msvc safe check warning
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

// support deprecated api for low version openssl
#ifndef OPENSSL_SUPPRESS_DEPRECATED
#define OPENSSL_SUPPRESS_DEPRECATED
#endif

#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#ifndef AES_BUF_SIZE
#define AES_BUF_SIZE 4096
#endif

namespace libcpp
{

class aes
{
public:
    enum class algo 
    {
        aes_128_ecb,
        aes_192_ecb,
        aes_256_ecb,

        aes_128_gcm,
        aes_192_gcm,
        aes_256_gcm,

        aes_128_cbc,
        aes_192_cbc,
        aes_256_cbc,

        aes_128_cfb1,
        aes_192_cfb1,
        aes_256_cfb1,

        aes_128_cfb8,
        aes_192_cfb8,
        aes_256_cfb8,

        aes_128_cfb128,
        aes_192_cfb128,
        aes_256_cfb128,

        aes_128_cfb = aes_128_cfb128,
        aes_192_cfb = aes_192_cfb128,
        aes_256_cfb = aes_256_cfb128,

        aes_128_ofb,
        aes_192_ofb,
        aes_256_ofb,

        aes_128_ctr,
        aes_192_ctr,
        aes_256_ctr,

        aes_128_ccm,
        aes_192_ccm,
        aes_256_ccm,

        aes_128_xts,
        aes_256_xts,

        aes_128_wrap,
        aes_192_wrap,
        aes_256_wrap,

        aes_128_wrap_pad,
        aes_192_wrap_pad,
        aes_256_wrap_pad,

        aes_128_cbc_hmac_sha1,
        aes_256_cbc_hmac_sha1,
        aes_128_cbc_hmac_sha256,
        aes_256_cbc_hmac_sha256,

# ifndef OPENSSL_NO_OCB
        aes_128_ocb,
        aes_192_ocb,
        aes_256_ocb,
#endif
    };

    enum class padding
    {
        aes_pkcs5_padding,
        aes_pkcs7_padding,
        aes_zero_padding,
        aes_iso10126_padding,
        aes_ansix923_padding,
        aes_iso_iec_7816_4_padding,
        aes_no_padding
    };

public:
    static bool encode(unsigned char* dst,
                       std::size_t& dst_len,
                       const unsigned char* src, 
                       const std::size_t src_len, 
                       const unsigned char* key, 
                       const std::size_t key_len,
                       const algo algorithm = algo::aes_256_ecb,
                       const padding pad_style = padding::aes_zero_padding,
                       const unsigned char* iv = nullptr,
                       const std::size_t iv_len = 16)
    {
        if (!is_plain_valid(algorithm, pad_style, src_len) || !is_key_valid(algorithm, key, key_len) || !is_iv_valid(algorithm, iv, iv_len))
            return false;

        std::vector<unsigned char> padded_src;
        std::size_t padded_len;
        _pad_block(padded_src, padded_len, algorithm, pad_style, src, src_len);

        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx)
            return false;

        // preinit iv & iv_len
        if (1 != EVP_EncryptInit_ex(ctx, _select_cipher(algorithm), NULL, NULL, NULL)) 
        {
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }
        if (is_aead_mode(algorithm)) 
        {
            if (iv && iv_len > 0) 
            {
                if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, static_cast<int>(iv_len), NULL)) 
                {
                    EVP_CIPHER_CTX_free(ctx);
                    return false;
                }
            }
        }

        if (1 != EVP_EncryptInit_ex(ctx, _select_cipher(algorithm), NULL, key, iv)) 
        {
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }

        // disable openssl auto padding
        if (!is_aead_mode(algorithm))
            EVP_CIPHER_CTX_set_padding(ctx, 0);

        int outlen1 = 0, outlen2 = 0;
        if (1 != EVP_EncryptUpdate(ctx, dst, &outlen1, padded_src.data(), static_cast<int>(padded_len))) 
        {
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }

        if (1 != EVP_EncryptFinal_ex(ctx, dst + outlen1, &outlen2)) 
        {
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }

        dst_len = outlen1 + outlen2;

        // add tag
        if (is_aead_mode(algorithm))
        {
            unsigned char tag[16] = {0};
            int tag_len = 16;
            if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, tag_len, tag)) 
            {
                EVP_CIPHER_CTX_free(ctx);
                return false;
            }
            memcpy(dst + dst_len, tag, tag_len);
            dst_len += tag_len;
        }

        EVP_CIPHER_CTX_free(ctx);
        return true;
    }

    static bool encode(std::ostream& out,
                       std::istream& in,
                       const unsigned char* key, 
                       const std::size_t key_len,
                       const algo algorithm = algo::aes_256_ecb,
                       const padding pad_style = padding::aes_zero_padding,
                       const unsigned char* iv = nullptr,
                       const std::size_t iv_len = 16)
    {
        if (!in || !out)
            return false;

        if (!is_plain_valid(algorithm, pad_style, in) || !is_key_valid(algorithm, key, key_len) || !is_iv_valid(algorithm, iv, iv_len))
            return false;
            
        int block_size = _get_block_size(algorithm);
        bool is_aead = is_aead_mode(algorithm);
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx)
            return false;

        if (1 != EVP_EncryptInit_ex(ctx, _select_cipher(algorithm), NULL, NULL, NULL)) 
        {
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }

        if (is_aead) 
        {
            if (iv && iv_len > 0) 
            {
                if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, static_cast<int>(iv_len), NULL)) 
                {
                    EVP_CIPHER_CTX_free(ctx);
                    return false;
                }
            }
        }

        if (1 != EVP_EncryptInit_ex(ctx, _select_cipher(algorithm), NULL, key, iv)) 
        {
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }

        if (!is_aead)
            EVP_CIPHER_CTX_set_padding(ctx, 0);

        std::vector<unsigned char> inbuf(AES_BUF_SIZE);
        std::vector<unsigned char> outbuf(AES_BUF_SIZE + block_size * 2);
        std::size_t total_read = 0;
        bool last_block = false;
        std::vector<unsigned char> last_plain;

        while (!last_block) 
        {
            in.read(reinterpret_cast<char*>(inbuf.data()), AES_BUF_SIZE);
            std::streamsize read_len = in.gcount();
            total_read += static_cast<std::size_t>(read_len);

            if (read_len < AES_BUF_SIZE)
                last_block = true;

            if (!last_block) 
            {
                int outlen = 0;
                if (1 != EVP_EncryptUpdate(ctx, outbuf.data(), &outlen, inbuf.data(), static_cast<int>(read_len))) 
                {
                    EVP_CIPHER_CTX_free(ctx);
                    return false;
                }
                out.write(reinterpret_cast<char*>(outbuf.data()), outlen);
            } else {
                last_plain.assign(inbuf.begin(), inbuf.begin() + static_cast<int>(read_len));
            }
        }

        std::vector<unsigned char> padded_plain;
        std::size_t padded_len;
        _pad_block(padded_plain, padded_len, algorithm, pad_style, last_plain.data(), last_plain.size());

        int outlen1 = 0, outlen2 = 0;
        if (padded_len > 0) 
        {
            if (1 != EVP_EncryptUpdate(ctx, outbuf.data(), &outlen1, padded_plain.data(), static_cast<int>(padded_len))) 
            {
                EVP_CIPHER_CTX_free(ctx);
                return false;
            }
            out.write(reinterpret_cast<char*>(outbuf.data()), outlen1);
        }

        if (1 != EVP_EncryptFinal_ex(ctx, outbuf.data(), &outlen2)) 
        {
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }
        
        if (outlen2 > 0)
            out.write(reinterpret_cast<char*>(outbuf.data()), outlen2);

        if (is_aead) 
        {
            unsigned char tag[16] = {0};
            int tag_len = 16;
            if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, tag_len, tag)) 
            {
                EVP_CIPHER_CTX_free(ctx);
                return false;
            }
            out.write(reinterpret_cast<char*>(tag), tag_len);
        }

        EVP_CIPHER_CTX_free(ctx);
        return true;
    }

    static bool encode(std::string& dst,
                       const std::string& src, 
                       const std::string& key, 
                       const algo algorithm = algo::aes_256_ecb,
                       const padding pad_style = padding::aes_zero_padding,
                       const std::string& iv = std::string())
    {
        std::size_t dst_len = encode_len_reserve(src.size(), algorithm);
        dst.resize(dst_len);
        const unsigned char* iv_ptr = (algorithm >= algo::aes_128_ecb && algorithm <= algo::aes_256_ecb) ? 
            nullptr : reinterpret_cast<const unsigned char*>(iv.c_str());
        const std::size_t iv_len = (iv_ptr == nullptr) ? 0 : iv.size();
        if (!encode(reinterpret_cast<unsigned char*>(const_cast<char*>(dst.data())), 
                    dst_len, 
                    reinterpret_cast<const unsigned char*>(src.c_str()), 
                    src.size(), 
                    reinterpret_cast<const unsigned char*>(key.c_str()), 
                    key.size(), 
                    algorithm,
                    pad_style,
                    iv_ptr,
                    iv_len))
            return false;

        dst.resize(dst_len);
        return true;
    }

    static bool encode_file(const char* dst_file_path,
                            const char* src_file_path, 
                            const unsigned char* key, 
                            const std::size_t key_len,
                            const algo algorithm = algo::aes_256_ecb,
                            const padding pad_style = padding::aes_zero_padding,
                            const unsigned char* iv = nullptr,
                            const std::size_t iv_len = 16)
    {
        std::ifstream in(src_file_path, std::ios::binary);
        std::ofstream out(dst_file_path, std::ios::binary);
        if (!in.is_open() || !out.is_open())
            return false;

        return encode(out, in, key, key_len, algorithm, pad_style, iv, iv_len);
    }

    static bool encode_file(const std::string& dst_file_path,
                            const std::string& src_file_path,
                            const std::string& key,
                            const algo algorithm = algo::aes_256_ecb,
                            const padding pad_style = padding::aes_zero_padding,
                            const std::string& iv = std::string())
    {
        const unsigned char* iv_ptr = (algorithm >= algo::aes_128_ecb && algorithm <= algo::aes_256_ecb) ? 
            nullptr : reinterpret_cast<const unsigned char*>(iv.c_str());
        const std::size_t iv_len = (iv_ptr == nullptr) ? 0 : iv.size();
        return encode_file(dst_file_path.c_str(),
                           src_file_path.c_str(),
                           reinterpret_cast<const unsigned char*>(key.c_str()),
                           key.size(),
                           algorithm,
                           pad_style,
                           iv_ptr,
                           iv_len);
    }

    static bool decode(unsigned char* dst,
                       std::size_t& dst_len,
                       const unsigned char* src,
                       const std::size_t src_len,
                       const unsigned char* key,
                       const std::size_t key_len,
                       const algo algorithm = algo::aes_256_ecb,
                       const padding pad_style = padding::aes_zero_padding,
                       const unsigned char* iv = nullptr,
                       const std::size_t iv_len = 16)
    {
        if (!is_key_valid(algorithm, key, key_len) || !is_iv_valid(algorithm, iv, iv_len))
            return false;

        int block_size = _get_block_size(algorithm);
        if (pad_style == padding::aes_no_padding && src_len % block_size != 0) 
            return false;

        std::size_t cipher_len = src_len;
        const unsigned char* tag_ptr = nullptr;
        if (is_aead_mode(algorithm) && src_len > 16) 
        {
            cipher_len = src_len - 16;
            tag_ptr = src + cipher_len;
        }

        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx)
            return false;

        if (1 != EVP_DecryptInit_ex(ctx, _select_cipher(algorithm), NULL, NULL, NULL)) 
        {
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }
        if (is_aead_mode(algorithm)) 
        {
            if (iv && iv_len > 0) 
            {
                if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, static_cast<int>(iv_len), NULL)) 
                {
                    EVP_CIPHER_CTX_free(ctx);
                    return false;
                }
            }
        }

        if (1 != EVP_DecryptInit_ex(ctx, NULL, NULL, key, iv)) 
        {
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }

        if (!is_aead_mode(algorithm))
            EVP_CIPHER_CTX_set_padding(ctx, 0);

        if (is_aead_mode(algorithm) && tag_ptr) 
        {
            if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, 16, (void*)tag_ptr)) {
                EVP_CIPHER_CTX_free(ctx);
                return false;
            }
        }

        int outlen1 = 0, outlen2 = 0;
        if (1 != EVP_DecryptUpdate(ctx, dst, &outlen1, src, static_cast<int>(cipher_len))) 
        {
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }

        if (1 != EVP_DecryptFinal_ex(ctx, dst + outlen1, &outlen2)) 
        {
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }

        dst_len = outlen1 + outlen2;
        _unpad_block(dst, dst_len, algorithm, pad_style, block_size);

        EVP_CIPHER_CTX_free(ctx);
        return true;
    }

    static bool decode(std::ostream& out,
                       std::istream& in,
                       const unsigned char* key, 
                       const std::size_t key_len,
                       const algo algorithm = algo::aes_256_ecb,
                       const padding pad_style = padding::aes_zero_padding,
                       const unsigned char* iv = nullptr,
                       const std::size_t iv_len = 16)
    {
        if (!in || !out)
            return false;

        if (!is_key_valid(algorithm, key, key_len) || !is_iv_valid(algorithm, iv, iv_len))
            return false;

        std::streamsize file_size = in.tellg();
        in.seekg(0, std::ios::beg);
        int block_size = _get_block_size(algorithm);
        bool is_aead = is_aead_mode(algorithm);

        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx)
            return false;

        unsigned char tag[16] = {0};
        std::streamsize cipher_len = file_size;
        if (is_aead && file_size > 16) 
        {
            cipher_len = file_size - 16;
            in.seekg(cipher_len, std::ios::beg);
            in.read(reinterpret_cast<char*>(tag), 16);
            in.seekg(0, std::ios::beg);
        }

        if (1 != EVP_DecryptInit_ex(ctx, _select_cipher(algorithm), NULL, NULL, NULL)) 
        {
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }

        if (is_aead) 
        {
            if (iv && iv_len > 0) 
            {
                if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, static_cast<int>(iv_len), NULL)) 
                {
                    EVP_CIPHER_CTX_free(ctx);
                    return false;
                }
            }
        }

        if (1 != EVP_DecryptInit_ex(ctx, _select_cipher(algorithm), NULL, key, iv)) 
        {
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }

        if (!is_aead)
            EVP_CIPHER_CTX_set_padding(ctx, 0);
        
        if (is_aead) 
        {
            if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, 16, tag)) 
            {
                EVP_CIPHER_CTX_free(ctx);
                return false;
            }
        }

        std::vector<unsigned char> inbuf(AES_BUF_SIZE + block_size * 2);
        std::vector<unsigned char> outbuf(AES_BUF_SIZE + block_size * 2);
        std::vector<unsigned char> last_cipher;
        std::streamsize total_read = 0;
        bool last_block = false;
        while (total_read < cipher_len) 
        {
            std::streamsize to_read = std::min(static_cast<std::streamsize>(AES_BUF_SIZE), cipher_len - total_read);
            in.read(reinterpret_cast<char*>(inbuf.data()), to_read);
            std::streamsize read_len = in.gcount();
            total_read += read_len;

            if (total_read == cipher_len)
                last_block = true;

            if (!last_block) 
            {
                int outlen = 0;
                if (1 != EVP_DecryptUpdate(ctx, outbuf.data(), &outlen, inbuf.data(), static_cast<int>(read_len)))
                {
                    EVP_CIPHER_CTX_free(ctx);
                    return false;
                }
                out.write(reinterpret_cast<char*>(outbuf.data()), outlen);
            } else {
                last_cipher.assign(inbuf.begin(), inbuf.begin() + static_cast<std::size_t>(read_len));
            }
        }

        int outlen1 = 0, outlen2 = 0;
        if (!last_cipher.empty()) 
        {
            if (1 != EVP_DecryptUpdate(ctx, outbuf.data(), &outlen1, last_cipher.data(), static_cast<int>(last_cipher.size()))) 
            {
                EVP_CIPHER_CTX_free(ctx);
                return false;
            }
        }

        if (1 != EVP_DecryptFinal_ex(ctx, outbuf.data() + outlen1, &outlen2)) 
        {
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }

        std::size_t plain_len = outlen1 + outlen2;
        _unpad_block(outbuf.data(), plain_len, algorithm, pad_style, block_size);

        if (plain_len > 0)
            out.write(reinterpret_cast<char*>(outbuf.data()), plain_len);

        EVP_CIPHER_CTX_free(ctx);
        return true;
    }

    static bool decode(std::string& dst,
                       const std::string& src, 
                       const std::string& key, 
                       const algo algorithm = algo::aes_256_ecb,
                       const padding pad_style = padding::aes_zero_padding,
                       const std::string& iv = std::string())
    {
        dst.resize(decode_len_reserve(src.size()));
        std::size_t dst_len = dst.size();
        const unsigned char* iv_ptr = (algorithm >= algo::aes_128_ecb && algorithm <= algo::aes_256_ecb) ? 
            nullptr : reinterpret_cast<const unsigned char*>(iv.c_str());
        const std::size_t iv_len = (iv_ptr == nullptr) ? 0 : iv.size();
        if (!decode(reinterpret_cast<unsigned char*>(const_cast<char*>(dst.data())),
                    dst_len, 
                    reinterpret_cast<const unsigned char*>(src.c_str()), 
                    src.size(), 
                    reinterpret_cast<const unsigned char*>(key.c_str()), 
                    key.size(), 
                    algorithm,
                    pad_style,
                    iv_ptr,
                    iv_len))
        {
            dst.clear();
            return false;
        }

        dst.resize(dst_len);
        return true;
    }

    static bool decode_file(const char* dst_file_path,
                            const char* src_file_path, 
                            const unsigned char* key, 
                            const std::size_t key_len,
                            const algo algorithm = algo::aes_256_ecb,
                            const padding pad_style = padding::aes_zero_padding,
                            const unsigned char* iv = nullptr,
                            const std::size_t iv_len = 16)
    {
        std::ifstream in(src_file_path, std::ios::binary | std::ios::ate);
        std::ofstream out(dst_file_path, std::ios::binary);
        if (!in.is_open() || !out.is_open())
            return false;

        return decode(out, in, key, key_len, algorithm, pad_style, iv, iv_len);
    }

    static bool decode_file(const std::string& dst_file_path,
                            const std::string& src_file_path,
                            const std::string& key,
                            const algo algorithm = algo::aes_256_ecb,
                            const padding pad_style = padding::aes_zero_padding,
                            const std::string& iv = std::string())
    {
        const unsigned char* iv_ptr = (algorithm >= algo::aes_128_ecb && algorithm <= algo::aes_256_ecb) ? 
            nullptr : reinterpret_cast<const unsigned char*>(iv.c_str());
        const std::size_t iv_len = (iv_ptr == nullptr) ? 0 : iv.size();
        return decode_file(dst_file_path.c_str(),
                           src_file_path.c_str(),
                           reinterpret_cast<const unsigned char*>(key.c_str()),
                           key.size(),
                           algorithm,
                           pad_style,
                           iv_ptr,
                           iv_len);
    }

    // reserve encode dst buf size
	static std::size_t encode_len_reserve(const std::size_t src_len, 
											const algo algorithm = algo::aes_256_ecb)
	{
        int block_size = _get_block_size(algorithm);
        if (block_size >= 8)
            return ((src_len / block_size) + 2) * block_size;

        return src_len + 16;
	}

	// reserve decode dst buf size
	static std::size_t decode_len_reserve(const std::size_t src_len)
	{
		return src_len;
	}

    // check key format
    static bool is_key_valid(const algo algorithm, const unsigned char* key, const std::size_t key_len)
    {
        if (key == nullptr) 
            return false;

        switch (algorithm) {
        case algo::aes_128_ecb: 
        case algo::aes_128_cbc: 
        case algo::aes_128_cfb1: 
        case algo::aes_128_cfb8: 
        case algo::aes_128_cfb128:
        case algo::aes_128_ofb: 
        case algo::aes_128_ctr: 
        case algo::aes_128_ccm: 
        case algo::aes_128_gcm: 
        case algo::aes_128_xts:
        case algo::aes_128_wrap: 
        case algo::aes_128_wrap_pad:
        case algo::aes_128_cbc_hmac_sha1: 
        case algo::aes_128_cbc_hmac_sha256:
# ifndef OPENSSL_NO_OCB
        case algo::aes_128_ocb:
# endif
            return key_len == 16;
        case algo::aes_192_ecb: 
        case algo::aes_192_cbc: 
        case algo::aes_192_cfb1: 
        case algo::aes_192_cfb8: 
        case algo::aes_192_cfb128:
        case algo::aes_192_ofb: 
        case algo::aes_192_ctr: 
        case algo::aes_192_ccm: 
        case algo::aes_192_gcm:
        case algo::aes_192_wrap: 
        case algo::aes_192_wrap_pad:
# ifndef OPENSSL_NO_OCB
        case algo::aes_192_ocb:
# endif
            return key_len == 24;
        case algo::aes_256_ecb: 
        case algo::aes_256_cbc: 
        case algo::aes_256_cfb1: 
        case algo::aes_256_cfb8: 
        case algo::aes_256_cfb128:
        case algo::aes_256_ofb: 
        case algo::aes_256_ctr: 
        case algo::aes_256_ccm: 
        case algo::aes_256_gcm: 
        case algo::aes_256_xts:
        case algo::aes_256_wrap: 
        case algo::aes_256_wrap_pad:
        case algo::aes_256_cbc_hmac_sha1: 
        case algo::aes_256_cbc_hmac_sha256:
# ifndef OPENSSL_NO_OCB
        case algo::aes_256_ocb:
# endif
            return key_len == 32;
    default:
            return false;
        }
    }

    // check iv format
    static bool is_iv_valid(const algo algorithm, const unsigned char* iv, const std::size_t iv_len)
    {
        if (algorithm >= algo::aes_128_ecb && algorithm <= algo::aes_256_ecb)
            return iv == nullptr;

        if (iv_len != 16)
            return false;

        return true;
    }

    // check plain
    static bool is_plain_valid(const algo algorithm, 
                               const padding pad_style, 
                               const std::size_t plain_len)
    {
        int block_sz = _get_block_size(algorithm);
        if (pad_style == padding::aes_no_padding && plain_len % block_sz != 0)
            return false;

        return true;
    }

    // check stream plain
    static bool is_plain_valid(const algo algorithm, const padding pad_style, std::istream& in)
    {
        if (!in)
            return false;

        if (pad_style == padding::aes_no_padding)
        {
            std::streampos current_pos = in.tellg();
            in.seekg(0, std::ios::end);
            std::streampos end_pos = in.tellg();
            in.seekg(current_pos);
            std::size_t total_length = static_cast<std::size_t>(end_pos - current_pos);
            int block_size = _get_block_size(algorithm);
            return total_length % block_size == 0;
        }

        return true;
    }

    // check stream mode
    static bool is_stream_mode(const algo algorithm)
    {
        switch (algorithm)
        {
        case algo::aes_128_cfb1:
        case algo::aes_192_cfb1:  
        case algo::aes_256_cfb1:
        case algo::aes_128_cfb8:
        case algo::aes_192_cfb8:
        case algo::aes_256_cfb8:
        case algo::aes_128_cfb128:
        case algo::aes_192_cfb128:
        case algo::aes_256_cfb128:
        case algo::aes_128_ofb:
        case algo::aes_192_ofb:
        case algo::aes_256_ofb:
        case algo::aes_128_ctr:
        case algo::aes_192_ctr:
        case algo::aes_256_ctr:
            return true;
        default:
            return false;
        }
    }

    // check aead mode
    static bool is_aead_mode(const algo algorithm)
    {
        switch (algorithm)
        {
        case algo::aes_128_gcm:
        case algo::aes_192_gcm:
        case algo::aes_256_gcm:
        case algo::aes_128_ccm:
        case algo::aes_192_ccm:
        case algo::aes_256_ccm:
            return true;
        default:
            return false;
        }
    }

private:
    static const EVP_CIPHER* _select_cipher(const algo algorithm)
    {
        switch (algorithm)
        {
        case algo::aes_128_ecb: { return EVP_aes_128_ecb(); }
        case algo::aes_192_ecb: { return EVP_aes_192_ecb(); }
        case algo::aes_256_ecb: { return EVP_aes_256_ecb(); }

        case algo::aes_128_gcm: { return EVP_aes_128_gcm(); }
        case algo::aes_192_gcm: { return EVP_aes_192_gcm(); }
        case algo::aes_256_gcm: { return EVP_aes_256_gcm(); }

        case algo::aes_128_cbc: { return EVP_aes_128_cbc(); }
        case algo::aes_192_cbc: { return EVP_aes_192_cbc(); }
        case algo::aes_256_cbc: { return EVP_aes_256_cbc(); }

        case algo::aes_128_cfb1: { return EVP_aes_128_cfb1(); }
        case algo::aes_192_cfb1: { return EVP_aes_192_cfb1(); }
        case algo::aes_256_cfb1: { return EVP_aes_256_cfb1(); }

        case algo::aes_128_cfb8: { return EVP_aes_128_cfb8(); }
        case algo::aes_192_cfb8: { return EVP_aes_192_cfb8(); }
        case algo::aes_256_cfb8: { return EVP_aes_256_cfb8(); }

        case algo::aes_128_cfb128: { return EVP_aes_128_cfb128(); }
        case algo::aes_192_cfb128: { return EVP_aes_192_cfb128(); }
        case algo::aes_256_cfb128: { return EVP_aes_256_cfb128(); }

        case algo::aes_128_ofb: { return EVP_aes_128_ofb(); }
        case algo::aes_192_ofb: { return EVP_aes_192_ofb(); }
        case algo::aes_256_ofb: { return EVP_aes_256_ofb(); }

        case algo::aes_128_ctr: { return EVP_aes_128_ctr(); }
        case algo::aes_192_ctr: { return EVP_aes_192_ctr(); }
        case algo::aes_256_ctr: { return EVP_aes_256_ctr(); }

        case algo::aes_128_ccm: { return EVP_aes_128_ccm(); }
        case algo::aes_192_ccm: { return EVP_aes_192_ccm(); } 
        case algo::aes_256_ccm: { return EVP_aes_256_ccm(); }

        case algo::aes_128_xts: { return EVP_aes_128_xts(); }
        case algo::aes_256_xts: { return EVP_aes_256_xts(); }

        case algo::aes_128_wrap: { return EVP_aes_128_wrap(); }
        case algo::aes_192_wrap: { return EVP_aes_192_wrap(); }
        case algo::aes_256_wrap: { return EVP_aes_256_wrap(); }

        case algo::aes_128_wrap_pad: { return EVP_aes_128_wrap_pad(); }
        case algo::aes_192_wrap_pad: { return EVP_aes_192_wrap_pad(); }
        case algo::aes_256_wrap_pad: { return EVP_aes_256_wrap_pad(); }

        case algo::aes_128_cbc_hmac_sha1: { return EVP_aes_128_cbc_hmac_sha1(); }
        case algo::aes_256_cbc_hmac_sha1: { return EVP_aes_256_cbc_hmac_sha1(); }
        case algo::aes_128_cbc_hmac_sha256: { return EVP_aes_128_cbc_hmac_sha256(); }
        case algo::aes_256_cbc_hmac_sha256: { return EVP_aes_256_cbc_hmac_sha256(); }

# ifndef OPENSSL_NO_OCB
        case algo::aes_128_ocb: { return EVP_aes_128_ocb(); }
        case algo::aes_192_ocb: { return EVP_aes_192_ocb(); }
        case algo::aes_256_ocb: { return EVP_aes_256_ocb(); }
#endif

        default: { return nullptr; }
        }
    }

    // get block size
    static int _get_block_size(const algo algorithm)
    {
        switch (algorithm) {
        case algo::aes_128_ecb:
        case algo::aes_192_ecb:
        case algo::aes_256_ecb:
        case algo::aes_128_cbc:
        case algo::aes_192_cbc:
        case algo::aes_256_cbc:
        case algo::aes_128_cfb128:
        case algo::aes_192_cfb128:
        case algo::aes_256_cfb128:
        case algo::aes_128_ofb:
        case algo::aes_192_ofb:
        case algo::aes_256_ofb:
        case algo::aes_128_ctr:
        case algo::aes_192_ctr:
        case algo::aes_256_ctr:
            return 16;
        case algo::aes_128_cfb8:
        case algo::aes_192_cfb8:
        case algo::aes_256_cfb8:
            return 1;
        case algo::aes_128_cfb1:
        case algo::aes_192_cfb1:
        case algo::aes_256_cfb1:
            return 1;
        default:
            return EVP_CIPHER_block_size(_select_cipher(algorithm));
        }
    }

    // pad block
    static void _pad_block(std::vector<unsigned char>& padded_src, 
                           std::size_t& padded_len,
                           const algo algorithm, 
                           const padding pad_style, 
                           const unsigned char* src, 
                           const std::size_t src_len)
    {
        int block_size = static_cast<int>(_get_block_size(algorithm));
        padded_len = src_len;
        if (pad_style != padding::aes_no_padding && block_size > 1) 
        {
            unsigned char pad_val = static_cast<unsigned char>(block_size - (src_len % block_size));
            if (pad_val == 0) 
                pad_val = static_cast<unsigned char>(block_size);

            padded_len = src_len + pad_val;
            padded_src.resize(padded_len, 0);
            memcpy(padded_src.data(), src, src_len);
            switch (pad_style) {
            case padding::aes_pkcs5_padding:
            case padding::aes_pkcs7_padding:
                for (std::size_t i = src_len; i < padded_len; ++i)
                    padded_src[i] = pad_val;
                break;
            case padding::aes_zero_padding:
                break;
            case padding::aes_iso10126_padding:
                for (std::size_t i = src_len; i < padded_len - 1; ++i)
                    padded_src[i] = static_cast<unsigned char>(rand() % 256);
                padded_src[padded_len - 1] = pad_val;
                break;
            case padding::aes_ansix923_padding:
                padded_src[padded_len - 1] = pad_val;
                break;
            case padding::aes_iso_iec_7816_4_padding:
                padded_src[src_len] = 0x80;
                break;
            default:
                break;
            }
        } else {
            // no padding
            padded_src.assign(src, src + src_len);
            padded_len = src_len;
        }
    }

    // unpad block
    static void _unpad_block(unsigned char* dst, 
                             std::size_t& dst_len,
                             const algo algorithm, 
                             const padding pad_style, 
                             const std::size_t block_size)
    {
        if (!is_aead_mode(algorithm) && pad_style != padding::aes_no_padding && block_size > 1) 
        {
            std::size_t unpad_len = dst_len;
            switch (pad_style) {
            case padding::aes_pkcs5_padding:
            case padding::aes_pkcs7_padding: {
                if (unpad_len == 0) 
                    break;

                unsigned char pad = dst[unpad_len - 1];
                if (pad == 0 || pad > block_size) 
                    break;

                bool valid = true;
                for (std::size_t i = unpad_len - pad; i < unpad_len; ++i)
                    if (dst[i] != pad) 
                        valid = false;

                if (valid) 
                    dst_len = unpad_len - pad;
                break;
            }
            case padding::aes_zero_padding: {
                while (dst_len > 0 && dst[dst_len - 1] == 0)
                    --dst_len;

                break;
            }
            case padding::aes_iso10126_padding: {
                if (unpad_len == 0) 
                    break;

                unsigned char pad = dst[unpad_len - 1];
                if (pad == 0 || pad > block_size) 
                    break;

                dst_len = unpad_len - pad;
                break;
            }
            case padding::aes_ansix923_padding: {
                if (unpad_len == 0) 
                    break;

                unsigned char pad = dst[unpad_len - 1];
                if (pad == 0 || pad > block_size) 
                    break;

                bool valid = true;
                for (std::size_t i = unpad_len - pad; i < unpad_len - 1; ++i)
                    if (dst[i] != 0) 
                        valid = false;

                if (valid) 
                    dst_len = unpad_len - pad;
                break;
            }
            case padding::aes_iso_iec_7816_4_padding: {
                int i = dst_len - 1;
                while (i >= 0 && dst[i] == 0) 
                    --i;

                if (i >= 0 && dst[i] == 0x80)
                    dst_len = static_cast<std::size_t>(i);
                
                break;
            }
            default:
                break;
            }
        }
    }

private:
    aes() = default;
    ~aes() = default;
    aes(const aes&) = delete;
    aes& operator=(const aes&) = delete;
    aes(aes&&) = delete;
    aes& operator=(aes&&) = delete;
};

}

#endif