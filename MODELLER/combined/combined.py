from modeller import *
from modeller.automodel import *

log.verbose()
env = environ()
env.io.atom_files_directory = ['../templates']

class MyModel(automodel):
    def special_patches(self, aln):
        self.rename_segments('A', 0)

a = MyModel(env, 
            alnfile='combined.ali',
            knowns=('3i4rB', '3kfoA', '4q9tB', '4q9tA', '2jo8A', '2qiwA',
                    '3cigA', '2eloA', '3guzA', '2ciwA', '1a92A', '1gdjA',
                    '1x4oA'),
            sequence='combined',
            assess_methods=(assess.DOPE, assess.GA341))
a.starting_model = 1
a.ending_model = 1

a.make()
