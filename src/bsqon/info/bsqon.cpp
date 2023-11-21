#include "bsqon.h"

namespace BSQON
{
    std::vector<std::pair<char32_t, UnicodeString>> s_escape_names_unicode = {
        {0, U"0;"},
        {1, U"SOH;"},
        {2, U"STX;"},
        {3, U"ETX;"},
        {4, U"EOT;"},
        {5, U"ENQ;"},
        {6, U"ACK;"},
        {7, U"a;"},
        {8, U"b;"},
        {9, U"t;"},
        {10, U"n;"},
        {11, U"v;"},
        {12, U"f;"},
        {13, U"r;"},
        {14, U"SO;"},
        {15, U"SI;"},
        {16, U"DLE;"},
        {17, U"DC1;"},
        {18, U"DC2;"},
        {19, U"DC3;"},
        {20, U"DC4;"},
        {21, U"NAK;"},
        {22, U"SYN;"},
        {23, U"ETB;"},
        {24, U"CAN;"},
        {25, U"EM;"},
        {26, U"SUB;"},
        {27, U"e;"},
        {28, U"FS;"},
        {29, U"GS;"},
        {30, U"RS;"},
        {31, U"US;"},
        {127, U"DEL;"},

        {32, U"space;"},
        {33, U"bang;"},
        {34, U"quote;"},
        {35, U"hash;"},
        {36, U"dollar;"},
        {37, U"percent;"},
        {38, U"amp;"},
        {39, U"apos;"},
        {40, U"lparen;"},
        {41, U"rparen;"},
        {42, U"star;"},
        {43, U"plus;"},
        {44, U"comma;"},
        {45, U"dash;"},
        {46, U"dot;"},
        {47, U"slash;"},
        {58, U"colon;"},
        {59, U"semicolon;"},
        {60, U"langle;"},
        {61, U"equal;"},
        {62, U"rangle;"},
        {63, U"question;"},
        {64, U"at;"}, 
        {91, U"lbracket;"},
        {92, U"backslash;"},
        {93, U"rbracket;"},
        {94, U"caret;"},
        {95, U"underscore;"},
        {96, U"backtick;"},
        {123, U"lbrace;"},
        {124, U"pipe;"},
        {125, U"rbrace;"},
        {126, U"tilde;"}
    };

    std::vector<std::pair<char, std::string>> s_escape_names_ascii = {
        {0, "0;"},
        {1, "SOH;"},
        {2, "STX;"},
        {3, "ETX;"},
        {4, "EOT;"},
        {5, "ENQ;"},
        {6, "ACK;"},
        {7, "a;"},
        {8, "b;"},
        {9, "t;"},
        {10, "n;"},
        {11, "v;"},
        {12, "f;"},
        {13, "r;"},
        {14, "SO;"},
        {15, "SI;"},
        {16, "DLE;"},
        {17, "DC1;"},
        {18, "DC2;"},
        {19, "DC3;"},
        {20, "DC4;"},
        {21, "NAK;"},
        {22, "SYN;"},
        {23, "ETB;"},
        {24, "CAN;"},
        {25, "EM;"},
        {26, "SUB;"},
        {27, "e;"},
        {28, "FS;"},
        {29, "GS;"},
        {30, "RS;"},
        {31, "US;"},
        {127, "DEL;"},

        {32, "space;"},
        {33, "bang;"},
        {34, "quote;"},
        {35, "hash;"},
        {36, "dollar;"},
        {37, "percent;"},
        {38, "amp;"},
        {39, "apos;"},
        {40, "lparen;"},
        {41, "rparen;"},
        {42, "star;"},
        {43, "plus;"},
        {44, "comma;"},
        {45, "dash;"},
        {46, "dot;"},
        {47, "slash;"},
        {58, "colon;"},
        {59, "semi;"},
        {60, "langle;"},
        {61, "equal;"},
        {62, "rangle;"},
        {63, "question;"},
        {64, "at;"}, 
        {91, "lbracket;"},
        {92, "backslash;"},
        {93, "rbracket;"},
        {94, "caret;"},
        {95, "underscore;"},
        {96, "backtick;"},
        {123, "lbrace;"},
        {124, "pipe;"},
        {125, "rbrace;"},
        {126, "tilde;"}
    };

    std::optional<char32_t> decodeHexEscape(std::string escc)
    {
        //u 1-4 digits and a ;
        if(escc.size() == 2 || 6 < escc.size()) {
            return std::nullopt;
        }

        uint32_t cval;
        auto sct = sscanf(escc.c_str(), "u%x;", &cval);
        if(sct != 1) {
            return std::nullopt;
        }
        else {
            return std::make_optional<char32_t>((char32_t)cval);
        }
    }

    UnicodeString resolveEscapeUnicodeFromCode(char32_t c)
    {
        auto ii = std::find_if(s_escape_names_unicode.cbegin(), s_escape_names_unicode.cend(), [c](const std::pair<char32_t, UnicodeString>& p) { return p.first == c; });
        return ii->second;
    }

    char32_t resolveEscapeUnicodeFromName(const UnicodeString& name)
    {
        auto ii = std::find_if(s_escape_names_unicode.cbegin(), s_escape_names_unicode.cend(), [name](const std::pair<char32_t, UnicodeString>& p) { return p.second == name; });
        return ii->first;
    }

    std::string resolveEscapeASCIIFromCode(char c)
    {
        auto ii = std::find_if(s_escape_names_ascii.cbegin(), s_escape_names_ascii.cend(), [c](const std::pair<char, std::string>& p) { return p.first == c; });
        return ii->second;
    }

    char resolveEscapeASCIIFromName(const std::string& name)
    {
        auto ii = std::find_if(s_escape_names_ascii.cbegin(), s_escape_names_ascii.cend(), [name](const std::pair<char, std::string>& p) { return p.second == name; });
        return ii->first;
    }

    std::optional<UnicodeString> StringValue::unescapeString(const uint8_t* bytes, size_t length)
    {
        //assume string has "..." so we need to remove them

        std::string acc;
        for(size_t i = 1; i < length - 1; ++i) {
            uint8_t c = bytes[i];

            if(c == '%') {
                auto sc = std::find(bytes + i, bytes + length, ';');
                if(sc == bytes + length) {
                    return std::nullopt;
                }

                auto escc = std::string(bytes + i + 1, sc);
                if(escc[0] == 'u') {
                    //it should be a hex number of 1-4 digits
                    auto esc = decodeHexEscape(escc);
                    if(!esc.has_value()) {
                        return std::nullopt;
                    }

                    acc = std::move(acc) + std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>{}.to_bytes(esc.value());
                }
                else {
                    auto esc = resolveEscapeASCIIFromName(escc);
                    if(esc == 0) {
                        return std::nullopt;
                    }

                    acc = std::move(acc) + esc;
                }

                i += escc.size();
            }
            else {
                acc = std::move(acc) + (char)c;
            }
        }

        return std::make_optional(std::move(std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>{}.from_bytes(acc)));
    }

    std::vector<uint8_t> StringValue::escapeString(const UnicodeString& sv)
    {
        UnicodeString acc = U"\"";
        for(auto ii = sv.cbegin(); ii != sv.cend(); ++ii) {
            char32_t c = *ii;

            if(c == U'%' || c == U'"' || c == U'\n' || c == U'\t' || c == U'\r') {
                acc = std::move(acc) + resolveEscapeUnicodeFromCode(c);
            }
            else {
                acc = std::move(acc) + c;
            }
        }
        acc = std::move(acc) + U"\"";

        std::string utf8 = std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>{}.to_bytes(acc);

        std::vector<uint8_t> res(utf8.size());
        std::transform(utf8.cbegin(), utf8.cend(), res.begin(), [](char c) { return (uint8_t)c; });

        return res;
    }

    StringValue* StringValue::createFromParse(const Type* vtype, SourcePos spos, const uint8_t* bytes, size_t length)
    {
        auto sv = std::move(StringValue::unescapeString(bytes, length));
        if(!sv.has_value()) {
            return nullptr;
        }

        return new StringValue(vtype, spos, std::move(sv.value()));
    }

    std::optional<std::string> ASCIIStringValue::unescapeString(const uint8_t* bytes, size_t length)
    {
        //assume string has '...' (or `...`) so we need to remove them

        std::string acc;
        for(size_t i = 1; i < length - 1; ++i) {
            char c = bytes[i];

            if(c == '%') {
                auto sc = std::find(bytes + i, bytes + length, ';');
                if(sc == bytes + length) {
                    return std::nullopt;
                }

                auto escc = std::string(bytes + i + 1, sc);
                if(escc[0] == 'u') {
                    //it should be a hex number of 1-4 digits
                    auto esc = decodeHexEscape(escc);
                    if(!esc.has_value() || esc.value() > 127) {
                        return std::nullopt;
                    }

                    acc = std::move(acc) + (char)esc.value();
                }
                else {
                    auto esc = resolveEscapeASCIIFromName(escc);
                    if(esc == 0) {
                        return std::nullopt;
                    }

                    acc = std::move(acc) + esc;
                }

                i += escc.size();
            }
            else {
                acc = std::move(acc) + (char)c;
            }
        }

        return std::make_optional(std::move(acc));
    }

    std::vector<uint8_t> ASCIIStringValue::escapeString(const std::string& sv)
    {
        std::string acc = "'";
        for(auto ii = sv.cbegin(); ii != sv.cend(); ++ii) {
            char c = *ii;

            if(c == '%' || c == '\'' || c == '\n' || c == '\t' || c == '\r') {
                acc = std::move(acc) + resolveEscapeASCIIFromCode(c);
            }
            else {
                acc = std::move(acc) + c;
            }
        }
        acc = std::move(acc) + "'";

        std::vector<uint8_t> res(acc.size());
        std::transform(acc.cbegin(), acc.cend(), res.begin(), [](char c) { return (uint8_t)c; });

        return res;
    }

    ASCIIStringValue* ASCIIStringValue::createFromParse(const Type* vtype, SourcePos spos, const uint8_t* bytes, size_t length)
    {
        auto sv = std::move(ASCIIStringValue::unescapeString(bytes, length));
        if(!sv.has_value()) {
            return nullptr;
        }

        return new ASCIIStringValue(vtype, spos, std::move(sv.value()));
    }

    uint8_t ByteBufferValue::extractByteValue(char hb, char lb)
    {
        uint8_t h = ('0' <= hb && hb <= '9') ? (hb - '0') : (hb - 'a' + 10);
        uint8_t l = ('0' <= lb && lb <= '9') ? (lb - '0') : (lb - 'a' + 10);

        return (h << 4) | l;
    }

    ByteBufferValue* ByteBufferValue::createFromParse(const Type* vtype, SourcePos spos, const char* chars)
    {
        auto bblen = strlen(chars) - 4; //0x[...]
        
        const char* curr = chars + 3;
        const char* bbend = chars + bblen;

        if(bblen % 2 != 0) {
            return nullptr;
        }

        std::vector<uint8_t> buff;
        buff.reserve(bblen / 2);

        while(curr != bbend) {
            auto hb = *curr++;
            auto lb = *curr++;
            
            uint8_t bv = ByteBufferValue::extractByteValue(hb, lb);
            buff.push_back(bv);
        }

        return new ByteBufferValue(vtype, spos, std::move(buff));
    }

    StringOfValue* StringOfValue::createFromParse(const Type* vtype, SourcePos spos, const uint8_t* bytes, size_t length, const BSQRegex* validator)
    {
        auto str = StringValue::unescapeString(bytes, length);
        if(!str.has_value()) {
            return nullptr;
        }

        return validator->test(&str.value()) ? new StringOfValue(vtype, spos, std::move(str.value())) : nullptr;
    }

    ASCIIStringOfValue* ASCIIStringOfValue::createFromParse(const Type* vtype, SourcePos spos, const uint8_t* bytes, size_t length, const BSQRegex* validator)
    {
        auto str = std::move(ASCIIStringValue::unescapeString(bytes, length));
        if(!str.has_value()) {
            return nullptr;
        }

        return validator->test(&str.value()) ? new ASCIIStringOfValue(vtype, spos, std::move(str.value())) : nullptr;
    }

    PathValue* PathValue::createFromParse(const Type* vtype, SourcePos spos, const uint8_t* chars, size_t length, const BSQPath* validator)
    {
        auto str = std::move(ASCIIStringValue::unescapeString(chars, length));
        if(!str.has_value()) {
            return nullptr;
        }

        return validator->test(str.value()) ? new PathValue(vtype, spos, std::move(str.value())) : nullptr;
    }

    PathFragmentValue* PathFragmentValue::createFromParse(const Type* vtype, SourcePos spos, const uint8_t* chars, size_t length, const BSQPath* validator)
    {
        //skip the leading 'f'
        auto str = std::move(ASCIIStringValue::unescapeString(chars + 1, length - 1));
        if(!str.has_value()) {
            return nullptr;
        }

        return validator->test(str.value()) ? new PathFragmentValue(vtype, spos, std::move(str.value())) : nullptr;
    }

    PathGlobValue* PathGlobValue::createFromParse(const Type* vtype, SourcePos spos, const uint8_t* chars, size_t length, const BSQPath* validator)
    {
        //skip the leading 'g'
        auto str = std::move(ASCIIStringValue::unescapeString(chars + 1, length - 1));
        if(!str.has_value()) {
            return nullptr;
        }

        return validator->test(str.value()) ? new PathGlobValue(vtype, spos, std::move(str.value())) : nullptr;
    }

    std::vector<TypeKey> s_known_key_order = {
        "None",
        "Bool",
        "Nat",
        "Int",
        "BigNat",
        "BigInt",
        "String",
        "ASCIIString",
        "UTCDateTime",
        "PlainDate",
        "PlainTime",
        "TickTime",
        "LogicalTime",
        "ISOTimeStamp",
        "UUIDv4",
        "UUIDv7",
        "SHAContentHash"
    };

    int Value::keyCompare(const Value* v1, const Value* v2)
    {
        if(v1->vtype->tkey != v2->vtype->tkey) {
            auto iter1 = std::find(s_known_key_order.cbegin(), s_known_key_order.cend(), v1->vtype->tkey);
            auto iter2 = std::find(s_known_key_order.cbegin(), s_known_key_order.cend(), v2->vtype->tkey);

            if(iter1 == s_known_key_order.cend() && iter2 == s_known_key_order.cend()) {
                return (v1->vtype->tkey < v2->vtype->tkey) ? -1 : 1;
            }
            else if (iter1 == s_known_key_order.cend()) {
                return 1;
            }
            else if (iter2 == s_known_key_order.cend()) {
                return -1;
            }
            else {
                return iter1 < iter2 ? -1 : 1;
            }
        }
        else {
            const std::string dtype = v1->vtype->tkey;

            if(v1->vtype->tag == TypeTag::TYPE_PRIMITIVE) {
                if(dtype == "None") {
                    return false;
                }
                else if(dtype == "Bool") {
                    return BoolValue::keyCompare(static_cast<const BoolValue*>(v1), static_cast<const BoolValue*>(v2));
                }
                else if(dtype == "Nat") {
                    return NatNumberValue::keyCompare(static_cast<const NatNumberValue*>(v1), static_cast<const NatNumberValue*>(v2));
                }
                else if(dtype == "Int") {
                    return IntNumberValue::keyCompare(static_cast<const IntNumberValue*>(v1), static_cast<const IntNumberValue*>(v2));
                }
                else if(dtype == "BigNat") {
                    return BigNatNumberValue::keyCompare(static_cast<const BigNatNumberValue*>(v1), static_cast<const BigNatNumberValue*>(v2));
                }
                else if(dtype == "BigInt") {
                    return BigIntNumberValue::keyCompare(static_cast<const BigIntNumberValue*>(v1), static_cast<const BigIntNumberValue*>(v2));
                }
                else if(dtype == "String") {
                    return StringValue::keyCompare(static_cast<const StringValue*>(v1), static_cast<const StringValue*>(v2));
                }
                else if(dtype == "ASCIIString") {
                    return ASCIIStringValue::keyCompare(static_cast<const ASCIIStringValue*>(v1), static_cast<const ASCIIStringValue*>(v2));
                }
                else if(dtype == "UUIDv4") {
                    return UUIDv4Value::keyCompare(static_cast<const UUIDv4Value*>(v1), static_cast<const UUIDv4Value*>(v2));
                }
                else if(dtype == "UUIDv7") {
                    return UUIDv7Value::keyCompare(static_cast<const UUIDv7Value*>(v1), static_cast<const UUIDv7Value*>(v2));
                }
                else if(dtype == "SHAContentHash") {
                    return SHAContentHashValue::keyCompare(static_cast<const SHAContentHashValue*>(v1), static_cast<const SHAContentHashValue*>(v2));
                }
                else if(dtype == "UTCDateTime") {
                    return UTCDateTimeValue::keyCompare(static_cast<const UTCDateTimeValue*>(v1), static_cast<const UTCDateTimeValue*>(v2));
                }
                else if(dtype == "PlainDate") {
                    return PlainDateValue::keyCompare(static_cast<const PlainDateValue*>(v1), static_cast<const PlainDateValue*>(v2));
                }
                else if(dtype == "PlainTime") {
                    return PlainTimeValue::keyCompare(static_cast<const PlainTimeValue*>(v1), static_cast<const PlainTimeValue*>(v2));
                }
                else if(dtype == "TickTime") {
                    return TickTimeValue::keyCompare(static_cast<const TickTimeValue*>(v1), static_cast<const TickTimeValue*>(v2));
                }
                else if(dtype == "LogicalTime") {
                    return LogicalTimeValue::keyCompare(static_cast<const LogicalTimeValue*>(v1), static_cast<const LogicalTimeValue*>(v2));
                }
                else {
                    //should be ISOTimestamp
                    return ISOTimeStampValue::keyCompare(static_cast<const ISOTimeStampValue*>(v1), static_cast<const ISOTimeStampValue*>(v2));
                }
            }
            else {
                switch (v1->vtype->tag)
                {
                case TypeTag::TYPE_ENUM:
                    return EnumValue::keyCompare(static_cast<const EnumValue*>(v1), static_cast<const EnumValue*>(v2));
                case TypeTag::TYPE_STRING_OF:
                    return StringOfValue::keyCompare(static_cast<const StringOfValue*>(v1), static_cast<const StringOfValue*>(v2));
                case TypeTag::TYPE_ASCII_STRING_OF:
                    return ASCIIStringOfValue::keyCompare(static_cast<const ASCIIStringOfValue*>(v1), static_cast<const ASCIIStringOfValue*>(v2));
                case TypeTag::TYPE_PATH:
                    return PathValue::keyCompare(static_cast<const PathValue*>(v1), static_cast<const PathValue*>(v2));
                case TypeTag::TYPE_PATH_FRAGMENT:
                    return PathFragmentValue::keyCompare(static_cast<const PathFragmentValue*>(v1), static_cast<const PathFragmentValue*>(v2));
                case TypeTag::TYPE_PATH_GLOB:
                    return PathGlobValue::keyCompare(static_cast<const PathGlobValue*>(v1), static_cast<const PathGlobValue*>(v2));
                default:
                    //it must be a typedecl
                    return Value::keyCompare(static_cast<const TypedeclValue*>(v1)->basevalue, static_cast<const TypedeclValue*>(v2)->basevalue);
                }
            }
        }
    }
}

