/**
   Ensemble generation software given a SAXS profile
    Author: Dina Schneidman
 */
#include "lib/EnsembleGenerator.h"
#include "lib/ExaustiveEnsembleGenerator.h"
#include "lib/SAXSEnsembleScore.h"
#include "lib/EM2DEnsembleScore.h"
#include "lib/ProfileClustering.h"

#include <IMP/saxs/SolventAccessibleSurface.h>
#include <IMP/saxs/FormFactorTable.h>

#include <IMP/atom/pdb.h>

#include <IMP/benchmark/Profiler.h>

#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <fstream>
#include <vector>
#include <string>

using namespace IMP::saxs;

namespace {
std::string trim_extension(const std::string file_name) {
  if(file_name[file_name.size()-4] == '.')
    return file_name.substr(0, file_name.size() - 4);
  return file_name;
}

void read_pdb(const std::string file,
              std::vector<std::string>& pdb_file_names,
              std::vector<IMP::kernel::Particles>& particles_vec,
              bool residue_level, bool heavy_atoms_only, int multi_model_pdb) {

  IMP::Model *model = new IMP::Model();

  IMP::atom::Hierarchies mhds;
  IMP::atom::PDBSelector* selector;
  if(residue_level) // read CA only
    selector = new IMP::atom::CAlphaPDBSelector();
  else
    if(heavy_atoms_only) // read without hydrogens
      selector =  new IMP::atom::NonWaterNonHydrogenPDBSelector();
    else // read with hydrogens
      selector = new IMP::atom::NonWaterPDBSelector();

  if(multi_model_pdb == 2) {
    mhds = read_multimodel_pdb(file, model, selector, true);
  } else {
    if(multi_model_pdb == 3) {
      IMP::atom::Hierarchy mhd =
        IMP::atom::read_pdb(file, model, selector, false, true);
      mhds.push_back(mhd);
    } else {
      IMP::atom::Hierarchy mhd =
        IMP::atom::read_pdb(file, model, selector, true, true);
      mhds.push_back(mhd);
    }
  }

  for(unsigned int h_index=0; h_index<mhds.size(); h_index++) {
    IMP::kernel::ParticlesTemp ps = get_by_type(mhds[h_index], IMP::atom::ATOM_TYPE);
    if(ps.size() > 0) { // pdb file
      std::string pdb_id = file;
      if(mhds.size() > 1) {
        pdb_id = trim_extension(file) + "_m" +
          std::string(boost::lexical_cast<std::string>(h_index+1)) + ".pdb";
      }
      pdb_file_names.push_back(pdb_id);
      particles_vec.push_back(IMP::get_as<IMP::kernel::Particles>(ps));
      std::cout << ps.size() << " atoms were read from PDB file " << file;
      if(mhds.size() > 1) std::cout << " MODEL " << h_index+1;
      std::cout << std::endl;
    }
  }
}

void read_profiles(const std::string profile_filenames_file,
                   const IMP::saxs::Profile* exp_profile,
                   IMP::saxs::Profiles& profiles,
                   std::vector<std::string>& file_names,
                   std::vector<double>& scores) {
  // open input file with profile file names
  std::ifstream in_file(profile_filenames_file.c_str());
  if(!in_file) {
    IMP_THROW("Can't find input file " << profile_filenames_file, IOException);
  }
  std::string curr_file_name;
  unsigned int profile_counter=0;
  while (!in_file.eof()) {
    getline(in_file, curr_file_name);
    boost::trim(curr_file_name); // remove all spaces
    std::vector < std::string > split_results;
    boost::split(split_results, curr_file_name, boost::is_any_of("\t "),
                 boost::token_compress_on);
    if (split_results.size() < 1 || split_results.size() > 2) continue;
    curr_file_name = split_results[0].c_str();
    if(curr_file_name.length() <= 0) continue;
    double score = 0;
    if(split_results.size() > 1) {
      score = atof(split_results[1].c_str());
    }

    // read a profile from current file
    IMP::saxs::Profile *profile =
      new IMP::saxs::Profile(exp_profile->get_min_q(),
                             exp_profile->get_max_q(),
                             exp_profile->get_delta_q());
    profile->read_partial_profiles(curr_file_name);
    if(profile->size() == 0) {
      IMP_THROW("Can't parse input file " << curr_file_name, IOException);
    }

    //profile->copy_errors(*exp_profile);
    profile->set_name(curr_file_name);
    profile->set_id(profile_counter);
    profiles.push_back(profile);
    file_names.push_back(curr_file_name);
    if(split_results.size() > 1) scores.push_back(score);
    profile_counter++;
    std::cout << "Profile read from file " << curr_file_name << " size = "
              << profile->size() << " score " << score << std::endl;
  }
  std::cout << profile_counter << " profiles were read" << std::endl;
}


IMP::saxs::Profile* compute_profile(IMP::kernel::Particles particles,
                                    float min_q, float max_q, float delta_q,
                                    IMP::saxs::FormFactorTable* ft,
                                    IMP::saxs::FormFactorType ff_type,
                                    float water_layer_c2, bool fit,
                                    bool reciprocal, bool ab_initio,
                                    bool vacuum) {
  IMP_NEW(Profile, profile, (min_q, max_q, delta_q));
  if(reciprocal) profile->set_ff_table(ft);

  // compute surface accessibility and average radius
  IMP::Floats surface_area;
  IMP::saxs::SolventAccessibleSurface s;
  float average_radius = 0.0;
  if(water_layer_c2 != 0.0) {
    // add radius
    for(unsigned int i=0; i<particles.size(); i++) {
      float radius = ft->get_radius(particles[i], ff_type);
      IMP::core::XYZR::setup_particle(particles[i], radius);
      average_radius += radius;
    }
    surface_area = s.get_solvent_accessibility(IMP::core::XYZRs(particles));
    average_radius /= particles.size();
    profile->set_average_radius(average_radius);
  }

  // pick profile calculation based on input parameters
  if(!fit) { // regular profile, no c1/c2 fitting
    if(ab_initio) { // bead model, constant form factor
      profile->calculate_profile_constant_form_factor(particles);
    } else if(vacuum) {
        profile->calculate_profile_partial(particles, surface_area, ff_type);
        profile->sum_partial_profiles(0.0, 0.0); // c1 = 0;
    } else {
      profile->calculate_profile(particles, ff_type, reciprocal);
    }
  } else { // c1/c2 fitting
    if(reciprocal)
      profile->calculate_profile_reciprocal_partial(particles, surface_area,
                                                    ff_type);
    else
      profile->calculate_profile_partial(particles, surface_area, ff_type);
  }
  return profile.release();
}

}

int main(int argc, char* argv[]) {
  // output arguments
  for (int i = 0; i < argc; i++) std::cerr << argv[i] << " ";
  std::cerr << std::endl;

  int ensemble_size = 5;
  int best_k = 1000;
  float background_adjustment_q = 0.0;
  float chi_percentage_cluster_thr = 0.3;
  float chi_thr = 0.0;
  float weight_thr = 0.05;
  bool brute_force = false;
  bool nnls = false;
  bool fixed_c1_c2_score = true;
  std::string matrix_file_name;

  po::options_description
    desc("Usage: <experimental_profile> <pdb_file1> <pdb_file2> ...");
  desc.add_options()
    ("help", "Any number of input profiles is supported. \
The weights are computed to minimize the chi between the first profile \
and a weighted average of the rest.")
    ("input-files", po::value< std::vector<std::string> >(),
     "input profile files")
    ("ensemble-size,s", po::value<int>(&ensemble_size)->default_value(5),
     "maximal ensemble size (default = 5)")
    ("bestK,k", po::value<int>(&best_k)->default_value(1000), "bestK (default = 1000)")
    ("threshold,t", po::value<float>(&chi_percentage_cluster_thr)->default_value(0.3),
     "chi value percentage threshold for profile similarity (default = 0.3)")
    ("chi_threshold,c", po::value<float>(&chi_thr)->default_value(0.0),
     "chi based threshold")
    ("weight_threshold,w", po::value<float>(&weight_thr)->default_value(0.05),
     "minimal weight threshold for a profile to contribute to the ensemble")
    ("background_q,b",
     po::value<float>(&background_adjustment_q)->default_value(0.0),
     "background adjustment, not used by default. if enabled, \
recommended q value is 0.2")
    //("brute_force,b", "use exaustive brute-force enumeration (default = false)")
    ("nnls,n", "run Non negative least square on all profiles (default = false)")
    ("fixed_c1_c2_score,f", "fix c1/c2 for fast scoring, optimize for output only (default = true)")
    ("output_matrix,m",
     po::value<std::string>(&matrix_file_name),
     "output profiles into matrix file");
    ;

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
  if(vm.count("brute_force")) brute_force=true;
  if(vm.count("nnls")) nnls=true;
  if(vm.count("fixed_c1_c2_score")) fixed_c1_c2_score=false;

  // read the experimental profile
  IMP::saxs::Profile* exp_profile = new IMP::saxs::Profile(files[0]);
  if(exp_profile->size() == 0) {
    std::cerr << "can't parse input exp. profile file "
              << files[0] << std::endl;
    return 1;
  }
  if(background_adjustment_q > 0.0) {
    exp_profile->background_adjust(background_adjustment_q);
  }

  // compute profiles for input pdbs or read from files
  Profiles profiles;
  std::vector<std::string> file_names;
  std::vector<double> scores;
  if(files.size() > 2) {
    // 1. read pdbs
    std::vector<IMP::kernel::Particles> particles_vec;
    for(unsigned int i=1; i<files.size(); i++) {
      // check if file exists
      std::ifstream in_file(files[i].c_str());
      if(!in_file) {
        std::cerr << "Can't open file " << files[i] << std::endl;
        exit(1);
      }
      read_pdb(files[i], file_names, particles_vec, false, true, 1);
    }
    IMP::saxs::FormFactorTable* ft = IMP::saxs::default_form_factor_table();
    IMP::saxs::FormFactorType ff_type = IMP::saxs::HEAVY_ATOMS;

    // 2. compute profiles for input pdbs
    for(unsigned int i=0; i<particles_vec.size(); i++) {
      // compute profile
      std::cerr << "Computing profile for " << file_names[i]
                << " "  << particles_vec[i].size() << " atoms "<< std::endl;
      IMP::saxs::Profile* profile = compute_profile(particles_vec[i],
                                                    exp_profile->get_min_q(),
                                                    exp_profile->get_max_q(),
                                                    exp_profile->get_delta_q(),
                                                    ft, ff_type, 4.0, true,
                                                    false, false, false);
      profile->set_name(file_names[i]);
      profile->set_id(i);
      // save the profile
      profiles.push_back(profile);
    }
  } else {
    // read precomputed partial profiles
    read_profiles(files[1], exp_profile, profiles, file_names, scores);
  }

  IMP::benchmark::Profiler pp("prof");

  if(brute_force) {
    SAXSEnsembleScore *ensemble_score =
      new SAXSEnsembleScore(profiles, exp_profile,
                            fixed_c1_c2_score, weight_thr);
    ExaustiveEnsembleGenerator eeg(profiles.size(), ensemble_score);
    std::string out_file_name = files[1] + "_ensembles_" +
      std::string(boost::lexical_cast<std::string>(ensemble_size)) + ".txt";
    eeg.generate_intervals(ensemble_size, out_file_name);

  } else {
    ProfileClustering profile_clustering(exp_profile, profiles, scores,
                                         chi_percentage_cluster_thr, chi_thr);
    const IMP::saxs::Profiles& clustered_profiles =
      profile_clustering.get_representatives();
    // write a file with clustered filenames
    std::ofstream clusters_file("cluster_representatives.txt");
    for(unsigned int i=0; i<clustered_profiles.size(); i++) {
      clusters_file << clustered_profiles[i]->get_name() << std::endl;
    }
    clusters_file.close();


    std::vector<EnsembleScore*> scorers;

    // ensemble generation
    SAXSEnsembleScore *saxs_ensemble_score =
      new SAXSEnsembleScore(clustered_profiles, exp_profile,
                            fixed_c1_c2_score, weight_thr);
    scorers.push_back(saxs_ensemble_score);

    std::string em2d_file_name = "file";
    EM2DEnsembleScore *em2d_ensemble_score = new EM2DEnsembleScore(em2d_file_name);
    scorers.push_back(em2d_ensemble_score);

    if(matrix_file_name.length() > 0) {
      std::ofstream out(matrix_file_name.c_str());
      saxs_ensemble_score->output_matrix(out, saxs_ensemble_score->get_average_c1(), saxs_ensemble_score->get_average_c2());
    }

    if(nnls) {
      IMP::saxs::WeightedFitParameters fp = saxs_ensemble_score->get_fit_parameters();
      std::ofstream s("nnls.txt");
      s.setf(std::ios::fixed, std::ios::floatfield);
      s << std::setw(6) << std::setprecision(3) << " Chi = " << fp.get_chi()
        << " c1 = " << fp.get_c1() << " c2 = " << fp.get_c2() << std::endl;
      for(unsigned int i=0; i<clustered_profiles.size(); i++) {
        s << std::setw(4) << i << " " << clustered_profiles[i]->get_name();
        s << std::setw(5) << std::setprecision(3) << "(" << fp.get_weights()[i] << ") ";
        s << std::endl;
      }
      s.close();
    }

    EnsembleGenerator eg(clustered_profiles.size(), best_k, scorers);
    eg.generate(ensemble_size);
  }

  return 0;
}
