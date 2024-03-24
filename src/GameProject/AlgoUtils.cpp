#include "pch.h"
#include "AlgoUtils.h"
#include <set>
#include <unordered_map>
#include <utility>
#include <algorithm>

size_t AlgoUtils::PointHash::operator()(const Point2f& p) const
{
  return std::hash<int>()(p.x) ^ std::hash<int>()(p.y);
}

bool AlgoUtils::PointYThenX(const Point2f& a, const Point2f& b)
{
  if (a.y < b.y || (a.y == b.y && a.x < b.x)) {
    return true;
  } else if (a == b) {
    return false;
  } else {
    return false;
  }
}

// https://stackoverflow.com/questions/13746284/merging-multiple-adjacent-rectangles-into-one-polygon
// I don't fully understand the implementation, but all that matters right now is that it works,
// which is all I wanted for this project.
std::vector<std::vector<Point2f>> AlgoUtils::RectanglesToPolygons(const std::vector<Rectf>& rectangles)
{
  if (rectangles.empty()) {
    return std::vector<std::vector<Point2f>>();
  }

  std::set<Point2f, decltype(AlgoUtils::PointYThenX)*> points(AlgoUtils::PointYThenX);
  for (const Rectf& rect : rectangles) {
    const Point2f p1{ rect.left, rect.bottom };
    const Point2f p2{ rect.left + rect.width, rect.bottom + rect.height };

    points.insert(p1);
    points.insert(Point2f(p2.x, p1.y));
    points.insert(p2);
    points.insert(Point2f(p1.x, p2.y));
  }

  std::vector<Point2f> sorted_x(points.begin(), points.end());
  std::vector<Point2f> sorted_y(points.begin(), points.end());
  sort(sorted_x.begin(), sorted_x.end(), [](const Point2f& a, const Point2f& b) { return a.x < b.x; });
  sort(sorted_y.begin(), sorted_y.end(), AlgoUtils::PointYThenX);

  std::unordered_map<Point2f, Point2f, PointHash> edges_h;
  std::unordered_map<Point2f, Point2f, PointHash> edges_v;

  for (size_t i = 0; i < sorted_y.size() -1; i += 2) {
    edges_h[sorted_y[i]] = sorted_y[i + 1];
    edges_h[sorted_y[i + 1]] = sorted_y[i];
  }

  for (size_t i = 0; i < sorted_x.size() -1; i += 2) {
    edges_v[sorted_x[i]] = sorted_x[i + 1];
    edges_v[sorted_x[i + 1]] = sorted_x[i];
  }

  std::vector<std::vector<Point2f>> polygons;
  while (!edges_h.empty()) {
    std::vector<AlgoUtils::PointWithFlag> polygon;
    polygon.emplace_back(edges_h.begin()->first, 0);

    while (true) {
      const Point2f& curr = polygon.back().point;
      Point2f next_vertex;
      int flag;

      if (polygon.back().flag == 0) {
        auto iter = edges_v.find(curr);
        if (iter != edges_v.end()) {
          next_vertex = iter->second;
          flag = 1;
        }
      } else {
        auto iter = edges_h.find(curr);
        if (iter != edges_h.end()) {
          next_vertex = iter->second;
          flag = 0;
        }
      }

      if (next_vertex == polygon.front().point) {
        //polygon.pop_back(); // remove the closing point
        break;
      } else {
        polygon.emplace_back(next_vertex, flag);
      }
    }

    std::vector<Point2f> poly;
    for (const AlgoUtils::PointWithFlag& vertex : polygon) {
      poly.push_back(vertex.point);
      edges_h.erase(vertex.point);
      edges_v.erase(vertex.point);
    }

    polygons.push_back(poly);
  }

  return polygons;
}
