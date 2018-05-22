"""Add the comparative model used as a starting guess for the integrative
   modeling."""

import ihm.startmodel
import ihm.metadata
import ihm.model
import Bio.PDB

pdb_file = '../../MODELLER/combined/23904.B99990030.pdb'
offset = 1  # numbering starts at 0, but ours does at 1
chain = 'A'

class StartingModel(ihm.startmodel.StartingModel):
    # Override StartingModel so we can provide coordinates

    def __init__(self, file_name, **kwargs):
        super(StartingModel, self).__init__(**kwargs)
        self.file_name = file_name

    def get_atoms(self):
        # Read atoms from our starting model using BioPython, and yield
        # ihm.model.Atom objects
        p = Bio.PDB.PDBParser()
        s = p.get_structure('rep', self.file_name)
        for model in s:
            for nchain, chain in enumerate(model):
                assert(nchain == 0) # should only be one chain
                for nres, residue in enumerate(chain):
                    for atom in residue:
                        coord = atom.get_vector()
                        yield ihm.model.Atom(asym_unit=self.asym_unit,
                                seq_id=nres+1,
                                atom_id=atom.get_id(), x=coord[0], y=coord[1],
                                z=coord[2], type_symbol=atom.element,
                                biso=atom.get_bfactor())


def get_starting_modeller_model(asym):
    """Read the MODELLER model that we used as a starting guess for
       AllosMod & MES, and return it as an ihm.startmodel object."""
    p = ihm.metadata.PDBParser()
    m = p.parse_file(pdb_file)
    return StartingModel(file_name=pdb_file, asym_unit=asym,
                         dataset=m['dataset'], asym_id=chain,
                         templates=m['templates'][chain], offset=offset,
                         software=m['software'][0])
