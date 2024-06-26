#include "base.h"
#include "structs.h"

//-----------------------------------------------------------------
// Window Constructors
//-----------------------------------------------------------------
Window::Window( const std::string& title , float width , float height , bool isVSyncOn )
	:title{ title }
	,width{ width }
	,height{ height }
	,isVSyncOn{ isVSyncOn }
{
}

//-----------------------------------------------------------------
// Point2f Constructors
//-----------------------------------------------------------------
Point2f::Point2f( )
	:Point2f{ 0.0f, 0.0f }
{
}
Point2f::Point2f( float x, float y )
	:x{ x }, y{ y }
{
}

Point2f Point2f::operator-(float val) const
{
  return Point2f(
    x - val,
    y - val
  );
}

Point2f Point2f::operator+(float val) const
{
  return Point2f(
    x + val,
    y + val
  );
}

Point2f Point2f::operator*(float val) const
{
  return Point2f(
    x * val,
    y * val
  );
}

Point2f Point2f::operator+(const Point2f& other) const
{
  return Point2f(
    x + other.x, 
    y + other.y
  );
}

Point2f Point2f::operator*(const Point2f& other) const
{
  return Point2f(
    x * other.x,
    y * other.y
  );
}

bool Point2f::operator!=(const Point2f& other) const
{
  return x != other.x && y != other.y;
}

bool Point2f::operator==(const Point2f& other) const
{
  return x == other.x && y == other.y;
}

float Point2f::Distance(const Point2f& other) const
{
  return sqrtf(powf(x - other.x, 2) + powf(y - other.y, 2));
}

// Length of the point
float Point2f::Length() const
{
  return sqrtf(x * x + y * y);
}

float Point2f::SquareLength() const
{
  return x * x + y * y;
}

//-----------------------------------------------------------------
// Rectf Constructors
//-----------------------------------------------------------------
Rectf::Rectf( )
	:Rectf{ 0.0f, 0.0f, 0.0f, 0.0f }
{
}

Rectf::Rectf( float left, float bottom, float width, float height )
	:left{ left }
	,bottom{ bottom }
	,width{ width }
	,height{ height }
{
}

Point2f Rectf::Center() const
{
    return Point2f(
      left + width / 2.f,
      bottom + height / 2.f
    );
}

std::vector<Point2f> Rectf::Polygon() const
{
  return std::vector<Point2f>({ Point2f{left, bottom}, Point2f{left + width, bottom}, Point2f{left, bottom + height}, Point2f{left + width, bottom + height} });
}

//-----------------------------------------------------------------
// Color4f Constructors
//-----------------------------------------------------------------
Color4f::Color4f( )
	:Color4f{ 0.0f, 0.0f, 0.0f, 1.0f }
{
}

Color4f::Color4f( float r, float g, float b, float a )
	:r{ r }
	,g{ g }
	,b{ b }
	,a{ a }
{
}

//-----------------------------------------------------------------
// Circlef Constructors
//-----------------------------------------------------------------
Circlef::Circlef( )
	:Circlef{ 0.0f, 0.0f, 0.0f }
{
}

Circlef::Circlef( float centerX, float centerY, float radius )
	:Circlef{ Point2f{ centerX, centerY }, radius }
{
}

Circlef::Circlef( const Point2f& center, float radius )
	:center{ center }
	,radius{ radius }
{
}

//-----------------------------------------------------------------
// Ellipsef Constructors
//-----------------------------------------------------------------
Ellipsef::Ellipsef( )
	:Ellipsef{ 0.0f, 0.0f, 0.0f, 0.0f }
{
}


Ellipsef::Ellipsef( const Point2f& center, float radiusX, float radiusY )
	: center{ center }
	, radiusX{ radiusX }
	, radiusY{ radiusY }
{
}

Ellipsef::Ellipsef( float centerX, float centerY, float radiusX, float radiusY )
	: Ellipsef{ Point2f{ centerX, centerY }, radiusX, radiusY }
{
}
