#pragma once
#include "structs.h"
#include "ObjectBlueprint.h"

#include <iostream>
#include <fstream>
#include <vector>

// A utility class for easily writing to binary files, reads should always happen in the same order as writes et vice versa
// NOTE: this does not support sharing a file of a platform which has different primitive sizes, to another platform which has different primitive sizes
class BinaryStream final
{
public:
  // Create a stream of the given file, if the file does not exist, will write to a new one
  BinaryStream(const std::string& filePath);

  // Reading operations

  // PRIMITIVES
  int      ReadInt();
  float    ReadFloat();
  char     ReadChar();
  double   ReadDouble();

  // STRUCTS
  Point2f  ReadPoint();
  Color4f  ReadColor();
  Rectf    ReadRect();
  Circlef  ReadCircle();
  Ellipsef ReadEllipse();

  // VECTORS
  // Storage format: 1 int for size -> values

  std::vector<int>      ReadIntVec();
  std::vector<float>    ReadFloatVec();
  std::vector<char>     ReadCharVec();
  std::vector<double>   ReadDoubleVec();
                        
  std::vector<Point2f>  ReadPointVec();
  std::vector<Color4f>  ReadColorVec();
  std::vector<Rectf>    ReadRectVec();
  std::vector<Circlef>  ReadCircleVec();
  std::vector<Ellipsef> ReadEllipseVec();

  std::vector<ObjectBlueprint> ReadBlueprintVec();

  // Writing operations
  void Write(int val);
  void Write(float val);
  void Write(char val);
  void Write(double val);

  // STRUCTS
  void Write(const Point2f& point);
  void Write(const Color4f& color);
  void Write(const Rectf& rect);
  void Write(const Circlef& circle);
  void Write(const Ellipsef& ellipse);

  // VECTORS
  void Write(const std::vector<int>& ints);
  void Write(const std::vector<float>& floats);
  void Write(const std::vector<char>& chars);
  void Write(const std::vector<double>& doubles);
  void Write(const std::vector<Point2f>& points);
  void Write(const std::vector<Color4f>& colors);
  void Write(const std::vector<Rectf>& rects);
  void Write(const std::vector<Circlef>& circless);
  void Write(const std::vector<Ellipsef>& ellipses);

  // Object related
  void Write(const std::vector<ObjectBlueprint>& blueprints);

  void Save();
  void Load();
  void Clear();
private:
  // This is general information, the RawData will be the main working horse of the binary streams
  std::string m_FilePath;

  // The main working horse of the stream
  std::vector<char> m_RawData;
  size_t m_Position;

  void Read(char data[], size_t size); // Reads a given amount of bytes
  void Write(char data[], size_t size);
};