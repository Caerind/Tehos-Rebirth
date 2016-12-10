#include "Compression.hpp"

namespace ke
{

const std::string base64_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                 "abcdefghijklmnopqrstuvwxyz"
                                 "0123456789+/";

bool base64_encode(std::string& data)
{
    int count = 0;
    unsigned char input_bytes[3] = { '\0', '\0', '\0' };
    unsigned char byte_array[4];
    std::string result;
    for (unsigned int i = 0; i < data.size(); ++i)
    {
        input_bytes[count++] = data[i];
        if (count == 3 || (i == data.size() - 1))
        {
            byte_array[0] = input_bytes[0] >> 2;
            byte_array[1] = ((input_bytes[0] & 0x3) << 4) | (input_bytes[1] >> 4);
            byte_array[2] = ((input_bytes[1] & 0xf) << 2) | (input_bytes[2] >> 6);
            byte_array[3] = input_bytes[2] & 0x3f;
            memset(input_bytes, '\0', 3);
            for (int j = 0; j < count + 1; j++)
                result += base64_table[byte_array[j]];
            if (count != 3)
                for (int i = count; i < 3; ++i)
                    result += '=';
            else
                count = 0;
        }
    }
    data = result;
    return true;
}

bool base64_decode(std::string& data)
{
    int count = 0;
    unsigned char input_bytes[4] = { '\0', '\0', '\0', '\0' };
    unsigned char byte_array[3];
    std::string result;
    for (unsigned int i = 0; i < data.size(); ++i)
    {
        if (is_valid_base64(data[i]))
            input_bytes[count++] = static_cast<unsigned char>(base64_table.find(data[i]));
        if (count == 4 || data[i] == '=')
        {
            byte_array[0] = (input_bytes[0] << 2) | ((input_bytes[1] & 0x30) >> 4);
            byte_array[1] = ((input_bytes[1] & 0xf) << 4) | ((input_bytes[2] & 0x3c) >> 2);
            byte_array[2] = ((input_bytes[2] & 0x3) << 6) | input_bytes[3];
            memset(input_bytes, '\0', 4);
            for (int j = 0; j < count - 1; j++)
                result += byte_array[j];
            if (count != 4)
                break;
            else
                count = 0;
        }
    }
    data = result;
    return true;
}

bool decompressString(std::string& data)
{
    z_stream zstream;
    std::string outstring;
    zstream.zalloc = Z_NULL;
    zstream.zfree = Z_NULL;
    zstream.opaque = Z_NULL;
    zstream.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(data.data()));
    zstream.avail_in = data.size();
    int result;
    result = inflateInit2(&zstream, 15 + 32);
	char outbuffer[32768];
    if (result != Z_OK)
    {
        return false;
    }
    do
    {
        zstream.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zstream.avail_out = sizeof(outbuffer);
        result = inflate(&zstream, Z_SYNC_FLUSH);
        switch (result)
        {
            case Z_NEED_DICT:
            case Z_STREAM_ERROR:
                result = Z_DATA_ERROR;
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                inflateEnd(&zstream);
                return false;
        }
        if (outstring.size() < zstream.total_out)
        {
            outstring.append(outbuffer, zstream.total_out - outstring.size());
        }
    } while (result != Z_STREAM_END);
    if (zstream.avail_in != 0)
    {
        return false;
    }
    inflateEnd(&zstream);
    data = outstring;
    return true;
}

bool compressString(std::string& data)
{
    z_stream zs; // z_stream is zlib's control structure
    memset(&zs, 0, sizeof(zs));
    if (deflateInit(&zs, Z_BEST_COMPRESSION) != Z_OK)
    {
        return false;
    }
    zs.next_in = (Bytef*)data.data();
    zs.avail_in = data.size(); // set the z_stream's input
    int ret;
    char outbuffer[32768];
    std::string outstring;
    // retrieve the compressed bytes blockwise
    do
    {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);
        ret = deflate(&zs, Z_FINISH);
        if (outstring.size() < zs.total_out)
        {
            // append the block to the output string
            outstring.append(outbuffer, zs.total_out - outstring.size());
        }
    } while (ret == Z_OK);
    deflateEnd(&zs);
    if (ret != Z_STREAM_END) // an error occurred that was not EOF
    {
        return false;
    }
    data = outstring;
    return true;
}

bool compress(std::string& data)
{
    std::string d = data;
    if (compressString(d))
    {
        if (base64_encode(d))
        {
            data = d;
            return true;
        }
    }
    return false;
}

bool decompress(std::string& data)
{
    std::string d = data;
    if (base64_decode(d))
    {
        if (decompressString(d))
        {
            data = d;
            return true;
        }
    }
    return false;
}

bool is_valid_base64(unsigned char c)
{
    return (isalnum(c) || (c == '+') || (c == '/'));
}

void loadCompressedXml(std::string const& filename, pugi::xml_document& doc)
{
    std::ifstream iFile(filename);
    std::ofstream oFile(".temp");
    if (iFile && oFile)
    {
        std::string s;
        while (std::getline(iFile,s))
        {
            decompress(s);
            oFile << s << std::endl;
        }
    }
    iFile.close();
    oFile.close();

    doc.load_file(".temp");

    remove(".temp");
}

void saveCompressedXml(std::string const& filename, pugi::xml_document const& doc)
{
    doc.save_file(".temp");

    std::ifstream iFile(".temp");
    std::ofstream oFile(filename);
    if (iFile && oFile)
    {
        std::string s;
        while (std::getline(iFile,s))
        {
            compress(s);
            oFile << s << std::endl;
        }
    }
    iFile.close();
    oFile.close();

    remove(".temp");
}

} // namespace ke