from modeller import *
from modeller.automodel import *

log.verbose()
env = environ()
env.io.atom_files_directory = ['../23902', '../23904C']

class MyModel(automodel):
    def special_patches(self, aln):
        self.rename_segments('A', 0)

a = MyModel(env, 
            alnfile='all_align_final_23904.ali',
            knowns=('23902a', '23904C'),
            sequence='23904',
            assess_methods=(assess.DOPE, assess.GA341))
a.starting_model = 1
a.ending_model = 400

a.make()
