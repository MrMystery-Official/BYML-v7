# BYML v7 Reader/Writer

A fast C++ library to read and write BYML v7 files.
## Usage
First of all, include the `Byml.h` header file. To load a BYML, you can pass a path to a file or a vector of unsigned chars, which contains the file data directly. This is especially usefull when loading a with ZStandard compressed BYML.
```cpp
BymlFile BYML("Dungeon001_Static.bcett.byml");
```
To write the BYML, you use `WriteToFile`, which takes a path and a TableGeneration parameter. When working with large files, you can set the TableGeneration to Manual, which allowes you to edit the tables manually and there is no need to regenerate the whole table. You can also use `ToBinary`, to write the BYML into a vector of unsigned chars. Again, this is usefull when working with compression.
```cpp
BymlFile BYML("Dungeon001_Static.bcett.byml");
BYML.WriteToFile("Dungeon001_Static_New.bcett.byml", BymlFile::TableGeneration::Auto);
std::vector<unsigned char> Bytes = BYML.ToBinary(BymlFile::TableGeneration::Auto);
```
To read a node from the BYML, simplly run `GetNode` and pass the key. If you want to get a child of another node, run `GetChild` on the parent node. To `GetChild` you have to pass the key of the node or the index. To read the value of a node, run `GetValue`. Supported types are: `std::string, uint32_t, int32_t, uint64_t, int64_t, float, double, bool`.
```cpp
BymlFile BYML("Dungeon001_Static.bcett.byml");
BYML.GetNode("Actors")->GetChild(0)->GetChild("Gyaml")->GetValue<std::string>(); //Read an actors Gyaml in a Banc as a string
```
To modify a node, use the following methods:
```cpp
BymlFile BYML("Dungeon001_Static.bcett.byml");
BymlFile::Node* ActorNode = BYML.GetNode("Actors")->GetChild(0);
ActorNode->GetChild("Gyaml")->SetValue<std::string>("TBox_Dungeon_Iron");
ActorNode->GetChild("Gyaml")->GetType() = BymlFile::Type::StringIndex;
ActorNode->GetChild("Gyaml")->GetKey() = "Gyaml";
```
When creating a node, you have to pass a type to the constructor. If you want, you can also pass the key in the constructor.
```cpp
BymlFile::Node TestNode(BymlFile::Type::UInt32);
BymlFile::Node NewGyamlNode(BymlFile::Type::StringIndex, "Gyaml");
```
To add a child to the node, use the `AddChild` function.
```cpp
BymlFile::Node* ActorNode;
BymlFile::Node GyamlNode(BymlFile::Type::StringIndex, "Gyaml");
GyamlNode.SetValue<std::string>("TBox_Dungeon_Iron");
ActorNode->AddChild(GyamlNode);
```
