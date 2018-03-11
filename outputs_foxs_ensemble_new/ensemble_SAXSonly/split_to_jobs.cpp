#include <IMP/base/check_macros.h>

#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <fstream>
#include <iostream>
#include <vector>
#include <string>

namespace {

void read_profiles(const std::string profile_filenames_file,
                   std::vector<std::string>& file_names) {
  // open input file with profile file names
  std::ifstream in_file(profile_filenames_file.c_str());
  if(!in_file) {
    IMP_THROW("Can't find input file " << profile_filenames_file, IOException);
  }
  std::string curr_file_name;
  while (!in_file.eof()) {
    getline(in_file, curr_file_name);
    boost::trim(curr_file_name); // remove all spaces
    std::vector < std::string > split_results;
    boost::split(split_results, curr_file_name, boost::is_any_of("\t "),
                 boost::token_compress_on);
    if (split_results.size() < 1 || split_results.size() > 2) continue;
    curr_file_name = split_results[0].c_str();
    if(curr_file_name.length() <= 0) continue;
    file_names.push_back(curr_file_name);
  }
  std::cout << file_names.size() << " profiles were read" << std::endl;
}


void extend_interval_dimension(unsigned int N, unsigned int interval_size,
                               const std::vector<int>& curr_interval,
                               std::vector<std::vector<int> >& new_intervals) {
  // determine range
  unsigned int n=0;
  if(curr_interval.size() > 0) n = curr_interval[curr_interval.size()-1]+1;

  for(unsigned int i=n; i<N; i+=interval_size) {
    std::vector<int> new_interval(curr_interval);
    new_interval.push_back(i);
    new_interval.push_back(std::min(N, i+interval_size) -1);
    new_intervals.push_back(new_interval);
  }

}

void enumerate_intervals(unsigned int N, unsigned int interval_size, unsigned int dimension,
                         std::vector<std::vector<int> >& intervals) {


  std::vector<std::vector<int> > curr_intervals, new_intervals;
  std::vector<int> empty_interval;
  // init
  extend_interval_dimension(N, interval_size, empty_interval, curr_intervals);

  for(unsigned int d=1; d<dimension; d++) {
    for(unsigned int i=0; i<curr_intervals.size(); i++) {
      extend_interval_dimension(N, interval_size, curr_intervals[i], new_intervals);
    }
    curr_intervals = new_intervals;
    new_intervals.clear();
  }
  intervals = curr_intervals;
}

}


int main(int argc, char* argv[]) {
  // output arguments
  for (int i = 0; i < argc; i++) std::cerr << argv[i] << " ";
  std::cerr << std::endl;

  int ensemble_size = 5;
  int interval_size = 10;
  po::options_description desc("Usage: <profile_file_names_file> ");
  desc.add_options()
    ("help", "split one enumeration job into many small ones. \
prepares files with relevant filenames")
    ("input-files", po::value< std::vector<std::string> >(),
     "input profile files")
    ("ensemble-size,e", po::value<int>(&ensemble_size)->default_value(5),
     "maximal ensemble size (default = 5)")
    ("interval-size,i", po::value<int>(&interval_size)->default_value(10),
     "maximal ensemble size (default = 10)");

  po::positional_options_description p;
  p.add("input-files", -1);
  po::variables_map vm;
  po::store(
      po::command_line_parser(argc,argv).options(desc).positional(p).run(), vm);
  po::notify(vm);

  std::vector<std::string> files;
  if(vm.count("input-files")) {
    files = vm["input-files"].as< std::vector<std::string> >();
  }
  if(vm.count("help") || files.size() == 0) {
    std::cout << desc << "\n";
    return 0;
  }

  std::vector<std::string> file_names;
  read_profiles(files[0], file_names);

  std::vector<std::vector<int> > intervals;
  enumerate_intervals(file_names.size(), interval_size, ensemble_size, intervals);


  for(unsigned int i=0; i<intervals.size(); i++) {
    std::string out_file_name = "filenames_" +
      std::string(boost::lexical_cast<std::string>(i+1)) + ".txt";
    std::ofstream out_file(out_file_name.c_str());
    std::cout << "interval: " << out_file_name << " ";
    std::ostream_iterator<unsigned int> out_it(std::cout, ", ");
    std::copy(intervals[i].begin(), intervals[i].end(), out_it);
    std::cout << std::endl;

    for(unsigned int j=0; j<intervals[i].size(); j+=2) {
      unsigned int min_index = intervals[i][j];
      unsigned int max_index = intervals[i][j+1];

      for(unsigned int k=min_index; k<=max_index; k++) {
        out_file << file_names[k] << std::endl;
      }

    }
  }
}
