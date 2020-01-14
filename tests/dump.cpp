#include <parsebgp.hpp>
#include <parsebgp/io.hpp>
#include <iostream>

namespace pbgp = parsebgp;
namespace mrt = parsebgp::mrt;
namespace io = parsebgp::io;

int main(int argc, char *argv[]) {
  if (argc != 2) return 1;

  io::GzipStream gzs(argv[1], 32678 * 4);
  pbgp::Options options;

  options.set_ignore_not_implemented(true);
  auto reader = io::mrt_reader(gzs, std::move(options));
  for(auto msg : reader) {
    if(msg) {
      // msg->dump();
      auto tdv2 = msg->to_table_dump_v2();
      if (tdv2.subtype().is_rib_ip()) {
        auto rib = tdv2.to_rib();
        for(auto entry : rib) {
          if (entry.path_attributes().has_communities()) {
            std::cout << "COMMUNITIES: ";
            for(auto comm : entry.path_attributes().communities()) {
              std::cout << comm.asn << ":" << comm.value << " ";
            }
            std::cout << std::endl;
          }
        }
      }
    } else {
      throw std::runtime_error(std::to_string(msg.error().value()));
    }
  }
  return 0;
}
