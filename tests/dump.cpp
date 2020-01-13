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
    if(msg) msg->dump();
    else {
      std::cerr << msg.error().value() << std::endl;
      std::cerr << msg.error().is_stream_error() << std::endl;
      std::cerr << msg.error().to_stream_error().value() << std::endl;
    }
  }
  return 0;
}
