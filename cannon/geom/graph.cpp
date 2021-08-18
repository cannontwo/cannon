#include <cannon/geom/graph.hpp>

#include <boost/graph/astar_search.hpp>

using namespace cannon::geom;

void Graph::add_edge(Vertex u, Vertex v, double weight) {
  auto [e, added] = boost::add_edge(u, v, g_);
  boost::put(weights_, e, weight);
}

std::vector<Vertex> Graph::astar(Vertex start, Vertex goal,
                                 std::function<double(Vertex)> heur) {
  std::vector<Vertex> p(boost::num_vertices(g_));

  boost::astar_search(
      g_, start, heur,
      boost::predecessor_map(&p[0]).visitor(
          boost::default_astar_visitor()));

  std::list<Vertex> shortest_path; 
  for (Vertex v = goal; ; v = p[v]) {
    shortest_path.push_front(v);
    if (p[v] == v)
      break;
  }

  return std::vector<Vertex>(shortest_path.begin(), shortest_path.end());
}
