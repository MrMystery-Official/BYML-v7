#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "BinaryVectorReader.h"
#include "BinaryVectorWriter.h"

class BymlFile
{
public:

    using byte = unsigned char;

	enum class Type : byte
	{
        StringIndex = 0xa0,
        Array = 0xc0,
        Dictionary = 0xc1,
        KeyTable = 0xc2,
        Bool = 0xd0,
        Int32 = 0xd1,
        Float = 0xd2,
        UInt32 = 0xd3,
        Int64 = 0xd4,
        UInt64 = 0xd5,
        Double = 0xd6,
        Null = 0xff
	};

    enum class TableGeneration : byte
    {
        Auto = 0,
        Manual = 1
    };

	class Node
	{
    public:
        Node(BymlFile::Type Type, std::string Key = "");

        BymlFile::Type& GetType();
        std::string& GetKey();
        template<class T> T GetValue();
        template<class T> void SetValue(T Value);

        bool HasChild(std::string Key);
        BymlFile::Node* GetChild(std::string Key);
        BymlFile::Node* GetChild(int Index);
        void AddChild(BymlFile::Node& Node);
        std::vector<BymlFile::Node>& GetChildren();
    private:
        BymlFile::Type m_Type;
        std::string m_Key;
        std::vector<byte> m_Value;

        std::vector<BymlFile::Node> m_Children;
	};

	BymlFile(std::string Path);
	BymlFile(std::vector<byte>& Bytes);

    std::vector<BymlFile::Node>& GetNodes();
    BymlFile::Type& GetType();
    BymlFile::Node* GetNode(std::string Key);

    void AddHashKeyTableEntry(std::string Key);
    void AddStringTableEntry(std::string String);

    void GenerateHashKeyTable(BymlFile::Node* Node);
    void GenerateStringTable(BymlFile::Node* Node);

    std::vector<byte> ToBinary(BymlFile::TableGeneration TableGeneration);
    void WriteToFile(std::string Path, BymlFile::TableGeneration TableGeneration);
private:
    std::vector<BymlFile::Node> m_Nodes;
    std::vector<std::string> m_HashKeyTable;
    std::vector<std::string> m_StringTable;
    BymlFile::Type m_Type = BymlFile::Type::Null;
    uint32_t m_WriterLastOffset = 0;
    uint32_t m_WriterReservedDataOffset = 0;

    void ParseTable(BinaryVectorReader& Reader, std::vector<std::string>* Dest, int TableOffset);
    int AlignUp(int Value, int Size);
    bool IsNumber(const std::string& str);
    uint32_t GetStringTableIndex(std::string Value);
    uint32_t GetHashKeyTableIndex(std::string Value);
    void ParseNode(BinaryVectorReader& Reader, int Offset, BymlFile::Type Type, std::string Key, BymlFile::Node* Parent);
    void WriteNode(BinaryVectorWriter& Writer, uint32_t DataOffset, uint32_t Offset, BymlFile::Node* Node);
};