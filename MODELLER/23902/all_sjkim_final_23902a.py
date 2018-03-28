from modeller import *
from modeller.automodel import *

log.verbose()
env = environ()
env.io.atom_files_directory = ['../templates']

class MyModel(automodel):
    def special_patches(self, aln):
        self.rename_segments('A', 0)

a = MyModel(env, 
            alnfile='all_align_final_23902a.ali',
            knowns=('15133B', '15133A', '2jo8A', '2qiwA', '3cigA', '2eloA',
                    '3guzA', '3i4rB'),
            sequence='23902a',
            assess_methods=(assess.DOPE, assess.GA341))
a.starting_model = 1
a.ending_model = 200

a.make()
