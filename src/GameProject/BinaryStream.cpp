#include "pch.h"
#include<iterator>
#include "BinaryStream.h"

// For file compression
#include "AlgoUtils.h"

BinaryStream::BinaryStream(const std::string& filePath)
 : m_FilePath(filePath), m_RawData(std::vector<char>()), m_Position(0)
{
}

int BinaryStream::ReadInt()
{
  char* memblock{ new char[sizeof(int)]};
  Read(memblock, sizeof(int));

  int val{};
  std::memcpy(&val, memblock, sizeof(int));

  delete[] memblock;
  return val;
}

float BinaryStream::ReadFloat()
{
  char* memblock{ new char[sizeof(float)] };
  Read(memblock, sizeof(float));

  float val{};
  std::memcpy(&val, memblock, sizeof(float));

  delete[] memblock;
  return val;
}

char BinaryStream::ReadChar()
{
  char* memblock{ new char[sizeof(char)] };
  Read(memblock, sizeof(char));

  char val{};
  std::memcpy(&val, memblock, sizeof(char));

  delete[] memblock;
  return val;
}

double BinaryStream::ReadDouble()
{
  char* memblock{ new char[sizeof(double)] };
  Read(memblock, sizeof(double));

  double val{};
  std::memcpy(&val, memblock, sizeof(double));

  delete[] memblock;
  return val;
}

Point2f BinaryStream::ReadPoint()
{
  const float x{ ReadFloat() };
  const float y{ ReadFloat() };

  return Point2f{
    x,
    y
  };
}

Color4f BinaryStream::ReadColor()
{
  const float r{ ReadFloat() };
  const float g{ ReadFloat() };
  const float b{ ReadFloat() };
  const float a{ ReadFloat() };

  return Color4f{
    r,
    g,
    b,
    a
  };
}

Rectf BinaryStream::ReadRect()
{
  const float left{ ReadFloat() };
  const float bottom{ ReadFloat() };
  const float width{ ReadFloat() };
  const float height{ ReadFloat() };

  return Rectf{
    left,
    bottom,
    width,
    height
  };
}

Circlef BinaryStream::ReadCircle()
{
  const Point2f center{ ReadPoint() };
  const float radius{ ReadFloat() };

  return Circlef{
    center,
    radius
  };
}

Ellipsef BinaryStream::ReadEllipse()
{
  const Point2f center{ ReadPoint() };
  const float width{ ReadFloat() };
  const float height{ ReadFloat() };

  return Ellipsef {
    center,
    width,
    height
  };
}

std::vector<int> BinaryStream::ReadIntVec()
{
  const int size{ ReadInt() };
  std::vector<int> val{};
  val.reserve(size);

  if (size < 0) {
    std::cout << "Vector size invalid" << std::endl;
    exit(-1); // Invalid reading is a good reason to quit before anything significant goes wrong
  }

  // Read the values
  for (size_t i = 0; i < size; i++)
  {
    val.push_back(ReadInt());
  }

  return val;
}

std::vector<float> BinaryStream::ReadFloatVec()
{
  const int size{ ReadInt() };
  std::vector<float> val{};
  val.reserve(size);

  if (size < 0) {
    std::cout << "Vector size invalid" << std::endl;
    exit(-1); // Invalid reading is a good reason to quit before anything significant goes wrong
  }

  // Read the values
  for (size_t i = 0; i < size; i++)
  {
    val.push_back(ReadFloat());
  }

  return val;
}

std::vector<char> BinaryStream::ReadCharVec()
{
  const int size{ ReadInt() };
  std::vector<char> val{};
  val.reserve(size);

  if (size < 0) {
    std::cout << "Vector size invalid" << std::endl;
    exit(-1); // Invalid reading is a good reason to quit before anything significant goes wrong
  }

  // Read the values
  for (size_t i = 0; i < size; i++)
  {
    val.push_back(ReadChar());
  }

  return val;
}

std::vector<double> BinaryStream::ReadDoubleVec()
{
  const int size{ ReadInt() };
  std::vector<double> val{};
  val.reserve(size);

  if (size < 0) {
    std::cout << "Vector size invalid" << std::endl;
    exit(-1); // Invalid reading is a good reason to quit before anything significant goes wrong
  }

  // Read the values
  for (size_t i = 0; i < size; i++)
  {
    val.push_back(ReadDouble());
  }

  return val;
}

std::vector<Point2f> BinaryStream::ReadPointVec()
{
  const int size{ ReadInt() };
  std::vector<Point2f> val{};
  val.reserve(size);

  if (size < 0) {
    std::cout << "Vector size invalid" << std::endl;
    exit(-1); // Invalid reading is a good reason to quit before anything significant goes wrong
  }

  // Read the values
  for (size_t i = 0; i < size; i++)
  {
    val.push_back(ReadPoint());
  }

  return val;
}

std::vector<Color4f> BinaryStream::ReadColorVec()
{
  const int size{ ReadInt() };
  std::vector<Color4f> val{};
  val.reserve(size);

  if (size < 0) {
    std::cout << "Vector size invalid" << std::endl;
    exit(-1); // Invalid reading is a good reason to quit before anything significant goes wrong
  }

  // Read the values
  for (size_t i = 0; i < size; i++)
  {
    val.push_back(ReadColor());
  }

  return val;
}

std::vector<Rectf> BinaryStream::ReadRectVec()
{
  const int size{ ReadInt() };
  std::vector<Rectf> val{};
  val.reserve(size);

  if (size < 0) {
    std::cout << "Vector size invalid" << std::endl;
    exit(-1); // Invalid reading is a good reason to quit before anything significant goes wrong
  }

  // Read the values
  for (size_t i = 0; i < size; i++)
  {
    val.push_back(ReadRect());
  }

  return val;
}

std::vector<Circlef> BinaryStream::ReadCircleVec()
{
  const int size{ ReadInt() };
  std::vector<Circlef> val{};
  val.reserve(size);

  if (size < 0) {
    std::cout << "Vector size invalid" << std::endl;
    exit(-1); // Invalid reading is a good reason to quit before anything significant goes wrong
  }

  // Read the values
  for (size_t i = 0; i < size; i++)
  {
    val.push_back(ReadCircle());
  }

  return val;
}

std::vector<Ellipsef> BinaryStream::ReadEllipseVec()
{
  const int size{ ReadInt() };
  std::vector<Ellipsef> val{};
  val.reserve(size);

  if (size < 0) {
    std::cout << "Vector size invalid" << std::endl;
    exit(-1); // Invalid reading is a good reason to quit before anything significant goes wrong
  }

  // Read the values
  for (size_t i = 0; i < size; i++)
  {
    val.push_back(ReadEllipse());
  }

  return val;
}

std::vector<ObjectBlueprint> BinaryStream::ReadBlueprintVec()
{
  const int size{ ReadInt() };
  std::vector<ObjectBlueprint> val{}; // Create a vector
  val.reserve(size);

  if (size < 0) {
    std::cout << "Vector size invalid" << std::endl;
    exit(-1); // Invalid reading is a good reason to quit before anything significant goes wrong
  }

  // Read the values
  for (size_t i = 0; i < size; i++)
  {
    const int id{ ReadInt() };
    const Point2f position{ ReadPoint() };
    const ObjectBlueprint blueprint{ ObjectBlueprint(id, position) };
    val.push_back(blueprint);
  }

  return val;
}

void BinaryStream::Write(int val)
{
  Write(reinterpret_cast<char*>(&val), sizeof(int));
}

void BinaryStream::Write(float val)
{
  Write(reinterpret_cast<char*>(&val), sizeof(float));
}

void BinaryStream::Write(char val)
{
  Write(&val, sizeof(char));
}

void BinaryStream::Write(double val)
{
  Write(reinterpret_cast<char*>(&val), sizeof(double));
}

void BinaryStream::Write(const Point2f& point)
{
  Write(point.x);
  Write(point.y);
}

void BinaryStream::Write(const Color4f& color)
{
  Write(color.r);
  Write(color.g);
  Write(color.b);
  Write(color.a);
}

void BinaryStream::Write(const Rectf& rect)
{
  Write(rect.left);
  Write(rect.bottom);
  Write(rect.width);
  Write(rect.height);
}

void BinaryStream::Write(const Circlef& circle)
{
  Write(circle.center);
  Write(circle.radius);
}

void BinaryStream::Write(const Ellipsef& ellipse)
{
  Write(ellipse.center);
  Write(ellipse.radiusX);
  Write(ellipse.radiusY);
}

void BinaryStream::Write(const std::vector<int>& ints)
{
  Write((int)ints.size());

  for (const int val : ints)
  {
    Write(val);
  }
}

void BinaryStream::Write(const std::vector<float>& floats)
{
  Write((int)floats.size());

  for (const float val : floats)
  {
    Write(val);
  }
}

void BinaryStream::Write(const std::vector<char>& chars)
{
  Write((int)chars.size());

  for (const char val : chars)
  {
    Write(val);
  }
}

void BinaryStream::Write(const std::vector<double>& doubles)
{
  Write((int)doubles.size());

  for (const double val : doubles)
  {
    Write(val);
  }
}

void BinaryStream::Write(const std::vector<Point2f>& points)
{
  Write((int)points.size());

  for (const Point2f& val : points)
  {
    Write(val);
  }
}

void BinaryStream::Write(const std::vector<Color4f>& colors)
{
  Write((int)colors.size());

  for (const Color4f& val : colors)
  {
    Write(val);
  }
}

void BinaryStream::Write(const std::vector<Rectf>& rects)
{
  Write((int)rects.size());

  for (const Rectf& val : rects)
  {
    Write(val);
  }
}

void BinaryStream::Write(const std::vector<Circlef>& circles)
{
  Write((int)circles.size());

  for (const Circlef& val : circles)
  {
    Write(val);
  }
}

void BinaryStream::Write(const std::vector<Ellipsef>& ellipses)
{
  Write((int)ellipses.size());

  for (const Ellipsef& val : ellipses)
  {
    Write(val);
  }
}

void BinaryStream::Write(const std::vector<ObjectBlueprint>& blueprints)
{
  Write((int)blueprints.size());

  for (const ObjectBlueprint& blueprint : blueprints) {
    Write(blueprint.GetObjectId());
    Write(blueprint.GetPosition());
  }
}

// This seems simple enough
void BinaryStream::Save()
{
  // Compress the data
  std::vector<char> compressed{ AlgoUtils::Compress(m_RawData) };

  // Save the compressed data to a file
  std::ofstream file(m_FilePath, std::ios::out | std::ios::binary);
  file.write(compressed.data(), compressed.size());
  file.close();
}

void BinaryStream::Clear()
{
  m_Position = 0;
  m_RawData.clear();
  m_RawData.shrink_to_fit();
}

// Source https://cplusplus.com/doc/tutorial/files/ & 
// https://stackoverflow.com/questions/259297/how-do-you-copy-the-contents-of-an-array-to-a-stdvector-in-c-without-looping 
// for more information on this
void BinaryStream::Load()
{
  std::ifstream file(m_FilePath, std::ios::in | std::ios::binary | std::ios::ate);

  // If the file is not open
  if (!file.is_open())
  {
    std::cerr << "Unable to open file: '" << m_FilePath << std::endl;
    return;
  };

  std::streampos size{ file.tellg() }; // get the size of the file

  if (size < 0) {
    std::cerr << "Error determining file size." << std::endl;
    file.close();
    return;
  }

  char* memblock{ new char[size] {} }; // Reserve a memory block
  file.seekg(0, std::ios::beg);     // Go to the beginning of the file
  file.read(memblock, size);        // Read the contents of the file

  std::vector<char> compressed{};

  compressed.reserve(size);

  // Copy over the values of memblock into compressed
  for (std::streamsize index = 0; index < size; ++index) {
    compressed.push_back(memblock[index]); // Dereference memblock to get each character
  }

  // Cleanup file read
  delete[] memblock;
  file.close();

  // Decompress the data
  std::vector<char> decompressed{ AlgoUtils::Decompress(compressed) };

  // Load the data into raw data
  m_RawData.clear();
  m_RawData.shrink_to_fit();
  m_RawData.reserve(decompressed.size());
  m_RawData.insert(m_RawData.end(), decompressed.begin(), decompressed.end());

  m_Position = 0;
}

void BinaryStream::Read(char data[], size_t size)
{
  if (size + m_Position > m_RawData.size()) {
    std::cerr << "Read size invalid" << std::endl;
    exit(-1); // Quit when we get an invalid read, this should not happen under any circumstances
  }

  // Read into data
  for (size_t index = 0; index < size; ++index)
  {
    data[index] = m_RawData[m_Position + index];
  }

  m_Position += size;
}

void BinaryStream::Write(char data[], size_t size)
{
  m_RawData.reserve(size);
  for (size_t index = 0; index < size; index++)
  {
    m_RawData.push_back(data[index]);
  }
}
