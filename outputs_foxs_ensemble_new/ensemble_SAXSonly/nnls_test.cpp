/**
    a program to determine the weights of the individual profiles
    within the experimental profile using NNLS
    Author: Dina Schneidman
 */
#include <IMP/base/log.h>
#include <IMP/base/flags.h>
#include <IMP/saxs/Profile.h>
#include <IMP/saxs/WeightedProfileFitter.h>

#include <boost/program_options.hpp>
namespace po = boost::program_options;



int main(int argc, char* argv[]) {
  // output arguments
  for (int i = 0; i < argc; i++) std::cerr << argv[i] << " ";
  std::cerr << std::endl;

  po::options_description
    desc("Usage: <experimental_profile> <profile_file1> <profile_file2> ...");
  desc.add_options()
    ("help", "Any number of input profiles is supported. \
The weights are computed to minimize the chi between the first profile \
and a weighted average of the rest.")
    ("input-files", po::value< std::vector<std::string> >(),
     "input profile files")
    ;

  po::positional_options_description p;
  p.add("input-files", -1);
  po::variables_map vm;
  po::store(
      po::command_line_parser(argc,argv).options(desc).positional(p).run(), vm);
  po::notify(vm);

  std::vector<std::string> files, dat_files;
  if(vm.count("input-files")) {
    files = vm["input-files"].as< std::vector<std::string> >();
  }
  if(vm.count("help") || files.size() == 0) {
    std::cout << desc << "\n";
    return 0;
  }

  // read the experimental profile
  IMP::saxs::Profile* exp_profile = new IMP::saxs::Profile(files[0]);
  int n = exp_profile->size();
  if(n == 0) {
    std::cerr << "can't parse input exp. profile file "
              << files[0] << std::endl;
    return 1;
  }

  // read the profiles for weighting
  std::vector<IMP::saxs::Profile *> profiles;
  bool fit_file = true; // turn into program option

  for(unsigned int i=1; i<files.size(); i++) {
    IMP::saxs::Profile *profile = new IMP::saxs::Profile(files[i], fit_file);
    if(profile->size() == 0) {
      std::cerr << "can't parse input file " << files[i] << std::endl;
      return 1;
    } if((int)profile->size() != n) {
      std::cerr << "wrong profile size: " << files[i] << " should be "
                << n << " has " << profile->size() << std::endl;
      return 1;
    } else {
      dat_files.push_back(files[i]);
      profiles.push_back(profile);
      std::cout << "Profile read from file " << files[i] << " size = "
                << profile->size() << std::endl;
    }
  }


  IMP::saxs::WeightedProfileFitter wpf(exp_profile);
  std::vector<double> weights;
  float chi = wpf.compute_score((IMP::saxs::ProfilesTemp)profiles, weights);

  std::cout << "Chi = " << chi << std::endl;
  for(unsigned int i=0; i<weights.size(); i++) {
    std::cout << " weights[" << i << "]" << weights[i] << std::endl;
  }
  return 0;
}
