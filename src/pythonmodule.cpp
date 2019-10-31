// xg
#include "xg.hpp"
//using namespace xg;

// Pybind11
#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/iostream.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>
namespace py = pybind11;

PYBIND11_MODULE(xg, m)
{

    py::class_<handlegraph::handle_t>(m, "handle", "the handle, which refers to oriented nodes");
    py::class_<handlegraph::path_handle_t>(m, "path_handle", "the path handle type, which refers to paths");
    py::class_<handlegraph::step_handle_t>(m, "step_handle", "the step handle type, which refers to path paths")
        .def("first", [](handlegraph::step_handle_t &step_handle) {
                return (reinterpret_cast<const int64_t*>(&step_handle)[0]);
        })
        .def("second", [](handlegraph::step_handle_t &step_handle) {
                return (reinterpret_cast<const int64_t*>(&step_handle)[1]);
        });
    py::class_<handlegraph::edge_t>(m, "edge", "edges link two handles together")
        .def("first", [](const handlegraph::edge_t &edge_handle) {
                return (&edge_handle)->first;
        })
        .def("second", [](const handlegraph::edge_t &edge_handle) {
                return (&edge_handle)->second;
        });
    // Expose class Graph to Python.
    py::class_<xg::XG>(m, "graph", "the xg graph type")
        .def(py::init())
        .def("has_node",
              &xg::XG::has_node,
             "Return true if the given node is in the graph.",
             py::arg("node_id"))
        .def("get_handle",
             &xg::XG::get_handle,
             "Return the handle for the given node id.",
             py::arg("node_id"),
             py::arg("is_reverse") = false)
        .def("get_id",
             &xg::XG::get_id,
             "Return the id of the given handle.",
             py::arg("handle"))
        .def("get_is_reverse",
             &xg::XG::get_is_reverse,
             "Return true if the handle refers to the node reverse complement.",
             py::arg("handle"))
        .def("flip",
             &xg::XG::flip,
             "Flip the handle to the opposite orientation.",
             py::arg("handle"))
        .def("get_length",
             &xg::XG::get_length,
             "Return the length of the node referred to by the handle.",
             py::arg("handle"))
        .def("get_sequence",
             &xg::XG::get_sequence,
             py::arg("handle"))
         .def("follow_edges",
              [](const xg::XG& g, const handlegraph::handle_t& handle, bool go_left, const std::function<bool(const handlegraph::handle_t&)>& iteratee) {
                  return g.follow_edges(handle, go_left, [&iteratee](const handlegraph::handle_t& h) { iteratee(h); return true; });
              },
             "Follow edges starting at a given node.")
        .def("for_each_handle",
             [](const xg::XG& g, const std::function<bool(const handlegraph::handle_t&)>& iteratee, bool parallel) {
                 return g.for_each_handle([&iteratee](const handlegraph::handle_t& h){ iteratee(h); return true; }, parallel);
             },
             "Iterate over all the nodes in the graph.",
             py::arg("iteratee"),
             py::arg("parallel") = false)
        .def("get_node_count",
             &xg::XG::get_node_count,
             "Return the number of nodes in the graph.")
        .def("min_node_id",
             &xg::XG::min_node_id,
             "Return the minimum node id in the graph.")
        .def("max_node_id",
             &xg::XG::max_node_id,
             "Return the maximum node id in the graph.")
//         .def("get_degree",
//              &xg::XG::get_degree,
//              "Return the degree of the given node.")
//         .def("forward",
//              &xg::XG::forward,
//              "Return the forward version of the handle.")
//         .def("edge_handle",
//              &xg::XG::edge_handle,
//              "Return the edge handle for the given pair of handles.")
        .def("has_path",
             &xg::XG::has_path,
             "Return if a path with the givenv name exists in the graph.")
        .def("get_path_handle",
             &xg::XG::get_path_handle,
             "Return the path handle for the named path.")
        .def("get_path_name",
             &xg::XG::get_path_name,
             "Return the path name for a given path handle.")
        .def("get_path_count",
             &xg::XG::get_path_count,
             "Return the path count of the graph")
//         .def("steps_of_handle",
//              &xg::XG::steps_of_handle,
//              "Obtain the steps on a given handle.")
        .def("for_each_path_handle",
             [](const xg::XG& g,
                const std::function<bool(const handlegraph::path_handle_t&)>& iteratee) {
                 return g.for_each_path_handle([&iteratee](const handlegraph::path_handle_t& p) {
                         iteratee(p); return true;
                     });
             },
             "Invoke the callback for each path in the graph.")
        .def("for_each_step_on_handle",
             [](const xg::XG& g,
                const handlegraph::handle_t& handle,
                const std::function<bool(const handlegraph::step_handle_t&)>& iteratee) {
                 return g.for_each_step_on_handle(handle, [&iteratee](const handlegraph::step_handle_t& s) {
                         iteratee(s); return true;
                     });
             },
             "Invoke the callback for each of the steps on a given handle.")
        .def("get_handle_of_step",
             &xg::XG::get_handle_of_step,
             "Return the handle that a given step occurs on.")
        .def("get_path",
             &xg::XG::get_path,
             "Return the path of a given step handle.")
        .def("path_begin",
             &xg::XG::path_begin,
             "Return the step handle for the first step in the given path.")
        .def("path_end",
             &xg::XG::path_end,
             "Return a step handle to a fictitious handle one past the end of the path.")
        .def("path_back",
             &xg::XG::path_back,
             "Return a step handle to the last step, which is arbitrary in the case\nof a circular path.")
        .def("path_front_end",
             &xg::XG::path_front_end,
             "Return a step handle to a fictitious handle one past the start of the path.")
        .def("has_next_step",
             &xg::XG::has_next_step,
             "Returns true if the step is not the last step on the path, else false.")
        .def("has_previous_step",
             &xg::XG::has_previous_step,
             "Returns true if the step is not the first step on the path, else false.")
        .def("get_next_step",
             &xg::XG::get_next_step,
             "Returns a handle to the next step on the path. Calling on an end marker\nstep returns the same end marker.")
        .def("get_previous_step",
             &xg::XG::get_previous_step,
             "Returns a handle to the previous step on the path. Calling on a front\nend marker step returns the same end marker.")
        .def("get_path_handle_of_step",
             &xg::XG::get_path_handle_of_step,
             "Returns a handle to the path that an step is on.")
        /*
        .def("get_ordinal_rank_of_step",
             &xg::XG::get_ordinal_rank_of_step,
             "Returns the 0-based ordinal rank of a step on a path. (warning: not implemented in odgi)")
        */
//         .def("is_empty",
//              &xg::XG::is_empty,
//              "Returns true if the given path is empty, and false otherwise.")
//         .def("for_each_step_in_path",
//              &xg::XG::for_each_step_in_path,
//              "Invoke the callback for each step in a given path.")
        .def("get_is_circular",
             &xg::XG::get_is_circular,
             "Returns true if the path is circular.")
//         .def("has_edge",
//              &xg::XG::has_edge,
//              "Returns true if the given edge exists")
        .def("to_gfa",
             [](const xg::XG& g) {
                 py::scoped_ostream_redirect stream(
                     std::cout,
                     py::module::import("sys").attr("stdout")
                     );
                 g.to_gfa(std::cout);
             },
             "Display as GFA")
        .def("serialize",
             [](xg::XG& g, const std::string& file) {
                 std::ofstream out(file.c_str());
                 g.serialize(out);
             },
             "Save the graph to the given file, returning the number of bytes written.")
        .def("load",
             [](xg::XG& g, const std::string& file) {
                 std::ifstream in(file.c_str());
                 g.load(in);
             },
             "Load the graph from the given file.")
        // Definition of class_<xg::graph_t> ends here.
    ;


}
