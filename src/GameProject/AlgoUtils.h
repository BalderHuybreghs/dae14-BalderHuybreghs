#pragma once
#include <vector>

// General utilities for algorithms
namespace AlgoUtils
{
  // Hashing of a pair, used in the tilemap implementation
  struct PairHash
  {
    template <class T1, class T2> // Template class telling t1 and t2 can be of any type, allowing for any pair to work
    std::size_t operator()(const std::pair<T1, T2>& p) const
    {
      size_t hash1 = std::hash<T1>{}(p.first);
      size_t hash2 = std::hash<T2>{}(p.second);
      return hash1 ^ hash2; // Combine the hash values
    }
  };

  // Hashing of a point used to convert rects to polygons
  struct PointHash
  {
    size_t operator()(const Point2f& p) const;
  };

  // Simple point with flag struct used in RectanglesToPolygons
  struct PointWithFlag
  {
    Point2f point;
    int flag;
    PointWithFlag(const Point2f& p, int f) : point(p), flag(f) {}
  };

  bool PointYThenX(const Point2f& a, const Point2f& b); // Ordering of points

  // Convert a list of rectangles to a list of polygons, this is used
  // in the tilemap implementation
  // LOOK AT https://stackoverflow.com/questions/13746284/merging-multiple-adjacent-rectangles-into-one-polygon 
  // FOR IMPLEMENTATION DETAILS
  std::vector<std::vector<Point2f>> RectanglesToPolygons(const std::vector<Rectf>& rectangles);
};

