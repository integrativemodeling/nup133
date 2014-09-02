# Nup133

These scripts demonstrate the use of [IMP](http://salilab.org/imp),
[MODELLER](http://salilab.org/modeller),
[FoXS](http://salilab.org/foxs), and
[Minimal Ensemble Search](http://bl1231.als.lbl.gov/saxs_protocols/mes.php) in the modeling of the
Nup133 in the Nuclear Pore Complex (NPC). First, MODELLER is used to generate
structures for the individual components in the Nup133. Then, IMP
is used to fit these components together into the electron microscopy density
map of Nup133.

A full description of the scripts can be found in
[Integrative structure-function mapping of the nucleoporin Nup133 suggests a conserved mechanism for membrane anchoring of the nuclear pore complex](http://mcponline.org/content/early/2014/08/19/mcp.M114.040915).

## Steps (UNDER CONSTRUCTION)

First, make a directory for the output by running `mkdir output`. Output
files that were generated when these scripts were run for the first time are
also provided, in the `precalculate_results` directory. Then, the scripts can
be run in order:

1. Template identification:
    `scripts/script1_build_profile.py`
2. Template(s) selection by sequence:
    `scripts/script2_compare_templates.py`
3. Density map segmentation:
    `scripts/script3_density_segmentation.py`
4. Template selection by fitting to a density map:
    `scripts/script4_score_templates_by_cc.py`
5. Template alignment:
    `scripts/script5_template_alignment.py`
6. Model building and assessment:
    `scripts/script6_model_building_and_assessment.py` and
    `scripts/script7_pairwise_rmsd.py`
7. Multiple fitting into a density map:
    `scripts/script8_split_density.py` and
    `scripts/script9_symmetric_multiple_fitting.py`

## Information

_Author(s)_: Seung Joong Kim

_Date_: September 2nd, 2014

_License_: [LGPL](http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html).
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

_Last known good IMP version_: [![build info](https://salilab.org/imp/systems/?sysstat=1)](http://salilab.org/imp/systems/)

_Testable_: Yes.

_Parallelizeable_: No

_Publications_:
 - Seung Joong Kim*, Javier Fernandez-Martinez*, Parthasarathy Sampathkumar*, Anne Martel, Tsutomu Matsui, Hiro Tsuruta, Thomas Weiss, Yi Shi, Ane Markina-Inarrairaegui, Jeffery B. Bonanno, J. Michael Sauder, Stephen K. Burley, Brian T. Chait, Steven C. Almo, Michael P. Rout, and Andrej Sali, [Integrative structure-function mapping of the nucleoporin Nup133 suggests a conserved mechanism for membrane anchoring of the nuclear pore complex](http://mcponline.org/content/early/2014/08/19/mcp.M114.040915), Molecular & Cellular Proteomics, 2014, mcp.M114.040915.
# Nup84 complex
Nup84 modeling script using DSS/EDC crosslinks and 2D EM data.
Authors: Riccardo Pellarin, Elina Tjioe, and Seung Joong Kim

The script works with the latest IMP, PMI and isd_emxl libraries.
Compile IMP using mpi (needed for the replica exchange, use "module load mpi/openmpi-x86_64-nodlopen" on the cluster)

## List of files and directories:

data		                         contains all relevant data, input structure, etc.

nup84.isd.modeling.withXrayInterface.py  the main modeling script with 3 crystal interfaces
nup84.isd.modeling.py                    the main modeling script with no crystal interfaces

nup84.topology.withXrayInterface.py      constructs Nup84 subunits with 3 crystal interfaces, as well as calculates the densities for the EM restraints
nup84.topology.py                        constructs Nup84 subunits with no crystal interfaces, as well as calculates the densities for the EM restraints

output.1/pdbs    the production will write the best scoring models into pdb files they are initialized and then updated as long as the calculation goes
                 (They are the best 500 models, so at the beginning they are empty, since you haven't start the calculation yet)

output.1/rmfs    the production will write the rmf3 files for lowest temperature replica.
			
stat.n.out	 log files. They contain all relevant numbers of the calculation.

Python dependencies:
biopython 		(to read fasta files)
sklearn   		(for the gaussian mixture model decomposition of the EM map)

## Biopython and sklearn are required to run nup84 script:
------------------------------------------------------------
sudo yum install python-biopython
sudo yum install scikit-learn

## Getting IMP, PMI, ISD_EMXL and compiling it with fast-build on Fedora:
-----------------------------------------------------------------------
git clone https://github.com/salilab/imp.git imp-latest
cd imp-latest/modules
git clone https://github.com/salilab/isd_emxl.git
git clone https://github.com/salilab/pmi.git
cd ../
module load mpi
./setup_git.py
cd ..
mkdir imp-latest-build
cd imp-latest-build/
cmake ../imp-latest -DCMAKE_CXX_COMPILER=mpicxx -DCMAKE_BUILD_TYPE=Release -DIMP_MAX_CHECKS=NONE -DIMP_MAX_LOG=SILENT
make

## Running nup84 script:
---------------------------
with 3 crytal interfaces:
imp-latest-build/setup_environment.sh /path_to/mpirun -np 4 python nup84.isd.modeling.withXrayInterface.py &> nup84.isd.modeling.withXrayInterface.out

with no crytal interfaces:
imp-latest-build/setup_environment.sh /path_to/mpirun -np 4 python nup84.isd.modeling.py &> nup84.isd.modeling.out

## Information

_Author(s)_: Seung Joong Kim, Riccardo Pellarin, Peter Cimermancic

_Date_: September 2nd, 2014

_License_: [LGPL](http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html).
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

_Last known good IMP version_: [![build info](https://salilab.org/imp/systems/?sysstat=1)](http://salilab.org/imp/systems/)

_Testable_: Yes.

_Parallelizeable_: No

_Publications_:
 - Yi Shi*, Javier Fernandez-Martinez*, Elina Tjioe*, Riccardo Pellarin*, Seung Joong Kim*, Rosemary Williams, Dina Schneidman-Duhovny, Andrej Sali, Michael P. Rout, and Brian T. Chait1, [Structural characterization by cross-linking reveals the detailed architecture of a coatomer-related heptameric module from the nuclear pore complex](http://mcponline.org/content/early/2014/08/26/mcp.M114.041673), Molecular & Cellular Proteomics, 2014, mcp.M114.041673.

 *These authors contributed equally to this work as co-first authors.
