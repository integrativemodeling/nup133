import ihm.startmodel
import ihm.metadata

pdb_file = '../../MODELLER/23904/23904.B99990030.pdb'
offset = 1  # numbering starts at 0, but ours does at 1
chain = 'A'

class StartingModel(ihm.startmodel.StartingModel):
    pass

def get_starting_modeller_model(asym):
    """Read the MODELLER model that we used as a starting guess for
       AllosMod & MES, and return it as an ihm.startmodel object."""
    p = ihm.metadata.PDBParser()
    m = p.parse_file(pdb_file)
    return StartingModel(asym, dataset=m['dataset'], asym_id=chain,
                         templates=m['templates'], offset=offset)
