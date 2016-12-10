////////////////////////////////////////////////////////////
//
// The MIT License (MIT)
//
// STP - SFML TMX Parser
// Copyright (c) 2013-2014 Manuel Sabogal
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
////////////////////////////////////////////////////////////

// TODO : Use mini-z

#ifndef KE_COMPRESSION_HPP
#define KE_COMPRESSION_HPP

#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>

#include <zlib.h>

#include "../ExtLibs/pugixml.hpp"

namespace ke
{

bool base64_encode(std::string& data);
bool base64_decode(std::string& data);
bool decompressString(std::string& data);
bool compressString(std::string& data);

bool compress(std::string& data);
bool decompress(std::string& data);

bool is_valid_base64(unsigned char c);

void loadCompressedXml(std::string const& filename, pugi::xml_document& doc);
void saveCompressedXml(std::string const& filename, pugi::xml_document const& doc);

} // namespace ke

#endif // KE_COMPRESSION_HPP
