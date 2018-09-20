#!/usr/bin/env python

import unittest
import os
import sys
import subprocess
import glob
import ihm.reader

TOPDIR = os.path.abspath(os.path.join(os.path.dirname(sys.argv[0]), '..'))

class Tests(unittest.TestCase):
    def run_modeller_script(self, script_dir, script_name, model_name, resrng):
        """Run a Modeller script and test the output model"""
        os.chdir(os.path.join(TOPDIR, 'MODELLER', script_dir))
        # Run script
        p = subprocess.check_call(["python", script_name, "--test"])
        # Make sure PDB was produced with the requested residue range
        with open('%s.B99990001.pdb' % model_name) as fh:
            pdb_lines = [x for x in fh.readlines() if x.startswith('ATOM')]
        rng = (int(pdb_lines[0][22:26]), int(pdb_lines[-1][22:26]))
        self.assertEqual(rng, resrng)

    def test_combined(self):
        """Test generation of combined starting model"""
        self.run_modeller_script('combined', 'combined.py',
                                 'combined', (0, 1165))

    def test_23917(self):
        """Test generation of comparative model for construct 23917"""
        self.run_modeller_script('23917', 'all_sjkim_final.py',
                                 '23917', (1, 664))

    def test_23902(self):
        """Test generation of comparative model for construct 23902"""
        self.run_modeller_script('23902', 'all_sjkim_final_23902a.py',
                                 '23902a', (0, 523))

    def test_23904C(self):
        """Test generation of comparative model for construct 23904C"""
        self.run_modeller_script('23904C', 'all_sjkim_final_23904.py',
                                 '23904', (1, 664))

    def test_23904(self):
        """Test generation of comparative model for construct 23904"""
        self.run_modeller_script('23904', 'all_sjkim_final_23904.py',
                                 '23904', (0, 1165))

    def test_mmcif(self):
        """Test generation of mmCIF output"""
        os.chdir(os.path.join(TOPDIR, 'outputs_foxs_ensemble_new', 'pdb-dev'))
        if os.path.exists("nup133.cif"):
            os.unlink("nup133.cif")
        p = subprocess.check_call(["./make-mmcif.py"])
        # Check output files
        with open('nup133.cif') as fh:
            s, = ihm.reader.read(fh)
        self._check_mmcif_file(s)
        with open('nup133.bcif', 'rb') as fh:
            s, = ihm.reader.read(fh, format="BCIF")
        self._check_mmcif_file(s)

    def _check_mmcif_file(self, s):
        self.assertEqual(len(s.citations), 1)
        self.assertEqual(s.citations[0].doi, '10.1074/mcp.M114.040915')
        self.assertEqual(len(s.software), 6)
        self.assertEqual(len(s.orphan_starting_models), 1)
        # Should be 4 states, each containing a single model
        self.assertEqual(len(s.state_groups), 1)
        self.assertEqual(len(s.state_groups[0]), 4)
        models = []
        for state in s.state_groups[0]:
            for mg in state:
                models.extend(mg)
        # Check number of atoms and spheres
        self.assertEqual([len(m._atoms) for m in models], [9482]*4)
        self.assertEqual([len(m._spheres) for m in models], [0]*4)
        # Check entities and chains
        self.assertEqual([len(e.sequence) for e in s.entities], [1166])
        self.assertEqual([a.details for a in s.asym_units], ['Nup133'])
        # PDB numbering should start at 0 (not 1)
        self.assertEqual(s.asym_units[0].residue(1).seq_id, 1)
        self.assertEqual(s.asym_units[0].residue(1).auth_seq_id, 0)
        # 25 restraints - 2 XL datasets, 23 EM2D images
        self.assertEqual(len(s.restraints), 25)
        xl1, xl2 = s.restraints[:2]
        self.assertEqual(xl1.linker_type, 'DSS')
        self.assertEqual(len(xl1.experimental_cross_links), 18)
        self.assertEqual(len(xl1.cross_links), 18)
        self.assertEqual(xl1.dataset.location.path,
                         'Crosslinks/DSS_EDC_crosslinks.txt')
        # No fits reported
        self.assertEqual(sum(len(x.fits) for x in xl1.cross_links), 0)

        self.assertEqual(xl2.linker_type, 'EDC')

        em2d_rsr = s.restraints[3:]
        for i, em2d in enumerate(em2d_rsr):
            self.assertAlmostEqual(em2d.image_resolution, 15.0, places=1)


if __name__ == '__main__':
    unittest.main()
