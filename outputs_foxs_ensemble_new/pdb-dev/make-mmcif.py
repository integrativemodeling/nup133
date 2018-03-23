#!/usr/bin/env python

import ihm.dumper
import ihm.representation
import ihm.model
import pdb
import mes
import saxs

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

g = ihm.model.StateGroup()
for model, fraction in mes.get_models_with_fractions():
    m = pdb.Model(assembly=assembly, protocol=None, representation=rep,
                  file_name=model, asym_units=[asym])
    s = ihm.model.State([ihm.model.ModelGroup([m])],
                        population_fraction=fraction)
    g.append(s)
system.state_groups.append(g)

with open('nup133.cif', 'w') as fh:
    ihm.dumper.write(fh, [system])
