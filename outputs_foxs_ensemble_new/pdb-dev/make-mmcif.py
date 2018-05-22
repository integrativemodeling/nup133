#!/usr/bin/env python

"""Collect data on the Nup133 system and output an mmCIF file suitable for
   deposition at PDB-Dev, https://pdb-dev.wwpdb.org/. This uses the python-ihm
   library, available at https://github.com/ihmwg/python-ihm.
"""

import ihm.dumper
import ihm.representation
import ihm.model
import ihm.protocol
import ihm.analysis
import os
import sys
import pdb
import mes
import saxs
import em2d
import xlink
import compmodel

sys.path.append('../../util/')
import make_archive

title = ("Integrative structure-function mapping of the nucleoporin "
         "Nup133 suggests a conserved mechanism for membrane anchoring "
         "of the nuclear pore complex.")
system = ihm.System(title=title)

system.citations.append(ihm.Citation(
          pmid='25139911', title=title,
          journal="Mol Cell Proteomics", volume=13, page_range=(2911,2926),
          year=2014,
          authors=['Kim SJ', 'Fernandez-Martinez J', 'Sampathkumar P',
                   'Martel A', 'Matsui T', 'Tsuruta H', 'Weiss TM', 'Shi Y',
                   'Markina-Inarrairaegui A', 'Bonanno JB', 'Sauder JM',
                   'Burley SK', 'Chait BT', 'Almo SC', 'Rout MP', 'Sali A'],
          doi='10.1074/mcp.M114.040915'))

# We used HHpred to detect remote homologs for some input subunits
system.software.append(ihm.Software(
          name='HHpred', classification='protein homology detection',
          description='Protein homology detection by HMM-HMM comparison',
          version='2.0.16',
          location='https://toolkit.tuebingen.mpg.de/hhpred'))
# We used PSIPRED to predict secondary structure for subunits
system.software.append(ihm.Software(
          name='PSIPRED', classification='secondary structure prediction',
          description='Protein secondary structure prediction based on '
                      'position-specific scoring matrices',
          version='4.0',
          location='http://bioinf.cs.ucl.ac.uk/psipred/'))
# We used DISOPRED to predict (and remove) disordered regions in
# the subunits
system.software.append(ihm.Software(
          name='DISOPRED', classification='disorder prediction',
          description='prediction of protein disorder', version=3,
          location='http://bioinf.cs.ucl.ac.uk/psipred/?disopred=1'))
# We used various tools from IMP (e.g. FoXS)
system.software.append(ihm.Software(
          name="Integrative Modeling Platform (IMP)",
          version="2.2",
          classification="integrative model building",
          description="integrative model building",
          location='https://integrativemodeling.org'))
# We used AllosMod for sampling
allosmod = ihm.Software(
          name='AllosMod', classification='sampling',
          description='modeling on a custom energy landscape',
          location='https://salilab.org/allosmod')

# System is a single chain - extract its sequence from one of the output PDBs
entity = ihm.Entity(pdb.get_sequence(), description='Nup133')
system.entities.append(entity)
# Note that our published model is numbered from 0, i.e. offset by -1
# from the internal 1-based mmCIF numbering
asym = ihm.AsymUnit(entity, details='Nup133', auth_seq_id_map=-1)
system.asym_units.append(asym)

assembly = ihm.Assembly([asym], name='Modeled assembly')

# Atomic model, starting from a Modeller comparative model
modeller_model = compmodel.get_starting_modeller_model(asym)
rep = ihm.representation.Representation(
        [ihm.representation.AtomicSegment(asym, rigid=False,
                                          starting_model=modeller_model)])

# Read in experimental datasets
sf = saxs.SAXSFits(asym)
saxs_restraints = list(sf.add_from_csv())
system.restraints.extend(saxs_restraints)

em2d_fits = em2d.EM2DFits(assembly)
em2d_restraints = list(em2d_fits.add_all())
system.restraints.extend(em2d_restraints)

xlink_fits = xlink.CrossLinkFits(asym)
xlink_restraints = list(xlink_fits.add_restraints())
system.restraints.extend(xlink_restraints)

saxs_em2d_datasets = ihm.dataset.DatasetGroup(r.dataset
                                for r in saxs_restraints + em2d_restraints)
xlink_datasets = ihm.dataset.DatasetGroup(r.dataset for r in xlink_restraints)

# Describe the modeling protocol
protocol = ihm.protocol.Protocol(name='Modeling')
# Taking the original comparative model as input, first we ran AllosMod,
# which didn't use the experimental datasets:
protocol.steps.append(ihm.protocol.Step(
                 assembly=assembly, dataset_group=None,
                 method='AllosMod',
                 name='MD-based conformational sampling',
                 num_models_begin=1, num_models_end=7000,
                 software=allosmod))
# Next we ran MES using both saxs and em2d:
protocol.steps.append(ihm.protocol.Step(
                 assembly=assembly, dataset_group=saxs_em2d_datasets,
                 method='MES',
                 name='Minimal Ensemble Search',
                 num_models_begin=7000, num_models_end=4, multi_state=True))
# Finally we validated against crosslinks
analysis = ihm.analysis.Analysis()
analysis.steps.append(ihm.analysis.ValidationStep(
                   assembly=assembly, dataset_group=xlink_datasets,
                   feature='other', num_models_begin=4, num_models_end=4))
protocol.analyses.append(analysis)

# Output the final models selected by MES. Each is represented as a state
# with given population fraction
g = ihm.model.StateGroup()
for nmodel, model, fraction in mes.get_models_with_fractions():
    m = pdb.Model(assembly=assembly, protocol=protocol, representation=rep,
                  file_name=model, asym_units=[asym])
    em2d_fits.add_model(nmodel, m, em2d_restraints)
    s = ihm.model.State([ihm.model.ModelGroup([m])],
                        type='minimal ensemble',
                        name='Minimal ensemble conformation',
                        experiment_type='Fraction of bulk',
                        population_fraction=fraction)
    g.append(s)
system.state_groups.append(g)

# Replace local paths with pointers to deposited files at Zenodo
repos = []
for subdir, zipname in make_archive.ARCHIVES.items():
    repos.append(ihm.location.Repository(
          doi="10.5281/zenodo.1209565", root="../../%s" % subdir,
          url="https://zenodo.org/record/1209565/files/%s.zip" % zipname,
          top_directory=os.path.basename(subdir)))
repos.append(ihm.location.Repository(
             doi="10.5281/zenodo.1209565", root="../..",
             url="https://zenodo.org/record/1209565/files/nup133-master.zip",
             top_directory="nup133-master"))
system.update_locations_in_repositories(repos)

# Write out an mmCIF file
with open('nup133.cif', 'w') as fh:
    ihm.dumper.write(fh, [system])
