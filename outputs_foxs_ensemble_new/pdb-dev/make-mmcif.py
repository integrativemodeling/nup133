#!/usr/bin/env python

import ihm.dumper
import ihm.representation
import ihm.model
import pdb
import mes
import saxs
import em2d

system = ihm.System()

system.citations.append(ihm.Citation(
          pmid='25139911',
          title="Integrative structure-function mapping of the nucleoporin "
                "Nup133 suggests a conserved mechanism for membrane anchoring "
                "of the nuclear pore complex.",
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

entity = ihm.Entity(pdb.get_sequence(), description='Nup133')
system.entities.append(entity)
asym = ihm.AsymUnit(entity, details='Nup133')
system.asym_units.append(asym)

assembly = ihm.Assembly([asym], name='Modeled assembly')
rep = ihm.representation.Representation(
        [ihm.representation.AtomicSegment(asym, rigid=False)])

sf = saxs.SAXSFits(asym)
saxs_restraints = list(sf.add_from_csv())
system.restraints.extend(saxs_restraints)

em2d_fits = em2d.EM2DFits(assembly)
em2d_restraints = list(em2d_fits.add_all())
system.restraints.extend(em2d_restraints)

g = ihm.model.StateGroup()
for model, fraction in mes.get_models_with_fractions():
    m = pdb.Model(assembly=assembly, protocol=None, representation=rep,
                  file_name=model, asym_units=[asym])
    em2d_fits.add_model(m, em2d_restraints)
    s = ihm.model.State([ihm.model.ModelGroup([m])],
                        population_fraction=fraction)
    g.append(s)
system.state_groups.append(g)

with open('nup133.cif', 'w') as fh:
    ihm.dumper.write(fh, [system])
