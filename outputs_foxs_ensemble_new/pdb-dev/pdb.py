import Bio.PDB
import ihm.model

def get_sequence():
    """Get the primary sequence"""
    p = Bio.PDB.PDBParser()
    # Use one of the final models (should all have the same sequence)
    s = p.get_structure('rep', '../MES_results/Int325_6_B10770001_MSE.pdb')
    ppb = Bio.PDB.PPBuilder()
    pp, = ppb.build_peptides(s)
    return str(pp.get_sequence())

class Model(ihm.model.Model):
    """Pass a BioPython model through to IHM"""
    def __init__(self, file_name, asym_units, **kwargs):
        super(Model, self).__init__(**kwargs)
        self.file_name = file_name
        self.asym_units = asym_units

    def get_atoms(self):
        p = Bio.PDB.PDBParser()
        s = p.get_structure('rep', self.file_name)
        for model in s:
            for nchain, chain in enumerate(model):
                asym = self.asym_units[nchain]
                for nres, residue in enumerate(chain):
                    for atom in residue:
                        coord = atom.get_vector()
                        yield ihm.model.Atom(asym_unit=asym, seq_id=nres+1,
                                atom_id=atom.get_id(), x=coord[0], y=coord[1],
                                z=coord[2])
