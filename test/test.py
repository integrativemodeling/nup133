#!/usr/bin/env python

import unittest
import os
import sys
import subprocess
import glob

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

    def test_mmcif(self):
        """Test generation of mmCIF output"""
        os.chdir(os.path.join(TOPDIR, 'outputs_foxs_ensemble_new', 'pdb-dev'))
        if os.path.exists("nup133.cif"):
            os.unlink("nup133.cif")
        p = subprocess.check_call(["./make-mmcif.py"])
        # Check size of output file
        with open("nup133.cif") as fh:
            wcl = len(fh.readlines())
        self.assertEqual(wcl, 50803)

if __name__ == '__main__':
    unittest.main()
